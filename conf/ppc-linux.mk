######################################################################
#
# Makefile system dependent definitions for libndi
#
######################################################################

# CONFIG = ppc-linux

# Standard macros
CC		:= mpixlc_r
FC		:= mpixlf95
CFLAGS		:= -qcheck=all -qarch=auto
CFLAGS		+= -qspill=10000 -qmaxmem=15000
CFLAGS		+= -qlanglvl=stdc99

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
FFLAGS		:= -qfixed=132
ASFLAGS		:= $(CFLAGS)

# defines
CPPFLAGS	+= -DLINUX
FLOAT32FLAGS	:= -DFLOAT32
FLOAT64FLAGS	:= -DFLOAT64
NO_NAME_SUB_FLAG:= -D__NDI_NO_NAME_SUB
STANDALONE_FLAG	:= -D__STANDALONE
