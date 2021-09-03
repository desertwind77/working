/*
 * tcp.h
 */

#ifdef __TCP_H__

int tcp_open(char *host, int port);
int readn(int fd, char *ptr, long nbytes);
int writen (int fd, char *ptr, long nbytes);
int readline (int fd, char *ptr, int maxlen);

#endif
