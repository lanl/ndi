######################################################################
#
# Makefile system dependent definitions for libndi
#
######################################################################

# CONFIG = alpha-tru64

# Standard macros
CC		:= cc
FC		:= f77
CFLAGS		:= -Wall
FFLAGS		:= -g -extend_source -cpp
ifdef DEBUG
CFLAGS		+= -g
else
CFLAGS		+= -O
endif
ASFLAGS		:= $(CFLAGS)
CPPFLAGS	:= -I.
LDFLAGS		:= -L.
LDLIBS		:= -lndi

# defines
CPPFLAGS	+= -DTRU64
FLOAT32FLAGS    := -DFLOAT32
FLOAT64FLAGS    := -DFLOAT64
NO_NAME_SUB_FLAG:= -D__NDI_NO_NAME_SUB
STANDALONE_FLAG	:= -D__STANDALONE
#CPPFLAGS	+= -DPURIFY
ifndef DEBUG
CPPFLAGS	+= -DNDEBUG
endif
