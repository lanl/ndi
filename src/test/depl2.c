

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

#define MAX_ARRAY_SIZE 1000


int main ()

{
   int ierr;
   int ret_int;
   ndi_float64 ret_flt64_val;
   ndi_float64 ret_flt64[MAX_ARRAY_SIZE];

   ierr = ndi_init();
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi_set_option( NDI_DEPLETION, "test_depl" );

      
   /* Set isotope, no collapsing */
   ierr = ndi_set_isotope("92235");
   if (ierr) 
     printf("\n  **Error %d in setting isotope.\n", ierr);

   ierr = ndi_get_int_val( NDI_NUM_TARGET, &ret_int );
   ierr = ndi_get_float64_val_n( NDI_TOT_PROD_Q,  92235, &ret_flt64_val );
   ierr = ndi_get_float64_vec_x( NDI_TOT_PROD_XS, "92235", ret_flt64, MAX_ARRAY_SIZE );  

   ierr = ndi_set_isotope("Pu239");
   if (ierr) 
     printf("\n  **Error %d in setting isotope.\n", ierr);


   ierr = ndi_set_option( NDI_DEPLETION, "default" );

   ierr = ndi_get_int_val( NDI_NUM_TARGET, &ret_int );
   ierr = ndi_get_float64_val_n( NDI_TOT_PROD_Q,  92235, &ret_flt64_val );
   ierr = ndi_get_float64_vec_x( NDI_TOT_PROD_XS, "92235", ret_flt64, MAX_ARRAY_SIZE );  

   ierr = ndi_close();
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );
    
   return(0);
}

/****************************************************************
****************************************************************/

