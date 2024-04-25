#include "hashset.h"
#include <stdlib.h>

#define DEFAULT_CAPACITY 16
#define MAX_CAPACITY 134217728L
#define DFAULT_LOAD_FACTOR 0.75
#define TRIGGER 100 /* number of changes that will trigger a load check */

typedef struct node {
	struct node *next;
	void *value;
} Node;

typedef struct s_data {
	long (*hash)(void *, long n);
	int (*cmp)(void *, void *);
	long size;
	long capacity;
	long changes;
	double load;
	double loadFactor;
	double increment;
	Node **buckets;
	void (*freeV)(void *v);

} SData;

static void purge(SData *sd, void (*freeV)(void *v)) {
	long i;

	for (i = 0L; i < sd->capacity; i++){
		Node *p, *q;
		p = sd->buckets[i];
		while (p != NULL){
			if (freeV != NULL) {
				(*freeV)(p->value);
			}
			q = p->next;
			free(p);
			p = q;
		}
		sd->buckets[i] = NULL;
	}

}

static void s_destroy(const Set *s){
	SData *sd = (SData *)s->self;
	purge(sd, sd->freeV);
	free(sd->buckets);
	free(sd);
	free((void *)s);
}

static void s_clear(const Set *s){
	SData *sd = (SData *)s->self;
	purge(sd, sd->freeV);
	sd->size = 0;
	sd->load = 0.0;
	sd->changes = 0;
}

static Node *findMember(SData *sd, void *value, long *bucket){
	long i = sd->hash(value, sd->capacity);
	Node *p;

	*bucket = i;
	for (p = sd->buckets[i]; p != NULL; p = p->next) {
		if (sd->cmp(p->value, value) == 0) {
			break;
		}
	}
	return p;
}

/*
 * helper function that resizes the hash table
 */
static void resize(SData *sd) {
	int N;
	Node *p, *q, **array;
	long i, j;

	N = 2 * sd->capacity;
	if (N > MAX_CAPACITY)
		N = MAX_CAPACITY;
	if (N == sd->capacity)
		return;
	array = (Node **)malloc(N * sizeof(Node *));
	if (array == NULL)
		return;
	for (j = 0; j < N; j++)
		array[j] = NULL;
	/*
	 * now redistribute the entries into the new set of buckets
	 */
	for (i = 0; i < sd->capacity; i++) {
		for (p = sd->buckets[i]; p != NULL; p = q) {
			q = p->next;
			j = sd->hash(p->value, N);
			p->next = array[j];
			array[j] = p;
		}
	}
	free(sd->buckets);
	sd->buckets = array;
	sd->capacity = N;
	sd->load /= 2.0;
	sd->changes = 0;
	sd->increment = 1.0 / (double)N;
}

/*
 * helper function that inserts new member into table
 */
static int insertMember(SData *sd, void *value, long i){
	Node *p = (Node *)malloc(sizeof(Node));
	int ans = 0;

	if (p != NULL){
		p->value = value;
		p->next = sd->buckets[i];
		sd->buckets[i]  = p;
		sd->size++;
		sd->load += sd->increment;
		sd->changes++;
		ans = 1;
	}
	return ans;
}

static bool s_add(const Set *s, void *value){
	SData *sd = (SData *)s->self;
	long i;
	Node *p;
	int ans = 0;

	if (sd->changes > TRIGGER) {
		sd->changes = 0;
		if (sd->load > sd->loadFactor)
			resize(sd);
	}
	p = findMember(sd, value, &i);
	if (p == NULL){
		ans = insertMember(sd, value, i);
	}
	return ans;
}

static bool s_containsMember(const Set *s, void *value){
	SData *sd = (SData *)s->self;
	long bucket;

	return (findMember(sd, value, &bucket) != NULL);
}

static bool s_isEmpty(const Set *s) {
	SData *sd = (SData *)s->self;
	return (sd->size == 0L);
}

