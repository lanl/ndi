
/*
 * File:	ndi_find_index.c
 * Description:	Internal functions--given an MT, finds array index of data.
 * Time-stamp:	<1998-12-18 14:24:04 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "ndi.h"
#include "ndi_internal.h"

static double ndi_find_closest_bound_index( double inval, double val1, double val2 );

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
/**********************************************************************/
int ndi_find_tn_part_index( const long ptype , const sec_part_distr *part, int num_parts )

/*  This is only used for tndata */

{

   int i;

   for ( i = 0; i < num_parts; i++ ) {
     if ( ptype == part[i].part_type ) {
       return(i);
     }
   }

   return(-1);
}

/**********************************************************************/
/**********************************************************************/
int ndi_find_mg_part_index( const int ptype , const sec_part_mg *part, int num_parts )

/*  This is only used for multigroup secondary particles */

{

   int i;
   sec_part_mg curr_part;

   for ( i = 0; i < num_parts; i++ ) {
       curr_part = part[i];
       if ( curr_part.part_type == ptype ) {
	 return(i);
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
/**********************************************************************/
int ndi_find_temp_bound_index( double default_temp, edist_data *edist, int ntemps, 
			       int *hi, int *lo, int *closest )
{

  /* Assumes data is given in ASCENDING order */

  int ierr = 0;
  int i;

  /* Initialize */
  *hi = 0;
  *lo = 0;
  *closest = 0;
  
  /* Find the closest temperature */ 
  if ( ntemps > 1 ) {

    if ( default_temp < edist[0].temp ) {
      /* Temp. is higher than the highest temp given in library
          -- Keep default values */
	  ierr = 30;

    } else if ( default_temp > edist[ntemps - 1].temp ) {
      /* Temp. is higher than the highest temp given in library */
	  *hi = ntemps - 1;
	  *lo = ntemps - 1;
	  *closest = ntemps - 1;
	  ierr = 30;

    } else {
      /* Temp. is within given distribution */
      for ( i = 0; i < (ntemps - 1); i++ ) {

	if ( default_temp == edist[i].temp ) {
	  /* Temp is the current max. temp */
	  *hi = i;
	  *lo = i;
	  *closest = i;
	  break;

	} else if ( default_temp == edist[i+1].temp ) {
	  /* Temp is the current min. temp */
	  *hi = i+1;
	  *lo = i+1;
	  *closest = i+1;
	  break;

	}  else if ( default_temp > edist[i+1].temp )  {
	  /* Temp is above max. temp -- THIS SHOULD NEVER BE TRUE */
	  

	}  else if ( default_temp <  edist[i+1].temp ) {
	  /* Temp is in the current bin*/
	  double junk;

	  *hi = i+1;
	  *lo = i;
	  junk = ndi_find_closest_bound_index( default_temp, edist[i].temp, edist[i+1].temp );
	  if ( junk == edist[i].temp ) {
	    *closest = i;
	  } else if  ( junk == edist[i+1].temp ) {
	    *closest = i+1;
	  } else {
	    /* error */
	  }

	  break;
	  
	}
      }
    }
  } else if ( ntemps == 1 ) {
    /* Leave the defaults */
  } else {
    /* No temperature data given */
    return(-9);
  }

  return(ierr);
}


/**********************************************************************/
/**********************************************************************/
int ndi_find_bounds_index( double *x, int numpts, double xin, 
			   int *lo, int *hi, int *closest )
     /* Assumes that x is in ASCENDING order */
{
  int i;
  int ierr = 0;

  /* Initialize */
  *hi = 0;
  *lo = 0;
  *closest = 0;
  
  if ( numpts > 1 ) {

    /* Check if xin is outside of range */
    if ( xin < x[0] ) {
      /* xin is less than all values -- keep default */
      ierr = 30;
    } else if ( xin > x[numpts - 1] ) {
      /* xin is greater than all values -- set to highest */
      *hi = numpts - 1;
      *lo = numpts - 1;
      *closest = numpts - 1;
      ierr = 30;
    } else {
      /* xin is within given distribution */
      for ( i = 0; i < ( numpts - 1 ); i++ ) {
	if ( xin == x[i] ) {
	  *hi = i;
	  *lo = i;
	  *closest = i;
	  break;
	} else if ( xin == x[i+1] ) {
	  *hi = i+1;
	  *lo = i+1;
	  *closest = i+1;
	  break;  
	} else if ( xin < x[i] ) {
	  /* Should not be true */
	} else if ( xin < x[i+1] ) {
	  /* The point is contained within the current bin */
	  double junk;

	  *lo = i;
	  *hi = i+1;
	  junk = ndi_find_closest_bound_index( xin, x[i], x[i]);
	  if ( junk == x[i] ) {
	    *closest = i;
	  } else {
	    *closest = i + 1;
	  }
	  break;
	}
      }
    }
  }
  return(ierr);
}
/**********************************************************************/
/**********************************************************************/
static double ndi_find_closest_bound_index( double inval, double val1, double val2 )
{

  /* default_temp is a global variable */

  if ( fabs( val1 - inval ) <= fabs( inval - val2 ) ) {
    return ( val1 );
  } else {
    return ( val2 );
  }

}
