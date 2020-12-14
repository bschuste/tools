CC = gcc
CFLAGS=-Wall -g -ansi
CLIB=
CINC = -I../inc

H_SOURCES =			    \
	../inc/viSdefs.h    \
	../inc/viSerror.h   \
	../inc/viSstruct.h


C_SOURCES = 		\
	viSmatrice.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIBS) $(CINC) -o viSmatrice 2>&1 

clean:
	/bin/rm -f *.o viSmatrice.exe viSmatrice
