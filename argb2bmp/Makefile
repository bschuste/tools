# Makefile for YUV --> BMP converter

#include $(IADK)/common_defs.mk
MSTOOLS    = C:/MSVC7
MSCL       = $(MSTOOLS)/bin/cl.exe
MSPLATFORM = $(MSTOOLS)/PlatformSDK

#
# Where to put objects and modules we're making
#
OBJDIR = .


Y2B_EXE = ./y2b.exe

LIBS = \
    $(MSTOOLS)/lib/libc.lib \
    $(MSTOOLS)/lib/OLDNAMES.LIB \
    $(MSTOOLS)/lib/kernel32.lib \

Y2B_OBJS = \
	$(OBJDIR)/y2b.obj


all: $(Y2B_EXE)

#	link.exe kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib \
#		advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib \
#		-nologo -dll -debug -machine:I386 -out:$@ -implib:jlib.lib $(JLIB_OBJS)

# This demonstrates how an application could link with jlib.
#jlib_app.exe: jlib_app.obj jlib.dll
#	$(MSCL) -Fe$@ jlib_app.obj jlib.lib

$(Y2B_EXE): $(Y2B_OBJS)
	@$(ECHO) --
	@$(ECHO) -- Linking $@ 
	@$(ECHO) --
	$(MSCL) -Fe$@ $(LIBS) $(Y2B_OBJS)

$(OBJDIR)/y2b.obj: yuv2bmp.c
	@$(ECHO) --
	@$(ECHO) -- Compiling $< 
	@$(ECHO) --
	$(MSCL) -DWinNT -I. -I$(IADK)/../../include -I$(MSPLATFORM)/Include -I$(MSTOOLS)/include -Fo$@ -c $<

clean:
	$(RM) -f $(OBJDIR)/*.obj
	$(RM) -f $(ALL_TOOLS)

# end of Makefile
