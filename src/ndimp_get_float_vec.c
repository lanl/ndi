#include <stdio.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_var_type.h"

static int GET_COMB_FLOAT_VEC(state_struct *handle[], 
			      const int num_parts,
			      const int in_key, FLOAT_TYPE * return_vec,
			      const int vec_size);

/**********************************************************************/
int NDIMP_GET_FLOAT_VEC(state_struct *h, int in_key,
                        FLOAT_TYPE * ret_vec,
			int vec_size)
{
    int ierr = 0;
    state_struct **handle;
    int num_parts;

    handle = h->handle_map;
    num_parts = h->handle_free;

    (void) NDI_INIT_FLOAT_VEC(ret_vec, vec_size);

    /* Check each particle is multigroup */
    /* Check num_parts <= MAX_HANDLE_NUM */

    switch (in_key) {

    case NDI_E_BOUNDS:
    case NDI_WGTS:
    case NDI_VEL:
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

/*
    case NDI_CHI_MAT_TOT:
    case NDI_CHI_MAT_PR:
    case NDI_CHI_MAT_DEL:
        if ((mat_order == 't') && (ierr >= 0))
            TRANSPOSE_FLOAT(ret_vec, );
        break;
*/

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
