######################################################################
#
# Makefile system dependent definitions for libndi
#
######################################################################

# CONFIG = intel-linux

# Standard macros
CC		:= /usr/bin/gcc
FC		:= pgf95
CFLAGS		:= -Wall
ASFLAGS		:= $(CFLAGS)

# defines
CPPFLAGS	+= -DLINUX
NO_NAME_SUB_FLAG:= -D__NDI_NO_NAME_SUB
FLOAT32FLAGS    := -DFLOAT32
FLOAT64FLAGS    := -DFLOAT64
STANDALONE_FLAG	:= -D__STANDALONE
