#include "ndi_keywords.h"

      program main

      integer :: ierr, num_grps, 
     &           gendir_handle, library_handle, data_handle
      real*8 :: max_absolute_relative_error
      real*8, dimension(:), allocatable :: reference_array, trial_array,
     &                                     sink_array, lib_total_xs
      real*8, dimension(4) :: trial_collapse, relative_error,
     &                        reference_collapse = 
     & [ 5.881459,   7.506130,   7.800927,  20.53932 ]

      call ndi2_open_gendir( gendir_handle, "", ierr )
      if ( ierr .ne. 0 ) then
         write(*,*) "****Error opening NDI gendir is ", ierr
         stop
      endif

      call ndi2_make_handle
     & ( gendir_handle, "multigroup_neutron", data_handle, ierr )
      if ( ierr .ne. 0 ) then
         print *, "****Error setting NDI data handle is ", ierr
         stop
      endif

      call ndi2_set_option( data_handle, NDI_LIBRARY, "mendf5", ierr)
      if ( ierr .ne. 0 ) then
         print *, "****Error setting NDI library is ", ierr
         stop
      endif

      call ndi2_set_isotope(data_handle, "U235", ierr)
      if ( ierr .ne. 0 ) then
         write(*,*) "****Error setting isotope is ", ierr
         stop ierr
      endif

      write(*,*) "User-specified Weights Extension Tests"
      write(*,*) "======================================="
      write(*,*) "Library: mendf5"
      write(*,*) "Library Type: multigroup_neutron"
      write(*,*) "Isotope: U235"
      write(*,*) "LibraryInformation"
      write(*,*) 
      write(*,*) "=================="

      call ndi2_get_size( data_handle, NDI_WGTS, num_grps, ierr)
      if ( ierr .ne. 0 ) then
         write(*,*) "****Error getting weight array size is ", ierr
         stop ierr
      endif

      write(*,*) "Weight array size: ", num_grps
      write(*, *)
      allocate( reference_array(num_grps) )
      allocate( trial_array(num_grps) )
      allocate( sink_array(num_grps) )
      allocate( lib_total_xs(num_grps) )
      write(*, *) "Extracting Library Weights..."
      write(*, *) "Command:"
      write(*, *) "-------------------------------------------"
      write(*, *)"call ndi2_get_float64_vec",
     & "( data_handle, NDI_WGTS, reference_array, num_grps, ierr ) "
      write(*, *) "-------------------------------------------"
      write(*, *)
      call ndi2_get_float64_vec
     &( data_handle, NDI_WGTS, reference_array, 30, ierr )
      if ( ierr .ne. 0 ) then
         print *, "****Error getting weights is ", ierr
         stop
      endif
      write(*,*)
      write(*,*) "Library Weights"
      write(*,*) "============================================="
      call print_float64_vec(reference_array, num_grps)
      write(*,*)
      trial_array = reference_array + 1.0;
      write(*,*) "Argument Weights"
      write(*,*) "============================================="
      call print_float64_vec(trial_array, num_grps)
      write(*,*)
      write(*,*) "Setting user-specified weights..."
      write(*,*) "Command:"
      write(*,*) "-------------------------------------------"
      write(*,*) "call ndi2_set_float64_vec_option",
     & "( data_handle, NDI_WGTS, trial_array, num_grps, ierr ) "
      write(*, *) "-------------------------------------------"
      write(*, *)
      call ndi2_set_float64_vec_option
     & ( data_handle, NDI_WGTS, trial_array, num_grps, ierr )
      if ( ierr .ne. 0 ) then
         print *, "****Error setting weights is ", ierr
         stop
      endif
      write(*,*) 
      write(*,*) 
     &"Extracting Library Weights after user specification..."
      write(*,*) "Command:"
      write(*,*) "-------------------------------------------"
      write(*, *)"call ndi2_get_float64_vec",
     & "( data_handle, NDI_WGTS, sink_array, num_grps, ierr ) "
      write(*, *) "-------------------------------------------"
      call ndi2_get_float64_vec
     &( data_handle, NDI_WGTS, sink_array, 30, ierr )
      if ( ierr .ne. 0 ) then
         print *, "****Error getting user weights is ", ierr
         stop
      endif
      call print_float64_vec(sink_array, num_grps)
      write(*,*) 
      write(*,*) 
     &"Extracting uncollapsed total cross section..."
      write(*,*) "Command:"
      write(*,*) "-------------------------------------------"
      write(*, *)"call ndi2_get_float64_vec",
     & "( data_handle, NDI_SIG_TOT, lib_total_xs, lib_total_xs, ierr ) "
      write(*, *) "-------------------------------------------"
      call ndi2_get_float64_vec
     & ( data_handle, NDI_SIG_TOT, lib_total_xs, num_grps);
      call print_float64_vec(lib_total_xs, num_grps)
      write(*,*) 
      write(*,*) 
     &"Collapsing to lanl_4 structure..."
      write(*,*) "Command:"
      write(*,*) "-------------------------------------------"
      write(*,*) "call ndi2_set_option",
     & "(data_handle, NDI_COLLAPSE, \"4_lanl\", ierr)"
      write(*,*) "call ndi2_get_float64_vec",
     & "(data_handle, NDI_SIG_TOT, trial_collapse, 4, ierr) "
      write(*,*) "-------------------------------------------"
      write(*,*) 
      call ndi2_set_option(data_handle, NDI_COLLAPSE, "4_lanl", ierr)
      call ndi2_get_float64_vec
     & (data_handle, NDI_SIG_TOT, trial_collapse, 4, ierr)
      call print_float64_vec(trial_collapse, 4)
      write(*,*) 
      write(*,*) "Reference Solution:"
      call print_float64_vec(reference_collapse, 4)
      relative_error = 
     & ( trial_collapse - reference_collapse ) / reference_collapse
      max_absolute_relative_error = maxval( abs( relative_error ) )
      write(*,*) "max error: ", max_absolute_relative_error
      if (max_absolute_relative_error > 1E-6 ) then
         print *, "****Error: weights do not agree "
         stop
      endif
      end

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      subroutine print_float64_vec(flt64_vec, num_elements)
      
      integer num_elements
      real*8 flt64_vec(*)

      print 20, (flt64_vec(j), j=1,num_elements)
 20   format(4(1pe15.6, 2x))
      
      return
      end

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
