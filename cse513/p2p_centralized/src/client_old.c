/*
 * client.c : P2P client
 */
#include <stdio.h>
#include <unistd.h>
#include "config.h"
#include "fifo.h"

#define PORT 6001

typedef struct search_result_t search_result_t;
struct search_result_t {
	int node_id;
	char *hostname;
};

// Prototype
int read_data2(char *filename, fifo_t *key_list);
void printbuffer(int *ptr, int count);
void usage(char *command);
void CB_marshal_key(fifo_queue_t *qq, void *arg);
int CB_search_local_db(fifo_queue_t *qq, void *key); 
keys_t *search_local_db(int key, fifo_t *key_list);
int join(char *machine_name, int port, int node_id, fifo_t *key_list); 
search_result_t *search(char *machine_name, int port, 
		int node_id, int key);
int leave(char *machine_name, int port, int node_id);
int dead(char *machine_name, int port, int node_id);
int leave_or_dead (short opcode, char *machine_name, 
		int port, int node_id);

int main(int argc, char **argv) {
	fifo_t *key_list;
	char opt, *delim="d:p:v?";
	char *machine_name;
	int port, node_id; 
	search_result_t *result;
	keys_t *object;

	port=PORT;
	while((opt = getopt(argc,argv,delim)) != EOF) {
		switch(opt) {
			case 'd' : node_id = atoi(optarg); break;
			case 'p' : port = atoi(optarg); break;
			case '?' : 
			default  : usage(argv[0]); exit(0);
		}
	}

	if( (argc-optind)!=2 ) {
		usage (argv[0]);
		exit(1);
	}
	machine_name = argv[optind];

	key_list = FIFO_create();
	read_data(argv[optind+1], key_list);

	// Search local DB
	if((object = search_local_db(8, key_list)) != NULL) {
		printf("Title = %s\n", object->title); 
		printf("ARTIST = %s\n", object->artist); 
		printf("COUNTRY = %s\n", object->country); 
		printf("COMPANY = %s\n", object->company);
		printf("YEAR = %s\n", object->year);
		printf("\n");
	}

	// Demonstrate how to transmit data via the protocol
	join(machine_name, port, node_id, key_list);

	if( (result = search(machine_name, port, node_id, 15)) != NULL) {
		printf("15 is in the client %d (%s)\n", result->node_id, 
				result->hostname);
	}

	if( (result = search(machine_name, port, node_id, 25)) != NULL) {
		printf("25 is in the client %d (%s)\n", result->node_id, 
				result->hostname);
	}

	if( (result = search(machine_name, port, node_id, 35)) != NULL) {
		printf("35 is in the client %d (%s)\n", result->node_id, 
				result->hostname);
	}

	if( (result = search(machine_name, port, node_id, 45)) != NULL) {
		printf("45 is in the client %d (%s)\n", result->node_id, 
				result->hostname);
	}

	leave(machine_name, port, node_id);
}

// ------------------- Sub-routines ----------------------------------
/*
 * read_data2
 * Functionality : Read the list of keys and associated data, then
 *		keep them in the key_list. For testing only. (Deprecated)
 * Return Value :  1 if successful, -1 otherwise.
 * Parameter :
 *		filename - filename
 *		key_list - the FIFO_queue to keep the keys and their 
 *			associated data
 */
int read_data2(char *filename, fifo_t *key_list) {
	int  count, tmp, *ptr;
	FILE *fp;
	char buffer[1024], data[1024];

	if( (fp=fopen(filename,"r")) == NULL) {
		puts("Cannot open file\n");
		return -1;
	}
	
	count = 0;
	while( fscanf(fp,"%d", &tmp) != EOF) {
		ptr = (int *)malloc(sizeof(int));
		*ptr = tmp;
//		printf("%d ", tmp);
		FIFO_enqueue(key_list, FIFO_element(ptr)); 
		count++;
	}   
//	printf("\n");

	fclose(fp);
	return(count);
}

/* 
 * usage
 * Functionality : Print help message
 * Return Value : NONE
 * Parameter :
 *		command - command
 */
void usage(char *command) {
	printf("usage : %s <server> <input file>\n", command);
	printf("Valid option is\n");
	printf(" p <port> : connect server at the specified port\n");
	printf(" v        : print version of %s\n", command);
	printf(" ?        : print this help message\n");
}

/*
 * CB_marshal_key
 * Functionality : Arrange the keys in the key_list so as to be able
 *		to transfer to the server during joining 
 * Return Value : NONE
 * Parameter :
 *		qq - an element in the key_list table
 *		arg - the pointer to the position to be written to in an array
 */
void CB_marshal_key(fifo_queue_t *qq, void *arg) {
	int *pointer = (int *)*((int *)arg);
	keys_t *value;

	if(qq->ptr != NULL) {
		value = (keys_t *)qq->ptr;
		*pointer = value->key;
		*((int *)arg) += 4;
	}
}

int CB_search_local_db(fifo_queue_t *qq, void *key) {
	keys_t *object;

	if(qq->ptr != NULL) {
		object = (keys_t *)qq->ptr;

		if(object->key == *((int *)key))
			return 1;
		else 
			return 0;
	}
}

