/*
 * server.c : Skeleton multi-thread server
 * By: Athichart Tangpong
 *
 * Problems:
 *		1. The server can serve only 2037 connections. The server 
 *			can not create more than 2037 threads.
 *
 *	Potential solutions:
 *		1. Use thread pool instead of creating a new thread on demand.
 *
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
#include "config.h"
#include "fifo.h"

#define  PORT 6001

typedef struct node_t node_t;
struct node_t {
	int node_id;
	char *hostname;
	fifo_t *keys;
};

// Global variable 
extern char *optarg;
extern int errno;
fifo_t *node_queue;

// Prototype 
void * thr_func(void *ptr);
int perform_sort(int fd); 
void usage(char *st); 
int CB_search_node_queue(fifo_queue_t *qq, void *node_id); 
int CB_search_key(fifo_queue_t *qq, void *key);
int CB_search_key2(fifo_queue_t *qq, void *key);
int join(int fd, fifo_t *node_queue);
int leave(int fd, fifo_t *node_queue);
int search(int fd, fifo_t *node_queue);
int terminate(int fd, fifo_t *node_queue);

int main(int argc,char **argv) {
	struct	sockaddr_in cli_addr, serv_addr;
	int		sockfd, newsockfd;
	int		status, port, clilen;
	char		*delim="p:s:r:dv?";
	char		opt;
	fd_set 	watchset;
	struct 	timeval tv;

	port = PORT;
	while((opt = getopt(argc,argv,delim)) != EOF) {
		switch(opt) {
			case 'p' : port = atoi(optarg); break;
			case '?' : 
			default  : usage(argv[0]); exit(0);
		}
	}
	node_queue = FIFO_create();

	// Open a TCP socket (an Internet stream socket.)
	if( (sockfd=socket(AF_INET,SOCK_STREAM,0))<0 ) {
		perror("Server:can't open stream socket\n");
		exit(1);
	}

	// Bind our local address so that the client can send to us.
	bzero((char *) &serv_addr,sizeof(serv_addr));
	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port	     = htons(port);
	if( bind(sockfd,(struct sockaddr *) &serv_addr,
				sizeof(serv_addr))<0) {
		perror("Server: can't bind local address\n");
		exit(1);
	}
	listen(sockfd,5);

	FD_ZERO(&watchset);
	FD_SET(sockfd, &watchset);
	tv.tv_sec = 10;
	tv.tv_usec = 0;

	for(;;) {
		pthread_t *th;
		int *para;
		int res;

		if(select(sockfd+1, &watchset, NULL, NULL, &tv) < 0) {
			perror("Server: select error\n");
			return -1;
		}

		if(FD_ISSET(sockfd, &watchset) != 0) {
			int size;

			clilen=sizeof(cli_addr);
			if((newsockfd = accept(sockfd, (struct sockaddr *) 
						&cli_addr,&clilen)) < 0) {
				perror("Server: accept error\n");
				return -1;
			}

			th = (pthread_t *)malloc(sizeof(pthread_t));
			para = (int *)malloc(sizeof(int));
			*para = newsockfd;
	
			if( (res = pthread_create(th, NULL, thr_func, 
							(void *)para)) != 0) {
				printf("Server: pthread_create error\n");
				return -1;
			}
			pthread_detach(*th);
		} else {
			FD_SET(sockfd, &watchset);
		}
	}	/* for(;;) */
}

/*
 * thr_func
 * Functionality : Skeleton for the Multi-thread Server
 * Return Value : None
 * Parameter :
 *		ptr - pointer to the parameter
 */
void * thr_func(void *ptr) {
	int sockfd = *((int *)ptr);
	int flag;

	perform(sockfd);
	close(sockfd);
	free(ptr);

	return NULL;
}

// ---------------------- Customized routines -----------------------

/* 
 * CB_search_node_queue
 * Functionality : Search for the queue for the node "node_id"
 * Return Value : 1 if found, 0 otherwise.
 * Parameter :
 *		qq : an element in the look-up table which is a queue 
 *			of keys on a particular node
 *		node_id : the node_id to be searched for
 */
