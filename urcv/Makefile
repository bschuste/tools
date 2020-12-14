#CC = gcc
CFLAGS=-Wall -g
CLIB=-lpthread
CINC=


C_SOURCES = urcv.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIB) $(CINC) -o urcv

# $CC -Wall -lpthread -o urcv 2>&1 | tee build.log

clean:
	/bin/rm -f *.o urcv build.log
