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
    int i, j, k;
    int str_size;
    int num_zaids = 36;
    char zaid[25];
    char *info_str;
    char *zaid_list[] = { "92236.601nm", "92236.602nm", "92236.603nm", 
			  "92236.604nm", "92236.605nm", "92236.606nm", 
			  "92236.607nm", "92236.608nm", "92236.609nm", 
			  
			  "92236.610nm", "92236.611nm", "92236.612nm",
			  "92236.613nm", "92236.614nm", "92236.615nm", 
			  "92236.616nm", "92236.617nm", "92236.618nm",

			  "92236.619nm", "92236.620nm", "92236.621nm", 
			  "92236.622nm", "92236.623nm", "92236.624nm", 
			  "92236.625nm", "92236.626nm", "92236.627nm", 

			  "92236.628nm", "92236.629nm", "92236.630nm",
			  "92236.631nm", "92236.632nm", "92236.631nm", 
			  "92236.634nm", "92236.635nm", "92236.632nm" 
    };

    int num_pnu;
    int num_grps;
    ndi_float64 *nu = NULL;
    ndi_float64 *p = NULL;
    ndi_float64 *norm = NULL;
    ndi_float64 *mean = NULL;
    ndi_float64 *sf = NULL;
    ndi_float64 *sp = NULL;

    /*  Initialize */
    ierr += ndi_init();
    if (ierr)
	printf("\n  **Error %d in NDI init.\n", ierr );

    ierr += ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
    if (ierr)
	printf( "\n  **Error %d NDI_LIB_TYPE\n", ierr);

    ierr += ndi_set_option( NDI_LIBRARY, "chi_test_lib" );
    if (ierr)
	printf( "\n  **Error %d NDI_LIBRARY\n", ierr);

    for ( i = 0; i < num_zaids; i++ ) 
    {
	ierr += ndi_set_isotope(zaid_list[i]); /* Set isotope */
	if (ierr) 
	    printf("\n  **Error %d in setting isotope %s.\n", 
		   ierr, zaid_list[i] );

	str_size = ndi_get_size (NDI_INFO, &ierr );
	if (ierr) 
	    printf("\n  **Error %d info string size.\n", ierr );

	info_str = (char *)malloc( str_size * sizeof(char) );
	ierr += ndi_get_string_val( NDI_INFO, info_str, str_size );
	if (ierr)
	    printf("  Error %d getting information for isotope %s.\n", 
		   ierr, zaid_list[i] );

	printf("\n\n ************************************** PNU TEST %d "
	       , i + 1 );
	printf("********************************** \n");
	printf("    MULTIGROUP NEUTRON DATA PNU TEST\n " );
	/*printf("   %s", info_str );*/
	printf(" ************************************************************************************ \n" );
     
	if ( info_str ) free( info_str );

	ierr += ndi_get_string_val( NDI_ZAID, zaid, 25 );
	printf("\nZaid = %s\n\n", zaid );
     
	ierr += ndi_get_int_val(NDI_NUM_PNU, &num_pnu);
	if (ierr)
	    printf("\n  **Error %d getting NUM_PNU.\n", ierr);

	num_grps = ndi_get_size(NDI_NU_PR, &ierr);
	if (ierr)
	    printf("\n  **Error %d NDI_NU_PR size.\n", ierr);

	nu = (ndi_float64 *)malloc(num_grps * sizeof(ndi_float64));
	num_grps = ndi_get_size_n(NDI_PNU, 0, &ierr);
	if (ierr)
	    printf("\n  **Error %d NDI_PNU size.\n", ierr);

	p = (ndi_float64 *)malloc(num_grps * sizeof(ndi_float64));
	ierr += ndi_get_float64_vec(NDI_NU_PR, nu, num_grps);
	if (ierr)
	    printf("\n  **Error %d NDI_NU_PR.\n", ierr);

	norm = (ndi_float64 *)malloc(num_grps * sizeof(ndi_float64));
	mean = (ndi_float64 *)malloc(num_grps * sizeof(ndi_float64));

	for (j = 0; j < num_grps; j++)
	{
	    norm[j] = -1.0;
	    mean[j] = -nu[j];
	}

	for (j = 0; j < num_pnu; j++)
	{
	    ierr += ndi_get_float64_vec_n(NDI_PNU, j, p, num_grps);
	    if (ierr)
		printf("\n  **Error %d NDI_PNU.\n", ierr);
	    for (k = 0; k < num_grps; k++)
	    {
		norm[k] += p[k];
		mean[k] += j * p[k];
	    }
	}

	print_float64_vec(norm, num_grps);
	print_float64_vec(mean, num_grps);

	num_grps = ndi_get_size_n(NDI_SIG_PNU, 0, &ierr);
	if (ierr)
	    printf("\n  **Error %d NDI_SIG_PNU size.\n", ierr);

	sf = (ndi_float64 *)malloc(num_grps * sizeof(ndi_float64));
	sp = (ndi_float64 *)malloc(num_grps * sizeof(ndi_float64));
	
	ierr += ndi_get_float64_vec(NDI_NU_SIG_F_PR, sf, num_grps);
	if (ierr)
	    printf("\n  **Error %d NDI_NU_SIG_F_PR.\n", ierr);

	ierr += ndi_get_float64_vec_n(NDI_SIG_PNU, 0, sp, num_grps);
	if (ierr)
	    printf("\n  **Error %d NDI_SIG_PNU.\n", ierr);

	for (k = 0; k < num_grps; k++)
	    sp[k] -= sf[k] / nu[k];
	print_float64_vec(sp, num_grps);

	num_grps = ndi_get_size_n(NDI_SIG_PNU, 1, &ierr);
	if (ierr)
	    printf("\n  **Error %d NDI_SIG_PNU size.\n", ierr);

	ierr += ndi_get_float64_vec_n(NDI_SIG_PNU, 1, sp, num_grps);
	if (ierr)
	    printf("\n  **Error %d NDI_SIG_PNU.\n", ierr);

	for (k = 0; k < num_grps; k++)
	    sp[k] -= sf[k];

	print_float64_vec(sp, num_grps);

	free((void *)sf);
	free((void *)sp);
	free((void *)nu);
	free((void *)p);
	free((void *)mean);
	free((void *)norm);

   }  /* for i */

    ierr += ndi_close();
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
