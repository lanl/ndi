#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"


typedef float float32;
typedef double float64;

int kerma_example(void);
static void print_float64_vec( float64 *flt64_ptr, int num_elements );

#ifdef __STANDALONE
int main ()
{
  int ierr = 0;

  ierr = kerma_example();
  return(ierr);
}
#endif  /* __STANDALONE */

int kerma_example(void)
{
   int ierr;
   int num_grps;
   int handle = 0;
   float64 heat[100];
   float64 kerma[100];


  printf("\n\n  NDI TEST SUITE -- kerma_example\n\n");

   /* Setup */
   ierr = ndi2_open(&handle, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );

   ierr = ndi2_set_option(handle, NDI_LIB_TYPE, "multigroup_neutron" );
   if (ierr)
     printf("  **Error %d in setting library type.\n\n", ierr );

   ierr = ndi2_set_option(handle, NDI_LIBRARY, "mendf6" );
   if (ierr)
     printf("  **Error %d in setting library.\n\n", ierr );

   ierr = ndi2_set_option(handle, NDI_COLLAPSE, "16_group" );
   if (ierr)
     printf("  **Error %d in setting energy-group structure.\n\n", ierr );

   /* Isotope U-235 */
   ierr = ndi2_set_isotope(handle, "u235" );
   if (ierr) 
     printf("\n  **Error %d in setting isotope u235.\n", ierr );

   /* Get number of groups for later use */
   ierr = ndi2_get_int_val(handle, NDI_NUM_GRPS, &num_grps );
   if (ierr) 
     printf("\n  **Error %d in getting number of groups for isotope u235.\n", ierr );

   /* Total heating -- in units MeV/collision*/
   ierr = ndi2_get_float64_vec(handle, NDI_HEATING, heat, 100 );
   print_float64_vec ( heat, num_grps );
   if (ierr) 
     printf("\n  **Error %d in getting heating for isotope u235.\n", ierr );

   /* Total kerma -- in units MeV-barns (heating * total cross section) */
   ierr = ndi2_get_float64_vec(handle, NDI_KERMA, kerma, 100 );
   print_float64_vec ( kerma, num_grps );
   if (ierr) 
     printf("\n  **Error %d in getting kerma for isotope u235.\n", ierr );

   ierr = ndi2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );

   return(ierr);
}



/**********************************************************************/

static void print_float64_vec( float64 *flt64_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 4 ) == 0) )
	 printf("\n");
      printf("%22.15e  ", flt64_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/
