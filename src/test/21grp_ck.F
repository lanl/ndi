#include "ndi_keywords.h"

      program main

      integer ierr
      integer num_grps
      real*8 e(100)

      call ndi_init(ierr)
      call ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron", ierr)
      call ndi_set_option( NDI_LIBRARY, "mendf6", ierr)
      call ndi_set_option( NDI_COLLAPSE, "21_lanl", ierr)
      call ndi_set_option( NDI_REAC_ID, "c", ierr)
      call ndi_set_option( NDI_E_AVG, "from_sgndf", ierr)

!     Isotope 1002      
      call ndi_set_isotope("1001", ierr)
      call ndi_get_int_val( NDI_NUM_GRPS, num_grps, ierr)
      call ndi_get_float64_vec( NDI_ENERGY_AVAIL, e, 100, ierr )
      print *, "1001"
      if ( ierr .ne. 0 ) then
         print *, "****Error getting available energy is ", ierr
      endif
      print *, "Energy available:"
      call print_float64_vec(e, num_grps)
      call ndi_get_float64_vec( NDI_ENERGY_N, e, 100, ierr )
      if ( ierr .ne. 0 ) then
         print *, "****Error getting energy to neutrons is ", ierr
      endif
      print *, "Energy to neutrons:"
      call print_float64_vec(e, num_grps)
      print *, " "

!     Isotope 1002      
      call ndi_set_isotope("1002", ierr)
      print *, "1002"
      call ndi_get_float64_vec( NDI_ENERGY_AVAIL, e, 100, ierr )
      if ( ierr .ne. 0 ) then
         print *, "****Error getting available energy is ", ierr
      endif
      print *, "Energy available:"
      call print_float64_vec(e, num_grps)
      print *, " "
      call ndi_get_float64_vec( NDI_ENERGY_N, e, 100, ierr )
      if ( ierr .ne. 0 ) then
         print *, "****Error getting energy to neutrons is ", ierr
      endif
      print *, "Energy to neutrons:"
      call print_float64_vec(e, num_grps)
      print *, " "

      call ndi_close()
 
      end

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      subroutine print_float64_vec(flt64_vec, num_elements)
      
      integer num_elements
      real*8 flt64_vec(*)

      print 20, (flt64_vec(j), j=1,num_elements)
 20   format(4(1pe15.6, 2x))
      
      return
      end

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
