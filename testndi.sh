#!/bin/sh

GNU_OS_OUT=`./gnu-os`
BUILD_DIR=build-$GNU_OS_OUT

(cd $BUILD_DIR; gmake check )
