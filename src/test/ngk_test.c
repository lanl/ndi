#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

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


    ierr = ndi_init();
    if (ierr)
	printf("  **Error %d in NDI init.\n\n", ierr );
    ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
    ierr = ndi_set_option( NDI_LIBRARY, "ngk6" );
    if (ierr)
	printf("  **Error %d setting library ngk6.\n\n", ierr );
    
    for ( i = 0; i < num_zaid; i++ ) {
	printf("***********************ISOTOPE=%s***********************\n\n",
	       zaid_list[i] );
	ierr = ndi_set_isotope(zaid_list[i]);
	if (ierr) 
	    printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );

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

	/* DATE */
	ierr = ndi_get_string_val( NDI_DATE_PROCESSED, string, 132 );
	if (ierr) {
	    printf("\n  **Error %d getting DATE PROCESSED.\n", ierr );
	} else {
	    printf ("date processed=%s\n", string);
	}

	/* LAST NEUTRON GROUP */
	ierr = ndi_get_int_val( NDI_LAST_GRP, &int_val );
	if (ierr) {
	    printf("\n  **Error %d getting LAST NEUTRON GROUP.\n", ierr );
	} else {
	    printf ("last n group=%d\n", int_val);
	}

	/* TEMPERATURE */
	ierr = ndi_get_float64_val( NDI_TEMP, &double_val );
	if (ierr) {
	    printf("\n  **Error %d getting TEMPERATURE.\n", ierr );
	} else {
	    printf ("temperature=%e\n", double_val);
	}

	/* SIGMA ZERO */
	ierr = ndi_get_float64_val( NDI_SIG_0, &double_val );
	if (ierr) {
	    printf("\n  **Error %d getting SIGMA ZERO.\n", ierr );
	} else {
	    printf ("sigma(0)=%e\n", double_val);
	}

	/* ATOMIC WEIGHT */
	ierr = ndi_get_float64_val( NDI_AT_WGT, &double_val );
	if (ierr) {
	    printf("\n  **Error %d getting ATOMIC WEIGHT.\n", ierr );
	} else {
	    printf ("atomic weight=%e\n", double_val);
	}

	/* AWR */
	ierr = ndi_get_float64_val( NDI_AWR, &double_val );
	if (ierr) {
	    printf("\n  **Error %d getting ATOMIC WEIGHT RATIO.\n", ierr );
	} else {
	    printf ("awr=%e\n", double_val);
	}

	/* ENERGY BOUNDARIES */
	vec_size = ndi_get_size( NDI_E_BOUNDS, &ierr );
	double_vec = (double *)malloc( vec_size * sizeof(double) );
	ierr = ndi_get_float64_vec( NDI_E_BOUNDS, double_vec, vec_size );
	if (ierr) {
	    printf("\n  **Error %d getting ENERGY BOUNDARIES.\n", ierr );
	} else {
	    printf ("ebounds:\n");
	    print_double_vec( double_vec, vec_size );
	}
	free(double_vec);

	/* WEIGHTS */
	vec_size = ndi_get_size( NDI_WGTS, &ierr );
	double_vec = (double *)malloc( vec_size * sizeof(double) );
	ierr = ndi_get_float64_vec( NDI_WGTS, double_vec, vec_size );
	if (ierr) {
	    printf("\n  **Error %d getting WEIGHTS.\n", ierr );
	} else {
	    printf ("weights:\n");
	    print_double_vec( double_vec, vec_size );
	}
	free(double_vec);

	/* VELOCITIES */
	vec_size = ndi_get_size( NDI_VEL, &ierr );
	double_vec = (double *)malloc( vec_size * sizeof(double) );
	ierr = ndi_get_float64_vec( NDI_VEL, double_vec, vec_size );
	if (ierr) {
	    printf("\n  **Error %d getting VELOCITIES.\n", ierr );
	} else {
	    printf ("velocities:\n");
	    print_double_vec( double_vec, vec_size );
	}
	free(double_vec);

	/* TOTAL CROSS SECTION */
	vec_size = ndi_get_size( NDI_SIG_TOT, &ierr );
	double_vec = (double *)malloc( vec_size * sizeof(double) );
	ierr = ndi_get_float64_vec( NDI_SIG_TOT, double_vec, vec_size );
	if (ierr) {
	    printf("\n  **Error %d getting TOTAL CROSS SECTION.\n", ierr );
	} else {
	    printf ("total xs:\n");
	    print_double_vec( double_vec, vec_size );
	}
	free(double_vec);

	/* NU SIGMA(F) */
	vec_size = ndi_get_size( NDI_NU_SIG_F_PR, &ierr );
	double_vec = (double *)malloc( vec_size * sizeof(double) );
	ierr = ndi_get_float64_vec( NDI_NU_SIG_F_PR, double_vec, vec_size );
	if (ierr) {
	    printf("\n  **Error %d getting NU_SIG_F_PR.\n", ierr );
	} else {
	    printf ("nu*sigma(f) prompt:\n");
	    print_double_vec( double_vec, vec_size );
	}
	free(double_vec);

	/* CHI (PR) */
	vec_size = ndi_get_size( NDI_CHI_VEC_PR, &ierr );
	double_vec = (double *)malloc( vec_size * sizeof(double) );
	ierr = ndi_get_float64_vec( NDI_CHI_VEC_PR, double_vec, vec_size );
	if (ierr) {
	    printf("\n  **Error %d getting CHI(PR).\n", ierr );
	} else {
	    printf ("chi(prompt):\n");
	    print_double_vec( double_vec, vec_size );
	}
	free(double_vec);

	/* CHI (TOT) */
	vec_size = ndi_get_size( NDI_CHI_VEC_TOT, &ierr );
	double_vec = (double *)malloc( vec_size * sizeof(double) );
	ierr = ndi_get_float64_vec( NDI_CHI_VEC_TOT, double_vec, vec_size );
	if (ierr) {
	    printf("\n  **Error %d getting CHI(TOT).\n", ierr );
	} else {
	    printf ("chi(total):\n");
	    print_double_vec( double_vec, vec_size );
	}
	free(double_vec);

	/* FISSION Qs */
	vec_size = ndi_get_size( NDI_FISS_Q, &ierr );
	double_vec = (double *)malloc( vec_size * sizeof(double) );
	ierr = ndi_get_float64_vec( NDI_FISS_Q, double_vec, vec_size );
	if (ierr) {
	    printf("\n  **Error %d getting FISSION Q.\n", ierr );
	} else {
	    printf ("fission q:\n");
	    print_double_vec( double_vec, vec_size );
	}
	free(double_vec);

	/* REACTIONS */
	vec_size = ndi_get_size_n( NDI_SIG_REAC, 0, &ierr );
	double_vec = (double *)malloc( vec_size * sizeof(double) );
        ierr = ndi_get_int_val(NDI_NUM_REAC, &num_reac);
	printf("reactions:\n");
	for (j = 0; j < num_reac; j++ ) {
	    ierr = ndi_get_int_val_n( NDI_REAC_MT, j, &int_val );
	    ierr = ndi_get_float64_val_n( NDI_REAC_Q, int_val, &double_val );
	    printf("  mt=%d  q=%e\n", int_val, double_val );
	    ierr = ndi_get_float64_vec_n( NDI_SIG_REAC, int_val, double_vec, vec_size );
	    if (ierr) {
		printf("\n  **Error %d getting REACTION %d.\n", ierr, int_val );
	    } else {
		print_double_vec( double_vec, vec_size );
	    }
	}
	free(double_vec);

	/* SCATTERING MATRIX */
	vec_size = ndi_get_size_n( NDI_PN_FULL, 0, &ierr );
	double_vec = (double *)malloc( vec_size * sizeof(double) );
        ierr = ndi_get_int_val(NDI_PN_ORDER, &pn_order);
	printf("scattering:\n");
	for (j = 0; j < pn_order; j++ ) {
	    printf("  order=%d\n", j );
	    ierr = ndi_get_float64_vec_n( NDI_PN_FULL, j, double_vec, vec_size );
	    if (ierr) {
		printf("\n  **Error %d getting SCATTING MATRIX (%d).\n", ierr, j );
	    } else {
		print_double_vec( double_vec, vec_size );
	    }
	}
	free(double_vec);
	

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
