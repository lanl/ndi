c     $Id$
c     Test multi-temperature/mult-sigma0 functionality
c       ndi2_set_zaid_float64_temp_sig_0

      program main

      integer ZAIDS
      parameter (ZAIDS=24)
      
      integer gendir
      integer ierr
      integer handle
      character*(ZAIDS) zaid
      real*8 temp
      real*8 sig0

      print *, "Open gendir:"
      call ndi2_open_gendir(gendir, " ", ierr)
      call check_err(ierr)

      print *, "Make handle:"
      call ndi2_make_handle(gendir, "multigroup_neutron", 
     $     handle, ierr)
      call check_err(ierr)

      print *, "Set library:"
      call ndi2_set_option(handle,  44, 'SS_TEST', ierr)
      call check_err(ierr)

      print *, "Set isotope:"
      call ndi2_set_zaid(handle, "92235", ierr )
      call check_err(ierr)

      print *, "Get zaid:"
      call ndi2_get_string_val(handle, 1, zaid, ierr)
      if (ierr .eq. 0) then
         print *, "zaid = ", zaid
      else
         print *, "  **Error =", ierr
      endif

      print *, "Get temperature:"
      call ndi2_get_float64_val(handle, 6, temp, ierr)
      if (ierr .eq. 0) then
         print *, "temperature = ", temp
      else
         print *, "  **Error =", ierr
      endif

      print *, "Get sig0:"
      call ndi2_get_float64_val(handle, 7, sig0, ierr)
      if (ierr .eq. 0) then
         print *, "sig0 = ", sig0
      else
         print *, "  **Error =", ierr
      endif


c Test with T=300K, sig0=0.1 (ZAID is on gendir)
      temp=2.5852155e-08
      sig0=0.1
      print*," "
      print *, "Set T=2.5852155e-08 MeV, sig0=0.1."
      print *, "  In gendir, 92235.101nm"
      call ndi2_set_zaid_float64_temp_sig_0(handle, 
     & "92235", temp, sig0, ierr)
      call check_err(ierr)

      print *, "Get zaid, temperature, and sig0:"
      call ndi2_get_string_val(handle, 1, zaid, ierr)
      if (ierr /= 0 .or. trim(zaid) /= '92235.101nm') then
         print *, "  **ZAID Error =", ierr
         print *, "  **ZAID =", zaid
      endif
      call ndi2_get_float64_val(handle, 6, temp, ierr)
      if (ierr /= 0) then
         print *, "  **TEMP Error =", ierr
      endif
      call ndi2_get_float64_val(handle, 7, sig0, ierr)
      if (ierr /= 0) then
         print *, "  **SIG0 Error =", ierr
      endif

      print 10000, "zaid = ",zaid,"temperature = ",temp,"sig0 =",sig0

