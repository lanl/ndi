#!/bin/sh

GNU_OS_OUT=`./gnu-os`
BUILD_DIR=build-$GNU_OS_OUT
DEBUG_DIR=debug-$GNU_OS_OUT

[ -d $BUILD_DIR ] || mkdir $BUILD_DIR;
[ -d $DEBUG_DIR ] || mkdir $DEBUG_DIR;

(cd $BUILD_DIR; ../configure; gmake )
(cd $DEBUG_DIR; ../configure; gmake DEBUG=1 )
