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

      data text /0.0, 2.58e-8, 100.0 /

      print *, "Open gendir:"
      call ndi2_open_gendir(gendir, "", ierr)
      if (ierr .eq. 0) then
         print *, "ok."
      else
         print *, "  **Error = ", ierr
      endif

      print *, "Make handle:"
      call ndi2_make_handle(gendir, "multigroup_neutron", 
     $     handle, ierr)
      if (ierr .eq. 0) then
         print *, "ok."
      else
         print *, "  **Error = ", ierr
      endif

      print *, "Set library:"
      call ndi2_set_option(handle,  NDI_LIBRARY, 'MENDF5', ierr)
      if (ierr .eq. 0) then
         print *, "ok."
      else
         print *, "  **Error =", ierr
      endif

      print *, "Set isotope:"
      call ndi2_set_zaid(handle, "92235", ierr )
      if (ierr .eq. 0) then
         print *, "ok."
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

      print *, "Get temperature:"
      call ndi2_get_float64_val(handle, NDI_TEMP, temp, ierr)
      if (ierr .eq. 0) then
         print *, "temperature = ", temp
      else
         print *, "  **Error =", ierr
      endif

      print *, "Get temperature array size:"
      call ndi2_get_size(handle, NDI_TEMPS, size, ierr)
      if (ierr .eq. 0) then
         print *, "Number of temps = ", size
      else
         print *, "  **Error = ", ierr
      endif

      print *, "Get temperature array:"
      call ndi2_get_float64_vec(handle, NDI_TEMPS, tarr, ACTUAL, ierr)
      if (ierr .eq. 0) then
         do i = 1, size
            print *, "Temperature(", i, ") = ", tarr(i)
         enddo
      else
         print *, "  **Error =", ierr
      endif

      do i = 1, size

         print *, "Attempt to set temperature = ", tarr(i)
         call ndi2_set_zaid_float64_temp(handle, "92235", tarr(i),
     $        ierr)
         if (ierr .eq. 0) then
            print *, "ok."
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
            print *, "ok."
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
      if (ierr .eq. 0) then
         print *, "ok."
      else
         print *, "  **Error = ", ierr
      endif

      end
