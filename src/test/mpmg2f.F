#include "ndi_keywords.h"

#define MAX_VEC_SIZE 70000

      program main

      integer ierr
      integer i
      integer gendir_handle
      integer data_handle
      integer return_int
      integer int_vec_size
      integer int_vec(MAX_VEC_SIZE)
      integer flt_vec_size
      integer handle0, handle1
      integer num_libraries
      character*8 library(3)
      character*16 part_zaid
      real*8 flt64_vec(MAX_VEC_SIZE)
      
      num_libraries = 3
      data library / 'ngk6', 'm6_ng', 'acti_ng' /

      do 500, i = 1, num_libraries

c     Initialize gendir 
         call ndi2_open_gendir(gendir_handle, "", ierr)
         if (ierr .ne. 0) then
            write(*, 10) ierr
 10         format("  **Error ", i3, " in NDI open gendir.")
         endif

c     Get mpmg handle 
         call ndi2_get_handle_gendir(gendir_handle, data_handle, ierr)
         if (ierr .ne. 0) then
            write(*, 20) ierr
 20         format("  **Error ", i3, " in NDI get_handle mpmg.")
         endif

c     set mpmg handle type 
         call ndi2_set_lib_type(data_handle, "multigroup_multi", ierr)
         if (ierr .ne. 0) then
            write(*,30) ierr
 30         format("  **Error ", i3, " in NDI set_lib_type mpmg.")
         endif

c     setup particles
         write(*, 40) library(i)
 40      format("LIBRARY: ", a10)
         call ndi2_set_option(data_handle, NDI_LIBRARY, library(i),
     $        ierr)
         call ndi2_set_zaid(data_handle, "92235", ierr)

c     test getting zaid back
         call ndi2_get_size(data_handle, NDI_ZAID, int_vec_size, ierr)
         if (ierr .ne. 0) then
            write(*, 42) ierr
 42         format("  **Error ", i3, "getting zaid size")
         endif
         call ndi2_get_string_val(data_handle, NDI_ZAID, part_zaid,
     $        ierr)
         if (ierr .ne. 0) then
            write(*, 43) ierr
 43         format("  **Error ", i3, "getting zaid")
         endif

c     start accessing data 
         call ndi2_get_int_val(data_handle, NDI_NUM_GRPS, return_int,
     $        ierr)
         if (ierr .ne. 0) then 
            write(*, 70) ierr
 70         format("  **Error ", i3, " getting number of groups.")
         endif
         write(*, 80) return_int
 80      format("  Number of groups:  ", i3)

         call ndi2_get_int_val(data_handle, NDI_PN_ORDER, return_int,
     $        ierr)
         if (ierr .ne. 0) then 
            write(*, 90) ierr
 90         format("  **Error ", i3, " getting pn order.")
         endif
         write(*, 100) return_int
 100     format("  Pn order:  ", i3)

         call ndi2_get_int_val(data_handle, NDI_NUM_PNU_PR, return_int,
     $        ierr)
         if (ierr .ne. 0) then 
            write(*, 110) ierr
 110        format("  **Error ", i3, " getting num pnu.")
         endif
         write(*, 120) return_int
 120     format("  No. pnu:  ", i3)

         call ndi2_get_int_val(data_handle, NDI_NUM_REAC, return_int,
     $        ierr)
         if (ierr .ne. 0) then 
            write(*, 130) ierr
 130        format("  **Error ", i3, " getting num reactions.")
         endif
         write(*, 140) return_int
 140     format("  No. reactions:  ", i3)
         
c     LAST NEUTRON AND PHOTON GRPS 
         call ndi2_get_int_val_n(data_handle, NDI_LAST_GRP, 0, 
     $        return_int, ierr)
         if (ierr .ne. 0) then 
            write(*, 150) ierr
 150        format("  **Error ", i3, " getting last neutron group.")
         endif
         write(*, 160) return_int
 160     format("  Last neutron group:  ", i3)
         call ndi2_get_int_val_n(data_handle, NDI_LAST_GRP, 1, 
     $        return_int, ierr)
         if (ierr .ne. 0) then 
            write(*, 170) ierr
 170        format("  **Error ", i3, " getting last photon.")
         endif
         write(*, 180) return_int
 180     format("  Last photon group:  ", i3)
         
         
c     LIST OF MTS 
         call ndi2_get_size(data_handle, NDI_REAC_MT, int_vec_size,
     $        ierr) 
         if (ierr .ne. 0) then 
            write(*, 190) ierr
 190        format("  **Error ", i3, " getting size of reaction MTs.")
         endif
         write(*, 200) int_vec_size
 200     format("  Reaction MTs size:  ", i3) 
         
         call ndi2_get_int_vec(data_handle, NDI_REAC_MT, int_vec,
     $        MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 210) ierr
 210        format("  **Error ", i3, " getting reaction MTs.")
         endif
         write(*, 220) (int_vec(j), j = 1, int_vec_size)
 220     format(5(i5, 2x))
         
