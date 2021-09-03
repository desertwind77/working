/*
 * p2p.c : Skeleton multi-thread server
 * By: Athichart Tangpong
 *
 * Problems:
 *		1. The server can serve only 2037 connections. The server 
 *			can not create more than 2037 threads.
 *		2. There is segmentation fault.
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
 
#define PORT 					9091		// Default port
#define M						3			// Number of bits
#define ADDR_LEN				30
// Opcodes of the protocol
#define GET_NODE				100
#define GET_PRED				101
#define GET_SUCC				102
#define SET_PRED				103
#define SET_SUCC				104
#define CLOSET					105		
#define UPDATE					106
#define PROBE					107
#define PROBE_RET				108
#define GET_SUCC_KEY 		109
#define SEND_KEY				110
#define DIST_KEY_HOST		111
#define DIST_KEY_OTHER		112
#define SEARCH_KEY      	113
#define ASK_FINGER_TABLE 	114
#define SEND_FINGER_TABLE 	115

#define SOCK_TIMEOUT_SEC 10
#define SOCK_TIMEOUT_USEC 0

#define UPDATE_TIMEOUT_SEC 15

typedef struct node_t node_t;
struct node_t {
	int id;
	char address[ADDR_LEN];
};

typedef struct finger_table_t finger_table_t;
struct finger_table_t {
	int start;
	int int_start;
	int int_end;
	node_t node;
};

// Global variable 
extern char *optarg;
extern int errno;
finger_table_t finger_table[M+1];	// Finger table
node_t *my_node;				// The client's info
node_t *my_pred;				// My predecessor
char *machine_name = NULL;	// The machine with which the client 
									// is going to join
char *data_file;				// The local database
int  port;						// Port at which clients communicate  
int  update_timeout = UPDATE_TIMEOUT_SEC; // Period to update the finger table
int  terminated = 0;
int  joined = 0;
int my_data[100];

// Prototype 
void * thr_func(void *ptr);
void usage(char *st); 
int perform(int fd); 
void* main_func(void *ptr);
void* update_func(void *ptr); 
int pow(int base, int power);
int in_interval_include_exclude(int begin, int end, int check); 
int in_interval_exclude_exclude(int begin, int end, int check); 
int in_interval_exclude_include(int begin, int end, int check); 
int send_node_info(int fd, node_t *node); 
int request_node_info(int fd, node_t *node); 
int find_successor_handler(int fd); 
int closet_preceding_finger_handler(int fd); 
int update_finger_table_handler(int fd); 
int get_successor(node_t *source, node_t *node); 
int get_predecessor(node_t *source, node_t *node); 
int set_predecessor(node_t *source, node_t *input); 
int set_successor(node_t *source, node_t *input);
int find_successor(node_t *source, int id, node_t *output); 
void find_predecessor(int key, node_t *output); 
int  closet_preceding_finger(node_t *src, node_t *des, int key);
void init_finger_table(char *host); 
void update_others(); 
int update_finger_table(node_t *host, node_t *s, int index); 
void print_finger(); 
void help();
void leave();
void probe(); 
void probe_handler(int fd); 
void probe_return();
void probe_return_handler(int fd);
int send_key(node_t *target, int key);
void distribute_my_key();
int ask_the_host_to_distribute_its_key();
int send_ack(int fd, int ack);
int ask_other_to_distribute_their_keys(node_t *source);
int get_DIST_KEY_OTHER(int fd);
void print_key();
int get_ask_key(int fd);
int ask_key(node_t *source, int key, int get_key);
int search_key(int key, int get_key);
int ask_other_to_send_their_finger_tables(node_t *source);
int send_my_finger_table(node_t *target);
int get_other_finger_table(int fd);
int get_ASK_FINGER_TABLE(int fd);
int send_keys_to_my_successor();

int main(int argc,char **argv) {
	struct	sockaddr_in cli_addr, serv_addr;
	int		sockfd, newsockfd;
	int		status, clilen;
	char		*delim="s:p:d:m:?";
	char		opt;
	fd_set 	watchset;
	struct 	timeval tv;
	pthread_t main_thr, update_thr;
	int 		i;

	port = PORT;
	my_node = (node_t *)malloc(sizeof(node_t)); 
	my_pred = (node_t *)malloc(sizeof(node_t)); 
	while((opt = getopt(argc,argv,delim)) != EOF) {
		switch(opt) {
			case 'p' : port = atoi(optarg); break;
			case 'd' : my_node->id = atoi(optarg); break;
			case 'm' : machine_name = (char *)strdup(optarg); break;
			case 's' : update_timeout = atoi(optarg); break;
			case '?' : 
			default  : usage(argv[0]); exit(0);
		}
	}

	if (my_node->id == 1) {
		if ((argc-optind) != 1) {
			usage(argv[0]);
			exit(1);
		}
		data_file = argv[optind];
	} else {
		if (machine_name == NULL) {
			usage(argv[0]);
			exit(1);
		}
	}

	for (i=0; i<100; i++) my_data[i] = 0;
	create_key_list();
	if (my_node->id == 1) read_file(data_file);

	if (gethostname(my_node->address, ADDR_LEN) < 0) {
		printf("Error: unable to get hostname\n");
		exit(1);
	}
	
	if (pthread_create(&main_thr, NULL, main_func, NULL) != 0) {
		printf("Error : unable to create main thread\n");
		exit(1);
	}
	pthread_detach(main_thr);

	if (my_node->id == 1) {
		if (pthread_create(&update_thr, NULL, update_func, NULL) != 0) {
			printf("Error : unable to create update thread\n");
			exit(1);
		}
		pthread_detach(update_thr);
	}

	// Open a TCP socket (an Internet stream socket.)
	if( (sockfd=socket(AF_INET,SOCK_STREAM,0))<0 ) {
		perror("Error : unable 't open stream socket\n");
		exit(1);
	}

	// Bind our local address so that the client can send to us.
	bzero((char *) &serv_addr,sizeof(serv_addr));
	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port	     = htons(port);
	if( bind(sockfd,(struct sockaddr *) &serv_addr,
				sizeof(serv_addr))<0) {
		perror("Error : unable bind local address\n");
		exit(1);
	}
	listen(sockfd,5);

	FD_ZERO(&watchset);
	FD_SET(sockfd, &watchset);
	tv.tv_sec = SOCK_TIMEOUT_SEC;
	tv.tv_usec = SOCK_TIMEOUT_USEC;

	while(!terminated) {
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
	}	/* while() */

	close(sockfd);
	printf("Close\n");
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
 * usage
 * Functionality : Print command-line usage
 * Return Value : None
 * Parameter :
 * 	st - the program name
 */
