#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"
#include "ndi_state_handle.h"

static void print_double_vec( double *flt_ptr, int num_elements );

int main (void)
{
    int ierr = 0;
    int i, j;
    int vec_size;
    int num_reac;
    int pn_order;
    int int_val;
    char string[132];
    double double_val;
    double *double_vec;

    int num_zaid= 2;
    char *zaid_list[] = { "92235", "h1"};
    int num_grp_n = 21;
    char n_egroup[15];

    ndi_state_handle *h[2];
    
    ierr = ndi_init();
    if (ierr)
	printf("  **Error %d in NDI init.\n\n", ierr );
    ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
    ierr = ndi_set_option( NDI_LIBRARY, "mendf6" );
    if (ierr)
	printf("  **Error %d setting library mendf6.\n\n", ierr );
    
    for ( i = 0; i < num_zaid; i++ ) {
	printf("***********************ISOTOPE=%s***********************\n\n",
	       zaid_list[i] );
	ierr = ndi_set_isotope(zaid_list[i]);
	if (ierr) 
	    printf("\n  **Error %d in setting isotope %s.\n", ierr, 
		   zaid_list[i] );

	/* ZAID */

	ierr = ndi_get_string_val( NDI_ZAID, string, 132);
	if (ierr) {
	    printf("\n  **Error %d getting ZAID.\n", ierr );
	} else {
	    printf ("zaid=%s\n", string);
	}

	/* INFO */
	ierr = ndi_get_string_val( NDI_INFO, string, 132 );
	if (ierr) {
	    printf("\n  **Error %d getting INFO.\n", ierr );
	} else {
	    printf ("info=%s\n", string);
	}

    }
    
    for ( i = 0; i < num_zaid; i++ ) {
	h[i] = ndi_state_handle_new(); /* make a new state */
	ndi_state_handle_swap(h[i]); /* swap global state with this */
	ierr = ndi_init();
	if (ierr)
	    printf("  **Error %d in NDI init.\n\n", ierr );
	ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
	ierr = ndi_set_option( NDI_LIBRARY, "mendf6" );
	if (ierr)
	    printf("  **Error %d setting library mendf6.\n\n", ierr );
	printf("***********************ISOTOPE=%s***********************\n\n",
	       zaid_list[i] );
	ierr = ndi_set_isotope(zaid_list[i]);
	if (ierr) 
	    printf("\n  **Error %d in setting isotope %s.\n", ierr, 
		   zaid_list[i] );

	/* ZAID */
	ierr = ndi_get_string_val( NDI_ZAID, string, 132);
	if (ierr) {
	    printf("\n  **Error %d getting ZAID.\n", ierr );
	} else {
	    printf ("zaid=%s\n", string);
	}

	/* INFO */
	ierr = ndi_get_string_val( NDI_INFO, string, 132 );
	if (ierr) {
	    printf("\n  **Error %d getting INFO.\n", ierr );
	} else {
	    printf ("info=%s\n", string);
	}
	ierr = ndi_close();
	ndi_state_handle_swap(h[i]); /* restore global state */
/*	ndi_state_handle_free(h[i]); */
    }

    for ( i = 0; i < num_zaid; i++ ) {
	printf("***********************ISOTOPE=%s***********************\n\n",
	       zaid_list[i] );
	ierr = ndi_set_isotope(zaid_list[i]);
	if (ierr) 
	    printf("\n  **Error %d in setting isotope %s.\n", ierr, 
		   zaid_list[i] );

	/* ZAID */
	ierr = ndi_get_string_val( NDI_ZAID, string, 132);
	if (ierr) {
	    printf("\n  **Error %d getting ZAID.\n", ierr );
	} else {
	    printf ("zaid=%s\n", string);
	}

	/* INFO */
	ierr = ndi_get_string_val( NDI_INFO, string, 132 );
	if (ierr) {
	    printf("\n  **Error %d getting INFO.\n", ierr );
	} else {
	    printf ("info=%s\n", string);
	}

    }

    ierr = ndi_close();
    return(ierr);
}       
/**********************************************************************/
/**********************************************************************/

static void print_double_vec( double *flt_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 5 ) == 0) )
	 printf("\n");
      printf("%13.6e   ", flt_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/
