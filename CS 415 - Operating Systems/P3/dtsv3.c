/* 
 * Name: Elias Charles
 * DuckID: eliasc@uoregon.edu
 * 
 * This is my own work.
 * 
 * CIS 415 Project 3
 */

#include "BXP/bxp.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include <valgrind/valgrind.h>
#include "prioqueue.h"
#include "queue.h"
#include "cskmap.h"
#define UNUSED __attribute__((unused))
#define INTERVAL 1000000

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

typedef struct event {
	char *svid;
	bool cancelled;
	char **data;
	int repeats;
} Event;

unsigned long normalize(struct timeval *t){
	return (t->tv_sec*1000L + t->tv_usec/1000L);
}

BXPEndpoint ep;
BXPService svc;
struct timeval now, later;
const Queue *q = NULL;
const PrioQueue *pq = NULL;
const CSKMap *map = NULL;

int cmp(void *x, void *y){
	long x1 = *((long *)x);
	long y1 = *((long *)y);
	return x1 < y1;
}

int extractWords(char *buf, char *sep, char *words[]) {
    int i;
    char *p;
    for (p = strtok(buf, sep), i = 0; p != NULL; p = strtok(NULL, sep),i++)
        words[i] = p;
    words[i] = NULL;
    return i;
}



void createEvent(int svid, char **data){
	/* 			 	
	 *				createEvent():
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Allocates Event structure onto heap
	 * inserts into map by svid
	 * inserts into prioQ by time since epoch
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	int i;
	char key[9];
	sprintf(key, "%d", svid);
	Event *e = (Event *)malloc(sizeof(Event));
	e->svid = strdup(key);
	e->cancelled = false;
	e->data = (char **)malloc(sizeof(char *) * 7);
	for (i = 0; i < 7; i++){
		e->data[i] = strdup(data[i]);
	}

	map->putUnique(map, key, (void *)e);
	struct timeval epoch;
	gettimeofday(&epoch, NULL);

	if ((strcmp(data[0], "Repeat")) == 0){
		// Calculate priority (current time + msecs)
		int msecs, reps;
		sscanf(data[2], "%d", &msecs);
		//epoch.tv_usec += msecs % 1000;  //  milliseconds to microseconds
		//epoch.tv_usec = epoch.tv_usec % 1000000; // normalize
		useconds_t prio = epoch.tv_usec + (msecs % 1000) * 1000;
		sscanf(data[3], "%d", &reps);
		e->repeats = reps;
		pthread_mutex_lock(&m1);
		pq->insert(pq, (void *)&prio, (void *)e);
		pthread_mutex_unlock(&m1);
	}
	else{
		useconds_t prio;
		long secs, usecs;
		sscanf(data[2], "%ld", &secs);
		sscanf(data[3], "%ld", &usecs);
		epoch.tv_sec += secs - epoch.tv_sec;
		epoch.tv_usec += usecs - epoch.tvusec;

		prio = (onesh.tv_sec + onesh.tv_usec) % 1000000;
		printf("String secs: %d\n", prio);
		//gettimeofday(&epoch, NULL);
		pthread_mutex_lock(&m1);
		printf("hello\n");
		//normalize
		//printf("current time: %ld - time until fire: %d", epoch.tv_usec, prio);
		int ret = pq->insert(pq, (void *)&test, (void *)e);
		printf("%d\n", ret);
		pq->removeMin(pq, (void **)&val, (void **)&event);
		printf("%ld\n", *val);
		pthread_mutex_unlock(&m1);
	}
}

void freeEvent(void *e){
	int i;
	Event *tmp = (Event *)e;
	for (i = 0; i < 7; i++){
		free(tmp->data[i]);
	}
	free(tmp->data);
	free(tmp->svid);
	free(tmp);
}
 
void *receiver(){
	/* 
	 * 		 receiver():
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~
	 *  Validates requests
	 *  Registers event objects
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	char query[10000], response[10001];
	unsigned qlen, rlen;
	int N;
	unsigned long svid = 0;
	char *w[25];
	while ((qlen = bxp_query(svc, &ep, query, 10000)) > 0) {
     	N = extractWords(query, "|", w);

		if ((strcmp(w[0], "OneShot") == 0) || (strcmp(w[0], "Repeat") == 0)){
			if (N != 7){
				sprintf(response, "0");
			}
			else{
				createEvent(svid, w);
				sprintf(response, "1%08lu", svid);
			}
		} 
		else if (strcmp(w[0], "Cancel") == 0) {
			if (N != 2){
				sprintf(response, "0");
			}
			else{
				void *e;
				int ret = map->get(map, w[1], &e);
				if (ret)
					((Event *)e)->cancelled = true;
				sprintf(response, "1%08lu", svid);
			}
		} 
		else {
			sprintf(response, "0");
		}

		rlen = strlen(response) + 1;
		assert(bxp_response(svc, &ep, response, rlen));
		VALGRIND_MONITOR_COMMAND("leak_check summary");
		svid++;
	}
	return (NULL);
}

void *eventHandler(){
	/* 
	 * 				eventHandler():
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * 	Processes events stored in execution queue
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	void *event;
	unsigned long clid;
	unsigned port;
	int msecs;	
	while(1){
		while(!q->isEmpty(q)){
			pthread_mutex_lock(&m2);
			q->dequeue(q, &event);
			pthread_mutex_unlock(&m2);
			/* Extract data from event struct */
			Event *e = (Event *)event;
	  		sscanf(e->data[1], "%lu", &clid);
	  		sscanf(e->data[6], "%u", &port);
	  		
	  		if ((strcmp(e->data[0], "Repeat")) == 0){
	  			bool insert = false;
	  			printf("Event fired: %lu|%s|%s|%u\n", clid, e->data[4], e->data[5], port);
	  			if (e->repeats == 0){	// Check repeat counter
	  				insert = true;
	  			}
	  			else{
	  				--e->repeats;
	  				insert = true;
	  				if (e->repeats == 0){
	  					/* Free the Repeat request if it has expired */
	  					freeEvent(event);
	  					insert = false;
	  					VALGRIND_MONITOR_COMMAND("leak_check summary");
	  				}
	  			}
	  			if (insert){
	  				/* Update timeval and reinsert into prioqueue */
	  				sscanf(e->data[2], "%d", &msecs);
	  				struct timeval priority;
		  			gettimeofday(&priority, NULL);
		  			priority.tv_usec += msecs * 1000;  //  milliseconds to microseconds
					priority.tv_sec += priority.tv_usec / 1000000;  // normalize
					priority.tv_usec = priority.tv_usec % 1000000; // normalize
		  			pthread_mutex_lock(&m2);
	  				pq->insert(pq, (void *)&priority.tv_usec, event);
	  				pthread_mutex_unlock(&m2);
	  			}
	  		} else{	/* OneShot Request */
	  			printf("Event fired: %lu|%s|%s|%u\n", clid, e->data[4], e->data[5], port);
	  			freeEvent(event);
	  			VALGRIND_MONITOR_COMMAND("leak_check summary");
	  		}
		}
	}
}


