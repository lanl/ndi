#include "ndi_keywords.h"
#define ZAID_STR_SIZE 133

      program main

      integer ierr
      integer i
      character*ZAID_STR_SIZE zaid_str

      integer num_izaid
      integer izaid(13)

      integer num_zaid
      character*24 zaid_list(17)

      num_izaid = 13
      data izaid / 1001,
     $  0,
     $  1,
     $  92235,
     $  103270,
     $  2095240,
     $  20001002,
     $  110290,
     $  -999,
     $  999999,
     $  2,
     $  110000,
     $  1109000 /

      num_zaid = 17
      data zaid_list / "92235.600nm  ", 
     $                 "1001.", 
     $                 " 1002.   ",
     $                 "1003  ",
     $                 " 1039087", 
     $                 "pu239", 
     $                 "10020 ",
     $                 "h2",
     $                 " He-4",
     $                 "O16",
     $                 "C-12",
     $                 "i-127",
     $                 "TM169 ",
     $                 "TA-182",
     $                 "pu-240.6",
     $                 " h-1.50 ",
     $                 "Fl-19" /
      print *, ""
       print *, ""
       print *, "  ****************************************************"
       print *, "    TEST TRANSLATION OF ISOTOPIC SYMBOL TO ZAID "
       print *, " *****************************************************"
       print *, ""

       call ndi_init(ierr)
       if ( ierr .ne. 0 ) then
          print *, "  **Error %d in NDI init."
       endif

c     For all ZAIDS
       print *, "Interpret izaid before setting lib type."
       do 100, i = 1, num_izaid
          call ndi_get_string_val_n( NDI_ZAID, izaid(i), zaid_str,
     $         ierr )
          if ( ierr .ne. 0 ) then
             print *, "  ****ERROR = ", ierr
          endif
          print *, "  izaid = ", izaid(i), "  SZA = ", zaid_str
 100  continue

      call ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron", ierr )
      if ( ierr .ne. 0 ) then
         print *, "  ****ERROR ", ierr, " setting lib type."
         print *, ""
      endif

c     For all ZAIDs 
      print *, " "
      print *, "Interpret izaid after setting lib type."
      do 200, i = 1, num_izaid
          call ndi_get_string_val_n( NDI_ZAID, izaid(i), zaid_str,
     $         ierr )
          if ( ierr .ne. 0 ) then
             print *, "  ****ERROR = ", ierr
          endif
          print *, "  izaid = ", izaid(i), "  SZA = ", zaid_str
 200   continue
        
c     For all ZAIDs
      print *, " "
      print *, "Check translation of symbols in set_isotope."

      do 300, i = 1, num_zaid
         print *, "ISOTOPE=>", zaid_list(i), "<"
         call ndi_set_isotope( zaid_list(i), ierr )
         if ( ierr .ne. 0 ) then
            print *, "  **Error ", ierr, " in setting isotope ",
     $           zaid_list(i)
         else
            call ndi_get_string_val( NDI_ZAID, zaid_str, ierr )
            if ( ierr .ne. 0 ) then
               print *, "  ****ERROR = ", ierr
            endif
            print *, "  Zaid = >", zaid_str, "<"
         endif
 300   continue

      call ndi_close(ierr)

      end



