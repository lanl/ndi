#include "ndi_keywords.h"

      program main

      integer ZAIDSS
      parameter (ZAIDSS=133)

      integer ierr
      integer i
      character*(ZAIDSS) zaid_str

      integer num_izaid
      integer izaid(14)

      integer num_zaid
      character*24 zaid_list(17)

      integer num_imt
      integer imt(15)

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

      num_imt = 15
      data imt / 1017,
     $           1102,
     $           2017,
     $           2102,
     $           3017,
     $           3102,
     $           16000,
     $           16001,
     $           16002,
     $           16003,
     $           17000,
     $           17001,
     $           102000,
     $           102001,
     $           102002 /

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
       write(*,8)
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

c     Test Requirement 19
       write(*, 12)
 12    format(/, "Check translation of MT to Label.")
       call ndi_set_isotope("92235", ierr)
       do 400 i = 1, 999
          call ndi_get_string_val_n(NDI_REAC_LABEL, i, zaid_str, ierr)
          if (ierr .eq. -24) then
c     skip invalid MTs
             continue
          else if (ierr .lt. 0) then
             write(*, 4) ierr
          else 
             write(*, 13) i, zaid_str
 13          format("  MT=", i4, ": ", a16)
          endif
 400   continue
c     Test Requirement 19 extended MTs
       write(*, 14)
 14    format(/, "Check translation of extended MT to label.")
       do 500 i = 1, num_imt
          call ndi_get_string_val_n(NDI_REAC_LABEL, imt(i),
     $         zaid_str, ierr)
          if (ierr .eq. -24) then
             continue
          else if (ierr .lt. 0) then
             write(*, 4) ierr
          else
             write(*, 15) imt(i), zaid_str
 15          format("  MT=", i7, ": ", a16)
          endif
 500   continue

      call ndi_close(ierr)

      end



