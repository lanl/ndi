#include "ndi_keywords.h"


      program main

      integer ierr
      integer arr_size
      integer arr_num
      character*13 str_arr(500)
      character*12 str_arr2(500)
      real*8 awr(500)

      call ndi_init(ierr)
      if ( ierr .ne. 0 ) then
         stop
      endif

      call ndid_get_size( NDI_ZAID, "multigroup_neutron",
     $     arr_size, ierr )
      print *, "String array size is ", arr_size
      
c      call ndid_get_string_vec( NDI_ZAID, "multigroup_neutron",
c     $   str_arr, 500, 13, ierr )
c      print *, " String vec error is ", ierr
c      do i = 1, arr_size
c         call ndi_set_isotope( str_arr(i), ierr )
c         print *, i,":  >", str_arr(i), "< err = ", ierr 
c      enddo
c      print *, " "
     
      call ndid_get_string_vec( NDI_ZAID, "multigroup_neutron",
     $     str_arr2, 500, 12, ierr )
      print *, " String vec error is ", ierr
      do i = 1, arr_size
         call ndi_set_isotope( str_arr2(i), ierr )
         print *, i,":  >", str_arr2(i), "< err = ", ierr 
      enddo

      call ndid_zaid_float64_vec( NDI_AWR, "multigroup_neutron",
     $     str_arr, 500, 13, awr, 500, ierr )
      print *, " ZAID float error is ", ierr
      do i = 1, arr_size
         call ndi_set_isotope( str_arr(i), ierr )
         print *, i,":  >", str_arr(i), "<  ", awr(i), "  err = ", ierr
      enddo
      
      call ndi_close(ierr)
      end
