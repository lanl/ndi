#include <stdio.h>
#include <stdlib.h>
#include "ndi.h"

static void print_int_array( int *int_array, int array_size );

int main ()

{
    int ierr;
    int num_interp;
    int *integer_arr;
    int gendir_h;
    int handle;

    printf("\n\n  ************************************************************** \n" );
    printf("    TEST ACCESSING SIGVBAR AND EINBAR INTERPOLATION PARAMETERS\n " );
    printf(" ************************************************************** \n" );

    ierr = NDI2_open_gendir(&gendir_h, "");
    if (ierr)
	printf("  **Error %d in NDI init.\n\n", ierr );

    ierr = NDI2_make_handle(gendir_h, "tn", &handle);
    if( ierr != 0) printf("  Error %d making particle handle.\n", ierr);

    ierr = NDI2_set_option( handle, NDI_LIBRARY, "hale98" );
    if (ierr)
	printf("  **Error %d setting lib type.\n\n", ierr );

     ierr = NDI2_set_reaction(handle, "d+t->n+a");
    if (ierr) {
	printf("\n  **Error %d in setting reaction d+t->n+a.\n", ierr );
    } else {
	printf( "\nGet sigvbar interpolation parameters for d+t->n+a.\n");
	ierr = NDI2_get_int_val( handle, NDI_NUM_SIGVBAR_INTERP, &num_interp );
	printf( "   Number of interpolation regions (err=%d):  %d\n", 
		ierr, num_interp );
	if ( ierr != 0 ) {
	    printf("  ****ERROR = %d getting number of sigvbar interp regions\n", ierr );
	} else {
	    num_interp = NDI2_get_size( handle, NDI_SIGVBAR_INTERP_REG, &ierr );
	    integer_arr = (int *)malloc( num_interp * sizeof(int) );
	    ierr = NDI2_get_int_vec( handle, NDI_SIGVBAR_INTERP_REG, integer_arr, num_interp );
	    printf( "   Interpolation region boundaries (err=%d; size=%d):\n",
		    ierr, num_interp);
	    print_int_array( integer_arr, num_interp );
	    free( integer_arr );

	    num_interp = NDI2_get_size( handle, NDI_SIGVBAR_INTERP_REG, &ierr );
	    integer_arr = (int *)malloc( num_interp * sizeof(int) );
	    ierr = NDI2_get_int_vec( handle, NDI_SIGVBAR_INTERP_TYPE, integer_arr, num_interp );
	    printf( "   Interpolation types(err=%d; size=%d):\n",
		    ierr, num_interp);
	    print_int_array( integer_arr, num_interp );
	    free( integer_arr );
	}

	printf( "\nGet einbar interpolation parameters for d+t->n+a.\n");
	ierr = NDI2_get_int_val( handle, NDI_NUM_EINBAR_INTERP, &num_interp );
	printf( "   Number of interpolation regions (err=%d):  %d\n", 
		ierr, num_interp );
	if ( ierr != 0 ) {
	    printf("  ****ERROR = %d getting number of einbar interp regions\n", ierr );
	} else {
	    num_interp = NDI2_get_size( handle, NDI_EINBAR_INTERP_REG, &ierr );
	    integer_arr = (int *)malloc( num_interp * sizeof(int) );
	    ierr = NDI2_get_int_vec( handle, NDI_EINBAR_INTERP_REG, integer_arr, num_interp );
	    printf( "   Interpolation region boundaries (err=%d; size=%d):\n",
		    ierr, num_interp);
	    print_int_array( integer_arr, num_interp );
	    free( integer_arr );

	    num_interp = NDI2_get_size( handle, NDI_EINBAR_INTERP_REG, &ierr );
	    integer_arr = (int *)malloc( num_interp * sizeof(int) );
	    ierr = NDI2_get_int_vec( handle, NDI_EINBAR_INTERP_TYPE, integer_arr, num_interp );
	    printf( "   Interpolation types(err=%d; size=%d):\n",
		    ierr, num_interp);
	    print_int_array( integer_arr, num_interp );
	    free( integer_arr );
	}	
    }

    ierr = NDI2_close_gendir(gendir_h);

    return(ierr);
}

/**********************************************************************/

static void print_int_array( int *int_array, int array_size ) {

    int n;
    
    printf("   ");
    for (n = 0; n < array_size; n++) {
	if ( ( (n % 10) == 0) && (n != 0) )
	    printf("\n   ");
	printf("%d   ", int_array[n]);
    }
    printf("\n");
}

/**********************************************************************/
  
