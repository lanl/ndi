#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;

static void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );


int main () {

    int ierr = 0;
    int ret_size;
    int ret_int;
    int gendir_h;
    int handle;
    int num_p_grps = 2;
    char zaid[132];
    ndi_float64 photon_grp[] = { 20.0, 4.0, 0.1 };
    ndi_float64 *ret_vec64;

    ierr = NDI2_open_gendir(&gendir_h, "");
   if ( ierr != 0 ) {
      return(0);
   }
   ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle);
   if( ierr != 0) printf("  Error %d getting particle handle.\n", ierr);

   ierr = NDI2_set_option( handle, NDI_COLLAPSE, "12_group" );
   if ( ierr != 0 ) printf("  Error setting neutron structure is %d.\n\n", ierr );

   ierr = NDI2_set_isotope( handle, "92235" );
   if ( ierr != 0 ) printf("  Error setting isotope 92235 is %d.\n\n", ierr );
   ierr = NDI2_get_string_val(handle, NDI_ZAID, zaid, sizeof(zaid));
   printf("\n\nZAID: %s\n", zaid);
   
   ierr = NDI2_set_option( handle, NDI_CURR_PART, "0" );
   if ( ierr != 0 ) printf("  Error setting current particle is %d.\n\n", ierr );
   
    ierr = NDI2_set_float64_vec_option(handle, NDI_E_BOUNDS_CURR_PART, photon_grp, num_p_grps + 1);
   if ( ierr != 0 ) printf("  Error setting photon structure is %d.\n\n", ierr );

   ret_size = NDI2_get_size(handle, NDI_NUM_GRPS_CURR_PART, &ierr );
   if ( ierr != 0 ) printf("  Error getting size of photon group number is %d.\n\n", ierr );
   ierr = NDI2_get_int_val( handle, NDI_NUM_GRPS_CURR_PART, &ret_int);
   if ( ierr != 0 ) printf("  Error getting photon group number is %d.\n\n", ierr );
   printf("  Number of photon groups: %d  (size=%d)\n", ret_int, ret_size);

   ret_size = NDI2_get_size(handle, NDI_PN_ORDER_CURR_PART, &ierr );
   if ( ierr != 0 ) printf("  Error size of getting photon pn order is %d.\n\n", ierr );
   ierr = NDI2_get_int_val( handle, NDI_PN_ORDER_CURR_PART, &ret_int);
   if ( ierr != 0 ) printf("  Error getting photon pn order is %d.\n\n", ierr );
   printf("  Photon Pn order: %d  (size=%d)\n", ret_int, ret_size);

   ret_size = NDI2_get_size(handle, NDI_E_BOUNDS_CURR_PART, &ierr );
   if ( ierr != 0 ) printf("  Error getting size of photon energy bounds is %d.\n\n", ierr );
   ret_vec64 = (ndi_float64 *)malloc(ret_size * sizeof(ndi_float64));
   ierr = NDI2_get_float64_vec( handle, NDI_E_BOUNDS_CURR_PART, ret_vec64, ret_size );
   if ( ierr != 0 ) printf("  Error getting photon energy bounds is %d.\n\n", ierr );
   printf("  Photon energy boundaries: (size=%d)\n", ret_size);
   print_float64_vec(ret_vec64, ret_size);
   free(ret_vec64);

   ret_size = NDI2_get_size(handle, NDI_HEATING_CURR_PART, &ierr );
   if ( ierr != 0 ) printf("  Error getting size of photon heating is %d.\n\n", ierr );
   ret_vec64 = (ndi_float64 *)malloc(ret_size * sizeof(ndi_float64));
   ierr = NDI2_get_float64_vec( handle, NDI_HEATING_CURR_PART, ret_vec64, ret_size );
   if ( ierr != 0 ) printf("  Error getting photon heating is %d.\n\n", ierr );
   printf("  Photon heating: (size=%d)\n", ret_size);
   print_float64_vec(ret_vec64, ret_size);
   free(ret_vec64);

   ret_size = NDI2_get_size(handle, NDI_KERMA_CURR_PART, &ierr );
   if ( ierr != 0 ) printf("  Error getting size of photon kerma is %d.\n\n", ierr );
   ret_vec64 = (ndi_float64 *)malloc(ret_size * sizeof(ndi_float64));
   ierr = NDI2_get_float64_vec( handle, NDI_KERMA_CURR_PART, ret_vec64, ret_size );
   if ( ierr != 0 ) printf("  Error getting photon kerma is %d.\n\n", ierr );
   printf("  Photon kerma: (size=%d)\n", ret_size);
   print_float64_vec(ret_vec64, ret_size);
   free(ret_vec64);

   ret_size = NDI2_get_size_n(handle, NDI_PN_PROD_FULL_CURR_PART, 0, &ierr );
   if ( ierr != 0 ) printf("  Error getting size of photon production matrix is %d.\n\n", ierr );
   ret_vec64 = (ndi_float64 *)malloc(ret_size * sizeof(ndi_float64));
   ierr = NDI2_get_float64_vec_n( handle, NDI_PN_PROD_FULL_CURR_PART, 0, ret_vec64, ret_size );
   if ( ierr != 0 ) printf("  Error getting photon production is %d.\n\n", ierr );
   printf("  Photon production: (size=%d)\n", ret_size);
   print_float64_vec(ret_vec64, ret_size);
   free(ret_vec64);

   NDI2_close_gendir(gendir_h);
   return(ierr);
}

/**********************************************************************/

static void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 4 ) == 0) )
	 printf("\n");
      printf("%22.15e  ", flt64_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/
