#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "prioqueue.h"
#include "cskmap.h"

const PrioQueue *pq;
const CSKMap *map;

typedef struct event {
	char *id;
	bool cancelled;
	char **data;
} Event;


int cmp(void *x, void *y){
	return *(int *)x < *(int *)y;
}

void createEvent(int id, char **data){
	int i;
	char key[9];
	sprintf(key, "%d", id);
	Event *e = (Event *)malloc(sizeof(Event));
	e->id = strdup(key);
	e->cancelled = false;
	e->data = (char **)malloc(sizeof(char *) * 7);
	for (i = 0; i < 7; i++){
		e->data[i] = strdup(data[i]);
	}
	struct timeval epoch;
	map->putUnique(map, key, (void *)e);
}

void freeEvent(void *e){
	int i;
	for (i = 0; i < 7; i++){
		free(((Event *)e)->data[i]);
	}
	free(((Event *)e)->data);
	free(((Event *)e)->id);
	free(((Event *)e));
}

int main(int argc, char *argv[]){
	map = CSKMap_create(&freeEvent);
	char *tmp[7];
	int key = 123;
	void *value;
	tmp[0] = "hi";
	tmp[1] = "WORLD!";
	for (int i = 2; i < 7; i++){
		tmp[i] = "123.3";
	}
	
	createEvent(key, tmp);

	map->get(map, "123", &value);
	printf("data[2] -> %s\n", ((Event *)value)->data[2]);
	gettimeofday(&(((Event *)value)->epoch), NULL);
	printf("%ld\n", ((Event *)value)->epoch.tv_usec);
	map->clear(map);

	createEvent(key, tmp);


	/*int x = 1;
	int y = 2;
	char *w[25];
	void *val;
	int *prio;

	w[0] = "hello";
	w[1] = "world!";

	pq = PrioQueue_create(&cmp, NULL, doNothing);
	pq->insert(pq, (void *)&x, (void *)w);
	//pq->insert(pq, (void *)&y, (void *)15);
	printf("pqsize: %d\n", pq->size(pq));
	
	pq->removeMin(pq, (void **)&prio, (void *)&val);

	printf("popped: val=%s %s prio=%d\n", ((char **)val)[0], ((char **)val)[1], *prio);
	printf("pqsize: %d\n", pq->size(pq));*/
	/*
	map = CSKMap_create(doNothing);
	char *key;
	int *ret;
	int id = 1020;
	int val = 69;
	sprintf(key, "%d", id);
	map->put(map, key, (void *)&val);
	map->get(map, key, (void **)&ret);
	printf("[GET] -> key: %s val: %d\n", key, *ret);
	*/
	return 0;
}