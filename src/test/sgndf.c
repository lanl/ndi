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
    int arr_size;
    int test_count = 0;
    int num_zaids = 4;
    char *zaid_list[] = { "u235", "li6", "pu239", "c12"}; 
    int num_grp_structs = 4;
    char *collapse_type[] = { "30_lanl", "16_lanl2", "16_lanl", "16_lanl2"};

    ndi_float64 *return_vec = NULL;
 
    /*  Initialize */
    ierr = ndi_init();
    if (ierr)
        printf("\n  **Error %d in NDI init.\n", ierr );
    
    ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
    if (ierr)
        printf( "\n  **Error %d NDI_LIB_TYPE\n", ierr);
    
    ierr = ndi_set_option( NDI_LIBRARY, "mendf6" );
    if (ierr)
        printf( "\n  **Error %d NDI_LIBRARY\n", ierr);

    ierr = ndi_set_option( NDI_E_AVG, "from_sgndf" );
    if (ierr)
        printf( "\n  **Error %d NDI_E_AVG\n", ierr);

    for (i = 0; i < num_zaids; i++) {
        ierr = ndi_set_option(NDI_MAT_ORDER, "default");
        if (ierr)
            printf( "\n  **Error %d NDI_MAT_ORDER\n", ierr);

        for (j = 0; j < num_grp_structs; j++) {
            
            test_count++;
            printf("\n\n **************************** SGNDF TEST %d: %s "
                   , test_count, zaid_list[i] );
            printf("********************************** \n");
            
            if (j != (num_grp_structs - 1)) {
                printf("    %s group structure \n", collapse_type[j] );
                ierr = 0;
            } else {
                printf("    %s group structure with transpose \n", collapse_type[j] );
                ierr = ndi_set_option(NDI_MAT_ORDER, "transpose");
            }
            printf(" ************************************************************************************ \n" );

            if (ierr)
                printf( "\n  **Error %d NDI_MAT_ORDER\n", ierr);
        
            ierr = ndi_set_option(NDI_COLLAPSE, collapse_type[j]);
            if (ierr)
                printf( "\n  **Error %d NDI_COLLAPSE\n", ierr);
            
            
            ierr = ndi_set_isotope(zaid_list[i]); /* Set isotope */
            if (ierr) {
                printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
            }

            /* Average energy */
            arr_size = ndi_get_size (NDI_E_AVG, &ierr );
            if (ierr)
                printf("\n  **Error %d getting average energy size.\n", ierr);
            return_vec = (ndi_float64 *)malloc( arr_size * sizeof(ndi_float64) );
            ierr = ndi_get_float64_vec(NDI_E_AVG, return_vec, arr_size);
            if (ierr)
                printf("\n  **Error %d getting average energy.\n", ierr);

            printf(" Average energy:\n");
            print_float64_vec(return_vec, arr_size);

            free((void **)return_vec);

            /* Energy to n */
            arr_size = ndi_get_size (NDI_ENERGY_N, &ierr );
            if (ierr)
                printf("\n  **Error %d getting energy to n size.\n", ierr);
            return_vec = (ndi_float64 *)malloc( arr_size * sizeof(ndi_float64) );
            ierr = ndi_get_float64_vec(NDI_ENERGY_N, return_vec, arr_size);
            if (ierr)
                printf("\n  **Error %d getting energy to n.\n", ierr);

            printf(" Energy to n:\n");
            print_float64_vec(return_vec, arr_size);

            free((void **)return_vec);

            /* Energy available */
            arr_size = ndi_get_size (NDI_ENERGY_AVAIL, &ierr );
            if (ierr)
                printf("\n  **Error %d getting energy available size.\n", ierr);
            return_vec = (ndi_float64 *)malloc( arr_size * sizeof(ndi_float64) );
            ierr = ndi_get_float64_vec(NDI_ENERGY_AVAIL, return_vec, arr_size);
            if (ierr)
                printf("\n  **Error %d getting energy available.\n", ierr);

            printf(" Energy available:\n");
            print_float64_vec(return_vec, arr_size);

            free((void **)return_vec);

            /* Prompt transfer matrix (P0) */
            arr_size = ndi_get_size_n (NDI_TRANSFER_MAT_PR, 0, &ierr );
            if (ierr)
                printf("\n  **Error %d getting prompt transfer matrix size.\n", ierr);
            return_vec = (ndi_float64 *)malloc( arr_size * sizeof(ndi_float64) );
            ierr = ndi_get_float64_vec_n(NDI_TRANSFER_MAT_PR, 0, return_vec, arr_size);
            if (ierr)
                printf("\n  **Error %d getting prompt transfer matrix.\n", ierr);

            printf(" Prompt transfer matrix (P0):\n");
            print_float64_vec(return_vec, arr_size);

            free((void **)return_vec);

            /* Prompt fission transfer matrix (Reqt. 63) */
            arr_size = ndi_get_size (NDI_FISS_XFER_MAT_PR, &ierr );
            if (ierr)
                printf("\n  **Error %d getting prompt fission transfer matrix size.\n", ierr);
            return_vec = (ndi_float64 *)malloc( arr_size * sizeof(ndi_float64) );
            ierr = ndi_get_float64_vec(NDI_FISS_XFER_MAT_PR, return_vec, arr_size);
            if (ierr)
                printf("\n  **Error %d getting fission prompt transfer matrix.\n", ierr);

            printf(" Prompt fission transfer matrix:\n");
            print_float64_vec(return_vec, arr_size);

            free((void **)return_vec);

        }
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
