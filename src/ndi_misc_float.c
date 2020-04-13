
/*
 * File:	ndi_misc_float.c
 * Description:	Miscellaneous functions operating with floats.

 * Time-stamp:	<2003-03-24 13:51:13 jomc>
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

/* Fill vec with num_vals copies of val.

   Return 0 if sucessful
          7 if vec_len < num_val
*/
int NDI_CONST_FLOAT_VEC(FLOAT_TYPE val, int num_vals, 
                        FLOAT_TYPE *vec, int vec_len) 
{
    int i;
    int n;
    int ierr = 0;

    if (vec_len < num_vals) {
        n = vec_len;
        ierr = 7;
    } else {
        n = num_vals;
    }

    for (i=0; i < n; i++)
        vec[i] = val;

    return ierr;
}

/**********************************************************************/

int NDI_INIT_FLOAT_VEC(FLOAT_TYPE * vec, int num_elements)
{

    int i;
    int ierr = 0;
    FLOAT_TYPE *curr_vec = vec;

    for (i = 0; i < num_elements; i++) {
	*curr_vec = 0.0;
	curr_vec++;
    }
    return (ierr);
}

/*********************************************************************/
int GET_SIMPLE_FLOAT_VEC(state_struct *state, FILE *stream,
			 long int f_ptr, int collapse_type,
			 int library_grp_num, FLOAT_TYPE * return_vec,
			 int return_size, int binary)

     /* Reads data from stream begining at f_ptr.  If collapse_flag
        is set (and collapse_type != 0), then collapse into the
        return_vec array.  Method of collapse is specified by variable
        collapse_type:
        < 0:  Do not collapse.
        = 0:  use normal weighted averaging.
        = 1:  weighted average of the reciprocal (e.g. velocities).
        = 2:  simply sum with "flooring"--see Note (e.g. chi vectors).
        = 3:  simply sum with no "flooring" (e.g. weights ).
      */
{
    /* Add return_size */

    /*  collapse_type < 0 means this data should not be collapsed */

    int ierr = 0;
    FLOAT_TYPE *temp_vec = NULL;

    if (!state) {
	return (-2);
    }

    if (f_ptr > 0) {
	if ((collapse_type >= 0) && (state->collapse_flag)) {
	    temp_vec =
		(FLOAT_TYPE *) ndi_malloc(library_grp_num *
					  sizeof(FLOAT_TYPE));
	    if (!temp_vec) {
		/* Out of memory */
		return (-14);
	    }
	    ierr = NDI_READ_FLOAT_BLOCK(stream, f_ptr,
					library_grp_num,
					temp_vec, library_grp_num, binary);
	    ierr = NDI_COLLAPSE_FLOAT(state, temp_vec, library_grp_num,
				      collapse_type,
				      return_vec, return_size);
	    if (temp_vec)
		ndi_free((void **) &temp_vec);
	} else {
	    ierr = NDI_READ_FLOAT_BLOCK(stream, f_ptr,
					library_grp_num,
					return_vec, return_size, binary);
	}
    } else {
	/* Error: data not available */
	ierr = -9;
    }

    return (ierr);
}


/*********************************************************************/
int NDI_TRANSPOSE_FLOAT(FLOAT_TYPE * in_mat, int row_num, int col_num)
{
    FLOAT_TYPE *temp_flt;
    int row;
    int col;
    int ierr = 0;
    int new_indx = 0;

    temp_flt =
	(FLOAT_TYPE *) ndi_malloc((row_num * col_num) *
				  sizeof(FLOAT_TYPE));
    if (temp_flt) {
	for (col = 0; col < col_num; col++) {
	    for (row = 0; row < row_num; row++) {
		temp_flt[new_indx] = in_mat[(row * col_num) + col];
		new_indx++;
	    }
	}

	ierr = COPY_MATRIX(in_mat, row_num*col_num, 
                           temp_flt, row_num*col_num);
	ndi_free((void **) &temp_flt);
    } else {
	/*Out of memory */
	ierr = -14;
    }

    return (ierr);
}

/*********************************************************************/

