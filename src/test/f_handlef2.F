#include "ndi_keywords.h"

c Valgrind errors are expected because of the double open_gendir
c and close_gendir calls.

      program main

      integer FVECS, AWRS, STRS, ZAIDS
      parameter (FVECS=2000, AWRS=500, STRS=500, ZAIDS=13)
      
      character*(ZAIDS) zaid, tmp_zaid
      character*132 info
c      character*30 vers
      character*12 vec_str
      integer gendir_h
      integer handle
      integer int_val_num
      integer int_val_n_num
      integer int_vec_num
      integer int_vec_n_num
      integer flt_val_num
      integer flt_val_n_num
      integer flt_vec_num
      integer flt_vec_n_num
      integer flt_vec_x_num
      integer sec_part_int_val
      integer sec_part_float_vec
      integer sec_part_float_vec_n
      integer int_value(10)
      integer int_val_n(4)
      integer int_vec(3)
      integer int_vec_n(6)
      integer flt_value(4)
      integer flt_val_n(1)
      integer flt_vec(25)
      integer flt_vec_n(8)
      integer flt_vec_x(1)
      integer sec_part(6)
      character*12 int_value_str(10)
      character*12 int_val_n_str(4)
      character*12 int_vec_str(3)
      character*12 int_vec_n_str(6)
      character*12 flt_val_str(4)
      character*12 flt_val_n_str(1)
      character*12 flt_vec_str(25)
      character*12 flt_vec_n_str(8)
      character*12 flt_vec_x_str(1)
      character*12 sec_part_str(6)
      character*12 zaid_list(8)
      character*(ZAIDS) str_arr(STRS)
      character*8   num_grp_str
      integer return_int, vec_num, return_int_vec(20)
      integer num_element, zd_cnt, arr_size
      real*8 return_flt, return_flt_vec(FVECS)


      int_val_num = 10
      data int_value / NDI_NUM_GRPS, NDI_UP_GRPS,
     $     NDI_DOWN_GRPS , NDI_PN_ORDER,
     $     NDI_NUM_REAC, NDI_NUM_SEC_PARTS,
     $     NDI_NUM_TARGET, NDI_NUM_RPRODS,
     $     NDI_NUM_GRPS_CURR_PART, NDI_PN_ORDER_CURR_PART/

      data int_value_str /"Groups", "Upscat", "Downscat", "ScatMat",
     $     "Edits", "SecPart", "NumTarget", "NumRProds", "PhGrps",
     $     "PhScatMat"/

      int_val_n_num = 4
      data int_val_n / NDI_RPROD_ALL,
     $     NDI_RPROD_RMO,
     $     NDI_RPROD_FEW, NDI_REAC_MT /

      data int_val_n_str /"NumProdAll", "NumProdRMO",
     $     "NumProdFew", "MT" /

      int_vec_num = 3
      data int_vec / NDI_RPROD_ALL, NDI_RPROD_RMO,
     $     NDI_RPROD_FEW /

      data int_vec_str / "Rx--all", "Rx--rmo",
     $     "Rx--few" /

      int_vec_n_num = 6
      data int_vec_n / NDI_RPROD_ID_ALL, NDI_RPROD_MLT_ALL,
     $     NDI_RPROD_ID_RMO, NDI_RPROD_MLT_RMO, NDI_RPROD_ID_FEW,
     $     NDI_RPROD_MLT_FEW /

      data int_vec_n_str / "Rx id all", "Rx mult all",
     $     "Rx id rmo", "Rx mult rmo",
     $     "Rx id few", "Rx mult few" /

      flt_val_num = 4
      data flt_value / NDI_TEMP, NDI_SIG_0, NDI_AT_WGT,
     $     NDI_AWR /

      data flt_val_str / "Temp", "Sig0", "At wgt", "AWR" /

      flt_val_n_num = 1
      data flt_val_n / NDI_REAC_Q /

      data flt_val_n_str / "ReacQ" /

      flt_vec_num = 25
      data flt_vec / NDI_E_BOUNDS, NDI_WGTS, NDI_VEL,
     $     NDI_E_AVG, NDI_ENERGY_N, NDI_ENERGY_AVAIL,
     $     NDI_HEATING, NDI_KERMA, NDI_SIG_TOT, NDI_NU_TOT,
     $     NDI_NU_PR, NDI_NU_DEL, NDI_NU_SIG_F_TOT, NDI_NU_SIG_F_PR,
     $     NDI_NU_SIG_F_DEL, NDI_CHI_VEC_TOT, NDI_CHI_VEC_PR,
     $     NDI_CHI_VEC_DEL, NDI_CHI_MAT_TOT, NDI_CHI_MAT_PR, 
     $     NDI_CHI_MAT_DEL, NDI_NORM, NDI_FISS_Q, NDI_REAC_Q,
     $     NDI_E_BOUNDS_CURR_PART /

      data flt_vec_str /"Ebounds", "Weights", "Velocities","AvgE",
     $     "EnergyN", "EAvail", "Heating", "Kerma", "SigTot", "NuTot",
     $     "Nupr", "NuDel", "NubarTotSigF","NubarPrSigF", 
     $      "NubarDelSigF", "ChiVecTot", "ChiVecTPr", "ChiVecTDel",
     $     "ChiMatTot", "ChiMatPr", "ChiMatDel", "Norm", "FissQ", 
     $     "ReacQ", "PhotonE"/

      flt_vec_n_num = 8
      data flt_vec_n / NDI_PN_DOWN_FULL, NDI_PN_FULL, 
     $     NDI_TRANSFER_MAT_TOT, NDI_TRANSFER_MAT_PR, NDI_CORR_DIAG,
     $     NDI_CORR_BHS, NDI_SIG_REAC, NDI_PN_PROD_FULL_CURR_PART  /

      data flt_vec_n_str /"Pn Scat (LD)", "Pn Full", "TransMatTot",
     $      "TransMatPr", "DiagCorr", "BHSCorr", "Reaction XS",     
     $     "Ph scat (f)"/


      flt_vec_x_num = 1
      data flt_vec_x / NDI_TOT_PROD_XS /

      data flt_vec_x_str /"TotProdXS" /


      sec_part_int_val = 2
      sec_part_float_vec = 3
      sec_part_float_vec_n = 1 
      data sec_part / NDI_NUM_GRPS_CURR_PART, NDI_PN_ORDER_CURR_PART,
     $     NDI_E_BOUNDS_CURR_PART, NDI_HEATING_CURR_PART,
     $     NDI_KERMA_CURR_PART, NDI_PN_PROD_FULL_CURR_PART /

      data sec_part_str / "NumGrps", "PnOrder", "Ebounds", "PartHeat",
     $     "PartKerma", "ProdFull" /

      Data zaid_list / "92235.600nm", "1001.600nm", "26054.600nm",
     $   "b10", "s", "o16", "k41", "f19" /
