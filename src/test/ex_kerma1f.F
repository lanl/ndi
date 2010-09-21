#include "ndi_keywords.h"

      program main

      integer ierr
      integer kerma_size
      integer heating_size
      integer vec_size
      real*8 kerma(500)
      real*8 heating(500)

      call ndi_init(ierr)
      call ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron", ierr)
      call ndi_set_option( NDI_LIBRARY, "mendf6", ierr)
      call ndi_set_option( NDI_COLLAPSE, "16_lanl2", ierr)
      call ndi_set_isotope("u235", ierr)

      vec_size = 500
      call ndi_get_size(NDI_KERMA, kerma_size, ierr)
      print *, "Kerma size is: ", kerma_size
      call ndi_get_size(NDI_HEATING, heating_size, ierr)
      print *, "Heating size is: ", heating_size
      print *, ""


      call ndi_get_float64_vec(NDI_VEL, kerma, vec_size, ierr)
      print *, "Velocities:"
      call print_float64_vec(kerma, kerma_size)

      ! Use default kerma/heating */
      call ndi_get_float64_vec(NDI_HEATING, heating, vec_size, ierr)
      print *, "Default heating:"
      call print_float64_vec(heating, heating_size)
      if (ierr.ne.0) then 
         print *, "  Error", ierr,
     $        "getting heating (default) for isotope u235."
      endif
      call ndi_get_float64_vec(NDI_KERMA, kerma, vec_size, ierr)
      print *, "Default kerma:"
      call print_float64_vec(kerma, kerma_size)
      if (ierr.ne.0) then 
         print *, "  Error", ierr,
     $        "getting kerma (default) for isotope u235."
      endif
      print *, ""

      ! Use kerma/heating from library (same as default 
      call ndi_set_option(NDI_KERMA, "use_data", ierr)
      call ndi_get_float64_vec(NDI_HEATING, heating, vec_size, ierr)
      print *, "Library heating:"
      call print_float64_vec(heating, heating_size)
      if (ierr.ne.0) then 
         print *, "  Error", ierr,
     $        "getting heating (data) for isotope u235."
      endif
      call ndi_get_float64_vec(NDI_KERMA, kerma, vec_size, ierr)
      print *, "Library kerma:"
      call print_float64_vec(kerma, kerma_size)
      if (ierr.ne.0) then 
         print *, "  Error", ierr,
     $        "getting kerma (data) for isotope u235."
      endif
      print *, ""

      ! Use kerma/heating from library if available, otherwise use calculation 
      ! In mendf6 library kerma/heating values ARE available, so this option will 
      !      return the same as LIBRARY values above 
      call ndi_set_option(NDI_KERMA, "use_data_calc", ierr)
      call ndi_get_float64_vec(NDI_HEATING, heating, vec_size, ierr)
      print *, "Data/calc heating:"
      call print_float64_vec(heating, heating_size)
      if (ierr.ne.0) then 
         print *, "  Error", ierr,
     $        "getting heating (data_calc) for isotope u235."
      endif
      call ndi_get_float64_vec(NDI_KERMA, kerma, vec_size, ierr)
      print *, "Data/calc kerma:"
      call print_float64_vec(kerma, kerma_size)
      if (ierr.ne.0) then 
         print *, "  Error", ierr,
     $        "getting kerma (data_calc) for isotope u235."
      endif
      print *, ""


      ! Use calculated kerma/heating only 
      call ndi_set_option(NDI_KERMA, "use_legacy_calc", ierr)
      call ndi_get_float64_vec(NDI_HEATING, heating, vec_size, ierr)
      print *, "Calculated heating:"
      call print_float64_vec(heating, heating_size)
      if (ierr.ne.0) then 
         print *, "  Error", ierr,
     $        "getting heating (calc) for isotope u235."
      endif
      call ndi_get_float64_vec(NDI_KERMA, kerma, vec_size, ierr)
      print *, "Calculated kerma:"
      call print_float64_vec(kerma, kerma_size)
      if (ierr.ne.0) then 
         print *, "  Error", ierr,
     $        "getting kerma (calc) for isotope u235."
      endif
      print *, ""


      ! See how average energy option affects calculated kerma/heating values 
      call ndi_set_option( NDI_E_AVG, "from_sgndf" , ierr)
      call ndi_set_option(NDI_KERMA, "use_legacy_calc", ierr)
      call ndi_get_float64_vec(NDI_HEATING, heating, vec_size, ierr)
      print *, "Calculated heating (SGNDF energy):"
      call print_float64_vec(heating, heating_size)
      if (ierr.ne.0) then 
         print *, "  Error", ierr,
     $        "getting heating (calc/sgndf energy) for isotope u235."
      endif
      call ndi_get_float64_vec(NDI_KERMA, kerma, vec_size, ierr)
      print *, "Calculated kerma (SGNDF energy):"
      call print_float64_vec(kerma, kerma_size)
      if (ierr.ne.0) then 
         print *, "  Error", ierr,
     $        "getting kerma (calc/sgndf energy) for isotope u235."
      endif
      print *, ""

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
