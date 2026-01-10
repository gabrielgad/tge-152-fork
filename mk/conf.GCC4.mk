include ../mk/conf.GCC.mk

COMPILER.c      =gcc
COMPILER.cc     =g++ -Wno-invalid-offsetof

# Removed -mtune=i686 for x86-64 compatibility
# CFLAGS += -mtune=i686
