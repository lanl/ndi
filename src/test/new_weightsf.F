!     Test Fortran functionality of setting a new weight function.
!     See new_weights.c for a more detailed test

      program main

      integer ZAIDS
      parameter (ZAIDS=24)
      
      integer gendir
      integer ierr
      integer handle
      integer num_grps
      character*(ZAIDS) zaid
      real*8 temp
      real*8 sig0
      real*8, allocatable :: weights(:), sig_tot(:)

      print *, "Open gendir:"
      call ndi2_open_gendir(gendir, " ", ierr)
      call check_err(ierr)

      print *, "Make handle multigroup_neutron:"
      call ndi2_make_handle(gendir, "multigroup_neutron", handle, ierr)
c     &     handle, ierr)
      call check_err(ierr)

      print *, "Set library weights_test:"
      call ndi2_set_option(handle,  44, 'weights_test', ierr)
      call check_err(ierr)

      print *, "Set isotope 92236.901nm:"
      call ndi2_set_zaid(handle, "92236", ierr )
      call check_err(ierr)

      print *, "Get zaid:"
      call ndi2_get_string_val(handle, 1, zaid, ierr)
      call check_err(ierr)
      if (ierr .eq. 0) then
         print *, "  zaid = ", zaid
      endif

      print *, "Get number of groups:"
      call ndi2_get_int_val(handle, 3, num_grps, ierr)
      call check_err(ierr)
      print *, "  number of groups = ", num_grps

      print *, "Set number of groups to collapse to:"
      call ndi2_set_option(handle, 46, '2_lanl', ierr)
      call check_err(ierr)

      print *, "Get new number of groups:"
      call ndi2_get_int_val(handle, 3, num_grps_new, ierr)
      call check_err(ierr)
      print *, "  number of groups = ", num_grps_new

      allocate(sig_tot(num_grps_new))

      print *, "Get the total cross section:"
      call ndi2_get_float64_vec(handle, 13, sig_tot, num_grps_new, ierr)
      call check_err(ierr)
      print *, "  sig_tot = ", sig_tot

      ! Create the new weight function
      allocate(weights(num_grps))
      do i = 1, num_grps
        weights(i) = float(i)
      end do

      print *, "Set new weight function"
      call ndi2_set_float64_vec_option(handle, 11, weights, num_grps,
     & ierr )
      call check_err(ierr)

      print *, "Get the new total cross section:"
      call ndi2_get_float64_vec(handle, 13, sig_tot, num_grps_new, ierr)
      call check_err(ierr)
      ! Should be 7.3163 and 15.2016
      print *, "  sig_tot = ", sig_tot

      print *, "Close gendir:"
      call ndi2_close_gendir(gendir, ierr)
      call check_err(ierr)

      end

      subroutine check_err(ierr)
      
      integer ierr

      if (ierr .eq. 0) then
         print *, " ok."
      else
         print *, "  **Error = ", ierr
      endif
      end

