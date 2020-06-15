
/*
 * File:	ndi_get_float_val.c
 * Description:	User function--returns a real value.
 * Time-stamp:	<99/10/20 10:57:37 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"
#include "ndi_sf.h"

static int GET_FLOAT_VAL_MG(state_struct *state, int in_key,
			    FLOAT_TYPE * ret_val);
static int GET_FLOAT_VAL_TN(state_struct *state, int in_key,
			    FLOAT_TYPE * ret_val);
static int GET_FLOAT_VAL_DOS(state_struct *state, int in_key,
			     FLOAT_TYPE * ret_val);
static int CALC_EAVG_OUT(state_struct *state, FILE *lib_stream,
			 FLOAT_TYPE * ret_val);

/**********************************************************************/

int NDI_GET_FLOAT_VAL_H(state_struct *state, int in_key,
			FLOAT_TYPE * ret_val)


/*********************************************************************
  
    Purpose: Return a single integer value which is an element of
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

    *ret_val = 0.0;		/* Default return value is zero */

    /* Check that ndi_init has been called. */
    if (!state) {
	return -2;
    }

    if (!state->ndi_initialized) {
	return -2;
    }
#ifdef FLOAT32
    if (sizeof(float) == 8) {
	/*  32-bit operation not implemented on 64-bit platform */
	return -10;
    }
#endif

    if (is_gendir(state)) {
	ierr = -2;		/* case not handled currently */
    } else if (state->library_type == 'm') {
	ierr = GET_FLOAT_VAL_MG(state, in_key, ret_val);
    } else if (state->library_type == 's') {
	ierr = GET_FLOAT_VAL_TN(state, in_key, ret_val);
    } else if (state->library_type == 'd') {
	ierr = GET_FLOAT_VAL_DOS(state, in_key, ret_val);
    } else if (state->library_type == 'p') {
	*ret_val = 0.0;
	ierr = -4;
    } else if (state->library_type == ' ') {
	/* Library type was not set */
	ierr = -16;
    } else {
	/* Incompatible library type -- bad input key */
	ierr = -4;
    }

    return ierr;

}

/**********************************************************************/
/**********************************************************************/

static int GET_FLOAT_VAL_MG(state_struct *state, int in_key,
			    FLOAT_TYPE * ret_val)
{

    int ierr = 0;

    if (!state) {
	return -2;
    }

    /* Check that isotope is set */
    if (!state->iso_stat.set_flag) {
	return -7;
    }

   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {

    case NDI_TEMP:		/*  Temperature  */
	if (state->iso_stat.temp >= 0) {
	    *ret_val = (FLOAT_TYPE) state->iso_stat.temp;
	} else {
	    ierr = -9;
	    *ret_val = 0.0;
	}
	break;

    case NDI_SIG_0:		/* Sigma zero  */
	if (state->iso_stat.sig_0 >= 0) {
	    *ret_val = (FLOAT_TYPE) state->iso_stat.sig_0;
	} else {
	    ierr = -9;
	    *ret_val = 0.0;
	}
	break;

    case NDI_AT_WGT:		/* Atomic weight  */
	if (state->iso_stat.at_wgt > 0.0) {
	    *ret_val = (FLOAT_TYPE) state->iso_stat.at_wgt;
        } else if (state->iso_stat.awr > 0.0) {
            *ret_val = (FLOAT_TYPE) (state->iso_stat.awr * M_NEUT_AMU);
	} else {
	    ierr = -9;
	    *ret_val = 0.0;
	}
	break;

    case NDI_AWR:		/* Atomic weight ratio  */
	if (state->iso_stat.awr > 0.0) {
	    *ret_val = (FLOAT_TYPE) state->iso_stat.awr;
        } else if (state->iso_stat.at_wgt > 0.0) {
            *ret_val = (FLOAT_TYPE) (state->iso_stat.at_wgt / M_NEUT_AMU);
	} else {
	    ierr = -9;
	    *ret_val = 0.0;
	}
	break;

    case NDI_SF_RATE:           /* Spontaneous Fission Rate (Reqt 46) */
        if (state-> iso_stat.fissionable == 0) { /* Not fissionable */
            *ret_val = 0.0;
            ierr = 0;
        } else {
            spont_fiss *sf;     /* Pointer to static data */

            sf = ndi_get_sf_data(state->iso_stat.zaid); /* Lookup data */
            if (!sf) {
                *ret_val = 0.0;
                ierr = 9;       /* Data not available */
            } else {            /* Data found */
                /* [decay/(atom*sh)][fissions/decay] */
                *ret_val = sf->lambda * sf->ratio;
                ierr = 0;
            }
        }
        break;

      /*******************  UNKNOWN KEYWORD  ********************/

    default:
	ierr = -4;
	break;
    }

    return ierr;
}