int NDI_VEC2MAT(const FLOAT_TYPE * vec, int num_vec,
		FLOAT_TYPE * return_mat, int num_mat, int mat_size)
{

    int ierr = 0;
    int i = 0;
    int j = 0;
    int mat_cnt = 0;
    int rows = num_vec;
    int cols;

    if (rows != 0) {
	cols = num_mat / rows;
    } else {
	return (ierr);
    }

    for (j = 0; j < cols; j++) {
	for (i = 0; i < num_vec; i++) {
	    if (mat_cnt < mat_size) {
		return_mat[mat_cnt] = vec[i];
		mat_cnt++;
	    } else {
		/* matrix is not big enough */
		ierr = 7;
		return (ierr);
	    }
	}
    }
    return (ierr);

}

/*********************************************************************/
/*********************************************************************/

int GET_SIG_REAC(state_struct *state, isotope_status * mg, int vec_num,
		 FLOAT_TYPE * return_vec, int *return_size, int binary)

/*  Purpose:  Returns the cross-section for particle 'mg' for the 
    reaction indicated by the mt corresponding to 'vec_num'.
    
    Parameters:
    mg -- in:pointer to multigroup status structure.
    return_vec -- out:array to fill with average energy; if no data
        available, will fill return_vec with the number of zeroes
	corresponding to the number of groups.
    return_size -- in/out:on input shows the size of the array vec;
        on output shows the size remaining in array ( i.e. original 
        return_size minus the number of elements filled ).  May return
	a negative value if return_size was not large enough.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/
{
    int ierr = 0;
    int reac_index;
    int mt = 0;

    if (!state) {
	return (-2);
    }

    if (state->reac_id == 'c') {
	mt = ndi_c_c_to_mt(vec_num);
	if (mt <= 0) {
	    ierr = -24;
	    return (ierr);	/* Invalid MT or reaction identifier */
	}
    } else {
	mt = vec_num;
    }

    ierr = ndi_find_reac_index(mg->reac_mt, &reac_index, mt, mg->num_reac);

    if ((ierr >= 0) && (mg->sig_reac.stat[reac_index].file_ptr)) {
	/* Reaction cross-section exists for this isotope--
	   set temp_size and temp_vec */

	ierr = GET_SIMPLE_FLOAT_VEC(state, state->lib_stream,
				    mg->sig_reac.stat[reac_index].file_ptr,
				    0, mg->num_grps, return_vec,
				    *return_size, binary );

    } else {			/* ierr >= 0 ... */
	/* Error: no data for this isotope */
	ierr = NDI_INIT_FLOAT_VEC(return_vec, *return_size);
	ierr = 9;
    }

    /* Check fission reactions for non-fissionable isotope */
    if ((mt == 18) || (mt == 19) || (mt == 20) || (mt == 21) || (mt == 38)) {
	(void) check_fissionable_err(&ierr, mg->fissionable);
    }
    return (ierr);
}

/*********************************************************************/
/*********************************************************************/

int COPY_MATRIX(FLOAT_TYPE *dest, int d_size, FLOAT_TYPE *source, int s_size)
{
    int ierr = 0;
    int i, n;

    if (d_size < s_size) {
        n = d_size;
        ierr = 7;               /* Data truncated */
    } else {
        n = s_size;
    }
   
    for (i = 0; i < n; i++) {
	dest[i] = source[i];
    }

    return (ierr);
}

/*********************************************************************/

