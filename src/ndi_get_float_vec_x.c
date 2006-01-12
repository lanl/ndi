
/*
 * File:	ndi_get_float_vec_x.c
 * Description:	Source for libndi.a
 * Time-stamp:	<2000-02-03 15:26:44 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"


/**********************************************************************/

int
NDI_GET_FLOAT_VEC_X_H(state_struct *state, int in_key,
		      const char *in_str, FLOAT_TYPE * return_vec,
		      int vec_size)
{

    int ierr = 0;
    int tmp_err = 0;

    /* Initialize return_vec with zeroes */
    ierr = NDI_INIT_FLOAT_VEC(return_vec, vec_size);

    if (!state) {
	return (-2);
    }

    /* Check that ndi_init has been called. */
    if (!state->ndi_initialized) {
	return (-2);
    }
#ifdef FLOAT32
    if (sizeof(float) == 8) {
	ierr = -2;
	return (ierr);
    }
#endif

    if (state->library_type == ' ') {
	/* Library type was not set */
	return (-16);
    } else if ((state->library_type != 'm') &&
	       (state->library_type != 'd')) {
	/* Incompatible library -- bad input key */
	return (-4);
    }

    /* Check that isotope is set */
    if (!state->iso_stat.set_flag) {
	return (-7);
    }

   /*************************/
    /*   Process input key   */
   /*************************/

    switch (in_key) {

    case NDI_TOT_PROD_XS:	/* Total production x.s. for target given by 
				   global var. 'zaid' and product given by in_str */
	if (strcmp(in_str, state->def_fiss_prod_id) == 0) {
	    ierr = GET_PROD_REAC(state, "99999", return_vec, vec_size);
	} else {
	    ierr = GET_PROD_REAC(state, in_str, return_vec, vec_size);
	}
	break;

    case NDI_RPROD_MLT:		/* Reaction multiplicity */
	if (strcmp(in_str, state->def_fiss_prod_id) == 0) {
	    ierr = GET_RPROD_MLT(state, "99999", return_vec, vec_size);
	} else {
	    ierr = GET_RPROD_MLT(state, in_str, return_vec, vec_size);
	}
	break;


      /**********  UNKNOWN KEYWORD  ***********/

    default:
	ierr = -4;
	break;
    }


    if ((tmp_err != 0) && (ierr >= 0)) {
	ierr = tmp_err;
    }
    return (ierr);
}

/*********************************************************************/
/*********************************************************************/

int GET_PROD_REAC(state_struct *state, const char *rprod_in,
	      FLOAT_TYPE * return_vec, int vec_size)
{
    int i, j;
    int ierr = 0;
    int curr_index = -1;
    int num_grps = 0;
    int num_elements;
    int curr_mt;
    int sig_reac_err = 0;
    int old_reac_size = 3;
    char old_reac_id[3];
    FLOAT_TYPE *curr_xs = NULL;
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


    /* Determine no. of groups and create temporary x.s. array */
    if (state->collapse_flag) {
	num_grps = state->grp_num_new;
    } else {
	num_grps = state->iso_stat.num_grps;
    }
    if (num_grps <= 0) {
	/* Bad energy group structure */
	ierr = 5;
	return (ierr);
    }
    curr_xs = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
    if (!curr_xs) {
	/* Out of memory */
	return (-14);
    }


    /* For each reaction, read MT and mult */
    /*     find sig_reac for MT, multiply by mult. and  and to total */

    ierr = ndi_c_check_option_h(state, NDI_REAC_ID, old_reac_id, old_reac_size);
    ierr = ndi_c_set_option_h(state, NDI_REAC_ID, "mt");
    for (i = 0; i < curr_prod->num_reac; i++) {
	curr_mt = curr_prod->mt[i];
	/* Must change get_sig_reac to not convert to c values */
	num_elements = num_grps;
	sig_reac_err = GET_SIG_REAC(state, &state->iso_stat, curr_mt, curr_xs,
                                    &num_elements);
	if (sig_reac_err == 9) {
	    ierr = 9;
	} else {
	    for (j = 0; j < num_grps; j++) {
		if (j < vec_size) {
		    return_vec[j] =
			return_vec[j] + curr_xs[j] * curr_prod->mult[i];
		} else {
		    /* Warning--array size not large enough */
		    if (ierr != 9) {	/* No data warning takes precedence 
					   over too small array warning */
			ierr = 7;
		    }
		}
	    }
	}
    }

    ndi_c_set_option_h(state, NDI_REAC_ID, old_reac_id);
    if (curr_xs)
	ndi_free((void **) &curr_xs);
    return (ierr);

}

/*********************************************************************/
/*********************************************************************/

int GET_RPROD_MLT(state_struct *state, const char *rprod_in,
		 FLOAT_TYPE *return_vec, int vec_size)
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
	return_vec[i] = curr_prod->mult[i];
    }

    return (ierr);

}

/*********************************************************************/
/*********************************************************************/
