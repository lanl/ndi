
/*
 * File:	ndi_get_int_vec.c
 * Description:	User function--returns an integer vector.
 * Time-stamp:	<2000-02-01 17:39:56 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

static int get_int_vec_mg(state_struct *state, int in_key,
			  int *return_vec, int vec_size);
static int get_int_vec_tn(state_struct *state, int in_key,
			  int *return_vec, int vec_size);

/**********************************************************************/

int ndi_c_get_int_vec_h(state_struct *state, int in_key, int *return_vec,
			int vec_size)

/*  Purpose: Return an array of integers.
    
    Parameters: 
    in_key -- in: key to specify type of data required
    ret_val -- out: string value returned

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/
{
    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */


    ierr = ndi_init_int_vec(return_vec, vec_size); /* zero return vector */

    /* Check that ndi_init has been called. */
    if ((!state) || (!state->ndi_initialized)) {
	return (-2);
    }

    if (is_gendir(state)) {
	ierr = -2;		/* case not handled currently */
    } else if ((state->library_type == 'm') || 
	       (state->library_type == 'd')) {
	ierr = get_int_vec_mg(state, in_key, return_vec, vec_size);
    } else if (state->library_type == 's') {
	ierr = get_int_vec_tn(state, in_key, return_vec, vec_size);
    } else if (state->library_type == 'p') {
	ierr = ndimp_get_int_vec(state, in_key, return_vec, vec_size);
    } else if (state->library_type == ' ') {
	/* Library type was not set */
	ierr = -16;
    } else {
	/* Incompatible library type -- bad input key */
	ierr = -4;
    }

    return (ierr);
}

