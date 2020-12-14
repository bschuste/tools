CC = gcc
CFLAGS=-Wall -g
CLIB=

C_SOURCES = 		\
	viScat.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIBS) -o viScat.exe

clean:
	/bin/rm -f *.o viScat.exe
