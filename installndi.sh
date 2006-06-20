#!/bin/sh

GROUP=pdata
GNU_OS_OUT=`./gnu-os`
BUILD_DIR=build-$GNU_OS_OUT
DEBUG_DIR=debug-$GNU_OS_OUT

INSTALL_DIR=`(cd $BUILD_DIR; gmake print_install | tail -1)`

if [ -d $INSTALL_DIR ]; then
    echo -n "Directory $INSTALL_DIR already exists.  Overwrite?  "
    read ANSWER 
    case $ANSWER in
        n*|N*)
            exit 0
            ;;
        y*|Y*)
            ;;
        *)
            exit 0;;
    esac
else
    umask 002
    mkdir $INSTALL_DIR
fi

(cd $BUILD_DIR; gmake install)
(cd $DEBUG_DIR; gmake DEBUG=1 install)
