#include "ndi_keywords.h"


      program main

      integer ierr
      character*15 mg_lib1
      character*15 mg_lib2
      character*15 mg_lib3
      character*15 tn_lib1
      character*15 tn_lib2
      character*15 tn_lib3
      character*15 dos_lib1
      character*15 dos_lib2
      character*15 dos_lib3

 
       mg_lib1 = "mendf6"
       mg_lib2 = "mendf10"
       mg_lib3 = "MEndf6"
       tn_lib1 = "hale98"
       tn_lib2 = "testlib-tn"
       tn_lib3 = "TESTLIB_tn"
       dos_lib1 = "dostest"
       dos_lib2 = "dosimetrytest"
       dos_lib3 = "abuNTESt"

      call ndi_init(ierr)
      if ( ierr .ne. 0 ) then
         stop
       endif

      call ndi_set_option(NDI_LIB_TYPE, "multigroup_neutron", ierr)
      call ndi_set_option(NDI_LIBRARY, mg_lib1, ierr)
      print *, "  Setting multigroup library ", mg_lib1,
     $     "error is ",ierr
      call ndi_set_option(NDI_LIBRARY, mg_lib2, ierr)
      print *, "  Setting multigroup library ", mg_lib2,
     $     "error is ",ierr
      call ndi_set_option(NDI_LIBRARY, mg_lib3, ierr)
      print *, "  Setting multigroup library ", mg_lib3,
     $     "error is ",ierr

      call ndi_set_option(NDI_LIB_TYPE, "tn", ierr)
      call ndi_set_option(NDI_LIBRARY, tn_lib1, ierr)
      print *, "  Setting tn library ", tn_lib1, "error is ", ierr
      call ndi_set_option(NDI_LIBRARY, tn_lib2, ierr)
      print *, "  Setting tn library ", tn_lib2, "error is ", ierr
      call ndi_set_option(NDI_LIBRARY, tn_lib3, ierr)
      print *, "  Setting tn library ", tn_lib3, "error is ", ierr
   
      call ndi_set_option(NDI_LIB_TYPE, "dosimetry_neutron", ierr)
      call ndi_set_option(NDI_LIBRARY, dos_lib1, ierr)
      print *, "  Setting dosimetry library ", dos_lib1, "error is ",
     $     ierr
      call ndi_set_option(NDI_LIBRARY, dos_lib2, ierr)
      print *, "  Setting dosimetry library ", dos_lib2, "error is ",
     $     ierr
      call ndi_set_option(NDI_LIBRARY, dos_lib3, ierr)
      print *, "  Setting dosimetry library ", dos_lib3, "error is ",
     $     ierr
   
      call ndi_close(ierr)
      end
