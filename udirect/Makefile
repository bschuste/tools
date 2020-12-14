#CC = gcc
CFLAGS=-Wall -g
CLIB=-lpthread
CINC=


C_SOURCES = udirect.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIB) $(CINC) -o udirect


# $CC -Wall -lpthread -o udirect 2>&1 | tee build.log

clean:
	/bin/rm -f *.o udirect build.log
