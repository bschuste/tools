# Makefile for system utilities
#

MSCL = C:/MSVC9/bin/cl.exe
MSTOOLS = C:/MSVC9
ROOT_DIR = ..
SRCDIR = $(ROOT_DIR)/udp_snd
#
# Where to put objects and executables we're making
#
OBJDIR = $(SRCDIR)
UDP_RCV = $(SRCDIR)/udp_snd.exe

LIBS = \
    $(MSTOOLS)/lib/LIBC.LIB \
    $(MSTOOLS)/lib/OLDNAMES.LIB \
    $(MSTOOLS)/lib/kernel32.lib \
    $(MSTOOLS)/lib/ws2_32.lib \
    
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
	$(OBJDIR)/udp_snd.obj

all: $(UDP_RCV)

# -MT means link with LIBCMT.LIB


$(UDP_RCV): $(PTS_OBJS)
	@$(ECHO) --
	@$(ECHO) --  
	@$(ECHO) --
	$(MSCL) -Feudp_snd $(PTS_OBJS) $(LIBS)
	$(RM) $(OBJDIR)/udp_snd.obj

$(OBJDIR)/udp_snd.obj: udp_snd.c
	@$(ECHO) --
	@$(ECHO) -- 
	@$(ECHO) --
	$(MSCL) -I$(MSTOOLS)/include -I$(ROOT_DIR)/inc \
	        -I$(ROOT_DIR)/lib -Fo$@ -c $<
	        
clean:
	$(RM) *.obj udp_snd.exe udp_snd

# end of Makefile
