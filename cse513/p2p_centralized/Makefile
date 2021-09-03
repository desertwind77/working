CC = gcc
CFLAGS = -g -D_REENTRANT -D__PTHREAD__
CFLAGS += $(INCLUDES)
LEX = lex
#LIBS    += -lnsl -lsocket -lpthread
LIBS    += -lpthread
INCLUDES = -I./include
SRC     = ./src
SERVER = $(SRC)/server.c $(SRC)/tcp.c $(SRC)/fifo.c
BATCH = $(SRC)/client_old.c $(SRC)/tcp.c $(SRC)/fifo.c $(SRC)/lex.yy.c $(SRC)/scanner.c
CLIENT = $(SRC)/client.c $(SRC)/tcp.c $(SRC)/fifo.c $(SRC)/lex.yy.c $(SRC)/scanner.c

main: server client batch

server:
	$(CC) $(CFLAGS) $(LIBS) -o svc $(SERVER)

batch:
	$(LEX) $(SRC)/scanner.l
	mv lex.yy.c $(SRC)
	$(CC) $(CFLAGS) $(LIBS) -o batch $(BATCH)

client:
	$(LEX) $(SRC)/scanner.l
	mv lex.yy.c $(SRC)
	$(CC) $(CFLAGS) $(LIBS) -o cli $(CLIENT)

clean:
	rm -f a.out *.o cli svc core
