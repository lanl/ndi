#include "ndi_keywords.h"

#define MAX_VEC_SIZE 200

      program main

      integer ierr
      integer gendir
      integer mpmg
      integer handle(3)
      integer return_int
      integer int_vec_size
      integer int_vec(MAX_VEC_SIZE)
      integer flt_vec_size
      integer pebounds
      real*8 pbounds(3)
      real*8 flt64_vec(MAX_VEC_SIZE)
      
      pebounds = 3
      data pbounds / 20.0, 4.0, 0.1 /

c     Inintialize gendir 
      call ndi2_open_gendir(gendir, "", ierr)
      if (ierr .ne. 0) then
         write(*, 10) ierr
 10      format("  **Error ", i3, " in NDI open 0.")
      endif

c     Get mpmg handle 
      call ndi2_get_handle_gendir(gendir, mpmg, ierr)
      if (ierr .ne. 0) then
         write(*, 20) ierr
 20      format("  **Error ", i3, " in NDI get_handle 0.")
      endif

c      set mpmg handle type 
       call ndi2_set_lib_type(mpmg, "multigroup_multi", ierr)
       if (ierr .ne. 0) then
          write(*,30) ierr
 30       format("  **Error ", i3, " in NDI set_lib_type 0.")
       endif

c     Initialize particle 0 
       call ndi2_get_handle_mpmg(mpmg, handle(0), ierr)
       if (ierr .ne. 0) then
          write(*,40) ierr
 40       format("  **Error ", i3, " in NDI get_handle_mpmg 0." )
       endif

       call ndi2_set_option(handle(0), NDI_LIB_TYPE,
     $      "multigroup_neutron", ierr)
       call ndi2_set_option(handle(0), NDI_LIBRARY, "specmg", ierr)
       call ndi2_set_option(handle(0), NDI_COLLAPSE, "2_lanl", ierr)
       call ndi2_set_isotope(handle(0), "U235", ierr)

c     Initialize particle 1 
       call ndi2_get_handle_mpmg(mpmg, handle(1), ierr)
       if (ierr .ne. 0) then
          write(*, 50) ierr
 50       format("  **Error ", i3, " in NDI get_handle_mpmg 1." )
       endif

       call ndi2_set_option(handle(1), NDI_LIB_TYPE,
     $      "multigroup_photon", ierr)
       call ndi2_set_option(handle(1), NDI_LIBRARY, "photmg",
     $      ierr)
       call ndi2_set_float64_vec_option(handle(1), NDI_COLLAPSE,
     $      pbounds, pebounds, ierr)
       call ndi2_set_isotope(handle(1), "92000", ierr)

c     Initialize particle 2 
       call ndi2_get_handle_mpmg(mpmg, handle(2), ierr)
       if (ierr .ne. 0) then
          write(*, 60) ierr
 60       format("  **Error ", i3, " in NDI init 2." )
       endif

       call ndi2_set_option(handle(2), NDI_LIB_TYPE,
     $      "multigroup_neutron", ierr)
       call ndi2_set_option(handle(2), NDI_LIBRARY, "mendf5",
     $      ierr)
       call ndi2_set_option(handle(2), NDI_COLLAPSE, "2_lanl",
     $      ierr)

       call ndi2_set_isotope(handle(2), "U238", ierr)

c     start accessing data 
       call ndi2_get_int_val(mpmg, NDI_NUM_GRPS, return_int,
     $      ierr)
       if (ierr .ne. 0) then 
          write(*, 70) ierr
 70       format("  **Error ", i3, " getting number of groups.")
       endif
       write(*, 80) return_int
 80    format("  Number of groups:  ", i3)

       call ndi2_get_int_val(mpmg, NDI_PN_ORDER, return_int,
     $      ierr)
       if (ierr .ne. 0) then 
          write(*, 90) ierr
 90        format("  **Error ", i3, " getting pn order.")
        endif
        write(*, 100) return_int
 100    format("  Pn order:  ", i3)

        call ndi2_get_int_val(mpmg, NDI_NUM_PNU_PR, return_int,
     $       ierr)
        if (ierr .ne. 0) then 
           write(*, 110) ierr
 110        format("  **Error ", i3, " getting num pnu.")
         endif
         write(*, 120) return_int
 120     format("  No. pnu:  ", i3)

         call ndi2_get_int_val(mpmg, NDI_NUM_REAC, return_int,
     $        ierr)
         if (ierr .ne. 0) then 
            write(*, 130) ierr
 130        format("  **Error ", i3, " getting num reactions.")
         endif
         write(*, 140) return_int
 140     format("  No. reactions:  ", i3)
         
