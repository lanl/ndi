#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

int main ()

{
   int ierr;
   int i;
   int z,a,num_grps;
   int gendir_h;
   int handle;
   float err;
   double sig0;
   double temp;
   double double_val;
   double *double_vec, *double_vec2;
   char test_zaid[20] = "";
   char zaid_in[20] = "";

   char *zaid_list[] = { "92235.600nm", "1001.", "1039087", "pu239", "10020"  };

   printf("\n\n  ********************************************************* \n" );
   printf("    TEST--TEST COMPILER USING HE-4 133 GROUP DATA\n" );
   printf("    Some compilers (Intel) may obtain different results\n" );
   printf("    during a group collapse to 133.\n" );
   printf("  ********************************************************** \n\n" );

   ierr = NDI2_open_gendir(&gendir_h, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );

   ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle);
   if( ierr != 0) printf("  Error %d making particle handle.\n", ierr);

   ierr = NDI2_set_option(handle, NDI_LIBRARY, "mt80");
   if( ierr != 0) printf("  Error %d setting library.\n", ierr);

   ierr = NDI2_set_zaid(handle, "2004"); //, &handle);

   if( ierr != 0) printf("  Error %d setting isotope U-235.\n", ierr);

   ierr = NDI2_get_string_val(handle, NDI_ZAID, test_zaid, 11);

   if( ierr != 0) printf("  Error %d getting zaid.\n", ierr);

   ierr = NDI2_get_float64_val(handle, NDI_TEMP, &double_val );
   if( ierr != 0) printf("  Error %d getting temperature.\n", ierr);

   ierr = NDI2_get_float64_val(handle, NDI_SIG_0, &double_val );
   if( ierr != 0) printf("  Error %d getting sigma_0.\n", ierr);

   ierr = NDI2_get_int_val(handle, NDI_NUM_GRPS, &num_grps);
   if( ierr != 0) printf("  Error %d getting number of groups.\n", ierr);

   double_vec = (double *) malloc(num_grps * sizeof(double));
   ierr = NDI2_get_float64_vec_n(handle, NDI_SIG_REAC, 2, double_vec, num_grps);
   if( ierr != 0) printf("  Error %d getting MT=2 cross sections.\n", ierr);

   double_vec2 = (double *) malloc(num_grps * sizeof(double));
   ierr = NDI2_get_float64_vec(handle, NDI_SIG_TOT, double_vec2, num_grps);
   if( ierr != 0) printf("  Error %d getting MT=2 cross sections.\n", ierr);

   for( i = 0; i < num_grps; ++i) {
       printf("XS(%d)=%20.14e %20.14e\n", i, double_vec[i], double_vec2[i]);
   }
   free((void *) double_vec);
   free((void *) double_vec2);

   ierr = NDI2_set_option(handle, NDI_COLLAPSE, "133_lanl");
   if( ierr != 0) printf("  Error %d setting group collapse 133_lanl.\n", ierr);

   ierr = NDI2_get_int_val(handle, NDI_NUM_GRPS, &num_grps);
   if( ierr != 0) printf("  Error %d getting number of groups.\n", ierr);

   double_vec = (double *) malloc(num_grps * sizeof(double));
   ierr = NDI2_get_float64_vec_n(handle, NDI_SIG_REAC, 2, double_vec, num_grps);
   if( ierr != 0) printf("  Error %d getting MT=2 cross sections.\n", ierr);

   double_vec2 = (double *) malloc(num_grps * sizeof(double));
   ierr = NDI2_get_float64_vec(handle, NDI_SIG_TOT, double_vec2, num_grps);
   if( ierr != 0) printf("  Error %d getting MT=2 cross sections.\n", ierr);

   for( i = 0; i < num_grps; ++i) {
       printf("XS(%d)=%20.14e %20.14e\n", i, double_vec[i], double_vec2[i]);
   }
   free((void *) double_vec);



/* Close Gendir */

   ierr = NDI2_close_gendir(gendir_h);
   if (ierr) printf("  **Error %d in closing gendir\n\n", ierr );
  
}
