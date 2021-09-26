// fifo.h : FIFO with mutex lock

#ifndef __FIFO_H__
#define __FIFO_H__

#include <pthread.h>
#include <stdbool.h>

typedef struct list_elem_ list_elem_t;

struct list_elem_ {
	void *ptr;              // content
	list_elem_t* next;
};

typedef struct {
   pthread_mutex_t *lock;
   unsigned int count;
   list_elem_t *head;
} list_t;

fifo_t * FIFO_create();
void FIFO_destroy(fifo_t *);
fifo_queue_t * FIFO_element(void *ptr); // ptr must be allocated
int FIFO_is_empty(fifo_t *);
int FIFO_count(fifo_t *f);
void FIFO_enqueue(fifo_t *, fifo_queue_t *);
fifo_queue_t * FIFO_dequeue(fifo_t *);
fifo_queue_t * FIFO_search(fifo_t *, int (*func)(fifo_queue_t *));
fifo_queue_t * FIFO_search2(fifo_t *,
		int (*func)(fifo_queue_t *, void *), void *arg);
fifo_queue_t * FIFO_remove2(fifo_t *,
		int (*func)(fifo_queue_t *, void *), void *arg);
void FIFO_foreach(fifo_t *, void (*func)(fifo_queue_t *));
void FIFO_foreach2(fifo_t *f,
				void (*func)(fifo_queue_t *, void *), void *arg);
void FIFO_lock(fifo_t *);
void FIFO_unlock(fifo_t *);

#endif
