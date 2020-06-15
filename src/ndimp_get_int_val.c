#include <stdio.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

/**********************************************************************/
int ndimp_get_int_val(state_struct *h, int in_key, int *return_val)
{

    int i;
    int tmp_err = 0;
    int ierr = 0;
    int temp_val = 0;
    state_struct *statep;
    state_struct **handle;
    int num_parts;

    handle = h->handle_map;
    num_parts = h->handle_free;

    *return_val = 0;
    /* Check each particle is multigroup */
    /* Check num_parts <= MAX_HANDLE_NUM */
    switch (in_key) {

    case NDI_NUM_GRPS:
	/* Return sum */
	for (i = 0; i < num_parts; i++) {
	    if ((statep = (handle[i]))) {
		tmp_err = ndi_c_get_int_val_h(statep, in_key, &temp_val);
		*return_val += temp_val;
	    }
	}
	break;

    case NDI_PN_ORDER:
    case NDI_NUM_PNU_PR:
	/* Return maximum value */
	/* Note: may have to also check production matrices for pn_order */
	for (i = 0; i < num_parts; i++) {
	    if ((statep = (handle[i]))) {
		tmp_err = ndi_c_get_int_val_h(statep, in_key, &temp_val);
		if (*return_val < temp_val) {
		    *return_val = temp_val;
		}
	    }
	}
	break;

    case NDI_NUM_REAC:
	for (i = 0; i < num_parts; i++) {
	    if ((statep = (handle[i]))) {
		*return_val = ndimp_get_size(h, NDI_REAC_MT, &tmp_err);
	    }
	}
	break;

    case NDI_FISSIONABLE:
	/* Return true value, if any single-particle value is true */
	for (i = 0; i < num_parts; i++) {
	    if ((statep = (handle[i]))) {
		tmp_err = ndi_c_get_int_val_h(statep, in_key, return_val);
		if (*return_val) {
		    break;
		}
	    }
	}
	break;

    default:
	ierr = -4;
	break;

    }

    return ierr;
}
