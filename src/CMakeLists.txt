target_include_directories(ndi_C PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/>
  $<INSTALL_INTERFACE:"include/ndi">)

target_include_directories(ndi_F PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/>
  $<INSTALL_INTERFACE:"include/ndi">)

target_sources(ndi_F PRIVATE
  $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_LIST_DIR}/ndi_fortran.c>)

target_sources(ndi_F PUBLIC
  $<BUILD_INTERFACE:
    ${CMAKE_BINARY_DIR}/fmangle.h>)

target_sources(ndi_C PRIVATE
  $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_LIST_DIR}/full2diag.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_avail.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_bin_tree.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_c_values.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_check_option.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_close.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_close_lib_file.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_collapse_float.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_collapse_mat.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_define_float.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_element_index.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_fill_element.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_find_index.c
    #${CMAKE_CURRENT_LIST_DIR}/ndi_find_part_index.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_find_reaction_index.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_free.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_float_val.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_float_val_n.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_float_vec.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_float_vec_n.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_float_vec_x.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_int_val.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_int_val_n.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_int_vec.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_int_vec_n.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_int_vec_x.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_size.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_size_n.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_size_x.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_string_val.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_string_val_n.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_string_vec.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_get_string_vec_x.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_handle.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_init.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_init_iso_state.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_init_state.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_interp_integ.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_malloc.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_misc.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_misc_depl.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_misc_float.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_open_gendir.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_open_lib_file.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_parse_zaid.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_read_dir_block.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_read_float_block.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_read_int_block.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_read_rx_prod.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_read_string_block.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_set0.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_set_e_grps.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_set_float_vec_option.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_set_isotope.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_set_option.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_set_zaid_float_temp.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_sf.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_skip_float_block.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_skip_rx_prod.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_state.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_std_zaid.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_sza_symbol_translation.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_user_functions_handles.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_user_functions_no_h.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_watt.c
    ${CMAKE_CURRENT_LIST_DIR}/ndi_zaid_dir.c
    ${CMAKE_CURRENT_LIST_DIR}/ndid_get_size.c
    ${CMAKE_CURRENT_LIST_DIR}/ndid_get_string_vec.c
    ${CMAKE_CURRENT_LIST_DIR}/ndid_zaid_float_vec.c
    ${CMAKE_CURRENT_LIST_DIR}/ndimp_get_float_vec.c
    ${CMAKE_CURRENT_LIST_DIR}/ndimp_get_float_vec_n.c
    ${CMAKE_CURRENT_LIST_DIR}/ndimp_get_int_val.c
    ${CMAKE_CURRENT_LIST_DIR}/ndimp_get_int_val_n.c
    ${CMAKE_CURRENT_LIST_DIR}/ndimp_get_int_vec.c
    ${CMAKE_CURRENT_LIST_DIR}/ndimp_get_size.c
    ${CMAKE_CURRENT_LIST_DIR}/ndimp_get_size_n.c
    ${CMAKE_CURRENT_LIST_DIR}/ndimp_get_string_val.c
    ${CMAKE_CURRENT_LIST_DIR}/ndimp_set_option.c
    ${CMAKE_CURRENT_LIST_DIR}/ndimp_set_zaid.c> )

target_sources(ndi_C PUBLIC
  $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_LIST_DIR}/ndi.h
    ${CMAKE_CURRENT_LIST_DIR}/ndi_abundance.h
    ${CMAKE_CURRENT_LIST_DIR}/ndi_bin_tree.h
    ${CMAKE_CURRENT_LIST_DIR}/ndi_element_symbols.h
    ${CMAKE_CURRENT_LIST_DIR}/ndi_internal.h
    ${CMAKE_CURRENT_LIST_DIR}/ndi_keywords.h
    ${CMAKE_CURRENT_LIST_DIR}/ndi_sf.h
    ${CMAKE_CURRENT_LIST_DIR}/ndi_state.h
    ${CMAKE_CURRENT_LIST_DIR}/ndi_version.h
    ${CMAKE_CURRENT_LIST_DIR}/ndi_watt.h>)


