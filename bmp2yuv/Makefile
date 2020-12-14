# Makefile for BMP --> YUV converter

include $(IADK)/common_defs.mk
MSTOOLS    = C:/MSVC7
MSCL       = $(MSTOOLS)/bin/cl.exe
MSPLATFORM = $(MSTOOLS)/PlatformSDK

#
# Where to put objects and modules we're making
#
OBJDIR = $(BUILDAREA_DIR)/tools
TOOLDIR = $(IADK)/tools/win32


B2Y_EXE = $(TOOLDIR)/b2y.exe

LIBS = \
    $(MSTOOLS)/lib/libc.lib \
    $(MSTOOLS)/lib/OLDNAMES.LIB \
    $(MSTOOLS)/lib/kernel32.lib \

B2Y_OBJS = \
	$(OBJDIR)/b2y.obj \
    $(LIBS)


all: $(B2Y_EXE)

#	link.exe kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib \
#		advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib \
#		-nologo -dll -debug -machine:I386 -out:$@ -implib:jlib.lib $(JLIB_OBJS)

# This demonstrates how an application could link with jlib.
#jlib_app.exe: jlib_app.obj jlib.dll
#	$(MSCL) -Fe$@ jlib_app.obj jlib.lib

$(B2Y_EXE): $(B2Y_OBJS)
	@$(ECHO) --
	@$(ECHO) -- Linking $@ 
	@$(ECHO) --
	$(MSCL) -Fe$@ $(B2Y_OBJS)

$(OBJDIR)/b2y.obj: bmp2yuvosd.c
	@$(ECHO) --
	@$(ECHO) -- Compiling $< 
	@$(ECHO) --
	$(MSCL) -DWinNT -I. -I$(IADK)/../../include -I$(MSPLATFORM)/Include -I$(MSTOOLS)/include -Fo$@ -c $<

clean:
	$(RM) -f $(OBJDIR)/*.obj
	$(RM) -f $(ALL_TOOLS)

# end of Makefile
