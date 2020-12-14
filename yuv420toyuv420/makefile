# Makefile for DIB-YUV psnr

# include $(IADK)/common_defs.mk
MSTOOLS    = C:/MSVC7
MSCL       = $(MSTOOLS)/bin/cl.exe
MSPLATFORM = $(MSTOOLS)/PlatformSDK

#
# Where to put objects and modules we're making
#
OBJDIR = .


EXE = ./yuv420toyuv420.exe

LIBS = \
    $(MSTOOLS)/lib/libc.lib \
    $(MSTOOLS)/lib/OLDNAMES.LIB \
    $(MSTOOLS)/lib/kernel32.lib \

OBJS = \
	$(OBJDIR)/yuv420toyuv420.obj \
    $(LIBS)


all: $(EXE)

#	link.exe kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib \
#		advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib \
#		-nologo -dll -debug -machine:I386 -out:$@ -implib:jlib.lib $(JLIB_OBJS)

# This demonstrates how an application could link with jlib.
#jlib_app.exe: jlib_app.obj jlib.dll
#	$(MSCL) -Fe$@ jlib_app.obj jlib.lib
#	@$(ECHO) -- Linking $@
#	@$(ECHO) -- Compiling $< 

$(EXE): $(OBJS)
	@$(ECHO) --
	@$(ECHO) --
	$(MSCL) -Fe$@ $(OBJS) 2>&1 | tee build.log

$(OBJDIR)/yuv420toyuv420.obj: yuv420toyuv420.c
	@$(ECHO) --
	@$(ECHO) --
	$(MSCL) -DWinNT -I. -I$(MSPLATFORM)/Include -I$(MSTOOLS)/include -Fo$@ -c $< 2>&1 | tee build.log

clean:
	$(RM) -f $(OBJDIR)/*.obj
	$(RM) -f $(EXE)

# end of Makefile
