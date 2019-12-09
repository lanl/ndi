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
   int ierr, i;
   int vec_size;
 
   ndi_float64 *ret_vec64 = NULL;
   ndi_float64 *ret_vec = NULL; 

   /*  Initialize */
   ierr = ndi_init();
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi_set_option( NDI_LIBRARY, "mendf6" );

   printf(" ************************************************************************************ \n");
   printf("    MULTIGROUP NEUTRON DATA AVERAGE ENERGY TEST\n" );
   printf(" ************************************************************************************ \n" );

   /* Set isotope */
   ierr = ndi_set_isotope("u235");
   if (ierr) 
       printf("\n  **Error %d in setting isotope u235.\n", ierr );

   /*  Default energy averaging */
   /* No collapsing */
   printf("******** U235:  Default average bin energy; no collapse ********\n");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (default) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }
   
   /* Collapsing */
   printf("******** U235:  Default average bin energy; 16_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "16_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (default) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

   
   /* Set collapse structure to same as default */
   printf("******** U235:  Default average bin energy; 30_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "30_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (default) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

   /* Average energies as e = 1/2 m*vel^2 */
   /* No collapsing */
   printf("******** U235:  Velocity calculation; no collapse ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "default");
   ierr = ndi_set_option(NDI_E_AVG, "from_vel");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (from vel) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }
   
   /* Collapsing */
   printf("******** U235:  Velocity calculation; 16_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "16_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (from vel) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

   /* Set collapse structure to same as default */
   printf("******** U235:  Velocity calculation; 30_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "30_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (from vel) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }


   /* Average energies as arithmetic mean of boundary values */
   /* No collapsing */
   printf("******** U235:  Mean of boundary energies; no collapse ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "default");
   ierr = ndi_set_option(NDI_E_AVG, "from_e");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (arithmetic mean) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }
   
   /* Collapsing */
   printf("******** U235:  Mean of boundary energies; 16_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "16_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (arithmetic mean) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }
   
   /*  Set collapse structure to same as default */
   printf("******** U235:  Mean of boundary energies; 30_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "30_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (arithmetic mean) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }


   /* SGNDF average energies  */
   /* No collapsing */
   printf("******** U235:  SGNDF energies; no collapse ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "default");
   ierr = ndi_set_option(NDI_E_AVG, "from_sgndf");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (SGNDF) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }
   
   /* Collapsing */
   printf("******** U235:  SGNDF energies; 16_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "16_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (SGNDF) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

   
   /*  Set collapse structure to same as default */
   printf("******** U235:  SGNDF energies; 30_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "30_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (SGNDF) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

 /*  Collapse to 16_lanl2 */
  printf("******** U235:  SGNDF energies; 16_lanl2 ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "16_lanl2");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (SGNDF) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

   /* Average energies from legacy calculation */
   /* No collapsing */
   printf("******** U235:  Legacy calculation; no collapse ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "default");
   ierr = ndi_set_option(NDI_E_AVG, "legacy_calc");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (legacy calculation) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }
   
   /* Collapsing */
   printf("******** U235:  Legacy calculation; 16_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "16_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (legacy calculation) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

   
   /* Set collapse structure to same as default */
   printf("******** U235:  Legacy calculation; 30_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "30_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (legacy calculation) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

   /* Average energies from flat calculation (reqt 35)*/
   /* No collapsing */
   printf("******** U235:  Flat calculation; no collapse ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "default");
   ierr = ndi_set_option(NDI_E_AVG, "from_flat");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (flat calculation) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }
   
   /* Collapsing */
   printf("******** U235:  Flat calculation; 16_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "16_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (flat calculation) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

   
   /* Set collapse structure to same as default */
   printf("******** U235:  Flat calculation; 30_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "30_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (flat calculation) for isotope u235.\n", ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

   /* TD average energies  (reqt 35) */
   /* No collapsing */
   printf("******** U235:  TD energies; no collapse ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "default");
   ierr = ndi_set_option(NDI_E_AVG, "from_td");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (TD) for isotope u235.\n",
              ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD half moment (m=2/2) (reqt 83) */ 
   ret_vec = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec_n(NDI_TD_HALF_MOMENT, 2, 
                                ret_vec, vec_size);
   
   if (ierr) {
       printf("  Error %d getting second half moment for isotope u235.\n",
              ierr );
   }

   printf("Average second half moment: \n");
   print_float64_vec( ret_vec, vec_size );	 
   printf("\n");
   for (i = 0; i < vec_size; i++) {
       ret_vec[i] = (ret_vec[i]/ret_vec64[i] - 1) * 100.0;
   }
   printf("Relative %% difference:\n");
   print_float64_vec(ret_vec, vec_size);
   printf("\n");

   /* TD average velocities (reqt 67) */
   ierr = ndi_get_float64_vec(NDI_V_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average velocity (TD) for isotope u235.\n",
              ierr );
   }

   printf("Average velocity: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD half moment (m = 1/2) (reqt 83) */
   ret_vec = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec_n(NDI_TD_HALF_MOMENT, 1, 
                                ret_vec, vec_size);
   
   if (ierr) {
       printf("  Error %d getting first half moment for isotope u235.\n",
              ierr );
   }

   printf("Average first half moment: \n");
   print_float64_vec( ret_vec, vec_size );	 
   printf("\n");
   for (i = 0; i < vec_size; i++) 
       ret_vec[i] = (ret_vec[i]/ret_vec64[i] - 1/13.83159325) * 100.0;
   printf("Relative %% difference:\n");
   print_float64_vec(ret_vec, vec_size);
   printf("\n");

   /* TD weights */
   ierr = ndi_get_float64_vec(NDI_WGTS, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting TD weights for isotope u235.\n",
              ierr );
   }

   printf("Weights: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD half moment (m = 0/2) (reqt 83) */
   ret_vec = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec_n(NDI_TD_HALF_MOMENT, 0, 
                                ret_vec, vec_size);
   
   if (ierr) {
       printf("  Error %d getting zeroth half moment for isotope u235.\n",
              ierr );
   }

   printf("Average zeroth half moment: \n");
   print_float64_vec( ret_vec, vec_size );	 
   printf("\n");
   for (i = 0; i < vec_size; i++) 
       ret_vec[i] = (ret_vec[i]/ret_vec64[i] - 1) * 100.0;
   printf("Relative %% difference:\n");
   print_float64_vec(ret_vec, vec_size);
   printf("\n");

   /* 1/<1/v> */
   ierr = ndi_get_float64_vec(NDI_VEL, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average velocity (TD) for isotope u235.\n",
              ierr );
   }

   printf("Inverse average inverse velocity: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD half moment (-1/2) (reqt 83) */
   ret_vec = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec_n(NDI_TD_HALF_MOMENT, -1, 
                                ret_vec, vec_size);
   
   if (ierr) {
       printf("  Error %d getting first half moment for isotope u235.\n",
              ierr );
   }

   printf("Average minus first half moment: \n");
   print_float64_vec( ret_vec, vec_size );	 
   printf("\n");
   for (i = 0; i < vec_size; i++) 
       ret_vec[i] = (ret_vec[i]*ret_vec64[i] - 13.83159325) * 100.0;
   printf("Relative %% difference:\n");
   print_float64_vec(ret_vec, vec_size);
   printf("\n");

   if (ret_vec) {
       free(ret_vec);
       ret_vec = NULL;
   }
   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }
   
   /* Collapsing */
   printf("******** U235:  TD energies; 16_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "16_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (TD) for isotope u235.\n",
              ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD half moment (reqt 83) */
   ret_vec = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec_n(NDI_TD_HALF_MOMENT, 2, 
                                ret_vec, vec_size);
   
   if (ierr) {
       printf("  Error %d getting second half moment for isotope u235.\n",
              ierr );
   }

   printf("Average second half moment: \n");
   print_float64_vec( ret_vec, vec_size );	 
   printf("\n");
   for (i = 0; i < vec_size; i++) 
       ret_vec[i] = (ret_vec[i]/ret_vec64[i] - 1) * 100.0;
   printf("Relative %% difference:\n");
   print_float64_vec(ret_vec, vec_size);
   printf("\n");
   /**/

   /* TD average velocities (reqt 67) */
   ierr = ndi_get_float64_vec(NDI_V_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average velocity (TD) for isotope u235.\n",
              ierr );
   }

   printf("Average velocity: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD half moment (reqt 83) */ 
   ret_vec = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec_n(NDI_TD_HALF_MOMENT, 1, 
                                ret_vec, vec_size);
   
   if (ierr) {
       printf("  Error %d getting first half moment for isotope u235.\n",
              ierr );
   }

   printf("Average first half moment: \n");
   print_float64_vec( ret_vec, vec_size );	 
   printf("\n");
   for (i = 0; i < vec_size; i++) 
       ret_vec[i] = (ret_vec[i]/ret_vec64[i] - 1/13.83159325) * 100.0;
   printf("Relative %% difference:\n");
   print_float64_vec(ret_vec, vec_size);
   printf("\n");

   /* TD weights */
   ierr = ndi_get_float64_vec(NDI_WGTS, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting TD weights for isotope u235.\n",
              ierr );
   }

   printf("Weights: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD half moment (m = 0/2) (reqt 83) */
   ret_vec = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec_n(NDI_TD_HALF_MOMENT, 0, 
                                ret_vec, vec_size);
   
   if (ierr) {
       printf("  Error %d getting zeroth half moment for isotope u235.\n",
              ierr );
   }

   printf("Average zeroth half moment: \n");
   print_float64_vec( ret_vec, vec_size );	 
   printf("\n");
   for (i = 0; i < vec_size; i++) 
       ret_vec[i] = (ret_vec[i]/ret_vec64[i] - 1) * 100.0;
   printf("Relative %% difference:\n");
   print_float64_vec(ret_vec, vec_size);
   printf("\n");

   /* 1/<1/v> */
   ierr = ndi_get_float64_vec(NDI_VEL, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average velocity (TD) for isotope u235.\n",
              ierr );
   }

   printf("Inverse average inverse velocity: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD half moment (-1/2) (reqt 83) */
   ret_vec = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec_n(NDI_TD_HALF_MOMENT, -1, 
                                ret_vec, vec_size);
   
   if (ierr) {
       printf("  Error %d getting first half moment for isotope u235.\n",
              ierr );
   }

   printf("Average minus first half moment: \n");
   print_float64_vec( ret_vec, vec_size );	 
   printf("\n");
   for (i = 0; i < vec_size; i++) 
       ret_vec[i] = (ret_vec[i]*ret_vec64[i] - 13.83159325) * 100.0;
   printf("Relative %% difference:\n");
   print_float64_vec(ret_vec, vec_size);
   printf("\n");

   if (ret_vec) {
       free(ret_vec);
       ret_vec = NULL;
   }
   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

   
   /*  Set collapse structure to same as default */
   printf("******** U235:  TD energies; 30_lanl ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "30_lanl");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (TD) for isotope u235.\n",
              ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD average velocities (reqt 67) */
   ierr = ndi_get_float64_vec(NDI_V_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average velocity (TD) for isotope u235.\n",
              ierr );
   }

   printf("Average velocity: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

 /*  Collapse to 16_lanl2 */
  printf("******** U235:  TD energies; 16_lanl2 ********\n");
   ierr = ndi_set_option(NDI_COLLAPSE, "16_lanl2");
   vec_size = ndi_get_size(NDI_E_AVG, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average energy (TD) for isotope u235.\n",
              ierr );
   }

   printf("Average energy: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD average velocities (reqt 67) */
   ierr = ndi_get_float64_vec(NDI_V_AVG, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average velocity (TD) for isotope u235.\n",
              ierr );
   }

   printf("Average velocity: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   if (ret_vec) {
       free(ret_vec);
       ret_vec = NULL;
   }
   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

   printf("**** U235 @ T=1eV:  TD half-moments; no collapse ****\n");

   /* Set isotope */
   ierr = ndi_set_isotope("92235.094nm");
   if (ierr) 
       printf("\n  **Error %d in setting isotope u235.\n", ierr );


   /* TD weights */
   vec_size = ndi_get_size(NDI_WGTS, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_WGTS, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting TD weights for isotope u235.\n",
              ierr );
   }

   printf("Weights: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD half moment (m = 0/2) (reqt 83) */
   ret_vec = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec_n(NDI_TD_HALF_MOMENT, 0, 
                                ret_vec, vec_size);
   
   if (ierr) {
       printf("  Error %d getting zeroth half moment for isotope u235.\n",
              ierr );
   }

   printf("Average zeroth half moment: \n");
   print_float64_vec( ret_vec, vec_size );	 
   printf("\n");
   for (i = 0; i < vec_size; i++) 
       ret_vec[i] = (ret_vec[i]/ret_vec64[i] - 1) * 100.0;
   printf("Relative %% difference:\n");
   print_float64_vec(ret_vec, vec_size);
   printf("\n");

   /* 1/<1/v> */
   ierr = ndi_get_float64_vec(NDI_VEL, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average velocity (TD) for isotope u235.\n",
              ierr );
   }

   printf("Inverse average inverse velocity: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD half moment (-1/2) (reqt 83) */
   ret_vec = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec_n(NDI_TD_HALF_MOMENT, -1, 
                                ret_vec, vec_size);
   
   if (ierr) {
       printf("  Error %d getting first half moment for isotope u235.\n",
              ierr );
   }

   printf("Average minus first half moment: \n");
   print_float64_vec( ret_vec, vec_size );	 
   printf("\n");
   for (i = 0; i < vec_size; i++) 
       ret_vec[i] = (ret_vec[i]*ret_vec64[i] - 13.83159325) * 100.0;
   printf("Relative %% difference:\n");
   print_float64_vec(ret_vec, vec_size);
   printf("\n");

   if (ret_vec) {
       free(ret_vec);
       ret_vec = NULL;
   }
   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

   printf("**** U235 @ T=1keV:  TD half-moments; no collapse ****\n");

   /* Set isotope */
   ierr = ndi_set_isotope("92235.098nm");
   if (ierr) 
       printf("\n  **Error %d in setting isotope u235.\n", ierr );


   /* TD weights */
   vec_size = ndi_get_size(NDI_WGTS, &ierr);
   ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec(NDI_WGTS, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting TD weights for isotope u235.\n",
              ierr );
   }

   printf("Weights: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD half moment (m = 0/2) (reqt 83) */
   ret_vec = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec_n(NDI_TD_HALF_MOMENT, 0, 
                                ret_vec, vec_size);
   
   if (ierr) {
       printf("  Error %d getting zeroth half moment for isotope u235.\n",
              ierr );
   }

   printf("Average zeroth half moment: \n");
   print_float64_vec( ret_vec, vec_size );	 
   printf("\n");
   for (i = 0; i < vec_size; i++) 
       ret_vec[i] = (ret_vec[i]/ret_vec64[i] - 1) * 100.0;
   printf("Relative %% difference:\n");
   print_float64_vec(ret_vec, vec_size);
   printf("\n");

   /* 1/<1/v> */
   ierr = ndi_get_float64_vec(NDI_VEL, ret_vec64, vec_size);
   if (ierr) {
       printf("  Error %d getting average velocity (TD) for isotope u235.\n",
              ierr );
   }

   printf("Inverse average inverse velocity: \n");
   print_float64_vec( ret_vec64, vec_size );	 
   printf("\n");

   /* TD half moment (-1/2) (reqt 83) */
   ret_vec = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
   ierr = ndi_get_float64_vec_n(NDI_TD_HALF_MOMENT, -1, 
                                ret_vec, vec_size);
   
   if (ierr) {
       printf("  Error %d getting first half moment for isotope u235.\n",
              ierr );
   }

   printf("Average minus first half moment: \n");
   print_float64_vec( ret_vec, vec_size );	 
   printf("\n");
   for (i = 0; i < vec_size; i++) 
       ret_vec[i] = (ret_vec[i]*ret_vec64[i] - 13.83159325) * 100.0;
   printf("Relative %% difference:\n");
   print_float64_vec(ret_vec, vec_size);
   printf("\n");

   if (ret_vec) {
       free(ret_vec);
       ret_vec = NULL;
   }
   if (ret_vec64) {
       free(ret_vec64);
       ret_vec64 = NULL;
   }

   ierr = ndi_close();
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
      printf("%25.12e   ", flt64_ptr[n]);
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
