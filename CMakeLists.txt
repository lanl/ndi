cmake_minimum_required(VERSION 3.9)
get_directory_property(is_subproject PARENT_DIRECTORY)

project(ndi VERSION 1.1.0 LANGUAGES C Fortran)
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

add_library(ndi_float32 OBJECT "")
add_library(ndi_float64 OBJECT "")
add_library(ndi_C OBJECT "")
add_library(ndi_F OBJECT "")

if(ndi_shared_library)
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

set_propertY(TARGET ndi ndi_C ndi_F ndi_float32 ndi_float64 PROPERTY
  POSITION_INDEPENDENT_CODE "${ndi_position_independent_code}")

set_property(TARGET ndi ndi_C ndi_C ndi_F ndi_float32 ndi_float64 PROPERTY
  C_STANDARD 99)

set_property(TARGET ndi ndi_C ndi_C ndi_F ndi_float32 ndi_float64 PROPERTY
  C_EXTENSIONS OFF)

target_compile_definitions(ndi_float32 PRIVATE FLOAT32)
target_compile_definitions(ndi_float64 PRIVATE FLOAT64)

include(src/CMakeLists.txt)
