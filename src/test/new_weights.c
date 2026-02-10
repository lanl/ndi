#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ndi.h"

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );

int main ()

{
   int ierr;
   int i,j,k,g;
   int array_size;
   int str_size;
   int num_zaids = 1;
   int num_wgts = 3;
   int num_grps = 6;
   int gendir_h = -1;
   int handle = -1;
   char zaid[25];
   char *info_str;
   char *str_ptr;
   char *zaid_list[] = { "92236.901nm" };

   int num_keywords = 9;
   int keyword_list[] = { NDI_SIG_TOT,
                          NDI_VEL,
                          NDI_NU_TOT,
                          NDI_NU_SIG_F_TOT,
                          NDI_CHI_VEC_PR,
                          NDI_CHI_MAT_PR,
                          NDI_FISS_Q,
                          NDI_PN_FULL,
                          NDI_PN_FULL
   };

   char *keyword_label[]= { "sigma total", "velocity", "nu total", "nu-sig_f total",
                            "chi vector prompt", "chi matrix prompt", "fission-q",
                            "p0 scattering", "p1 scattering"
   };

   ndi_float64 *ret_vec64 = NULL;
   ndi_float64 weights[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
   ndi_float64 weights2[] = {2.0, 0.0, 2.0, 3.0, 3.0, 3.0};
   ndi_float64 weights3[] = {6.0, 5.0, 4.0, 3.0, 2.0, 1.0};


   /*  Initialize */
   ierr = NDI2_open_gendir(&gendir_h, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle);
   if ( ierr != 0 ) printf("  Error %d making particle handle.\n", ierr);
   ierr = NDI2_set_option( handle, NDI_LIBRARY, "weights_test" );
   if ( ierr != 0 ) printf("  Error %d setting library.\n");
   ierr = NDI2_set_option( handle, NDI_COLLAPSE, "2_lanl" );
   if ( ierr != 0 ) printf("  Error %d setting collapse.");

   for ( i = 0; i < num_zaids; i++ ) {

       /* Set isotope */
     ierr = NDI2_set_isotope(handle, zaid_list[i]);
     if (ierr) 
       printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );

     for ( k = 0; k < num_wgts; k++ ) {

       if (k == 1 ) {
	 for ( g = 0; g < num_grps; g++ ) {
           weights[g] = weights2[g];
         }
       } else if ( k == 2 ) {
         for ( g = 0; g < num_grps; g++ ) {
           weights[g] = weights3[g];
         }
       }

       ierr = NDI2_set_float64_vec_option( handle, NDI_WGTS, weights, 6);
       if (ierr)
         printf("\n  **Error %d setting weight vector.\n", ierr);
       str_size = NDI2_get_size (handle, NDI_INFO, &ierr );
       if (ierr) 
         printf("\n  **Error %d info string size.\n", ierr );
       info_str = (char *)malloc( str_size * sizeof(char) );
       ierr = NDI2_get_string_val( handle, NDI_INFO, info_str, str_size );
       if (ierr) {
         printf("  Error %d getting information for isotope %s.\n", ierr, zaid_list[i] );
       }

       printf(" ************************************************************************************ \n" );
       printf("    MULTIGROUP NEUTRON DATA SET WEIGHTS TEST %d\n ", k );
       printf(" ************************************************************************************ \n" );
     
       printf("\nWeights are:\n");
       print_float64_vec( weights, num_grps );

       if ( info_str ) { free( info_str ); }

       ierr = NDI2_get_string_val( handle, NDI_ZAID, zaid, 25 );
       printf("\nZaid = %s\n\n", zaid );

       /*  Loop over keywords */
       for ( j = 0; j < num_keywords; j++ ) {

         /* Stupid, messy print to get capitalized string */
         str_ptr = keyword_label[j];
         str_ptr++;
         printf( "%c%s:\n", toupper(*(keyword_label[j])), str_ptr );

         if ( j == 7 || j == 8 ) {
           array_size = NDI2_get_size_n( handle, keyword_list[j], 0, &ierr );
         } else {
           array_size = NDI2_get_size( handle, keyword_list[j], &ierr );
         }
         if (ierr) 
	   printf("\n  **Error %d %s array size.\n", ierr, keyword_label[j] );
         ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
         if ( j == 7 ) {
           ierr = NDI2_get_float64_vec_n( handle, keyword_list[j], 0, ret_vec64, array_size );
         } else if (j == 8 ) {
           ierr = NDI2_get_float64_vec_n( handle, keyword_list[j], 1, ret_vec64, array_size );
         } else {
           ierr = NDI2_get_float64_vec( handle, keyword_list[j], ret_vec64, array_size);
         }
         if (ierr) {
	   printf("  **Error %d in %s -- returned value is:\n", ierr, keyword_label[j] );
         }
         print_float64_vec( ret_vec64, array_size );	 
         printf("\n");
         if (ret_vec64) { free(ret_vec64); }

       }   /* for j */

     }  /* for k */

   } /* for i */

   ierr = NDI2_close_gendir(gendir_h);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );

   return(ierr);
}

/**********************************************************************/

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 6 ) == 0) )
	 printf("\n");
      printf("%13.6e   ", flt64_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/

