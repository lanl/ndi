#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ndi.h"

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );
void print_float32_vec( ndi_float32 *flt_ptr, int num_elements );
void print_int_vec( int *int_ptr, int num_elements );


int main ()

{
   int ierr;
   int i,j;
   int array_size;
   int str_size;
   int num_zaids = 36;
   int gendir_h = -1;
   int handle = -1;
   char zaid[25];
   char *info_str;
   char *str_ptr;
   char *zaid_list[] = { "92236.601nm", "92236.602nm", "92236.603nm", 
			 "92236.604nm", "92236.605nm", "92236.606nm", 
			 "92236.607nm", "92236.608nm", "92236.609nm", 

			 "92236.610nm", "92236.611nm", "92236.612nm",
			 "92236.613nm", "92236.614nm", "92236.615nm", 
			 "92236.616nm", "92236.617nm", "92236.618nm",

			 "92236.619nm", "92236.620nm", "92236.621nm", 
			 "92236.622nm", "92236.623nm", "92236.624nm", 
			 "92236.625nm", "92236.626nm", "92236.627nm", 

			 "92236.628nm", "92236.629nm", "92236.630nm",
			 "92236.631nm", "92236.632nm", "92236.631nm", 
			 "92236.634nm", "92236.635nm", "92236.632nm" 
   };

   int num_keywords = 12;
   int keyword_list[] = { NDI_NU_TOT,
			  NDI_NU_PR,
			  NDI_NU_DEL,
			  NDI_NU_SIG_F_TOT,
			  NDI_NU_SIG_F_PR,
			  NDI_NU_SIG_F_DEL,
			  NDI_CHI_VEC_TOT,
			  NDI_CHI_VEC_PR,
			  NDI_CHI_VEC_DEL,
			  NDI_CHI_MAT_TOT,
			  NDI_CHI_MAT_PR,
			  NDI_CHI_MAT_DEL,
   };

   char *keyword_label[]= { "nu total",
			    "nu prompt",
			    "nu delayed",
			    "nu_sig_f total",
			    "nu_sig_f prompt",
			    "nu_sig_f delayed",
			    "total chi vector",
			    "prompt chi vector",
			    "delayed chi vector",
			    "total chi matrix",
			    "prompt chi matrix",
			    "delayed chi matrix"
   };

   ndi_float64 *ret_vec64 = NULL;


   /*  Initialize */
   ierr = ndi2_open_gendir(&gendir_h, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi2_get_handle(gendir_h, &handle);
   if( ierr != 0) printf("  Error %d getting particle handle.\n", ierr);
   ierr = ndi2_set_option( handle, NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi2_set_option( handle, NDI_LIBRARY, "chi_test_lib" );

   for ( i = 0; i < num_zaids; i++ ) {

     /* Set isotope */
     ierr = ndi2_set_isotope(handle, zaid_list[i]);
     if (ierr) 
       printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );

     str_size = ndi2_get_size (handle, NDI_INFO, &ierr );
     if (ierr) 
       printf("\n  **Error %d info string size.\n", ierr );
     info_str = (char *)malloc( str_size * sizeof(char) );
     ierr = ndi2_get_string_val( handle, NDI_INFO, info_str, str_size );
     if (ierr) {
       printf("  Error %d getting information for isotope %s.\n", ierr, zaid_list[i] );
     }

     printf("\n\n ************************************** CHI TEST %d ", i + 1 );
     printf("********************************** \n");
     printf("    MULTIGROUP NEUTRON DATA CHI TEST\n " );
     printf("   %s", info_str );
     printf(" ************************************************************************************ \n" );
     
     if ( info_str ) { free( info_str ); }

     ierr = ndi2_get_string_val( handle, NDI_ZAID, zaid, 25 );
     printf("\nZaid = %s\n\n", zaid );
     

     /*  Loop over keywords */
     for ( j = 0; j < num_keywords; j++ ) {

       /* Stupid, messy print to get capitalized string */
       str_ptr = keyword_label[j];
       str_ptr++;
       printf( "%c%s:\n", toupper(*(keyword_label[j])), str_ptr );


       array_size = ndi2_get_size( handle, keyword_list[j], &ierr );
       if (ierr) 
	 printf("\n  **Error %d %s array size.\n", ierr, keyword_label[j] );
       ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
       ierr = ndi2_get_float64_vec( handle, keyword_list[j], ret_vec64, array_size);
       if (ierr) {
	 printf("  **Error %d in %s -- returned value is:\n", ierr, keyword_label[j] );
       }
       print_float64_vec( ret_vec64, array_size );	 
       printf("\n");
       if (ret_vec64) { free(ret_vec64); }

     }   /* for j */

   }  /* for i */

   ierr = ndi2_close_gendir(gendir_h);
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
/**********************************************************************/

void print_float32_vec( ndi_float32 *flt_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 6 ) == 0) )
	 printf("\n");
      printf("%13.6e   ", flt_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/

void print_int_vec( int *int_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( (n % 10) == 0) && (n != 0) )
	 printf("\n");
      printf("%d   ", int_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/
