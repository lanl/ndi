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
    int vec_size;
 
   ndi_float64 *ret_vec64 = NULL;


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

   /* Average energies as e = 1/2 m*vel^2 */
   /* No collapsing */
   ierr = ndi_set_option(NDI_COLLAPSE, "default");
   ierr = ndi_set_option(NDI_E_AVG, "from_vel");
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


   /* Average energies as arithmetic mean of boundary values */
   /* No collapsing */
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
