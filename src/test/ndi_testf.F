#include "ndi_keywords.h"


      program main

      
      character*13 zaid, tmp_zaid
      character*132 info
      character*30 vers
      character*12 vec_str
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
      character*13 str_arr(500)
      character*8   num_grp_str
      integer return_int, vec_num, return_int_vec(20)
      integer num_element, zd_cnt, arr_size
      real*8 return_flt, return_flt_vec(900), awr(500)


      int_val_num = 10
      data int_value / NDI_NUM_GRPS, NDI_UP_GRPS,
     $     NDI_DOWN_GRPS , NDI_PN_ORDER,
     $     NDI_NUM_REAC, NDI_NUM_SEC_PARTS,
     $     NDI_NUM_TARGET, NDI_NUM_RPRODS,
     $     NDI_NUM_GRPS_CURR_PART, NDI_PN_ORDER_CURR_PART/

      data int_value_str /'Groups', 'Upscat', 'Downscat', 'ScatMat',
     $     'Edits', 'SecPart', 'NumTarget', 'NumRProds', 'PhGrps',
     $     'PhScatMat'/

      int_val_n_num = 4
      data int_val_n / NDI_RPROD_ALL,
     $     NDI_RPROD_RMO,
     $     NDI_RPROD_FEW, NDI_REAC_MT /

      data int_val_n_str /'NumProdAll', 'NumProdRMO',
     $     'NumProdFew', 'MT' /

      int_vec_num = 3
      data int_vec / NDI_RPROD_ALL, NDI_RPROD_RMO,
     $     NDI_RPROD_FEW /

      data int_vec_str / 'Rx--all', 'Rx--rmo',
     $     'Rx--few' /

      int_vec_n_num = 6
      data int_vec_n / NDI_RPROD_ID_ALL, NDI_RPROD_MLT_ALL,
     $     NDI_RPROD_ID_RMO, NDI_RPROD_MLT_RMO, NDI_RPROD_ID_FEW,
     $     NDI_RPROD_MLT_FEW /

      data int_vec_n_str / 'Rx id all', 'Rx mult all',
     $     'Rx id rmo', 'Rx mult rmo',
     $     'Rx id few', 'Rx mult few' /

      flt_val_num = 4
      data flt_value / NDI_TEMP, NDI_SIG_0, NDI_AT_WGT,
     $     NDI_AWR /

      data flt_val_str / 'Temp', 'Sig0', 'At wgt', 'AWR' /

      flt_val_n_num = 1
      data flt_val_n / NDI_REAC_Q /

      data flt_val_n_str / 'ReacQ' /

      flt_vec_num = 25
      data flt_vec / NDI_E_BOUNDS, NDI_WGTS, NDI_VEL,
     $     NDI_E_AVG, NDI_ENERGY_N, NDI_ENERGY_AVAIL,
     $     NDI_HEATING, NDI_KERMA, NDI_SIG_TOT, NDI_NU_TOT,
     $     NDI_NU_PR, NDI_NU_DEL, NDI_NU_SIG_F_TOT, NDI_NU_SIG_F_PR,
     $     NDI_NU_SIG_F_DEL, NDI_CHI_VEC_TOT, NDI_CHI_VEC_PR,
     $     NDI_CHI_VEC_DEL, NDI_CHI_MAT_TOT, NDI_CHI_MAT_PR, 
     $     NDI_CHI_MAT_DEL, NDI_NORM, NDI_FISS_Q, NDI_REAC_Q,
     $     NDI_E_BOUNDS_CURR_PART /

      data flt_vec_str /'Ebounds', 'Weights', 'Velocities','AvgE',
     $     'EnergyN', 'EAvail', 'Heating', 'Kerma', 'SigTot', 'NuTot',
     $     'Nupr', 'NuDel', 'NubarTotSigF','NubarPrSigF', 
     $      'NubarDelSigF', 'ChiVecTot', 'ChiVecTPr', 'ChiVecTDel',
     $     'ChiMatTot', 'ChiMatPr', 'ChiMatDel', 'Norm', 'FissQ', 
     $     'ReacQ', 'PhotonE'/

      flt_vec_n_num = 8
      data flt_vec_n / NDI_PN_DOWN_FULL, NDI_PN_FULL, 
     $     NDI_TRANSFER_MAT_TOT, NDI_TRANSFER_MAT_PR, NDI_CORR_DIAG,
     $     NDI_CORR_BHS, NDI_SIG_REAC, NDI_PN_PROD_FULL_CURR_PART  /

      data flt_vec_n_str /'Pn Scat (LD)', 'Pn Full', 'TransMatTot',
     $      'TransMatPr', 'DiagCorr', 'BHSCorr', 'Reaction XS',     
     $     'Ph scat (f)'/


      flt_vec_x_num = 1
      data flt_vec_x / NDI_TOT_PROD_XS/

      data flt_vec_x_str /'TotProdXS'/


      sec_part_int_val = 2
      sec_part_float_vec = 3
      sec_part_float_vec_n = 1 
      data sec_part / NDI_NUM_GRPS_CURR_PART, NDI_PN_ORDER_CURR_PART,
     $     NDI_E_BOUNDS_CURR_PART, NDI_HEATING_CURR_PART,
     $     NDI_KERMA_CURR_PART, NDI_PN_PROD_FULL_CURR_PART /

      data sec_part_str / 'NumGrps', 'PnOrder', 'Ebounds', 'PartHeat',
     $     'PartKerma', 'ProdFull' /

      Data zaid_list / '92235.600nm', '1001.600nm', '26056.600nm',
     $   "b10", "s", "o16", "k41", "f19" /
