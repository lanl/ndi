
/*
 * $Id$
 * Description:	User function--return a vector which is an element of a matrix.
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

static int get_int_vec_x_mg(state_struct *state, int in_key,
			    const char *in_str, int *return_vec,
			    int vec_size);

static int get_reac_mt(state_struct *state, const char *rprod_in,
		       int *return_vec, int vec_size);

/**********************************************************************/

int ndi_c_get_int_vec_x_h(state_struct *state, int in_key,
			  const char *in_str, int *return_vec,
			  int vec_size)

/*  Purpose: Return an integer array which is a named element
    
    Parameters: 
    in_key -- in: key to specify type of data required
    in_str -- in: string name
    ret_val -- out: integer value returned

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/
{
    int ierr = 0;

    ierr = ndi_init_int_vec(return_vec, vec_size);

    /* Check that ndi_init has been called. */
    if ((!state) || (!state->ndi_initialized)) {
	return (-2);
    }

    if (is_gendir(state)) {
	ierr = -2;		/* case not handled currently */
    } else if (state->library_type == 'm') {
	ierr =
	    get_int_vec_x_mg(state, in_key, in_str, return_vec, vec_size);
    } else if (state->library_type == 'r') {
	ierr = -2;
    } else if (state->library_type == 'p') {
	ierr = -4;
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
static int get_int_vec_x_mg(state_struct *state, int in_key,
			    const char *in_str, int *return_vec,
			    int vec_size)
{
    int ierr = 0;

    if (!state) {
	return (-2);
    }

    /*  Check if isotope is set  */
    if (!state->iso_stat.set_flag) {
	return (-7);
    }

   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {

    case NDI_REAC_MT:
	ierr = get_reac_mt(state, in_str, return_vec, vec_size);
	break;

      /***********  UNKNOWN KEYWORD  **************/

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}

/**********************************************************************/
/**********************************************************************/
static int get_reac_mt(state_struct *state, const char *rprod_in,
		       int *return_vec, int vec_size)
{
    int i;
    int ierr = 0;
    int curr_index = -1;
    depl_target_struct *curr_targ;
    depl_rprod_struct *curr_prod;


    if (state) {
	curr_targ = state->depl_target;
    } else {
	return (-2);
    }

    /* CHECK ERRORS */

    if (!state->ndi_depl_set) {
	ierr = ndi_set_default_depl(state);
    }
    if (state->num_target <= 0) {
	return (-7);
    }

    ierr = ndi_set_curr_target(state, &curr_targ, &curr_index);
    if (curr_index == -1) {
	return (ierr);
    }

    curr_prod = curr_targ->rprod;
    curr_index = curr_targ->num_rprods;
    ierr = ndi_set_curr_rprod(state, &curr_prod, rprod_in, &curr_index);
    if (curr_index == -1) {
	return (ierr);
    }

    if (vec_size < curr_prod->num_reac) {
	return (7);
    }

    for (i = 0; i < curr_prod->num_reac; i++) {
	return_vec[i] = curr_prod->mt[i];
    }

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/
