#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"


int main ()

{
   int ierr;
   int i;
   int z,a;
   int gendir_h;
   int handle;
   float err;
   double sig0;
   double temp;
   double double_val;
   char test_zaid[20] = "";
   char zaid_in[20] = "";

   char *zaid_list[] = { "92235.600nm", "1001.", "1039087", "pu239", "10020"  };

   printf("\n\n  ********************************************************* \n" );
   printf("    TEST--SET ZAID BY TEMP AND SIGMA0\n " );
   printf(" ********************************************************** \n\n" );

   ierr = NDI2_open_gendir(&gendir_h, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );

   ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle);
   if( ierr != 0) printf("  Error %d making particle handle.\n", ierr);

   ierr = NDI2_set_option(handle, NDI_LIBRARY, "SS_TEST");
   if( ierr != 0) printf("  Error %d setting library.\n", ierr);

   ierr = NDI2_set_zaid(handle, "92235"); //, &handle);

   if( ierr != 0) printf("  Error %d setting isotope U-235.\n", ierr);

   ierr = NDI2_get_string_val(handle, NDI_ZAID, test_zaid, 11);

   if( ierr != 0) printf("  Error %d getting zaid.\n", ierr);

   ierr = NDI2_get_float64_val(handle, NDI_TEMP, &double_val );
   if( ierr != 0) printf("  Error %d getting temperature.\n", ierr);

   ierr = NDI2_get_float64_val(handle, NDI_SIG_0, &double_val );
   if( ierr != 0) printf("  Error %d getting sigma_0.\n", ierr);






/* temp = 300 K, sig0 = 0.1, ZAID is in gendir */
   sig0 = 0.1;
   temp = 2.5852155e-08;
   strncpy(zaid_in,"92235.101nm",11);

   ierr = NDI2_set_zaid_float64_temp_sig_0(handle, "92235", temp, sig0 );
   if( ierr != 0) printf("  Error %d setting ZAID.\n", ierr);

   ierr = NDI2_get_string_val(handle, NDI_ZAID, test_zaid, 11);

   if( ierr != 0 || strcmp(zaid_in, test_zaid)) {
       printf(" Case 1: Error %d getting zaid \n",ierr);
   }
   printf(" Case 1: Correct zaid = %s\n          Actual zaid = %s\n",zaid_in, test_zaid);

   ierr = NDI2_get_float64_val(handle, NDI_TEMP, &double_val );
   if( ierr != 0 || double_val != temp ) {
       printf(" Case 1: Error %d getting temperature \n",ierr);
    } 
    printf("         Correct temp = %e\n          Actual temp = %e\n", temp, double_val);
   if( double_val != temp ) printf("  Diff = %e \n", (double_val - temp));
  

   ierr = NDI2_get_float64_val(handle, NDI_SIG_0, &double_val );
   if( ierr != 0 || double_val != sig0 ) {
       printf(" Case 1: Error %d getting sigma0 \n",ierr);
   }
    printf("         Correct sigma0 = %e\n          Actual sigma0 = %e\n",sig0,double_val);
   if(double_val != sig0) printf("  Diff = %e \n", (double_val - sig0));


   printf(" \n");


/* temp = 300 K, sig0 = 10.0, ZAID is in gendir */
   sig0 = 10.0;
   temp = 2.5852155e-08;
   strncpy(zaid_in,"92235.001nm",11);

   ierr = NDI2_set_zaid_float64_temp_sig_0(handle, "92235", temp, sig0 );
   if( ierr != 0) printf("  Error %d setting ZAID.\n", ierr);

   ierr = NDI2_get_string_val(handle, NDI_ZAID, test_zaid, 11);

   if( ierr != 0 || strcmp(zaid_in, test_zaid)) {
       printf(" Case 2: Error %d getting zaid \n",ierr);
   }
   printf(" Case 2: Correct zaid = %s\n          Actual zaid = %s\n",zaid_in, test_zaid);
   

   ierr = NDI2_get_float64_val(handle, NDI_TEMP, &double_val );
   if( ierr != 0 || double_val != temp ) {
       printf(" Case 2: Error %d getting temperature \n",ierr);
    } 
    printf("         Correct temp = %e\n          Actual temp = %e\n", temp, double_val);
   if( double_val != temp ) printf("  Diff = %e \n", (double_val - temp));
  

   ierr = NDI2_get_float64_val(handle, NDI_SIG_0, &double_val );
   if( ierr != 0 || double_val != sig0 ) {
       printf(" Case 2: Error %d getting sigma_0 \n",ierr);
   }
    printf("         Correct sigma0 = %e\n          Actual sigma0 = %e\n",sig0,double_val);
   if(double_val != sig0) printf("  Diff = %e \n", (double_val - sig0));


   printf(" \n");


