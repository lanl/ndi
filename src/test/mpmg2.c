#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ndi.h"

#define ZAID_SIZE   24

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );
void print_float32_vec( ndi_float32 *flt_ptr, int num_elements );
void print_int_vec( int *int_ptr, int num_elements );


int main ()

{
   int ierr;
   int i;
   int gendir_handle;
   int data_handle;
   int return_int = 0;
   int int_vec_size = 0;
   int *int_vec = NULL;
   int flt_vec_size;
   int handle0, handle1;
   int num_libraries = 3;
   char *library[] = {"ngk6", "m6_ng", "acti_ng" };
   char part_zaid[ZAID_SIZE];
   ndi_float64 *flt64_vec = NULL;

   for (i = 0; i < num_libraries; i++) {
       /* Inintialize gendir */
       ierr = NDI2_open_gendir(&gendir_handle, "");
       if (ierr)
           printf("  **Error %d in NDI open 0.\n\n", ierr );

       /* Get mpmg handle */
       ierr = NDI2_make_handle(gendir_handle, "multigroup_multi", 
			       &data_handle);
       if (ierr)
           printf("  **Error %d in NDI make_handle 0.\n\n", ierr );

       printf("LIBRARY: %s\n", library[i]);
       ierr = NDI2_set_option(data_handle, NDI_LIBRARY, library[i]);
       ierr = NDI2_set_zaid(data_handle, "92235");

       /* test getting zaid back */
       int_vec_size = NDI2_get_size(data_handle, NDI_ZAID, &ierr); 
       if (ierr) 
           printf("\n  **Error %d getting size of ZAID.\n", ierr );
       ierr = NDI2_get_string_val(data_handle, NDI_ZAID, part_zaid, ZAID_SIZE);
       if  (ierr != 0)
           printf(" ***Error getting data_handle zaid is %d\n", ierr);
       if (strncmp(part_zaid, "92235", 5) != 0)
	   printf(" ***Error matching mpmg zaid\n");

       /* start accessing data */
       ierr = NDI2_get_int_val(data_handle, NDI_NUM_GRPS, &return_int);
       if (ierr) 
           printf("\n  **Error %d getting number of groups.\n", ierr );
       printf("  Number of groups:  %d\n", return_int);

       ierr = NDI2_get_int_val(data_handle, NDI_PN_ORDER, &return_int);
       if (ierr) 
           printf("\n  **Error %d getting pn order.\n", ierr );
       printf("  Pn order:  %d\n", return_int);

       ierr = NDI2_get_int_val(data_handle, NDI_NUM_PNU_PR, &return_int);
       if (ierr) 
           printf("\n  **Error %d getting num pnu.\n", ierr );
       printf("  No. pnu:  %d\n", return_int);

       ierr = NDI2_get_int_val(data_handle, NDI_NUM_REAC, &return_int);
       if (ierr) 
           printf("\n  **Error %d getting num reactions.\n", ierr );
       printf("  No. reactions:  %d\n", return_int);

       /* LAST NEUTRON AND PHOTON GRPS */
       ierr = NDI2_get_int_val_n(data_handle, NDI_LAST_GRP, 0, &return_int);
       if (ierr) 
           printf("\n  **Error %d getting last neutron group.\n", ierr );
       printf("  Last neutron group:  %d\n", return_int);
       ierr = NDI2_get_int_val_n(data_handle, NDI_LAST_GRP, 1, &return_int);
       if (ierr) 
           printf("\n  **Error %d getting last photon.\n", ierr );
       printf("  Last photon group:  %d\n", return_int);

 
       /* LIST OF MTS */
       int_vec_size = NDI2_get_size(data_handle, NDI_REAC_MT, &ierr); 
       if (ierr) 
           printf("\n  **Error %d getting size of reaction MTs.\n", ierr );
       printf("  Reaction MTs size:  %d\n", int_vec_size); 
   
       int_vec = (int *)calloc(int_vec_size, sizeof(int));
       ierr = NDI2_get_int_vec(data_handle, NDI_REAC_MT, int_vec, int_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting reaction MTs.\n", ierr );
       print_int_vec(int_vec, int_vec_size);
       if (int_vec) {
           free(int_vec);
       }


       /* EBOUNDS */
       flt_vec_size = NDI2_get_size(data_handle, NDI_E_BOUNDS, &ierr);
       if (ierr) 
           printf("\n  **Error %d getting size of ebounds.\n", ierr );
       printf("  Ebounds size:  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec(data_handle, NDI_E_BOUNDS, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting energy boundaries.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* AVERAGE BIN ENERGY */
       flt_vec_size = NDI2_get_size(data_handle, NDI_E_AVG, &ierr);
       if (ierr) 
           printf("\n  **Error %d getting size of e_avg.\n", ierr );
       printf("  Eavg size:  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec(data_handle, NDI_E_AVG, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting average energies.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* WEIGHTS */
       flt_vec_size = NDI2_get_size(data_handle, NDI_WGTS, &ierr);
       if (ierr) 
           printf("\n  **Error %d getting size of weigts.\n", ierr );
       printf("  Weights size:  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec(data_handle, NDI_WGTS, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting weights.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* VELOCITIES*/
       flt_vec_size = NDI2_get_size(data_handle, NDI_VEL, &ierr);
       if (ierr) 
           printf("\n  **Error %d getting size of velocities.\n", ierr );
       printf("  Velocity size:  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec(data_handle, NDI_VEL, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting velocities.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* TOTAL CROSS SECTION */
       flt_vec_size = NDI2_get_size(data_handle, NDI_SIG_TOT, &ierr);
       if (ierr) 
           printf("\n  **Error %d getting size of total cross section.\n", ierr );
       printf("  Total cross-section size:  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec(data_handle, NDI_SIG_TOT, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting total cross section.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* CHI VECTOR */
       flt_vec_size = NDI2_get_size(data_handle, NDI_CHI_VEC_PR, &ierr);
       if (ierr) 
           printf("\n  **Error %d getting size of prompt chi vector.\n", ierr );
       printf("  Prompt chi vector size:  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec(data_handle, NDI_CHI_VEC_PR, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting prompt chi vector.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* NUBAR */
       flt_vec_size = NDI2_get_size(data_handle, NDI_NU_PR, &ierr);
       if (ierr)
           printf("\n  **Error %d getting size of prompt nubar.\n", ierr );
       printf("  Prompt nubar size:  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec(data_handle, NDI_NU_PR, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting prompt nubar.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* NU_SIGF */
       flt_vec_size = NDI2_get_size(data_handle, NDI_NU_SIG_F_PR, &ierr);
       if (ierr) 
           printf("\n  **Error %d getting size of prompt nu_sigf.\n", ierr );
       printf("  Prompt nu*sigf size:  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec(data_handle, NDI_NU_SIG_F_PR, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting prompt nu_sigf.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* PNU PROMT */
       flt_vec_size = NDI2_get_size_n(data_handle, NDI_PNU_PR, 2, &ierr);
       if (ierr) 
           printf("\n  **Error %d getting size of prompt pnu.\n", ierr );
       printf("  Prompt pnu size (2):  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec_n(data_handle, NDI_PNU_PR, 2, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting prompt pnu.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* SIG*PNU PROMT */
       flt_vec_size = NDI2_get_size_n(data_handle, NDI_SIG_PNU_PR, 2, &ierr);
       if (ierr) 
           printf("\n  **Error %d getting size of prompt sig*pnu.\n", ierr );
       printf("  Prompt sig*pnu size (2):  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec_n(data_handle, NDI_SIG_PNU_PR, 2, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting prompt sig*pnu.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* REACTION CROSS SECTION (MT=2) */
       flt_vec_size = NDI2_get_size_n(data_handle, NDI_SIG_REAC, 2, &ierr);
       if (ierr) 
           printf("\n  **Error %d getting size of reaction cross section.\n", ierr );
       printf("  Reaction cross section size (mt=2):  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec_n(data_handle, NDI_SIG_REAC, 2, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting reaction cross section.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* DIAGONAL TRANSPORT CORRECTION (2) */
       flt_vec_size = NDI2_get_size_n(data_handle, NDI_CORR_DIAG, 2, &ierr);
       if (ierr) 
           printf("\n  **Error %d getting size of diagonal transport correction.\n", ierr );
       printf("  Diagonal transport correction size (2):  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec_n(data_handle, NDI_CORR_DIAG, 2, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting diagonal transport correction.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* BHS TRANSPORT CORRECTION (2) */
       flt_vec_size = NDI2_get_size_n(data_handle, NDI_CORR_BHS, 2, &ierr);
       if (ierr) 
           printf("\n  **Error %d getting size of BHS transport correction.\n", ierr );
       printf("  BHS transport correction size (2):  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec_n(data_handle, NDI_CORR_BHS, 2, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting BHS transport correction.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* SCATTERING MATRIX */
       flt_vec_size = NDI2_get_size_n(data_handle, NDI_PN_FULL, 0, &ierr);
       if (ierr) 
           printf("\n  **Error %d getting size of scattering matrix.\n", ierr );
       printf("  Scatter matrix size:  %d\n", flt_vec_size);
   
       flt64_vec = (ndi_float64 *)calloc(flt_vec_size, sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec_n(data_handle, NDI_PN_FULL, 0, flt64_vec, flt_vec_size);
       if (ierr) 
           printf("\n  **Error %d getting scattering matrix.\n", ierr );
       print_float64_vec(flt64_vec, flt_vec_size);
       if (flt64_vec) {
           free(flt64_vec);
       }

       /* Access single-particle data */
       ierr = NDI2_get_int_val_n(data_handle, NDI_PARTICLE_N, 0, &handle0);
       ierr = NDI2_get_string_val(handle0, NDI_ZAID, part_zaid, ZAID_SIZE);
       if  (ierr != 0)
           printf(" ***Error getting handle #0 is %d\n", ierr);
       printf("  ZAID for particle #0:  %s\n", part_zaid);

       ierr = NDI2_get_int_val_n(data_handle, NDI_PARTICLE_N, 1, &handle1);
       ierr = NDI2_get_string_val(handle1, NDI_ZAID, part_zaid, ZAID_SIZE);
       if  (ierr != 0)
           printf(" ***Error getting handle #1 is %d\n", ierr);
       printf("  ZAID for particle #1:  %s\n", part_zaid);

       printf("\n\n");
       
       /* CLOSE */
       ierr = NDI2_close_gendir(gendir_handle);
       if (ierr)
           printf("  **Error %d in closing NDI 0.\n\n", ierr );
   }

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
