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
   int gendir;
   int mpmg;
   int handle[2];
   int return_int = 0;
   int int_vec_size = 0;
   int *int_vec = NULL;
   int flt_vec_size;
   int pgrps = 2;
   ndi_float64 pbounds[] = { 20.0, 4.0, 0.1 };
   ndi_float64 *flt64_vec = NULL;

   /* Inintialize gendir */
   ierr = ndi2_open_gendir(&gendir, "");
   if (ierr)
     printf("  **Error %d in NDI open 0.\n\n", ierr );

   /* Get mpmg handle */
   ierr = ndi2_get_handle_gendir(gendir, &mpmg);
   if (ierr)
     printf("  **Error %d in NDI get_handle 0.\n\n", ierr );

   /* set mpmg handle type */
   ierr = ndi2_set_lib_type(mpmg, "multigroup_multi");
   if (ierr)
     printf("  **Error %d in NDI set_lib_type 0.\n\n", ierr );

   /*  Initialize particle 0 */
   ierr = ndi2_get_handle_mpmg(mpmg, &handle[0]);
   if (ierr)
     printf("  **Error %d in NDI get_handle_mpmg 0.\n\n", ierr );

   ierr = ndi2_set_option(handle[0], NDI_LIB_TYPE, "multigroup_neutron");
   ierr = ndi2_set_option(handle[0], NDI_LIBRARY, "specmg");
   ierr = ndi2_set_option(handle[0], NDI_COLLAPSE, "2_lanl");
   ierr = ndi2_set_isotope(handle[0], "U235");

   /*  Initialize particle 1 */
   ierr = ndi2_get_handle_mpmg(mpmg, &handle[1]);
   if (ierr)
     printf("  **Error %d in NDI get_handle_mpmg 1.\n\n", ierr );

   ierr = ndi2_set_option(handle[1], NDI_LIB_TYPE, "multigroup_photon");
   ierr = ndi2_set_option(handle[1], NDI_LIBRARY, "photmg");
   ierr = ndi2_set_float64_vec_option(handle[1], NDI_COLLAPSE, pbounds, pgrps + 1);
   ierr = ndi2_set_isotope(handle[1], "92000");

   /*  Initialize particle 2 */
/*
   ierr = ndi2_get_handle_mpmg(mpmg, &handle[2]);
   if (ierr)
     printf("  **Error %d in NDI init 2.\n\n", ierr );

   ierr = ndi2_set_option(handle[2], NDI_LIB_TYPE, "multigroup_neutron");
   ierr = ndi2_set_option(handle[2], NDI_LIBRARY, "mendf5");
   ierr = ndi2_set_option(handle[2], NDI_COLLAPSE, "2_lanl");

   ierr = ndi2_set_isotope(handle[2], "U238");
*/

   /* start accessing data */
   ierr = ndi2_get_int_val(mpmg, NDI_NUM_GRPS, &return_int);
   if (ierr) 
        printf("\n  **Error %d getting number of groups.\n", ierr );
   printf("  Number of groups:  %d\n", return_int);

   ierr = ndi2_get_int_val(mpmg, NDI_PN_ORDER, &return_int);
   if (ierr) 
        printf("\n  **Error %d getting pn order.\n", ierr );
   printf("  Pn order:  %d\n", return_int);

   ierr = ndi2_get_int_val(mpmg, NDI_NUM_PNU_PR, &return_int);
   if (ierr) 
        printf("\n  **Error %d getting num pnu.\n", ierr );
   printf("  No. pnu:  %d\n", return_int);

   ierr = ndi2_get_int_val(mpmg, NDI_NUM_REAC, &return_int);
   if (ierr) 
        printf("\n  **Error %d getting num reactions.\n", ierr );
   printf("  No. reactions:  %d\n", return_int);

   /* LAST NEUTRON AND PHOTON GRPS */
   ierr = ndi2_get_int_val_n(mpmg, NDI_LAST_GRP, 0, &return_int);
   if (ierr) 
       printf("\n  **Error %d getting last neutron group.\n", ierr );
   printf("  Last neutron group:  %d\n", return_int);
   ierr = ndi2_get_int_val_n(mpmg, NDI_LAST_GRP, 1, &return_int);
   if (ierr) 
       printf("\n  **Error %d getting last photon.\n", ierr );
   printf("  Last photon group:  %d\n", return_int);

 
   /* LIST OF MTS */
   int_vec_size = ndi2_get_size(mpmg, NDI_REAC_MT, &ierr); 
   if (ierr) 
        printf("\n  **Error %d getting size of reaction MTs.\n", ierr );
        printf("  Reaction MTs size:  %d\n", int_vec_size); 
   
   int_vec = (int *)calloc(int_vec_size, sizeof(int));
   ierr = ndi2_get_int_vec(mpmg, NDI_REAC_MT, int_vec, int_vec_size);
   if (ierr) 
        printf("\n  **Error %d getting reaction MTs.\n", ierr );
   print_int_vec(int_vec, int_vec_size);
   if (int_vec) {
        free(int_vec);
   }


   /* EBOUNDS */
   flt_vec_size = ndi2_get_size(mpmg, NDI_E_BOUNDS, &ierr);
   if (ierr) 
        printf("\n  **Error %d getting size of ebounds.\n", ierr );
   printf("  Ebounds size:  %d\n", flt_vec_size);
   
   flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
   ierr = ndi2_get_float64_vec(mpmg, NDI_E_BOUNDS, flt64_vec, flt_vec_size);
   if (ierr) 
        printf("\n  **Error %d getting energy boundaries.\n", ierr );
   print_float64_vec(flt64_vec, flt_vec_size);
   if (flt64_vec) {
        free(flt64_vec);
   }

   /* TOTAL CROSS SECTION */
    flt_vec_size = ndi2_get_size(mpmg, NDI_SIG_TOT, &ierr);
   if (ierr) 
        printf("\n  **Error %d getting size of total cross section.\n", ierr );
   printf("  Total cross-section size:  %d\n", flt_vec_size);
   
   flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
   ierr = ndi2_get_float64_vec(mpmg, NDI_SIG_TOT, flt64_vec, flt_vec_size);
   if (ierr) 
        printf("\n  **Error %d getting total cross section.\n", ierr );
   print_float64_vec(flt64_vec, flt_vec_size);
   if (flt64_vec) {
        free(flt64_vec);
   }

   /* SCATTERING MATRIX */
    flt_vec_size = ndi2_get_size_n(mpmg, NDI_PN_FULL, 0, &ierr);
   if (ierr) 
        printf("\n  **Error %d getting size of scattering matrix.\n", ierr );
   printf("  Scatter matrix size:  %d\n", flt_vec_size);
   
   flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
   ierr = ndi2_get_float64_vec_n(mpmg, NDI_PN_FULL, 0, flt64_vec, flt_vec_size);
   if (ierr) 
        printf("\n  **Error %d getting scattering matrix.\n", ierr );
   print_float64_vec(flt64_vec, flt_vec_size);
   if (flt64_vec) {
        free(flt64_vec);
   }
   

   /* CLOSE */
   ierr = ndi2_close_gendir(gendir);
   if (ierr)
     printf("  **Error %d in closing NDI 0.\n\n", ierr );

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
