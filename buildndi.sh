#!/bin/sh

GNU_OS_OUT=`./gnu-os`
BUILD_DIR=build-$GNU_OS_OUT

[ -d $BUILD_DIR ] || mkdir $BUILD_DIR;

(cd $BUILD_DIR; ../configure; gmake )
