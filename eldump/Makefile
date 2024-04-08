CC ?= gcc
CFLAGS=-Wall -g
CLIB=

C_SOURCES = 		\
	eldump.c

C_EXEC = eldump

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIBS) -o $(C_EXEC)

install: $(C_EXEC)
	/bin/cp $(C_EXEC) ~/.local/bin
	/bin/rm -f $(C_EXEC)

clean:
	/bin/rm -f *.o $(C_EXEC).exe $(C_EXEC)
