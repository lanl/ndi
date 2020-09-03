list(APPEND c_tests 5 5_handle abundance_test abundance_test_handle avg_e avg_e2 c_handle chi chi_handle depl depl_handle depl2 depl2_handle depl_zaid ekerma ebounds edist err22 ex_avg_e ex_kerma1 ex_kerma2 handle_bashing kerma_example kerma_example_handle library_check mg_sec_part mpmg mpmg_grps mpmg2 ndid ndid_handle neut ngk_test ngk_test_handle nonfissionables nonfissionables_handle old_rprod photon pnu pnu_handle radchem radchem_handle sgndf symbol_translate symbol_translate_handle testc tn3 tn3_handle tn4 tn4_handle tn5 tn5_handle tn_c tn_c_handle tn_chain tn_chain_handle tn_interp_params tn_interp_params_handle tn_new tn_new_handle tn_simple tn_simple_handle tn_use tn_use_handle tnreac_c tnreac_c_handle transfer_mat transpose_scat za za_handle gendir_handle ex_gendir_path gendir_path qsf_kerma zaid_temp_sig0 binary) 
list(APPEND f_tests ex_avg_ef ex_kerma1f ex_kerma2f f_handlef f_handlef2 library_checkf mpmgf  mpmg2f ndi_testf ndi_testf2 ndidf  ndidf_handle radchemf symbol_translatef transpose_scatf 21grp_ck gendir_handlef ex_gendir_pathf gendir_pathf zaid_tempf zaid_temp_sig_0)

set(top_srcdir ${ndi_SOURCE_DIR})
configure_file(gendir.in gendir @ONLY)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lib
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

foreach(test_name IN LISTS c_tests)
  add_executable(ndi.${test_name} ${CMAKE_CURRENT_LIST_DIR}/${test_name}.c)
  target_compile_definitions(ndi.${test_name} PUBLIC NDI_STANDALONE)
  target_link_libraries(ndi.${test_name} PUBLIC ndi)
  set_target_properties(ndi.${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}>)
  get_property(location TARGET ndi.${test_name} PROPERTY OUTPUT_NAME)
  add_test(NAME ndi.${test_name} COMMAND ${CMAKE_COMMAND}
    -D exe=${CMAKE_CURRENT_BINARY_DIR}/ndi.${test_name}
    -D output=${CMAKE_CURRENT_BINARY_DIR}/ndi.${test_name}.output
    -D reference=${CMAKE_CURRENT_SOURCE_DIR}/output/testc_${test_name}.out
    -D script=${CMAKE_CURRENT_LIST_DIR}/fcmp.pl
    -P ${CMAKE_CURRENT_LIST_DIR}/runtest.cmake)
endforeach()

foreach(test_name IN LISTS f_tests)
  add_executable(ndi.${test_name} ${CMAKE_CURRENT_LIST_DIR}/${test_name}.F)
  target_link_libraries(ndi.${test_name} PUBLIC ndi)
  get_property(location TARGET ndi.${test_name} PROPERTY OUTPUT_NAME)
  set_target_properties(ndi.${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}>)
  add_test(NAME ndi.${test_name} COMMAND ${CMAKE_COMMAND}
    -D exe=${CMAKE_CURRENT_BINARY_DIR}/ndi.${test_name}
    -D output=${CMAKE_CURRENT_BINARY_DIR}/ndi.${test_name}.output
    -D reference=${CMAKE_CURRENT_SOURCE_DIR}/output/testf_${test_name}.out
    -D script=${CMAKE_CURRENT_LIST_DIR}/fcmp.pl
    -P ${CMAKE_CURRENT_LIST_DIR}/runtest.cmake)
endforeach()

