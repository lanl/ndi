######################################################################
#
# Makefile system dependent definitions for libndi
#
######################################################################

# CONFIG = cray

# Standard macros
CC		:= cc
FC		:= cf77
CFLAGS		:= 
FFLAGS		:= -Wp"-F"
ASFLAGS		:= $(CFLAGS)

# defines
CPPFLAGS	+= -D CRAY
FLOAT32FLAGS    := -D FLOAT32
FLOAT64FLAGS    := -D FLOAT64
NO_NAME_SUB_FLAG:= -D __NDI_NO_NAME_SUB
STANDALONE_FLAG	:= -D __STANDALONE
