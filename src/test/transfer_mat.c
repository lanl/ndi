#include <stdio.h>
#include <stdlib.h>

#include "ndi.h"

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );

int main ()

{
   int ierr, i, j;
   int array_size;

   ndi_float64 *ret_vec64 = NULL;
   ndi_float64 *tmp_vec = NULL;

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

   /* Prompt PN full */
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
   
   /* Prompt chi vector */
   array_size = ndi_get_size(NDI_CHI_VEC_PR, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "CHI_VEC_PR" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec(NDI_CHI_VEC_PR, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "CHI_VEC_PR" );
   }

   /* allocate tmp_vec; initialize it as chi_matrix  */
   tmp_vec = (ndi_float64 *)malloc(array_size*array_size*sizeof(ndi_float64));
   for (i = 0; i < array_size; i++)
       for (j = 0; j < array_size; j++)
           tmp_vec[i*array_size+j] = ret_vec64[j];

   printf("\n         CHI VECTOR (PROMPT):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n");
   if (ret_vec64) { free(ret_vec64); }
   
   /* Prompt nusigf */
   array_size = ndi_get_size(NDI_NU_SIG_F_PR, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "NU_SIG_F_PR" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec(NDI_NU_SIG_F_PR, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "NU_SIG_F_PR" );
   }

   /* update tmp_vec with nusigf to get fission transfer matrix */
   for(i = 0; i < array_size; i++)
       for(j = 0; j < array_size; j++)
           tmp_vec[i*array_size+j] *= ret_vec64[i];

   printf("\n         NUSIGF (PROMPT):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n\n");
   if (ret_vec64) { free(ret_vec64); }
   
   /* Prompt fission transfer matrix (Reqt. 63) */
   array_size = ndi_get_size(NDI_FISS_XFER_MAT_PR, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "FISS_XFER_MAT_PR" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec(NDI_FISS_XFER_MAT_PR, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "FISS_XFER_MAT_PR" );
   }
   printf("92235.60 FISSION TRANSFER MATRIX (PROMPT):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n");

   for (i = 0; i < array_size; i++) {
       tmp_vec[i] -= ret_vec64[i];
       if (ret_vec64[i] != 0.0)
           tmp_vec[i] /= ret_vec64[i];
   }
   printf("92235.60 (sig_xf - nusigf*chi)/sig_xf (PROMPT):\n");
   print_float64_vec(tmp_vec, array_size);
   printf("\n");
          
   if (ret_vec64) { free(ret_vec64); }
   if (tmp_vec) free(tmp_vec);

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
  
   /* Total PN full */
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

   /* Total chi */
   array_size = ndi_get_size(NDI_CHI_VEC_TOT, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "CHI_VEC_TOT" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec(NDI_CHI_VEC_TOT, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "CHI_VEC_TOT" );
   }
   /* allocate tmp_vec; initialize it as chi_matrix  */
   tmp_vec = (ndi_float64 *)malloc(array_size*array_size*sizeof(ndi_float64));
   for (i = 0; i < array_size; i++)
       for (j = 0; j < array_size; j++)
           tmp_vec[i*array_size+j] = ret_vec64[j];

   printf("\n         CHI VECTOR (TOTAL):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n");
   if (ret_vec64) { free(ret_vec64); }

   /* Total nusigf */
   array_size = ndi_get_size(NDI_NU_SIG_F_TOT, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "NU_SIG_F_TOT" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec(NDI_NU_SIG_F_TOT, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "NU_SIG_F_TOT" );
   }
   /* update tmp_vec with nusigf to get fission transfer matrix */
   for(i = 0; i < array_size; i++)
       for(j = 0; j < array_size; j++)
           tmp_vec[i*array_size+j] *= ret_vec64[i];

   printf("\n         NUSIGF (TOTAL):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n");
   if (ret_vec64) { free(ret_vec64); }
   
   /* Total fission transfer matrix (Reqt. 63)*/
   array_size = ndi_get_size(NDI_FISS_XFER_MAT_TOT, &ierr );
   if (ierr) 
       printf("\n  **Error %d %s array size.\n", ierr, "FISS_XFER_MAT_TOT" );
   ret_vec64 = (ndi_float64 *)malloc(array_size * sizeof(ndi_float64) );
   ierr = ndi_get_float64_vec(NDI_FISS_XFER_MAT_TOT, ret_vec64, array_size);
   if (ierr) {
       printf("  **Error %d in %s -- returned value is:\n", ierr, "FISS_XFER_MAT_TOT" );
   }
   printf("\n92235.60 FISSION TRANSFER MATRIX (TOTAL):\n");
   print_float64_vec( ret_vec64, array_size );	 
   printf("\n");
   for (i = 0; i < array_size; i++) {
       tmp_vec[i] -= ret_vec64[i];
       if (ret_vec64[i] != 0.0)
           tmp_vec[i] /= ret_vec64[i];
   }
   printf("92235.60 (sig_xf - nusigf*chi)/sig_xf (TOTAL):\n");
   print_float64_vec(tmp_vec, array_size);
   printf("\n");
          
   if (ret_vec64) { free(ret_vec64); }
   if (tmp_vec) free(tmp_vec);

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
