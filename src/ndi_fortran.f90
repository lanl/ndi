     module ndi
       use iso_c_binding

     interface

     subroutine ndid_get_string_vec( in_key, in_value, str_arr,        &
                                     arr_size, str_size, ierr, flen1,  &
                                     flen2 )                           &
          bind(C, name='F_NDID_GET_STRING_VEC')
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, arr_size, str_size, ierr
       character(c_char), dimension(*) :: in_value, str_arr
       integer(c_int), value :: flen1, flen2

     end subroutine ndid_get_string_vec

     subroutine ndid_get_size( in_key, in_value, arr_size,             &
                               ierr, flen )                            &
          bind(C, name='F_NDID_GET_SIZE');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, arr_size, ierr
       character(c_char), dimension(*) :: in_value
       integer(c_int), value :: flen

     end subroutine ndid_get_size

     subroutine ndid_zaid_float64_vec( in_key, in_value, str_arr,      &
                                      arr_size, zaid_size, float_vec,  &
                                      fl_arr_size, ierr, flen1,        &
                                      flen2 )                          &
          bind(C, name='F_NDID_ZAID_FLOAT64_VEC');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, arr_size, zaid_size,   &
                                        fl_arr_size, ierr
       real(c_double), dimension(*) :: float_vec
       character(c_char), dimension(*) :: in_value, str_arr
       integer(c_int), value :: flen1, flen2

     end subroutine ndid_zaid_float64_vec

     subroutine ndid_zaid_float32_vec( in_key, in_value, str_arr,      &
                                       arr_size, zaid_size, float_vec, &
                                       fl_arr_size, ierr, flen1,       &
                                       flen2 )                         &
          bind(C, name='F_NDID_ZAID_FLOAT32_VEC');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, arr_size, zaid_size,   &
                                        fl_arr_size, ierr
       real(c_float), dimension(*) :: float_vec
       character(c_char), dimension(*) :: in_value, str_arr
       integer(c_int), value :: flen1, flen2

     end subroutine ndid_zaid_float32_vec

     subroutine ndi_init( ierr )                                       &
          bind(C, name='F_NDI_INIT');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: ierr

     end subroutine ndi_init

     subroutine ndi_close( ierr )                                      &
          bind(C, name='F_NDI_CLOSE');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: ierr

     end subroutine ndi_close

     subroutine ndi_set_option( in_key, in_value, ierr, len )          &
          bind(C, name='F_NDI_SET_OPTION');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, ierr
       character(c_char), dimension(*) :: in_value
       integer(c_int), value :: len

     end subroutine ndi_set_option

     subroutine ndi_check_option( in_key, out_value, ierr, len )       &
          bind(C, name='F_NDI_CHECK_OPTION');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, ierr
       character(c_char), dimension(*) :: out_value
       integer(c_int), value :: len

     end subroutine ndi_check_option

     subroutine ndi_set_isotope( zaid, ierr, zaid_len )                &
          bind(C, name='F_NDI_SET_ISOTOPE');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: ierr
       character(c_char), dimension(*) :: zaid
       integer(c_int), value :: zaid_len

     end subroutine ndi_set_isotope

     subroutine ndi_set_zaid( zaid, ierr, zaid_len )                   &
          bind(C, name='F_NDI_SET_ZAID');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: ierr
       character(c_char), dimension(*) :: zaid
       integer(c_int), value :: zaid_len

     end subroutine ndi_set_zaid

     subroutine ndi_set_zaid_float32_temp( zaid, temp, ierr,           &
                                           zaid_len )                  &
          bind(C, name='F_NDI_SET_ZAID_FLOAT32_TEMP');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: ierr
       real(c_float), intent(inout) :: temp
       character(c_char), dimension(*) :: zaid
       integer(c_int), value :: zaid_len

     end subroutine ndi_set_zaid_float32_temp

     subroutine ndi_set_zaid_float64_temp( zaid, temp, ierr,           &
                                           zaid_len )                  &
          bind(C, name='F_NDI_SET_ZAID_FLOAT64_TEMP');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: ierr
       real(c_double), intent(inout) :: temp
       character(c_char), dimension(*) :: zaid
       integer(c_int), value :: zaid_len

     end subroutine ndi_set_zaid_float64_temp

     subroutine ndi_set_reaction( zaid, ierr, zaid_len )               &
          bind(C, name='F_NDI_SET_REACTION');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: ierr
       character(c_char), dimension(*) :: zaid
       integer(c_int), value :: zaid_len

     end subroutine ndi_set_reaction

     subroutine ndi_get_size( in_key, return_val, ierr )             &
          bind(C, name='F_NDI_GET_SIZE');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, return_val, ierr

     end subroutine ndi_get_size

     subroutine ndi_get_size_n( in_key, vec_num, return_val, ierr )    &
          bind(C, name='F_NDI_GET_SIZE_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_num, return_val,   &
                                        ierr

     end subroutine ndi_get_size_n

     subroutine ndi_get_size_x( in_key, in_value, return_val, ierr,    &
                                len )                                  &
          bind(C, name='F_NDI_GET_SIZE_X');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, return_val, ierr
       character(c_char), dimension(*) :: in_value
       integer(c_int), value :: len

     end subroutine ndi_get_size_x

     subroutine ndi_get_int_val( in_key, return_val, ierr )            &
          bind(C, name='F_NDI_GET_INT_VAL');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, return_val, ierr

     end subroutine ndi_get_int_val

     subroutine ndi_get_int_val_n( in_key, vec_num, return_val, ierr ) &
          bind(C, name='F_NDI_GET_INT_VAL_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_num, return_val,   &
                                        ierr

     end subroutine ndi_get_int_val_n

     subroutine ndi_get_int_vec( in_key, return_val, vec_size, ierr )  &
          bind(C, name='F_NDI_GET_INT_VEC');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_size, ierr
       integer(c_int), dimension(*) :: return_val

     end subroutine ndi_get_int_vec

     subroutine ndi_get_int_vec_n( in_key, vec_num, return_val,        &
                                   vec_size, ierr )                    &
          bind(C, name='F_NDI_GET_INT_VEC_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_num, vec_size, ierr
       integer(c_int), dimension(*) :: return_val

     end subroutine ndi_get_int_vec_n

     subroutine ndi_get_int_vec_x( in_key, in_str, return_val,         &
                                   vec_size, ierr, in_str_len )        &
          bind(C, name='F_NDI_GET_INT_VEC_X');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_size, ierr
       integer(c_int), dimension(*) :: return_val
       character(c_char), dimension(*) :: in_str
       integer(c_int), value :: in_str_len

     end subroutine ndi_get_int_vec_x

     subroutine ndi_get_float32_val( in_key, return_val, ierr )        &
          bind(C, name='F_NDI_GET_FLOAT32_VAL');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, ierr
       real(c_float), intent(inout) :: return_val

     end subroutine ndi_get_float32_val

     subroutine ndi_get_float64_val( in_key, return_val, ierr )        &
          bind(C, name='F_NDI_GET_FLOAT32_VAL');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, ierr
       real(c_double), intent(inout) :: return_val

     end subroutine ndi_get_float64_val

     subroutine ndi_get_float32_val_n( in_key, vec_num, return_val,      &
                                     ierr )                            &
          bind(C, name='F_NDI_GET_FLOAT32_VAL_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_num, ierr
       real(c_float), intent(inout) :: return_val

     end subroutine ndi_get_float32_val_n

     subroutine ndi_get_float64_val_n( in_key, vec_num, return_val,    &
                                       ierr )                          &
          bind(C, name='F_NDI_GET_FLOAT64_VAL_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_num, ierr
       real(c_double), intent(inout) :: return_val

     end subroutine ndi_get_float64_val_n

     subroutine ndi_get_float32_vec( in_key, return_vec, vec_size,     &
                                     ierr )                            &
          bind(C, name='F_NDI_GET_FLOAT32_VEC');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_size, ierr
       real(c_float), dimension(*) :: return_vec

     end subroutine ndi_get_float32_vec

     subroutine ndi_get_float64_vec( in_key, return_vec, vec_size,     &
                                     ierr )                            &
          bind(C, name='F_NDI_GET_FLOAT32_VEC');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_size, ierr
       real(c_double), dimension(*) :: return_vec

     end subroutine ndi_get_float64_vec

     subroutine ndi_get_float32_vec_n( in_key, vec_num, return_vec,    &
                                       vec_size, ierr )                &
          bind(C, name='F_NDI_GET_FLOAT32_VEC_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_num, vec_size, ierr
       real(c_float), dimension(*) :: return_vec

     end subroutine ndi_get_float32_vec_n

     subroutine ndi_get_float64_vec_n( in_key, vec_num, return_vec,    &
                                       vec_size, ierr )                &
          bind(C, name='F_NDI_GET_FLOAT32_VEC_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_num, vec_size, ierr
       real(c_double), dimension(*) :: return_vec

     end subroutine ndi_get_float64_vec_n

     subroutine ndi_get_float32_vec_x( in_key, in_str, return_vec,     &
                                       vec_size, ierr, in_str_len )    &
          bind(C, name='F_NDI_GET_FLOAT32_VEC_X');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_size, ierr
       real(c_float), dimension(*) :: return_vec
       character(c_char), dimension(*) :: in_str
       integer(c_int), value :: in_str_len

     end subroutine ndi_get_float32_vec_x

     subroutine ndi_get_float64_vec_x( in_key, in_str, return_vec,     &
                                       vec_size, ierr, in_str_len )    &
          bind(C, name='F_NDI_GET_FLOAT32_VEC_X');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_size, ierr
       real(c_double), dimension(*) :: return_vec
       character(c_char), dimension(*) :: in_str
       integer(c_int), value :: in_str_len

     end subroutine ndi_get_float64_vec_x

     subroutine ndi_get_string_val( in_key, return_value, ierr,        &
                                    char_len )                         &
          bind(C, name='F_NDI_GET_STRING_VAL');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, ierr
       character(c_char), dimension(*) :: return_value
       integer(c_int), value :: char_len

     end subroutine ndi_get_string_val

     subroutine ndi_get_string_val_n( in_key, vec_num, return_val,     &
                                      ierr, char_len )                 &
          bind(C, name='F_NDI_GET_STRING_VAL_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, vec_num, ierr
       character(c_char), dimension(*) :: return_val
       integer(c_int), value :: char_len

     end subroutine ndi_get_string_val_n

     subroutine ndi_get_string_vec( in_key, return_val, arr_size,      &
                                    str_size, ierr, char_len )         &
          bind(C, name='F_NDI_GET_STRING_VEC');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, arr_size, str_size, ierr
       character(c_char), dimension(*) :: return_val
       integer(c_int), value :: char_len

     end subroutine ndi_get_string_vec

     subroutine ndi_get_string_vec_x( in_key, in_str, return_val,      &
                                      arr_size, str_size, ierr,        &
                                      str_len, val_len )               &
          bind(C, name='F_NDI_GET_STRING_VEC_X');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: in_key, arr_size, str_size, ierr
       character(c_char), dimension(*) :: in_str, return_val
       integer(c_int), value :: str_len, val_len

     end subroutine ndi_get_string_vec_x

              !             ndi2.0 user functions with handles

     subroutine ndi2_open_gendir( handle, path, ierr, path_len )       &
          bind(C, name='F_NDI2_OPEN_GENDIR');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, ierr
       character(c_char), dimension(*) :: path
       integer(c_int), value :: path_len

     end subroutine ndi2_open_gendir

     subroutine ndi2_set_option_gendir( handle, in_key, value, ierr,   &
                                        str_len )                      &
          bind(C, name='F_NDI2_SET_OPTION_GENDIR');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, ierr
       character(c_char), dimension(*) :: value
       integer(c_int), value :: str_len

     end subroutine ndi2_set_option_gendir

     subroutine ndi2_check_option_gendir( handle, in_key, out_value,   &
                                        ierr, string_size  )           &
          bind(C, name='F_NDI2_CHECK_OPTION_GENDIR');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, ierr
       character(c_char), dimension(*) :: out_value
       integer(c_int), value :: string_size

     end subroutine ndi2_check_option_gendir

     subroutine ndi2_close_gendir( handle, ierr )                      &
          bind(C, name='F_NDI2_CLOSE_GENDIR');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, ierr

     end subroutine ndi2_close_gendir

     subroutine ndi2_make_handle( handle1, handle2, ierr, len  )       &
          bind(C, name='F_NDI2_MAKE_HANDLE');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle1, handle2, ierr
       integer(c_int), value :: len

     end subroutine ndi2_make_handle

     subroutine ndi2_set_zaid( handle, zaid, ierr, len )               &
          bind(C, name='F_NDI2_SET_ZAID');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, ierr
       character(c_char), dimension(*) :: zaid
       integer(c_int), value :: len

     end subroutine ndi2_set_zaid

     subroutine ndi2_set_zaid_float32_temp( handle, zaid, temp, ierr,  &
                                            zaid_len  )                &
          bind(C, name='F_NDI2_SET_ZAID_FLOAT32_TEMP');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, ierr
       character(c_char), dimension(*) :: zaid
       real(c_float), intent(inout) :: temp
       integer(c_int), value :: zaid_len

     end subroutine ndi2_set_zaid_float32_temp

     subroutine ndi2_set_zaid_float64_temp( handle, zaid, temp, ierr,  &
                                            zaid_len  )                &
          bind(C, name='F_NDI2_SET_ZAID_FLOAT32_TEMP');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, ierr
       character(c_char), dimension(*) :: zaid
       real(c_double), intent(inout) :: temp
       integer(c_int), value :: zaid_len

     end subroutine ndi2_set_zaid_float64_temp

     subroutine ndi2_set_isotope( handle, zaid, ierr, len )            &
          bind(C, name='F_NDI2_SET_ISOTOPE');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, ierr
       character(c_char), dimension(*) :: zaid
       integer(c_int), value :: len

     end subroutine ndi2_set_isotope

     subroutine ndi2_set_reaction( handle, zaid, ierr, len )           &
          bind(C, name='F_NDI2_SET_REACTION');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, ierr
       character(c_char), dimension(*) :: zaid
       integer(c_int), value :: len

     end subroutine ndi2_set_reaction

     subroutine ndi2_get_handle( handle0, handle1, ierr  )             &
          bind(C, name='F_NDI2_GET_HANDLE');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle0, handle1, ierr

     end subroutine ndi2_get_handle

     subroutine ndi2_get_size( handle, in_key, return_val, ierr)       &
          bind(C, name='F_NDI2_GET_SIZE');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, return_val, ierr

     end subroutine ndi2_get_size

     subroutine ndi2_get_size_n( handle, in_key, vec_num, return_val,  &
                                 ierr )                                &
          bind(C, name='F_NDI2_GET_SIZE_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, vec_num,       &
                                        return_val, ierr

     end subroutine ndi2_get_size_n

     subroutine ndi2_get_size_x( handle, in_key, in_str, return_val,   &
                                 ierr, len  )                          &
          bind(C, name='F_NDI2_GET_SIZE_X');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, return_val, ierr 
       character(c_char), dimension(*) :: in_str
       integer(c_int), value :: len

     end subroutine ndi2_get_size_x

     subroutine ndi2_set_option( handle, in_key, value, ierr, len )    &
          bind(C, name='F_NDI2_SET_OPTION');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, ierr
       character(c_char), dimension(*) :: value
       integer(c_int), value :: len

     end subroutine ndi2_set_option

     subroutine ndi2_set_float64_vec_option( handle, in_key,           &
                                             option_array, array_size, &
                                             ierr  )                   &
          bind(C, name='F_NDI2_SET_FLOAT64_VEC_OPTION');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, array_size, ierr
       real(c_double), dimension(*) :: option_array

     end subroutine ndi2_set_float64_vec_option

     subroutine ndi2_check_option( handle, in_key, out_value, ierr,    &
                                   len  )                              &
          bind(C, name='F_NDI2_CHECK_OPTION');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, ierr
       character(c_char), dimension(*) :: out_value
       integer(c_int), value :: len

     end subroutine ndi2_check_option

     subroutine ndi2_get_int_val( handle, in_key, return_val, ierr )   &
          bind(C, name='F_NDI2_GET_INT_VAL');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, return_val, ierr

     end subroutine ndi2_get_int_val

     subroutine ndi2_get_int_val_n( handle, in_key, vec_num,           &
                                    return_val, ierr )                 &
          bind(C, name='F_NDI2_GET_INT_VAL_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, vec_num,       &
                                        return_val, ierr

     end subroutine ndi2_get_int_val_n

     subroutine ndi2_get_int_vec( handle, in_key, return_vec,          &
                                  vec_size, ierr  )                    &
          bind(C, name='F_NDI2_GET_INT_VEC');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, vec_size, ierr
       integer(c_int), dimension(*) :: return_vec

     end subroutine ndi2_get_int_vec

     subroutine ndi2_get_int_vec_n( handle, in_key, vec_num,           &
                                    return_vec, vec_size, ierr )       &
          bind(C, name='F_NDI2_GET_INT_VEC_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, vec_num,       &
                                        vec_size, ierr
       integer(c_int), dimension(*) :: return_vec

     end subroutine ndi2_get_int_vec_n

     subroutine ndi2_get_int_vec_x( handle, in_key, in_str,            &
                                    return_vec, vec_size, ierr, len )  &
          bind(C, name='F_NDI2_GET_INT_VEC_X');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, vec_size, ierr
       integer(c_int), dimension(*) :: return_vec
       character(c_char), dimension(*) :: in_str
       integer(c_int), value :: len

     end subroutine ndi2_get_int_vec_x

     subroutine ndi2_get_float32_val( handle, in_key, return_val,      &
                                      ierr )                           &
          bind(C, name='F_NDI2_GET_FLOAT32_VAL');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, ierr
       real(c_float), intent(inout) :: return_val

     end subroutine ndi2_get_float32_val

     subroutine ndi2_get_float32_val_n( handle, in_key, vec_num,       &
                                        return_val, ierr  )            &
          bind(C, name='F_NDI2_GET_FLOAT32_VAL_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, ierr, vec_num
       real(c_float), intent(inout) :: return_val

     end subroutine ndi2_get_float32_val_n

     subroutine ndi2_get_float32_vec( handle, in_key, return_vec,      &
                                         vec_size, ierr  )             &
          bind(C, name='F_NDI2_GET_FLOAT32_VEC');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, vec_size, ierr
       real(c_float), dimension(*) :: return_vec

     end subroutine ndi2_get_float32_vec

     subroutine ndi2_get_float32_vec_n( handle, in_key, vec_num,       &
                                        return_vec, vec_size, ierr )   &
          bind(C, name='F_NDI2_GET_FLOAT32_VEC_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, vec_num,       &
                                        vec_size, ierr
       real(c_float), dimension(*) :: return_vec

     end subroutine ndi2_get_float32_vec_n

     subroutine ndi2_get_float32_vec_x( handle, in_key, in_str,        &
                                        return_vec, vec_size, ierr,    &
                                        len )                          &
          bind(C, name='F_NDI2_GET_FLOAT32_VEC_X');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, vec_size, ierr
       real(c_float), dimension(*) :: return_vec
       character(c_char), dimension(*) :: in_str
       integer(c_int), value :: len

     end subroutine ndi2_get_float32_vec_x

     subroutine ndi2_get_float64_val( handle, in_key, return_val,      &
                                      ierr )                           &
          bind(C, name='F_NDI2_GET_FLOAT64_VAL');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, ierr
       real(c_double), intent(inout) :: return_val

     end subroutine ndi2_get_float64_val

     subroutine ndi2_get_float64_val_n( handle, in_key, vec_num,       &
                                        return_val, ierr )             &
          bind(C, name='F_NDI2_GET_FLOAT64_VAL_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, vec_num, ierr
       real(c_double), intent(inout) :: return_val

     end subroutine ndi2_get_float64_val_n

     subroutine ndi2_get_float64_vec( handle, in_key, return_vec,      &
                                      vec_size, ierr )                 &
          bind(C, name='F_NDI2_GET_FLOAT64_VEC');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, vec_size, ierr
       real(c_double), dimension(*) :: return_vec

     end subroutine ndi2_get_float64_vec

     subroutine ndi2_get_float64_vec_n( handle, in_key, vec_num,       &
                                        return_vec, vec_size, ierr )   &
          bind(C, name='F_NDI2_GET_FLOAT64_VEC_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, vec_num,       &
                                        vec_size, ierr
       real(c_double), dimension(*) :: return_vec

     end subroutine ndi2_get_float64_vec_n

     subroutine ndi2_get_float64_vec_x( handle, in_key, in_str,        &
                                        return_vec, vec_size, ierr,    &
                                        len )                          &
          bind(C, name='F_NDI2_GET_FLOAT64_VEC_X');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, vec_size, ierr
       real(c_double), dimension(*) :: return_vec
       character(c_char), dimension(*) :: in_str
       integer(c_int), value :: len

     end subroutine ndi2_get_float64_vec_x

     subroutine ndi2_get_string_val( handle, in_key, return_val, ierr, &
                                     len )                             &
          bind(C, name='F_NDI2_GET_STRING_VAL');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, ierr
       character(c_char), dimension(*) :: return_val
       integer(c_int), value :: len

     end subroutine ndi2_get_string_val

     subroutine ndi2_get_string_val_n( handle, in_key, vec_num,        &
                                       return_val, ierr, len )         &
          bind(C, name='F_NDI2_GET_STRING_VAL_N');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, vec_num, ierr
       character(c_char), dimension(*) :: return_val
       integer(c_int), value :: len

     end subroutine ndi2_get_string_val_n

     subroutine ndi2_get_string_vec( handle, in_key, str_arr,          &
                                     arr_size, str_size, ierr, flen2 ) &
          bind(C, name='F_NDI2_GET_STRING_VEC');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, arr_size,      &
                                        str_size, ierr
       character(c_char), dimension(*) :: str_arr
       integer(c_int), value :: flen2

     end subroutine ndi2_get_string_vec

     subroutine ndi2_get_string_vec_x( handle, in_key, in_str,         &
                                       return_val, arr_size, str_size, &
                                       ierr, str_len, val_len )        &
          bind(C, name='F_NDI2_GET_STRING_VEC_X');
       use iso_c_binding
       implicit none

       integer(c_int), intent(inout) :: handle, in_key, arr_size,      &
                                        str_size, ierr
       character(c_char), dimension(*) :: in_str, return_val
       integer(c_int), value :: str_len, val_len

     end subroutine ndi2_get_string_vec_x

     end interface
     end module ndi
