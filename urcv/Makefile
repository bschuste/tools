CC = gcc
CFLAGS=-Wall -g
CLIB=-L /usr/lib -lpthread
CINC=-I/usr/include


C_SOURCES = urcv.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIB) $(CINC) -o urcv

# gcc -Wall -L /usr/lib -lpthread -I/usr/include -o urcv
#     2>&1 
#    | tee build.log

clean:
	/bin/rm -f *.o urcv build.log
