#include "ndi_keywords.h"


      program main

      integer ierr
      integer str_size
      character*500 path

      call ndi_init(ierr)
      if ( ierr .ne. 0 ) then
         stop
      endif

      ! Note that only ndi_init needs to be called before asking for NDI_GENDIR_PATH
      call ndi_get_size( NDI_GENDIR_PATH, str_size, ierr )

      call ndi_get_string_val( NDI_GENDIR_PATH, path, ierr )
      write(*,300) path, str_size, ierr
 300  format(" GENDIR PATH: ", a100, " (size=",i6, "; err=", i6, ")")
   
      call ndi_close(ierr)
      end
