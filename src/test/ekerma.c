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
    int ierr = 0;
    int i;
    int j;
    int kerma_size;
    int heating_size;
    int num_zaids = 4;
    char *zaid_list[] = { "u235", "li6", "pu239", "c12"}; 

    ndi_float64 *kerma = NULL;
    ndi_float64 *heating = NULL;
 
    /* Go through once with no collapsing and once with collapsing */
    for (j = 0; j < 2; j++) {
        /*  Initialize */
        ierr = ndi_init();
        if (ierr)
            printf("\n  **Error %d in NDI init.\n", ierr );
        
        ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
        if (ierr)
            printf( "\n  **Error %d NDI_LIB_TYPE\n", ierr);
        
        ierr = ndi_set_option( NDI_LIBRARY, "kerma_test" );
        if (ierr)
            printf( "\n  **Error %d NDI_LIBRARY\n", ierr);
        
        if ( j == 0 ) {
            printf("\n****************  NO COLLAPSING  ****************\n");
        } else {
            printf("\n\n\n*****************   COLLAPSING  *****************\n");
            ierr = ndi_set_option(NDI_COLLAPSE, "16_lanl");
        }

        /* Library kerma -- no calculation */
        for (i = 0; i < num_zaids; i++) {
            
            printf("\n\n **************************** KERMA TEST %d: %s "
                   , i + 1, zaid_list[i] );
            printf("********************************** \n");
            printf("    MULTIGROUP NEUTRON -- LIBRARY KERMA W/NO CALCULATION (DEFAULT)\n" );
            printf(" ************************************************************************************ \n" );
        
            ierr = ndi_set_isotope(zaid_list[i]); /* Set isotope */
            if (ierr) {
                printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
            }

            kerma_size = ndi_get_size (NDI_KERMA, &ierr );
            if (ierr)
                printf("\n  **Error %d getting kerma size.\n", ierr);
            kerma = (ndi_float64 *)malloc( kerma_size * sizeof(ndi_float64) );
            ierr = ndi_get_float64_vec(NDI_KERMA, kerma, kerma_size);
            if (ierr)
                printf("\n  **Error %d getting kerma.\n", ierr);

            printf(" Kerma:\n");
            print_float64_vec(kerma, kerma_size);

            heating_size = ndi_get_size (NDI_HEATING, &ierr );
            if (ierr)
                printf("\n  **Error %d getting kerma size.\n", ierr);
            heating = (ndi_float64 *)malloc( heating_size * sizeof(ndi_float64) );
            ierr = ndi_get_float64_vec(NDI_HEATING, heating, heating_size);
            if (ierr)
                printf("\n  **Error %d getting kerma.\n", ierr);

            printf(" Heating:\n");
            print_float64_vec(heating, heating_size);

            free((void **)kerma);
            free((void **)heating);
        }

        /* Library kerma -- with calculation */
        for (i = 0; i < num_zaids; i++) {

            printf("\n\n **************************** KERMA TEST %d: %s "
                   , i + 1 + num_zaids, zaid_list[i] );
            printf("********************************** \n");
            printf("    MULTIGROUP NEUTRON -- LIBRARY KERMA w/ CALCULATION IF NOT FOUND \n" );
            printf(" ************************************************************************************ \n" );
        
            ierr = ndi_set_option(NDI_KERMA, "use_data_calc");
            ierr = ndi_set_isotope(zaid_list[i]); /* Set isotope */
            if (ierr) {
                printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
            }

            kerma_size = ndi_get_size (NDI_KERMA, &ierr );
            if (ierr)
                printf("\n  **Error %d getting kerma size.\n", ierr);
            kerma = (ndi_float64 *)malloc( kerma_size * sizeof(ndi_float64) );
            ierr = ndi_get_float64_vec(NDI_KERMA, kerma, kerma_size);
            if (ierr)
                printf("\n  **Error %d getting kerma.\n", ierr);

            printf(" Kerma:\n");
            print_float64_vec(kerma, kerma_size);

            heating_size = ndi_get_size (NDI_HEATING, &ierr );
            if (ierr)
                printf("\n  **Error %d getting kerma size.\n", ierr);
            heating = (ndi_float64 *)malloc( heating_size * sizeof(ndi_float64) );
            ierr = ndi_get_float64_vec(NDI_HEATING, heating, heating_size);
            if (ierr)
                printf("\n  **Error %d getting kerma.\n", ierr);

            printf(" Heating:\n");
            print_float64_vec(heating, heating_size);

            free((void **)kerma);
            free((void **)heating);
        }

        /* Library kerma -- with no calculation */
        for (i = 0; i < num_zaids; i++) {

            printf("\n\n **************************** KERMA TEST %d: %s "
                   , i + 1 + num_zaids, zaid_list[i] );
            printf("********************************** \n");
            printf("    MULTIGROUP NEUTRON -- LIBRARY KERMA w/ NO CALCULATION IF NOT FOUND  (SET)\n" );
            printf(" ************************************************************************************ \n" );
        
            ierr = ndi_set_option(NDI_KERMA, "use_data");
            ierr = ndi_set_isotope(zaid_list[i]); /* Set isotope */
            if (ierr) {
                printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
            }

            kerma_size = ndi_get_size (NDI_KERMA, &ierr );
            if (ierr)
                printf("\n  **Error %d getting kerma size.\n", ierr);
            kerma = (ndi_float64 *)malloc( kerma_size * sizeof(ndi_float64) );
            ierr = ndi_get_float64_vec(NDI_KERMA, kerma, kerma_size);
            if (ierr)
                printf("\n  **Error %d getting kerma.\n", ierr);

            printf(" Kerma:\n");
            print_float64_vec(kerma, kerma_size);

            heating_size = ndi_get_size (NDI_HEATING, &ierr );
            if (ierr)
                printf("\n  **Error %d getting kerma size.\n", ierr);
            heating = (ndi_float64 *)malloc( heating_size * sizeof(ndi_float64) );
            ierr = ndi_get_float64_vec(NDI_HEATING, heating, heating_size);
            if (ierr)
                printf("\n  **Error %d getting kerma.\n", ierr);

            printf(" Heating:\n");
            print_float64_vec(heating, heating_size);

            free((void **)kerma);
            free((void **)heating);
        }

        /* Find calculated kerma */
        ierr = ndi_set_option(NDI_KERMA, "use_calc");
        for (i = 0; i < num_zaids; i++) {

            printf("\n\n **************************** KERMA TEST %d: %s "
                   , i + 1 + (2 * num_zaids), zaid_list[i] );
            printf("********************************** \n");
            printf("    MULTIGROUP NEUTRON -- CALCULATED KERMA \n" );
            printf(" ************************************************************************************ \n" );
        
            ierr = ndi_set_isotope(zaid_list[i]); /* Set isotope */
            if (ierr) {
                printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
            }

            kerma_size = ndi_get_size (NDI_KERMA, &ierr );
            if (ierr)
                printf("\n  **Error %d getting kerma size.\n", ierr);
            kerma = (ndi_float64 *)malloc( kerma_size * sizeof(ndi_float64) );
            ierr = ndi_get_float64_vec(NDI_KERMA, kerma, kerma_size);
            if (ierr)
                printf("\n  **Error %d getting kerma.\n", ierr);

            printf(" Kerma:\n");
            print_float64_vec(kerma, kerma_size);

            heating_size = ndi_get_size (NDI_HEATING, &ierr );
            if (ierr)
                printf("\n  **Error %d getting kerma size.\n", ierr);
            heating = (ndi_float64 *)malloc( heating_size * sizeof(ndi_float64) );
            ierr = ndi_get_float64_vec(NDI_HEATING, heating, heating_size);
            if (ierr)
                printf("\n  **Error %d getting kerma.\n", ierr);

            printf(" Heating:\n");
            print_float64_vec(heating, heating_size);

            free((void *)kerma);
            free((void *)heating);
        }
        ierr = ndi_close();
        if (ierr)
            printf("  **Error %d in closing NDI.\n\n", ierr );
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
