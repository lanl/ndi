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
      call ndi_set_option( NDI_LIBRARY, "mendf5", ierr)
      call ndi_set_option( NDI_COLLAPSE, "16_llnl", ierr)
      call ndi_set_isotope("u235", ierr)

      vec_size = 500
      call ndi_get_size(NDI_KERMA, kerma_size, ierr)
      write (*,2 ) kerma_size
    2 format("Kerma size is: ", i3)
      call ndi_get_size(NDI_HEATING, heating_size, ierr)
      write (*, 3) heating_size
    3 format("Heating size is: ", i3,/)

      ! Use default kerma/heating 
      call ndi_get_float64_vec(NDI_HEATING, heating, heating_size, ierr)
      write (*, 4)
    4 format("Default heating:")
      call print_float64_vec(heating, heating_size)
      if (ierr.ne.0) then 
         write (*, 5) ierr
    5    format("  Error ", i3,
     $        " getting heating (default) for isotope u235.")
      endif
      call ndi_get_float64_vec(NDI_KERMA, kerma, vec_size, ierr)
      write (*, 6)
    6 format("Default kerma:")
      call print_float64_vec(kerma, kerma_size)
      if (ierr.ne.0) then 
         write (*, 7) ierr
    7    format("  Error ", i3,
     $        " getting kerma (default) for isotope u235.")
      endif
      write (*, 8)
    8 format(/)

      ! Use kerma/heating from library (same as default)
      call ndi_set_option(NDI_KERMA, "use_data", ierr)
      call ndi_get_float64_vec(NDI_HEATING, heating, heating_size, ierr)
      write (*, 9)
    9 format("Library heating:")
      call print_float64_vec(heating, heating_size)
      if (ierr.ne.0) then 
         write (*, 10) ierr
   10    format("  Error ", i3,
     $        " getting heating (data) for isotope u235.")
      endif
      call ndi_get_float64_vec(NDI_KERMA, kerma, vec_size, ierr)
      write (*, 11)
   11 format("Library kerma:")
      call print_float64_vec(kerma, kerma_size)
      if (ierr.ne.0) then 
         write (*, 12) ierr
   12    format("  Error ", i3,
     $        " getting kerma (data) for isotope u235.")
      endif
      write (*, 13)
   13 format(/)

      ! Use kerma/heating from library if available, otherwise use calculation 
      ! In mendf5 library kerma/heating values ARE NOT available, so this option will 
      !      return the same as CALCULATED values below 
      call ndi_set_option(NDI_KERMA, "use_data_calc", ierr)
      call ndi_get_float64_vec(NDI_HEATING, heating, heating_size, ierr)
      write (*, 14)
   14 format("Data/calc heating:")
      call print_float64_vec(heating, heating_size)
      if (ierr.ne.0) then 
         write (*, 15) ierr
   15    format("  Error ", i3,
     $        " getting heating (data_calc) for isotope u235.")
      endif
      call ndi_get_float64_vec(NDI_KERMA, kerma, vec_size, ierr)
      write (*, 16)
   16 format("Data/calc kerma:")
      call print_float64_vec(kerma, kerma_size)
      if (ierr.ne.0) then 
         write (*, 17) ierr
   17    format("  Error ", i3,
     $        " getting kerma (data_calc) for isotope u235.")
      endif
      write (*, 18)
   18 format(/)


      ! Use calculated kerma/heating only 
      call ndi_set_option(NDI_KERMA, "use_calc", ierr)
      call ndi_get_float64_vec(NDI_HEATING, heating, heating_size, ierr)
      write (*, 19)
   19 format("Calculated heating:")
      call print_float64_vec(heating, heating_size)
      if (ierr.ne.0) then 
         write (*, 20) ierr
   20    format("  Error getting heating (calc) for isotope u235.")
      endif
      call ndi_get_float64_vec(NDI_KERMA, kerma, vec_size, ierr)
      write (*, 21)
   21 format("Calculated kerma:")
      call print_float64_vec(kerma, kerma_size)
      if (ierr.ne.0) then 
         write (*, 22) ierr
   22    format("  Error getting kerma (calc) for isotope u235.")
      endif
      write (*, 23)
   23 format(/)


      ! See how average energy option affects calculated kerma/heating values 
      call ndi_set_option( NDI_E_AVG, "from_sgndf" , ierr)
      call ndi_set_option(NDI_KERMA, "use_calc", ierr)
      call ndi_get_float64_vec(NDI_HEATING, heating, heating_size, ierr)
      write (*, 24)
   24 format("Calculated heating (SGNDF energy):")
      call print_float64_vec(heating, heating_size)
      if (ierr.ne.0) then 
         write (*, 25) ierr
   25    format("  Error getting heating (calc/sgndf energy)",
     $        "for isotope u235.")
      endif
      call ndi_get_float64_vec(NDI_KERMA, kerma, vec_size, ierr)
      write (*, 26)
   26 format("Calculated kerma (SGNDF energy):")
      call print_float64_vec(kerma, kerma_size)
      if (ierr.ne.0) then 
         write (*, 27) ierr
   27    format("  Error getting kerma (calc/sgndf energy)",
     $        "for isotope u235.")
      endif
      write (*, 28)
   28 format(/)

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
