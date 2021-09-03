/*
 * client.c : P2P client
 */

#include "config.h"
#include "fifo.h"
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


#define PORT 6001
#define LISTENING_PORT 6022
#define GETDATA 10

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
void *thr_func(void *ptr);
int perform(int fd);
int get_data(int fd);
int listening_post();
void create_input_window_thread();
void *input_window_thread(void *ptr);
int request_data(char* m_name, int nodeid, int key);

char *machine_name;
int server_port;
int node_id;
fifo_t *key_list;

int main(int argc, char **argv) {
	char opt, *delim="d:p:v?";
   int i;

	server_port=PORT;
	while((opt = getopt(argc,argv,delim)) != EOF) {
		switch(opt) {
			case 'd' : node_id = atoi(optarg); break;
			case 'p' : server_port = atoi(optarg); break;
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

	// Demonstrate how to transmit data via the protocol
	join(machine_name, server_port, node_id, key_list);
       
	listening_post();	
}

int listening_post() {
	struct	sockaddr_in cli_addr, serv_addr;
	int		sockfd, newsockfd;
	int		status, port, clilen;
	fd_set 	watchset;
	struct 	timeval tv;
 
	port = LISTENING_PORT;

	// Open a TCP socket (an Internet stream socket.)
	if( (sockfd=socket(AF_INET,SOCK_STREAM,0))<0 ) {
		perror("Server-Client:can't open stream socket\n");
		exit(1);
	}

	// Bind our local address so that the client can send to us.
	bzero((char *) &serv_addr,sizeof(serv_addr));
	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port	     = htons(port);
	if( bind(sockfd,(struct sockaddr *) &serv_addr,
				sizeof(serv_addr))<0) {
		perror("Server-Client: can't bind local address\n");
		exit(1);
	}
	listen(sockfd,5);

	FD_ZERO(&watchset);
	FD_SET(sockfd, &watchset);
	tv.tv_sec = 10;
	tv.tv_usec = 0;
 
	create_input_window_thread();
         
	for(;;) {
		pthread_t *th;
		int *para;

		if(select(sockfd+1, &watchset, NULL, NULL, &tv) < 0) {
			perror("Server: select error\n");
			return -1;
		}

		if(FD_ISSET(sockfd, &watchset) != 0) {
			clilen=sizeof(cli_addr);
			if((newsockfd = accept(sockfd, (struct sockaddr *) 
						&cli_addr,&clilen)) < 0) {
				perror("Server: accept error\n");
				return -1;
			}

			th = (pthread_t *)malloc(sizeof(pthread_t));
			para = (int *)malloc(sizeof(int));
			*para = newsockfd;
		
			pthread_create(th, NULL, thr_func, (void *)para);
		} else {
			FD_SET(sockfd, &watchset);
		}
	}
}

void *thr_func(void *ptr) {
	int sockfd = *((int *) ptr);
  
	perform(sockfd);
	close(sockfd);
	free(ptr);
	return 0;
}

int perform(int fd) {
	short opcode;
  
	if (readn(fd, &opcode, sizeof(short))<0) {
		perror("Client: readn (at function perform) error\n");
      return -1;
    }
	
	switch(opcode) {
      case GETDATA: get_data(fd);    
	}
}

int send_data(int fd, char *value) {
	int *sz;
	char *data;

	sz = (int* ) malloc (sizeof(int)); 
	*sz = strlen(value);
	data = (char *) malloc ((*sz) *sizeof(char));
	strncpy(data,value,*sz);

	if (writen(fd, sz, sizeof(int))<0 ) {
		perror ("client: writen error \n");
		return(-1);
	}

	if (writen(fd, data , (*sz)*sizeof (char))<0  ) {
		perror("Client: writen error \n");
		return (-1);
	}
}

int get_data(int fd) {
	char *data;
	int *sz; 
	keys_t *object;
	int key_no;

	if (readn(fd, &key_no, sizeof(int))<0) {
		perror("Client: readn (at function get_data) error\n");
		return -1;
	}
	printf("\n>>>>> Incoming request for a key: %d\n",key_no);
  
	// Search local DB
	if((object = search_local_db(key_no, key_list)) != NULL) {
		printf("Title = %s\n", object->title); 
		printf("ARTIST = %s\n", object->artist); 
		printf("COUNTRY = %s\n", object->country); 
		printf("COMPANY = %s\n", object->company);
		printf("YEAR = %s\n", object->year);
		printf("\n");
  
		send_data(fd,object->title);
		send_data(fd,object->artist);
		send_data(fd,object->country);
		send_data(fd,object->company);
		send_data(fd,object->year);
  
		printf(">>>>> The requested packet has been sent ....... \n");
		printf("Command shell [s/x] : ");
	}
}

void create_input_window_thread() {
	pthread_t *th;
	int *para;

	para = (int *) malloc (sizeof(int));
	th = (pthread_t *) malloc(sizeof(pthread_t));
	pthread_create(th,NULL,input_window_thread,(void *) para);  
}

void *input_window_thread(void *ptr) {
	char command[3];
	int wantedkey;
	search_result_t *result;
	
	for (;;) {
		printf("Command shell [s/x] : ");
		scanf("%c",command);
		switch(command[0]) {
			case 's':
				printf("Search for: ");
				scanf("%d",&wantedkey);
				if( (result = search(machine_name, server_port, 
								node_id, wantedkey)) != NULL) {
					printf("%d is in the client %d (%s)\n", wantedkey, 
							result->node_id, result->hostname);
					request_data(result->hostname, result->node_id, 
							wantedkey); 
				} else {
					printf("The key cant be found ..... \n");
				}
				break;
			case 'x':
				leave(machine_name, server_port, node_id);
				printf("Thank you for using our program ..... \n");
				printf("Please dont make any segmentation fault !!!!, >:( \n");
				exit(0);
			default: 
				printf("s = search ; x = leave \n");  
		} 
	}

	return 0;
}

int receive_data(int sockfd, char* note) {
	int sz;
	char *data;

	if ( readn(sockfd, &sz, sizeof(int))<0) {
		perror("Client: readn error \n");	
	}

	data = (char *) malloc (sz * sizeof(char));
	if ( readn(sockfd, data, sz * sizeof(char))<0) {
		perror("Client: read error - search \n");
		return(-1);
	}
	data[sz]='\0';
	printf("%s : %s \n",note,data);
}

int request_data(char* m_name, int nodeid, int key) {
	short opcode = GETDATA;
	int sockfd = tcp_open(m_name, LISTENING_PORT); 
	int key_no = key;
        
	printf(">>>>> Trying to connect %s to request my data ..... \n",
			m_name); 

	if (sockfd<0) {
		printf(">>>>> Failed to connect the host machine ..... \n");
		printf(">>>>> Tell the server the host machine is down .... \n"); 
		dead(machine_name, server_port, nodeid);
		return(-1);
	}
       
	// Send opcode
	if( writen(sockfd, &opcode, sizeof(short))<0 ) {
		perror("Client: writen error\n");
		return(-1);
	}   

	// Send the key
	if( writen(sockfd, &key_no, sizeof(int))<0 ) {
		perror("Client: writen error\n");
		return(-1);      
	}

   receive_data(sockfd,"Title");
   receive_data(sockfd,"ARTIST");
   receive_data(sockfd,"COUNTRY");
   receive_data(sockfd,"COMPANY");
   receive_data(sockfd,"YEAR");
	close(sockfd); 
}

// ------------------- Sub-routines ----------------------------------

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
