#include "ndi_keywords.h"
#define ZAID_STR_SIZE 133

      program main

      integer ierr
      integer i
      character*ZAID_STR_SIZE zaid_str

      integer num_izaid
      integer izaid(14)

      integer num_zaid
      character*24 zaid_list(17)

      num_izaid = 14
      data izaid / 1001,
     $  0,
     $  1,
     $  92235,
     $  103270,
     $  2095240,
     $  20001002,
     $  110290,
     $  -999,
     $  99999,
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
      write(*, 2)
 2    format(/,/,
     $     "  ****************************************************",/,
     $     "    TEST TRANSLATION OF ISOTOPIC SYMBOL TO ZAID ",/,
     $     "  ****************************************************", /)

       call ndi_init(ierr)
       if ( ierr .ne. 0 ) then
          write(*, 3)
 3        format("  **Error %d in NDI init.")
       endif

c     For all ZAIDS
       write(*,40)
 40    format( "Interpret izaid before setting lib type.")
       do 100, i = 1, num_izaid
          call ndi_get_string_val_n( NDI_ZAID, izaid(i), zaid_str,
     $         ierr )
          if ( ierr .ne. 0 ) then
             write(*,4) ierr
 4           format("  ****ERROR = ", i5)
          endif
          write(*,5) izaid(i), zaid_str
 5        format("  izaid = ", i9,  "  SZA = ", a20)
 100  continue

      call ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron", ierr )
      if ( ierr .ne. 0 ) then
         write(*, 6) ierr
 6       format("  ****ERROR ", i5,  " setting lib type.", /)
      endif

c     For all ZAIDs 
      write(*, 7)
 7    format(/, "Interpret izaid after setting lib type.")
      do 200, i = 1, num_izaid
          call ndi_get_string_val_n( NDI_ZAID, izaid(i), zaid_str,
     $         ierr )
          if ( ierr .ne. 0 ) then
             write(*, 4) ierr
          endif
          write(*, 5)  izaid(i), zaid_str
 200   continue
        
c     For all ZAIDs
       write(*,*)
 8     format(/, "Check translation of symbols in set_isotope.")

      do 300, i = 1, num_zaid
         write(*, 9) zaid_list(i)
 9       format("ISOTOPE=>", a13,  "<" )
         call ndi_set_isotope( zaid_list(i), ierr )
         if ( ierr .ne. 0 ) then
            write(*,10) ierr,  zaid_list(i)
 10         format( "  **Error ", i5,  " in setting isotope ", a13)
         else
            call ndi_get_string_val( NDI_ZAID, zaid_str, ierr )
            if ( ierr .ne. 0 ) then
               write(*, 4) ierr
            endif
            write(*, 11) zaid_str
 11         format( "  Zaid = >", a13,  "<")
         endif
 300   continue

      call ndi_close(ierr)

      end