int CB_search_node_queue(fifo_queue_t *qq, void *node_id) {
	node_t *node;
	
	if(qq->ptr != NULL) {
		node = (node_t *)qq->ptr;
		if(node->node_id == *((int *)node_id))
			return 1;
		else 
			return 0;
	}

	return 0;
}

/* 
 * CB_search_key
 * Functionality : Search the look-up table for a node that 
 *		hold a particular key
 * Return Value : 1 if found, 0 otherwise.
 * Parameter :
 *		qq : an element in the look-up table which is a queue 
 *			of keys on a particular node
 *		key : the key to be searched for
 */
int CB_search_key(fifo_queue_t *qq, void *key) {
	fifo_queue_t *qqq;
	node_t *node;

	if(qq->ptr != NULL) {
		node = (node_t *)qq->ptr;
		if( (qqq = FIFO_search2(node->keys, CB_search_key2, key)) 
				!= NULL ) {
			return 1;
		} else {
			return 0;
		}
	}

	return 0;
}

/* 
 * CB_search_key2
 * Functionality : Search the queue for a node for a key
 * Return Value : 1 if found, 0 otherwise.
 * Parameter :
 *		qq : an element in the queue for a node which is a key
 *		key : the key to be searched for
 */
int CB_search_key2(fifo_queue_t *qq, void *key) {
	int l_key;
	
	if(qq->ptr != NULL) {
		l_key = *((int *)qq->ptr);
		if(l_key == *((int *)key)) {
			return 1;
		} else {
			return 0;
		}
	}

	return 0;
}

void CB_free_key(fifo_queue_t *qq) {
	int *key;
	
	if(qq->ptr != NULL) {
		key = (int *)qq->ptr;
		free(key);
	}
}

/*
 * perform
 * Functionality : Connection handler
 * Return Value : 1 if success, otherwise -1
 *	Parameter :
 *		fd - socket descriptor
 */
int perform(int fd) {
	short opcode;

	/* Receive opcode */
	if(readn(fd, &opcode, sizeof(short))<0) {
		perror("Server:readn error\n");
		return -1;
	}     

	switch(opcode) {
		case JOIN : join(fd, node_queue); break;
		case SEARCH : search(fd, node_queue); break; 
		case DEAD :
		case LEAVE: leave(fd, node_queue); break; 
		case EXIT : terminate(fd, node_queue); break;
	}

	return 1;
}

/*
 * usage
 * Functionality : Print command-line usage
 * Return Value : None
 * Parameter :
 * 	st - the program name
 */
void usage(char *st) {
	printf("Syntax : %s <option> <port>\n",st);
	printf("Valid option is\n");
	printf(" p <port>   : Server port\n");
	printf(" ?          : print this help message\n");
}

/*
 * join
 * Functionality : Handler for JOIN
 * Return Value : 1 if success, otherwise -1
 * Parameter : 
 *		fd - socket descriptor
 *		node_queue - the look-up table
 */
int join(int fd, fifo_t *node_queue) {
	int  node_id, key_no, size, in_size;
	int *buffer, *ptr, i, name_len;
	fifo_queue_t *qq;
	node_t *node;
	struct sockaddr_in name;

	name_len = sizeof(struct sockaddr);
	if(getpeername(fd, (struct sockaddr*)&name, &name_len)<0){
		  printf("Server: getpeername error\n");
		  return -1;
	}

	/* Receive node_id */
	if(readn(fd, &node_id, sizeof(int))<0) {
		perror("Server:readn error\n");
		return -1;
	}     

	// check if the client already join 
	if( (qq = FIFO_remove2(node_queue, CB_search_node_queue, 
					(void *)&node_id)) != NULL ) {
		// Free the keys for the node
		printf("Node %d re-enter.\n", node_id);
		node = (node_t *)qq->ptr;
		FIFO_foreach(node->keys, CB_free_key);
		FIFO_destroy(node->keys);
		free(node->hostname);
		free(node);
		free(qq);
	}

	/* Receive number */
	if(readn(fd, &key_no, sizeof(int))<0) {
		perror("Server:readn error\n");
		return -1;
	}     

	/* Receive key */
	buffer = (int *)malloc(key_no*sizeof(int));
	ptr = buffer;
	if(readn(fd, buffer, key_no*sizeof(int))<0) {
		perror("Server:readn error\n");
		return -1;
	}     

	// Create a new queue for the joining node 
	node = (node_t *)malloc(sizeof(node_t));
	node->node_id = node_id;
	node->hostname = (char *)malloc(20);
	sprintf(node->hostname, "%s", inet_ntoa(name.sin_addr));
	node->keys = FIFO_create();
	printf("Node %d (%s) joined and sent %d keys.\n", 
			node_id, node->hostname, key_no);
	printf("keys: ");
	for(i=0; i<key_no; i++) {
		int *tmp = (int *)malloc(sizeof(int));

		*tmp = *(ptr)++;
		FIFO_enqueue(node->keys, FIFO_element(tmp));
		printf("%d ", *tmp);
	}
	printf("\n");
	FIFO_enqueue(node_queue, FIFO_element(node));

	printf("Numbers of Clients = %d\n", FIFO_count(node_queue));

	free(buffer);
	return 0;
}

