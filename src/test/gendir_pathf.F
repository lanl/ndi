#include "ndi_keywords.h"


      program main

      integer ierr
      integer str_size
      character*500 path

      call ndi_init(ierr)
      if ( ierr .ne. 0 ) then
         stop
      endif

      call ndi_get_size( NDI_GENDIR_PATH, str_size, ierr )
      write(*,300) str_size, ierr
 300  format("String array size is ", i6, " (err=", i6, ")" )

      call ndi_get_string_val( NDI_GENDIR_PATH, path, ierr )
      write(*,301) path, ierr
 301  format(" PATH: ", a100, " (err=", i6, ")")
   
      call ndi_close(ierr)
      end
