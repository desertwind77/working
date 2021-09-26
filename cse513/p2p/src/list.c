// fifo.c : FIFO with mutex lock
#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"

/*
 * FIFO_create
 * Functioanlity: Create a new FIFO
 * Return value: a pointer to a FIFO
 * Parameter: NONE
 */
fifo_t * FIFO_create() {
	fifo_t *f;

	f = (fifo_t *)MALLOC(sizeof(fifo_t));
	f->count = 0;
#ifdef __PTHREAD__
	f->lock = (pthread_mutex_t *)MALLOC(sizeof(pthread_mutex_t));

	if(pthread_mutex_init(f->lock, NULL) != 0) {
		printf("Pthread Init failed\n");
		return NULL;
	}
#else
	f->lock  = (lwp_mutex_t *)MALLOC(sizeof(lwp_mutex_t));
#endif
	f->queue = NULL;

	return f;
}

/*
 * FIFO_destroy
 * Functioanlity: Free all memory allocated to a FIFO, including
 *		all of its elements
 * Return value: NONE
 * Parameter:
 *		f - a pointer to the FIFO
 */
void FIFO_destroy(fifo_t *f) {
	fifo_queue_t *cur, *free_me;

	if(f == NULL) return;

	FIFO_lock(f);

	/* Free all queue */
	cur = f->queue;
	f->queue = NULL;
	while(f->count > 0) {
		free_me = cur;
		cur = cur->next;
		FREE(free_me);
		f->count--;
	}

	FIFO_unlock(f);

	FREE(f->lock);		// Free mutex lock
}

/*
 * FIFO_element
 * Functioanlity: Encapsulate a void pointer in a fifo_queue_t in
 *		order to be eligible to be put into a FIFO
 * Return value: the fifo_queue_t of the input void pointer
 * Parameter:
 *		ptr -  anything that you want to be put into the FIFO
 */
fifo_queue_t * FIFO_element(void *ptr) {
	fifo_queue_t  *q;

	q = (fifo_queue_t *)MALLOC(sizeof(fifo_queue_t));
	q->ptr = ptr;
	q->next = NULL;

	return q;
}

/*
 * FIFO_is_empty
 * Functioanlity: Check if the queue is empty
 * Return value: 1 if empty; otherwise 0; -1 for error
 * Parameter:
 *		f - a pointer to the FIFO
 */
int FIFO_is_empty(fifo_t *f) {
	int temp;

	if(f == NULL) return -1;

	FIFO_lock(f);
	temp = f->count;
	FIFO_unlock(f);

	return temp == 0;
}

/*
 * FIFO_count
 * Functioanlity: Return the number of elements
 * Return value: Number of elements
 * Parameter:
 *		f - a pointer to the FIFO
 */
int FIFO_count(fifo_t *f) {
	int temp;

	if(f == NULL) return -1;

	FIFO_lock(f);
	temp = f->count;
	FIFO_unlock(f);

	return temp;
}

/*
 * FIFO_enqueue
 * Functioanlity: Put an element into the queue
 * Return value: NONE
 * Parameter:
 *		f - a pointer to the FIFO queue
 *		q - an element to put into the queue
 */
void FIFO_enqueue(fifo_t *f, fifo_queue_t *q) {
	fifo_queue_t *cur;

	if(f == NULL) return;

	FIFO_lock(f);

	if(f->queue == NULL) { /* f is empty */
		f->queue = q;
		f->count++;
	} else { 				 /* f is not empty */
		cur = f->queue;

		/* Move cur to the last element of the queue */
		while(cur->next != NULL) {
			cur = cur->next;
		}

		/* Append q */
		cur->next = q;
		f->count++;
	}

	FIFO_unlock(f);
}

/*
 * FIFO_dequeue
 * Functioanlity: Get and remove the element at the head of the queue
 * Return value: an element at the head of the queue; NULL in case that
 *		the FIFO is empty
 * Parameter:
 *		f - an pointer to the FIFO queue
 */
