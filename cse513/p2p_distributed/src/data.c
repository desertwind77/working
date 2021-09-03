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


typedef struct search_result_t search_result_t;
struct search_result_t {
	int node_id;
	char *hostname;
};


fifo_t *key_list;

// ------------------- Sub-routines ----------------------------------


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

void delete_local_db(int key, fifo_t *key_list) {
  if (FIFO_remove2(key_list,CB_search_local_db,(void*)&key) == NULL)
    printf("Something wrong on removing the key %d \n",key);

}

void create_key_list()
{
       key_list = FIFO_create();
}

int read_file(char *filename) {
       
	read_data(filename, key_list);
}


int send_data_key(int fd, char *value) {
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

void print_key_no(int key_no)
{
  keys_t *object;
  if((object = search_local_db(key_no, key_list)) != NULL) {
       printf("KEY = %d\n",object->key);
       printf("Title = %s\n", object->title); 
       printf("ARTIST = %s\n", object->artist); 
       printf("COUNTRY = %s\n", object->country); 
       printf("COMPANY = %s\n", object->company);
       printf("YEAR = %s\n", object->year);
       printf("\n");
  }
}

int get_data_key(int fd, int key_no, int mode) {
	char *data;
	int *sz; 
	keys_t *object;
  
	// Search local DB
	if((object = search_local_db(key_no, key_list)) != NULL) {
	       
  
		send_data_key(fd,object->title);
		send_data_key(fd,object->artist);
		send_data_key(fd,object->country);
		send_data_key(fd,object->company);
		send_data_key(fd,object->year);
		if (mode == 1)   // mode 1 --> after sending the data, the data is erased
		  {
		    delete_local_db(key_no,key_list);
		  }
	}
}

int receive_data_key(int sockfd, char* note, int mode, char storage[80]) {
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
	strncpy(storage,data,80);
        if (mode ==1)         // mode 1 --> only print
	   printf("%s : %s \n",note,storage);
                         // mode 2 --> save the packet to the keys_t
	  
        
}

int request_data_key(int sockfd, int key, int mode) {
   keys_t *object;
   char data[80], data1[80], data2[80], data3[80], data4[80];


   object= (keys_t*) malloc (sizeof(keys_t));

   
   if (mode == 1)
     printf("KEY: %d\n",key);
   object->title=(char *) malloc (80*sizeof(char));
   object->artist=(char *) malloc (80*sizeof(char));
   object->country=(char *) malloc (80*sizeof(char));
   object->company=(char *) malloc (80*sizeof(char));
   object->year=(char *) malloc (80*sizeof(char));

   receive_data_key(sockfd,"Title", mode, object->title);
   receive_data_key(sockfd,"ARTIST", mode, object->artist);
   receive_data_key(sockfd,"COUNTRY", mode, object->country);
   receive_data_key(sockfd,"COMPANY", mode, object->company);
   receive_data_key(sockfd,"YEAR", mode, object->year);
   
   
   if (mode==0)
     {
       object->key = key;
       printf("KEY = %d\n",object->key);
       printf("Title = %s\n", object->title); 
       printf("ARTIST = %s\n", object->artist); 
       printf("COUNTRY = %s\n", object->country); 
       printf("COMPANY = %s\n", object->company);
       printf("YEAR = %s\n", object->year);
       printf("\n");
       FIFO_enqueue(key_list,FIFO_element(object));
     }
}

