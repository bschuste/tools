CC = gcc
CFLAGS=-Wall -g
CLIB=
CINC = -I../inc

H_SOURCES =			    \
	../inc/viSdefs.h    \
	../inc/viSerror.h   \
	../inc/viSstruct.h

C_SOURCES = 		\
	h264parse.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIBS) $(CINC) -o h264parse 2>&1 | tee build.log

clean:
	/bin/rm -f *.o h264parse.exe h264parse
