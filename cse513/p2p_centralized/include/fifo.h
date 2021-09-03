/*
 * fifo.h
 * FIFO with mutex lock
 *	$Id: fifo.h,v 1.1 2002/11/18 06:07:49 tangpong Exp $
 */

#ifndef __FIFO_H__
#define __FIFO_H__

#ifdef __PTHREAD__
#include <pthread.h>
#else
#include <sys/lwp.h>
#endif

/* An ordinary singly link list */
typedef struct queue_s fifo_queue_t;
struct queue_s {
	void *ptr;				/* Content */
	fifo_queue_t * next;		/* Next element */
};

/* FIFO */
typedef struct {
	unsigned int count;	/* Number of elements in the FIFO */
	void *ext;				/* Customized datas structure */
#ifdef __PTHREAD__
	pthread_mutex_t *lock;
#else
	lwp_mutex_t *lock;	/* Mutex lock for the FIFO */
#endif
	fifo_queue_t *queue;		/* Queue */
} fifo_t;

/* Utilities */
void * MALLOC(int size);
void FREE(void *ptr);

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