fifo_queue_t * FIFO_dequeue(fifo_t *f) {
	fifo_queue_t *cur;

	if(f == NULL) return NULL;

	FIFO_lock(f);

	cur = NULL;
	if(f->count != 0) {
		cur = f->queue;
		f->queue = f->queue->next;
		cur->next = NULL;		// For the sake of safety
		f->count--;
	}

	FIFO_unlock(f);

	return cur;
}

/*
 * FIFO_search
 * Functioanlity: Search the FIFO "q" for an element that match the
 *		requirement specified in the callback subroutine "func"
 * Return value: an element if found; otherwise NULL
 * Parameter:
 *		f - a pointer to FIFO
 *		func - a calback function "int func_name(fifo_queue_t *q)" If
 *			the element matches, return 1; otherwise 0.
 */
fifo_queue_t * FIFO_search(fifo_t *f,  int (*func)(fifo_queue_t *)) {
	fifo_queue_t *cur, *dummy, *found;

	if(f == NULL) return NULL;

	FIFO_lock(f);

	found = NULL;	/* Not found */
	if(f->count != 0) {
		cur = f->queue;

		do {
			dummy = cur;
			cur = cur->next;
			if(func(dummy)) {
				found = dummy;
				break;
			}
		} while(cur != NULL);
	}


	FIFO_unlock(f);

	return found;
}

/*
 * FIFO_search2
 * Functioanlity: Search the FIFO "q" for an element that match the
 *		requirement specified in the callback subroutine "func", passing
 *		"arg" as the argument to "func"
 * Return value: an element if found; otherwise, NULL
 * Parameter:
 *		f - a pointer to FIFO
 *		func - a calback function "int func_name(fifo_queue_t *q,
 *			void* ptr)" If the element matches, return 1; otherwise 0.
 */
fifo_queue_t * FIFO_search2(fifo_t *f,
		int (*func)(fifo_queue_t *, void *), void *arg) {
	fifo_queue_t *cur, *dummy, *found;

	if(f == NULL) return NULL;

	FIFO_lock(f);

	found = NULL;	/* Not found */
	if(f->count != 0) {
		cur = f->queue;

		do {
			dummy = cur;
			cur = cur->next;
			if(func(dummy, arg)) {
				found = dummy;
				break;
			}
		} while(cur != NULL);
	}


	FIFO_unlock(f);

	return found;
}

/*
 * FIFO_remove2
 * Functioanlity: Search the FIFO "q" for an element that match the
 *		requirement specified in the callback subroutine "func", passing
 *		"arg" as the argument to "func" and the remove the found element
 *		from the queue.
 * Return value: an element if found; otherwise, NULL
 * Parameter:
 *		f - a pointer to FIFO
 *		func - a calback function "int func_name(fifo_queue_t *q,
 *			void* ptr)" If the element matches, return 1; otherwise 0.
 */
fifo_queue_t * FIFO_remove2(fifo_t *f,
		int (*func)(fifo_queue_t *, void *), void *arg) {
	fifo_queue_t *prev, *cur, *dummy, *found;

	if(f == NULL) return NULL;

	FIFO_lock(f);

	found = NULL;	/* Not found */
	if(f->count != 0) {
		prev = cur = f->queue;

		do {
			if(func(cur, arg)) {
				found = cur;
				f->count--;

				if(prev == cur)
					f->queue = cur->next;
				else
					prev->next = cur->next;

				break;
			}

			if(prev != cur) prev = cur;
			cur = cur->next;
		} while(cur != NULL);
	}


	FIFO_unlock(f);

	return found;
}

/*
 * FIFO_foreach
 * Functioanlity: Walk through each element in FIFO and apply the
 *		callback routine "func" with each element.
 * Return value: NONE
 * Parameter:
 *		f - a pointer to FIFO
 *		func - a calback function "void func_name(fifo_queue_t *)"
 */
void FIFO_foreach(fifo_t *f, void (*func)(fifo_queue_t *)) {
	fifo_queue_t *cur, *dummy;

	if(f == NULL) return;

	FIFO_lock(f);

	if(f->count != 0) {
		cur = f->queue;

		do {
			dummy = cur;
			cur = cur->next;
			func(dummy);
		} while(cur != NULL);
	}

	FIFO_unlock(f);
}

