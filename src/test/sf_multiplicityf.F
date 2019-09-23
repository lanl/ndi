c     $Id$
c     Test multi-temperature functionality (Requirement 22)

#include "ndi_keywords.h"

      program main

      integer MAX_SF_PNU2, ZAIDS, NUM_GRPS
      parameter (MAX_SF_PNU=11, ZAIDS=24, NUM_GRPS=12)

      integer gendir
      integer ierr
      integer handle
      character*(ZAIDS) zaid
      real*8 temp
      real*8 sf_rate
      integer size
      real*8 sf_spectra(NUM_GRPS)
      real*8 sf_mult(NUM_GRPS)

      integer i
      integer n
      integer g

      print *, "Open gendir:"
      call ndi2_open_gendir(gendir, "", ierr)
      call check_err(ierr)

      print *, "Make handle:"
      call ndi2_make_handle(gendir, "multigroup_neutron",
     $     handle, ierr)
      call check_err(ierr)

      print *, "Set library:"
      call ndi2_set_option(handle,  NDI_LIBRARY, "mendf6", ierr)
      call check_err(ierr)

      print *, "Set group structure"
      call ndi2_set_option(handle, NDI_COLLAPSE, "12_lanl", ierr)
      call check_err(ierr)

      print *, "Set isotope:"
      call ndi2_set_zaid(handle, "94240", ierr )
      call check_err(ierr)

      print *, "Get zaid:"
      call ndi2_get_string_val(handle, NDI_ZAID, zaid, ierr)
      if (ierr .eq. 0) then
         print *, "zaid = ", zaid
      else
         print *, "  **Error =", ierr
      endif

      print *, "Get number of groups:"
      call ndi2_get_int_val(handle, NDI_NUM_GRPS, n, ierr)
      if (ierr .eq. 0) then
         print *, "igm = ", n
      else
         print *, "  **Error =", ierr
      endif

      print *, "Get spontaneous fission rate:"
      call ndi2_get_float64_val(handle, NDI_SF_RATE, sf_rate, ierr)
      if (ierr .eq. 0) then
         print *, "SF rate = ", sf_rate
      else
         print *, "  **Error =", ierr
      endif

      print *, "Get spontaneous fission spectrum:"
      call ndi2_get_float64_vec(handle, NDI_SF_SPECTRA, sf_spectra, 
     &  num_grps, ierr)
      if (ierr .eq. 0) then
         do g = 1, n
           print *, "SF spectra (",g,") = ", sf_spectra(g)
         end do
      else
         print *, "  **Error =", ierr
      endif

      print *, "Get spontaneous fission multiplicities:"
      do i = 0, MAX_SF_PNU-1
        print *, "Multiplicity = ",i
        call ndi2_get_float64_vec_n(handle, NDI_SF_FMULT, i, sf_mult, n,
     &    ierr);
        if (ierr .eq. 0) then
           do g = 1, n
             print *, "Pi (",g,") = ", sf_mult(g)
           end do
        else
           print *, "  **Error =", ierr
        endif
      end do

      print *, "Close gendir:"
      call ndi2_close_gendir(gendir, ierr)
      call check_err(ierr)

      end

      subroutine check_err(ierr)

      integer ierr

      if (ierr .eq. 0) then
         print *, "ok."
      else
         print *, "  **Error = ", ierr
      endif
      end