c     EBOUNDS 
         call ndi2_get_size(data_handle, NDI_E_BOUNDS, flt_vec_size,
     $        ierr)
         if (ierr .ne. 0) then 
            write(*, 230) ierr
 230        format("  **Error ", i3, " getting size of ebounds.")
         endif
         write(*, 240) flt_vec_size
 240     format("  Ebounds size:  ", i3)
         
         call ndi2_get_float64_vec(data_handle, NDI_E_BOUNDS,
     $        flt64_vec, MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 250) ierr
 250        format("  **Error ", i3, " getting energy boundaries.")
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
 260     format(6(1pe15.8, 2x))
         
c     EAVG 
         call ndi2_get_size(data_handle, NDI_E_AVG, flt_vec_size,
     $        ierr)
         if (ierr .ne. 0) then 
            write(*, 231) ierr
 231        format("  **Error ", i3, " getting size of eavg.")
         endif
         write(*, 241) flt_vec_size
 241     format("  Eavg size:  ", i3)
         
         call ndi2_get_float64_vec(data_handle, NDI_E_AVG,
     $        flt64_vec, MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 251) ierr
 251        format("  **Error ", i3, " getting average energies.")
         endif
         write(*, 261) (flt64_vec(j), j = 1, flt_vec_size)
 261     format(6(1pe15.8, 2x))
         
