#include <stdio.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_var_type.h"

static int GET_MP_SCAT_MAT(const int *handle_arr, const int num_parts,
			   const int in_key, FLOAT_TYPE * return_vec,
			   const int vec_size);

/**********************************************************************/
int NDIMP_GET_FLOAT_VEC_N(const int *handle_arr, const int num_parts,
			  const int in_key, const int vec_num,
			  FLOAT_TYPE * ret_vec, const int vec_size)
{

    int ierr = 0;

    (void) NDI_INIT_FLOAT_VEC(ret_vec, vec_size);

    /* Check each particle is multigroup */
    /* Check num_parts <= MAX_HANDLE_NUM */

    switch (in_key) {

    case NDI_PN_FULL:
	ierr =
	    GET_MP_SCAT_MAT(handle_arr, num_parts, vec_num, ret_vec,
			    vec_size);
	break;

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}

/**********************************************************************/
static int GET_MP_SCAT_MAT(const int *handle_arr, const int num_parts,
			   const int pn_order, FLOAT_TYPE * return_vec,
			   const int ret_size)
{

    int ierr = 0;
    int in, out;
    int row, col;
    int tmp_err = 0;
    int tot_num_grps = 0;
    int temp_size;
    int temp_mat_count = 0;
    int ret_index;
    int row_start = 0;
    int row_end = 0;
    int col_start = 0;
    int col_end = 0;
    int num_grps_in, num_grps_out;
    int reset_curr_part_flag;
    char out_part_type_state[132];
    char out_part_type[132];
    ndi_float64 *ebounds_out = NULL;
    FLOAT_TYPE *curr_p;
    FLOAT_TYPE *temp_mat;
    state_struct *in_part;
    state_struct *out_part;


    tmp_err =
	ndimp_get_int_val(handle_arr, num_parts, NDI_NUM_GRPS,
			  &tot_num_grps);

    for (in = 0; in < num_parts; in++) {
	if (!(in_part = handle_to_state_p(handle_arr[in]))) {
	    return (-99);
	}
	tmp_err = ndi_c_get_int_val_h(in_part, NDI_NUM_GRPS, &num_grps_in);
	row_end = row_start + num_grps_in;
	col_start = 0;
	tmp_err =
	    ndi_c_check_option_h(in_part, NDI_CURR_PART,
				 out_part_type_state,
				 sizeof(out_part_type_state));
	if ((tmp_err != 9) && (tmp_err > 0)) {
	    reset_curr_part_flag = -1;
	} else {
	    reset_curr_part_flag = 0;
	}
	for (out = 0; out < num_parts; out++) {
	    if (!(out_part = handle_to_state_p(handle_arr[out]))) {
		return (-99);
	    }
	    /* Check that each particle is multigroup */
	    if (out_part->library_type != 'm') {
		return (-99);
	    }
	    tmp_err =
		ndi_c_get_int_val_h(out_part, NDI_NUM_GRPS, &num_grps_out);
	    col_end = col_start + num_grps_out;
	    if (in != out) {
		/* Set current particle parameters to outgoing particle */
		tmp_err =
		    ndi_c_check_option_h(out_part, NDI_PARTICLE_TYPE,
					 out_part_type,
					 sizeof(out_part_type));
		tmp_err =
		    ndi_c_set_option_h(in_part, NDI_CURR_PART,
				       out_part_type);
		if (tmp_err < 0) {
		    ierr = 9;
		    col_start = col_end;
		    continue;
		}

		temp_size =
		    ndi_c_get_size_h(out_part, NDI_E_BOUNDS, &tmp_err);
		ebounds_out =
		    (ndi_float64 *) ndi_malloc(temp_size *
					       sizeof(ndi_float64));
		/* Save e_bounds_curr_part state for later reset? */
		tmp_err =
		    ndi_c_set_float64_vec_option_h(in_part,
						   NDI_E_BOUNDS_CURR_PART,
						   ebounds_out, temp_size);
		if (ebounds_out)
		    ndi_free((void **) &ebounds_out);

		/* Get production matrix */
		temp_size =
		    ndi_c_get_size_n_h(in_part, NDI_PN_PROD_FULL_CURR_PART,
				       pn_order, &tmp_err);
		temp_mat =
		    (FLOAT_TYPE *) ndi_malloc(temp_size *
					      sizeof(FLOAT_TYPE));
		tmp_err =
		    NDI_GET_FLOAT_VEC_N_H(in_part,
					  NDI_PN_PROD_FULL_CURR_PART,
					  pn_order, temp_mat, temp_size);

		/* Reset current particle ebounds??? */

	    } else {
		/* Get scattering matrix */
		temp_size =
		    ndi_c_get_size_n_h(in_part, NDI_PN_FULL, pn_order,
				       &tmp_err);
		temp_mat =
		    (FLOAT_TYPE *) ndi_malloc(temp_size *
					      sizeof(FLOAT_TYPE));
		tmp_err =
		    NDI_GET_FLOAT_VEC_N_H(in_part, NDI_PN_FULL, pn_order,
					  temp_mat, temp_size);
	    }

	    /* Place values from temp_mat into appropriate place in the return_vec */
	    /* Indexing nightmare:  
	       rows represent incident particles; 
	       columns represent outgoing particles  */
	    temp_mat_count = 0;
	    curr_p = temp_mat;
	    for (row = row_start; row < row_end; row++) {
		for (col = col_start; col < col_end; col++) {
		    ret_index = (row * tot_num_grps) + col;
		    if ((ret_index < ret_size) &&
			(temp_mat_count < temp_size)) {
			return_vec[ret_index] = *curr_p;
			temp_mat_count++;
			curr_p++;
		    } else {
			ierr = 7;
			break;
		    }
		}
	    }
	    if (temp_mat)
		ndi_free((void **) &temp_mat);
	    col_start = col_end;
	}			/* out loop */
	if (reset_curr_part_flag) {
	    tmp_err =
		ndi_c_set_option_h(in_part, NDI_CURR_PART,
				   out_part_type_state);
	}
	row_start = row_end;
    }				/* in loop */

    return (ierr);
}
