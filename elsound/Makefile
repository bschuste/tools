# Makefile for system utilities
#

MSCL = C:/MSVC7/bin/cl.exe
MSTOOLS = C:/MSVC7
ROOT_DIR = ..
SRCDIR = $(ROOT_DIR)/visound
#
# Where to put objects and executables we're making
#
OBJDIR = $(SRCDIR)
CHECK = $(SRCDIR)/visound.exe

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
	$(OBJDIR)/PTS.obj \
	$(LIBS)
   
all: $(CHECK)

# -MT means link with LIBCMT.LIB


$(CHECK): $(PTS_OBJS)
	@$(ECHO) --
	@$(ECHO) --  
	@$(ECHO) --
	$(MSCL) -Fevisound $(PTS_OBJS)

$(OBJDIR)/PTS.obj: visound.c
	@$(ECHO) --
	@$(ECHO) -- 
	@$(ECHO) --
	$(MSCL) -I$(MSTOOLS)/include -I$(ROOT_DIR)/inc \
	        -I$(ROOT_DIR)/lib -Fo$@ -c $<
	        
clean:
	$(RM) -f $(PTS_OBJS)

# end of Makefile
