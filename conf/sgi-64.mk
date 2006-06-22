######################################################################
#
# Makefile system dependent definitions for libndi
#
######################################################################

# CONFIG = sgi-64

# Standard macros
CC		:= cc
FC		:= f77
CFLAGS		:= -64
FFLAGS          := -64 -col120
ASFLAGS		:= $(CFLAGS)

# defines
CPPFLAGS	+= -DSGI
FLOAT32FLAGS    := -DFLOAT32
FLOAT64FLAGS    := -DFLOAT64
NO_NAME_SUB_FLAG:= -D__NDI_NO_NAME_SUB
STANDALONE_FLAG	:= -D__STANDALONE
