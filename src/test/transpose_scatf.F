#include "ndi_keywords.h"

      program main

      integer MAXVEC, NUMPG
      parameter (MAXVEC=70000, NUMPG=2)

      integer ierr
      integer gendir, gendir2
      integer single_part_handle, single_part_handle2
      integer mpmg_handle, mpmg_handle2
      integer vec_size
      integer neutron_h, photon_h
      integer pgrps
      real*8 scat_trans(MAXVEC)
      real*8 scat_no_trans(MAXVEC)
      real*8 pbounds(NUMPG + 1)
      logical xpose
      integer num_grps

      pgrps = NUMPG
      data pbounds  / 20.0, 4.0, 0.1 /

      write(*,*) "**  TRANSPOSE SET IN DATA TABLE  **"
      write(*,*) " "
      call ndi2_open_gendir(gendir, "", ierr)
      call check_err(ierr, "opening gendir")

c     Single particle test
      write(*,*) " "
      write(*,*)  "*************** Single Particle Test ***************"
      write(*,*) " "
      call ndi2_make_handle(gendir, "multigroup_neutron", 
     $     single_part_handle, ierr)
      call check_err(ierr, "making single_particle_handle")

      call check_err(ierr, "setting multigroup neutron")
      
      call ndi2_set_option(single_part_handle, NDI_LIBRARY, "mendf6",
     $     ierr)
      call check_err(ierr, "setting mendf6 library")
      call ndi2_set_option(single_part_handle, NDI_COLLAPSE, 
     $     "6_lanl", ierr)
      call check_err(ierr, "setting 6_lanl group structure")
      call ndi2_set_zaid(single_part_handle, "92235", ierr)
      call check_err(ierr, "setting isotope 92235")

      call ndi2_get_size_n(single_part_handle, NDI_PN_FULL, 0,
     $     vec_size, ierr)
      call check_err(ierr, "getting pn full size")
      call ndi2_get_float64_vec_n(single_part_handle, NDI_PN_FULL, 0,
     $     scat_no_trans, MAXVEC, ierr)
      call check_err(ierr, "getting pn full")
      write(*,*) "  No transpose:"
      print 10, (scat_no_trans(i), i = 1, vec_size)
 10   format(6(1pE12.5, 2x))

      call ndi2_set_option(single_part_handle, NDI_MAT_ORDER,
     $     'transpose', ierr)
      call check_err(ierr, "setting mat order transpose")
      call ndi2_get_float64_vec_n(single_part_handle, NDI_PN_FULL, 0,
     $     scat_trans, MAXVEC, ierr)
      call check_err(ierr, "getting transposed pn full")
      write(*,*) " "
      write(*,*) " "
      write(*,*) "  Transpose:"
      print 10, (scat_trans(i), i = 1, vec_size)

