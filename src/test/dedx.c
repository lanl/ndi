#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

int main ()

{
   int ierr;
   int int_val;
   int num_targets, num_grps, num_dens, num_temps, num_tot;
   int *int_vec;
   int * target_zaids;
   int z,ai,i,n;
   int gendir_h;
   int handle;
   float err;
   double sig0;
   double temp;
   double double_val;
   double *double_vec;
   char test_zaid[20] = "";
   char zaid_in[20] = "";
   char string_val[80], tmp1[80], tmp2, tmp3, tmp4;

   printf("\n\n ********************************************************* \n" );
   printf("    TEST -- dE/dx data\n" );
   printf(" ********************************************************* \n\n" );

   ierr = NDI2_open_gendir(&gendir_h, "./gendir");
   if (ierr != 0) printf("  **Error %d in NDI init.\n\n", ierr );

   ierr = NDI2_make_handle(gendir_h, "dedx", &handle);
   if( ierr != 0) printf("  Error %d making dE/dx handle.\n", ierr);

   ierr = NDI2_set_option(handle, NDI_LIBRARY, "dedx");
   if( ierr != 0) printf("  Error %d setting library.\n", ierr);

   ierr = NDI2_set_zaid(handle, "1001");
   if( ierr != 0) printf("  Error %d setting isotope H-1.\n", ierr);

/* Test get_string_val */

   int_val = NDI2_get_size(handle, NDI_ZAID, &ierr);
   ierr = NDI2_get_string_val(handle, NDI_ZAID, string_val, int_val);
   if( ierr != 0) printf("  Error %d retrieving ZAID.\n", ierr);
   printf("ZAID of length %d: %s\n",int_val, &string_val);

   int_val = NDI2_get_size(handle, NDI_INFO, &ierr);
   ierr = NDI2_get_string_val(handle, NDI_INFO, tmp1, int_val);
   if( ierr != 0) printf("  Error %d retrieving info.\n", ierr);
   printf("Info: %s", &tmp1);

   int_val = NDI2_get_size(handle, NDI_DATE_PROCESSED, &ierr);
   ierr = NDI2_get_string_val(handle, NDI_DATE_PROCESSED, tmp1, int_val);
   if( ierr != 0) printf("  Error %d retrieving date processed.\n", ierr);
   printf("Date processed: %s", &tmp1);

   int_val = NDI2_get_size(handle, NDI_DATE_SOURCE, &ierr);
   ierr = NDI2_get_string_val(handle, NDI_DATE_SOURCE, tmp1, int_val);
   if( ierr != 0) printf("  Error %d retrieving date source.\n", ierr);
   printf("Date source: %s", &tmp1);

   int_val = NDI2_get_size(handle, NDI_LIBRARY_NAME, &ierr);
   ierr = NDI2_get_string_val(handle, NDI_LIBRARY_NAME, tmp1, int_val);
   if( ierr != 0) printf("  Error %d retrieving library name.\n", ierr);
   printf("Library name: %s\n", &tmp1);

/* Test get_float_val */

   ierr = NDI2_get_float64_val(handle, NDI_AT_WGT, &double_val);
   if( ierr != 0) printf("  Error %d retrieving atomic weight.\n", ierr);
   printf("Atomic weight %f\n\n", double_val);

/* Test get_int_val */

   ierr = NDI2_get_int_val(handle, NDI_NUM_GRPS, &num_grps);
   if( ierr != 0) printf("  Error %d retrieving number of energies.\n", ierr);
   printf("Number of energies %d\n", num_grps);

   ierr = NDI2_get_int_val(handle, NDI_NUM_DENSITIES, &num_dens);
   if( ierr != 0) printf("  Error %d retrieving number of densities.\n", ierr);
   printf("Number of densities %d\n", num_dens);

   ierr = NDI2_get_int_val(handle, NDI_NUM_TEMPS, &num_temps);
   if( ierr != 0) printf("  Error %d retrieving number of temperatures.\n", ierr);
   printf("Number of temperatures %d\n", num_temps);

   ierr = NDI2_get_int_val(handle, NDI_NUM_TARGET, &num_targets);
   if( ierr != 0) printf("  Error %d retrieving number of targets.\n", ierr);
   printf("Number of targets %d\n\n", num_targets);

   num_tot = num_grps * num_dens * num_temps;

/* _n */

   for( i = 0; i < num_targets; ++i) {
       ierr = NDI2_get_int_val_n(handle, NDI_TARGET_ZAID, i, &int_val);
       if( ierr != 0) printf("  Error %d retrieving target %d's ZAID.\n", ierr, i);
       printf("Target %d's ZAID %d\n",i, int_val);
   }
   printf("\n");

/* Test get_int_vec */

   target_zaids = (int *) malloc(num_targets * sizeof(int));
   ierr = NDI2_get_int_vec(handle, NDI_TARGET_ZAID, target_zaids, num_targets);
   if( ierr != 0) printf("  Error %d retrieving target ZAIDS.\n", ierr);
   for( i = 0; i < num_targets; ++i) {
       printf("Target %d's ZAID %d\n", i, target_zaids[i]);
   }
   printf("\n");

/* Test get_size */

   int_val = NDI2_get_size(handle, NDI_DEDX, &ierr);
   if( ierr != 0) printf("  Error %d retrieving size of dedx array.\n", ierr);
   printf("Size of dE/dx %d\n", int_val);

   int_val = NDI2_get_size(handle, NDI_TARGET_DEDX, &ierr);
   if( ierr != 0) printf("  Error %d retrieving size of target dedx array.\n", ierr);
   printf("Size of target dE/dx %d\n", int_val);

/* _n */

   int_val = NDI2_get_size_n(handle, NDI_TARGET_ZAID, 0, &ierr);
   if( ierr != 0) printf("  Error %d retrieving size of target 0's ZAID.\n", ierr);
   printf("Size of target 0's ZAID %d\n", int_val);

   int_val = NDI2_get_size_n(handle, NDI_TARGET_DEDX, 10, &ierr);
   if( ierr != 0) printf("  Error %d retrieving size of target 10's dedx array.\n", ierr);
   printf("Size of target 10's dE/dx %d\n", int_val);

/* _x */

   int_val = NDI2_get_size_x(handle, NDI_TARGET_ZAID, "1001", &ierr);
   if( ierr != 0) printf("  Error %d retrieving size of target 1001's ZAID.\n", ierr);
   printf("Size of target 1001's ZAID %d\n", int_val);

   int_val = NDI2_get_size_x(handle, NDI_TARGET_DEDX, "5011", &ierr);
   if( ierr != 0) printf("  Error %d retrieving size of target 5011's dedx array.\n", ierr);
   printf("Size of target 5011's dE/dx %d\n\n", int_val);

/* test get_float_vec */

   double_vec = (double *) malloc(num_grps * sizeof(double));
   ierr = NDI2_get_float64_vec(handle, NDI_ENERGIES, double_vec, num_grps);
   if( ierr != 0) printf("  Error %d retrieving energies.\n", ierr);
   for( i = 0; i < num_grps; ++i) {
       printf("Energy(%d)=%e\n", i, double_vec[i]);
   }
   free((void *) double_vec);
   printf("\n");

   double_vec = (double *) malloc(num_dens * sizeof(double));
   ierr = NDI2_get_float64_vec(handle, NDI_DENSITIES, double_vec, num_dens);
   if( ierr != 0) printf("  Error %d retrieving densities.\n", ierr);
   for( i = 0; i < num_dens; ++i) {
       printf("Density(%d)=%e\n", i, double_vec[i]);
   }
   free((void *) double_vec);
   printf("\n");

   double_vec = (double *) malloc(num_temps * sizeof(double));
   ierr = NDI2_get_float64_vec(handle, NDI_TEMPS, double_vec, num_temps);
   if( ierr != 0) printf("  Error %d retrieving densities.\n", ierr);
   for( i = 0; i < num_temps; ++i) {
       printf("Temperature(%d)=%e\n", i, double_vec[i]);
   }
   free((void *) double_vec);
   printf("\n");

   double_vec = (double *) malloc(num_tot * sizeof(double));
   ierr = NDI2_get_float64_vec(handle, NDI_DEDX, double_vec, num_tot);
   if( ierr != 0) printf("  Error %d retrieving free electron dE/dx.\n", ierr);
   for( i = 0; i < 10; ++i) {
       printf("dE/dx(%d)=%e\n", i, double_vec[i]);
   }
   for( i = num_tot - 10; i < num_tot; ++i) {
       printf("dE/dx(%d)=%e\n", i, double_vec[i]);
   }
   free((void *) double_vec);
   printf("\n");

/* _x */

   double_vec = (double *) malloc(num_tot * sizeof(double));
   ierr = NDI2_get_float64_vec_x(handle, NDI_TARGET_DEDX, "94239", double_vec, num_tot);
   if( ierr != 0) printf("  Error %d retrieving free electron dE/dx.\n", ierr);
   for( i = 0; i < 10; ++i) {
       printf("dE/dx(%d)=%e\n", i, double_vec[i]);
   }
   for( i = num_tot - 10; i < num_tot; ++i) {
       printf("dE/dx(%d)=%e\n", i, double_vec[i]);
   }
   free((void *) double_vec);
   printf("\n");

/* Close Gendir */

   ierr = NDI2_close_gendir(gendir_h);
   if (ierr) printf("  **Error %d in closing gendir\n\n", ierr );

}
