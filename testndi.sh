#!/bin/sh

GNU_OS_OUT=`./gnu-os`
BUILD_DIR=build-$GNU_OS_OUT
DEBUG_DIR=debug-$GNU_OS_OUT

(cd $BUILD_DIR; gmake check )
(cd $DEBUG_DIR; gmake DEBUG=1 check )
