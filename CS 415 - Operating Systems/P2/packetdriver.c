/* Elias Charles - eliasc@uoregon.edu */
/* CIS 415 Project 2: This is my own work. Discussed project verbally with Nathan Eng & Evan Kender*/
/* I did not exchange code with classmates */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include "queue.h"
#include "networkdevice.h"
#include "packetdriver.h"
#include "packetdescriptor.h"
#include "freepacketdescriptorstore__full.h"
#include "packetdescriptorcreator.h"
#include "BoundedBuffer.h"
#include "diagnostics.h"

#define UNUSED(x) (void)(x)
#define INBUF 10
#define POOLBUF 4
#define ARRAYBUF 2
#define APPBUF 11
#define ATTEMPTS 10

BoundedBuffer *bb[APPBUF];
BoundedBuffer *in;
BoundedBuffer *pool;
FreePacketDescriptorStore *fpds;
NetworkDevice *mynd;


static void *receiver(){
    /* ~~~~~~~~~~~
     * receiver():
     * ~~~~~~~~~~~
     * Awaits packet from network device;
     * attempts to return to application process via app buffer
     */
    PacketDescriptor *pd, *npd;
    PID pid;
    while(1){
        /* grab packet from pool buffer */
        pool->blockingRead(pool, (void **)&pd);
        /* initialize and register packet */
        initPD(pd);
        mynd->registerPD(mynd, pd);
        /* await packet from network device */
        mynd->awaitIncomingPacket(mynd);
        /* grab new packet from store to later insert into pool*/
        fpds->blockingGet(fpds, &npd);
        npd = pd;
        /* store in application buffer */
        pid = getPID(pd);
        bb[pid]->nonblockingWrite(bb[pid], (void *)npd);
        /* write old packet back to pool */
        pool->blockingWrite(pool, (void *)pd);
    }
    return (NULL);
}

static void *sender(){
    /* ~~~~~~~~~
     * sender():
     * ~~~~~~~~~
     * Handles packets from application thread; attempts to send to network device
     */
    int i;
    PacketDescriptor *pd;
    while (1){
        /* grab packet from in buffer */
        in->blockingRead(in, (void **)&pd); 
        /* attempt to send packet to network device */
        for (i = 0; i < ATTEMPTS; i++){
            if (mynd->sendPacket(mynd, pd))
                break;
        }
        /* return packet to packet store */
        if (!fpds->nonblockingPut(fpds, pd))
            fpds->blockingPut(fpds, pd);
    }
    return (NULL);
}

void init_packet_driver(NetworkDevice               *nd, 
                        void                        *mem_start, 
                        unsigned long               mem_length,
                        FreePacketDescriptorStore **fpds_ptr)
{   
    int i;
    pthread_t t1, t2;
    PacketDescriptor *pd;

    /* create Free Packet Descriptor Store using mem_start and mem_length */
    fpds = FreePacketDescriptorStore_create(mem_start, mem_length);

    /* return the FPDS to the code that called you */
    *fpds_ptr = fpds;
    mynd = nd;

    /* create any buffers required by your thread[s] */
    in = BoundedBuffer_create(INBUF);
    pool = BoundedBuffer_create(POOLBUF);
    for (i = 0; i < APPBUF; i++){
        bb[i] = BoundedBuffer_create(ARRAYBUF);
    }
    for (i = 0; i < POOLBUF; i++){
        fpds->blockingGet(fpds, &pd);
        pool->blockingWrite(pool, pd);
    }
    /* create any threads you require for your implementation */
    assert(pthread_create(&t1, NULL, &receiver, NULL) == 0);
    assert(pthread_create(&t2, NULL, &sender, NULL) == 0);
}

void blocking_send_packet(PacketDescriptor *pd){
    /* queue up packet descriptor for sending */
    /* do not return until it has been successfully queued */
    in->blockingWrite(in, (void *)pd);
}

int nonblocking_send_packet(PacketDescriptor *pd){
    /* if you are able to queue up packet descriptor immediately, do so and return 1 */
    /* otherwise, return 0 */
    return in->nonblockingWrite(in, (void *)pd);
}

void blocking_get_packet(PacketDescriptor **pd, PID pid){
    /* wait until there is a packet for `pid’ */
    /* return that packet descriptor to the calling application */
    bb[pid]->blockingRead(bb[pid], (void **)pd);
}

int nonblocking_get_packet(PacketDescriptor **pd, PID pid){
    /* if there is currently a waiting packet for `pid’, return that packet */
    /* to the calling application and return 1 for the value of the function */
    /* otherwise, return 0 for the value of the function */
    return bb[pid]->nonblockingRead(bb[pid], (void **)pd);
}
