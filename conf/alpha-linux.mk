######################################################################
#
# Makefile system dependent definitions for libndi
#
######################################################################

# CONFIG = alpha-linux

# Standard macros
CC		:= cc
FC		:= f77
CFLAGS		:= -Wall
ifdef DEBUG
CFLAGS		+= -g
FFLAGS		:= -g
else
CFLAGS		+= -O
FFLAGS		:= -O
endif
ASFLAGS		:= $(CFLAGS)
CPPFLAGS	:= -I.
LDFLAGS		:= -L .
LDLIBS		:= -lndi

# defines
CPPFLAGS	+= -DLINUX
FLOAT32FLAGS    := -DFLOAT32
FLOAT64FLAGS    := -DFLOAT64
NO_NAME_SUB_FLAG:= -D__NDI_NO_NAME_SUB
STANDALONE_FLAG	:= -D__STANDALONE
#CPPFLAGS	+= -DPURIFY
ifndef DEBUG
CPPFLAGS	+= -DNDEBUG
endif
