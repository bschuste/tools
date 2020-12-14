CC = gcc
CFLAGS=-Wall -g
CLIB=
CINC = -I../inc

H_SOURCES =			    \
	../inc/viSdefs.h    \
	../inc/viSerror.h   \
	../inc/viSstruct.h

C_SOURCES = 		\
	viSencrypt.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIBS) $(CINC) -o viSencrypt 2>&1 | tee build.log

clean:
	/bin/rm -f *.o viSencrypt.exe viSencrypt
