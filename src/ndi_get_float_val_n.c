
/*
 * File:	ndi_get_float_val_n.c
 * Description:	User function--return an element of a vector (float).
 * Time-stamp:	<1999-04-07 15:05:24 jomc>
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

static int GET_PROD_Q(state_struct * state, const int rprod_in,
		      FLOAT_TYPE * return_val);

/**********************************************************************/

int
NDI_GET_FLOAT_VAL_N_H(state_struct * state, int in_key, int vec_num,
		      FLOAT_TYPE * ret_val)

/**********************************************************************/
/*  Purpose: Return a single real value which is an element of
    a vector.
    
    Parameters: 
    in_key -- in: key to specify type of data required
    ret_val -- out: integer value returned
    vec_num -- in: integer specifying which element of array to 
                    return (zero-based)

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/
{
    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */
    int reac_index;
    int mt = 0;
    int tmp_err = 0;

    *ret_val = 0.0;		/* Default return value is zero */

    if (!state) {
	return (-2);
    }

    /* Special case--library type need not be set if asking for version. */
    if (in_key == NDI_ABUNDANCE) {
	ierr = GET_ABUNDANCE(vec_num, ret_val);
	return (ierr);
    }
    /* Check that ndi_init has been called. */
    if (!state->ndi_initialized) {
	return (-2);
    }
#ifdef FLOAT32
    if (sizeof(float) == 8) {
	/*  32-bit operation not implemented on 64-bit platform */
	return (-10);
    }
#endif


    if (state->library_type == ' ') {
	/* Library type was not set */
	return (-16);
    } else if ((state->library_type != 'm') &&
	       (state->library_type != 'd')) {
	/* Incompatible library type -- bad input key */
	return (-4);
    }

    /* Check that isotope is set */
    if (!state->iso_stat.set_flag) {
	return (-7);
    }

    /*  Process input key  */
    switch (in_key) {

      /***********************************************************************/
	/*           This section looks at non-standard isotope data           */
      /***********************************************************************/

    case NDI_REAC_Q:
	/* Reaction Q for reaction indexed by vec_num */
	/* Change this test */
	if (state->reac_id == 'c') {
	    mt = ndi_c_c_to_mt(vec_num);
	    if (mt <= 0) {
		ierr = -6;
		return (-24);	/* Invalid MT or reaction identifier */
	    }
	} else {
	    mt = vec_num;
	}

	ierr = ndi_find_reac_index(state->iso_stat.reac_mt, &reac_index,
				   mt, state->iso_stat.num_reac);
	if ((ierr >= 0) && (state->iso_stat.reac_q)) {
	    if (reac_index < state->iso_stat.num_reac) {
		*ret_val = (FLOAT_TYPE) state->iso_stat.reac_q[reac_index];
	    } else {
		ierr = ndi_check_array_err(state->iso_stat.num_reac);
	    }
	} else {
	    /* Data not in library. */
	    *ret_val = 0.0;
	    ierr = 9;
	}
	break;

/*
   case NDI_ABUNDANCE:
     ierr = GET_ABUNDANCE( vec_num, ret_val );
     break;
*/

    case NDI_TOT_PROD_Q:
	if (!state->ndi_depl_set) {
	    ierr = ndi_set_default_depl(state);
	}

	/* Check if incoming rprod zaid is the default fission product */
	if (vec_num == atoi(state->def_fiss_prod_id)) {
	    ierr = GET_PROD_Q(state, 99999, ret_val);
	} else {
	    ierr = GET_PROD_Q(state, vec_num, ret_val);
	}

	break;


      /**********  UNKNOWN KEYWORD  ***********/

    default:
	*ret_val = 0.0;
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

static int
GET_PROD_Q(state_struct * state, const int rprod_in,
	   FLOAT_TYPE * return_val)
{
    int i;
    int ierr = 0;
    int curr_index = -1;
    int curr_mt;
    int q_err = 0;
    int reac_index;
    FLOAT_TYPE max_q = 0.0;
    depl_target_struct *curr_targ;
    depl_rprod_struct *curr_prod;
    mg_zaid rprod_str;

    if (state) {
	curr_targ = state->depl_target;
    } else {
	return (-2);
    }


    /* CHECK ERRORS */

    *return_val = 0.0;

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


    sprintf(rprod_str, "%d", rprod_in);
    ierr = ndi_set_curr_rprod(state, &curr_prod, rprod_str, &curr_index);
    if (curr_index == -1) {
	return (ierr);
    }


    /* For each reaction, read MT and mult */
    /*     find maximum Q value */

    for (i = 0; i < curr_prod->num_reac; i++) {
	curr_mt = curr_prod->mt[i];
	/* Must change get_sig_reac to not convert to c values */

	q_err = ndi_find_reac_index(state->iso_stat.reac_mt, &reac_index,
				    curr_mt, state->iso_stat.num_reac);
	if (q_err >= 0) {
	    if (reac_index < state->iso_stat.num_reac) {
		/*  What about negative Q values ? */
		if (fabs(max_q) < fabs(state->iso_stat.reac_q[reac_index])) {
		    max_q = state->iso_stat.reac_q[reac_index];
		}
	    } else {
		/* Error -- subscript out of range */
		ierr = -31;
	    }
	} else {
	    ierr = q_err;
	}
    }

    *return_val = max_q;
    if ((q_err < 0) && (ierr >= 0)) {
	ierr = q_err;
    } else if ((q_err > 0) && (ierr == 0)) {
	ierr = q_err;
    }

    if (ierr == -9) {
	ierr = 9;
    }

    return (ierr);
}

/*********************************************************************/
/*********************************************************************/
