CC = gcc
CFLAGS = -g -D_REENTRANT -D__PTHREAD__
CFLAGS += $(INCLUDES)
LEX = lex
LIBS    += -lnsl -lsocket -lpthread
INCLUDES = -I./include
SRC     = ./src
P2P = $(SRC)/p2p.c $(SRC)/tcp.c $(SRC)/data.c $(SRC)/fifo.c $(SRC)/lex.yy.c $(SRC)/scanner.c 

main: p2p_rule

p2p_rule: 
	$(LEX) $(SRC)/scanner.l
	mv lex.yy.c $(SRC)
	$(CC) $(CFLAGS) $(LIBS) -o p2p $(P2P)

clean:
	rm -f a.out *.o core p2p
