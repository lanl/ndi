
/*
 * File:	ndi_get_size_x.c
 * Description:	User function--returns size of an element of a multi-dim array.
 * Time-stamp:	<00/10/11 16:46:26 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static int get_mg_size_x( int in_key, const char *in_str, int *ierr );

/**********************************************************************/

int ndi_c_get_size_x(int in_key, const char *in_str, int *ierr)

/* Purpose: Return the size of a "vector" in a multi-dimensional array.
   The vector is identified by in_str (a zero-based index).

   Parameters:
   in_key -- in: key to specify type of data required
   ierr -- out: error flag, where 0=no error,pos=warning, neg=error
   in_str -- in: indicates the element for which to find the size.

   Returns: Returns the integer indicating the number of elements
   in the array.  For single-valued data (temperature, sigma0, etc.)
   "1" is returned.  For string data the size of the string (number of
   characters plus the end of string character) is returned.  Returns 
   zero or negative as error.

**********************************************************************/

{
   int size = 0;              /* Default return value is '0' */
   *ierr = 0;                         /* Default error value */


   /* Check that ndi_init has been called. */
   if (!ndi_initialized) {
      *ierr = -2;
      return(size); }
   
   if ( library_type == 'm' ) {
      size = get_mg_size_x( in_key, in_str, ierr );
   } else {
      /* Incompatible library type */
      *ierr = -16;
      size = 0;
   }
   
   if ( size < 0 ) { size = 0; }

   return(size);
}
      
/**********************************************************************/
/**********************************************************************/

static int get_mg_size_x( int in_key, const char *in_str, int *ierr ) {

   int return_value = 0;              /* Default return value is '0' */

   *ierr = 0;                          /* Default error value */

   /* Check that isotope is set */
   if (! iso_stat.set_flag) {
     *ierr = -7;
     return(return_value);
   }

   /*************************/
   /*   Process input key   */
   /*************************/       
   switch (in_key) {

   case NDI_TOT_PROD_XS:                          /* Total production cross section */
     /* Check in_str is valid */
     /* instr */
     if ( collapse_flag != 0 ) {
       return_value = grp_num_new;
     } else {
       return_value = iso_stat.num_grps;
     }
     break;
     
      
     /***********  UNKNOWN KEYWORD  **************/
     
   default:
     *ierr = -4;
     break;
   }

   return(return_value);
}

/**********************************************************************/
/**********************************************************************/

