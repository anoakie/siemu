################################################################################
# Makefile for 8080a simulator siemu
# Author:	Anoakie Ray Turner
# Thanks to:	Jon Atkins for restructuring this file.
# Date:		Apr 15 00:32:12 MST 2002
################################################################################
# General assumptions:  Generic POSIX compatible environment with
#			compatible UNIX shell

SHELL = /bin/sh

VERSION_MAJOR = 1
VERSION_MINOR = 03

SRCDIR = src


#BEGIN CONFIG###################################################################


# Select the target platform
TARGET=Linux
#TARGET=WIN32

# Select endian.  Use LITTLEENDIAN for x86 processors.  Note:  Only LITTLEENDIAN
# had been tested  at this time, and should not be defined unless autodetection
# fails.
#CFLAGS+= -DLITTLEENDIAN
#CFLAGS+= -DBIGENDIAN


# Uncomment for release mode all the time
#RELEASE=1

# Uncomment for debugging on all the time
#DEBUG=1


# Uncomment for more accurate simulation (needs faster CPU)
# 
#   This allows the simulator to use busy-wait loops instead of delays for more
# accurate timing.
CFLAGS+= -DTIMING
#
#   This allows 8080a ROM dumps to use 7 extended and undocumented z80
# instructions.  Some ROMs need these extra instructions.  These extra
# instructions complete the 8080a instruction set.
#CFLAGS+= -DZ80
#
#   This enables more accurate video simulation.  This sets the default pixel
# value to green instead of while.
CFLAGS+= -DGREEN
#
#   This enables the most accurate video simulation.  This sets the pixels to
# the color of the plastic overlay on the screen.  This macro overrides the
# GREEN macro above.
CFLAGS+= -DCOLOR
#
#   This enables self modifying code.  If this flag is not set, an error will
# be written to the screen if memory below WORKRAM (defined in 8080avar.h) is
# written.
CFLAGS+= -DSELFMOD
#
#   This enables verbose debug information to be dumped to the screen if an
# unknown opcode is encountered.
#CFLAGS+= -DVERBOSE
#
#
#### Note that modifying the next six variables may cause the simulator to run
#### unreliably.
#
#   This enables frame skipping.  The digit after the ='s means that a frame
# is drawn every n frames, where n is that digit.  The default value is 1.
#CFLAGS+= -DFRAMESKIP=1
#
#   This sets the game speed.  The digit after the ='s means that the game is
# slowed down to 1/(1+n) the speed of it's normal speed, where n is that digit.
# 0 implies full speed.  The default value is 1.
#CFLAGS+= -DGAMESPEED=0
#
#   This sets the number of frames to be drawn per second.  The digit after the
# ='s means that the game will draw n frames per second, where n is that digit.
# The default value is 60.
#CFLAGS+= -DFPS=60
#
#   This sets the number of verticle retraces per second.  The digit after the
# ='s means that the game will call an interrupt up to n retraces per second,
# where n is that digit. The default value is 60.  Possible values are 60 for
# NTSC simulation and 50 for PAL simulation.
# 
CFLAGS+= -DHERTZ=60
#CFLAGS+= -DHERTZ=50
#
#
#   NODELAY removes the delay for interrupts, OLDGFX uses the old blitting
# engine.
#
#CFLAGS+= -DNODELAY
#CFLAGS+= -DOLDGFX



#END OF CONFIG#################################################################

VERSIONFLAGS = -DVERSION_MAJOR=\"$(VERSION_MAJOR)\" \
	       -DVERSION_MINOR=\"$(VERSION_MINOR)\"

TARGETS=exe

ifeq ($(TARGET),Linux)
	CC = cc
else
	#win32
	CC=i586-mingw32msvc-gcc
	EXE=.exe
	#   Uncoment to compile with the win32 option (as opposed to using make
	# win32)
	#MACRO=-DWIN32
	SDL_BINPATH=/usr/i586-mingw32msvc/bin/
endif

ifdef RELEASE
OPT_CFLAGS=-mpentium -march=pentium -fstrength-reduce -fomit-frame-pointer \
           -frerun-cse-after-loop -funroll-loops -ffast-math \
	   -malign-functions=5 -malign-jumps=5 -malign-loops=5 -malign-double \
	   -fno-exceptions -fno-rtti -O3
TARGETS=clean exe strip
endif

ifdef DEBUG
FDEBUG=-g
CFLAGS+= -DDEBUG
endif

SDL_CFLAGS = $(shell $(SDL_BINPATH)sdl-config --cflags)
SDL_LIBS   = $(shell $(SDL_BINPATH)sdl-config --libs)

CFLAGS += $(FDEBUG) -Wall $(SDL_CFLAGS) $(OPT_CFLAGS) $(MACRO) $(VERSIONFLAGS)
LIBS=$(SDL_LIBS)

EXECUTABLE = si$(EXE)

linux: default
all: default
default: $(TARGETS)

w32: windows
win32: windows
windows: 
	@$(MAKE) clean TARGET=WIN32 strip

clean:
	rm -f $(SRCDIR)/*.o $(EXECUTABLE) $(EXECUTABLE).exe -r dist

debug:
	@$(MAKE) DEBUG=1

release:
	@$(MAKE) RELEASE=1

dist:
	mkdir dist
	mkdir dist/siemu-$(VERSION_MAJOR).$(VERSION_MINOR)
	cp -Rp bin si* samp doc res/si.ico Makefile src patch \
		  dist/siemu-$(VERSION_MAJOR).$(VERSION_MINOR)
	mkdir dist/siemu-$(VERSION_MAJOR).$(VERSION_MINOR)/roms
	mkdir dist/siemu-$(VERSION_MAJOR).$(VERSION_MINOR)/roms/invaders
	mkdir dist/siemu-$(VERSION_MAJOR).$(VERSION_MINOR)/roms/spaceatt
	mkdir dist/siemu-$(VERSION_MAJOR).$(VERSION_MINOR)/roms/invrvnge
	cd dist && tar -zcvf siemu-$(VERSION_MAJOR).$(VERSION_MINOR).tar.gz \
		  siemu-$(VERSION_MAJOR).$(VERSION_MINOR)
	cd dist && zip -r -9 siemu-$(VERSION_MAJOR).$(VERSION_MINOR).zip \
		  siemu-$(VERSION_MAJOR).$(VERSION_MINOR)

strip: exe
	$(SDL_BINPATH)strip $(EXECUTABLE)
	upx $(EXECUTABLE)

exe: $(EXECUTABLE)
$(EXECUTABLE): $(patsubst %.c,%.o,$(wildcard $(SRCDIR)/*.c))
	$(CC) -o $@ $^ $(LIBS)
