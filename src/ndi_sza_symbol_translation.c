/*
 * File:	ndi_sza_symbol_translation.c
 * Description:	Functions for SZA to label conversions.
 * Time-stamp:	<2002-12-10 16:00:00 mcw>
 *
 */

/**********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_element_symbols.h"

/**********************************************************************/

int ndi_translate_sza_to_symbol( int sza, char *sym_str, int str_size)
{

    int Z, A, S;
    int ierr = 0;
    char tmp_sym_str[MAX_IN_STR_LEN] = "";
    char tmp_str[MAX_IN_STR_LEN] = "";

    if ( str_size < 1 ) {
	return(7);
    }

    ierr = ndi_parse_int_zaid( sza, &Z, &A, &S );
    
    if ( ( Z > 0 ) && ( Z <= MAX_Z ) ) {
	strcpy( tmp_sym_str, ndi_element_sym[Z]);
	if ( A > 0 ) {
	    sprintf( tmp_str, "%d", A);
	    strcat( tmp_sym_str, tmp_str );
	    if ( S > 0 ) {
		sprintf( tmp_str, "m%d", S );
		strcat( tmp_sym_str, tmp_str );
	    }
	}
    } else {
	if ( Z == 0 ) {
	    if ( A == 0 ) {
		strcpy( tmp_sym_str, "g" );
	    } else if ( A == 1 ) {
		strcpy ( tmp_sym_str, "n" );
	    } else {
		/* Error -- bad ZAID */
		ierr = -22;
	    }
	} else {
	    /* Error -- bad ZAID */
	    ierr = -22;
	}
    }

    if ( (strlen( tmp_sym_str ) + 1) <= str_size ) {
	strncpy( sym_str, tmp_sym_str, ( sizeof(char) * (strlen( tmp_sym_str ) + 1) ) );
    } else {
	if (str_size > 0) {
	    strncpy( sym_str, tmp_sym_str, ( sizeof(char) * str_size ) );
	    sym_str[str_size - 1] = '\0';    
	}
	ierr = 7;
    }
    return(ierr);
}
