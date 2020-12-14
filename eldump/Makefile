CC = gcc
CFLAGS=-Wall -g
CLIB=

C_SOURCES = 		\
	eldump.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIBS) -o eldump.exe

clean:
	/bin/rm -f *.o eldump.exe
