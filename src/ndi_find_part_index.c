
/*
 * File:	ndi_part_part_index.c
 * Description:	Internal functions--given a particle type, finds index structure.
 * Time-stamp:	<1999-11-19 17:43:42 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"


/**********************************************************************/
int ndi_find_part_index( const char *ptype , const sec_part_distr *part, int num_parts )

/*  This is only used for tndata */

{

   int i;
   char curr_char;
   sec_part_distr curr_part;

   for ( i = 0; i < num_parts; i++ ) {
     curr_part = part[i];
     if (curr_part.part_type) {
       curr_char = curr_part.part_type[0];
       if ( *ptype == curr_char ) {
	 /* check the first letter for n, p, a */
	 if ( strcmp( ptype, part[i].part_type ) == 0 ) {
	   return(i);
	 }
       }
     }
   }
   return(-1);
}

/**********************************************************************/
/**********************************************************************/
int ndi_find_iso_index( const char *ptype , const char **string_array, int num_parts )

/*  This is currently only used for depletion data */

{

   int i;

   for ( i = 0; i < num_parts; i++ ) {
      if ( strncmp( ptype,  string_array[i], (strlen(ptype)+1)*sizeof(char) ) == 0 ) {
	 return(i);
      }
   }

   return(-1);
}

/**********************************************************************/
