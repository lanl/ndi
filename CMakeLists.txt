cmake_minimum_required(VERSION 3.12)

set(subproject OFF)
if(DEFINED PROJECT_NAME)
  set(subproject ON)
endif()

project(ndi
  VERSION 2.2.0
  DESCRIPTION "Library Abstraction for NDI-format Nuclear Data"
  HOMEPAGE_URL "https://xcp-stash.lanl.gov/projects/NDT/repos/ndi"
  LANGUAGES C Fortran)

set(CMAKE_DEBUG_POSTFIX "-debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/.cmake")

include(CTest)
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(Warnings)

#
# NDI has no dependencies, but the specification block is provided for
# consistency with other projects.
#
# The file referenced in this block used by version branches of
# ndi to fix the version of the dependencies found by the
# find_package overload provided by Git Submodule Packages.
#
if(EXISTS "${PROJECT_SOURCE_DIR}/cmake/specification.cmake")
  set(git.submodule.packages.specification
    "${PROJECT_SOURCE_DIR}/cmake/specification.cmake" CACHE FILEPATH
    "(READ ONLY) Path to consumed git submodule packages specification file" FORCE)
  include("${PROJECT_SOURCE_DIR}/cmake/specification.cmake")
endif()

option(ndi.shared_library
  "Build the ndi unit tests and integrate with ctest"
  "${BUILD_SHARED_LIBS}")

CMAKE_DEPENDENT_OPTION(ndi.position_independent_code
  "Build the ndi library with position independent code"
  "${CMAKE_POSITION_INDEPENDENT_CODE}"
  "NOT ndi.shared_library" ON)

option(ndi.interprocedural_optimizations
  "Build the ndi library with interprocedural/link-time optimizations"
  ${CMAKE_INTERPROCEDURAL_OPTIMIZATION})

option(ndi.install.rpath_use_link_path
  "Retain the ndi build linker paths in installation binary rpath"
  ${CMAKE_INSTALL_RPATH_USE_LINK_PATH})

CMAKE_DEPENDENT_OPTION(ndi.tests
  "Build the ndi tests and integrate with ctest"
  ON "BUILD_TESTING; NOT subproject" OFF)

add_library(ndi_float32 OBJECT "")
add_library(ndi_float64 OBJECT "")
add_library(ndi_C OBJECT "")
add_library(ndi_F OBJECT "")

if(ndi.shared_library)
  set(library_type SHARED)
else()
  set(library_type STATIC)
endif()

add_library(ndi ${library_type}
  $<TARGET_OBJECTS:ndi_float32>
  $<TARGET_OBJECTS:ndi_float64>
  $<TARGET_OBJECTS:ndi_C>
  $<TARGET_OBJECTS:ndi_F>)

add_library(ndi::ndi ALIAS ndi)

set_target_properties(ndi ndi_C ndi_F ndi_float32 ndi_float64 PROPERTIES
  POSITION_INDEPENDENT_CODE "${ndi.position_independent_code}"
  INTERPRODECURAL_OPTIMIZATION "${ndi.interprocedural_optimization}"
  C_EXTENSIONS OFF
  C_STANDARD 99
  INTERFACE_C_STANDARD 99
  WARN_ALL ON
  WARN_ERROR ON
  LLVM_DISABLED_WARNINGS "unused-parameter"
  GNU_DISABLED_WARNINGS "unused-parameter"
  MSVC_DISABLED_WARNINGS "4267;4100;4244;4706;4127;4305;4456;4295;4701;4703;4996"
  Intel_DISABLED_WARNINGS "3656")

set_target_properties(ndi PROPERTIES
  MACOSX_RPATH ON
  SKIP_BUILD_RPATH OFF
  BUILD_WITH_INSTALL_RPATH OFF
  INSTALL_RPATH_USE_LINK_PATH "${ndi.install.rpath_use_link_path}")

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  target_compile_definitions(ndi PUBLIC _CRT_SECURE_NO_WARNINGS)
  target_compile_definitions(ndi_C PUBLIC _CRT_SECURE_NO_WARNINGS)
  target_compile_definitions(ndi_float32 PUBLIC _CRT_SECURE_NO_WARNINGS)
  target_compile_definitions(ndi_float64 PUBLIC _CRT_SECURE_NO_WARNINGS)
else()
  target_link_libraries(ndi PUBLIC m)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(install_rpath "@loader_path")
else()
  set(install_rpath "\\$ORIGIN")
endif()

set_property(TARGET ndi APPEND PROPERTY INSTALL_RPATH "${install_rpath}")

foreach(C_target IN ITEMS ndi ndi_C ndi_F ndi_float32 ndi_float64)
  target_compile_options(${C_target} PRIVATE
    $<TARGET_PROPERTY:shacl::cmake::Warnings_C,INTERFACE_COMPILE_OPTIONS>)
endforeach()

target_compile_definitions(ndi_float32 PRIVATE FLOAT32)
target_compile_definitions(ndi_float64 PRIVATE FLOAT64)

include(src/CMakeLists.txt)

install(FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/ndi.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/ndi_keywords.h"
  "${CMAKE_CURRENT_BINARY_DIR}/ndi_keywords.i"
  "${CMAKE_CURRENT_BINARY_DIR}/ndi_var_type.h"
  "${CMAKE_CURRENT_BINARY_DIR}/ndi_version.h"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(TARGETS ndi
  EXPORT ndiConfig
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

install(EXPORT ndiConfig
  FILE "ndi-config.cmake"
  NAMESPACE ndi::
  DESTINATION share/cmake/ndi)

write_basic_package_version_file("ndi-config-version.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion)

install(FILES
  "${PROJECT_BINARY_DIR}/ndi-config-version.cmake"
  DESTINATION share/cmake/ndi)

if(NOT subproject)
  include(InstallRequiredSystemLibraries)
  set(CPACK_PACKAGE_VENDOR "Los Alamos National Laboratory" )
  set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
  set(CPACK_PACKAGE_CONTACT "tgsaller@lanl.gov")
  include(CPack)
endif()