int MULT_FLOAT_VEC(state_struct *state, const ndi_float64 * vec1,
		   const ndi_float64 * vec2, int num_elements,
		   int collapse, FLOAT_TYPE * return_vec, int return_size)
{
    /*  collapse < 0 means that this vector should not be collapsed */

    int ierr = 0;
    int tmp_err = 0;
    int i;
    int max_result = num_elements;
    int result_size = return_size;
    FLOAT_TYPE *temp_vec = NULL;
    FLOAT_TYPE *result_p = return_vec;

    if (!state) {
	return (-2);
    }

    if ((state->collapse_flag != 0) && (collapse >= 0)) {
	/* Check global collapse_flag to see if collapsing is set;
	   also check local collapse to see what type of collapse (if any)
	 */
	temp_vec =
	    (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
	/* Check for null */
	result_p = temp_vec;
	result_size = num_elements;
    }
    if (max_result > result_size) {
	ierr = 7;
	max_result = result_size;
    }
    /* check max_result */
    for (i = 0; i < max_result; i++) {
	*result_p = vec1[i] * vec2[i];
	result_p++;
    }

    /* collapse if necessary */

    if ((state->collapse_flag != 0) && (collapse >= 0)) {
	tmp_err = NDI_COLLAPSE_FLOAT(state, temp_vec, num_elements,
				     collapse, return_vec, return_size);
	if ((ierr == 0) || (tmp_err < 0)) {
	    ierr = tmp_err;
	}
    }
    if (temp_vec) {
	ndi_free((void **) &temp_vec);
    }

    return (ierr);
}

/*********************************************************************/
int DIVIDE_FLOAT_VEC(state_struct *state, const ndi_float64 * vec1,
		     const ndi_float64 * vec2, int num_elements,
		     int collapse, FLOAT_TYPE * return_vec,
		     int return_size)
{


    /*  collapse < 0 means that this vector should not be collapsed
       Note:  when the divisor is 0--no error is generated, the result is set to 0 */

    int ierr = 0;
    int tmp_err = 0;
    int i;
    int max_result = num_elements;
    int result_size = return_size;
    FLOAT_TYPE *temp_vec = NULL;
    FLOAT_TYPE *result_p = return_vec;

    if (!state) {
	return (-2);
    }

    if ((state->collapse_flag != 0) && (collapse >= 0)) {
	/* Check global collapse_flag to see if collapsing is set;
	   also check local collapse to see what type of collapse (if any)
	 */
	temp_vec =
	    (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
	/* Check for null */
	result_p = temp_vec;
	result_size = num_elements;
    }
    if (max_result > result_size) {
	ierr = 7;
	max_result = result_size;
    }
    /* check max_result */
    for (i = 0; i < max_result; i++) {
	if (vec2[i] != 0) {
	    *result_p = vec1[i] / vec2[i];
	} else {
	    *result_p = 0.0;
	}
	result_p++;
    }

    /* collapse if necessary */

    if ((state->collapse_flag != 0) && (collapse >= 0)) {
	tmp_err = NDI_COLLAPSE_FLOAT(state, temp_vec, num_elements,
				     collapse, return_vec, return_size);
	if ((ierr == 0) || (tmp_err < 0)) {
	    ierr = tmp_err;
	}
    }
    if (temp_vec) {
	ndi_free((void **) &temp_vec);
    }

    return (ierr);
}

/*********************************************************************/
int NORM_FLOAT_VEC(FLOAT_TYPE * vec_p, int num_elements,
		   const FLOAT_TYPE norm_val)
{

    /* Assumes that the numbers will be non-negative */

    int ierr = 0;
    int i;
    FLOAT_TYPE *temp_p = vec_p;
    FLOAT_TYPE vec_sum = 0.0;

    /* find current normalization */
    for (i = 0; i < num_elements; i++) {
	vec_sum += *temp_p;
	temp_p++;
    }
    if ((vec_sum != norm_val) && (vec_sum != 0.0)) {
	temp_p = vec_p;
	for (i = 0; i < num_elements; i++) {
	    *temp_p /= vec_sum;
	    temp_p++;
	}
    }
    return (ierr);

}

/*********************************************************************/
int GET_ABUNDANCE(int izaid, FLOAT_TYPE * return_val)
{

#include "ndi_abundance.h"

    char zaid_char[16];
    int i;
    int ierr = 0;
    int a;
    int element_in;
    int element_curr;
    int element_found_flag = 0;	/* If isotope of same element as izaid 
				   is found, then = 1, else = 0 */

    *return_val = 0.0;

    /* Get element of input isotope */
    sprintf(zaid_char, "%d", izaid);
    ierr = ndi_parse_mg_zaid(zaid_char, &element_in, &a);

    for (i = 0; i < num_of_abundance_values; i++) {
	/* Check element of current isotope */
	sprintf(zaid_char, "%d", abundance_id[i]);
	ierr = ndi_parse_mg_zaid(zaid_char, &element_curr, &a);
	if (element_curr == element_in) {
	    element_found_flag = -1;
	}

	/* Simple-minded look-up */
	if (abundance_id[i] == izaid) {
	    *return_val = (FLOAT_TYPE) abundance_value[i];
	    return (ierr);
	}
    }

    /*  Check to see if this isotope has zero abundance (i.e. another
       isotope of this element is given) or no data (i.e. no other 
       element of this isotope is given 
     */

    if (element_found_flag == 0) {
	/* No data is given--set warning */
	ierr = 9;
    }

    return (ierr);

}

/*********************************************************************/
int CHI_MAT_TO_VEC(state_struct *state, FILE *stream,
		   long int chi_mat_ptr, FLOAT_TYPE * nu_sig_f,
		   FLOAT_TYPE * return_vec, int return_size,
		   int collapse_type, int lib_num_grp, int binary)

     /*  NOTE: lib_num_grp is the number of groups on the library */
{

    int ierr = 0;
    int in;			/* Number of incoming energy groups */
    int out;			/* Number of outgoing energy groups */
    FLOAT_TYPE *chi_matrix = NULL;
    FLOAT_TYPE *weights = NULL;
    FLOAT_TYPE *temp_vec = NULL;
    FLOAT_TYPE norm;

    if (!state) {
	return (-2);
    }

    if (chi_mat_ptr > 0) {

	/* Read chi matrix from file  */
	chi_matrix = (FLOAT_TYPE *) ndi_malloc(lib_num_grp * lib_num_grp
					       * sizeof(FLOAT_TYPE));
	if (!chi_matrix) {
	    /* Out of memory */
	    return (-14);
	}
	/* Do not want the matrix collapsed so use read_float_block instead of
	   get_simple_float_vec ( or use collapse_flag == -1) */
	ierr = NDI_READ_FLOAT_BLOCK(stream, chi_mat_ptr,
				    lib_num_grp * lib_num_grp,
				    chi_matrix, lib_num_grp * lib_num_grp,
				    binary);

	/* Get wgts for averaging */
	if (state->iso_stat.wgts.file_ptr > 0) {

	    weights =
		(FLOAT_TYPE *) ndi_malloc(lib_num_grp *
					  sizeof(FLOAT_TYPE));
	    /* Check returned value */
	    ierr =
		NDI_READ_FLOAT_BLOCK(stream, state->iso_stat.wgts.file_ptr,
				     lib_num_grp, weights, lib_num_grp,
				     binary);
	}

	/* Flux-average matrix to form vector */
	temp_vec =
	    (FLOAT_TYPE *) ndi_malloc(lib_num_grp * sizeof(FLOAT_TYPE));
	/* Check returned value */
	norm = 0.0;
	for (out = 0; out < lib_num_grp; out++) {
	    temp_vec[out] = 0.0;
	    for (in = 0; in < lib_num_grp; in++) {
		temp_vec[out] += nu_sig_f[in] * weights[in] *
		    chi_matrix[in * lib_num_grp + out];
	    }
	    norm += temp_vec[out];
	}

	/* normalize */
	if (norm != 0.0) {
	    for (out = 0; out < lib_num_grp; out++) {
		temp_vec[out] /= norm;
	    }
	}

	/* Collapse vector, if necessary */
	/* Check this call */
	/* What about checking return_size ? */

	if (state->collapse_flag) {
	    ierr =
		NDI_COLLAPSE_FLOAT(state, temp_vec, lib_num_grp,
				   collapse_type, return_vec, return_size);
	} else {
	    /* This is really ugly--do something better! */
	    for (out = 0; out < lib_num_grp; out++) {
		if (out < return_size) {
		    return_vec[out] = temp_vec[out];
		} else {
		    ierr = 7;
		    break;
		}
	    }
	}

	if (chi_matrix) {
	    ndi_free((void **) &chi_matrix);
	}
	if (weights) {
	    ndi_free((void **) &weights);
	}
	if (temp_vec) {
	    ndi_free((void **) &temp_vec);
	}

    }

    return (ierr);

}


/*********************************************************************/

/*********************************************************************/
