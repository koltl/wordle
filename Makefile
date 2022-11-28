
OBJS	= client.o server.o
SOURCE	= client.c, server.c
HEADER	= 
CC	 = gcc
FLAGS	 = -g -c 
LFLAGS = -L. -llibrary


all: client.c server.c
	make lib_comp
	gcc -g -o client client.c library.a
	gcc -g -o server server.c library.a
	make clean
client_run: client.c
	make lib_comp
	gcc -g -o client client.c library.a
	make clean

server_run: server.c
	make lib_comp
	gcc -g -o server server.c library.a
	make clean

lib_comp: errcheck.c msgHandle.c
	gcc -g -c errcheck.c -o errcheck.o
	gcc -g -c msgHandle.c -o msgHandle.o
	ar rcs library.a errcheck.o msgHandle.o


clean:
	rm errcheck.o msgHandle.o