void usage(char *st) {
	printf("Syntax : %s <option> <data file>\n",st);
	printf("Valid option is\n");
	printf(" p <port>   : Server port\n");
	printf(" d <ID>     : The client ID\n");
	printf(" m <machine>: The machine with which the client is joining\n");
	printf(" s <period> : The period of detecting dead nodes\n");
	printf(" ?          : print this help message\n");
}

/*
 * perform
 * Functionality : Connection handler
 * Return Value : 1 if success, otherwise -1
 *	Parameter :
 *		fd - socket descriptor
 */
int perform(int fd) {
	int opcode;

	/* Receive opcode */
	if(readn(fd, &opcode, sizeof(int))<0) {
		perror("Server: readn error\n");
		return -1;
	}     

	switch(opcode) {
		case CLOSET : closet_preceding_finger_handler(fd); break;
		case GET_NODE : return send_node_info(fd, my_node);
		case GET_SUCC : return send_node_info(fd, 
									 &(finger_table[1].node));
		case GET_PRED : return send_node_info(fd, my_pred);
		case GET_SUCC_KEY : find_successor_handler(fd); break;
		case SET_PRED : return request_node_info(fd, my_pred);
		case SET_SUCC : return request_node_info(fd, 
									 &(finger_table[1].node));
		case UPDATE : update_finger_table_handler(fd); break;
		case PROBE : probe_handler(fd); break;
		case PROBE_RET : probe_return_handler(fd); break;
		case DIST_KEY_HOST : distribute_my_key(); send_ack(fd,1); break; 
		case SEND_KEY : get_key(fd); break;
		case DIST_KEY_OTHER : get_DIST_KEY_OTHER(fd); break;
		case SEARCH_KEY : get_ask_key(fd); break;
		case ASK_FINGER_TABLE: get_ASK_FINGER_TABLE(fd); break;
		case SEND_FINGER_TABLE: get_other_finger_table(fd); break;
	}

	return 1;
}

