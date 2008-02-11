#include "ndi_keywords.h"

      program main

      integer ZLEN, ALEN
      parameter(ZLEN=24, ALEN=10)

      integer gendir0
      integer ierr
      integer size
      character*(ZLEN) zaid(ALEN)
      real*8 awr(ALEN)

      print *, "Open gendir:"
      call ndi2_open_gendir(gendir0, "", ierr)
      if ( ierr .ne. 0 ) then
         write (*, 100) ierr
 100     format("  **Error ", i6, " in NDI open gendir.")
      else
         print *, "  ok"
      endif

      print *, "Set library type:"
      call ndi2_set_option_gendir(gendir0, NDI_LIB_TYPE_DEFAULT,
     $     "multigroup_neutron", ierr)
      if ( ierr .ne. 0 ) then
         write (*, 200) ierr
 200     format("  **Error ", i6, " in NDI set option.")
      else
         print *, "  ok"
      endif

      print *, "Set library:"
      call ndi2_set_option_gendir(gendir0, NDI_LIBRARY_DEFAULT,
     $     "mendf5", ierr)
      if ( ierr .ne. 0 ) then
         write (*, 300) ierr
 300     format("  **Error ", i6, " in NDI set option.")
      else
         print *, "  ok"   
      endif

      print *, "Get array size:"
      call ndi2_get_size_x(gendir0, NDI_TEMP, "92", size, ierr)
      if ( ierr .ne. 0 ) then
         write (*, 400) ierr
 400     format("  **Error ", i6, " in NDI get size x.")
      else
         write (*, 500) size
 500     format("  Size =", i6)
      endif

      print *, "Get zaid list:"
      call ndi2_get_string_vec_x(gendir0, NDI_ZAID, "92",
     $     zaid, ALEN, ZLEN, ierr)
      if ( ierr .ne. 0 ) then
         write (*, 600) ierr
 600     format("  **Error ", i6, " in NDI get string vec x.")
      else
         do i = 1, size
            write (*, 650) zaid(i)
 650        format("  ", a24)
         enddo
      endif

      print *, "Get atomic weight ratio list:"
      call ndi2_get_float64_vec_x(gendir0, NDI_AWR, "92",
     $     awr, ALEN, ierr)
      if ( ierr .ne. 0 ) then
         write (*, 700) ierr
 700     format("  **Error ", i6, " in NDI get float64 vec x.")
      else
         do i = 1, size
            write (*, 750) awr(i)
 750        format("  ", f10.5)
         enddo
      endif

      print *, "Close gendir:"
      call ndi2_close_gendir(gendir0, ierr)
      if ( ierr .ne. 0 ) then
         write (*, 800) ierr
 800     format("  **Error ", i6, " in NDI close gendir.")
      else
         print *, "  ok"
      endif

      end
