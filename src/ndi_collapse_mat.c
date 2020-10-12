
/*
 * File:        ndi_collapse_mat.c
 * Description: Internal function--collapse matrix using weighting function.
 * Time-stamp:  <1999-07-13 13:25:30 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"

/**********************************************************************/

#ifdef FLOAT32
int
ndi_collapse_mat32(const FLOAT_TYPE * old_mat, int old_grp_num_in,
                   int old_grp_num_out, int new_grp_num_in,
                   int new_grp_num_out, const int *new_ebounds_in,
                   const int *new_ebounds_out,
                   const ndi_float64 * local_weight,
                   FLOAT_TYPE * new_mat, int new_size)
{
#else
int
ndi_collapse_mat64(const FLOAT_TYPE * old_mat, int old_grp_num_in,
                   int old_grp_num_out, int new_grp_num_in,
                   int new_grp_num_out, const int *new_ebounds_in,
                   const int *new_ebounds_out,
                   const ndi_float64 * local_weight,
                   FLOAT_TYPE * new_mat, int new_size)
{
#endif

/*********************************************************************
    
    Purpose:  Collapse a square matrix using a weighting function.

    Parameters: 
    old_mat -- in: original, uncollapsed matrix.
    old_grp_num_out -- in: number of outgoing groups (#cols) in uncollapsed matrix.
    old_grp_num_in -- in: number of incident groups (#rows) in uncollapsed matrix.
    new_grp_num_out -- in: number of outgoing groups (#cols) in collapsed matrix.
    new_grp_num_in -- in: number of incident groups (#rows) in collapsed matrix.
    new_ebounds_in -- in: vector of integers representing the index of the old in ebounds
                      which are contained in the new ebounds (new ebounds subset of old)
    new_ebounds_out -- in:  vector of integers representing the index of the old out ebounds
                      which are contained in the new ebounds (new ebounds subset of old)
    new_mat -- out: resulting collapsed matrix
    new_size -- in: size of new_mat

    Assumptions:
    The new incident (and outgoing) energy boundaries is a subset of the old boundaries.

    Collapse by summing over columns (outgoing) and averaging over
    rows (incoming).

    Input variable 'new_ebounds_X' is a vector containing the indices of the 
    old (library) energy bounds which are the new collapsed energy bounds 
    (i.e. the collapsed energy boundaries are a subset of the old energy boundaries).


    Return values:  
    Returns negative value for error condition, positive value for 
    warning, and a '0' if everything's ok.  new_mat points to
    collapsed matrix.

    Note:  In the terminology used below, the incoming particle is
    described by the rows, the outgoing particle by the columns.  The
    "matrix" is a vector where the elements are given in row-major order
    (R0C0, R0C1, R0C2, R0C3,..., R1C0, R1C1, R1C2,...,RnC0, RnC1,...RnCn)
    Increasing rows and columns describe decreasing energy.

***********************************************************************/

    int i;                        /* counter over old incident energy groups (rows) */
    int j;                        /* counter over old outgoing energy groups (cols) */
    int k;                        /* counter over new incident energy groups (rows) */
    int l;                        /* counter over new outgoing energy groups (cols) */
    int ierr = 0;                 /* Error flag: 0=no error,pos=warning, neg=error */
    int curr_new_index;           /* Pointer into new (collapsed) vector */
    int curr_old_index;           /* Pointer into old (uncollapsed) vector */
    int low_bounds_in = 0;
    int up_bounds_in = 0;
    int low_bounds_out = 0;       /* Smallest old index contained in current new group */
    int up_bounds_out = 0;        /* Largest old index contained in current new group */
    FLOAT_TYPE in_sum = 0;        /* Summing rows */
    FLOAT_TYPE out_sum = 0;       /* Summing columns */
    FLOAT_TYPE wt_sum = 0;        /* Sum of weights */


    /* Check for non-valid matrix */
    if ((old_grp_num_in <= 0) || (old_grp_num_out <= 0)) {
        /*  Fill with zeroes and return */
        NDI_INIT_FLOAT_VEC(new_mat, new_size);
        ierr = 9;
        return ierr;
    }

    for (k = 0; k < new_grp_num_in; k++) {
        for (l = 0; l < new_grp_num_out; l++) {

            if (new_ebounds_in) {
                low_bounds_in = new_ebounds_in[k];
                up_bounds_in = new_ebounds_in[k + 1];
            } else {
                low_bounds_in = k;
                up_bounds_in = k + 1;
            }

            /* Outgoing groups must be "floored"--leftover "higher" and 
               "lower" groups must be put into first and last collaped
               groups.
             */
            if (new_ebounds_out) {
                low_bounds_out = new_ebounds_out[l];
                up_bounds_out = new_ebounds_out[l + 1];
            } else {
                low_bounds_out = l;
                up_bounds_out = l + 1;
            }
            if (l == 0) {
                low_bounds_out = 0;
            }
            if ((l + 1) == new_grp_num_out) {
                up_bounds_out = old_grp_num_out;
            }

            in_sum = 0;
            wt_sum = 0;
            for (i = low_bounds_in; i < up_bounds_in; i++) {
                out_sum = 0;
                for (j = low_bounds_out; j < up_bounds_out; j++) {
                    curr_old_index = (i * old_grp_num_out) + j;

                    if ((curr_old_index >=
                         (old_grp_num_in * old_grp_num_out))
                        || (curr_old_index < 0)) {
                        /* Bounds check */
                        ierr = -21;
                        return ierr;
                    }
                    out_sum += old_mat[curr_old_index];
                }                /* for j */

                in_sum += out_sum * local_weight[i];
                wt_sum += local_weight[i];

            }                    /* for i */

            if (wt_sum != 0) {
                curr_new_index = (k * new_grp_num_out) + l;
                if (curr_new_index >= new_size) {
                    /* Bounds check */
                    ierr = 7;
                    return ierr;
                }

                new_mat[curr_new_index] = in_sum / wt_sum;
            } else {
                /* Unexpected library data */
                ierr = -12;
            }

        }                        /* for l */
    }                            /* for k */

    return ierr;
}

/**********************************************************************/
