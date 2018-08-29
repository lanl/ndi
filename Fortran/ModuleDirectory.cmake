function(target_Fortran_module_dir target linkage dir)
  file(MAKE_DIRECTORY "${dir}")

  if( CMAKE_CONFIGURATION_TYPES )
    target_include_directories("${target}" "${linkage}" "${dir}/$<CONFIG>")
  else()
    target_include_directories("${target}" "${linkage}" "${dir}")
  endif()

  set_target_properties("${target}" PROPERTIES
    Fortran_MODULE_DIRECTORY "${dir}"
  )
endfunction()
