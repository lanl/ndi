#include "ndi_keywords.h"


      program main

      integer ierr
      integer vec_size
      integer e_avg_size
      real*8 ret_vec64(500)


      call ndi_init(ierr)
      call ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron", ierr)
      call ndi_set_option( NDI_LIBRARY, "mendf6", ierr)
      call ndi_set_option( NDI_COLLAPSE, "16_llnl", ierr)
      call ndi_set_isotope("u235", ierr)

      vec_size = 500
      call ndi_get_size(NDI_E_AVG, e_avg_size, ierr)
      print *, "Average energy size is: ", e_avg_size

      ! Use default average energy 
      call ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size, ierr)
      if (ierr.ne.0) then
         print *, "  Error", ierr,
     $        "getting average energy (default) for isotope u235."
      endif
      print *, "Default average energy:"
      call print_float64_vec(ret_vec64, e_avg_size)
      
      
      ! Use calculation of average energy from average velocity (same as default) 
      call ndi_set_option(NDI_E_AVG, "from_vel", ierr)
      call ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size, ierr)
      if (ierr.ne.0) then
         print *, "  Error ", ierr,
     $        " getting average energy (from_vel) for isotope u235." 
      endif
      print *, "Average energy from velocity calc:"
      call print_float64_vec(ret_vec64, e_avg_size)
   
    
      ! Use calculation of average energy from mean of bin boundary energies 
      ! This is the old default use this to match values obtained from NDI before release 1.3.0 
      call ndi_set_option(NDI_E_AVG, "from_e", ierr)
      call ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size, ierr)
      if (ierr.ne.0) then
         print *, "  Error", ierr,
     $        "getting average energy (from_e) for isotope u235." 
      endif
      print *, "Average energy from mean of energy bounds:"
      call print_float64_vec(ret_vec64, e_avg_size)

    
      ! Use legacy calculation of average energy 
      call ndi_set_option(NDI_E_AVG, "legacy_calc", ierr)
      call ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size, ierr)
      if (ierr.ne.0) then
         print *, "  Error ", ierr,
     $        " getting average energy (legacy_calc) for isotope u235."
      endif
      print *, "Average energy from legacy calc:"
      call print_float64_vec(ret_vec64, e_avg_size)

    
      ! Use SGNDF values of average energy 
      call ndi_set_option(NDI_E_AVG, "from_sgndf", ierr)
      call ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size, ierr)
      if (ierr.ne.0) then
         print *, "  Error ", ierr,
     $        " getting average energy (from_sgndf) for isotope u235."
      endif
      print *, "Average energy from SGNDF:"
      call print_float64_vec(ret_vec64, e_avg_size)

      call ndi_close(ierr)
      end

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      subroutine print_float64_vec(flt64_vec, num_elements)
      
      integer num_elements
      real*8 flt64_vec(*)

      print 10, (flt64_vec(j), j=1,num_elements)
 10   format(4(1pe15.6, 2x))
      
      return
      end

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
