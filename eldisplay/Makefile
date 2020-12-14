CC = gcc
CFLAGS=-Wall -g
CLIB=-lopengl32

C_SOURCES = 		\
	cif411torgb.c	\
	qcif411torgb.c	\
	readcif411.c	\
	readqcif411.c     \
	avSdisplay.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIBS) -o avSdisplay.exe

clean:
	/bin/rm -f *.o avSdisplay.exe
