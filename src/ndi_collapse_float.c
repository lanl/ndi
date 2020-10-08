
/*
 * $Id$
 * Description:        Internal function--collapse vector using weighting function.
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_var_type.h"

/**********************************************************************/

/*********************************************************************
    
    Purpose:  Collapse a vector using a weighting function.

    Parameters: 
    old_vec -- in: original, uncollapsed vector
    old_grp_num -- in: number of groups in uncollapsed vector
    return_vec -- out: resulting collapsed vector
    return_size -- in: size of array return_vec
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
    warning, and a '0' if everything's ok.  return_vec points to
    collapsed vector.

    NOTE: If recip_flag = 2, then it is assumed that the vector should
    be 'floored'.  That is all data below the lowest new energy bin should
    be added to the lowest energy bin; all data above the highest new 
    energy bin should be added to the highest energy bin.

    NOTE:  There is no bounds checking on old_vec.  It is assumed that
    it is at least of size old_grp_num.

***********************************************************************/
#ifdef FLOAT32
int ndi_collapse_float32(state_struct *state, const FLOAT_TYPE * old_vec,
                     int old_grp_num, int recip_flag,
                     FLOAT_TYPE * return_vec, int return_size)
#else
int ndi_collapse_float64(state_struct *state, const FLOAT_TYPE * old_vec,
                     int old_grp_num, int recip_flag,
                     FLOAT_TYPE * return_vec, int return_size)
#endif
{
    int i, j;
    FLOAT_TYPE wt_sum;                /* Sum of weights */
    int ierr = 0;                /* Error flag: 0=no error,pos=warning, neg=error */
    int up_bound;
    int low_bound;

    if (!state) {
        return -2;
    }

    if (state->grp_num_new <= 0) {
        /* Number of groups not set  */
        return -21;
    }

    for (i = 0; i < state->grp_num_new; i++) {

        /* Bounds check */
        if (i >= return_size) {
            ierr = 7;
            break;
        }

        low_bound = state->new_ebounds[i];
        up_bound = state->new_ebounds[i + 1];

        if (recip_flag == 2) {        /* case 2: flooring */
            if (i == 0) {
                low_bound = 0;
            } else if (i == (state->grp_num_new - 1)) {
                up_bound = old_grp_num;
            }
        }

        wt_sum = 0;
        return_vec[i] = 0;
        for (j = low_bound; j < up_bound; j++) {

            /*  Accumulate sum based on type of averaging requested */
            switch (recip_flag) {

            case 0:                /* Use normal flux-weighted averaging */
                return_vec[i] += old_vec[j] * state->lib_weight[j];
                break;
            case 1:                /* Use reciprocal averaging */
                return_vec[i] += state->lib_weight[j] / old_vec[j];
                break;
            case 2:                /* Simply sum */
            case 3:
                return_vec[i] += old_vec[j];
                break;
            }

            wt_sum += state->lib_weight[j];
        }

        /* Divide by weighting factor */
        switch (recip_flag) {

        case 0:
            if (wt_sum > fabs((double) return_vec[i] * FLOAT_EPSILON)) {
                return_vec[i] /= wt_sum;
            } else {
                ierr = -12;
            }
            break;

        case 1:
            if (return_vec[i] > fabs((double) wt_sum * FLOAT_EPSILON)) {
                return_vec[i] = wt_sum / return_vec[i];
            } else {
                return_vec[i] = 0;
                ierr = 5;
            }
            break;

        case 2:
        case 3:
            break;
        }

    }

    return ierr;
}