/* temp = 300 K, sig0 = 1.0, ZAID is not in gendir */
   sig0 = 1.0;
   temp = 2.5852155e-08;
   strncpy(zaid_in,"92235.101nm",11);

   ierr = NDI2_set_zaid_float64_temp_sig_0(handle, "92235", temp, sig0 );
   if( ierr != -8) printf("  Error %d setting ZAID.\n", ierr);

   ierr = NDI2_get_string_val(handle, NDI_ZAID, test_zaid, 11);

   if( ierr != 0 || strcmp(zaid_in, test_zaid)) {
       printf(" Case 3: Error %d getting zaid \n",ierr);
   }
   printf(" Case 3: Correct zaid = %s\n          Actual zaid = %s\n",zaid_in, test_zaid);

   ierr = NDI2_get_float64_val(handle, NDI_TEMP, &double_val );
   if( ierr != 0 || double_val != temp ) {
       printf(" Case 3: Error %d getting temperature \n",ierr);
    } 
   printf("         Correct temp = %e\n          Actual temp = %e\n", temp, double_val);
   if( double_val != temp ) printf("  Diff = %e \n", (double_val - temp));
  
/* Adjust sig0 to match stored */
   sig0 = 0.1;

   ierr = NDI2_get_float64_val(handle, NDI_SIG_0, &double_val );
   if( ierr != 0 || double_val != sig0 ) {
       printf(" Case 3: Error %d getting sigma_0 \n",ierr);
   }
   printf("         Correct sigma0 = %e\n          Actual sigma0 = %e\n",sig0,double_val);
   if(double_val != sig0) printf("  Diff = %e \n", (double_val - sig0));



   printf(" \n");



/* temp = 500 K, sig0 = 0.1, ZAID is not in gendir */
   sig0 = 0.1;
   temp = 4.3086923E-8;
   strncpy(zaid_in,"92235.701nm",11);

   ierr = NDI2_set_zaid_float64_temp_sig_0(handle, "92235", temp, sig0 );
   if( ierr != -8) printf("  Error %d setting ZAID.\n", ierr);

   ierr = NDI2_get_string_val(handle, NDI_ZAID, test_zaid, 11);

   if( ierr != 0 || strcmp(zaid_in, test_zaid)) {
       printf(" Case 4: Error %d getting zaid \n",ierr);
   }
   printf(" Case 4: Correct zaid = %s\n          Actual zaid = %s\n",zaid_in, test_zaid);

   temp = 5.170431E-8;

   ierr = NDI2_get_float64_val(handle, NDI_TEMP, &double_val );
   if( ierr != 0 || double_val != temp ) {
       printf(" Case 4: Error %d getting temperature \n",ierr);
    } 
    printf("         Correct temp = %e\n          Actual temp = %e\n", temp, double_val);
   if( double_val != temp ) printf("  Diff = %e \n", (double_val - temp));

   ierr = NDI2_get_float64_val(handle, NDI_SIG_0, &double_val );
   if( ierr != 0 || double_val != sig0 ) {
       printf(" Case 4: Error %d getting sigma_0 \n",ierr);
   }
    printf("         Correct sigma0 = %e\n          Actual sigma0 = %e\n",sig0,double_val);
   if(double_val != sig0) printf("  Diff = %e \n", (double_val - sig0));



   printf(" \n");



