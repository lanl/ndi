

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ndi.h"

#define MAX_ARRAY_SIZE 1000

static void print_all( int num_targ, ndi_float64 prod_q, ndi_float64 *prod_xs, int xs_size );

int main ()

{
   int ierr;
   int ret_int;
   int xs_size;
   int gendir_h = -1;
   int handle = -1;

   ndi_float64 ret_flt64_val;
   ndi_float64 ret_flt64[MAX_ARRAY_SIZE];

   ierr = ndi2_open_gendir(&gendir_h, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi2_get_handle(gendir_h, &handle);
   if( ierr != 0) printf("  Error %d getting particle handle.\n", ierr);
   ierr = ndi2_set_option(handle,  NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi2_set_option(handle,  NDI_DEPLETION, "test2" );

      
   /* Set isotope, no collapsing */
   ierr = ndi2_set_isotope(handle, "94242");
   if (ierr) 
     printf("\n  **Error %d in setting isotope.\n", ierr);

   ierr = ndi2_get_int_val(handle, NDI_NUM_TARGET, &ret_int );
   ierr = ndi2_get_float64_val_n(handle, NDI_TOT_PROD_Q,  94240, &ret_flt64_val );
   xs_size = ndi2_get_size_x(handle, NDI_TOT_PROD_XS, "94240", &ierr );
   ierr = ndi2_get_float64_vec_x(handle, NDI_TOT_PROD_XS, "94240", ret_flt64, MAX_ARRAY_SIZE );  
   
   printf( "Target=94242; product=94240:\n");
   print_all( ret_int, ret_flt64_val, ret_flt64, xs_size );


   ierr = ndi2_set_isotope(handle,"Pu239");
   if (ierr) 
     printf("\n  **Error %d in setting isotope.\n", ierr);


   ierr = ndi2_set_option(handle, NDI_DEPLETION, "default" );

   ierr = ndi2_get_int_val(handle, NDI_NUM_TARGET, &ret_int );
   ierr = ndi2_get_float64_val_n(handle, NDI_TOT_PROD_Q,  94240, &ret_flt64_val );
   xs_size = ndi2_get_size_x(handle, NDI_TOT_PROD_XS, "94240", &ierr );
   ierr = ndi2_get_float64_vec_x(handle, NDI_TOT_PROD_XS, "94240", ret_flt64, MAX_ARRAY_SIZE );  
   
   printf( "Target=Pu239; product=94240:\n");
   print_all( ret_int, ret_flt64_val, ret_flt64, xs_size );

   ierr = ndi2_close_gendir(gendir_h);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );
    
   return(0);
}

/****************************************************************
****************************************************************/

static void print_all( int num_targ, ndi_float64 prod_q, ndi_float64 *prod_xs, int xs_size )
{

   int i;

    printf("  num target: %d\n  prod q:  %12.6e\n  prod xs:\n", num_targ, prod_q );
    for ( i = 0; i < xs_size; i++ ) {
	if ( ( i != 0 ) && ( fmod(i, 6) == 0 ) ) {
	    printf("\n");
	}
	printf("  %11.5e", prod_xs[i] );
    }
    printf("\n");
}