/*
 * leave 
 * Functionality : Handler for LEAVE 
 * Return Value : 1 if success, otherwise -1
 * Parameter : 
 *		fd - socket descriptor
 *		node_queue - the look-up table
 */
int leave(int fd, fifo_t *node_queue) {
	int	node_id;
	fifo_queue_t *qq;
	node_t *node;

	// Receive node_id 
	if(readn(fd, &node_id, sizeof(int))<0) {
		perror("Server:readn error\n");
		return -1;
	}     

	// Remove the node from node_queue
	if( (qq = FIFO_remove2(node_queue, CB_search_node_queue, 
					(void *)&node_id)) != NULL ) {
		// Free the keys for the node
		node = (node_t *)qq->ptr;
		FIFO_foreach(node->keys, CB_free_key);
		FIFO_destroy(node->keys);
		free(node->hostname);
		free(node);
		free(qq);
	}

	printf("Node %d left.\n", node_id);
	printf("Number of client = %d\n", FIFO_count(node_queue));

	return 0;
}

/*
 * search 
 * Functionality : Handler for SEARCH 
 * Return Value : 1 if success, otherwise -1
 * Parameter : 
 *		fd - socket descriptor
 *		node_queue - the look-up table
 */
int search(int fd, fifo_t *node_queue) {
	int  node_id, key, result;
	fifo_queue_t *qq;
	node_t *node;

	// Receive node_id 
	if(readn(fd, &node_id, sizeof(int))<0) {
		perror("Server:readn error\n");
		return -1;
	}     

	// Receive number
	if(readn(fd, &key, sizeof(int))<0) {
		perror("Server:readn error\n");
		return -1;
	}     

	// Search for the key
	result = -1;
	if( (qq = FIFO_search2(node_queue, CB_search_key, 
					(void *)&key)) != NULL ) {
		node = (node_t *)qq->ptr;
		result = node->node_id;
	}

	// Return search result
	if( writen(fd, &result, sizeof(int))<0 ) {
		printf("Server: writen error\n");
		return(-1);     
	}


	if(result != -1) {
		int size;

		size = strlen(node->hostname);
		if( writen(fd, &size, sizeof(int)) < 0 ) {
			printf("Server: writen error\n");
			return(-1);     
		}
		if( writen(fd, node->hostname, size) < 0 ) {
			printf("Server: writen error\n");
			return(-1);     
		}

		printf("Node %d searched for the key %d and found on the client %d.\n", 
				node_id, key, result);
	} else {
		printf("Node %d searched for the key %d and don't find\n\n ", 
				node_id, key);
	}

	return 0;
}

/*
 * terminate 
 * Functionality : Handler for EXIT (Terminate the server gracefully) 
 * Return Value : 1 if success, otherwise -1
 * Parameter : 
 *		fd - socket descriptor
 *		node_queue - the look-up table
 */
int terminate(int fd, fifo_t *node_queue) {
	// Free allocated resource
	exit(0);
}
