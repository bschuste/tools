# Makefile for system utilities
#

MSCL = C:/MSVC7/bin/cl.exe
MSTOOLS = C:/MSVC7
ROOT_DIR = ..
SRCDIR = $(ROOT_DIR)/elrcv
#
# Where to put objects and executables we're making
#
OBJDIR = $(SRCDIR)
UDP_RCV = $(SRCDIR)/elrcv.exe

LIBS = \
    $(MSTOOLS)/lib/LIBC.LIB \
    $(MSTOOLS)/lib/OLDNAMES.LIB \
    $(MSTOOLS)/lib/kernel32.lib \
    
#    $(MSTOOLS)/lib/gdi32.lib \
#    $(MSTOOLS)/lib/winspool.lib \
#    $(MSTOOLS)/lib/comdlg32.lib \
#	$(MSTOOLS)/lib/advapi32.lib \
#    $(MSTOOLS)/lib/shell32.lib \
#    $(MSTOOLS)/lib/ole32.lib \
#    $(MSTOOLS)/lib/oleaut32.lib \
#    $(MSTOOLS)/lib/uuid.lib \
#    $(MSTOOLS)/lib/odbc32.lib \
#    $(MSTOOLS)/lib/odbccp32.lib
    
#    $(MSTOOLS)/lib/user32.lib \
 
PTS_OBJS = \
	$(OBJDIR)/udp_rcv.obj

all: $(UDP_RCV)

# -MT means link with LIBCMT.LIB


$(UDP_RCV): $(PTS_OBJS)
	@$(ECHO) --
	@$(ECHO) --  
	@$(ECHO) --
	$(MSCL) -Feudp_rcv $(PTS_OBJS) $(LIBS)
	$(RM) $(OBJDIR)/udp_rcv.obj

$(OBJDIR)/udp_rcv.obj: udp_rcv.c
	@$(ECHO) --
	@$(ECHO) -- 
	@$(ECHO) --
	$(MSCL) -I$(MSTOOLS)/include -I$(ROOT_DIR)/inc \
	        -I$(ROOT_DIR)/lib -Fo$@ -c $<
	        
clean:
	$(RM) *.obj udp_rcv.exe udp_rcv

# end of Makefile
