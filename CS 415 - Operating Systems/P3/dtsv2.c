/* 
 * Name: Elias Charles
 * DuckID: eliasc@uoregon.edu
 * 
 * This is my own work.
 * 
 * CIS 415 Project 3
 */

#include "BXP/bxp.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#define UNUSED __attribute__((unused))

#include <unistd.h>
BXPEndpoint ep;
BXPService svc;

int extractWords(char *buf, char *sep, char *words[]) {
    int i;
    char *p;

    for (p = strtok(buf, sep), i = 0; p != NULL; p = strtok(NULL, sep),i++)
        words[i] = p;
    words[i] = NULL;
    return i;
}

void *receiver(){
	
	char query[10000], response[10001];
	unsigned qlen, rlen;
	int N;
	while ((qlen = bxp_query(svc, &ep, query, 10000)) > 0) {
		char *w[25];
     	N = extractWords(query, "|", w);
		if ((strcmp(w[0], "OneShot") == 0) || (strcmp(w[0], "Repeat") == 0)){
			if (N != 7)
				sprintf(response, "0");
			else
				sprintf(response, "1%s", query);
		} else if (strcmp(w[0], "Cancel") == 0) {
			if (N != 2)
				sprintf(response, "0");
			else
				sprintf(response, "1%s", query);
		} else {
			sprintf(response, "0");
		}

		rlen = strlen(response) + 1;
		assert(bxp_response(svc, &ep, response, rlen));
	}
	return (NULL);
}

int main(UNUSED int argc, UNUSED char *argv[]) {
	pthread_t t1;
	assert(bxp_init(19999, 1));
	assert((svc = bxp_offer("DTS")));
	assert(pthread_create(&t1, NULL, &receiver, NULL) == 0);
	pthread_join(t1, NULL);
	return 0;
}
