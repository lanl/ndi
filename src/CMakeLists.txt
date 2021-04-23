add_custom_command(
  OUTPUT ndi_keywords.i
  COMMAND ${CMAKE_COMMAND}
    -D source_dir="${ndi_SOURCE_DIR}/src"
    -D binary_dir="${ndi_BINARY_DIR}"
    -P ${ndi_SOURCE_DIR}/cmake/split.cmake
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

add_custom_target(generate_ndi_keywords ALL DEPENDS ndi_keywords.i)

add_executable(ndi_define_float
  ${CMAKE_CURRENT_LIST_DIR}/ndi_define_float.c)

add_custom_command(
  OUTPUT ndi_var_type.h
  COMMAND $<TARGET_FILE:ndi_define_float> > ndi_var_type.h) #NOTE: probably not portable

add_custom_target(generate_ndi_var_type DEPENDS ndi_var_type.h)


# Seperate the C and Fortran code into disparate CMake targets for windows compatibility.
add_library(ndi_define_fmanglec STATIC)
target_sources(ndi_define_fmanglec PRIVATE ${CMAKE_CURRENT_LIST_DIR}/fcallscc.c)
add_executable(ndi_define_fmangle ${CMAKE_CURRENT_LIST_DIR}/fcallscf.f)
target_link_libraries(ndi_define_fmangle PRIVATE ndi_define_fmanglec)

add_custom_command(
  OUTPUT fmangle.h
  COMMAND $<TARGET_FILE:ndi_define_fmangle> > fmangle.h) #NOTE: probably not portable

add_custom_target(generate_fmangle
  DEPENDS fmangle.h)

file(WRITE ${PROJECT_BINARY_DIR}/ndi_version.h "#define NDI_VERSION_DATA \"${ndi_VERSION}\"")

add_dependencies(ndi_float32 generate_ndi_var_type)
add_dependencies(ndi_float64 generate_ndi_var_type)
add_dependencies(ndi_C generate_ndi_var_type)
add_dependencies(ndi_F generate_fmangle)

target_include_directories(ndi INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/>
  $<BUILD_INTERFACE:${ndi_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>)

target_sources(ndi_float32 PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/ndi_set_zaid_float_temp.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_set_zaid_float_temp_sig_0.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_collapse_float.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_collapse_mat.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_float_val.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_float_val_n.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_float_vec.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_float_vec_n.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_float_vec_x.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_misc_float.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_read_float_block.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndid_zaid_float_vec.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndimp_get_float_vec.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndimp_get_float_vec_n.c")

target_sources(ndi_float64 PRIVATE
  $<TARGET_PROPERTY:ndi_float32,SOURCES>)

target_sources(ndi_C PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/ndi_avail.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_bin_tree.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_check_option.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_close.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_close_lib_file.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_c_values.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndid_get_size.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndid_get_string_vec.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_element_index.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_fill_element.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_find_index.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_free.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_int_val.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_int_val_n.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_int_vec.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_int_vec_n.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_int_vec_x.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_size.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_size_n.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_size_x.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_string_val.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_string_val_n.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_string_vec.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_get_string_vec_x.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_handle.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_init.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_init_iso_state.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_init_state.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_interp_integ.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_malloc.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_misc.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_misc_depl.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_open_gendir.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_open_lib_file.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_parse_zaid.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_read_dir_block.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_read_int_block.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_read_rx_prod.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_read_string_block.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_set0.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_set_e_grps.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_set_isotope.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_set_float_vec_option.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_set_option.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_skip_rx_prod.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_skip_float_block.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_state.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_std_zaid.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_sza_symbol_translation.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_user_functions_handles.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_user_functions_no_h.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_zaid_dir.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndimp_get_int_val.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndimp_get_int_val_n.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndimp_get_int_vec.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndimp_get_size.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndimp_get_size_n.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndimp_get_string_val.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndimp_set_option.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndimp_set_zaid.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_watt.c"
  "${CMAKE_CURRENT_LIST_DIR}/ndi_sf.c")

target_sources(ndi_F PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/ndi_fortran.c")
target_include_directories(ndi_F PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR})

if(ndi.tests)
  add_executable(generate_u235bin
    ${CMAKE_CURRENT_LIST_DIR}/test/lib/ndi_generate_u235bin.c)

  add_custom_command(
    OUTPUT u235bin
    COMMAND $<TARGET_FILE:generate_u235bin>
    WORKING_DIRECTORY ${ndi_SOURCE_DIR}
    COMMENT "Generating U-235"
    VERBATIM)

  add_custom_target(
    generate_u235bin_dummy ALL
    DEPENDS u235bin)

  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test)
endif()
