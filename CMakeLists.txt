cmake_minimum_required(VERSION 3.9)
get_directory_property(is_subproject PARENT_DIRECTORY)

project(ndi VERSION 1.1.0 LANGUAGES C Fortran)
include(CTest)
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)

option(ndi.shared_library
  "Build the ndi unit tests and integrate with ctest"
  "${BUILD_SHARED_LIBS}")

CMAKE_DEPENDENT_OPTION(ndi.position_independent_code
  "Build the ndi library with position independent code"
  "${position_independent_code}"
  "NOT ndi.shared_library" ON)

CMAKE_DEPENDENT_OPTION(ndi.tests
  "Build the ndi tests and integrate with ctest"
  ON "BUILD_TESTING; NOT is_subproject" OFF)

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

set_propertY(TARGET ndi ndi_C ndi_F ndi_float32 ndi_float64 PROPERTY
  POSITION_INDEPENDENT_CODE "${ndi.position_independent_code}")

set_property(TARGET ndi ndi_C ndi_C ndi_F ndi_float32 ndi_float64 PROPERTY
  C_STANDARD 99)

set_property(TARGET ndi ndi_C ndi_C ndi_F ndi_float32 ndi_float64 PROPERTY
  C_EXTENSIONS OFF)

include(CheckCCompilerFlag)

list(APPEND ndi.warning_flags "-pedantic" "-Wall" "-Werror" "-Wno-error=strict-overflow")
foreach(flag IN LISTS ndi.warning_flags)
  check_c_compiler_flag("${flag}" flag_available)
  if(${flag_available})
    set_property(TARGET ndi ndi_C ndi_F ndi_float32 ndi_float64 APPEND PROPERTY COMPILE_OPTIONS ${flag})
  endif()
endforeach()

target_compile_definitions(ndi_float32 PRIVATE FLOAT32)
target_compile_definitions(ndi_float64 PRIVATE FLOAT64)

include(src/CMakeLists.txt)

install(TARGETS ndi
  EXPORT ndiConfig
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ GROUP_WRITE WORLD_READ)

install(EXPORT ndiConfig
  FILE ndiConfig.cmake
  NAMESPACE ndi::
  DESTINATION share
  PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ GROUP_WRITE WORLD_READ)

install(FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ndi.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ndi_keywords.h
  ${CMAKE_CURRENT_BINARY_DIR}/src/ndi_keywords.i
  ${CMAKE_CURRENT_BINARY_DIR}/src/ndi_var_type.h
  DESTINATION include/ndi
  PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ GROUP_WRITE WORLD_READ)

write_basic_package_version_file("ndiConfigVersion.cmake"
  VERSION ${ndi_VERSION}
  COMPATIBILITY SameMajorVersion)