c     $   "27056.600nm", "u235", "u-238", "o-16", "al27"/
c  "92234.50", "u-235" /


      call ndi2_open_gendir(gendir_h, " ", ierr)

c     Test error on initialization before close
      call ndi2_open_gendir(gendir_h, " ", ierr) 

      call ndi2_make_handle(gendir_h, "multigroup_neutron", 
     $     handle, ierr)

      num_grp_str = "2_group"

      write(*,300) "lib_type=", ierr
      call ndi2_set_option(handle,  NDI_LIBRARY, 'MENDF5', ierr)
      write(*,300) "library=", ierr
 300  format(a10,i3)
      call ndi2_set_option(handle,  NDI_E_AVG, 'from_e', ierrr)

c      call ndi2_set_option(handle,  NDI_COLLAPSE, num_grp_str, ierr) 

      call ndid_get_size( NDI_ZAID, "multigroup_neutron",
     $     arr_size, ierr )
      write(*, 299)  arr_size
 299  format("String array size is", i6)
c      call ndid_zaid_float64_vec( NDI_AWR, "multigroup_neutron", str_arr, STRS, 
c     $ ZAIDS, awr, AWRS, ierr )
c     print *, " ZAID float error is ", ierr
      call ndid_get_string_vec( NDI_ZAID, "multigroup_neutron",
     $     str_arr, STRS, ZAIDS, ierr )
      write(*, 298) ierr
 298  format(" String vec error is ", i6)

      do 100, zd_cnt = 1, arr_size
         
         if (zd_cnt.eq.8) then
            call ndi2_set_option(handle,  NDI_LIBRARY, "default",
     $           ierr)
         endif
          
c     Set new zaid
         zaid = str_arr(zd_cnt)
         call ndi2_set_isotope(handle, str_arr(zd_cnt), ierr )
         write(*, 301) zaid
 301     format("*****SETTING ISOTOPE ", a13 )
         write(*,302) zd_cnt, str_arr(zd_cnt), ierr
 302     format(i5, ":  >", a13, "<   err = ", i5)
         if (ierr.ne.0) then
            write(*,303) zaid
 303        format("Error setting isotope ", a13)
            goto 101
         endif

         call ndi2_set_option(handle,  NDI_CURR_PART, "0", ierr )

