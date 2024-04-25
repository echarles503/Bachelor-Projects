CC=gcc

CFLAGS=-Wall -W -g -Werror 

INCLUDE=./include/llistcskmap.c ./include/iterator.c

all: client server

client: client.c raw.c
	$(CC) client.c raw.c $(CFLAGS) -o client

server: server.c
	$(CC) server.c $(CFLAGS) -I./include/ -L./include/ -lmap -o server 

clean:
	rm -f client server *.o

