#include <stdio.h>

#include "ndi.h"
#include "ndi_internal.h"

/**********************************************************************/

int ndimp_get_size_n(const int *handle_arr, const int num_parts, 
                     int in_key, int vec_num, int *ierr) {

     int return_val = 0;
     int tmp_int = 0;

     /* Check each particle is multigroup */
     /* Check num_parts <= MAX_HANDLE_NUM */

     switch (in_key) {
          
     case NDI_PN_FULL:
         /*  */
         ndimp_get_int_val( handle_arr, num_parts, NDI_NUM_GRPS, &tmp_int);
         return_val = tmp_int * tmp_int;
         break;

     default:
          *ierr = -4;
          return_val = 0;
          break;
     }

     return(return_val);
   
}
/**********************************************************************/
