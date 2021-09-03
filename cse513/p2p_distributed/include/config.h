#ifndef __CONFIG_H__
#define __CONFIG_H__

// Opcodes of the protocol
#define JOIN	1
#define SEARCH	2
#define LEAVE 	3
#define SREPLY	4
#define GET		5
#define GREPLY	6
#define DEAD	7
#define EXIT	20
#define CLOSET	100
#define PRED	101
#define SUCC	102
#define UPDATE	103

typedef struct keys_t keys_t;
struct keys_t {
	int key;
	char *title;
	char *artist;
	char *country;
	char *company;
	char *year;
};

#endif
