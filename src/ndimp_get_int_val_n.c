#include <stdio.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static int get_comb_reac_mt_n(state_struct *handle[], 
			      const int num_parts,
			      const int vec_num, int *return_val);
static int get_last_group_n(state_struct *handle[], 
			    const int num_parts,
			    const int vec_num, int *return_val);

/**********************************************************************/
int ndimp_get_int_val_n(state_struct *handle[], const int num_parts,
			const int in_key, const int vec_num, int *ret_val)
{

    int ierr = 0;

    *ret_val = 0;

    /* Check each particle is multigroup */
    /* Check num_parts <= MAX_HANDLE_NUM */

    switch (in_key) {

    case NDI_REAC_MT:
	ierr = get_comb_reac_mt_n(handle, num_parts, vec_num, ret_val);
	break;

    case NDI_LAST_GRP:
	ierr = get_last_group_n(handle, num_parts, vec_num, ret_val);
	break;

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}

/**********************************************************************/
static int get_comb_reac_mt_n(state_struct *handle[], 
			      const int num_parts,
			      const int vec_num, int *return_val)
{

    int ierr = 0;
    int vec_size = 0;
    int *mt_arr;

    *return_val = 0;

    vec_size = ndimp_get_size(handle, num_parts, NDI_REAC_MT, &ierr);
    mt_arr = (int *) ndi_malloc(vec_size * sizeof(int));
    ierr = ndimp_get_int_vec(handle, num_parts, NDI_REAC_MT,
			     mt_arr, vec_size);

    if (vec_num < vec_size) {
	*return_val = mt_arr[vec_num];
    }

    if (mt_arr)
	ndi_free((void **) &mt_arr);

    return (ierr);
}

/**********************************************************************/
static int get_last_group_n(state_struct *handle[], 
			    const int num_parts,
			    const int vec_num, int *return_val)
{

    int temp_val = 0;
    int ierr = 0;
    int sum = 0;
    int i;
    int end_index = vec_num;
    state_struct *statep;

    *return_val = 0;

    /* Should vec_num be part type? and translate to particle index? */

    if (end_index > num_parts) {
	return (-33);
    }

    for (i = 0; i <= end_index; i++) {
	if ((statep = (handle[i]))) {
	    ierr = ndi_c_get_int_val_h(statep, NDI_NUM_GRPS, &temp_val);
	    if (temp_val >= 0) {
		sum += temp_val;
	    }
	}
    }

    *return_val = sum;

    return (ierr);
}
