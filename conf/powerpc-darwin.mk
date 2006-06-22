######################################################################
#
# Makefile system dependent definitions for libndi
#
######################################################################

# CONFIG = darwin

# Standard macros
CC		:= cc
FC		:= /Applications/Absoft/bin/f90
ABSOFT          =  /Applications/Absoft
CFLAGS		:=
FFLAGS		:=
ASFLAGS		:= $(CFLAGS)

# defines
CPPFLAGS	+= -DDARWIN
FLOAT32FLAGS    := -DFLOAT32
FLOAT64FLAGS    := -DFLOAT64
NO_NAME_SUB_FLAG:= -D__NDI_NO_NAME_SUB
STANDALONE_FLAG	:= -D__STANDALONE
