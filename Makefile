
OBJS	= client.o server.o
SOURCE	= client.c server.c
FLAGS	 = gcc -g -o
LFLAGS = gcc -g -c


all: ${SOURCE}
	make lib_comp
	${FLAGS} client client.c library.a
	${FLAGS}  server server.c library.a
	make clean
client_run: client.c
	make lib_comp
	${FLAGS}  client client.c library.a
	make clean

server_run: server.c
	make lib_comp
	${FLAGS}  server server.c library.a
	make clean

lib_comp: errcheck.c msgHandle.c
	${LFLAGS}  errcheck.c -o errcheck.o
	${LFLAGS}  msgHandle.c -o msgHandle.o
	ar rcs library.a errcheck.o msgHandle.o


clean:
	rm errcheck.o msgHandle.o
