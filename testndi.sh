#!/bin/sh

BUILD_DIR=build-`gnu-os`
(cd $BUILD_DIR; gmake check )

DEBUG_DIR=debug-`gnu-os`
(cd $DEBUG_DIR; gmake DEBUG=1 check )
