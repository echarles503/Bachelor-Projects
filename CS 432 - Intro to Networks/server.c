#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h> /* for close() for socket */ 
#include "duckchat.h"
#include "include/llistcskmap.h"
#define USAGE "Usage: ./server domain_name port_num"

int hostname_to_ip(char *hostname , char *ip)
{
	//int sockfd;  
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_in *h;
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;

	if ( (rv = getaddrinfo( hostname , "http" , &hints , &servinfo)) != 0) 
	{	
		strcpy(ip, hostname);
		//fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first
	for(p = servinfo; p != NULL; p = p->ai_next) 
	{
		h = (struct sockaddr_in *) p->ai_addr;
		strcpy(ip , inet_ntoa( h->sin_addr ) );
	}
	
	freeaddrinfo(servinfo); // all done with this structure
	return 0;
}
void freeValue(void *e){
	int *a = (int *)e;
	a += 0;
}

int main(int argc, char *argv[])
{	

	if (argc != 3)
	{
		printf("%s\n", USAGE);
		exit(EXIT_FAILURE);
	}
	// Resolve the ip from the hostname
	char *hostname = argv[1];
	char ip[100];
	hostname_to_ip(hostname, ip);
	int port = atoi(argv[2]);

	int sockfd;
	struct sockaddr_in sa;
	ssize_t recsize;
	socklen_t fromlen;

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(ip);
	sa.sin_port = htons(port);
	fromlen = sizeof(sa);

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1)
	{
		perror("bind failed: Cannot assign requested address");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	CSKMap *userList = CSKMap_create(NULL);
	CSKMap *chList = CSKMap_create(NULL);
	CSKMap *Common = CSKMap_create(NULL);
	chList->put(chList, "Common", (void *)Common);

	struct request_say req;
	void *offset = &req;

	while(1)
	{
		recsize = recvfrom(sockfd, &req, sizeof(struct request_say), 0,
			(struct sockaddr*)&sa, &fromlen);

		if (recsize < 0)
		{
			fprintf(stderr, "%s\n", strerror(errno));
			close(sockfd);
			exit(EXIT_FAILURE);
		}
		/* Grab IP and username */
		char *peerip = strdup(inet_ntoa(sa.sin_addr)); //UNSAFE!!!
		char *usr;
		if (!userList->get(userList, peerip, (void **)&usr))
			usr = "unknown user";

		 /* Login Request */
		if (req.req_type == 0)
		{
			struct request_login *login = offset;
			login = offset;
			/* Map IP to username*/
			usr = strdup(login->req_username);
			userList->putUnique(userList, peerip, (void *)usr);
			// Send message to all users in channel
			printf("server: %s logs in\n", usr);
		}
			/* Logout Request */
		else if (req.req_type == 1)
		{	
			// TODO: Remove user from map
			//		 Remove peerip from all channels and free entries
			userList->remove(userList, peerip);
			printf("server: %s logs out\n", usr);
		}
			/* Join Request */
		else if (req.req_type == 2)
		{
			struct request_join *j = offset;
			CSKMap *chset = CSKMap_create(NULL);
			CSKMap *tmp;
			/* fetch user from userList */
			char *channel = strdup(j->req_channel);
			/* Put unique channel name in the set of channels */
			chList->putUnique(chList, channel, (void *)chset);
			/* Get the new channel from our set of channels and add user */
			chList->get(chList, j->req_channel, (void **)&tmp);
			tmp->putUnique(tmp, peerip, (void *)usr);
			printf("server: %s join channel %s\n", usr, channel);
			
		}
		 /* Leave Request */
		if (req.req_type == 3) 
		{
			struct request_leave *l = offset;

			if (!chList->remove(chList, l->req_channel))
			{
				printf("server: %s trying to leave non-existent channel %s\n", usr, l->req_channel);
				// Send error packet
			}
			else
			{
				printf("server: %s leaves channel %s\n", usr, l->req_channel);
			}
		}
		if (req.req_type == 4)
		{
			struct request_say *say = offset;
			printf("server: %s sends say message in %s\n", usr, say->req_channel);
		}
	}
}

// TODO:
// - Send responses to client