/* temp = 450 K, sig0 = 0.1, ZAID is not in gendir*/
   sig0 = 0.1;
   temp = 3.87782325E-8;
   strncpy(zaid_in,"92235.701nm",11);

   ierr = NDI2_set_zaid_float64_temp_sig_0(handle, "92235", temp, sig0 );
   if( ierr != -8) printf("  Error %d setting ZAID.\n", ierr);

   ierr = NDI2_get_string_val(handle, NDI_ZAID, test_zaid, 11);

   if( ierr != 0 || strcmp(zaid_in, test_zaid)) {
       printf(" Case 5: Error %d getting zaid \n",ierr);
   }
   printf(" Case 5: Correct zaid = %s\n          Actual zaid = %s\n",zaid_in, test_zaid);

   temp = 5.170431E-8;

   ierr = NDI2_get_float64_val(handle, NDI_TEMP, &double_val );
   if( ierr != 0 || double_val != temp ) {
       printf(" Case 5: Error %d getting temperature \n",ierr);
    } 
    printf("         Correct temp = %e\n          Actual temp = %e\n", temp, double_val);
   if( double_val != temp ) printf("  Diff = %e \n", (double_val - temp));
  

   ierr = NDI2_get_float64_val(handle, NDI_SIG_0, &double_val );
   if( ierr != 0 || double_val != sig0 ) {
       printf(" Case 5: Error %d getting sigma_0 \n",ierr);
   }
    printf("         Correct sigma0 = %e\n          Actual sigma0 = %e\n",sig0,double_val);
   if(double_val != sig0) printf("  Diff = %e \n", (double_val - sig0));



   printf(" \n");



/* temp = 400 K, sig0 = 0.1, ZAID is not in gendir */
   sig0 = 0.1;
   temp = 3.446954E-8;
   strncpy(zaid_in,"92235.101nm",11);

   ierr = NDI2_set_zaid_float64_temp_sig_0(handle, "92235", temp, sig0 );
   if( ierr != -8) printf("  Error %d setting ZAID.\n", ierr);

   ierr = NDI2_get_string_val(handle, NDI_ZAID, test_zaid, 11);

   if( ierr != 0 || strcmp(zaid_in, test_zaid)) {
       printf(" Case 6: Error %d getting zaid \n",ierr);
   }
   printf(" Case 6: Correct zaid = %s\n          Actual zaid = %s\n",zaid_in, test_zaid);

   temp = 2.5852155e-08;

   ierr = NDI2_get_float64_val(handle, NDI_TEMP, &double_val );
   if( ierr != 0 || double_val != temp ) {
       printf(" Case 6: Error %d getting temperature \n",ierr);
    } 
    printf("         Correct temp = %e\n          Actual temp = %e\n", temp, double_val);
   if( double_val != temp ) printf("  Diff = %e \n", (double_val - temp));
  

   ierr = NDI2_get_float64_val(handle, NDI_SIG_0, &double_val );
   if( ierr != 0 || double_val != sig0 ) {
       printf(" Case 6: Error %d getting sigma_0 \n",ierr);
   }
    printf("         Correct sigma0 = %e\n          Actual sigma0 = %e\n",sig0,double_val);
   if(double_val != sig0) printf("  Diff = %e \n", (double_val - sig0));



   printf(" \n");



/* temp = 500 K, sig0 = 1.0E6, zaid not in gendir */
   sig0 = 1.0E6;
   temp = 4.3086923E-8;
   strncpy(zaid_in,"92235.603nm",11);

   ierr = NDI2_set_zaid_float64_temp_sig_0(handle, "92235", temp, sig0 );
   if( ierr != -8) printf("  Error %d setting ZAID.\n", ierr);

   ierr = NDI2_get_string_val(handle, NDI_ZAID, test_zaid, 11);

   if( ierr != 0 || strcmp(zaid_in, test_zaid)) {
       printf(" Case 7: Error %d getting zaid \n",ierr);
   }
   printf(" Case 7: Correct zaid = %s\n          Actual zaid = %s\n",zaid_in, test_zaid);

   temp = 5.170431E-8;
   sig0 = 1.0E3;

   ierr = NDI2_get_float64_val(handle, NDI_TEMP, &double_val );
   if( ierr != 0 || double_val != temp ) {
       printf(" Case 7: Error %d getting temperature \n",ierr);
    } 
    printf("         Correct temp = %e\n          Actual temp = %e\n", temp, double_val);
   if( double_val != temp ) printf("  Diff = %e \n", (double_val - temp));  

   ierr = NDI2_get_float64_val(handle, NDI_SIG_0, &double_val );
   if( ierr != 0 || double_val != sig0 ) {
       printf(" Case 7: Error %d getting sigma_0 \n",ierr);
   }
    printf("         Correct sigma0 = %e\n          Actual sigma0 = %e\n",sig0,double_val);
   if(double_val != sig0) printf("  Diff = %e \n", (double_val - sig0));



   printf(" \n");




