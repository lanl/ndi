#include "ndi_keywords.h"


      program main

      integer i
      integer ierr
      integer mt
      integer ret_size
      integer vec_size
      integer ret_int(500)
      real*8 ret_vec64(500)


      call ndi_init(ierr)
      call ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron", ierr)
      call ndi_set_option( NDI_LIBRARY, "mendf6", ierr)
      call ndi_set_option( NDI_COLLAPSE, "16_llnl", ierr)
      call ndi_set_option( NDI_REAC_ID, "c", ierr)
      call ndi_set_isotope("pu239", ierr)

      vec_size = 500
      call ndi_get_size(NDI_REAC_MT, ret_size, ierr)
      if (ierr.ne.0) then
         print *, "  Error", ierr,
     $        "getting MT size for isotope pu239."
      endif
      print *, "MT size is: ", ret_size

      ! Get reaction ID array
      call ndi_get_int_vec(NDI_REAC_MT, ret_int, vec_size, ierr)
      if (ierr.ne.0) then
         print *, "  Error", ierr,
     $        "getting MTs for isotope pu239."
      endif
      print *, "Reaction IDs:"
      call print_int_vec(ret_int, ret_size)
      

      ! Get reaction ID's
      call ndi_get_int_val(NDI_NUM_REAC, ret_size, ierr)
      print *, "Reaction IDs:"
      do 10 i = 1, ret_size
         call ndi_get_int_val_n(NDI_REAC_MT, i-1, mt, ierr)
         if (ierr.ne.0) then
            print *, mt, "  Error", ierr,
     $           "getting MTs for isotope pu239."
         else
            print *, "   ", mt
         endif
 10   continue


      ! Get energy_avail and calculated kerma
      call ndi_set_option(NDI_KERMA, "use_calc", ierr)
      call ndi_get_size(NDI_KERMA, ret_size, ierr)
      call ndi_get_float64_vec(NDI_KERMA, ret_vec64, vec_size, ierr)
      if (ierr.ne.0) then
         print *, "  Error ", ierr,
     $        " getting calculated kerma for isotope pu239." 
      endif
      print *, "Calculated kerma:"
      call print_float64_vec(ret_vec64, ret_size)

      call ndi_get_size(NDI_ENERGY_AVAIL, ret_size, ierr)
      call ndi_get_float64_vec(NDI_ENERGY_AVAIL, ret_vec64,
     $     vec_size, ierr)
      if (ierr.ne.0) then
         print *, "  Error ", ierr,
     $        " getting available energy for isotope pu239." 
      endif
      print *, "Available energy:"
      call print_float64_vec(ret_vec64, ret_size)
   
    
      ! Total depletion cross section
      call ndi_get_size(NDI_TOT_DEPL_XS, ret_size, ierr)
      call ndi_get_float64_vec(NDI_TOT_DEPL_XS, ret_vec64,
     $     vec_size, ierr)
      if (ierr.ne.0) then
         print *, "  Error", ierr,
     $        "getting total depletion cross section for isotope pu239." 
      endif
      print *, "Total depletion cross section:"
      call print_float64_vec(ret_vec64, ret_size)

    
      ! Total production cross section
      call ndi_get_float64_val_n(NDI_TOT_PROD_Q, 94240, q, ierr)
      print *, "Total production Q for 94249:", q
      if (ierr.ne.0) then
         print *, "  Error", ierr,
     $        "getting total production Q for 94240."
      endif

      call ndi_get_size_n(NDI_TOT_PROD_XS, 0, ret_size, ierr)
      call ndi_get_float64_vec_n(NDI_TOT_PROD_XS, 94240, ret_vec64,
     $     vec_size, ierr)
      if (ierr.ne.0) then
         print *, "  Error", ierr,
     $   "getting tot production cross section (int) for isotope pu239." 
      endif
      print *, "Total production cross section (int) for 94240:"
      call print_float64_vec(ret_vec64, ret_size)

      call ndi_get_float64_vec_x(NDI_TOT_PROD_XS, "94240", ret_vec64,
     $     vec_size, ierr)
      if (ierr.ne.0) then
         print *, "  Error", ierr,
     $   "getting tot production cross section (str) for isotope pu239." 
      endif
      print *, "Total production cross section (str) for 94240:"
      call print_float64_vec(ret_vec64, ret_size)


      call ndi_close(ierr)
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

      subroutine print_int_vec(int_vec, num_elements)
      
      integer num_elements
      integer int_vec(*)

      print 30, (int_vec(j), j=1,num_elements)
 30   format(6(i8, 2x))
      
      return
      end

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
