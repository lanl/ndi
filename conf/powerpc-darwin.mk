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
ifdef DEBUG
CFLAGS		+= -g
FFLAGS		+= -g
else
CFLAGS		+= -O
FFLAGS		+= -O
endif
ASFLAGS		:= $(CFLAGS)
CPPFLAGS	:= -I.
LDFLAGS		:= -L.
LDLIBS		:= -lndi -lm

# defines
CPPFLAGS	+= -DDARWIN
FLOAT32FLAGS    := -DFLOAT32
FLOAT64FLAGS    := -DFLOAT64
NO_NAME_SUB_FLAG:= -D__NDI_NO_NAME_SUB
STANDALONE_FLAG	:= -D__STANDALONE
ifndef DEBUG
CPPFLAGS	+= -DNDEBUG
endif
