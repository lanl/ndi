include(Fortran)

set(module_dir "${CMAKE_CURRENT_BINARY_DIR}/fortran_modules")

add_library(testlib STATIC "test_mod.F90")
target_Fortran_module_dir(testlib PRIVATE "${module_dir}")

set(test_module_dir "${module_dir}")
if( CMAKE_CONFIGURATION_TYPES )
  set(test_module_dir "${module_dir}/${CMAKE_BUILD_TYPE}")
endif()

add_test(
  NAME Fortran.ModuleDirectory
  COMMAND ${CMAKE_COMMAND} -D module_file=${test_module_dir}/test_mod.mod -P ${CMAKE_CURRENT_LIST_DIR}/check_exists.cmake
)
