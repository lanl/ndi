#!/bin/sh

BUILD_DIR=build-`gnu-os`
[ -d $BUILD_DIR ] || mkdir $BUILD_DIR;
(cd $BUILD_DIR; ../configure; gmake )

DEBUG_DIR=debug-`gnu-os`
[ -d $DEBUG_DIR ] || mkdir $DEBUG_DIR;
(cd $DEBUG_DIR; ../configure; gmake DEBUG=1 )
