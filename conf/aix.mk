######################################################################
#
# Makefile system dependent definitions for libndi
#
######################################################################

# CONFIG = aix

# Standard macros
CC		:= cc
FC		:= xlf
CFLAGS		:= -qcheck=all -qarch=auto
#CFLAGS		+= -q32
#CFLAGS		+= -qwarn64
#CFLAGS		+= -qinfo
#CFLAGS		+= -qprofile=p
#CFLAGS		+= -qprofile=p
#CFLAGS		+= -qalias=noaryovrlp
#CFLAGS		+= -qzerosize
#CFLAGS		+= -qflag-i:i
#CFLAGS		+= -qsuppress=1501-510:1518-099
#CFLAGS		+= -qlanglvl=90pure
FC		= xlf
FFLAGS		:= -qfixed=132
ifdef DEBUG
CFLAGS		+= -g
FFLAGS		+= -g
else
CFLAGS		+= -O
FFLAGS		+= -O
endif
ASFLAGS		:= $(CFLAGS)
CPPFLAGS	:= -I.
LDFLAGS		:= -L .
LDLIBS		:= -lndi -lm

# defines
CPPFLAGS	+= -DAIX
FLOAT32FLAGS	:= -DFLOAT32
FLOAT64FLAGS	:= -DFLOAT64
NO_NAME_SUB_FLAG:= -D__NDI_NO_NAME_SUB
STANDALONE_FLAG	:= -D__STANDALONE
CPPFLAGS	+= -D__NDI_NO_NAME_SUB
#CPPFLAGS	+= -DPURIFY
ifndef DEBUG
CPPFLAGS	+= -DNDEBUG
endif