c     Return string values
         call ndi2_get_string_val(handle, NDI_ZAID, tmp_zaid, ierr)
         write(*, 304)  tmp_zaid, ierr
 304     format('ZAID: ', a13, ' err= ', i3)
         call ndi2_get_string_val(handle, NDI_INFO, info, ierr)
         write(*,2) info
 2       format( a132, / ) 
c         call ndi2_get_string_val(handle, NDI_VERSION, vers, ierr)
c         print *, "VERSION: ", vers
c         write(*,2)
         
c     Return integer values

         do 10 i = 1, int_val_num
            call ndi2_get_int_val(handle, int_value(i),
     $           return_int, ierr)
            write(*,11) int_value_str(i), return_int
 11         format(a12, ': ', i6)
 10      continue
         write(*,2) 
         
         
c     Return integer element from vector

         vec_num = 2
         do 20 i = 1, int_val_n_num
            call ndi2_get_int_val_n(handle, int_val_n(i), vec_num, 
     $           return_int, ierr)
            write(*,21) int_val_n_str(i), vec_num, return_int
 21         format(a12, i2, ': ', i6)
 20      continue
         write(*,2) 
         
         
c     Return float value

         do 50, i = 1, flt_val_num
            call ndi2_get_float64_val(handle, flt_value(i),
     $           return_flt, ierr)
            write(*, 51) flt_val_str(i), return_flt
 51         format(a12, ': ', 1pE12.5)
 50      continue
         write(*,2) 
         
         
c     Return float element from vector

         vec_num = 2
         do 53, i = 1, flt_val_n_num
            call ndi2_get_float64_val_n(handle, flt_val_n(i), vec_num,
     $            return_flt, ierr)
            write(*,52) flt_val_n_str(i), vec_num, return_flt
 52         format(a12, i3, ':', 1pE12.5)
 53      continue
         write(*,2) 
         
         
         
c     Return integer vector 
         do 40 i = 1, int_vec_num
            write(*, 42) int_vec_str(i)
   42       format(a12, ": ")
            call ndi2_get_size(handle, int_vec(i), num_element, ierr)
            call ndi2_get_int_vec(handle, int_vec(i), return_int_vec,
     $           20, ierr)
            if (num_element .gt. 20) then
               num_element = 20
            endif
            if (num_element.gt. 0) then
               print 41, (return_int_vec(j), j=1,num_element)
 41            format(10(i6, 3x))
            endif
 40      continue
         write(*,2) 
         
        
         
c     Return integer vector from matrix

         vec_num = 2
         do 43 i = 1, int_vec_n_num
            write(*, 45) int_vec_n_str(i), vec_num
 45         format(a12, i2, ": ")
            call ndi2_get_size_n(handle, int_vec_n(i), vec_num, 
     $           num_element, ierr)
            call ndi2_get_int_vec_n(handle, int_vec_n(i), vec_num, 
     $           return_int_vec, 20, ierr)
            if (num_element .gt. 20) then
               num_element = 20
            endif
            if ( num_element .gt. 0 ) then
               print 44, (return_int_vec(j), j=1,num_element)
 44            format(10(i6, 3x))
            endif
 43      continue
         write(*,2) 
         
         
c     Return float vector

         do 60 i = 1, flt_vec_num
            write(*, 62) flt_vec_str(i)
   62        format(a12, ": ")
            call ndi2_get_size(handle, flt_vec(i), num_element, ierr)
            call ndi2_get_float64_vec(handle, flt_vec(i), 
     $           return_flt_vec, FVECS, ierr)
            if (num_element .gt. FVECS) then
               num_element = FVECS
            endif
            if (num_element.gt. 0 ) then
               print 61, (return_flt_vec(j), j = 1, num_element)
 61            format(6(1pE12.5, 2x))
            endif
            if ( (i.eq.11).and.(zd_cnt.eq.5)) then
                call ndi2_set_option(handle,  NDI_MAT_ORDER, "row",
     $              ierr)
            endif
 60      continue
         write(*,2) 
         
