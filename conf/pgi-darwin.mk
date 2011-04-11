######################################################################
#
# Makefile system dependent definitions for libndi
#
######################################################################

# CONFIG = pgi-darwin

# Standard macros
CC		:= pgcc
FC		:= pgfortran
CFLAGS		:= -pc 64
FFLAGS		:= -pc 64
ASFLAGS		:= $(CFLAGS)

# defines
CPPFLAGS	+= -DDARWIN
NO_NAME_SUB_FLAG:= -D__NDI_NO_NAME_SUB
FLOAT32FLAGS    := -DFLOAT32
FLOAT64FLAGS    := -DFLOAT64
STANDALONE_FLAG	:= -D__STANDALONE

