######################################################################
#
# Makefile system dependent definitions for libndi
#
######################################################################

# CONFIG = x86_65-linux

# Standard macros
CC		:= /usr/bin/gcc
CPP		:= $(CC) -E
FC		:= pgf95
CFLAGS		:= -std=c99 -pedantic -Wall -Werror -fPIC
ASFLAGS		:= $(CFLAGS)

# defines
CPPFLAGS	+= -DLINUX 
NO_NAME_SUB_FLAG:= -D__NDI_NO_NAME_SUB
FLOAT32FLAGS    := -DFLOAT32
FLOAT64FLAGS    := -DFLOAT64
STANDALONE_FLAG	:= -D__STANDALONE
