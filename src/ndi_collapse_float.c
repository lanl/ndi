
/*
 * File:	ndi_collapse_float.c
 * Description:	Internal function--collapse vector using weighting function.
 * Time-stamp:	<1999-04-01 15:06:32 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_var_type.h"

/**********************************************************************/

#ifdef FLOAT32
int ndi_collapse_float32( const FLOAT_TYPE *old_vec, int old_grp_num, 
			 FLOAT_TYPE *collapse_vec, int recip_flag )
#else
int ndi_collapse_float64( const FLOAT_TYPE *old_vec, int old_grp_num, 
			 FLOAT_TYPE *collapse_vec, int recip_flag )
#endif
/*********************************************************************
    
    Purpose:  Collapse a vector using a weighting function.

    Parameters: 
    old_vec -- in: original, uncollapsed vector
    old_grp_num -- in: number of groups in uncollapsed vector
    collapse_vec -- out: resulting collapsed vector
    recip_flag -- in: indicates method of collapse
                  0 = use normal weighted averaging 
                  1 = weighted average of the reciprocal (e.g. velocities)
                  2 = simply sum with "flooring"--see Note (e.g. chi vectors)
                  3 = simply sum with no "flooring" (e.g. weights )

    Assumptions:
    Global variable 'grp_num_new' contains number of energy groups in
    collapsed energy structure.  Global variable 'new_ebounds' is a
    vector containing the indices of the library energy bounds which
    are the new collapsed energy bounds (i.e. the collapsed energy
    boundaries are a subset of the library energy boundaries).

    Return values:  
    Returns negative value for error condition, positive value for 
    warning, and a '0' if everything's ok.  collapse_vec points to
    collapsed vector.

    NOTE: If recip_flag = 2, then it is assumed that the vector should
    be 'floored'.  That is all data below the lowest new energy bin should
    be added to the lowest energy bin; all data above the highest new 
    energy bin should be added to the highest energy bin.

***********************************************************************/

{
  int i, j;
  FLOAT_TYPE wt_sum; /* Sum of weights */
  int ierr = 0;      /* Error flag: 0=no error,pos=warning, neg=error */
  int up_bound;
  int low_bound;
  
  if ( grp_num_new <= 0 ) {
     /* Number of groups not set  */
     return(-21);
  }

  for ( i = 0; i < grp_num_new; i++ ) {
    wt_sum = 0;
    collapse_vec[i] = 0;
    low_bound = new_ebounds[i];
    up_bound = new_ebounds[i + 1];
    if ( recip_flag == 2 ) {
       if ( i == 0 ) { 
	  low_bound = 0; 
       } else if ( i == ( grp_num_new - 1) ) {
	  up_bound = old_grp_num;
       }
    }
    for ( j = low_bound; j < up_bound; j++ ) {

       /*  Accumulate sum based on type of averaging requested */
	switch (recip_flag) {
	    
	case 0:                     /* Use normal flux-weighted averaging */
	   collapse_vec[i] += old_vec[j] * lib_weight[j];
	   break;
	case 1:                     /* Use reciprocal averaging */
	    collapse_vec[i] += lib_weight[j] / old_vec[j];
 	    break;
	case 2:                     /* Simply sum */
        case 3:
	    collapse_vec[i] += old_vec[j];
	    break;
	}
	
	wt_sum += lib_weight[j];
    }

    /* Divide by weighting factor */
    switch (recip_flag) {
	
    case 0:
       if ( wt_sum > 0 ) {
	  collapse_vec[i] /= wt_sum;
       } else {
	  ierr = -12;
       }
       break;
	
    case 1:
       if ( collapse_vec[i] != 0 ) {
	  collapse_vec[i] = wt_sum / collapse_vec[i];
       } else {
	  collapse_vec[i] = 0;
	  ierr = 5;
       }
       break;
	
    case 2:
    case 3:
       break;
    }
    
  }
  
  return(ierr);
}
