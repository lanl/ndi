#!/bin/sh

GNU_OS_OUT=`gnu-os`
BUILD_DIR=build-$GNU_OS_OUT
DEBUG_DIR=debug-$GNU_OS_OUT

if [ -r src/VERSION ] 
then
    ndi_version=`head -1  src/VERSION`
else
    echo "VERSION file not found."
    exit 1
fi

case $GNU_OS_OUT in 
    i686*linux*) 
        platform=intel-linux
        NDI_ROOT=/usr/projects/data/nuclear/ndi
        ;;
    alpha*osf*) 
        platform=alpha-tru64
        NDI_ROOT=/usr/projects/data/nuclear/ndi
        ;;
    mips*irix*) 
        platform=sgi-64
        NDI_ROOT=/usr/projects/data/nuclear/ndi
        ;;
    *sun-solaris*) 
        platform=sun
        NDI_ROOT=/usr/local/codes/data/nuclear/ndi
        ;;
esac

INSTALL_DIR=$NDI_ROOT/v$ndi_version/$platform
if [ -d $NDI_ROOT/v$ndi_version ]; then
    echo Do not know how to negate!
else
    umask 002
    mkdir $NDI_ROOT/v$ndi_version
fi

if [ -d $INSTALL_DIR ]; then
    echo -n "Directory $INSTALL_DIR already exists.  Overwrite?"
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

cd $BUILD_DIR
gmake prefix=$INSTALL_DIR install
cd ../$DEBUG_DIR/src
cp libndi.a $INSTALL_DIR/lib/libndi_debug.a
cd ../..