c     Does matrix == (matrix')'?
      call ndi2_get_int_val(single_part_handle, NDI_NUM_GRPS,
     $     num_grps, ierr)
      call check_err(ierr, "getting number of groups")
      print *, "matrix == (matrix')' = ", xpose(scat_no_trans, num_grps,
     $     num_grps, scat_trans)

c     Fission transfer matrix (Reqt. 63)
      call ndi2_set_option(single_part_handle, NDI_MAT_ORDER,
     $     'default', ierr)
      call check_err(ierr, "setting mat order transpose")
      call ndi2_get_float64_vec(single_part_handle,
     $      NDI_FISS_XFER_MAT_PR, scat_no_trans, MAXVEC, ierr)
      call check_err(ierr, "getting fiss xfer mat")
      write(*,*) "  No transpose:"
      print 10, (scat_no_trans(i), i = 1, vec_size)

      call ndi2_set_option(single_part_handle, NDI_MAT_ORDER,
     $     'transpose', ierr)
      call check_err(ierr, "setting mat order transpose")
      call ndi2_get_float64_vec(single_part_handle, 
     $     NDI_FISS_XFER_MAT_PR, scat_trans, MAXVEC, ierr)
      call check_err(ierr, "getting transposed fiss xfer mat")
      write(*,*) " "
      write(*,*) " "
      write(*,*) "  Transpose:"
      print 10, (scat_trans(i), i = 1, vec_size)

c     Does matrix == (matrix')'?
      call ndi2_get_int_val(single_part_handle, NDI_NUM_GRPS,
     $     num_grps, ierr)
      call check_err(ierr, "getting number of groups")
      print *, "matrix == (matrix')' = ", xpose(scat_no_trans, num_grps,
     $     num_grps, scat_trans)
      
c     Multiparticle test
      write(*,*) " "
      write(*,*) " "
      write(*,*)  "*************** Multiparticle Test ***************"
      write(*,*) " "
      call ndi2_make_handle(gendir, "multigroup_multi", 
     $     mpmg_handle, ierr)
      call check_err(ierr, "making mpmg handle")
     
      call ndi2_set_option(mpmg_handle, NDI_LIBRARY, "ngk6",
     $     ierr)
      call check_err(ierr, "setting ngk6 library")
      call ndi2_set_zaid(mpmg_handle, "92235", ierr)
      call check_err(ierr, "setting zaid 92235")
      call ndi2_set_option(mpmg_handle, NDI_MAT_ORDER,
     $     "default", ierr)
      call check_err(ierr, "setting mat order default")

c     Set group structures
      call ndi2_get_int_val_n(mpmg_handle, NDI_PARTICLE_N, 0,
     $     neutron_h, ierr)
      call check_err(ierr, "getting neutron handle")
      call ndi2_get_int_val_n(mpmg_handle, NDI_PARTICLE_N, 1,
     $     photon_h, ierr)
      call check_err(ierr, "getting photon handle")
      call ndi2_set_option(neutron_h, NDI_COLLAPSE, "4_lanl",
     $     ierr)
      call check_err(ierr, "setting neutron group structure")
      call ndi2_set_float64_vec_option(photon_h, NDI_COLLAPSE,
     $     pbounds, pgrps + 1, ierr)
      call check_err(ierr, "getting photon group structure")

c     Get data
      call ndi2_get_size_n(mpmg_handle, NDI_PN_FULL, 0,
     $     vec_size, ierr)
      call check_err(ierr, "getting pn full size")
      call ndi2_get_float64_vec_n(mpmg_handle, NDI_PN_FULL, 0,
     $     scat_no_trans, MAXVEC, ierr)
      call check_err(ierr, "getting pn full")
      write(*,*) "  No transpose:"
      print 10, (scat_no_trans(i), i = 1, vec_size)

      call ndi2_set_option(mpmg_handle, NDI_MAT_ORDER,
     $     "transpose", ierr)
      call check_err(ierr, "setting mat order transpose")
      call ndi2_get_float64_vec_n(mpmg_handle, NDI_PN_FULL, 0,
     $     scat_trans, MAXVEC, ierr)
      call check_err(ierr, "setting pn full")
      write(*,*) " "
      write(*,*) " "
      write(*,*) "  Transpose:"
      print 10, (scat_trans(i), i = 1, vec_size)

c     Does matrix == (matrix')'?
      call ndi2_get_int_val(mpmg_handle, NDI_NUM_GRPS,
     $     num_grps, ierr)
      call check_err(ierr, "getting number of groups")
      print *, "matrix == (matrix')' = ", xpose(scat_no_trans, num_grps,
     $     num_grps, scat_trans)

      call ndi2_close_gendir(gendir, ierr)
      call check_err(ierr, "closing gendir")


c     ************ SET TRANSPOSE IN GENDIR ************
      write(*,*) " "
      write(*,*) "**  TRANSPOSE SET IN GENDIR  **"
      write(*,*) " "

      call ndi2_open_gendir(gendir2, "", ierr)
      call check_err(ierr, "opening gendir2")

c     Single particle test
      write(*,*) " "
      write(*,*)  "*************** Single Particle Test ***************"
      write(*,*) " "
      call ndi2_make_handle(gendir2, "multigroup_neutron", 
     $     single_part_handle, ierr)
      call check_err(ierr, "making single_particle_handle")

      call ndi2_set_option(single_part_handle, NDI_LIBRARY, "mendf6",
     $     ierr)
      call check_err(ierr, "setting mendf6 library")
      call ndi2_set_option(single_part_handle, NDI_COLLAPSE, 
     $     "6_lanl", ierr)
      call check_err(ierr, "setting 6_lanl group structure")
      call ndi2_set_zaid(single_part_handle, "92235", ierr)
      call check_err(ierr, "setting isotope 92235")

      call ndi2_get_size_n(single_part_handle, NDI_PN_FULL, 0,
     $     vec_size, ierr)
      call check_err(ierr, "getting pn full size")
      call ndi2_get_float64_vec_n(single_part_handle, NDI_PN_FULL, 0,
     $     scat_no_trans, MAXVEC, ierr)
      call check_err(ierr, "getting pn full")
      write(*,*) "  No transpose:"
      print 10, (scat_no_trans(i), i = 1, vec_size)


c     Get another data handle after setting transpose
      call ndi2_set_option_gendir(gendir2, NDI_MAT_ORDER_DEFAULT,
     $     'transpose', ierr)
      call check_err(ierr, "setting default mat order")
      call ndi2_make_handle(gendir2, "multigroup_neutron", 
     $     single_part_handle2, ierr)
      call check_err(ierr, "making single_part_handle2")
      
      call ndi2_set_option(single_part_handle2, NDI_LIBRARY, "mendf6",
     $     ierr)
      call check_err(ierr, "setting mendf6 library")
      call ndi2_set_option(single_part_handle2, NDI_COLLAPSE, 
     $     "6_lanl", ierr)
      call check_err(ierr, "setting 6_lanl group structure")
      call ndi2_set_zaid(single_part_handle2, "92235", ierr)
      call check_err(ierr, "setting isotope 92235")

      call ndi2_get_float64_vec_n(single_part_handle2, NDI_PN_FULL, 0,
     $     scat_trans, MAXVEC, ierr)
      call check_err(ierr, "getting transposed pn full")
      write(*,*) " "
      write(*,*) " "
      write(*,*) "  Transpose:"
      print 10, (scat_trans(i), i = 1, vec_size)
      
      call ndi2_get_int_val(single_part_handle2, NDI_NUM_GRPS,
     $     num_grps, ierr)
      call check_err(ierr, "getting number of groups")
      print *, "matrix == (matrix')' = ", xpose(scat_no_trans, num_grps,
     $     num_grps, scat_trans)

c     Multiparticle test
      write(*,*) " "
      write(*,*) " "
      write(*,*)  "*************** Multiparticle Test ***************"
      write(*,*) " "
      call ndi2_make_handle(gendir2, "multigroup_multi", 
     $     mpmg_handle, ierr)
      call check_err(ierr, "making mpmg handle")

c     Set options
      call ndi2_set_option(mpmg_handle, NDI_LIBRARY, "ngk6",
     $     ierr)
      call check_err(ierr, "setting ngk6 library")
      call ndi2_set_zaid(mpmg_handle, "92235", ierr)
      call check_err(ierr, "setting zaid 92235")
      call ndi2_set_option(mpmg_handle, NDI_MAT_ORDER,
     $     "default", ierr)
      call check_err(ierr, "setting mat order default")

c     Set group structures
      call ndi2_get_int_val_n(mpmg_handle, NDI_PARTICLE_N, 0,
     $     neutron_h, ierr)
      call check_err(ierr, "getting neutron handle")
      call ndi2_get_int_val_n(mpmg_handle, NDI_PARTICLE_N, 1,
     $     photon_h, ierr)
      call check_err(ierr, "getting photon handle")
      call ndi2_set_option(neutron_h, NDI_COLLAPSE, "4_lanl",
     $     ierr)
      call check_err(ierr, "setting neutron group structure")
      call ndi2_set_float64_vec_option(photon_h, NDI_COLLAPSE,
     $     pbounds, pgrps + 1, ierr)
      call check_err(ierr, "getting photon group structure")

c     Get data
      call ndi2_get_size_n(mpmg_handle, NDI_PN_FULL, 0,
     $     vec_size, ierr)
      call check_err(ierr, "getting pn full size")
      call ndi2_get_float64_vec_n(mpmg_handle, NDI_PN_FULL, 0,
     $     scat_no_trans, MAXVEC, ierr)
      call check_err(ierr, "getting pn full")
      write(*,*) "  No transpose:"
      print 10, (scat_no_trans(i), i = 1, vec_size)

c     Get another data handle after setting transpose
      call ndi2_set_option_gendir(gendir2, NDI_MAT_ORDER_DEFAULT,
     $     'transpose', ierr)
      call check_err(ierr, "setting default mat order transpose")
      call ndi2_make_handle(gendir2, "multigroup_multi", 
     $     mpmg_handle2, ierr)
      call check_err(ierr, "getting mpmg_handle2")

      call ndi2_set_option(mpmg_handle2, NDI_LIBRARY, "ngk6",
     $     ierr)
      call check_err(ierr, "setting ngk6 library")
      call ndi2_set_zaid(mpmg_handle2, '92235', ierr)
      call check_err(ierr, "setting zaid 92235")

c     Set group structures
      call ndi2_get_int_val_n(mpmg_handle2, NDI_PARTICLE_N, 0,
     $     neutron_h, ierr)
      call check_err(ierr, "getting neutron handle")
      call ndi2_get_int_val_n(mpmg_handle2, NDI_PARTICLE_N, 1,
     $     photon_h, ierr)
      call check_err(ierr, "getting photon handle")
      call ndi2_set_option(neutron_h, NDI_COLLAPSE, "4_lanl",
     $     ierr)
      call check_err(ierr, "setting neutron group structure")
      call ndi2_set_float64_vec_option(photon_h, NDI_COLLAPSE,
     $     pbounds, pgrps + 1, ierr)
      call check_err(ierr, "getting photon group structure")

c     Get data
      call ndi2_get_size_n(mpmg_handle2, NDI_PN_FULL, 0,
     $     vec_size, ierr)
      call check_err(ierr, "getting transposed pn full size")
      call ndi2_get_float64_vec_n(mpmg_handle2, NDI_PN_FULL, 0,
     $     scat_trans, MAXVEC, ierr)
      call check_err(ierr, "setting pn full")
      write(*,*) " "
      write(*,*) " "
      write(*,*) "  Transpose:"
      print 10, (scat_trans(i), i = 1, vec_size)

c     Does matrix == (matrix')'?
      call ndi2_get_int_val(mpmg_handle2, NDI_NUM_GRPS,
     $     num_grps, ierr)
      call check_err(ierr, "getting number of groups")
      print *, "matrix == (matrix')' = ", xpose(scat_no_trans, num_grps,
     $     num_grps, scat_trans)

      call ndi2_close_gendir(gendir2, ierr)
      call check_err(ierr, "closing gendir2")

      end

      subroutine check_err(ierr, msg)

      integer ierr
      character*(*) msg

      if (ierr .lt. 0) then
         write(*, 20) ierr, msg
 20      format("NDI ERROR ", i4, ":  ", a50)
      else if (ierr .gt. 0) then
         write(*, 30) ierr, msg
 30      format("NDI WARNING ", i4, ":  ", a50)
      endif

      return
      end

      function xpose(mat, row_num, col_num, mat_t)
      real*8 mat(*)
      integer row_num
      integer col_num
      real*8 mat_t(*)
      logical xpose

      integer row, col
      
      xpose = .true.
      do row = 0, row_num-1
         do col = 0, col_num-1
            if (mat(row*col_num+col+1) .ne.
     $           mat_t(col*row_num+row+1)) then
               xpose = .false.
               goto 10
            end if
         end do
      end do

 10   continue
      return 
      end
