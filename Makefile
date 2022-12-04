# http://www.lab.dit.upm.es/~cdatlab/exs/rpc/test/
# rpcgen example Makefile
# Internetworking with TCP/IP, Volume III
#
CC=gcc -ansi  -D_GNU_SOURCE
PROGS = client server

DEFS = 
CFLAGS = -g ${DEFS} ${INCLUDE}

SRC = ./Serveur/server.c ./Client_Ex/client.c ./Serveur/command_handler.c

all: ${PROGS}

server: ./Serveur/server.c ./Serveur/command_handler.c
	gcc ./Serveur/server.c ./Serveur/command_handler.c -o server

client: ./Client_Ex/client.c
	gcc ./Client_Ex/client.c -o client

clean: 
	rm -f Makefile.bak a.out tags core make.out ${PROGS}

install: all FRC
	@echo nothing to install.

print: Makefile ${SRC} FRC
	lpr -J"${PROG} source" Makefile ${SRC}

tags: ${SRC}
	ctags ${SRC}
