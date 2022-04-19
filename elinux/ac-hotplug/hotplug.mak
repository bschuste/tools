###################################################################################################
## Very generic Makefile:
## It generates the object files from all cpp or cc files in a directory and links them together.
## You only have to supply it with your TARGET, LIBS, FLAGS, PATHS etc.
###################################################################################################

# ---> WHICH ENDINGS DO THE SOURCE FILES HAVE ?
ENDING_SOURCE := c
ENDING_HEADER := h

# ---> Is the target a shared lib, a static lib or a executable ?
TARGETTYPE      := executable

# ---> The name of the executable:
TARGETNAME      := hotplugd

# ---> Compiler options
LOCAL_CFLAGS = -g -O

#CC              := arm-poky-linux-gnueabi-c++  -march=armv7-a -mfpu=neon -mthumb -mtune=cortex-a9
#CXX             := arm-poky-linux-gnueabi-c++  -march=armv7-a -mfpu=neon -mthumb -mtune=cortex-a9
#CC              := arm-none-linux-gnueabi-c++
#CXX             := arm-none-linux-gnueabi-c++
CFLAGS          := $(LOCAL_CFLAGS)
#CXXFLAGS        := -std=c99 -pipe -O3 -fno-rtti -Wall -W -g -D_REENTRANT -DPATH2CONF=\"ScanDraiD.conf\"  #-DDEBUG
#INCPATH         := -I../../osal -I../../osal/Linux

#FIXME create an entry
#scp hotplug.service root@192.168.86.106:/lib/systemd/system/

# ---> Linkeroptions
#LINK            := arm-none-linux-gnueabi-c++
LINK            := ${CXX}
LFLAGS          := -g
LIBS            := #-lpthread -ldl
LIBPATH         :=  
# ---> Generates object files of all *.c, *.cc, *.cpp, *.h etc. files thar are found in directory
HEADERS         := 
SOURCES         := hotplug.c
OBJECTS         := hotplug.o
# --> Tools
LN              := ln -s
TAR             := tar -cf
GZIP            := gzip -9f
BZIP            := bzip2
RM              := rm -vf
TAGS            := ctags --extra=+q
CP              := cp -vf
LDCONFIG        := /sbin/ldconfig
RANLIB          := ranlib
AR              := ar crv

# -------------------> Do not edit <-------------------- #


# ---> Implicit rules

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
.C.o:
	$(CC) -c  $(CCFLAGS) $(INCPATH)  -o $@ $<
.c.o:
	$(CC) -c  $(CFLAGS) $(INCPATH)   -o $@ $<

# ---> Build rules

.PHONY : all depend objects clean proper world executable


all : $(TARGETTYPE)

executable: $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGETNAME) $(OBJECTS) $(LIBPATH) $(LIBS)

depend:
	$(CXX) ${INCPATH} $(DEP_FLAG) $(SOURCES) > $(DEPS)

objects: $(OBJECTS)

clean:
	$(RM) $(TARGETNAME)
	$(RM) $(OBJECTS)
	$(RM) *.rpo
	$(RM) *.d
	$(RM) *~ core

proper: clean
	$(RM) *.a
	$(RM) *.bak
	$(RM) *.old
	$(RM) $(DEPS)
	$(RM) tags
	$(RM) *~

tags: $(HEADERS) $(SOURCES)
	$(TAGS) $(HEADERS) $(SOURCES)

world: proper tags depend all

-include $(DEPS)