static bool s_remove(const Set *s, void *value){
	SData *sd = (SData *)s->self;
	long i;
	Node *member;
	int ans = 0;

	member = findMember(sd, value, &i);
	if (member != NULL) {
		Node *p, *c;
		/* determine where the member lives in the singly linked list */
		for (p = NULL, c = sd->buckets[i]; c != member; p = c, c = c->next)
			;
		if (p == NULL)
			sd->buckets[i] = member->next;
		else
			p->next = member->next;
		sd->size--;
		sd->load -= sd->increment;
		sd->changes++;
		if (sd->freeV != NULL)
			sd->freeV(member->value);
		free(member);
		ans = 1;
	}
	return ans;
}

static long s_size(const Set *s) {
	SData *sd = (SData *)s->self;
	return sd->size;
}

/*
 * helper function for toArray method
 * generates an array of void * from set
 * returns pointer to the or array or NULL if malloc failure
 */
static void **members(SData *sd) {
	void **tmp = NULL;
	if (sd->size > 0L) {
		size_t nbytes = sd->size * sizeof(void *);
		tmp = (void **)malloc(nbytes);
		if (tmp != NULL) {
			long i, n = 0L;
			for (i = 0L; i < sd->capacity; i++) {
				Node *p = sd->buckets[i];
				while (p != NULL) {
					tmp[n++] = (p->value);
					p = p->next;
				}
			}
		}
	}
	return tmp;
}

static void **s_toArray(const Set *s, long *len){
	SData *sd = (SData *)s->self;
	void **tmp = (void **)members(sd);

	if (tmp != NULL)
		*len = sd->size;
	return tmp;
}

static const Iterator *s_itCreate(const Set *s) {
	SData *sd = (SData *)s->self;
	const Iterator *it = NULL;
	void **tmp = (void **)members(sd);

	if (tmp != NULL) {
		it = Iterator_create(sd->size, tmp);
		if (it == NULL)
			free(tmp);
	}
	return it;
}


static Set template = {
	NULL, s_destroy, s_clear, s_add, s_containsMember, s_isEmpty, s_remove,
	s_size, s_toArray, s_itCreate
};

/*
 * helper function to create a new Set dispatch table
 */

static const Set *newSet(long capacity, double loadFactor, long (*hash)(void *, long),
						 int (*cmp)(void *, void *), void (*freeV)(void *)) {
	Set *s = (Set *)malloc(sizeof(Set));
	long N;
	double lf;
	Node **array;
	long i;
	if (s != NULL) {
		SData *sd = (SData *)malloc(sizeof(SData));
		if (sd != NULL){
			N = ((capacity > 0)  ? capacity : DEFAULT_CAPACITY);
			if (N > MAX_CAPACITY)
				N = MAX_CAPACITY;
			lf = ((loadFactor > 0.000001) ? loadFactor : DEFAULT_LOAD_FACTOR);
			array = (Node **)malloc(N * sizeof(Node *));
			if (array != NULL) {
				sd->capacity = N;
				sd->loadFactor = lf;
				sd->size = 0L;
				sd->load = 0.0;
				sd->changes= 0L;
				sd->increment = 1.0 / (double)N;
				sd->hash = hash;
				sd->cmp = cmp;
				sd->freeV = freeV;
				sd->buckets = array;
				for (i = 0; i < N; i++)
					array[i] = NULL;
				*s = template;
				s->self = sd;
			}
			else {
				free(sd);
				free(s);
				s = NULL;
			}
		}
		else {
			free(s);
			s = NULL;
		}
	}
	return s;
}

const Set *HashSet(void (*freeValue)(void*), int (*cmpFxn)(void*, void*),
                   long capacity, double loadFactor,
                   long (*hashFxn)(void *m, long N)
                  ) {
	/* Set instance and return to the caller */
	return newSet(capacity, loadFactor, hashFxn, cmpFxn, freeValue);
}