/* temp = 600 K, sig0 = 1000, zaid in gendir */
   sig0 = 1.0E3;
   temp = 5.170431E-8;
   strncpy(zaid_in,"92235.603nm",11);

   ierr = NDI2_set_zaid_float64_temp_sig_0(handle, "92235", temp, sig0 );
   if( ierr != 0) printf("  Error %d setting ZAID.\n", ierr);

   ierr = NDI2_get_string_val(handle, NDI_ZAID, test_zaid, 11);

   if( ierr != 0 || strcmp(zaid_in, test_zaid)) {
       printf(" Case 8: Error %d getting zaid \n",ierr);
   }
   printf(" Case 8: Correct zaid = %s\n          Actual zaid = %s\n",zaid_in, test_zaid);

   ierr = NDI2_get_float64_val(handle, NDI_TEMP, &double_val );
   if( ierr != 0 || double_val != temp ) {
       printf(" Case 8: Error %d getting temperature \n",ierr);
    } 
    printf("         Correct temp = %e\n          Actual temp = %e\n", temp, double_val);
   if( double_val != temp ) printf("  Diff = %e \n", (double_val - temp));
  

   ierr = NDI2_get_float64_val(handle, NDI_SIG_0, &double_val );
   if( ierr != 0 || double_val != sig0 ) {
       printf(" Case 8: Error %d getting sigma_0\n",ierr);
    } 
    printf("         Correct sigma0 = %e\n          Actual sigma0 = %e\n",sig0,double_val);
   if(double_val != sig0) printf("   Diff = %e \n", (double_val - sig0));
  


   printf(" \n");



/* temp = 600 K, sig0 = 5.0, zaid not in gendir */
   sig0 = 6.0;
   temp = 5.170431E-8;
   strncpy(zaid_in,"92235.602nm",11);

   ierr = NDI2_set_zaid_float64_temp_sig_0(handle, "92235", temp, sig0 );
   if( ierr != -8) printf("  Error %d setting ZAID.\n", ierr);

   ierr = NDI2_get_string_val(handle, NDI_ZAID, test_zaid, 11);

   if( ierr != 0 || strcmp("92235.602nm", test_zaid)) {
       printf(" Case 9: Error %d getting zaid \n",ierr);
       printf("  Note: we use 92235.602nm because 92235.601nm already exists in gendir file.");
   }
   printf(" Case 9: Correct zaid = %s\n          Actual zaid = %s\n",zaid_in, test_zaid);

   ierr = NDI2_get_float64_val(handle, NDI_TEMP, &double_val );
   if( ierr != 0 || double_val != temp ) {
       printf(" Case 9: Error %d getting temperature\n",ierr);
    } 
    printf("         Correct temp = %e\n          Actual temp = %e\n", temp, double_val);
   if( double_val != temp ) printf("  Diff = %e \n", (double_val - temp));
  

   sig0 = 10.0;

   ierr = NDI2_get_float64_val(handle, NDI_SIG_0, &double_val );
   if( ierr != 0 || double_val != sig0 ) {
       printf(" Case 9: Error %d getting sigma_0\n", ierr);
   }
    printf("         Correct sigma0 = %e\n          Actual sigma0 = %e\n",sig0,double_val);
   if(double_val != sig0) printf("  Diff = %e \n", (double_val - sig0));


/* Close Gendir */

   ierr = NDI2_close_gendir(gendir_h);
   if (ierr) printf("  **Error %d in closing gendir\n\n", ierr );
  
}