void* main_func(void *ptr) {
	char ch;
	node_t node;
	int key;

	while (!terminated) {
		printf("%s> ", my_node->address); 
		ch = getchar();
		if ((ch < 'a') || (ch > 'z')) continue;
		switch(ch) {
			case 'h' : help(); break;
			case 'j' : if(!joined) join(machine_name); break;
			case 'p' : print_finger(); break;
			case 'x' : 
				if(my_node->id != 1) leave(); 
				terminated = 1;  
				break;
			case 'k' : print_key(); break;
			case 's' : 
				printf("%s> search for key number: ",my_node->address);
				scanf("%d",&key);
				search_key(key,0); 
				break;
			case 'g' : 
				printf("%s> get data for key number: ",my_node->address);
				scanf("%d",&key);
				search_key(key,1); 
				break;
			case 'a' : 
				ask_other_to_send_their_finger_tables(my_node); 
				break; 
			case 't' :
				scanf("%d", &key); 
				find_successor(my_node, key, &node);
				printf("The successor of %d is %d (%s)\n", key, 
						node.id, node.address);
				break;
		}
	}
}

void help() {
	printf("Available comamnds:\n");
	printf("g : get the data associated with a key\n");
	printf("h : print this help message\n");
	printf("j : join the specified node (the option -m)\n");
	printf("p : print the finger tables of all nodes\n");
	printf("s : search for a key\n");
	printf("x : terminate the program gracefully\n");
	printf("k : print the list of hosted keys\n");
	printf("%s> ", my_node->address); 
}

int pow(int base, int power) {
	int i, product = 1;

	for (i=0; i<power; i++)
		product *= base;

	return product;
}

/*
 * in_interval : Check if check is in (begin, end).
 * PARAMETER :
 *		begin - the beginning of the interval
 *		end 	- the end of the interval
 *		check	- what to be checked
 */
int in_interval_include_exclude(int begin, int end, int check) {
	int i;

	i = begin;
	do {
		if (i == check) return 1;
		i = (i + 1) % pow(2, M);
	} while (i != end);

	return 0;
}

int in_interval_exclude_include(int begin, int end, int check) {
	int i;

	i = (begin + 1) % pow(2, M);
	end = (end + 1) % pow(2, M);
	do {
		if (i == check) return 1;
		i = (i + 1) % pow(2, M);
	} while (i != end);

	return 0;
}

int in_interval_exclude_exclude(int begin, int end, int check) {
	int i;

	i = (begin + 1) % pow(2, M);
	do {
		if (i == check) return 1;
		i = (i + 1) % pow(2, M);
	} while (i != end);

	return 0;
}

/*
 * send_node_info : Send the info of a node to a client 
 */
