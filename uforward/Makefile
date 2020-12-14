CC = gcc
CFLAGS=-Wall -g
CLIB=-L /usr/lib -lpthread
CINC=-I/usr/include


C_SOURCES = uforward.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIB) $(CINC) -o uforward

# gcc -Wall -L /usr/lib -lpthread -I/usr/include -o uforward
#     2>&1 
#    | tee build.log

clean:
	/bin/rm -f *.o uforward build.log
