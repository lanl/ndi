#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

#define ZAID_STR_SIZE    133

int main ()

{
    int ierr;
    int i;
    int gendir_h;
    int handle;
    char zaid_str[ZAID_STR_SIZE];

    int num_izaid = 14;
    int izaid[] = { 1001,
		    0,
		    1,
		    92235,
		    103270,
		    2095240,
		    20001002,
		    110290,
		    -999,
                    99999,
		    999999,
		    2,
		    110000,
		    1109000
    };
		    
    int num_zaid = 17;
    char *zaid_list[] = { "92235.600nm  ", 
			  "1001.", 
			  " 1002.   ",
			  "1003  ",
			  " 1039087", 
			  "pu239", 
			  "10020 ",
                          "h2",
                          " He-4",
                          "O16",
                          "C-12",
                          "i-127",
                          "TM169 ",
                          "TA-182",
                          "pu-240.6",
                          " h-1.50 ",
                          "Fl-19"};

    printf("\n\n  ********************************************************* \n" );
    printf("    TEST TRANSLATION OF ISOTOPIC SYMBOL TO ZAID\n " );
    printf(" ********************************************************** \n\n" );

    ierr = NDI2_open_gendir(&gendir_h, "");
    if (ierr)
	printf("  **Error %d in NDI init.\n\n", ierr );

    ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle);
    if( ierr != 0) printf("  Error %d making particle handle.\n", ierr);

    /*  For all ZAIDs */
    printf( "Interpret izaid before setting lib type.\n");
    for ( i = 0; i < num_izaid; i++ ) {
	ierr = NDI2_get_string_val_n(handle, NDI_ZAID, izaid[i], zaid_str, ZAID_STR_SIZE );
	if ( ierr != 0 ) 
	    printf("  ****ERROR = %d\n", ierr );
	printf ( "  izaid = %d;  SZA = %s\n", izaid[i], zaid_str );
    }

    /*  For all ZAIDs */
    printf( "\nInterpret izaid after setting lib type.\n");
    for ( i = 0; i < num_izaid; i++ ) {
	ierr = NDI2_get_string_val_n(handle, NDI_ZAID, izaid[i], zaid_str, ZAID_STR_SIZE );
	if ( ierr != 0 ) 
	    printf("  ****ERROR = %d\n", ierr );
	printf ( "  izaid = %d;  SZA = %s\n", izaid[i], zaid_str );
    }

    /*  For all ZAIDs */
    printf( "\nCheck translation of symbols in set_isotope.\n");
    for ( i = 0; i < num_zaid; i++ ) {
	printf("ISOTOPE=>%s<\n", zaid_list[i] );
	ierr = NDI2_set_isotope(handle, zaid_list[i]);
	if (ierr) {
	    printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
	} else {
	    ierr = NDI2_get_string_val(handle, NDI_ZAID, zaid_str, ZAID_STR_SIZE );
	    if ( ierr != 0 ) 
		printf("  ****ERROR = %d\n", ierr );
	    printf( "  Zaid = >%s<\n", zaid_str );
	}
    }

    ierr = NDI2_close_gendir(gendir_h);

    return(ierr);
}
