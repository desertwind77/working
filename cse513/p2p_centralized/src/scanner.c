#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "fifo.h"
#include "scanner.h"

// Global variables
char lexeme[MAX_TOKEN_LEN + 1];
extern FILE *yyin;

// Prototypes
extern int yylex (void);
extern int yywrap ( void );
char *expect(int expected);

char *title() 		{ return (char *)expect(RTITLE); }
char *artist() 	{ return (char *)expect(RARTIST); }
char *country() 	{ return (char *)expect(RCOUNTRY); }
char *company() 	{ return (char *)expect(RCOMPANY); }
char *year() 		{ return (char *)expect(RYEAR); }

char *expect(int expected) {
	int token, first = 1;
	char *ptr, *buffer = (char *)malloc(MAX_TOKEN_LEN + 1);

	sprintf(buffer, "");
	while((token = yylex()) != expected) {
		if(!first) {
			ptr = (char *)strdup(" ");
			strcat(buffer, ptr);
		} else {
			first = 0;
		}

		ptr = (char *)strdup(lexeme);
		strcat(buffer, ptr);
	}

	return buffer;
}

int read_data(char *filename, fifo_t *key_list) {
	int token;
	keys_t *object;

	yyin = fopen(filename, "r");

	token = yylex();
	while(token != 0) {
		if((token == TEXT) && !strcmp(lexeme, "key")){
			object = (keys_t *)malloc(sizeof(keys_t));

			if((token = yylex()) == NUMBER) {
				object->key = atoi(lexeme);
				printf("Key = %d\n", object->key);
			}

			token = yylex();
			while((token != 0) && strcmp(lexeme, "key")) {
				switch(token ) {
					case LTITLE : object->title = title(); break;
					case LARTIST : object->artist = artist(); break;
					case LCOUNTRY : object->country = country(); break;
					case LCOMPANY : object->company = company(); break;
					case LYEAR : object->year = year(); break;
				}

				token = yylex();
			}

			FIFO_enqueue(key_list, FIFO_element(object));

			printf("Title = %s\n", object->title);
			printf("ARTIST = %s\n", object->artist);
			printf("COUNTRY = %s\n", object->country);
			printf("COMPANY = %s\n", object->company);
			printf("YEAR = %s\n", object->year);
			printf("\n");
		}
	}

	return 0;
}

/*
 * test_parser
 * Functionality : Test the parser. No use in any program
 * Return Value : NONE
 * Parameter :
 * 	filename - the file to be parsed
 */
void test_parser(char *filename) {
	int token;

	yyin = fopen(filename, "r");

	while((token=yylex()) != 0) {
		switch(token) {
			case TEXT : printf("TEXT\t\t"); break;
			case NUMBER : printf("NUMBER\t\t"); break;
			case LTITLE : printf("LTITLE\t\t"); break;
			case RTITLE : printf("RTITLE\t\t"); break;
			case LARTIST : printf("LARTIST\t\t"); break;
			case RARTIST : printf("RARTIST\t\t"); break;
			case LCOUNTRY : printf("LCOUNTRY\t\t"); break;
			case RCOUNTRY : printf("RCOUNTRY\t\t"); break;
			case LCOMPANY : printf("LCOMPANY\t\t"); break;
			case RCOMPANY : printf("RCOMPANY\t\t"); break;
			case LYEAR : printf("LYEAR\t\t"); break;
			case RYEAR : printf("RYEAR\t\t"); break;
			case ' ' : continue;
		}
		printf("%s\n", lexeme);
	}
}
