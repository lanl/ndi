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
   int i,j, k, l;
   int array_size;
   int num_zaids = 4;
   int gendir_h = -1;
   int handle = -1;
   char zaid[25];
   char *str_ptr;
   char *zaid_list[] = { 
       "H2", "Be7", "U235", "Pu239"
   };

   int num_keywords = 14;
   int keyword_list[] = { NDI_FISS_Q,
			  NDI_NU_TOT,
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
                          NDI_SF_SPECTRA
   };

   char *keyword_label[]= { "fission_q",
			    "nu total",
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
			    "delayed chi matrix",
                            "spontaneous fission spectra"
   };

   ndi_float64 *ret_vec64 = NULL;
   ndi_float64 rate;


   /*  Initialize */
   ierr = NDI2_open_gendir(&gendir_h, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle);
   if( ierr != 0) printf("  Error %d making particle handle.\n", ierr);


   for (l = 0; l < 2; l++) {
       for ( i = 0; i < num_zaids; i++ ) {

           /* Set isotope */
           ierr = NDI2_set_isotope(handle, zaid_list[i]);
           if (ierr) 
               printf("\n  **Error %d in setting isotope %s.\n", 
                      ierr, zaid_list[i] );

           if (l == 1)
               ierr = NDI2_set_option(handle, NDI_COLLAPSE, "1_lanl");

           ierr = NDI2_get_string_val(handle, NDI_ZAID, zaid, 25 );
           printf("\nZaid = %s\n\n", zaid );

           /* Requirement 46 */
           ierr = NDI2_get_int_val(handle, NDI_SPONTANEOUS_FISSIONABLE, &j);
           if (ierr)
               printf("\n  **Error %d checking spontaneous fissionable.\n", 
                      ierr);
           else
               printf("\nSpontaneous Fissionable? = %d\n\n", j);
           ierr = NDI2_get_float64_val(handle, NDI_SF_RATE, &rate);
           if (ierr)
               printf("\n  **Error %d getting spontaneous fission rate. \n",
                      ierr);
           else
               printf("\nSpontaneous Fission Rate = %e\n\n", rate);

           /*  Loop over keywords */
           for ( j = 0; j < num_keywords; j++ ) {

               /* Stupid, messy print to get capitalized string */
               str_ptr = keyword_label[j];
               str_ptr++;
               printf( "%c%s:\n", toupper(*(keyword_label[j])), str_ptr );

               array_size = NDI2_get_size(handle, keyword_list[j], &ierr );
               if (ierr) 
                   printf("\n  **Error %d %s array size.\n", 
                          ierr, keyword_label[j] );
               ret_vec64 = (ndi_float64 *)malloc(array_size * 
                                                 sizeof(ndi_float64) );
               ierr = NDI2_get_float64_vec(handle, keyword_list[j], ret_vec64, 
                                           array_size);
               printf("  **Error %d in %s -- returned value is:\n", 
                      ierr, keyword_label[j] );

               if (keyword_list[j] == NDI_SF_SPECTRA)  /* rescale data */
                   for (k = 0; k < array_size; k++) 
                       ret_vec64[k] *= 6.023e23*1.0e8*3.154e7; /* #/mole/yr */
               print_float64_vec( ret_vec64, array_size );	 
               printf("\n");
               if (ret_vec64) { free(ret_vec64); }

           }   /* for j */

           /* Total fission cross section */
           printf ( "Total fission:\n");
           array_size = NDI2_get_size_n(handle, NDI_SIG_REAC, 18, &ierr );
           if (ierr) 
               printf("\n  **Error %d total_fission array size.\n", ierr );
           ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64));
           ierr = NDI2_get_float64_vec_n(handle, NDI_SIG_REAC, 18, ret_vec64, 
                                         array_size);
           printf("  **Error %d in total_fission -- returned value is:\n", 
                  ierr );
           print_float64_vec( ret_vec64, array_size );	 
           printf("\n");
           if (ret_vec64) { free(ret_vec64); }
     

       }  /* for i */
   }

   ierr = NDI2_close_gendir(gendir_h);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );

   return(ierr);
}

/**********************************************************************/

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 4 ) == 0) )
	 printf("\n");
      printf("%22.15e  ", flt64_ptr[n]);
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
