
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

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

#define MAX_Z  110

static int ndi_parse_zaid( const char *in_zaid, char **a_ptr, char **z_ptr, 
			   char **dot_ptr, char **end_ptr);
static short   ndi_convert2z( const char *z1 );

/**********************************************************************/

int ndi_std_zaid(char *in_zaid ) {

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


  char *z_ptr;
  char *a_ptr;
  char *dot_ptr;
  char *end_ptr;
  int z;
  int ext_len;
  char real_zaid[12];
  char end_str = '\0';
  int ierr = 0;
  int i;

  /*************  Check return values ***************/

  memset( real_zaid, (int)end_str, sizeof(real_zaid) );

  /*********************************************
   ***       PROCESS NDI_ZAID ROOT           ***
   *********************************************/

  /*  Set initial pointers  */
  if ( ( ierr = ndi_parse_zaid(in_zaid, &a_ptr, &z_ptr, &dot_ptr, &end_ptr) ) < 0 ) {
     return(ierr);
  }

  /* Process Z */
  if ( isdigit(*z_ptr) ) {
     strncat(real_zaid, z_ptr, (a_ptr - z_ptr)); 
  } else {
     z = (int)ndi_convert2z( z_ptr );
     if( z > 0 ) {
	sprintf( real_zaid, "%d", z );
     } else {
	/*  Unable to convert zaid  */
	return(-22);
     }
  }


  /* Process A */
  for ( i = 0; i < (3 - ( dot_ptr - a_ptr )); i++ ) {
     strcat( real_zaid, "0" );
  }
  if ( a_ptr != dot_ptr ) {
    strncat(real_zaid, a_ptr, (dot_ptr - a_ptr) );
  }

  /*********************************************
   ***      PROCESS NDI_ZAID EXTENSION       ***
   *********************************************/

  ext_len = (int) (end_ptr - dot_ptr);
  if ( ext_len > 0 ) {
     strncat( real_zaid, dot_ptr, ext_len );
  }
  if ( strchr( real_zaid, '.' ) == NULL ) {
    strcat( real_zaid, "." );
  }

  strcpy( in_zaid, real_zaid );

  return( ierr );

}

/**********************************************************************/
            /*   FUNCTIONS INTERNAL TO FILE  */
/**********************************************************************/

static int ndi_parse_zaid( const char *in_zaid, char **a_ptr, char **z_ptr, 
			   char **dot_ptr, char **end_ptr) {

/*  Purpose: Return pointers to Z, A, dot, and end of a zaid string.
    
    Parameters: 
    in_zaid -- in: a zaid or element symbol
    *a_ptr -- out: points to beginning of A value (or end of str if
                  not present)
    *z_ptr -- out: points to beginning of Z value (or element symbol)
    *dot_ptr -- out: points to '.' in string (or end of str if not present)
    *end_ptr -- out: points to end of string

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/

  int ierr = 0;      /* Error flag: 0=no error,pos=warning, neg=error */
  int zaid_len = (int)strlen(in_zaid);  /* Length of input zaid not including
					   string-terminating character */
  int i = 0;
  char *temp_ptr;

  /* Find beginning non-blank character and set z_ptr */
  *z_ptr = NULL;
  while ( i < zaid_len ) {
    if ( in_zaid[i] != ' ' ) {
      *z_ptr = (char *)&in_zaid[i];
      break;
    } else {
      i++;
    }
  }
  if ( *z_ptr == NULL ) {
     /* Bad zaid */
     return(-22);
  }

  /* Find ending non-blank, non-null character and set end_ptr to
     character just after this  */
  for ( i = zaid_len - 1; i >= 0; i-- ) {
     if ( ( in_zaid[i] != ' ' ) && ( in_zaid[i] != '\0' ) ) {
	*end_ptr = (char *)&(in_zaid[i + 1]);
	break;
     }
  }
  if ( i < 0 ) {
     /* Bad zaid */
     return(-22);
  }

  /* Find the decimal pt. and set dot_ptr.  If no decimal, set
     dot_ptr equal to end_ptr */
  *dot_ptr = strchr( in_zaid, '.' );
  if ( *dot_ptr == NULL ) {
     *dot_ptr = *end_ptr;
  }

  /* Set a_ptr */
  /* If Z is numeric, A is assumed to be the last 3 digits in root */
  /* If Z is non-numeric, A is the first numeric value after */
  /* If no A is given, a_ptr set equal to dot_ptr */

  if ( isdigit(**z_ptr) ) {
     if ( ((*dot_ptr - *z_ptr ) / sizeof(char)) > 3 ) {
	*a_ptr = *dot_ptr - ( 3 * sizeof(char) );
     } else {
	/* Bad zaid */
	ierr = -22;
     }
  } else {
     temp_ptr = *z_ptr; 
     while ( temp_ptr != *dot_ptr ) {
	if ( isdigit(*temp_ptr)) {
	   *a_ptr = temp_ptr;
	   break;
	} else {
	   temp_ptr++;
	}
     }
     *a_ptr = temp_ptr;
  }

  return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static short ndi_convert2z( const char *z1 ) {

/*  Purpose: Convert an alphabetic element symbol to an integer 
    representing its Z-number.
    
    Parameters: 
    z1 -- in: pointer to the first character of symbol

    Assumptions:
    The character pointed to by z1 is alphabetic and non-space.
    The global array, ndi_elements, which matches symbols to Z-number
    has already been filled.

    Return values:  Returns the Z-number of the specified element or
    0 or negative number on error.

***********************************************************************/


   char *temp_ptr = (char *)z1;   /* Current character from input zaid */
   char convert_str[2];   /* String to be converted */
   short temp = 0;

   /* Set first character and assure lower-case */
   convert_str[0] = *temp_ptr;
   if ( isupper( convert_str[0] ) ) {
      convert_str[0] = tolower(convert_str[0]);
   }

   /* Set 2nd character--lower case and check for non-valid entries */
   temp_ptr++;
   convert_str[1] = *temp_ptr;
   if ( ( isdigit( convert_str[1] ) ) || ( convert_str[1] == '\0' ) ) {
      convert_str[1] = ' ';
   } else {
      if ( isupper( convert_str[1] ) ) {
	 convert_str[1] = tolower( convert_str[1] ); 
      }
   }
  if ( convert_str[1] == '-' ) {
    convert_str[1] = ' ';
  }

  temp = ndi_elements[element_index(convert_str)];
  if ( ( temp <= MAX_Z ) && ( temp > 0 ) )
    {
      return(temp);
    } else {
      /* Invalid Z-number */
      return(0);
    }
}