/*
 * FIFO_foreach2
 * Functioanlity: Walk through each element in FIFO and apply the
 *		callback routine "func" with each element, passing arg as an
 *		additional parameter to "func"
 * Return value: NONE
 * Parameter:
 *		f - a pointer to FIFO
 *		func - a calback function "void func_name(fifo_queue_t *,
 *			void *arg)"
 *		arg - parameter to the callback function "func"
 */
void FIFO_foreach2(fifo_t *f,
		void (*func)(fifo_queue_t *, void *), void *arg) {
	fifo_queue_t *cur, *dummy;

	if(f == NULL) return;

	FIFO_lock(f);

	if(f->count != 0) {
		cur = f->queue;

		do {
			dummy = cur;
			cur = cur->next;
			func(dummy, arg);
		} while(cur != NULL);
	}

	FIFO_unlock(f);
}

/*
 * FIFO_lock
 * Functioanlity: Lock a FIFO
 * Return value: NONE
 * Parameter:
 *		f - a pointer to the FIFO to be locked
 */
void FIFO_lock( fifo_t *f ) {
#ifndef __NO_FIFO_LOCK__
   int res;

   if ( f == NULL ) return;

   if ( ( res = pthread_mutex_lock( f->lock ) ) != 0 ) {
      fprintf( stderr, "%s unable to lock mutex\n", __func__ );
      exit( -1 );
   }
#endif
}

/*
 * FIFO_unlock
 * Functioanlity: Unlock a FIFO
 * Return value: NONE
 * Parameter:
 *		f - a pointer to the FIFO to be locked
 */
void FIFO_unlock( fifo_t *f ) {
#ifndef __NO_FIFO_LOCK__
   int res;

   if ( f == NULL ) return;

   if( ( res = pthread_mutex_unlock( f->lock ) ) != 0 ) {
      fprintf( stderr, "%s unable to unlock mutex\n", __func__ );
      exit(-1);
   }
#endif
}

#ifdef __FIFO_TEST__
// Routines for testing FIFO
const int MAX= 300;

void printEntry( fifo_queue_t *q ) {
   printf( "foreach: %d\n", *( (int *)q->ptr ) );
}

int test( fifo_queue_t *q ) {
   if( *( (int *)q->ptr ) == 3999 ) {
      return 1;
   } else {
      return 0;
   }
}

int CB_test_remove2( fifo_queue_t *qq, void *arg ) {
   if( *((int *)qq->ptr) == *( (int *)arg ) ) {
      return 1;
   } else {
      return 0;
   }
}

void pause() {
   printf( "Paused: Please press [RETURN] to continue\n" );
   getchar();
}

int main( int argc, char **argv ) {
   fifo_t *ff;
   fifo_queue_t *qq;
   int j, arg;

   ff = FIFO_create();

   // Test FIFO_enqueue
   for ( j=0 ; j <= MAX ; j++ ) {
      int *i;

      i = (int *)malloc( sizeof( int ) );
      if( i == NULL ) {
         fprintf( stderr, "%s unable to allocate memory\n", __func__ );
         return -1;
      }

      *i = j;
      qq = FIFO_element( i );
      printf( "Enqueue: %d\n", *( (int *)qq->ptr ) );
      FIFO_enqueue( ff, qq );
   }

   pause();

   // Test FIFO_search
   if( (qq = FIFO_search(ff, test)) != NULL ) {
      printf("FOUND: %d\n", *((int *)qq->ptr));
   }

   pause();

   // Test FIFO_remove2
   for(j=100;j<=200; j++) {
      if( (qq = FIFO_remove2(ff, CB_test_remove2, &j)) != NULL ) {
         printf("FOUND: %d\n", *((int *)qq->ptr));
      }
   }

   pause();

   // Test FIFO_foreach
   FIFO_foreach(ff, printEntry);
   pause();

   // Test FIFO_dequeue
   while ( ( qq = FIFO_dequeue( ff ) ) != NULL ) {
      printf( "Dequeue: %d\n", *( (int *)qq->ptr ) );
      if ( qq != NULL ) free( qq );
   }

   FIFO_destroy( ff );
}

#endif