keys_t *search_local_db(int key, fifo_t *key_list) {
	fifo_queue_t *qq;

	if((qq = FIFO_search2(key_list, CB_search_local_db, 
					(void *)&key)) != NULL) 
		return (keys_t *)qq->ptr;
	else 
		return NULL;
}

/* join 
	Functionality: register the client to the server and publish the keys
		of data that are hosted by the client.
	Return Value: 1 if successful, otherwise -1
	Parameter:
		machine_name 	- server name
		port				- server port
		node_id			- client's node ID
		key_list			- 
*/
int join(char *machine_name, int port, int node_id, fifo_t *key_list) {
	short opcode = JOIN;
	int sockfd = tcp_open(machine_name, port); 
   int number,*buffer, *ptr;

	number = FIFO_count(key_list);

	// Send opcode
	if( writen(sockfd, &opcode, sizeof(short))<0 ) {
		printf("Client: writen error\n");
		return(-1);
	}   

	// Send node_id
	if( writen(sockfd, &node_id, sizeof(int))<0 ) {
		printf("Client: writen error\n");
		return(-1);     
	}

   // Send the number of keys
	if( writen(sockfd, &number, sizeof(int))<0 ) {
		printf("Client: writen error\n");
		return(-1);      
	}

	// Send the list of keys
	buffer = (int *)malloc(number*sizeof(int));
	ptr = buffer;
	FIFO_foreach2(key_list, CB_marshal_key, (void *)&ptr); 

#ifdef __DEBUG__
	{ int i, *buffer2;
		buffer2 = buffer;
		for(i=0; i<number; i++) 
			printf("%d ", *(buffer2)++);
		printf("\n");
	}
#endif

	if( writen(sockfd, buffer, number*sizeof(int))<0 ) {
		printf("Client: writen error\n");
		return(-1);      
	}

	close(sockfd); 
	return(0);
}

/* search 
	Functionality: search for a data with associated key
	Return Value: Node Id and the hostname of the client 
		hosting the data if found, otherwise NULL
	Parameter:
		machine_name 	- server name
		port				- server port
		node_id			- client's node ID
		key				- the key to be searched for
*/
search_result_t * search(char *machine_name, int port, 
		int node_id, int key) {
	short opcode = SEARCH;
	int sockfd = tcp_open(machine_name, port); 
	int result;

	// Send opcode
	if( writen(sockfd, &opcode, sizeof(short))<0 ) {
		printf("Client: writen error\n");
		return NULL;
	}   

	// Send node_id
	if( writen(sockfd, &node_id, sizeof(int))<0 ) {
		printf("Client: writen error\n");
		return NULL;
	}

   // Send the key
	if( writen(sockfd, &key, sizeof(int))<0 ) {
		printf("Client: writen error\n");
		return NULL;
	}

	// Read the result back from the server
	if( readn(sockfd, &result, sizeof(int))<0 ) {
		printf("Client: writen error\n");
		return NULL;
	}

	// Read the hostname 
	if(result != -1) {
		search_result_t *res;
		int size;
		
		res = (search_result_t *)malloc(sizeof(search_result_t));
		res->node_id = result;

		// Read the size
		if( readn(sockfd, &size, sizeof(int))<0 ) {
			printf("Client: writen error\n");
			return NULL;
		}

		// Read the hostname
		res->hostname = (char *)malloc(size);
		if( readn(sockfd, res->hostname, size)<0 ) {
			printf("Client: writen error\n");
			return NULL;
		}

		return res;
	}
	
	return NULL;
}

/*
 * leave
 * Functionality : Send a LEAVE message
	Return Value : 1 if successful, otherwise -1
	Parameter :
		machine_name 	- server name
		port				- server port
		node_id			- client's node ID
 */
int leave(char *machine_name, int port, int node_id) {
	return leave_or_dead(LEAVE, machine_name, port, node_id);
}

/*
 * dead 
 * Functionality : Send a DEAD message to notify the server of the 
 * 	death of a client
 * Return Value : 1 if successful, otherwise -1
	Parameter :
		machine_name 	- server name
		port				- server port
		node_id			- client's node ID
 */
int dead(char *machine_name, int port, int node_id) {
	return leave_or_dead(DEAD, machine_name, port, node_id);
}

/* leave_or_dead
	Functionality : unregister the client from the server
	Return Value : 1 if successful, otherwise -1
	Parameter :
		opcode			- operation (DEAD or LEAVE);
		machine_name 	- server name
		port				- server port
		node_id			- client's node ID
 */
int leave_or_dead (short opcode, char *machine_name, 
		int port, int node_id) {
	int sockfd = tcp_open(machine_name, port); 
	int result;

	// Send opcode
	if( writen(sockfd, &opcode, sizeof(short))<0 ) {
		printf("Client: writen error\n");
		return(-1);
	}   

	// Send node_id
	if( writen(sockfd, &node_id, sizeof(int))<0 ) {
		printf("Client: writen error\n");
		return(-1);     
	}
	
	return 0;
}