c     Return float vector from matrix

         do 70 i = 1, flt_vec_n_num
             if (i.eq.3) then
                 vec_num = 2
             else
                 vec_num = 0
             endif
            write(*, 71) flt_vec_n_str(i), vec_num
 71         format(a12, i2,": ")
            call ndi2_get_size_n(handle, flt_vec_n(i), vec_num, 
     $           num_element, ierr)
            call ndi2_get_float64_vec_n(handle, flt_vec_n(i), vec_num,
     $           return_flt_vec,  FVECS, ierr)
            if (num_element .gt. FVECS) then
               num_element = FVECS
            endif
            if (num_element. gt. 0 ) then
               print 72, (return_flt_vec(j), j = 1, num_element)
 72            format(6(1pE12.5, 2x))
            endif
 70      continue
         write(*,2) 
         
         
c     Return float vector given string

         vec_str = "92235"
         do 80 i = 1, flt_vec_x_num
            write(*, 81) flt_vec_x_str(i), vec_str
 81         format(a12, "(", a12,") : ")
            call ndi2_get_size_x(handle, flt_vec_x(i), vec_str, 
     $           num_element, ierr)
            call ndi2_get_float64_vec_x(handle, flt_vec_x(i), vec_str,
     $           return_flt_vec,  FVECS, ierr)
            if (num_element .gt. FVECS) then
               num_element = FVECS
            endif
            if (num_element .gt. 0 ) then
               print 82, (return_flt_vec(j), j = 1, num_element)
 82            format(6(1pE12.5, 2x))
            endif
 80      continue
         write(*,2) 
                  

c     Return secondary particle data


c     Integer values
         index_start = 1
         do 90 i = index_start, index_start + sec_part_int_val - 1
            write(*, 92) sec_part_str(i)
 92         format(a12, ": ")
            call ndi2_get_size(handle, sec_part(i), num_element, ierr)
            call ndi2_get_int_val(handle, sec_part(i), return_int,
     $           ierr)
            write(*, 297) return_int
 297        format(i12)
 90      continue

c     Float vector
         index_start = index_start + sec_part_int_val
         do 93 i = index_start, index_start + sec_part_float_vec - 1
            write(*, 94) sec_part_str(i)
 94         format(a12, ": ")
            call ndi2_get_size(handle, sec_part(i), num_element, ierr)
            if (num_element .gt. 0 ) then
            call ndi2_get_float64_vec(handle, sec_part(i), 
     $           return_flt_vec, FVECS, ierr)
            if (num_element .gt. FVECS) then
               num_element = FVECS
            endif
               print 95, (return_flt_vec(j), j = 1, num_element)
 95            format(6(1pE12.5, 2x))
            endif
 93      continue


c     Float vector from matrix
         index_start = index_start + sec_part_float_vec 
         do 96 i = index_start, index_start + sec_part_float_vec_n - 1
            vec_num = 2
            write(*,97) sec_part_str(i), vec_num
 97         format(a12, i2,": ")
            call ndi2_get_size_n(handle, sec_part(i), vec_num, 
     $           num_element, ierr)
            if (num_element .gt. FVECS) then
               num_element = FVECS
            endif
            if ( num_element.gt. 0 ) then
            call ndi2_get_float64_vec_n(handle, sec_part(i), vec_num,
     $           return_flt_vec, FVECS, ierr)
               print 98, (return_flt_vec(j), j = 1, num_element)
 98            format(6(1pE12.5, 2x))
            endif

 96      continue
         write(*,2) 

 101     continue

 100  continue


      write(*, 296)
 296  format(/,/)
      call ndi2_set_option(handle,  NDI_LIB_TYPE,
     $     "multigroup_neutron", ierr)
      if (ierr . ne . 0 ) then
         write(*, 295) ierr
 295     format("  **Error ", i6, " setting lib type.")
      endif

      call ndi2_set_isotope(handle,  "94239.99", ierr)
      if (ierr . ne . 0 ) then
         write(*, 294) ierr
 294     format("  **Error ", i6, " setting 94239.99.")
      else 
         write(*, 293) 
 293     format("  **Isotope set successfully.")
      endif

      call ndi2_set_isotope(handle,  "92235.99", ierr)
      if (ierr . ne . 0 ) then
         write(*, 292) ierr
 292     format("  **Error ", i6, " setting 92235.")
      else 
         write(*, 291)
 291     format("  **Isotope set successfully.")
      endif

      call ndi2_close_gendir(gendir_h, ierr)

c     Test error on closing twice
      call ndi2_close_gendir(gendir_h, ierr)
      end





