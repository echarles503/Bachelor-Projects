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
#include <valgrind/valgrind.h>

#define UNUSED __attribute__((unused))

BXPEndpoint ep;
BXPService svc;

void *receiver(){
	char query[10000], response[10001];
	unsigned qlen, rlen;
	
	while ((qlen = bxp_query(svc, &ep, query, 10000)) > 0) {
		sprintf(response, "1%s", query);
		rlen = strlen(response) + 1;
		assert(bxp_response(svc, &ep, response, rlen));
	}
	return (NULL);
}

int main(UNUSED int argc, UNUSED char *argv[]) {
	void *ret;
	pthread_t t1;
	
	assert(bxp_init(19999, 1));
	assert((svc = bxp_offer("DTS")));
	assert(pthread_create(&t1, NULL, &receiver, NULL) == 0);
	pthread_join(t1, &ret);
	return 0;
}
