/*
 * tcp.c : Basic TCP functions
 * By : Modified from Richard Steven's (Unix Network Programming)
 */

#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/lwp.h>
#include <sys/types.h>
#include <sys/socket.h>

// Required by inet_addr()
#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif

// Required by gethostbyname_r() 
#define	BUFFER_SIZE		2048

lwp_mutex_t get_host_name;

/*
 * tcp_open (host,service,port)
 * Function  : Open TCP connection to the specified host at the 
 *		specified port
 * Return    : socket descriptor if OK, else -1 on error
 * Parameter :
 * 	host - hostname or IP address
 *		port - server's port 
 */ 
int tcp_open (char *host,int port) {
	unsigned long inaddr;
	struct sockaddr_in tcp_srv_addr;		// Server's address
	struct hostent	*hp, *res;				// For hostname lookup
	int fd, error;
	char buffer[BUFFER_SIZE];
	

	bzero ((char *)&tcp_srv_addr, sizeof(tcp_srv_addr));
	tcp_srv_addr.sin_family = AF_INET;
	tcp_srv_addr.sin_port = htons(port); 

	hp = (struct hostent *)MALLOC(sizeof(struct hostent));
	
	// Determining host IP address or host name 
	if((inaddr = inet_addr(host)) != INADDR_NONE) {
		// The address is in the dotted-decimal format
		bcopy ((char *)&inaddr, (char *)&tcp_srv_addr.sin_addr, 
			sizeof(inaddr));
	} else {
		// Lookup the hostname
//		_lwp_mutex_lock(get_host_name);

		if( (res = gethostbyname_r(host, hp, buffer, 
				2048, &error)) == NULL) {
			printf("[tcp_open]: unable to find the host %s\n", host);
//			_lwp_mutex_unlock(get_host_name);
			return (-1);
		}

//		_lwp_mutex_unlock(get_host_name);

		// Hostname lookup succeeded
		bcopy (hp->h_addr, (char *)&tcp_srv_addr.sin_addr, hp->h_length);
	}

	// Create a socket 
	if((fd=socket(AF_INET,SOCK_STREAM,0))<0) {
		printf("[tcp_open]: unable to create a socket\n");
		return(-1);
	}

	// Connect to the server
	if(connect(fd,(struct sockaddr *)&tcp_srv_addr,
				sizeof(tcp_srv_addr)) <0) {
		printf("[tcp_open]: unable to connect to server\n");
		close(fd);
		return(-1);
	}

	return(fd);
}

/*
 * readn (fd, ptr, nbytes)
 * Function  : read nbytes from file descriptor into ptr buffer.
 * Return    : number of read byte if success; otherwise, less than 0
 * Parameter :
 *		fd     = file (socket) descriptor
 *		ptr    = output buffer
 *		nbytes = number of bytes to be read
 */
int readn (int fd, char *ptr, long nbytes) {
  long nleft,nread;
  
  nleft=nbytes;
  while(nleft>0) {
    nread = read(fd,ptr,nleft);
	 
    if(nread<0) return(nread);  /* error, return <0 */
    else if(nread==0) break;    /* EOF */

    nleft -= nread;
    ptr   += nread;
  }

  return(nbytes-nleft);         /* return >= 0 */
}

/*
 * writen (fd, ptr, nbytes)
 * Function  : write nbytes from ptr buffer to file descriptor.
 * Return    : number of written byte if success; otherwise, less than 0
 * Parameter :
 *		fd     = file (socket) descriptor
 *		ptr    = input buffer
 *		nbytes = number of bytes to be written 
 */
int writen (int fd, char *ptr, long nbytes) {
  long nleft,nwritten;

  nleft = nbytes;
  while(nleft>0) {
    nwritten = write(fd,ptr,nleft);
	 
    if(nwritten<=0) return(nwritten);

    nleft -= nwritten;
    ptr   += nwritten;
  }

  return(nbytes-nleft);
}

/*
 * readline (fd, ptr, maxlen)
 * Function  : read 1 line or maxlen bytes from fd into prt
 * Return    : number of read byte if success; otherwise, less than 0
 * Parameter :
 *		fd     = file (socket) descriptor
 *		ptr    = output buffer
 *		maxlen = the maximum number of bytes to be written 
 */
int readline (int fd, char *ptr, int maxlen) {
  int n,rc;
  char c;
  
  for(n=1;n<maxlen;n++) {
    if((rc=read(fd,&c,1))==1) {
      *ptr++ = c;
      if(c=='\n') break;
    }
    else 
		 if(rc==0) { 
			 if(n==1) return(0); 
			 else break; 
		 } else return(-1);
  }

  *ptr = 0;
  return(n);
}
