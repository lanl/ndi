######################################################################
#
# Makefile system dependent definitions for libndi
#
######################################################################

# CONFIG = sun

# Standard macros
CC		:= cc
FC		:= f77
CFLAGS		:=
FFLAGS		:= -e
ASFLAGS		:= $(CFLAGS)

# defines
CPPFLAGS	+= -DSUN
FLOAT32FLAGS    := -DFLOAT32
FLOAT64FLAGS    := -DFLOAT64
NO_NAME_SUB_FLAG:= -D__NDI_NO_NAME_SUB
STANDALONE_FLAG	:= -D__STANDALONE
