CC = gcc
CFLAGS=-Wall -g
CLIB=-L /usr/lib -lpthread
CINC=-I/usr/include


C_SOURCES = udirect.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIB) $(CINC) -o 	/bin/rm -f *.o udirect


# gcc -Wall -L /usr/lib -lpthread -I/usr/include -o udirect
#     2>&1 
#    | tee build.log

clean:
	/bin/rm -f *.o udirect build.log
