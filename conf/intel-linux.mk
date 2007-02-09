######################################################################
#
# Makefile system dependent definitions for libndi
#
######################################################################

# CONFIG = intel-linux

# Standard macros
CC		:= cc
FC		:= lf95
CFLAGS		:= -Wall -fPIC
ASFLAGS		:= $(CFLAGS)

# defines
CPPFLAGS	+= -DLINUX
NO_NAME_SUB_FLAG:= -D__NDI_NO_NAME_SUB
FLOAT32FLAGS    := -DFLOAT32
FLOAT64FLAGS    := -DFLOAT64
STANDALONE_FLAG	:= -D__STANDALONE
