
/*
 * File:	ndi_set_float_vec_option.c
 * Description:	User function--set defaults.
 * Time-stamp:	<2003-10-24 17:49:16 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ndi.h"
#include "ndi_internal.h"

static int ndi_set_collapse(state_struct * state,
			    const ndi_float64 * ebound_array,
			    const int new_num_grps);
static int ndi_set_ebounds_curr_part(state_struct * state,
				     const ndi_float64 * ebound_array,
				     const int new_num_grps);

/**********************************************************************/
int
ndi_c_set_float64_vec_option_h(state_struct * state, const int in_key,
			       const ndi_float64 * option_array,
			       const int array_size)

/*  Purpose: Set options/defaults.

    Parameters:
    in_key -- in: key to specify option to set.
    option_array -- in: array of floats specifying the value of the option.
    array_size -- in:integer specifying the size of option_array.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok.

***********************************************************************/
{
    int ierr = 0;		/* Error flag: 0=no error, pos=warning, neg=error */

    /* Check that ndi_init has been called. */
    if ((!state) || (!state->ndi_initialized)) {
	return (-2);
    }

    /*** Check that NDI is initialized ***/

    /*************************/
    /*   Process input key   */
    /*************************/
    switch (in_key) {

    case NDI_COLLAPSE:
	/* Set energy groups to collapse to */
	ierr = ndi_set_collapse(state, option_array, array_size - 1);
	break;

    case NDI_E_BOUNDS_CURR_PART:
	/* Set energy groups to collapse secondary particle */
	ierr =
	    ndi_set_ebounds_curr_part(state, option_array, array_size - 1);
	break;

	/**********  UNKNOWN KEYWORD  ***********/

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}


/**********************************************************************/
/*   FUNCTIONS INTERNAL TO FILE  */
/**********************************************************************/

static int
ndi_set_collapse(state_struct * state,
		 const ndi_float64 * ebound_array, const int new_num_grps)

/* Valid input values:
   30_group, 21_group, 16_group, 14_group, 13_group, 12_group, 10_group,
   9_group, 8_group, 7_group, 6_group, 5_group, 4_group, 3_group, 
   2_group, 1_group, 16_LLNL, default

   Global variables changed (and valid values):
   grp_num_new -- integer giving number of groups; 0 for default or error
   new_ebounds -- array of indices of lib_ebounds array with new energy 
   boundaries (i.e. new boundaries subset of library bounds).
   collapse_flag -- 0 = no collapsing required, -1 = collapsing required
   n_group -- (same as valid input values)

   -------------------------------------------------------------------------*/
{

    int ierr = 0;
    int i;
    int old_num_grps = 0;
    ndi_float64 last_bound = -1;
    double *curr_p = NULL;

    if (!state) {
	return (-2);
    }

    /* Check that number of groups is reasonable */
    if (new_num_grps <= 0) {
	return (-23);
    }

    /* Check appropriate library type */
    if ((state->library_type != 'm') &&
	(state->library_type != 'd') && (state->library_type != 's')) {
	return (-23);
    }

    /* Check monotonically decreasing and
       copy ebounds into state->collapse_ebounds array 
       for passing into ndi_set_e_grps */

    if (state->collapse_ebounds)
	ndi_free((void **) &state->collapse_ebounds);
    state->collapse_ebounds =
	(double *) ndi_calloc(new_num_grps + 1, sizeof(double));
    curr_p = state->collapse_ebounds;
    state->grp_num_new = new_num_grps;
    state->collapse_flag = -1;

    for (i = 0; i < new_num_grps + 1; i++) {
	if ((i == 0) || ((i > 0) && (ebound_array[i] < last_bound))) {
	    *curr_p = (double) ebound_array[i];
	    last_bound = ebound_array[i];
	    curr_p++;
	} else {
	    if (i > 0) {
		state->collapse_flag = 0;
		strcpy(state->n_group, "default");
		if (state->collapse_ebounds) {
		    ndi_free((void **) &state->collapse_ebounds);
		}
		return (-23);
	    }
	}
    }

    if ((state->library_type == 'm') || (state->library_type == 'd')) {
	/* Multigroup or dosimetry */
	old_num_grps = state->iso_stat.num_grps;
	ierr =
	    ndi_set_e_grps(old_num_grps, state->lib_ebounds,
			   &state->grp_num_new, &state->collapse_ebounds,
			   "user", state->library_type,
			   &state->new_ebounds, &state->collapse_flag);

    }

    if (ierr >= 0) {
	strcpy(state->n_group, "user");
    } else {
	strcpy(state->n_group, "default");
    }
    return (ierr);
}

/**********************************************************************/
/**********************************************************************/

static int
ndi_set_ebounds_curr_part(state_struct * state,
			  const ndi_float64 * ebound_array,
			  const int new_num_grps)

/* Valid input values:
   30_group, 21_group, 16_group, 14_group, 13_group, 12_group, 10_group,
   9_group, 8_group, 7_group, 6_group, 5_group, 4_group, 3_group, 
   2_group, 1_group, 16_LLNL, default

   Global variables changed (and valid values):
   grp_num_new -- integer giving number of groups; 0 for default or error
   new_ebounds -- array of indices of lib_ebounds array with new energy 
   boundaries (i.e. new boundaries subset of library bounds).
   collapse_flag -- 0 = no collapsing required, -1 = collapsing required
   n_group -- (same as valid input values)

   -------------------------------------------------------------------------*/
{

    int ierr = 0;
    int i;
    ndi_float64 last_bound = -1;
    double *curr_p = NULL;
    sec_part_mg *part = NULL;

    if (!state) {
	return (-2);
    }

    /* Check that number of groups is reasonable */
    if (new_num_grps <= 0) {
	return (-23);
    }

    if ((state->library_type == 'm') || (state->library_type == 'd')) {
	/* Multigroup or dosimetry */
	if (state->iso_stat.sec_part &&
	    (state->curr_part_index >= 0) &&
	    (state->curr_part_index < state->iso_stat.num_sec_parts)) {
	    part = &state->iso_stat.sec_part[state->curr_part_index];
	    if (!part) {
		return (-91);
	    }

	    /* Check monotonically decreasing and
	       copy ebounds into state->collapse_ebounds array 
	       for passing into ndi_set_e_grps */

	    if (part->collapse_ebounds)
		ndi_free((void **) &part->collapse_ebounds);
	    part->collapse_ebounds =
		(double *) ndi_calloc(new_num_grps + 1, sizeof(double));
	    curr_p = part->collapse_ebounds;
	    part->grp_num_new = new_num_grps;
	    part->collapse_flag = -1;

	    for (i = 0; i < part->grp_num_new + 1; i++) {
		if ((i == 0) ||
		    ((i > 0) && (ebound_array[i] < last_bound))) {
		    *curr_p = (double) ebound_array[i];
		    last_bound = ebound_array[i];
		    curr_p++;
		} else {
		    if (i > 0) {
			part->collapse_flag = 0;
			strcpy(part->n_group, "default");
			if (part->collapse_ebounds) {
			    ndi_free((void **) &part->collapse_ebounds);
			}
			return (-23);
		    }
		}
	    }

	    ierr =
		ndi_set_e_grps(part->num_grps, part->e_bounds,
			       &part->grp_num_new, &part->collapse_ebounds,
			       "user", state->library_type,
			       &part->new_ebounds, &part->collapse_flag);

	}
    } else {
	/* Bad input for current library type */
	ierr = -4;
    }

    if (ierr >= 0) {
	strcpy(part->n_group, "user");
    } else {
	strcpy(part->n_group, "default");
    }
    return (ierr);
}

/**********************************************************************/
/**********************************************************************/