c Test with T=300K, sig0=10.0 (ZAID is on gendir)
      print*," "
      print *, "Set T=2.5852155e-08 MeV, sig0=10.0."
      print *, "  In gendir, 92235.001nm"
      sig0=10.0
      call ndi2_set_zaid_float64_temp_sig_0(handle,
     & "92235", temp, sig0, ierr)
      call check_err(ierr)

      print *, "Get zaid, temperature, and sig0:"
      call ndi2_get_string_val(handle, 1, zaid, ierr)
      if (ierr /= 0 .or. trim(zaid) /= '92235.001nm') then
         print *, "  **ZAID Error =", ierr
         print *, "  **ZAID =", zaid
      endif
      call ndi2_get_float64_val(handle, 6, temp, ierr)
      if (ierr /= 0) then
         print *, "  **TEMP Error =", ierr
      endif
      call ndi2_get_float64_val(handle, 7, sig0, ierr)
      if (ierr /= 0) then
         print *, "  **SIG0 Error =", ierr
      endif

      print 10000, "zaid = ",zaid,"temperature = ",temp,"sig0 =",sig0

      print*," "
      print*,"Set T=2.5852155e-08 MeV, sig0=1.0. "
      print *, "  Not in gendir. Closest: 92235.101nm"
      sig0=1.0
      call ndi2_set_zaid_float64_temp_sig_0(handle,
     & "92235", temp, sig0, ierr)
      call check_noerr(ierr)

      print *, "Get zaid, temperature, and sig0:"
      call ndi2_get_string_val(handle, 1, zaid, ierr)
      if (ierr /= 0 .or. trim(zaid) /= '92235.101nm') then
         print *, "  **ZAID Error =", ierr
         print *, "  **ZAID =", zaid
      endif
      call ndi2_get_float64_val(handle, 6, temp, ierr)
      if (ierr /= 0) then
         print *, "  **TEMP Error =", ierr
      endif
      call ndi2_get_float64_val(handle, 7, sig0, ierr)
      if (ierr /= 0) then
         print *, "  **SIG0 Error =", ierr
      endif

      print 10000, "zaid = ",zaid,"temperature = ",temp,"sig0 =",sig0

      print*," "
      print*,"Set T=4.3086923E-8 MeV, sig0=0.1. "
      print *, "  Not in gendir. Closest: 92235.701nm"
      temp=4.3086923E-8
      sig0=0.1
      call ndi2_set_zaid_float64_temp_sig_0(handle,
     & "92235", temp, sig0, ierr)
      call check_noerr(ierr)

      print *, "Get zaid, temperature, and sig0:"
      call ndi2_get_string_val(handle, 1, zaid, ierr)
      if (ierr /= 0 .or. trim(zaid) /= '92235.701nm') then
         print *, "  **ZAID Error =", ierr
         print *, "  **ZAID =", zaid
      endif
      call ndi2_get_float64_val(handle, 6, temp, ierr)
      if (ierr /= 0) then
         print *, "  **TEMP Error =", ierr
      endif
      call ndi2_get_float64_val(handle, 7, sig0, ierr)
      if (ierr /= 0) then
         print *, "  **SIG0 Error =", ierr
      endif

      print 10000, "zaid = ",zaid,"temperature = ",temp,"sig0 =",sig0

      print*," "
      print*,"Set T=4.3086923E-8 MeV, sig0=1.0E6. "
      print *, "  Not in gendir. Closest: 92235.603nm"
      sig0=1E6
      call ndi2_set_zaid_float64_temp_sig_0(handle,
     & "92235", temp, sig0, ierr)
      call check_noerr(ierr)

      print *, "Get zaid, temperature, and sig0:"
      call ndi2_get_string_val(handle, 1, zaid, ierr)
      if (ierr /= 0 .or. trim(zaid) /= '92235.603nm') then
         print *, "  **ZAID Error =", ierr
         print *, "  **ZAID =", zaid
      endif
      call ndi2_get_float64_val(handle, 6, temp, ierr)
      if (ierr /= 0) then
         print *, "  **TEMP Error =", ierr
      endif
      call ndi2_get_float64_val(handle, 7, sig0, ierr)
      if (ierr /= 0) then
         print *, "  **SIG0 Error =", ierr
      endif

      print 10000, "zaid = ",zaid,"temperature = ",temp,"sig0 =",sig0

      print*," "
      print *, "Set T=5.170431E-8 MeV, sig0=1000.0. "
      print *, "  In gendir, 92235.603nm"
      temp=5.170431E-8
      sig0=1000.0
      call ndi2_set_zaid_float64_temp_sig_0(handle,
     & "92235", temp, sig0, ierr)
      call check_err(ierr)

      print *, "Get zaid, temperature, and sig0:"
      call ndi2_get_string_val(handle, 1, zaid, ierr)
      if (ierr /= 0 .or. trim(zaid) /= '92235.603nm') then
         print *, "  **ZAID Error =", ierr
         print *, "  **ZAID =", zaid
      endif
      call ndi2_get_float64_val(handle, 6, temp, ierr)
      if (ierr /= 0) then
         print *, "  **TEMP Error =", ierr
      endif
      call ndi2_get_float64_val(handle, 7, sig0, ierr)
      if (ierr /= 0) then
         print *, "  **SIG0 Error =", ierr
      endif

      print 10000, "zaid = ",zaid,"temperature = ",temp,"sig0 =",sig0

      print*," "
      print *, "Set T=5.170431E-8 MeV, sig0=1.0. "
      print *, "  Not in gendir, 92235.701nm"
      sig0=1.0
      call ndi2_set_zaid_float64_temp_sig_0(handle,
     & "92235", temp, sig0, ierr)
      call check_noerr(ierr)

      print *, "Get zaid, temperature, and sig0:"
      call ndi2_get_string_val(handle, 1, zaid, ierr)
      if (ierr /= 0 .or. trim(zaid) /= '92235.701nm') then
         print *, "  **ZAID Error =", ierr
         print *, "  **ZAID =", zaid
      endif
      call ndi2_get_float64_val(handle, 6, temp, ierr)
      if (ierr /= 0) then
         print *, "  **TEMP Error =", ierr
      endif
      call ndi2_get_float64_val(handle, 7, sig0, ierr)
      if (ierr /= 0) then
         print *, "  **SIG0 Error =", ierr
      endif

      print 10000, "zaid = ",zaid,"temperature = ",temp,"sig0 =",sig0
      
      print*," "
      print *, " Close gendir:"
      call ndi2_close_gendir(gendir, ierr)
      call check_err(ierr)

10000 format(2x,A7,A11,2x,A14,ES14.7,2x,A6,ES10.2)

      end

      subroutine check_err(ierr)
      
      integer ierr

      if (ierr .eq. 0) then
         print *, "ok."
      else
         print *, "  **Error = ", ierr
      endif
      end

      subroutine check_noerr(ierr)

      integer ierr

      if (ierr /= 0) then
         print *, "ok."
      else
         print *, "  **No Error = ", ierr
      endif
      end



