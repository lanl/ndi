cmake_minimum_required(VERSION 3.9)
get_directory_property(is_subproject PARENT_DIRECTORY)

project(ndi VERSION 1.1.0 LANGUAGES C)
include(CTest)
include(CMakeDependentOption)

option(ndi_shared_library
   "Build the ndi unit tests and integrate with ctest"
   "${BUILD_SHARED_LIBS}")

CMAKE_DEPENDENT_OPTION(ndi_position_independent_code
   "Build the ndi library with position independent code"
   "${position_independent_code}"
   "NOT ndi_shared_library" ON)

CMAKE_DEPENDENT_OPTION(ndi_tests
   "Build the ndi tests and integrate with ctest"
   ON "BUILD_TESTING; NOT is_subproject" OFF)

set(ndi_data_path "$ENV{NDI_DATA_PATH}" CACHE PATH
  "Path to the installation of the NDI-format nuclear data libraries")

if(ndi_shared_library)
  add_library(ndi_C SHARED "")
#  add_library(ndi_F SHARED "")
else()
  add_library(ndi_C STATIC "")
#  add_library(ndi_F STATIC "")
endif()

add_library(ndi::ndi_C ALIAS ndi_C)
#add_library(ndi::ndi_F ALIAS ndi_F)

#[===[
This
is
a
block
comment
#]===]

set_target_properties(ndi_C PROPERTIES
  POSITION_INDEPENDENT_CODE "${ndi_position_independent_code}")

#set_target_properties(ndi_F PROPERTIES
#  POSITION_INDEPENDENT_CODE "${ndi_position_independent_code}")

target_compile_features(ndi_C PUBLIC c_std_90)
set_target_properties(ndi_C PROPERTIES C_EXTENSIONS OFF)

target_compile_definitions(ndi_C PUBLIC FLOAT_TYPE=float)

include(src/CMakeLists.txt)
#include(src/test/CMakeLists.txt)

#add_subdirectory(src)
#add_subdirectory(

