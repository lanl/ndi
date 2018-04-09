This directory contains the source code for NDI, a nuclear data interface.

BUILDING NDI
====

## Prerequisite

+ CMake 3.9
+ A C compiler
+ A Fortran compiler

## Instructions

1. Acquire the source code

    ```
    git clone ssh://git@xcp-stash.lanl.gov:7999/ndt/ndi.git
    ```

1. Establish your environment

    On LANL platforms, this can be accomplished using the environment module system.

    ```
    module load cmake
    module load <your preferred compiler vendor>
    ```


1. Configure your build

    ```
    cd ndi
    mkdir build
    cd build
    cmake [options] ..
    ```
    
    Optionally, the configuration can be customized by calling to the `ccmake` text-based interface or `cmake-gui` graphical user interface.
    Notably, when building with a static configuration build tool---such as GNU Make or Google Ninja---it's advisable to set the `CMAKE_BUILD_TYPE` variable to either *Release* or *Debug*, according to your use case.
    In addition, for those intending to install NDI, the installation directory can be specified by setting the `CMAKE_INSTALLATION_PREFIX` variable.
    See the CMake [documentation](https://cmake.org/cmake/help/v3.9/manual/cmake.1.html) for advanced usage.


1. Build the source

    ```
    cmake --build .
    ```
    
    See the CMake build tool [documentation](https://cmake.org/cmake/help/v3.9/manual/cmake.1.html#build-tool-mode) for advanced usage.

1. Run the tests

    ```
    ctest
    ```
    
    See the CMake testing tool [documentation](https://cmake.org/cmake/help/v3.9/manual/ctest.1.html) for advanced usage.


1. (Optional) Install the NDI library.

    ```
    cmake --build . --target install
    ```
    
