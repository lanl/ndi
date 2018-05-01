module ndi
  use iso_c_binding
  interface
     subroutine ndi_get_size( in_key, in_value, arr_size, ierr, flen ) &
          bind(C, name='F_NDID_GET_SIZE')
       use iso_c_binding
       implicit none
       integer(c_int), intent(inout) :: in_key, arr_size, ierr
       character(c_char), dimension(*) :: in_value
       integer(c_int), value :: flen
     end subroutine ndi_get_size

     subroutine ndi_get_string_vec( in_key, in_value, str_arr, arr_size, &
                                   str_size, ierr, flen1, flen2 )        &
          bind(C, name='F_NDID_GET_STRING_VEC')
       use iso_c_binding
       implicit none
       integer(c_int), intent(inout) :: in_key, arr_size, str_size, ierr
       character(c_char), dimension(*) :: in_value, str_arr
       integer(c_int), value :: flen1, flen2
     end subroutine ndi_get_string_vec


  end interface
end module ndi
