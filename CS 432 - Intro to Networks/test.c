#include <stdio.h>
#include <stdlib.h>
#include "include/llistcskmap.h"

int main(int argc, char*argv[]){
	printf("WTF!!!\n");
	int val = 10;
	int *tmp;
	const CSKMap *m = CSKMap_create(NULL);
	m->put(m, "put", (void *)&val);
	m->get(m, "put", (void **)&tmp);
	printf("proof of concept %d\n", *tmp);
	return 0;
}