CC ?= gcc
CFLAGS=-Wall -g
CLIB=
CINC = -I../inc

H_SOURCES =			 \
	../inc/eldefs.h  \
	../inc/elerr.h   \
	../inc/elportab.h

C_SOURCES = 		 \
	h264parse.c

C_EXEC = h264parse

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIBS) $(CINC) -o $(C_EXEC) 2>&1 | tee build.log

install: $(C_EXEC)
	/bin/cp $(C_EXEC) ~/.local/bin
	/bin/rm -f $(C_EXEC)

clean:
	/bin/rm -f *.o $(C_EXEC).exe $(C_EXEC)
