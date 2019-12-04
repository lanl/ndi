#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ndi::ndi" for configuration ""
set_property(TARGET ndi::ndi APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(ndi::ndi PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib64/libndi.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ndi::ndi )
list(APPEND _IMPORT_CHECK_FILES_FOR_ndi::ndi "${_IMPORT_PREFIX}/lib64/libndi.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
