
CC = gcc
CFLAGS=-Wall -g
CLIB=
CINC = -I../inc

C_SOURCES = 		\
	elacstn.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIBS) $(CINC) -o elacstn.exe 2>&1 | tee build.log

clean:
	/bin/rm -f *.o elacstn.exe
