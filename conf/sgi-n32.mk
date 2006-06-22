######################################################################
#
# Makefile system dependent definitions for libndi
#
######################################################################

# CONFIG = sgi-n32

# Standard macros
CC		:= cc
FC		:= f77
CFLAGS		:= -n32
FFLAGS          := -n32 -col120
ASFLAGS		:= $(CFLAGS)

# defines
CPPFLAGS	+= -DSGI
FLOAT32FLAGS    := -DFLOAT32
FLOAT64FLAGS    := -DFLOAT64
NO_NAME_SUB_FLAG:= -D__NDI_NO_NAME_SUB
STANDALONE_FLAG	:= -D__STANDALONE
