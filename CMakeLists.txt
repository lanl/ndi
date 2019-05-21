cmake_minimum_required(VERSION 3.12)

if( DEFINED PROJECT_NAME )
  set( subproject ON )
else()
  set( subproject OFF )
endif()

project(ndi 
  VERSION 2.1.3
  DESCRIPTION "Library Abstraction for NDI-format Nuclear Data"
  LANGUAGES C Fortran)

include(CTest)
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)

list(APPEND CMAKE_MODULE_PATH "${ndi_SOURCE_DIR}/.cmake")
include(Warnings)

option(ndi.shared_library
  "Build the ndi unit tests and integrate with ctest"
  "${BUILD_SHARED_LIBS}")

CMAKE_DEPENDENT_OPTION(ndi.position_independent_code
  "Build the ndi library with position independent code"
  "${position_independent_code}"
  "NOT ndi.shared_library" ON)

CMAKE_DEPENDENT_OPTION(ndi.tests
  "Build the ndi tests and integrate with ctest"
  ON "BUILD_TESTING; NOT subproject" OFF)

add_library(ndi_float32 OBJECT "")
add_library(ndi_float64 OBJECT "")
add_library(ndi_C OBJECT "")
add_library(ndi_F OBJECT "")

if(ndi.shared_library)
  add_library(ndi SHARED
    $<TARGET_OBJECTS:ndi_float32>
    $<TARGET_OBJECTS:ndi_float64>
    $<TARGET_OBJECTS:ndi_C>
    $<TARGET_OBJECTS:ndi_F>)
else()
  add_library(ndi STATIC
    $<TARGET_OBJECTS:ndi_float32>
    $<TARGET_OBJECTS:ndi_float64>
    $<TARGET_OBJECTS:ndi_C>
    $<TARGET_OBJECTS:ndi_F>)
endif()

add_library(ndi::ndi ALIAS ndi)
target_link_libraries(ndi PUBLIC m)

set_target_properties(ndi ndi_C ndi_F ndi_float32 ndi_float64 PROPERTIES
  POSITION_INDEPENDENT_CODE "${ndi.position_independent_code}"
  C_EXTENSIONS OFF
  C_STANDARD 99
  INTERFACE_C_STANDARD 99
  WARN_ALL ON
  WARN_ERROR ON
  LLVM_DISABLED_WARNINGS "unused-parameter"
  GNU_DISABLED_WARNINGS "unused-parameter")

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
  DESTINATION include/ndi)

install(TARGETS ndi
  EXPORT ndiConfig
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin)

install(EXPORT ndiConfig
  FILE ndiConfig.cmake
  NAMESPACE ndi::
  DESTINATION share/cmake/ndi)

write_basic_package_version_file("ndiConfigVersion.cmake"
  VERSION ${ndi_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES
  "${ndi_BINARY_DIR}/ndiConfigVersion.cmake"
  DESTINATION share/cmake/ndi)

if(NOT subproject)
  include( InstallRequiredSystemLibraries )
  set( CPACK_PACKAGE_VENDOR "Los Alamos National Laboratory" )
  set( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" )
  set( CMAKE_PROJECT_HOMEPAGE_URL "https://xcp-stash.lanl.gov/projects/NDT/repos/ndi/browse" )
  set( CPACK_PACKAGE_CONTACT "amccartney@lanl.gov" )
  include( CPack )
endif()