int send_node_info(int fd, node_t *node) {
	if( writen(fd, &(node->id), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	if( writen(fd, &(node->address), ADDR_LEN) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	return 0;
}

/*
 * request_node_info : Request the info of a node
 */
int request_node_info(int fd, node_t *node) {
	if( readn(fd, &(node->id), sizeof(int)) < 0) {
		perror("Server: readn error\n");
		return -1;
	}

	if( readn(fd, &(node->address), ADDR_LEN) < 0) {
		perror("Server: readn error\n");
		return -1;
	}

	return 0;
}

/*
 *
 */
int closet_preceding_finger_handler(int fd) {
	int i, id;

	if (readn(fd, &id, sizeof(int)) < 0) {
		printf("Error: readn error\n");
		return -1;
	}

	for (i=M; i>0; i--)  {
		if (in_interval_exclude_exclude(my_node->id, id,
					finger_table[i].node.id)) {
			send_node_info(fd, &(finger_table[i].node));
			return 0;
		}
	}
}

/*
 *
 */
int find_successor_handler(int fd) {
	node_t tmp_node, output;
	int id;

	if (readn(fd, &id, sizeof(int)) < 0) {
		printf("Error: readn error\n");
		return -1;
	}

	find_predecessor(id, &tmp_node);
	get_successor(&tmp_node, &output);
	send_node_info(fd, &output);

	return 0;
}


// -------------------------- Client's code ------------------------
int get_node(char *host, node_t *node) {
	int opcode = GET_NODE;
	int fd = tcp_open(host, port);	
	
	if (fd <0) return -1;

	if( writen(fd, &(opcode), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	request_node_info(fd, node);
	close(fd);
	return 0;
}

/*
 *
 */
int get_successor(node_t *source, node_t *node) {
	int opcode = GET_SUCC;
	int fd = tcp_open(source->address, port);	

	if (fd <0) return -1;

	if( writen(fd, &(opcode), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	request_node_info(fd, node);
	close(fd);
	return 0;
}

/*
 *
 */
int get_predecessor(node_t *source, node_t *node) {
	int opcode = GET_PRED;
	int fd = tcp_open(source->address, port);	

	if (fd <0) return -1;

	if( writen(fd, &(opcode), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	request_node_info(fd, node);
	close(fd);
	return 0;
}

int set_predecessor(node_t *source, node_t *input) {
	int opcode = SET_PRED;
	int fd = tcp_open(source->address, port);	

	if (fd <0) return -1;

	if( writen(fd, &(opcode), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	send_node_info(fd, input);
	close(fd);
	return 0;
}

int set_successor(node_t *source, node_t *input) {
	int opcode = SET_SUCC;
	int fd = tcp_open(source->address, port);	

	if (fd <0) return -1;

	if( writen(fd, &(opcode), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	send_node_info(fd, input);
	close(fd);
	return 0;
}

/*
 * find_successor 
 */
int find_successor(node_t *source, int id, node_t *output) {
	int opcode = GET_SUCC_KEY;
	int fd = tcp_open(source->address, port);	

	if (fd <0) return -1;

	if( writen(fd, &(opcode), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	if( writen(fd, &(id), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	request_node_info(fd, output);
	close(fd);
	return 0;
}

/*
 * find_predecessor 
 */
void find_predecessor(int key, node_t *output) {
	node_t node, succ, tmp;

	node.id = my_node->id;
	sprintf(node.address,"%s", my_node->address);
	get_successor(&node, &succ);
//	printf("Find_pred_outloop: S=%d E=%d K=%d\n", node.id, succ.id, key);
	while (!in_interval_exclude_include(node.id, succ.id, key)) {
//		printf("Find_pred_loop: S=%d E=%d K=%d\n", node.id, succ.id, key);
		closet_preceding_finger(&node, &tmp, key); 
		node.id = tmp.id;
		sprintf(node.address, "%s", tmp.address);
		get_successor(&node, &succ);
//		printf("Find_pred_loop: S=%d E=%d K=%d\n", node.id, succ.id, key);
	}

	output->id = node.id;
	sprintf(output->address, "%s", node.address);
}

/*
 * closet_preceding_finger
 */
int closet_preceding_finger(node_t *src, node_t *des, int key) {
	int fd = tcp_open(src->address, port);
	int opcode = CLOSET;

	if (fd <0) return -1;

	if( writen(fd, &opcode, sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	if( writen(fd, &key, sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	request_node_info(fd, des);
	close(fd);
	return 0;
} 

int join(char *host) {
	node_t node;
	int i;

	for (i=1; i<=M; i++) {
		finger_table[i].start = (my_node->id + pow(2, i-1)) % pow(2, M);
		finger_table[i].int_start = finger_table[i].start;
		finger_table[i-1].int_end = finger_table[i].int_start;
	}
	finger_table[M].int_end = finger_table[1].start - 1;
	if (finger_table[M].int_end < 0) {
		finger_table[M].int_end += pow(2, M);
	}

	if (host != NULL) {
		// This client is going to join the network. And host is 
		// an arbitary node in the network
		get_node(host, &node);
		init_finger_table(host);
		update_others();

		for (i=1; i<M; i++) {
			if (in_interval_include_exclude(my_node->id, 
						finger_table[i].node.id, finger_table[i+1].start)) {
				finger_table[i+1].node.id = finger_table[i].node.id; 
				sprintf(finger_table[i+1].node.address, "%s", 
						finger_table[i].node.address); 
			} else {
				find_successor(&node, finger_table[i+1].start, 
						&(finger_table[i+1].node));
			}
		}

		ask_the_host_to_distribute_its_key();
		ask_other_to_distribute_their_keys(my_node);
	} else {
		// This client is the only node in the network
		for (i=1; i<=M; i++) {
			finger_table[i].node.id = my_node->id;
			sprintf(finger_table[i].node.address,"%s", my_node->address);
		}

		my_pred->id = my_node->id;
		sprintf(my_pred->address, "%s", my_node->address);
	}

	joined = 1;
	return 0;
}

void init_finger_table(char *host) {
	node_t node;
	int i;

	get_node(host, &node);
	find_successor(&node, finger_table[1].start, 
			&(finger_table[1].node));
	get_predecessor(&(finger_table[1].node), my_pred);
	set_predecessor(&(finger_table[1].node), my_node);

	for (i=1; i<M; i++) {
		if (in_interval_include_exclude(my_node->id, 
					finger_table[i].node.id, finger_table[i+1].start)) {
			finger_table[i+1].node.id = finger_table[i].node.id; 
			sprintf(finger_table[i+1].node.address, "%s", 
					finger_table[i].node.address); 
		} else {
			find_successor(&node, finger_table[i+1].start, 
					&(finger_table[i+1].node));
		}
	}
}

void update_others() {
	node_t node;
	int i, id;

	for (i=1; i<=M; i++) {
		id = ((my_node->id - pow(2, i-1)) + pow(2, M)) % pow(2, M);
		find_successor(my_node, id, &node);
		if (id != node.id)
			find_predecessor(id, &node);
		update_finger_table(&node, my_node, i);
	}
}

int update_finger_table(node_t *host, node_t *s, int index) {
	int opcode = UPDATE;
	int fd = tcp_open(host->address, port);

	if (fd <0) return -1;

	if( writen(fd, &opcode, sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	send_node_info(fd, s);

	if( writen(fd, &index, sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	close(fd);
	return 0;
}

int update_finger_table_handler(int fd) {
	node_t node;
	int index;

	request_node_info(fd, &node);

	if (readn(fd, &index, sizeof(int)) < 0) {
		printf("Error: readn error\n");
		return -1;
	}

	if (node.id == my_node->id) return;

	if (in_interval_include_exclude(my_node->id, 
				finger_table[index].node.id, node.id)) {
		finger_table[index].node.id = node.id;
		sprintf(finger_table[index].node.address, "%s", node.address);
		update_finger_table(my_pred, &node, index);
	}
}

void print_finger() {
	int i;

	printf("Node = %d (%s)\n", my_node->id, my_node->address);
	printf("Pred = %d (%s)\n", my_pred->id, my_pred->address);
	printf("Start\tBegin\tEnd\tID\tHost\n");
	for (i=1; i<=M; i++) {
		printf("%d\t%d\t%d\t%d\t%s\n", finger_table[i].start,
				finger_table[i].int_start, finger_table[i].int_end,
				finger_table[i].node.id, finger_table[i].node.address);
	}
}

/******************************/
/* Please write this function */ 
/******************************/
void leave() {
	// Send Key to the successor
 	send_keys_to_my_successor();
}

void* update_func(void *ptr) {
	struct timeval tv;

	tv.tv_sec = update_timeout;
	tv.tv_usec = 0;
	while (!terminated) {
		// Delay for "update_timeout" seconds
		if(select(0, NULL, NULL, NULL, &tv) < 0) {
			perror("Server: select error\n");
			terminated = 1;
			break;
		}

		if (!joined) continue;
		if (finger_table[1].node.id != my_node->id) probe();
	}
}

void probe() {
	int fd, i, opcode = PROBE;

	if ((fd = tcp_open(my_pred->address, port)) < 0) {
		// My successor is dead.
		// 1. Update finger_table
		for (i=2; i<=M; i++)
			if (finger_table[i].node.id == my_pred->id) {
				finger_table[i].node.id = my_node->id;
				sprintf(finger_table[i].node.address, "%s", 
						my_node->address);
			}

		// Send probe_ret
		if (my_pred->id != finger_table[1].node.id) {
			probe_return();
		} else {
			finger_table[1].node.id = my_pred->id = my_node->id;
			sprintf(finger_table[1].node.address, "%s", my_node->address);
			sprintf(my_pred->address, "%s", my_node->address);
		}
	} else {
		// Send a probe message via the predecessor ring
		// 1. Send the opcode
		if( writen(fd, &opcode, sizeof(int)) < 0) {
			perror("Server: writen error\n");
			return;
		}

		// 2. Send the initiator's info
		send_node_info(fd, my_node);
		close(fd);
	}
}

void probe_handler(int fd) {
	node_t node;
	int fd0, i, opcode = PROBE;

	request_node_info(fd, &node);
	// The ring is not broken.
	if (node.id == my_node->id) return;

	if ((fd0 = tcp_open(my_pred->address, port)) < 0) {
		// 1. Update finger_table
		for (i=2; i<=M; i++)
			if (finger_table[i].node.id == my_pred->id) {
				finger_table[i].node.id = my_node->id;
				sprintf(finger_table[i].node.address, "%s", 
						my_node->address);
			}

		probe_return();
	} else {
		// 1. Send the opcode
		if( writen(fd0, &opcode, sizeof(int)) < 0) {
			perror("Server: writen error\n");
			return;
		}

		// 2. Send the initiator's info
		send_node_info(fd0, &node);
		close(fd0);
	}
}

void probe_return() {
	int opcode = PROBE_RET;
	int fd = tcp_open(finger_table[1].node.address, port);

	if( writen(fd, &opcode, sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return;
	}

	// The probe_return initiator
	send_node_info(fd, my_node);
	// The dead node
	send_node_info(fd, my_pred);
}

void probe_return_handler(int fd) {
	node_t initiator, dead;
	int i, opcode = PROBE_RET;

	request_node_info(fd, &initiator);
	request_node_info(fd, &dead);

	// 1. Update finger_table
	for (i=2; i<=M; i++)
		if (finger_table[i].node.id == dead.id) {
			finger_table[i].node.id = initiator.id;
			sprintf(finger_table[i].node.address, "%s", initiator.address);
		}

	if ((finger_table[1].node.id != dead.id) && 
			((fd = tcp_open(finger_table[1].node.address, port)) > 0)) {
		if( writen(fd, &opcode, sizeof(int)) < 0) {
			perror("Server: writen error\n");
			return;
		}

		send_node_info(fd, &initiator);
		send_node_info(fd, &dead);
	} else {
		// Set the predecessor of the initiator of the probe return to me
		set_predecessor(&initiator, my_node);
		// Set my successor to the initiator of the probe return
		finger_table[1].node.id = initiator.id;
		sprintf(finger_table[1].node.address, "%s", initiator.address);
	}
}

// ------------------------------------------------------------------
int send_key(node_t *target, int key) {
	int opcode = SEND_KEY;
	int fd = tcp_open(target->address, port);
	int ack;

	if (fd<0)    // The target machine is down;
	  {
	    /********* DO SOMETHING HERE *********/
	  }
                                
	if( writen(fd, &(opcode), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	if( writen(fd, &(key), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}
                        
	/********* ADD HERE FOR SENDING KEY'S DATA *********/
	get_data_key(fd,key,1);    // 1 --> delete the data after sending

	if( readn(fd, &(ack), sizeof(int)) < 0) {
		perror("Server: readn error\n");
		return -1;
	}

	if (ack==0)
		printf("Something wrong on the sending a key [continue] \n");	

	close(fd);
	return 0;
}

int get_key(int fd) {
	int key;
	int ack = 1;

	if( readn(fd, &(key), sizeof(int)) < 0) {
		perror("Server: readn error\n");
		return -1;
	}

	/************ ADD HERE FOR RECEIVING KEY's DATA *********/
	request_data_key(fd,key,0);  // 0 --> add the data into my local database

	if (my_data[key] == 1) ack = 0;

	if( writen(fd, &(ack), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	my_data[key] = 1;
}

void distribute_my_key() {
	int i, key;
	node_t target_node;

	for (i=0; i<100; i++) {
		if (my_data[i]==1) {
			key = i%pow(2,M);

			if (key!=my_node->id) {
			   find_successor(my_node,key,&target_node);

			   if (my_node->id!=target_node.id) {
					my_data[i] = 0;
					printf("\nSend key %d to %s(node id: %d) \n",
							i, target_node.address,target_node.id);
					send_key(&target_node,i);
			    } 
			} 
		} 
	}
}

int ask_the_host_to_distribute_its_key() {
	int opcode = DIST_KEY_HOST;
	int fd = tcp_open(machine_name, port);
	int ack;
               

	if (fd<0) // if the host machine is down
	  {
	    /******* DO SOMETHING HERE **********/
	  }

	if( writen(fd, &(opcode), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	if( readn(fd, &(ack), sizeof(int)) < 0) {
		perror("Server: readn error\n");
		return -1;
	}

	if (ack==0)
		printf("Something wrong on asking the host to distribute its key [continue] \n");	

	close(fd);
	return 0;
}

int send_ack(int fd, int ack) {
	if( writen(fd, &(ack), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}
}

int ask_other_to_distribute_their_keys(node_t *source) {
	int opcode = DIST_KEY_OTHER;
	int fd = tcp_open(finger_table[1].node.address, port);	// send to my successor
	int ack;
                    
	if (fd<0) {
	  printf("The successor node is down, please until the finger table is updated, then run again \n");
	  return -1;

	}

	if( writen(fd, &(opcode), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	send_node_info(fd,source);

	if( readn(fd, &(ack), sizeof(int)) < 0) {
		perror("Server: readn error\n");
		return -1;
	}

	if (ack==0)
		printf("Something wrong on asking the host to distribute its key [continue] \n");	

	close(fd);
	return 0;
}

int get_DIST_KEY_OTHER(int fd) {
	node_t source;
	request_node_info(fd,&source);

	send_ack(fd,1);
	if (source.id != my_node->id) {
		// Propogate the mesage
		distribute_my_key();
		ask_other_to_distribute_their_keys(&source);
	}
}

void print_key()
{
	int i;

	printf("My Key List: ");
	for (i=0; i<100; i++)
		if (my_data[i] == 1)
			printf("%d ",i);
	printf("\n");
}

int get_ask_key(int fd)
{
       int key;
       int get_key;
 
       if( readn(fd, &(key), sizeof(int)) < 0) {
		perror("Server: readn error\n");
		return -1;
	}

       if( readn(fd, &(get_key), sizeof(int)) < 0) {
		perror("Server: readn error\n");
		return -1;
	}
       if (my_data[key]==0)
	 send_ack(fd,0);
       else
	 send_ack(fd,1);
       if (get_key==1)
	 {
	  
	   /******** ADD HERE TO SENDING KEY'S DATA ********/
	   get_data_key(fd,key,0);
	 }
       return 0;
}

int ask_key(node_t *source, int key, int get_key)
{
	int opcode = SEARCH_KEY;
        int fd = tcp_open(source->address, port);	// send to my successor
        int ack;
                
	if (fd < 0) {
	  printf("The host machine is down, please wait until the finger table is updated, then run again \n");
	  return -1;
	}
        if( writen(fd, &(opcode), sizeof(int)) < 0) {
                perror("Server: writen error\n");
                return -1;
        }
       if( writen(fd, &(key), sizeof(int)) < 0) {
                perror("Server: writen error\n");
                return -1;
        }
       if( writen(fd, &(get_key), sizeof(int)) < 0) {
                perror("Server: writen error\n");
                return -1;
        }
       if( readn(fd, &(ack), sizeof(int)) < 0) {
		perror("Server: readn error\n");
		return -1;
	}
       if ((get_key==1) && (ack==1))       // If asking the host to send key's data
	 {
	   /******* ADD HERE FOR RECEIVING KEY'S DATA *******/
	   request_data_key(fd,key,1);
	 }
		
        close(fd);
        return ack;
}

int search_key(int key, int get_key)  // get_key = 1 --> search and get data
{                                                   // get_key = 0 --> search only
        int temp_key; 
	node_t source;

	temp_key = key%pow(2,M);

	if (key<100) {
	  if (my_data[key]==1) {
	    if (get_key==1)
	      print_key_no(key);
	      printf("That key (%d) is in my local database \n",key);
	      return 1;
	  }
	  else 
	  {
	    find_successor(my_node,temp_key,&source);
	    if (ask_key(&source,key,get_key)==1) {
	      printf("That key (%d) is located in %s (node id: %d) \n",key,source.address,source.id);
	      return 1;
	    } else
	      printf("That key (%d) can't be found \n",key);
	  }
	} else
	  printf("The key(%d) must be less than 100 \n",key);
	return 0;
}

int ask_other_to_send_their_finger_tables(node_t *source) {
	int opcode = ASK_FINGER_TABLE;
	int fd = tcp_open(finger_table[1].node.address, port);	// send to my successor
	int ack;
                    
	if (fd<0) {
	  printf("The successor node is down, please wait until the finger table is updated, then run again \n");
	  return -1;

	}

	if( writen(fd, &(opcode), sizeof(int)) < 0) {
		perror("Server: writen error\n");
		return -1;
	}

	send_node_info(fd,source);

	if( readn(fd, &(ack), sizeof(int)) < 0) {
		perror("Server: readn error\n");
		return -1;
	}

	if (ack==0)
		printf("Something wrong on asking the host to send its finger table [continue] \n");	

	close(fd);
	return 0;
}

int send_my_finger_table(node_t *target)
{
        int opcode = SEND_FINGER_TABLE;
        int fd = tcp_open(target->address, port);     
        int ack;
                    
	if (fd<0) {
	  printf("The successor node is down, please wait until the finger table is updated, then run again \n");
	  return -1;

	}

        if( writen(fd, &(opcode), sizeof(int)) < 0) {
                perror("Server: writen error\n");
                return -1;
        }
	if( writen(fd, &(my_node->id), sizeof(int)) < 0) {
                perror("Server: writen error\n");
                return -1;
        }
	send_node_info(fd,&finger_table[1].node);
	send_node_info(fd,&finger_table[2].node);
	send_node_info(fd,&finger_table[3].node);

	if( readn(fd, &(ack), sizeof(int)) < 0) {
		perror("Server: readn error\n");
		return -1;
	}
	if (ack==0)
		printf("Something wrong on asking the host to send its finger table [continue] \n");	
        close(fd);
        return 0;
}

int get_other_finger_table(int fd)
{
        int node_id,i;
	node_t node[M];
	finger_table_t other_finger_table[M+1];

	if( readn(fd, &(node_id), sizeof(int)) < 0) {
		perror("Server: readn error\n");
		return -1;
	}
	request_node_info(fd,&node[0]);
	request_node_info(fd,&node[1]);
	request_node_info(fd,&node[2]);
	for (i=1; i<=M; i++) {
		other_finger_table[i].start = (node_id + pow(2, i-1)) % pow(2, M);
		other_finger_table[i].int_start = other_finger_table[i].start;
		other_finger_table[i-1].int_end = other_finger_table[i].int_start;
	}
	other_finger_table[M].int_end = other_finger_table[1].start - 1;
	if (other_finger_table[M].int_end < 0) {
		other_finger_table[M].int_end += pow(2, M);
	}
	printf("\nFinger Table of Node: %d \n", node_id);
	printf("==================================== \n");
	printf("Start  Int_Start  Int_End  Successor \n");
	for (i=1; i<=M; i++)
	  {
	    printf("%d\t%d\t%d\t%s(%d)\n",other_finger_table[i].start,other_finger_table[i].int_start,
            other_finger_table[i].int_end, node[i-1].address, node[i-1].id);
	  }
        send_ack(fd,1);
}

int get_ASK_FINGER_TABLE(int fd) {
	node_t source;
	request_node_info(fd,&source);

	send_ack(fd,1);
	if (source.id!=my_node->id) {
		// Propogate the mesage
		send_my_finger_table(&source);
		ask_other_to_send_their_finger_tables(&source);
	}
}

int send_keys_to_my_successor()
{
	int i, key;
         
        for (i=0; i<100; i++) {
                if (my_data[i]==1) {
                        key = i%pow(2,M);
        
                        my_data[i] = 0;
                        printf("\nSend key %d to %s(node id: %d) \n",
                               i,finger_table[1].node.address,finger_table[1].node.id);
                        send_key(&finger_table[1].node,i);
                }
        }
}
