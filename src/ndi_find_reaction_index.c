
/*
 * File:	ndi_find_reaction_index.c
 * Description:	Internal functions--given an MT, finds array index of data.
 * Time-stamp:	<1998-12-18 14:24:04 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"


/**********************************************************************/
int ndi_find_rprod_index(FILE *stream, multi_stat *rprod, int *ret_index, 
			 int mt ) 

{
   int ierr = 0;
   int curr_mt;
   int i;
   long file_pos;
   int num_prodx;
   int read_return;

   if (rprod->num_objs <= 0) {
      /* No reaction product data in library */
      return (-9);
   }

   for (i = 0; i < rprod->num_objs; i++ ) {
      file_pos = rprod->stat[i].file_ptr;
      fseek( stream, file_pos, SEEK_SET );
      
      /* Read reaction mt and number of rprods */
      read_return = fscanf(stream, "%d\n%d\n", &curr_mt, &num_prodx);
      if ( ( read_return == EOF) || ( read_return < 2 ) ) {
	 /* Error reading data */
	 return(-11);
      }
      if (curr_mt == mt) {
	 *ret_index = i;
	 return(ierr);
      }
   }
   /* Reaction not found */
   ierr = -9;
   return(ierr);
}

/**********************************************************************/
int ndi_find_reac_index(int *mt_array, int *ret_index, int mt, int num ) 

{
   int ierr = 0;
   int i;

   if (num <= 0) {
      /* No reaction product data in library */
      return (-9);
   }

   for (i = 0; i < num; i++ ) {
       if (mt_array[i] == mt) {
	 *ret_index = i;
	 return(ierr);
      }
   }
   /* Reaction not found */
   ierr = -9;
   return(ierr);
}

/**********************************************************************/