/**********************************************************************/
/**********************************************************************/
static int get_int_vec_mg(state_struct *state, int in_key,
			  int *return_vec, int vec_size)
{

    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */
    int n;
    int *curr_int = return_vec;
    int *source_ptr;

    if (!state) {
	return (-2);
    }


    /* Check that isotope is set */
    if (!state->iso_stat.set_flag) {
	return (-7);
    }

   /*************************/
    /*   Process input key   */
   /*************************/

    switch (in_key) {

    case NDI_RPRODS_ID:
	/* Vector of integer reaction product IDs ( for 
	   the target given by the global variable 'zaid') */

	if (!state->ndi_depl_set) {
	    ierr = ndi_set_default_depl(state);
	}
	if (state->num_target <= 0) {
	    return (-7);
	}
	ierr = ndi_get_rprod_vec_mg(state, return_vec, vec_size);
	break;

    case NDI_RPROD_ALL:
	/* Vector of reaction products for all reactions--all */
	if (state->iso_stat.rprod_all.num_objs > 0) {
	    for (n = 0; n < state->iso_stat.rprod_all.num_objs; n++) {
		*curr_int = state->iso_stat.rprod_all.stat[n].length;
		curr_int++;
	    }
	    (void) ndi_translate_fiss_prod(state, return_vec,
					   state->iso_stat.rprod_all.
					   num_objs);
	} else {
	    /* Error: data not available for this isotope */
	    ierr = -9;
	}
	break;

    case NDI_RPROD_RMO:
	/* Vector of reaction products for all reactions--rmo */
	if (state->iso_stat.rprod_rmo.num_objs > 0) {
	    for (n = 0; n < state->iso_stat.rprod_rmo.num_objs; n++) {
		*curr_int = state->iso_stat.rprod_rmo.stat[n].length;
		curr_int++;
	    }
	    (void) ndi_translate_fiss_prod(state, return_vec,
					   state->iso_stat.rprod_rmo.
					   num_objs);
	} else {
	    /* Error: data not available for this isotope */
	    ierr = -9;
	}
	break;

    case NDI_RPROD_FEW:
	/* Vector of reaction products for all reactions--all */
	if (state->iso_stat.rprod_few.num_objs > 0) {
	    for (n = 0; n < state->iso_stat.rprod_few.num_objs; n++) {
		*curr_int = state->iso_stat.rprod_few.stat[n].length;
		curr_int++;
	    }
	    (void) ndi_translate_fiss_prod(state, return_vec,
					   state->iso_stat.rprod_few.
					   num_objs);
	} else {
	    /* Error: data not available for this isotope */
	    ierr = -9;
	}
	break;

      /***********************************************************************/
	/*           This section looks at non-standard isotope data           */
      /***********************************************************************/

    case NDI_REAC_MT:		/* Vector containing number of reaction mts */
	if ((state->iso_stat.num_reac > 0) && (state->iso_stat.reac_mt)) {
	    source_ptr = state->iso_stat.reac_mt;
	    for (n = 0; n < state->iso_stat.num_reac; n++) {
                if (state->reac_id == 'c') {
                    *curr_int = ndi_c_mt_to_c(*source_ptr);
                    if (*curr_int <= 0) {
                        ierr = -24;
                    }
                } else {
 		*curr_int = *source_ptr;
                }
		curr_int++;
		source_ptr++;
	    }
	} else {
	    /* Error: data not available for this isotope */
	    ierr = -9;
	}
	break;


      /**********  UNKNOWN KEYWORD  ***********/

    default:
	ierr = -4;
	break;
    }

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/
static int get_int_vec_tn(state_struct *state, int in_key,
			  int *return_vec, int return_size)
{

    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */
    int num_elements;


    if (!state) {
	return (-2);
    }

    /* Check that isotope is set */
    if (!state->tn_stat.set_flag) {
	return (-7);
    }

   /*************************/
    /*   Process input key   */
   /*************************/

    switch (in_key) {

    case NDI_RPRODS_ID:
	/* Vector of reaction IDs */
	if (state->tn_stat.rprods.length > 0) {
	    num_elements = state->tn_stat.rprods.length;
	    ierr =
		ndi_read_rx_prod(state->lib_stream,
				 state->tn_stat.rprods.file_ptr,
				 &return_vec, ID_PARAM, &num_elements,
				 NO_MT);
	} else {
	    ierr = ndi_check_array_err(state->tn_stat.rprods.length);
	}
	break;

    case NDI_RPRODS_MLT:
	/* Vector of reaction product multiplicities */
	if (state->tn_stat.rprods.length > 0) {
	    num_elements = state->tn_stat.rprods.length;
	    ierr =
		ndi_read_rx_prod(state->lib_stream,
				 state->tn_stat.rprods.file_ptr,
				 &return_vec, MULT_PARAM, &num_elements,
				 NO_MT);
	} else {
	    ierr = ndi_check_array_err(state->tn_stat.rprods.length);
	}
	break;

    case NDI_REACTANTS_ID:
	/* Vector of reaction IDs */
	if (state->tn_stat.reactants.length > 0) {
	    num_elements = state->tn_stat.reactants.length;
	    ierr =
		ndi_read_rx_prod(state->lib_stream,
				 state->tn_stat.reactants.file_ptr,
				 &return_vec, ID_PARAM, &num_elements,
				 NO_MT);
	} else {
	    ierr = ndi_check_array_err(state->tn_stat.reactants.length);
	}
	break;

    case NDI_REACTANTS_MLT:
	/* Vector of reaction product multiplicities */
	if (state->tn_stat.reactants.length > 0) {
	    num_elements = state->tn_stat.reactants.length;
	    ierr =
		ndi_read_rx_prod(state->lib_stream,
				 state->tn_stat.reactants.file_ptr,
				 &return_vec, MULT_PARAM, &num_elements,
				 NO_MT);
	} else {
	    ierr = ndi_check_array_err(state->tn_stat.reactants.length);
	}
	break;

    case NDI_SIGVBAR_INTERP_REG:
	/* Vector giving end of sigvbar interpolation regions */
	if (state->tn_stat.int_sigvbars.length > 0) {
	    num_elements = state->tn_stat.int_sigvbars.length;
	    ierr =
		ndi_read_rx_prod(state->lib_stream,
				 state->tn_stat.int_sigvbars.file_ptr,
				 &return_vec, BOUNDARY_PARAM,
				 &num_elements, NO_MT);
	} else {
	    ierr = ndi_check_array_err(state->tn_stat.int_sigvbars.length);
	}
	break;

    case NDI_SIGVBAR_INTERP_TYPE:
	/* Vector of interpolation types for each sigvbar interp region */
	if (state->tn_stat.int_sigvbars.length > 0) {
	    num_elements = state->tn_stat.int_sigvbars.length;
	    ierr =
		ndi_read_rx_prod(state->lib_stream,
				 state->tn_stat.int_sigvbars.file_ptr,
				 &return_vec, TYPE_PARAM, &num_elements,
				 NO_MT);
	} else {
	    ierr = ndi_check_array_err(state->tn_stat.int_sigvbars.length);
	}
	break;

    case NDI_EINBAR_INTERP_REG:
	/* Vector giving end of einbar interpolation regions */
	if (state->tn_stat.int_einbars.length > 0) {
	    num_elements = state->tn_stat.int_einbars.length;
	    ierr =
		ndi_read_rx_prod(state->lib_stream,
				 state->tn_stat.int_einbars.file_ptr,
				 &return_vec, BOUNDARY_PARAM,
				 &num_elements, NO_MT);
	} else {
	    ierr = ndi_check_array_err(state->tn_stat.int_einbars.length);
	}
	break;

    case NDI_EINBAR_INTERP_TYPE:
	/* Vector of interpolation types for each einbar interp region */
	if (state->tn_stat.int_einbars.length > 0) {
	    num_elements = state->tn_stat.int_einbars.length;
	    ierr =
		ndi_read_rx_prod(state->lib_stream,
				 state->tn_stat.int_einbars.file_ptr,
				 &return_vec, TYPE_PARAM, &num_elements,
				 NO_MT);
	} else {
	    ierr = ndi_check_array_err(state->tn_stat.int_einbars.length);
	}
	break;

    case NDI_SEC_PART_TYPES:
	/* Vector of secondary particle types */
	if ((state->sec_part_filled > 0) && state->tn_stat.sec_part) {
	    int i;

	    for (i = 0; i < state->sec_part_filled; i++) {
		if (i < return_size) {
		    return_vec[i] = state->tn_stat.sec_part[i].part_type;
		} else {
		    ierr = 7;
		    break;
		}
	    }
	} else {
	    ierr = ndi_check_array_err(state->tn_stat.num_sec_parts);
	}
	break;

      /**********  UNKNOWN KEYWORD  ***********/

    default:
	ierr = -4;
	break;
    }

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/
