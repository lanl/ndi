#include <stdio.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static int get_comb_reac_mt(const int *handle_arr, const int num_parts,
			    int *return_vec, const int vec_size);

/**********************************************************************/
int ndimp_get_int_vec(const int *handle_arr, const int num_parts,
		      const int in_key, int *ret_vec, const int vec_size)
{

    int ierr = 0;

    ierr = ndi_init_int_vec(ret_vec, vec_size);

    /* Check each particle is multigroup */
    /* Check num_parts <= MAX_HANDLE_NUM */

    switch (in_key) {

    case NDI_REAC_MT:
	ierr = get_comb_reac_mt(handle_arr, num_parts, ret_vec, vec_size);
	break;

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}

/**********************************************************************/
static int get_comb_reac_mt(const int *handle_arr, const int num_parts,
			    int *return_vec, const int vec_size)
{

    int ierr = 0;
    int i, j, k;
    int tmp_err = 0;
    int max_size = 0;
    int num_unique_mts = 0;
    int curr_num = 0;
    int tmp_val;
    int *curr_mts;
    state_struct *statep;

    for (i = 0; i < num_parts; i++) {
	if ((statep = handle_to_state_p(handle_arr[i]))) {
	    tmp_err = ndi_c_get_int_val_h(statep, NDI_NUM_REAC, &tmp_val);
	    if (tmp_val >= 0) {
		max_size += tmp_val;
	    }
	}
    }

    curr_mts = (int *) ndi_calloc(max_size, sizeof(int));

    for (i = 0; i < num_parts; i++) {
	if ((statep = handle_to_state_p(handle_arr[i]))) {
	    tmp_err = ndi_c_get_int_val_h(statep, NDI_NUM_REAC, &curr_num);
	    if (curr_num > 0) {
		tmp_err =
		    ndi_c_get_int_vec_h(statep, NDI_REAC_MT, curr_mts,
					max_size);

		for (j = 0; j < curr_num; j++) {
		    if (num_unique_mts <= vec_size) {
			for (k = 0; k < num_unique_mts; k++) {
			    if (curr_mts[j] == return_vec[k]) {
				break;
			    }
			}
			if ((k < vec_size) &&
			    (curr_mts[j] != return_vec[k])) {
			    return_vec[num_unique_mts] = curr_mts[j];
			    num_unique_mts++;
			}
		    }
		}
	    }
	}
    }

    if (curr_mts)
	ndi_free((void **) &curr_mts);
    ierr = tmp_err;

    return (ierr);
}
