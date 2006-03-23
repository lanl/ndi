#include <stdio.h>
#include <stdlib.h>
#include "ndi.h"

static void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );

int main() {

    int ierr;
    int vec_size;
    ndi_float64 *ret_vec64 = NULL;

    ierr = ndi_init();
    ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
    ierr = ndi_set_option( NDI_LIBRARY, "mendf6" );
    ierr = ndi_set_option( NDI_COLLAPSE, "16_lanl2");
    ierr = ndi_set_isotope("u235");

    vec_size = ndi_get_size(NDI_E_AVG, &ierr);
    ret_vec64 = (ndi_float64 *)malloc(vec_size * sizeof(ndi_float64));
    

    /* Use default average energy */
    ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
    if (ierr) {
        printf("  Error %d getting average energy (default) for isotope u235.\n", ierr );
    }
    printf("\n\nDefault average energy:\n");
    print_float64_vec(ret_vec64, vec_size);

    
    /* Use calculation of average energy from average velocity (same as default) */
    ierr = ndi_set_option(NDI_E_AVG, "from_vel");
    ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
    if (ierr) {
        printf("  Error %d getting average energy (from_vel) for isotope u235.\n", ierr );
    }
    printf("\n\nAverage energy from velocity calc:\n");
     print_float64_vec(ret_vec64, vec_size);
   
    
    /* Use calculation of average energy from mean of bin boundary energies */
    /* This is the old default; use this to match values obtained from NDI before release 1.3.0 */
    ierr = ndi_set_option(NDI_E_AVG, "from_e");
    ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
    if (ierr) {
        printf("  Error %d getting average energy (from_e) for isotope u235.\n", ierr );
    }
    printf("\n\nAverage energy from mean of energy bounds:\n");
    print_float64_vec(ret_vec64, vec_size);

    
    /* Use legacy calculation of average energy */
    ierr = ndi_set_option(NDI_E_AVG, "legacy_calc");
    ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
    if (ierr) {
        printf("  Error %d getting average energy (legacy_calc) for isotope u235.\n", ierr );
    }
    printf("\n\nAverage energy from legacy calc:\n");
    print_float64_vec(ret_vec64, vec_size);

    
    /* Use SGNDF values of average energy */
    ierr = ndi_set_option(NDI_E_AVG, "from_sgndf");
    ierr = ndi_get_float64_vec(NDI_E_AVG, ret_vec64, vec_size);
    if (ierr) {
        printf("  Error %d getting average energy (from_sgndf) for isotope u235.\n", ierr );
    }
    printf("\n\nAverage energy from SGNDF:\n");
    print_float64_vec(ret_vec64, vec_size);

    ierr = ndi_close();

    return(ierr);
}

/**********************************************************************/

static void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 4 ) == 0) )
	 printf("\n");
      printf("%15.6e  ", flt64_ptr[n]);
   }
   printf("\n");
}
/**********************************************************************/



