#include <stdio.h>
#include <stdlib.h>

#include "ndi.h"

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );

int main ()

{
   int ierr;
   int array_size;

   ndi_float64 *ret_vec64 = NULL;


   /*  Initialize */
   ierr = ndi_init();
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi_set_option( NDI_COLLAPSE, "6_lanl" );
   ierr = ndi_set_isotope("92235.60");
   if (ierr) 
       printf("\n  **Error %d in setting isotope 92235.60.\n", ierr );
   

   /* Prompt transfer matrix */
   array_size = ndi_get_size_n(NDI_TRANSFER_MAT_PR, 0, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "TRANSFER_MAT_PR" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec_n(NDI_TRANSFER_MAT_PR, 0, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "TRANSFER_MAT_PR" );
   }
   printf("92235.60 TRANSFER MATRIX (PROMPT):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n");
   if (ret_vec64) { free(ret_vec64); }
   
   array_size = ndi_get_size_n(NDI_PN_FULL, 0, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "PN_FULL" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec_n(NDI_PN_FULL, 0, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "PN_FULL" );
   }
   printf("\n         FULL SCATTERING (0):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n");
   if (ret_vec64) { free(ret_vec64); }
   
   array_size = ndi_get_size(NDI_CHI_VEC_PR, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "CHI_VEC_PR" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec(NDI_CHI_VEC_PR, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "CHI_VEC_PR" );
   }
   printf("\n         CHI VECTOR (PROMPT):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n");
   if (ret_vec64) { free(ret_vec64); }
   
   array_size = ndi_get_size(NDI_NU_SIG_F_PR, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "NU_SIG_F_PR" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec(NDI_NU_SIG_F_PR, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "NU_SIG_F_PR" );
   }
   printf("\n         NUSIGF (PROMPT):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n\n");
   if (ret_vec64) { free(ret_vec64); }
   

   /* Total transfer matrix */
   array_size = ndi_get_size_n(NDI_TRANSFER_MAT_TOT, 0, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "TRANSFER_MAT_TOT" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec_n(NDI_TRANSFER_MAT_TOT, 0, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "TRANSFER_MAT_TOT" );
   }
   printf("\n92235.60 TRANSFER MATRIX (TOTAL):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n");
   if (ret_vec64) { free(ret_vec64); }
  
   array_size = ndi_get_size_n(NDI_PN_FULL, 0, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "PN_FULL" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec_n(NDI_PN_FULL, 0, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "PN_FULL" );
   }
   printf("\n         FULL SCATTERING (0):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n");
   if (ret_vec64) { free(ret_vec64); }

   array_size = ndi_get_size(NDI_CHI_VEC_TOT, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "CHI_VEC_TOT" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec(NDI_CHI_VEC_TOT, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "CHI_VEC_TOT" );
   }
   printf("\n         CHI VECTOR (TOTAL):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n");
   if (ret_vec64) { free(ret_vec64); }
   
   array_size = ndi_get_size(NDI_NU_SIG_F_TOT, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "NU_SIG_F_TOT" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec(NDI_NU_SIG_F_TOT, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "NU_SIG_F_TOT" );
   }
   printf("\n         NUSIGF (TOTAL):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n");
   if (ret_vec64) { free(ret_vec64); }
   
   ierr = ndi_close();
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
