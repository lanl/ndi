
/*
 * File:	ndi_std_zaid.c
 * Description:	Internal function--convert input to standard zaid.
 * Time-stamp:	<1999-04-13 16:37:45 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ndi_internal.h"

static void ndi_process_extension ( char *in_str, char **ext_p );

/**********************************************************************/

int ndi_std_zaid(state_struct *state, char *in_zaid, int zaid_size ) {

/*  Purpose: To return a full or partial zaid (depending on the input
    zaid) in the standard format.
    
    Parameters: 
    in_zaid -- in & out: on input is a zaid or element symbol, on
                         output is the zaid in standard format

    Assumptions:
    The input zaid contains an element symbol or zaid specifying
    the Z and A number of an isotope.  Input can be in the form
    U-235, u235, 92235, U.  (Capitalization doesn't matter.  There
    may be a dash separating the element symbol and A number.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/

  int ierr = 0;
  int Z;
  int A;
  int S;
  char *zaid_extension_p = NULL;
  char real_zaid[MAX_IN_STR_LEN];

  real_zaid[0] = '\0';
  if (! state) {
      return(-2);
  }
  ierr = ndi_parse_str_zaid(state,in_zaid, &Z, &A, &S);

  /* Check errors */
  if ( ( Z <= MAX_Z ) && ( Z > 0 ) ) {
      sprintf( real_zaid, "%-d.", ( S*1000000 + Z*1000 + A) );

      (void)ndi_process_extension( in_zaid, &zaid_extension_p );
      strcat( real_zaid, zaid_extension_p );
      /* Check length */
      strcpy( in_zaid, real_zaid );
  } else {
      ierr = -22;
  }

  return( ierr );

}

/**********************************************************************/
            /*   FUNCTIONS INTERNAL TO FILE  */
/**********************************************************************/
static void ndi_process_extension ( char *in_str, char **ext_p ) {
    
    /* Set pointer ext_p to the beginning of the ZAID extension 
       (after the '.') or to the '\0' if there is no extension.

       Assuming in_str ends with string-terminating character '\0'.
    */

    char *char_p = NULL;
    char *end_p = NULL;
    int in_str_len = strlen(in_str);
    int i;

    /* Find last non-blank character and set ending pointer */
    for ( i = (in_str_len - 1); i >= 0; i-- ) {
	if ( ! isspace(in_str[i]) ) {
	    end_p = &in_str[i+1];
	    break;
	}
    }

    /* Set pointer to mark the beginning of the ZAID extension */
    char_p = strchr(in_str, '.');

    if (char_p) {
	*ext_p = char_p + 1;
    } else {
	*ext_p = end_p;
    }

    return;
}
