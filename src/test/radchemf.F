#include "ndi_keywords.h"


      program main

      
      character*13 zaid, tmp_zaid, num_grp_str
      integer return_int, vec_num, return_int_vec(20)
      integer num_target, i, tmp_izaid
      real*8 return_flt, return_flt_vec(900), awr(500)


c     Test error on initialization before close
      call ndi_init(ierr) 
      if ( ierr.ne.0 ) then
         write(*, 300) "Error initializing NDI is ", ierr
 300     format(a35, i6)
      endif

      num_grp_str = '2_group'
      call ndi_set_option( NDI_LIB_TYPE, 'dosimetry_neutron', ierr )
      if ( ierr.ne.0 ) then
         write(*, 300) "Error setting library type is ", ierr
      endif
      call ndi_set_option( NDI_CHAIN, "y", ierr)
      if ( ierr.ne.0 ) then
         write(*, 300) "Error setting library chain is ", ierr
      endif
      call ndi_set_option( NDI_COLLAPSE, "21_group", ierr )
      if ( ierr.ne.0 ) then
         write(*, 300) "Error setting energy groups is ", ierr
      endif


      call ndi_get_int_val( NDI_NUM_TARGET, num_target, ierr )
      if ( ierr.ne.0 ) then
         write(*, 300) "Error getting number of targets is ", ierr
      endif
      
      do i = 1, num_target
         call ndi_get_string_val_n( NDI_TARGET, i - 1, tmp_zaid, ierr)
      if ( ierr.ne.0 ) then
         write(*, 300) "Error getting targets id is ", ierr
      endif
      call ndi_get_int_val_n( NDI_TARGET, i-1, tmp_izaid, ierr)
      if ( ierr.ne.0 ) then
         write(*, 300) "Error getting target (int) is ", ierr
      endif
      
      call ndi_set_isotope( tmp_zaid, ierr )
      if ( ierr.ne.0 ) then
         write(*, 300) "Error setting isotope is ", ierr
      endif
        
      write(*, 301) tmp_zaid, tmp_izaid
 301  format("TARGET = ", a13, "   TARGET(int) = ", i13)
      end do

      call ndi_close(ierr)

      end