c     WEIGHTS
         call ndi2_get_size(data_handle, NDI_WGTS, flt_vec_size, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting size of weights."
 275        format("  **Error ", i3, a40 )
         endif
         write(*, 285) "  Weight size:  ", flt_vec_size
 285     format(a40, i3)
         
         call ndi2_get_float64_vec(data_handle, NDI_WGTS, flt64_vec,
     $        MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting weights."
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
          
c     VELOCITIES
         call ndi2_get_size(data_handle, NDI_VEL, flt_vec_size, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting size of velocities."
         endif
         write(*, 285) "  Velocity size:  ", flt_vec_size
         
         call ndi2_get_float64_vec(data_handle, NDI_VEL, flt64_vec,
     $        MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting velocity."
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
          
c     TOTAL CROSS SECTION 
         call ndi2_get_size(data_handle, NDI_SIG_TOT, flt_vec_size,
     $        ierr)
         if (ierr .ne. 0) then 
            write(*, 270) ierr
 270        format("  **Error ", i3,
     $           " getting size of total cross section.")
         endif
         write(*, 280) flt_vec_size
 280     format("  Total cross-section size:  ", i3)
         
         call ndi2_get_float64_vec(data_handle, NDI_SIG_TOT, flt64_vec,
     $        MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 290) ierr
 290        format("  **Error ", i3, " getting total cross section.")
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
         
c     CHI VECTOR
         call ndi2_get_size(data_handle, NDI_CHI_VEC_PR, flt_vec_size,
     $        ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting size of prompt chi vector."
         endif
         write(*, 285) "  Prompt chi size:  ", flt_vec_size
         
         call ndi2_get_float64_vec(data_handle, NDI_CHI_VEC_PR, 
     $        flt64_vec, MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting prompt chi vector."
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
         
c     NUBAR
         call ndi2_get_size(data_handle, NDI_NU_PR, flt_vec_size, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting size of prompt nubar."
         endif
         write(*, 285) "  Prompt nubar size:  ", flt_vec_size
         
         call ndi2_get_float64_vec(data_handle, NDI_NU_PR, flt64_vec,
     $        MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting prompt nubar."
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
         
c     NUBAR_SIG_F
         call ndi2_get_size(data_handle, NDI_NU_SIG_F_PR, flt_vec_size,
     $        ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting size of prompt nu_sigf."
         endif
         write(*, 285) "  Prompt nu_sigf size:  ", flt_vec_size
         
         call ndi2_get_float64_vec(data_handle, NDI_NU_SIG_F_PR, 
     $        flt64_vec, MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting prompt nu_sigf."
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
         
c     PNU PROMPT
         call ndi2_get_size_n(data_handle, NDI_PNU_PR, 2, flt_vec_size,
     $        ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting size of prompt pnu."
         endif
         write(*, 285) "  Prompt pnu:  ", flt_vec_size
         
         call ndi2_get_float64_vec_n(data_handle, NDI_PNU_PR, 2, 
     $        flt64_vec, MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting prompt pnu. (2)"
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
         
c     SIG*PNU PROMPT
         call ndi2_get_size_n(data_handle, NDI_SIG_PNU_PR, 2,
     $        flt_vec_size, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting size of prompt sig_pnu."
         endif
         write(*, 285) "  Prompt sig*pnu:  ", flt_vec_size
         
         call ndi2_get_float64_vec_n(data_handle, NDI_SIG_PNU_PR, 2, 
     $        flt64_vec,MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr, " getting prompt sig*pnu. (2)"
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
         
c     REACTION CROSS SECTION (MT=2)
         call ndi2_get_size_n(data_handle, NDI_SIG_REAC, 2,
     $        flt_vec_size, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr,
     $           " getting size of reaction cross section."
         endif
         write(*, 285) "  Reaction cross section (mt=2):  ",
     $        flt_vec_size
         
         call ndi2_get_float64_vec_n(data_handle, NDI_SIG_REAC, 2, 
     $        flt64_vec, MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr,
     $           " getting reaction cross section (mt=2)."
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
         
c     DIAGONAL TRANSPORT CORRECTION (order = 2)
         call ndi2_get_size_n(data_handle, NDI_CORR_DIAG, 2,
     $        flt_vec_size, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr,
     $           " getting size of diagonal transport correction."
         endif
         write(*, 285) "  Diagonal transport correction (2):  ",
     $        flt_vec_size
         
         call ndi2_get_float64_vec_n(data_handle, NDI_CORR_DIAG, 2,
     $         flt64_vec, MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr,
     $           " getting diagonal transport correction."
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
          
c     BHS TRANSPORT CORRECTION (order = 2)
         call ndi2_get_size_n(data_handle, NDI_CORR_BHS, 2,
     $        flt_vec_size, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr,
     $           " getting size of BHS transport correction."
         endif
         write(*, 285) "  BHS transport correction (2):  ",
     $        flt_vec_size
         
         call ndi2_get_float64_vec_n(data_handle, NDI_CORR_BHS, 2,
     $         flt64_vec, MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 275) ierr,
     $           " getting BHS transport correction (2)."
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)


c     SCATTERING MATRIX 
         call ndi2_get_size_n(data_handle, NDI_PN_FULL, 0, flt_vec_size,
     $        ierr)
         if (ierr .ne. 0) then 
            write(*, 300) ierr
 300        format("  **Error ", i3,
     $           " getting size of scattering matrix.")
         endif
         write(*, 310) flt_vec_size
 310     format("  Scatter matrix size:  ", i5)
         
         call ndi2_get_float64_vec_n(data_handle, NDI_PN_FULL, 0,
     $        flt64_vec, MAX_VEC_SIZE, ierr)
         if (ierr .ne. 0) then 
            write(*, 320) ierr
 320        format("  **Error ", i3, " getting scattering matrix.")
         endif
         write(*, 260) (flt64_vec(j), j = 1, flt_vec_size)
         
c     ACCESS SINGLE-PARTICLE DATA
         call ndi2_get_int_val_n(data_handle, NDI_PARTICLE_N, 0, handle0,
     $        ierr)
         call ndi2_get_string_val(handle0, NDI_ZAID, part_zaid,
     $         ierr)
         if (ierr .ne. 0) then
            write(*,330) ierr
 330        format(" ***Error getting handle #0 is ", i3)
         endif
         write (*,335) "#0", part_zaid
 335     format("  ZAID FOR PARTICLE ", a2, ":  ", a16)

         call ndi2_get_int_val_n(data_handle, NDI_PARTICLE_N, 1,
     $        handle1, ierr)
         call ndi2_get_string_val(handle1, NDI_ZAID, part_zaid,
     $        ierr)
         if (ierr .ne. 0) then
            write(*,340) ierr
 340        format(" ***Error getting handle #1 is ", i3)
         endif
         write (*,335) "#1", part_zaid
            
         call ndi2_get_int_val_n(data_handle, NDI_PARTICLE_OF_TYPE, 0, 
     $        handle0, ierr)
         call ndi2_get_string_val(handle0, NDI_ZAID, part_zaid,
     $        ierr )
         if (ierr .ne. 0) then
            write(*,350) ierr
 350        format(" ***Error getting handle of particle 0 is ", i3)
         endif
         write (*,335) "0", part_zaid

         call ndi2_get_int_val_n(data_handle, NDI_PARTICLE_OF_TYPE, 1,
     $        handle1, ierr)
         call ndi2_get_string_val(handle1, NDI_ZAID, part_zaid,
     $        ierr)
         if (ierr .ne. 0) then
            write(*,360) ierr
 360        format(" ***Error getting handle of particle 1 is ", i3)
         endif
         write (*,335) "1", part_zaid


c     CLOSE 
         call ndi2_close_gendir(gendir_handle, ierr)
         if (ierr .ne. 0) then
            write(*, 330) ierr
 370        format("  **Error ", i3, " in closing NDI 0.")
         endif

         write(*, 380)
         write(*, 380)
 380     format()
         
 500  continue
      end
      