void *timer(){
	/* 
	 * 				   timer():
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * 	Harvests events inserted into prioqueue
     *  before the previous timer fire.
     * 
     *  Harvested events are stored in the
     *  execution queue.
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	useconds_t musecs = INTERVAL;
	void *event;
	useconds_t *prio;
	
	while(1) {
        usleep(musecs);                     // this thread sleeps
        gettimeofday(&now, NULL);           // determine current time of day
        //printf("now: secs %ld\n", now.tv_sec);

        while(!pq->isEmpty(pq)){
        	printf("wtf\n");
        	pthread_mutex_lock(&m1);
        	printf("wtf\n");
      		pq->min(pq, (void **)&prio, &event);
      		if (*prio > now.tv_usec){
      			printf("%d - %ld", *prio, now.tv_usec);
      			pthread_mutex_unlock(&m1);
      			break;
      		}
      		else{
      			//printf("REMOVING -> NOW: %ld, PRIO: %d\n", now.tv_usec, *prio);
      			pq->removeMin(pq, (void **)&prio, &event);
      			pthread_mutex_unlock(&m1);
      			if ((((Event *)event)->cancelled)){
      				freeEvent(event);
      				break;
      			}
  				pthread_mutex_lock(&m2);
  				if (event)
      				q->enqueue(q, event);
      			pthread_mutex_unlock(&m2);
      		}
        }
        gettimeofday(&later, NULL);         // determine current time of day
        musecs = now.tv_usec + INTERVAL - later.tv_usec;    // microsecs to sleep
    }
}

int main(UNUSED int argc, UNUSED char *argv[]) {
	/* 
	 * 				   main():
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * -Initialize Data structures
     * -Initialize DTS
     * -Initialize worker threads
	 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	if (!(q = Queue_create(&freeEvent))){
		q->destroy(q);
		exit(1);
	}
	if (!(pq = PrioQueue_create(&cmp, NULL, &freeEvent))){
		pq->destroy(pq);
		exit(1);
	}
	if (!(map = CSKMap_create(&freeEvent))){
		map->destroy(map);
		exit(1);
	}
	pthread_t t1, t2, t3;
	assert(bxp_init(19999, 1));
	assert((svc = bxp_offer("DTS")));
	VALGRIND_MONITOR_COMMAND("leak_check summary");
	assert(pthread_create(&t1, NULL, &receiver, NULL) == 0);
	assert(pthread_create(&t2, NULL, &timer, NULL) == 0);
	assert(pthread_create(&t3, NULL, &eventHandler, NULL) == 0);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	q->clear(q);
	q->destroy(q);
	pq->clear(pq);
	pq->destroy(pq);
	map->clear(map);
	map->destroy(map);
	return 0;
}
