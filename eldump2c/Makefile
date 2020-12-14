CC = gcc
CFLAGS=-Wall -g
CLIB=

C_SOURCES = 		\
	eldump2c.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIBS) -o eldump2c.exe

clean:
	/bin/rm -f *.o eldump2c.exe
