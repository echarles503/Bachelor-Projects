#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "duckchat.h"
#include "raw.h"
#define USAGE "Usage: ./client server_socket server_port username"



char channels[CHANNEL_MAX][512];
int nch = 0;
char curr_ch[CHANNEL_MAX] = "Common";

int hostname_to_ip(char *hostname , char *ip)
{  
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_in *h;
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo( hostname , "http" , &hints , &servinfo)) != 0) 
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

void reqLogout(int fd, struct sockaddr_in sa){
	struct request_logout lo;
	lo.req_type = REQ_LOGOUT;
	sendto(fd, &lo, sizeof(struct request_logout), 0, (struct sockaddr*)&sa, sizeof(sa));
}

void reqLogin(int fd, struct sockaddr_in sa, char user[USERNAME_MAX]){
	struct request_login login;
	login.req_type = REQ_LOGIN;
	strcpy(login.req_username, user);
	sendto(fd, &login, sizeof(struct request_login), 0, (struct sockaddr*)&sa, sizeof(sa));
}

void reqJoin(int fd, struct sockaddr_in sa, char channel[CHANNEL_MAX]){
	struct request_join j;
	j.req_type = REQ_JOIN;
	strcpy(j.req_channel, channel);
	sendto(fd, &j, sizeof(struct request_join), 0,  (struct sockaddr*)&sa, sizeof(sa));
}

void reqLeave(int fd, struct sockaddr_in sa, char channel[CHANNEL_MAX]){
	struct request_leave l;
	l.req_type = REQ_LEAVE;
	strcpy(l.req_channel, channel);
	sendto(fd, &l, sizeof(struct request_leave), 0,  (struct sockaddr*)&sa, sizeof(sa));
}

void reqSay(int fd, struct sockaddr_in sa, char channel[CHANNEL_MAX], char text[SAY_MAX]){
	struct request_say s;
	s.req_type = REQ_SAY;
	strcpy(s.req_channel, channel);
	strcpy(s.req_text, text);
	ssize_t nbytes = sendto(fd, &s, sizeof(struct request_say), 0,  (struct sockaddr*)&sa, sizeof(sa));
	printf("\nsizeof(say) = %ld", sizeof(struct request_say));
	printf("\nnbytes = %ld", nbytes);
}

void reqList(int fd, struct sockaddr_in sa){
	struct request_list li;
	li.req_type = REQ_LIST;
	sendto(fd, &li, sizeof(struct request_list), 0, (struct sockaddr*)&sa, sizeof(sa));
}

void reqWho(int fd, struct sockaddr_in sa, char channel[CHANNEL_MAX]){
	struct request_who w;
	w.req_type = REQ_WHO;
	strcpy(w.req_channel, channel);
	sendto(fd, &w, sizeof(struct request_who), 0,  (struct sockaddr*)&sa, sizeof(sa));
}

int nspaces(char *buf){
	int i;
	int c = 0;
	for (i = 0; buf[i] != '\0'; i++){
		if (buf[i] == ' ')
			c++;
	}
	return c;
}

void read_stdin(char *buf){
	raw_mode();	
	int i = 0;
	char c;
	while ((c = getchar()) != '\n' && c != EOF){
		if (i == 63)
			continue;
		printf("%c",c);
		buf[i] = c;
		i++;
	}
	buf[i] = '\0';
	cooked_mode();
	printf("\n> ");
}

int find_channel(char buf[CHANNEL_MAX]){
	int i;
	for (i = 0; i < nch; i++){
		if (strcmp(buf, channels[i]) == 0)
			return 1
	}
	return 0;
}

