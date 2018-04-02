cmake_minimum_required(VERSION 3.9)
get_directory_property(is_subproject PARENT_DIRECTORY)

project(ndi VERSION 1.1.0 LANGUAGES C Fortran)
include(CTest)
include(CMakeDependentOption)
include(FortranCInterface)

set(ENV{NDIROOT} "/usr/projects/data/nuclear/ndi")
set(ENV{NDI_VERSION_DATA}  "2.1.3beta")
#set(ENV{SRCDIR} ".")
set(ENV{TOP_SRCDIR} "/usr/projects/lindet/tgsaller/ndi/clean/ndi")

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
  add_library(ndi_C_32 SHARED "")
  add_library(ndi_C_64 SHARED "")
  add_library(ndi_F SHARED "")
else()
  add_library(ndi_C_32 STATIC "")
  add_library(ndi_C_64 STATIC "")
  add_library(ndi_F STATIC "")
endif()

target_link_libraries(ndi_F PUBLIC ndi_C_32 ndi_C_64)
target_link_libraries(ndi_C_32 PUBLIC m)
target_link_libraries(ndi_C_64 PUBLIC m)

add_library(ndi::ndi_C_32 ALIAS ndi_C_32)
add_library(ndi::ndi_C_64 ALIAS ndi_C_64)
add_library(ndi::ndi_F ALIAS ndi_F)

set_target_properties(ndi_C_32 PROPERTIES
  POSITION_INDEPENDENT_CODE "${ndi_position_independent_code}")

set_target_properties(ndi_C_64 PROPERTIES
  POSITION_INDEPENDENT_CODE "${ndi_position_independent_code}")

set_target_properties(ndi_F PROPERTIES
  POSITION_INDEPENDENT_CODE "${ndi_position_independent_code}")

target_compile_features(ndi_C_32 PUBLIC c_std_99)
target_compile_features(ndi_C_64 PUBLIC c_std_99)

set_target_properties(ndi_C_32 PROPERTIES C_EXTENSIONS OFF)
set_target_properties(ndi_C_64 PROPERTIES C_EXTENSIONS OFF)

target_compile_definitions(ndi_C_32 PUBLIC __STANDALONE)
target_compile_definitions(ndi_C_64 PUBLIC __STANDALONE)
target_compile_definitions(ndi_F PUBLIC __STANDALONE)
target_compile_definitions(ndi_C_32 PUBLIC FLOAT32)
target_compile_definitions(ndi_C_64 PUBLIC FLOAT64)
target_compile_definitions(ndi_F PUBLIC FLOAT32 FLOAT64)

include(src/CMakeLists.txt)

get_target_property(NDI_SOURCES ndi_C_32 SOURCES)
set_target_properties(ndi_C_64 PROPERTIES SOURCES "${NDI_SOURCES}")

#[===[
FortranCInterface_HEADER(fmangle.h SYMBOL_NAMESPACE "F_" SYMBOLS
  NDI_INIT 
  NDID_GET_SIZE
  NDID_GET_NUM
  NDID_GET_STRING_VEC
  NDI_INIT
  NDI_CLOSE
  NDI_SET_OPTION
  NDI_CHECK_OPTION
  NDI_SET_ZAID
  NDI_SET_ZAID_FLOAT32_TEMP
  NDI_SET_ZAID_FLOAT64_TEMP
  NDI_SET_ISOTOPE
  NDI_SET_REACTION
  NDI_GET_SIZE
  NDI_GET_SIZE_N
  NDI_GET_SIZE_X
  NDI_GET_INT_VAL
  NDI_GET_INT_VAL_N
  NDI_GET_INT_VEC
  NDI_GET_INT_VEC_N
  NDI_GET_INT_VEC_X
  NDI_GET_FLOAT64_VAL
  NDI_GET_FLOAT64_VAL_N
  NDI_GET_FLOAT64_VEC
  NDI_GET_FLOAT64_VEC_N
  NDI_GET_FLOAT64_VEC_X
  NDI_GET_FLOAT32_VAL
  NDI_GET_FLOAT32_VAL_N
  NDI_GET_FLOAT32_VEC
  NDI_GET_FLOAT32_VEC_N
  NDI_GET_FLOAT32_VEC_X
  NDI_GET_STRING_VAL
  NDI_GET_STRING_VAL_N
  NDI_GET_STRING_VEC
  NDI_GET_STRING_VEC_X
  NDID_ZAID_FLOAT64_VEC
  NDID_ZAID_FLOAT32_VEC
  NDI2_OPEN_GE  NDIR
  NDI2_SET_OPTION_GE  NDIR
  NDI2_CHECK_OPTION_GE  NDIR
  NDI2_CLOSE_GE  NDIR
  NDI2_SET_ZAID
  NDI2_SET_ZAID_FLOAT32_TEMP
  NDI2_SET_ZAID_FLOAT64_TEMP
  NDI2_SET_ISOTOPE
  NDI2_SET_REACTION
  NDI2_MAKE_HANDLE
  NDI2_GET_SIZE
  NDI2_GET_SIZE_N
  NDI2_GET_SIZE_X
  NDI2_SET_OPTION
  NDI2_SET_FLOAT32_VEC_OPTION
  NDI2_SET_FLOAT64_VEC_OPTION
  NDI2_CHECK_OPTION
  NDI2_GET_INT_VAL
  NDI2_GET_INT_VAL_N
  NDI2_GET_INT_VEC
  NDI2_GET_INT_VEC_N
  NDI2_GET_INT_VEC_X
  NDI2_GET_FLOAT32_VAL
  NDI2_GET_FLOAT32_VAL_N
  NDI2_GET_FLOAT32_VEC
  NDI2_GET_FLOAT32_VEC_N
  NDI2_GET_FLOAT32_VEC_X
  NDI2_GET_FLOAT64_VAL
  NDI2_GET_FLOAT64_VAL_N
  NDI2_GET_FLOAT64_VEC
  NDI2_GET_FLOAT64_VEC_N
  NDI2_GET_FLOAT64_VEC_X
  NDI2_GET_STRING_VAL
  NDI2_GET_STRING_VAL_N
  NDI2_GET_STRING_VEC
  NDI2_GET_STRING_VEC_X)
#]===]
