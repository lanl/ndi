#include <stdio.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_var_type.h"

static int GET_COMB_FLOAT_VEC(state_struct *handle[], 
			      const int num_parts,
			      const int in_key, FLOAT_TYPE * return_vec,
			      const int vec_size);
static int GET_MPMG_CHI_MAT(state_struct *h,
                            int in_key, FLOAT_TYPE * return_vec,
                            int vec_size);

/**********************************************************************/
int NDIMP_GET_FLOAT_VEC(state_struct *h, int in_key,
                        FLOAT_TYPE * ret_vec,
			int vec_size)
{
    int ierr = 0;
    int num_parts;
    char mat_order;
    state_struct **handle;

    handle = h->handle_map;
    num_parts = h->handle_free;

    mat_order = h->mat_order;

    (void) NDI_INIT_FLOAT_VEC(ret_vec, vec_size);

    /* Check each particle is multigroup */
    /* Check num_parts <= MAX_HANDLE_NUM */

    switch (in_key) {

    case NDI_E_BOUNDS:
    case NDI_WGTS:
    case NDI_VEL:
    case NDI_E_AVG:
    case NDI_HEATING:
    case NDI_KERMA:
    case NDI_SIG_TOT:
    case NDI_NU_TOT:
    case NDI_NU_PR:
    case NDI_NU_DEL:
    case NDI_NU_SIG_F_TOT:
    case NDI_NU_SIG_F_PR:
    case NDI_NU_SIG_F_DEL:
    case NDI_CHI_VEC_TOT:
    case NDI_CHI_VEC_PR:
    case NDI_CHI_VEC_DEL:

	ierr =
	    GET_COMB_FLOAT_VEC(handle, num_parts, in_key, ret_vec,
			       vec_size);
	break;


    case NDI_CHI_MAT_TOT:
    case NDI_CHI_MAT_PR:
    case NDI_CHI_MAT_DEL:
        ierr = GET_MPMG_CHI_MAT(h, in_key, ret_vec, vec_size);
        if ((mat_order == 't') && (ierr >= 0)) {
            int num_grps, tmp_err;
            tmp_err = ndimp_get_int_val(h, NDI_NUM_GRPS,
                                        &num_grps);
            if ( (tmp_err != 0) &&
                 ((ierr >= 0) || (tmp_err < 0 )))
                ierr = tmp_err;
            (void)NDI_TRANSPOSE_FLOAT(ret_vec, num_grps, num_grps );
        }
        break;


    default:
	ierr = -4;
	break;
    }

    return (ierr);
}

/**********************************************************************/
static int GET_COMB_FLOAT_VEC(state_struct *handle[], 
			      const int num_parts,
			      const int in_key, FLOAT_TYPE *return_vec,
			      const int vec_size)
{

    int ierr = 0;
    int i;
    int tmp_err = 0;
    int curr_size = vec_size;
    int last_size = 0;
    FLOAT_TYPE *curr_p = return_vec;
    state_struct *statep;

    for (i = 0; i < num_parts; i++) {
	if (curr_size <= 0) {
	    ierr = 7;
	    break;
	}
	if ((statep = (handle[i]))) {
	    last_size = ndi_c_get_size_h(statep, in_key, &tmp_err);
	    tmp_err =
		NDI_GET_FLOAT_VEC_H(statep, in_key, curr_p, curr_size);
	    /* Check return error */
	    curr_size -= last_size;
	    curr_p += last_size;
	}
    }


    return (ierr);
}
/**********************************************************************/
static int GET_MPMG_CHI_MAT(state_struct *h,
                            int in_key, FLOAT_TYPE * return_vec,
                            int ret_size)

/*  For now the mpmg chi matrix is the chi matrix returned from each
    particle and placed "on the diagonal", the rest of the vector is
    set to zeroes.
*/
{

    int ierr = 0;
    int in;
    int row, col;
    int tmp_err = 0;
    int temp_size;
    int temp_mat_count = 0;
    int ret_index;
    int row_start = 0;
    int row_end = 0;
    int col_start = 0;
    int col_end = 0;
    int num_grps;
    int tot_num_grps;
    char transpose_state;
    FLOAT_TYPE *curr_p;
    FLOAT_TYPE *temp_mat;
    state_struct *in_part;
    state_struct **handle;
    int num_parts;

    (void) NDI_INIT_FLOAT_VEC(return_vec, ret_size);

    handle = h->handle_map;
    num_parts = h->handle_free;

    tmp_err =
	ndimp_get_int_val(h, NDI_NUM_GRPS, &tot_num_grps);

    for (in = 0; in < num_parts; in++) {
	if (!(in_part = (handle[in]))) {
	    return (-99);
	}
        /* NO TRANSPOSE */
        transpose_state = in_part->mat_order;
        in_part->mat_order = 'd';
        
	tmp_err = ndi_c_get_int_val_h(in_part, NDI_NUM_GRPS, &num_grps);
	row_end = row_start + num_grps;
        col_end = col_start + num_grps;

        temp_size = ndi_c_get_size_h(in_part, in_key, &tmp_err);
        temp_mat =
            (FLOAT_TYPE *) ndi_malloc(temp_size * sizeof(ndi_float64));
	tmp_err =
            NDI_GET_FLOAT_VEC_H(in_part, in_key, temp_mat, temp_size);

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
        row_start = row_end;
        in_part->mat_order = transpose_state;
    }				/* in loop */

    return (ierr);
}

/**********************************************************************/
