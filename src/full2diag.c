#include "ndi_var_type.h"

/*  Purpose:  Takes a square scattering matrix and forms a "downscatter
    diagonal".  It will take the upscatter and add this into the 
    self-scatter.  Energy groups are required to be in descending
    order, rows represent the incoming particles, columns represent
    the outgoing (scattered) particles.  So within a row, numbers to
    the 'left' of the diagonal are summed into the diagonal.
    
    Parameters:
    full_mat --in:float array containing the full scattering matrix.
    num_grps -- in:number of energy groups; full_mat is assumed to
                have a size of num_grps x num_grps.
    return_vec -- out:array to fill with pn downcatter vector
    return_size -- in:the size of the array return_vec;

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/

static int NDI_FULL2DIAG(FLOAT_TYPE * full_mat, int num_grps,
                         FLOAT_TYPE * return_vec, int return_size)
{
    int ierr = 0;
    int row = 0;
    int col = 0;
    int num_row = num_grps;
    int num_col = num_grps;
    int ret_count = 0;
    FLOAT_TYPE *mat_ptr = full_mat;
    FLOAT_TYPE upscat_sum;

    if ((full_mat) && (return_vec)) {
        for (row = 0; row < num_row; row++) {
            upscat_sum = 0.0;
            for (col = row; col < num_col; col++) {
                if (((mat_ptr - full_mat) < (num_row * num_col))
                    && (ret_count < return_size)) {
                    if (col < row) {
                        upscat_sum += *mat_ptr;
                    } else if (col == row) {
                        return_vec[ret_count] = *mat_ptr + upscat_sum;
                    } else {
                        ret_count++;
                        return_vec[ret_count] = *mat_ptr;
                        ret_count++;
                    }
                    mat_ptr++;
                } else {
                    ierr = 7;
                    break;
                }
            }
            if (((mat_ptr - full_mat) >= (num_row * num_col))
                || (ret_count >= return_size)) {
                break;
            }
        }

    } else {
        if (!full_mat) {
            /* Bad matrix -- internal NDI error */
            ierr = -13;
        } else if (!return_vec) {
            /* Bad vector input by user */
            ierr = -25;
        }
    }
    return ierr;
}

/*********************************************************************/
/*********************************************************************/
