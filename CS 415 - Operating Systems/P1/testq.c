#include <stdio.h>
#include <stdlib.h>
#include "ADTs/queue.h"

int main(void){
	

	const Queue *q = Queue_create(0L);
	
	int tmp[5];
	for (int i = 0; i < 5; i++){
		tmp[i] = i;
		q->enqueue(q, &tmp[i]);
	}	
	
	while (q->size(q) != 0L){
		int *elem;
		q->dequeue(q, (void **)&elem);
		printf("Dequeued: %d\n", *elem);
	}

	q->destroy(q);
	return 0;
}

