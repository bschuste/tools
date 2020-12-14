CC = gcc
CFLAGS=-Wall -g -ansi
CLIB=
CINC = -I../inc

H_SOURCES =			    \
	../inc/viSdefs.h    \
	../inc/viSerror.h   \
	../inc/viSstruct.h


C_SOURCES = 		\
	viSpsnr.c

all: $(C_SOURCES)
	$(CC) $(CFLAGS) $(C_SOURCES) $(CLIBS) $(CINC) -o viSpsnr 2>&1 | tee build.log

clean:
	/bin/rm -f *.o viSpsnr.exe viSpsnr
