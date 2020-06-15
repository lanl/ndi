c     $Id$
c     Test multi-temperature functionality (Requirement 22)

#include "ndi_keywords.h"

      program main

      integer EXTREME, ACTUAL, ZAIDS
      parameter (EXTREME=3, ACTUAL=20, ZAIDS=24)
      
      integer gendir
      integer ierr
      integer handle
      character*(ZAIDS) zaid
      real*8 temp
      integer size
      real*8 tarr(ACTUAL)
      real*8 text(EXTREME)

      integer mp
      integer mpn
      integer mpg

      data text /0.0, 2.58e-8, 100.0 /

      print *, "Open gendir:"
      call ndi2_open_gendir(gendir, " ", ierr)
      call check_err(ierr)

      print *, "Make handle:"
      call ndi2_make_handle(gendir, "multigroup_neutron", 
     $     handle, ierr)
      call check_err(ierr)

      print *, "Set library:"
      call ndi2_set_option(handle,  NDI_LIBRARY, 'MULTI_TEMP', ierr)
      call check_err(ierr)

      print *, "Set isotope:"
      call ndi2_set_zaid(handle, "92235", ierr )
      call check_err(ierr)

      print *, "Get zaid:"
      call ndi2_get_string_val(handle, NDI_ZAID, zaid, ierr)
      if (ierr .eq. 0) then
         print *, "zaid = ", zaid
      else
         print *, "  **Error =", ierr
      endif

      print *, "Get temperature:"
      call ndi2_get_float64_val(handle, NDI_TEMP, temp, ierr)
      if (ierr .eq. 0) then
         print *, "temperature = ", temp
      else
         print *, "  **Error =", ierr
      endif

      print *, "Create multigroup multiparticle"
      call ndi2_make_handle(gendir, "multigroup_multi", mp, ierr)
      call check_err(ierr)

      print *, "Initialize neutron"
      call ndi2_make_handle(mp, "multigroup_neutron", mpn, ierr)
      call check_err(ierr)

      print *, "Set neutron library"
      call ndi2_set_option(mpn, NDI_LIBRARY, "multi_temp", ierr)
      call check_err(ierr)
      
      print *, "Initialize gamma"
      call ndi2_make_handle(mp, "multigroup_photon", mpg, ierr)
      call check_err(ierr)

      print *, "Set gamma library"
      call ndi2_set_option(mpg, NDI_LIBRARY, "mendf6g", ierr)
      call check_err(ierr)

c     Test multitemperature multiparticle (Reqt. 75)

      print *, "Set multi-particle isotope"
      call ndi2_set_zaid(mp, "92235", ierr)
      call check_err(ierr)

      print *, "Get multi-particle temperature array size:"
      call ndi2_get_size(mp, NDI_TEMPS, size, ierr)
      if (ierr .eq. 0) then
         print *, "Number of temps = ", size
      else
         print *, "  **Error = ", ierr
      endif

      print *, "Get multi-particle temperature array:"
      call ndi2_get_float64_vec(mp, NDI_TEMPS, tarr, ACTUAL, ierr)
      if (ierr .eq. 0) then
         do i = 1, size
            print *, "Temperature( ", i, " ) = ", tarr(i)
         enddo
      else
         print *, "  **Error =", ierr
      endif

      do i = 1, size

         print *, "Attempt to set temperature = ", tarr(i)
         call ndi2_set_zaid_float64_temp(handle, "92235", tarr(i),
     $        ierr)
         if (ierr .eq. 0) then
            print *, "ok setting 92235."
         else
            print *, "  **Error =", ierr
         endif

         print *, "Get zaid:"
         call ndi2_get_string_val(handle, NDI_ZAID, zaid, ierr)
         if (ierr .eq. 0) then
            print *, "zaid = ", zaid
         else
            print *, "  **Error =", ierr
         endif

c     Test multitemperature multiparticle (Reqt. 75)

         print *, "Attempt to set temperature = ", tarr(i)
         call ndi2_set_zaid_float64_temp(mp, "92235", tarr(i),
     $        ierr)
         if (ierr .eq. 0) then
            print *, "ok setting 92235."
         else
            print *, "  **Error =", ierr
         endif

         print *, "Get neutron zaid:"
         call ndi2_get_string_val(mpn, NDI_ZAID, zaid, ierr)
         if (ierr .eq. 0) then
            print *, "neutron zaid = ", zaid
         else
            print *, "  **Error =", ierr
         endif

         print *, "Get gamma zaid:"
         call ndi2_get_string_val(mpg, NDI_ZAID, zaid, ierr)
         if (ierr .eq. 0) then
            print *, "gamma zaid = ", zaid
         else
            print *, "  **Error =", ierr
         endif

c     Test zaid translation (Reqt. 60)
         call ndi2_set_zaid_float64_temp(handle, "U-235", tarr(i),
     $        ierr)
         if (ierr .eq. 0) then
            print *, "ok setting U-235."
         else
            print *, "  **Error = testing zaid translation", ierr
         endif

         print *, "Get zaid:"
         call ndi2_get_string_val(handle, NDI_ZAID, zaid, ierr)
         if (ierr .eq. 0) then
            print *, "zaid = ", zaid
         else
            print *, "  **Error =", ierr
         endif

         print *, "Actual temperature:"
         call ndi2_get_float64_val(handle, NDI_TEMP, temp, ierr)
         if (ierr .eq. 0) then
            print *, "temperature = ", temp
         else
            print *, "  **Error =", ierr
         endif

      enddo

      do i = 1, EXTREME

         print *, "Attempt to set temperature = ", text(i)
         call ndi2_set_zaid_float64_temp(handle, "92235", text(i),
     $        ierr)
         if (ierr .eq. 0) then
            print *, "ok setting zaid 92235."
         else
            print *, "  **Error =", ierr
         endif

         print *, "Get zaid:"
         call ndi2_get_string_val(handle, NDI_ZAID, zaid, ierr)
         if (ierr .eq. 0) then
            print *, "zaid = ", zaid
         else
            print *, "  **Error =", ierr
         endif

c     Test zaid translation (Reqt. 60)
         call ndi2_set_zaid_float64_temp(handle, "u235", text(i),
     $        ierr)
         if (ierr .eq. 0) then
            print *, "ok setting zaid u235."
         else
            print *, "  **Error testing zaid translation =", ierr
         endif

         print *, "Get zaid:"
         call ndi2_get_string_val(handle, NDI_ZAID, zaid, ierr)
         if (ierr .eq. 0) then
            print *, "zaid = ", zaid
         else
            print *, "  **Error =", ierr
         endif

         print *, "Actual temperature:"
         call ndi2_get_float64_val(handle, NDI_TEMP, temp, ierr)
         if (ierr .eq. 0) then
            print *, "temperature = ", temp
         else
            print *, "  **Error =", ierr
         endif

      enddo
      
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