/**********************************************************************/
/**********************************************************************/
static int GET_FLOAT_VAL_TN(state_struct *state, int in_key,
			    FLOAT_TYPE * ret_val)
{

    int ierr = 0;


    if (!state) {
	return -2;
    }

    /* Check that isotope is set */
    if (!state->tn_stat.set_flag) {
	return -7;
    }

   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {

    case NDI_REAC_Q:		/* Reaction Q-value */
	*ret_val = (FLOAT_TYPE) state->tn_stat.q_value;
	if (state->tn_stat.q_set == 0) {
	    ierr = -9;
	}
	break;

    case NDI_EAVG_OUT:		/* Average outgoing energy */
    case NDI_EOUTBAR:		/* Average outgoing energy */
	ierr = CALC_EAVG_OUT(state, state->lib_stream, ret_val);
	break;


      /*******************  UNKNOWN KEYWORD  ********************/

    default:
	ierr = -4;
	break;
    }

    return ierr;
}

/**********************************************************************/
/**********************************************************************/

static int GET_FLOAT_VAL_DOS(state_struct *state, int in_key,
			     FLOAT_TYPE * ret_val)
{

    int ierr = 0;

    if (!state) {
	return -2;
    }

    /* Check that isotope is set */
    if (!state->iso_stat.set_flag) {
	return -7;
    }

   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {

/*    case NDI_ABUNDANCE:                            */
	/*  Abundance  */
	/*
	   if ( state->iso_stat.abundance >= 0 ) {
	   *ret_val = (FLOAT_TYPE) state->iso_stat.abundance;
	   } else {
	   ierr = -9;
	   *ret_val = 0.0;
	   }
	   break;
	 */

    case NDI_TEMP:		/*  Temperature  */
	if (state->iso_stat.temp >= 0) {
	    *ret_val = (FLOAT_TYPE) state->iso_stat.temp;
	} else {
	    ierr = -9;
	    *ret_val = 0.0;
	}
	break;

    case NDI_SIG_0:		/* Sigma zero  */
	if (state->iso_stat.sig_0 >= 0) {
	    *ret_val = (FLOAT_TYPE) state->iso_stat.sig_0;
	} else {
	    ierr = -9;
	    *ret_val = 0.0;
	}
	break;

    case NDI_AT_WGT:		/* Atomic weight  */
	if (state->iso_stat.at_wgt >= 0) {
	    *ret_val = (FLOAT_TYPE) state->iso_stat.at_wgt;
	} else {
	    ierr = -9;
	    *ret_val = 0.0;
	}
	break;

    case NDI_AWR:
	/* Atomic weight ratio  */
	if (state->iso_stat.awr >= 0) {
	    *ret_val = (FLOAT_TYPE) state->iso_stat.awr;
	} else {
	    ierr = -9;
	    *ret_val = 0.0;
	}
	break;

      /*******************  UNKNOWN KEYWORD  ********************/

    default:
	ierr = -4;
	break;
    }

    return ierr;
}

/**********************************************************************/
/**********************************************************************/

static int CALC_EAVG_OUT(state_struct *state, FILE *lib_stream,
			 FLOAT_TYPE * ret_val)
     /* Calculates the average outgoing energy of the "current" secondary 
        particle (given by global var. curr_part_index) by integrating
        over that particles energy distribution
      */
{
    int ierr = 0;
    int num_temps;
    int hi, lo;
    int curr_temp_index = -1;
    long int curr_file_p;
    double *distr_E = NULL;
    double *distr_prob = NULL;
    double Eavg;
    sec_part_distr *part;
    interp_params curr_interp;

    if (!state) {
	return -2;
    }

    /* INITIALIZE */
    if (!state) {
	return -2;
    }

    /* Set current particle */
    if ((state->curr_part_index < 0) ||
	(state->curr_part_index >= state->sec_part_filled))
	return -30;

    part = &(state->tn_stat.sec_part[state->curr_part_index]);
    if (!part)
	return -30;

    /* Find closest temperature */
    num_temps = part->temp_interp.npts;
    ierr =
	ndi_find_temp_bound_index(state->default_temp, part->edist,
				  num_temps, &hi, &lo, &curr_temp_index);
    if (ierr < 0)
	return -30;


    /* Initialize distribution variables */
    curr_interp = part->edist[curr_temp_index].interp;
    curr_file_p = part->edist[curr_temp_index].file_ptr;
    distr_E = (double *) ndi_malloc(curr_interp.npts * sizeof(double));
    distr_prob = (double *) ndi_malloc(curr_interp.npts * sizeof(double));
    ierr = ndi_read_float_pair(lib_stream, curr_file_p,
			       curr_interp.npts, distr_E, distr_prob,
			       curr_interp.npts);

    /* FIND AVERAGE ENERGY */
    ierr = find_average_E(distr_E, distr_prob, &curr_interp, &Eavg);

    /* Check error */
    if (ierr < 0) {
	if (distr_E)
	    ndi_free((void **) &distr_E);
	if (distr_prob)
	    ndi_free((void **) &distr_prob);
	return -30;
    }


    if (distr_E)
	ndi_free((void **) &distr_E);
    if (distr_prob)
	ndi_free((void **) &distr_prob);

    /* Check error */

    /* Change from double to correct float type */
    *ret_val = (FLOAT_TYPE) Eavg;

    return ierr;
}

/**********************************************************************/
/**********************************************************************/
