#include "ndi_keywords.h"


      program main

      integer ierr
      integer arr_size
      integer arr_num
      character*13 str_arr(500)
      real*8 awr(500)

      integer gendir

      call ndi2_open_gendir(gendir, "", ierr)
      if ( ierr .ne. 0 ) then
         stop
      endif

      call ndi2_set_option_gendir(gendir, NDI_LIB_TYPE_DEFAULT, 
     $     "multigroup_neutron", ierr)
      if ( ierr .ne. 0 ) then
         stop
      endif

      call ndi2_get_size(gendir, NDI_AWR, arr_num, ierr)
      write(*, 290) arr_num
 290  format("String array num is ", i6)

      call ndi2_get_size(gendir, NDI_ZAID, arr_size, ierr )
      write(*,300) arr_size
 300  format("String array size is ", i6 )
      

      call ndi2_get_string_vec(gendir, NDI_ZAID, 
     $     str_arr, 500, 13, ierr )
      write(*,301) " String vec error is ", ierr
 301  format(a21, i6)

      call ndi2_get_float64_vec( gendir, NDI_AWR, awr, 500, ierr )
      write(*, 301) " float vec error is ", ierr

      do i = 1, arr_num
         write(*, 303) i,  str_arr(i), awr(i)
 303     format(i6, ":  >", a13, "<  ", f12.5)
      enddo

      call ndi2_close_gendir(gendir, ierr)
      end
