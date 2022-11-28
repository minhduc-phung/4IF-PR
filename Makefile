# http://www.lab.dit.upm.es/~cdatlab/exs/rpc/test/
# rpcgen example Makefile
# Internetworking with TCP/IP, Volume III
#
CC=gcc -ansi  -D_GNU_SOURCE
PROGS = client server

DEFS = 
CFLAGS = -g ${DEFS} ${INCLUDE}

SRC = ./Serveur/server2.c ./Client/client2.c

all: ${PROGS}

server: ./Serveur/server2.c
	gcc ./Serveur/server2.c -o server

client: ./Client/client2.c
	gcc ./Client/client2.c -o client

clean: 
	rm -f Makefile.bak a.out tags core make.out ${PROGS}

install: all FRC
	@echo nothing to install.

print: Makefile ${SRC} FRC
	lpr -J"${PROG} source" Makefile ${SRC}

tags: ${SRC}
	ctags ${SRC}