#include <stdio.h>
#include <string.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static int get_num_unique_reac(state_struct *h, int *ierr);
static int get_sum_of_particle_sizes(state_struct *h, int in_key,
				     int *ierr);

/**********************************************************************/

int ndimp_get_size(state_struct *h, int in_key, int *ierr)
{

    int return_val = 0;
    state_struct **handle;
    int n;

    handle = h->handle_map;
    n = h->handle_free;
    int i;

    /* Check each particle is multigroup */
    /* Check num_parts <= MAX_HANDLE_NUM */

    switch (in_key) {

    case NDI_E_BOUNDS:
    case NDI_WGTS:
    case NDI_VEL:
    case NDI_E_AVG:
    case NDI_ENERGY_N:
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
	/* Sum of individual particle sizes */
	return_val = get_sum_of_particle_sizes(h, in_key, ierr);
	break;

    case NDI_REAC_MT:
	return_val = get_num_unique_reac(h, ierr);
	break;

	/* Matrix values */
    case NDI_CHI_MAT_TOT:
    case NDI_CHI_MAT_PR:
    case NDI_CHI_MAT_DEL:
	return_val = get_sum_of_particle_sizes(h, in_key, ierr);
	return_val *= return_val;
	break;

	/* All single values */
    case NDI_ZAID:		/*  ZAID  */
	return_val = strlen(handle[0]->iso_stat.zaid) + 1;
	break;
    case NDI_NUM_GRPS:
    case NDI_PN_ORDER:
    case NDI_NUM_PNU_PR:
    case NDI_NUM_REAC:
    case NDI_FISSIONABLE:
	return_val = 1;
	break;

        /* Multi-temperature (Reqt. 75) */
    case NDI_TEMPS:
        *ierr = -9;             /* Data not available */
        for (i = 0; i < n; i++)
            if (handle[i]->particle_type == 1) /* neutron */
                return_val = ndi_c_get_size_h(handle[i], in_key, ierr);
        break;

    default:
	*ierr = -4;
	return_val = 0;
	break;
    }

    return return_val;

}

/**********************************************************************/
static int get_sum_of_particle_sizes(state_struct *h, const int in_key,
				     int *ierr)
{

    int i;
    int tmp_err = 0;
    int tmp_val;
    int sum = 0;
    state_struct *statep;
    state_struct **handle;
    int num_parts;

    handle = h->handle_map;
    num_parts = h->handle_free;

    for (i = 0; i < num_parts; i++) {
	if ((statep = handle[i])) {
	    tmp_val = ndi_c_get_size_h(statep, in_key, &tmp_err);
	    if (tmp_val >= 0) {
		sum += tmp_val;
	    }
	}
    }
    return sum;
}

/**********************************************************************/
static int get_num_unique_reac(state_struct *h, int *ierr)
{

    int i;
    int tmp_err = 0;
    int max_size = 0;
    int num_unique_mts = 0;
    int tmp_val;
    int *unique_mts;
    state_struct *statep;
    state_struct **handle;
    int num_parts;

    handle = h->handle_map;
    num_parts = h->handle_free;

    for (i = 0; i < num_parts; i++) {
	if ((statep = handle[i])) {
	    tmp_err = ndi_c_get_int_val_h(statep, NDI_NUM_REAC, &tmp_val);
	    if (tmp_val >= 0) {
		max_size += tmp_val;
	    }
	}
    }

    unique_mts = (int *) ndi_calloc(max_size, sizeof(int));

    *ierr = ndimp_get_int_vec(h, NDI_REAC_MT, unique_mts, max_size);
    for (i = 0; i < max_size; i++) {
	if (unique_mts[i] > 0) {
	    num_unique_mts++;
	}
    }

    if (unique_mts)
	ndi_free((void **) &unique_mts);

    *ierr = tmp_err;
    return num_unique_mts;
}
