O=.obj
EXT.EXE=$(BUILD_SUFFIX).bin
EXT.LIB=$(BUILD_SUFFIX).a
PRE.LIBRARY.LIB=../lib/$(DIR.OBJ)/
PRE.ENGINE.LIB=../engine/$(DIR.OBJ)/

EXT.DLL=$(BUILD_SUFFIX).dll
EXT.DLE=$(BUILD_SUFFIX).dle
EXT.DEP=.d

MKDIR=mkdir -p $(@:/=)
RM=rm -f
RMDIR=rm -rf
CP=cp
LN=cp

COMPILER.c      =gcc
COMPILER.cc     =g++
COMPILER.asm    =nasm
LINK            =ar
LINK.cc         =ld

# Updated for modern 64-bit Linux - force C++03 for compatibility
# Added -fPIC for shared library builds (pytge.so)
CFLAGS.GENERAL    = -std=gnu++03 -DUSE_FILE_REDIRECT -I/usr/include -MD \
		    $(shell pkg-config freetype2 --cflags 2>/dev/null) -ffast-math -pipe \
		    -Wno-write-strings -fpermissive -Wno-narrowing -fPIC

CFLAGS.DEBUG      = -g -DTORQUE_DEBUG
CFLAGS.RELEASE    = -O2 -finline-functions -fomit-frame-pointer
CFLAGS.DEBUGFAST  = -O -g -finline-functions 

ASMFLAGS          = -f elf -dLINUX

LFLAGS.GENERAL    =
LFLAGS.RELEASE    =
LFLAGS.DEBUG      =

# Use system libraries
LINK.LIBS.VORBIS = -logg -lvorbis -ltheora

LINK.LIBS.GENERAL = $(LINK.LIBS.VORBIS) -lGLU -lGL -lSDL -lpthread -ldl -lfreetype

LINK.LIBS.TOOLS   = $(LINK.LIBS.VORBIS) -lGLU -lGL -lSDL -lpthread -ldl
LINK.LIBS.SERVER  = $(LINK.LIBS.VORBIS) -lpthread -lSDL
LINK.LIBS.RELEASE =  -lXft
LINK.LIBS.DEBUG   =  -lXft

PATH.H.SYS        =