void send_to_peer(int fd, struct sockaddr_in sa, char *buf, char *ch){
	if (buf[0] == '/')
	{
		char *tok;
		int spaces = nspaces(buf);
		tok = strtok(buf, " "); //,-!?\r\t\f\v\n\0|/\\_

		/* Parsing user cmds */
		if ((strcmp(tok, "/exit") == 0)&&(spaces==0))
		{
			reqLogout(fd, sa);
			break;
		}
		else if ((strcmp(tok, "/join") == 0)&&(spaces==1))
		{
			tok = strtok(NULL, " ");
			reqJoin(fd, sa, tok);
			strcpy(channels[nch], tok);
			nch = (nch + 1) % 511;
			strcpy(curr_ch, tok);
		}
		else if ((strcmp(tok, "/leave") == 0)&&(spaces==1))
		{
			tok = strtok(NULL, " ");
			reqLeave(fd, sa, tok);
			if (strcmp(curr_ch, tok) == 0)
			{
				curr_ch = "";
			}
		}
		else if ((strcmp(tok, "/list") == 0)&&(spaces==0))
		{
			reqList(fd, sa);
		}
		else if ((strcmp(tok, "/who") == 0)&&(spaces==1))
		{
			tok = strtok(NULL, " ");
			reqWho(fd, sa, tok);
		}
		else if ((strcmp(tok, "/switch") == 0)&&(spaces==1))
		{
			tok = strtok(NULL, " ");
			if (find_channel(tok))
				strcpy(curr_ch, tok);
			else
				printf("\nYou have not subscribed to channel %s", tok);
		}
		else
		{
			printf("\n*Unknown Request");
		}
	}
	else /* say request */
	{	
		if (strlen(curr_ch) > 0)
			reqSay(fd, sa, curr_ch, buf);
	}
}

int read_from_peer(int fd, struct sockaddr_in sa)
{
	struct text_say resp;
	void *offset = &resp;

	int recsize = recvfrom(fd, &resp, sizeof(struct text_say), 0,
			(struct sockaddr*)&sa, sizeof(sa));
	if (recsize < 0)
	{
		close(fd);
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (resp.txt_type == 0)
	{
		/* say */
		struct text_say *say = offset;
		printf("[%s][%s]: %s\n", say->txt_channel, say->txt_username, say->txt_text);
		return 0;
	}
	else if (resp.txt_type == 1)
	{
		/* list */
		/*
		struct text_list *li = offset;
		int n = li->txt_nchannels;
		int i;
		for (i = 0; i < n; i++){
			printf("%s\n", li->txt_channels[i].ch_channel);
		}
		return 0;
		*/
	}
	else if (resp.txt_type == 2)
	{
		/* who */
		/*
		struct text_who *w = offset;
		int n = w->txt_nusernames;
		int i;
		printf("Users on channel %s:\n", w->txt_channel);
		for (i = 0; i < n; i++){
			printf("%s\n", w->txt_users[i].us_username);
		}
		return 0;
		*/
	}
	else if (resp.txt_type == 4)
	{
		/* ok! */
		return 0;
	}
	else
	{
		/* error */
		struct text_error *e = offset;
		printf("Error: %s\n", e->txt_error);
		return -1;
	}
}


int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		printf("%s\n", USAGE);
		exit(EXIT_FAILURE);
	}

	fd_set read_fds, write_fds;
	ssize_t recsize;

	int port = atoi(argv[2]);
	char ip[100];
	hostname_to_ip(argv[1], ip); /* Resolve hostname */


	struct sockaddr_in sa;
	memset(&sa, 0, sizeof(sa)); /* Zero out socket address */
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(ip);
	sa.sin_port = htons(port);

	
	/* Create socket */
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (fd == -1){
		printf("Error creating socket\n");
		//printf("error resolving hostname..\n");
		exit(EXIT_FAILURE);
	}

	/* Send login packets */
	channels[nch++] = "Common";
	reqLogin(fd, sa, argv[3]);
	reqJoin(fd, sa, curr_ch);

	
	printf("> ");
	while(1){
		
		/* initialize fd set */
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);
		FD_SET(fd, &read_fds);
		FD_SET(fd, &write_fds);

		int action = select(fd+1, &read_fds, &write_fds, 0, 0);

		switch (action)
		{
			case -1:
				perror("error: select()\n");
				close(fd);
				exit(EXIT_FAILURE);
			default:
				if (FD_ISSET(fd, &read_fds))
				{
					/* client ready to read */
					read_from_peer(fd, sa);
				}
				if (FD_ISSET(fd, &write_fds))
				{
					/* client ready to write */
					char buf[SAY_MAX];
					read_stdin(buf);
					send_to_peer(fd, sa, buf, curr_ch);
				}
		}
	}
	close(fd);
	return 0;
}