c     $   '27056.600nm', "u235", "u-238", "o-16", "al27"/
c  '92234.50', 'u-235' /


      call ndi_init(ierr)

c     Test error on initialization before close
      call ndi_init(ierr) 

      num_grp_str = '2_group'
      call ndi_set_option( NDI_LIB_TYPE, 'multigroup_neutron', ierr )

      print *, "lib_type=", ierr
      call ndi_set_option( NDI_LIBRARY, 'MENDF5', ierr)
      print *, "library=", ierr
      call ndi_set_option( NDI_MAT_ORDER, 'column', ierr)
      print *, "mat_order=", ierr

c      call ndi_set_option( NDI_COLLAPSE, num_grp_str, ierr) 

      do 100, zd_cnt = 1, 8

          if (zd_cnt.eq.8) then
              call ndi_set_option( NDI_LIBRARY, 'default', ierr)
          endif

c     Set new zaid
         zaid = zaid_list(zd_cnt)
         call ndi_set_isotope( zaid, ierr )
         print *, "Error setting isotope = ", ierr
         if (ierr.ne.0) then
            print *, "Error setting isotope ", zaid
            goto 101
         endif

         call ndi_set_option( NDI_CURR_PART, "photon", ierr )

c     Return string values
         call ndi_get_string_val(NDI_ZAID, tmp_zaid, ierr)
         print *, 'ZAID: ', tmp_zaid, '  err= ', ierr
         call ndi_get_string_val(NDI_INFO, info, ierr)
         print *, info
         write(*,2) 
 2       format( / )
c         call ndi_get_string_val(NDI_VERSION, vers, ierr)
c         print *, 'VERSION: ', vers
c         write(*,2)
         
c     Return integer values

         do 10 i = 1, int_val_num
            write(*,11) int_value_str(i)
 11         format(a12, ': ', $)
            call ndi_get_int_val(int_value(i), return_int, ierr)
            print *, return_int
 10      continue
         write(*,2) 
         
         
c     Return integer element from vector

         vec_num = 2
         do 20 i = 1, int_val_n_num
             write(*,21) int_val_n_str(i), vec_num
 21         format(a12, i2, ': ', $)
            call ndi_get_int_val_n(int_val_n(i), vec_num, return_int,
     $           ierr)
            print *, return_int
 20      continue
         write(*,2) 
         
         
c     Return float value

         do 50, i = 1, flt_val_num
            write(*, 51) flt_val_str(i)
 51         format(a12, ': ', $)
            call ndi_get_float64_val(flt_value(i), return_flt, ierr)
            print *, return_flt
 50      continue
         write(*,2) 
         
         
c     Return float element from vector

         vec_num = 2
         do 53, i = 1, flt_val_n_num
            write(*,52) flt_val_n_str(i), vec_num
 52         format(a12, i3, ':',$)
            call ndi_get_float64_val_n( flt_val_n(i), vec_num,
     $            return_flt, ierr)
            print *, return_flt
 53      continue
         write(*,2) 
         
         
c     Return integer vector

         do 30 i = 1, int_val_n_num
            write(*, 32) int_val_n_str(i)
 32         format(a12, ': ')
            call ndi_get_size(int_vec(i), num_element, ierr)
            call ndi_get_int_vec(int_vec(i), return_int_vec, 20, ierr)
            print 31, (return_int_vec(j), j=1,num_element)
   31       format(10(i3, 2x))
 30      continue
         write(*,2)
         
         
         
c     Return integer vector from matrix

         vec_num = 2
         do 43 i = 1, int_vec_n_num
            write(*, 45) int_vec_n_str(i), vec_num
 45         format(a12, i2, ': ')
            call ndi_get_size_n(int_vec_n(i), vec_num, num_element,
     $           ierr)
            call ndi_get_int_vec_n(int_vec_n(i), vec_num, 
     $           return_int_vec, 20, ierr)
            print 44, (return_int_vec(j), j=1,num_element)
 44         format(10(i6, 3x))
 43      continue
         write(*,2) 
         
         
c     Return float vector

         do 60 i = 1, flt_vec_num
            write(*, 62) flt_vec_str(i)
   62        format(a12, ': ')
c           TEST MATRIX ORDER OPTION
            if ( (i.eq.11).and.(zd_cnt.eq.5)) then
                print *, 'COLUMNWISE'
                call ndi_set_option( NDI_MAT_ORDER, 'column', ierr)
            endif
            call ndi_get_size(flt_vec(i), num_element, ierr)
            call ndi_get_float64_vec(flt_vec(i), return_flt_vec,
     $           900, ierr)
            print 61, (return_flt_vec(j), j = 1, num_element)
   61        format(6(1pE12.5, 2x))
            if ( (i.eq.11).and.(zd_cnt.eq.5)) then
                call ndi_set_option( NDI_MAT_ORDER, 'row', ierr)
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
 71         format(a12, i2,': ')
            call ndi_get_size_n(flt_vec_n(i), vec_num, num_element,
     $           ierr)
            call ndi_get_float64_vec_n(flt_vec_n(i), vec_num,
     $           return_flt_vec,  900, ierr)
            print 72, (return_flt_vec(j), j = 1, num_element)
 72         format(6(1pE12.5, 2x))
 70      continue
         write(*,2) 
         
 101     continue

 100  continue
      
      call ndi_close(ierr)

c     Test error on closing twice
      call ndi_close(ierr)
      end





