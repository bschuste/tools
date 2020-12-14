CC = gcc
CFLAGS = -Wall -g
CLIBS =
CINC = -Iinc

C_FILES = 		\
	streamanager.c  \
	client.c        \
	tsmanager.c     \
	viStest.c

H_FILES =			 \
	inc/defines.h    \
	inc/error.h      \
	inc/tsmanager.h  \
	externs.h	

all: $(C_FILES) $(H_FILES)
	 $(CC) $(CFLAGS) $(CLIBS) $(CINC) $(C_FILES) -o viStest 2>&1 | tee build.log

clean:
	/bin/rm -f *.o viStest.exe
