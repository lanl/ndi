#include <stdio.h>
#include <stdlib.h>
#include "ndi.h"

static void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );

int main () {

    int ierr;
    int kerma_size;
    int heating_size;
    ndi_float64 *kerma = NULL;
    ndi_float64 *heating = NULL;

    ierr = ndi_init();
    ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
    ierr = ndi_set_option( NDI_LIBRARY, "mendf6" );
    ierr = ndi_set_option( NDI_COLLAPSE, "16_llnl");
    ierr = ndi_set_isotope("u235");

    kerma_size = ndi_get_size(NDI_KERMA, &ierr);
    kerma = (ndi_float64 *)malloc(kerma_size * sizeof(ndi_float64));
    heating_size = ndi_get_size(NDI_HEATING, &ierr);
    heating = (ndi_float64 *)malloc(heating_size * sizeof(ndi_float64));

    /* Use default kerma/heating */
    ierr = ndi_get_float64_vec(NDI_HEATING, heating, heating_size);
    printf("\n\nDefault heating:\n");
    print_float64_vec(heating, heating_size);
    if (ierr) 
        printf("  Error %d getting heating (default) for isotope u235.\n", ierr);
    ierr = ndi_get_float64_vec(NDI_KERMA, kerma, kerma_size);
    printf("\nDefault kerma:\n");
    print_float64_vec(kerma, kerma_size);
    if (ierr) 
        printf("  Error %d getting kerma (default) for isotope u235.\n", ierr);


    /* Use kerma/heating from library (same as default */
    ierr = ndi_set_option(NDI_KERMA, "use_data");
    ierr = ndi_get_float64_vec(NDI_HEATING, heating, heating_size);
    printf("\n\nLibrary heating:\n");
    print_float64_vec(heating, heating_size);
    if (ierr) 
        printf("  Error %d getting heating (data) for isotope u235.\n", ierr);
    ierr = ndi_get_float64_vec(NDI_KERMA, kerma, kerma_size);
    printf("\n\nLibrary kerma:\n");
    print_float64_vec(kerma, kerma_size);
    if (ierr) 
        printf("  Error %d getting kerma (data) for isotope u235.\n", ierr);

    /* Use kerma/heating from library if available, otherwise use calculation */
    /* In mendf6 library kerma/heating values ARE available, so this option will 
       return the same as LIBRARY values above */
    ierr = ndi_set_option(NDI_KERMA, "use_data_calc");
    ierr = ndi_get_float64_vec(NDI_HEATING, heating, heating_size);
    printf("\n\nData/calc heating:\n");
    print_float64_vec(heating, heating_size);
    if (ierr) 
        printf("  Error %d getting heating (data_calc) for isotope u235.\n", ierr);
    ierr = ndi_get_float64_vec(NDI_KERMA, kerma, kerma_size);
    printf("\nData/calc kerma:\n");
    print_float64_vec(kerma, kerma_size);
    if (ierr) 
        printf("  Error %d getting kerma (data_calc) for isotope u235.\n", ierr);


    /* Use calculated kerma/heating only */
    ierr = ndi_set_option(NDI_KERMA, "use_calc");
    ierr = ndi_get_float64_vec(NDI_HEATING, heating, heating_size);
    printf("\n\nCalculated heating:\n");
    print_float64_vec(heating, heating_size);
    if (ierr) 
        printf("  Error %d getting heating (calc) for isotope u235.\n", ierr);
    ierr = ndi_get_float64_vec(NDI_KERMA, kerma, kerma_size);
    printf("\n\nCalculated kerma:\n");
    print_float64_vec(kerma, kerma_size);
    if (ierr) 
        printf("  Error %d getting kerma (calc) for isotope u235.\n", ierr);


    /* See how average energy option affects calculated kerma/heating values */
    ierr = ndi_set_option( NDI_E_AVG, "from_sgndf" );
    ierr = ndi_set_option(NDI_KERMA, "use_calc");
    ierr = ndi_get_float64_vec(NDI_HEATING, heating, heating_size);
    printf("\n\nCalculated heating (SGNDF energy):\n");
    print_float64_vec(heating, heating_size);
    if (ierr) 
        printf("  Error %d getting heating (calc/sgndf energy) for isotope u235.\n", ierr);
    ierr = ndi_get_float64_vec(NDI_KERMA, kerma, kerma_size);
    printf("\n\nCalculated kerma (SGNDF energy):\n");
    print_float64_vec(kerma, kerma_size);
    if (ierr) 
        printf("  Error %d getting kerma (calc/sgndf energy) for isotope u235.\n", ierr);


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