c     LAST NEUTRON AND PHOTON GRPS 
         call ndi2_get_int_val_n(mpmg, NDI_LAST_GRP, 0, return_int,
     $        ierr)
         if (ierr .ne. 0) then 
            write(*, 150) ierr
 150        format("  **Error ", i3, " getting last neutron group.")
         endif
         write(*, 160) return_int
 160     format("  Last neutron group:  ", i3)
         call ndi2_get_int_val_n(mpmg, NDI_LAST_GRP, 1, return_int,
     $        eirr)
         if (ierr .ne. 0) then 
            write(*, 170) ierr
 170        format("  **Error ", i3, " getting last photon.")
         endif
         write(*, 180) return_int
 180     format("  Last photon group:  ", i3,)
         
         
c     LIST OF MTS 
         call ndi2_get_size(mpmg, NDI_REAC_MT, int_vec_size, ierr) 
         if (ierr .ne. 0) then 
            write(*, 190) ierr
 190        format("  **Error ", i3, " getting size of reaction MTs.")
         endif
         write(*, 200) int_vec_size
 200     format("  Reaction MTs size:  ", i3) 
         
         call ndi2_get_int_vec(mpmg, NDI_REAC_MT, int_vec,
     $        MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 210) ierr
 210        format("  **Error ", i3, " getting reaction MTs.")
         endif
         write(*, 220) (int_vec(j), j = 1, int_vec_size)
 220     format(5(i5, 2x))
         
c     EBOUNDS 
         call ndi2_get_size(mpmg, NDI_E_BOUNDS, flt_vec_size, ierr)
         if (ierr .ne. 0) then 
            write(*, 230) ierr
 230        format("  **Error ", i3, " getting size of ebounds.")
         endif
         write(*, 240) flt_vec_size
 240     format("  Ebounds size:  ", i3)
         
         call ndi2_get_float64_vec(mpmg, NDI_E_BOUNDS,
     $        flt64_vec, MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 250) ierr
 250        format("  **Error ", i3, " getting energy boundaries.")
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
 260     format(6(1pe15.8, 2x))
         
         
c     TOTAL CROSS SECTION 
         call ndi2_get_size(mpmg, NDI_SIG_TOT, flt_vec_size, ierr)
         if (ierr .ne. 0) then 
            write(*, 270) ierr
 270        format("  **Error ", i3,
     $           " getting size of total cross section.")
         endif
         write(*, 280) flt_vec_size
 280     format("  Total cross-section size:  ", i3)
         
         call ndi2_get_float64_vec(mpmg, NDI_SIG_TOT, flt64_vec,
     $        flt_vec_size, ierr)
         if (ierr .ne. 0) then 
            write(*, 290) ierr
 290        format("  **Error ", i3, " getting total cross section.")
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
         
         
c     SCATTERING MATRIX 
         call ndi2_get_size_n(mpmg, NDI_PN_FULL, 0, flt_vec_size, ierr)
         if (ierr .ne. 0) then 
            write(*, 300) ierr
 300        format("  **Error ", i3,
     $           " getting size of scattering matrix.")
         endif
         write(*, 310) flt_vec_size
 310     format("  Scatter matrix size:  ", i3)
   
         call ndi2_get_float64_vec_n(mpmg, NDI_PN_FULL, 0,
     $        flt64_vec, flt_vec_size, ierr)
         if (ierr .ne. 0) then 
            write(*, 320) ierr
 320        format("  **Error ", i3, " getting scattering matrix.")
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
         
c     CLOSE 
         call ndi2_close_gendir(gendir, ierr)
         if (ierr .ne. 0) then
            write(*, 330) ierr
 330        format("  **Error ", i3, " in closing NDI 0.")
         endif
         
         end
      
