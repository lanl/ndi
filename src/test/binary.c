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
   int gendir_handle;
   int neutron_handle;
   int handle[3];
   int return_int = 0;
   int int_vec_size = 0;
   int *int_vec = NULL;
   int flt_vec_size;
   int pgrps = 2;
   double return_float = 0;
   char zaid[80];
   ndi_float64 pbounds[] = { 20.0, 4.0, 0.1 };
   ndi_float64 *flt64_vec = NULL;

   /* Inintialize gendir */
   ierr = NDI2_open_gendir(&gendir_handle, "../../src/test/gendir");
   if (ierr)
     printf("  **Error %d in NDI open gendir.\n\n", ierr );

   /* Get neutron handle */
   ierr = NDI2_make_handle(gendir_handle, "multigroup_neutron", &neutron_handle);
   if (ierr)
     printf("  **Error %d in NDI make_handle.\n\n", ierr );
   ierr = NDI2_get_string_val(gendir_handle, NDI_GENDIR_PATH, zaid, 80);

   /*  Initialize binary library */
   ierr = NDI2_set_option(neutron_handle, NDI_LIBRARY, "mtmg01");
   ierr = NDI2_set_option(neutron_handle, NDI_COLLAPSE, "2_lanl");
   ierr = NDI2_set_isotope(neutron_handle, "92235.141nm:b");

   /* start accessing data */

   printf("\n Start with binary data \n\n");

   ierr = NDI2_get_string_val(neutron_handle, NDI_ZAID, zaid, 80);
   if (ierr) 
        printf("\n  **Error %d getting number of groups.\n", ierr );
   printf("  Isotope zaid:  %s\n",zaid);

   ierr = NDI2_get_int_val(neutron_handle, NDI_NUM_GRPS, &return_int);
   if (ierr) 
        printf("\n  **Error %d getting number of groups.\n", ierr );
   printf("  Number of groups:  %d\n", return_int);

   ierr = NDI2_get_int_val(neutron_handle, NDI_PN_ORDER, &return_int);
   if (ierr) 
        printf("\n  **Error %d getting pn order.\n", ierr );
   printf("  Pn order:  %d\n", return_int);

   ierr = NDI2_get_int_val(neutron_handle, NDI_NUM_PNU_PR, &return_int);
   if (ierr) 
        printf("\n  **Error %d getting num pnu.\n", ierr );
   printf("  No. pnu:  %d\n", return_int);

   ierr = NDI2_get_int_val(neutron_handle, NDI_NUM_REAC, &return_int);
   if (ierr) 
        printf("\n  **Error %d getting num reactions.\n", ierr );
   printf("  No. reactions:  %d\n", return_int);

   ierr = NDI2_get_float64_val(neutron_handle, NDI_AT_WGT, &return_float);
   if (ierr)
        printf("\n  **Error %d getting num reactions.\n", ierr );
   printf("  Atomic weight:  %lf\n", return_float);

   /* LIST OF MTS */
   int_vec_size = NDI2_get_size(neutron_handle, NDI_REAC_MT, &ierr); 
   if (ierr) 
        printf("\n  **Error %d getting size of reaction MTs.\n", ierr );
        printf("  Reaction MTs size:  %d\n", int_vec_size); 
   
   int_vec = (int *)calloc(int_vec_size, sizeof(int));
   ierr = NDI2_get_int_vec(neutron_handle, NDI_REAC_MT, int_vec, int_vec_size);
   if (ierr) 
        printf("\n  **Error %d getting reaction MTs.\n", ierr );
   print_int_vec(int_vec, int_vec_size);
   if (int_vec) {
        free(int_vec);
   }

   /* EBOUNDS */
   flt_vec_size = NDI2_get_size(neutron_handle, NDI_E_BOUNDS, &ierr);
   if (ierr) 
        printf("\n  **Error %d getting size of ebounds.\n", ierr );
   printf("  Ebounds size:  %d\n", flt_vec_size);
   
   flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
   ierr = NDI2_get_float64_vec(neutron_handle, NDI_E_BOUNDS, flt64_vec, flt_vec_size);
   if (ierr) 
        printf("\n  **Error %d getting energy boundaries.\n", ierr );
   print_float64_vec(flt64_vec, flt_vec_size);
   if (flt64_vec) {
        free(flt64_vec);
   }

   /* TOTAL CROSS SECTION */
    flt_vec_size = NDI2_get_size(neutron_handle, NDI_SIG_TOT, &ierr);
   if (ierr) 
        printf("\n  **Error %d getting size of total cross section.\n", ierr );
   printf("  Total cross-section size:  %d\n", flt_vec_size);
   
   flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
   ierr = NDI2_get_float64_vec(neutron_handle, NDI_SIG_TOT, flt64_vec, flt_vec_size);
   if (ierr) 
        printf("\n  **Error %d getting total cross section.\n", ierr );
   print_float64_vec(flt64_vec, flt_vec_size);
   if (flt64_vec) {
        free(flt64_vec);
   }

   /* SCATTERING MATRIX */
    flt_vec_size = NDI2_get_size_n(neutron_handle, NDI_PN_FULL, 0, &ierr);
   if (ierr) 
        printf("\n  **Error %d getting size of scattering matrix.\n", ierr );
   printf("  Scatter matrix size:  %d\n", flt_vec_size);
   
   flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
   ierr = NDI2_get_float64_vec_n(neutron_handle, NDI_PN_FULL, 0, flt64_vec, flt_vec_size);
   if (ierr) 
        printf("\n  **Error %d getting scattering matrix.\n", ierr );
   print_float64_vec(flt64_vec, flt_vec_size);
   if (flt64_vec) {
        free(flt64_vec);
   }

   /* Repeat with ASCII data */

   printf("\n\n Try with ASCII data\n\n");

   ierr = NDI2_set_isotope(neutron_handle, "92235.121nm");

   /* start accessing data */
   ierr = NDI2_get_string_val(neutron_handle, NDI_ZAID, zaid, 80);
   if (ierr)
        printf("\n  **Error %d getting number of groups.\n", ierr );
   printf("  Isotope zaid:  %s\n",zaid);

   ierr = NDI2_get_int_val(neutron_handle, NDI_NUM_GRPS, &return_int);
   if (ierr)
        printf("\n  **Error %d getting number of groups.\n", ierr );
   printf("  Number of groups:  %d\n", return_int);

   ierr = NDI2_get_int_val(neutron_handle, NDI_PN_ORDER, &return_int);
   if (ierr)
        printf("\n  **Error %d getting pn order.\n", ierr );
   printf("  Pn order:  %d\n", return_int);

   ierr = NDI2_get_int_val(neutron_handle, NDI_NUM_PNU_PR, &return_int);
   if (ierr)
        printf("\n  **Error %d getting num pnu.\n", ierr );
   printf("  No. pnu:  %d\n", return_int);

   ierr = NDI2_get_int_val(neutron_handle, NDI_NUM_REAC, &return_int);
   if (ierr)
        printf("\n  **Error %d getting num reactions.\n", ierr );
   printf("  No. reactions:  %d\n", return_int);

   ierr = NDI2_get_float64_val(neutron_handle, NDI_AT_WGT, &return_float);
   if (ierr)
        printf("\n  **Error %d getting num reactions.\n", ierr );
   printf("  Atomic weight:  %lf\n", return_float);

   /* LIST OF MTS */
   int_vec_size = NDI2_get_size(neutron_handle, NDI_REAC_MT, &ierr);
   if (ierr)
        printf("\n  **Error %d getting size of reaction MTs.\n", ierr );
        printf("  Reaction MTs size:  %d\n", int_vec_size);

   int_vec = (int *)calloc(int_vec_size, sizeof(int));
   ierr = NDI2_get_int_vec(neutron_handle, NDI_REAC_MT, int_vec, int_vec_size);
   if (ierr)
        printf("\n  **Error %d getting reaction MTs.\n", ierr );
   print_int_vec(int_vec, int_vec_size);
   if (int_vec) {
        free(int_vec);
   }

   /* EBOUNDS */
   flt_vec_size = NDI2_get_size(neutron_handle, NDI_E_BOUNDS, &ierr);
   if (ierr)
        printf("\n  **Error %d getting size of ebounds.\n", ierr );
   printf("  Ebounds size:  %d\n", flt_vec_size);

   flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
   ierr = NDI2_get_float64_vec(neutron_handle, NDI_E_BOUNDS, flt64_vec, flt_vec_size);
   if (ierr)
        printf("\n  **Error %d getting energy boundaries.\n", ierr );
   print_float64_vec(flt64_vec, flt_vec_size);
   if (flt64_vec) {
        free(flt64_vec);
   }

   /* TOTAL CROSS SECTION */
    flt_vec_size = NDI2_get_size(neutron_handle, NDI_SIG_TOT, &ierr);
   if (ierr)
        printf("\n  **Error %d getting size of total cross section.\n", ierr );
   printf("  Total cross-section size:  %d\n", flt_vec_size);

   flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
   ierr = NDI2_get_float64_vec(neutron_handle, NDI_SIG_TOT, flt64_vec, flt_vec_size);
   if (ierr)
        printf("\n  **Error %d getting total cross section.\n", ierr );
   print_float64_vec(flt64_vec, flt_vec_size);
   if (flt64_vec) {
        free(flt64_vec);
   }

   /* SCATTERING MATRIX */
    flt_vec_size = NDI2_get_size_n(neutron_handle, NDI_PN_FULL, 0, &ierr);
   if (ierr)
        printf("\n  **Error %d getting size of scattering matrix.\n", ierr );
   printf("  Scatter matrix size:  %d\n", flt_vec_size);

   flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
   ierr = NDI2_get_float64_vec_n(neutron_handle, NDI_PN_FULL, 0, flt64_vec, flt_vec_size);
   if (ierr)
        printf("\n  **Error %d getting scattering matrix.\n", ierr );
   print_float64_vec(flt64_vec, flt_vec_size);
   if (flt64_vec) {
        free(flt64_vec);
   }
   

   /* CLOSE */
   ierr = NDI2_close_gendir(gendir_handle);
   if (ierr)
     printf("  **Error %d in closing NDI 0.\n", ierr );

   printf("\n Closing. \n\n");

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
