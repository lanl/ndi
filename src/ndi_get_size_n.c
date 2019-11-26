
/*
 * File:	ndi_get_size_n.c
 * Description:	User function--returns size of an element of a multi-dim array.
 * Time-stamp:	<1999-11-18 13:09:22 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

static int get_mg_size_n(state_struct *state, int in_key, int vec_num,
			 int *ierr);
static int get_tn_size(state_struct *state, int in_key, int vec_num,
		       int *ierr);
static int get_tnreac_size(state_struct *state, int in_key, int vec_num,
			   int *ierr);
static int get_dedx_size_n(state_struct *state, int in_key, int vec_num,
                           int *ierr);

/**********************************************************************/


/* Purpose: Return the size of a "vector" in a multi-dimensional array.
   The vector is identified by vec_num (a zero-based index).

   Parameters:
   in_key -- in: key to specify type of data required
   vec_num -- in: indicates the index of the element (**zero-based**)
   ierr -- out: error flag, where 0=no error,pos=warning, neg=error

   Returns: Returns the integer indicating the number of elements
   in the array.  For single-valued data (temperature, sigma0, etc.)
   "1" is returned.  For string data the size of the string (number of
   characters plus the end of string character) is returned.  Returns 
   zero or negative as error.

**********************************************************************/
int
ndi_c_get_size_n_h(state_struct *state, int in_key, int vec_num, int *ierr)
{
    int size = 0;		/* Default return value is '0' */

    *ierr = 0;			/* Default error value */
    /* Check that ndi_init has been called. */
    if ((!state) || (!state->ndi_initialized)) {
	*ierr = -2;
	return (size);
    }

    if (in_key == NDI_ZAID) {
	char tmp_str[MAX_IN_STR_LEN];
	*ierr =
	    ndi_translate_sza_to_symbol(vec_num, tmp_str, MAX_IN_STR_LEN);
	size = (int) strlen(tmp_str) + 1;
    } else if (in_key == NDI_ERROR) {
	size = strlen(ndi_error_string(vec_num)) + 1;
    } else {
	if (is_gendir(state)) {
	    *ierr = -2;		/* case not handled currently */
	} else if ((state->library_type == 'm') ||
		   (state->library_type == 'd')) {
	    size = get_mg_size_n(state, in_key, vec_num, ierr);
	} else if (state->library_type == 's') {
	    size = get_tn_size(state, in_key, vec_num, ierr);
	} else if (state->library_type == 'r') {
	    size = get_tnreac_size(state, in_key, vec_num, ierr);
	} else if (state->library_type == 'p') {
	    size = ndimp_get_size_n(state, in_key, vec_num, ierr);
        } else if (state->library_type == 'x') {
            size = get_dedx_size_n(state, in_key, vec_num, ierr);
	} else if (state->library_type == ' ') {
	    *ierr = -16;
	    size = 0;
	} else {
	    *ierr = -4;
	    size = 0;
	}
    }

    if (size < 0) {
	size = 0;
	if (ierr == 0) {
	    *ierr = 9;
	}
    }
    return (size);
}

/**********************************************************************/
/**********************************************************************/

static int get_mg_size_n(state_struct *state, int in_key, int vec_num,
			 int *ierr)
{

    int return_value = 0;	/* Default return value is '0' */
    int rprod_index;
    int grps_inc_part;
    int grps_curr_part;
    char tmp_str[MAX_IN_STR_LEN];

    if (!state) {
	*ierr = -2;
	return (0);
    }

    *ierr = 0;			/* Default error value */

    /* Check that isotope is set */
    if (in_key != NDI_TARGET) {
	/* Special case for targets -- isotope need not be set */
	if (!state->iso_stat.set_flag) {
	    *ierr = -7;
	    return (0);
	}
    }

   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {

     /****    Single-valued data -- return "1" ****/

    case NDI_RPROD_ALL:	/* sigma edit reaction products -- all */
    case NDI_RPROD_RMO:	/* sigma edit reaction products -- rmo */
    case NDI_RPROD_FEW:	/* sigma edit reaction products -- few */
    case NDI_REAC_MT:		/* Reaction mt for reaction indexed by vec_num */
    case NDI_REAC_Q:		/* Reaction Q value for reaction indexed by vec_num */
    case NDI_ABUNDANCE:	/* Abundance of input sza */
    case NDI_TOT_PROD_Q:	/* Total production Q for input product */
	return_value = 1;
	break;


     /****    String values ****/
    case NDI_SEC_PART_TYPES:	/* Secondary particle types  */
	if (state->iso_stat.sec_part) {
            if (0 <= vec_num && 
                vec_num < state->iso_stat.num_sec_parts) {
                sprintf(tmp_str, "%d",
                        state->iso_stat.sec_part[vec_num].part_type);
                return_value = (int) strlen(tmp_str) + 1;
            } else {
		/* Invalid vec_num */
		*ierr = -5;
            }
        } else {
            /* No secondary particle data for this particle */
            *ierr = 9;
        }
	break;

    case NDI_SEC_PART_ZAIDS:	/* Secondary particle ZAIDs  */
	if (state->iso_stat.sec_part) {
            if (0 <= vec_num &&
                vec_num < state->iso_stat.num_sec_parts) {
                return_value =
                    (int) strlen(state->iso_stat.sec_part[vec_num].zaid) + 1;
            } else {
		/* Invalid vec_num */
		*ierr = -5;
            }
        } else {
		/* No secondary particle data for this particle */
		*ierr = 9;
	}
	break;

    case NDI_TARGET:
	if (!state->ndi_depl_set) {
	    *ierr = ndi_set_default_depl(state);
	}

	if ((state->depl_target) && (state->num_target > vec_num)) {
	    char temp_id[MAX_IN_STR_LEN];
	    if (state->depl_target[vec_num].izaid != 99999) {
		sprintf(temp_id, "%d", state->depl_target[vec_num].izaid);
	    } else {
		/* Translate fission product identifier 99999 to default f.p. ZAID */
		strcpy(temp_id, state->def_fiss_prod_id);
	    }

	    return_value = (int) strlen(temp_id) + 1;
	} else {
	    if ((state->num_target <= 0) || !(state->depl_target)) {
		*ierr = 9;
	    } else {
		*ierr = -5;
	    }
	}
	break;

    case NDI_RPRODS_ID:
	if (!state->ndi_depl_set) {
	    *ierr = ndi_set_default_depl(state);
	}
	{
	    char temp_id[MAX_IN_STR_LEN];
	    *ierr =
		ndi_get_rprod_id_n(state, vec_num, temp_id,
				   MAX_IN_STR_LEN);
	    return_value = (int) strlen(temp_id) + 1;
	}
	break;

    case NDI_INC_PART_TYPES:	/* nth incident particle type */
	*ierr =
	    ndi_get_nth_inc_part_type(state, vec_num, tmp_str,
				      MAX_IN_STR_LEN);
	return_value = (int) strlen(tmp_str) + 1;
	break;

    case NDI_REAC_LABEL:
	*ierr = ndi_c_get_string_val_n_h(state, NDI_REAC_LABEL, vec_num,
					 tmp_str, MAX_IN_STR_LEN);
	return_value = (int) strlen(tmp_str) + 1;
	break;

     /**** Simple array values ****/

	/* Size of the vector is the number of groups */
    case NDI_TOT_PROD_XS:	/* Total production cross section */
    case NDI_SIG_REAC:		/* reaction cross section */
    case NDI_CORR_DIAG:	/* Diagonal transport correction  */
    case NDI_CORR_BHS:		/* Bell-Hansen-Sandmeier transport correction */
    case NDI_PNU_PR:		/* fission neutron mult. probabilities */
    case NDI_SIG_PNU_PR:	/* fission neutron mult. cross sections */
    case NDI_TD_HALF_MOMENT:
	if (state->collapse_flag != 0) {
	    return_value = state->grp_num_new;
	} else {
	    return_value = state->iso_stat.num_grps;
	}
	break;

	/* Others */
    case NDI_RPROD_ID_ALL:	/* reaction product ids -- all */
    case NDI_RPROD_MLT_ALL:	/* reaction product multiplicities--all */
	*ierr =
	    ndi_find_rprod_index(state->lib_stream,
				 &state->iso_stat.rprod_all, &rprod_index,
				 vec_num);
	if (*ierr >= 0) {
	    return_value =
		state->iso_stat.rprod_all.stat[rprod_index].length;
	}
	break;

    case NDI_RPROD_ID_RMO:	/* reaction product ids -- rmo */
    case NDI_RPROD_MLT_RMO:	/* reaction product multiplicities--rmo */
	*ierr =
	    ndi_find_rprod_index(state->lib_stream,
				 &state->iso_stat.rprod_rmo, &rprod_index,
				 vec_num);
	if (*ierr >= 0) {
	    return_value =
		state->iso_stat.rprod_rmo.stat[rprod_index].length;
	}
	break;


    case NDI_RPROD_ID_FEW:	/* reaction product ids -- few */
    case NDI_RPROD_MLT_FEW:	/* reaction product ids -- few */
	*ierr =
	    ndi_find_rprod_index(state->lib_stream,
				 &state->iso_stat.rprod_few, &rprod_index,
				 vec_num);
	if (*ierr >= 0) {
	    return_value =
		state->iso_stat.rprod_few.stat[rprod_index].length;
	}
	break;



     /**** Multi-dimensional arrays ****/

    case NDI_PN_DOWN_FULL:
	/* pn scattering -- full lower diagonal */
      /***Assumption for pn_down_full: down_grps == number of groups - 1 ***/
	if (state->collapse_flag != 0) {
	    return_value =
		(state->grp_num_new * (state->grp_num_new + 1)) / 2;
	} else {
	    return_value = ((state->iso_stat.num_grps) *
			    (state->iso_stat.num_grps + 1)) / 2;
	}
	break;

    case NDI_PN_FULL:		/* pn scattering -- full matrix */
    case NDI_TRANSFER_MAT_TOT:	/* Transfer matrix -- total */
    case NDI_TRANSFER_MAT_PR:	/* Transfer matrix -- prompt */
	if (state->collapse_flag != 0) {
	    return_value = state->grp_num_new * state->grp_num_new;
	} else {
	    return_value =
		state->iso_stat.num_grps * state->iso_stat.num_grps;
	}
	break;

    case NDI_PN_PROD_FULL_CURR_PART:
	/* pn scattering photon -- full */
	/* Assumption: photon groups are not collapsed */
      /*** Are photon production matrices collapsed????? ****/

	if ((state->iso_stat.sec_part) &&
	    (state->curr_part_index < state->iso_stat.num_sec_parts) &&
	    (state->curr_part_index >= 0)) {
	    /* Current particle has secondary particle info */

	    if (state->collapse_flag != 0) {
		grps_inc_part = state->grp_num_new;
	    } else {
		grps_inc_part = state->iso_stat.num_grps;
	    }
	    if (state->iso_stat.sec_part[state->curr_part_index].
		collapse_flag != 0) {
		grps_curr_part =
		    state->iso_stat.sec_part[state->curr_part_index].
		    grp_num_new;
	    } else {
		grps_curr_part =
		    state->iso_stat.sec_part[state->curr_part_index].
		    num_grps;
	    }
	    return_value = grps_inc_part * grps_curr_part;
	} else {
	    if (state->iso_stat.num_sec_parts >= vec_num) {
		/* Invalid vec_num */
		*ierr = -5;
	    } else {
		/* No secondary particle data for this particle */
		*ierr = 9;
	    }
	}
	break;


      /***********  UNKNOWN KEYWORD  **************/

    default:
	*ierr = -4;
	break;
    }

    return (return_value);
}

/**********************************************************************/
/**********************************************************************/


static int get_tn_size(state_struct *state, int in_key, int vec_num,
		       int *ierr)
{



    int return_value = 0;	/* Default return value is '0' */
    char tmp_str[MAX_IN_STR_LEN];

    *ierr = 0;			/* Default error value */

    if (!state) {
	*ierr = -2;
	return (0);
    }
    /* Check that isotope is set */
    if (!state->tn_stat.set_flag) {
	*ierr = -7;
	return (return_value);
    }

    switch (in_key) {

     /****  Single-valued data -- return "1" ****/
    case NDI_RPRODS_MLT:
    case NDI_REACTANTS_MLT:
	return_value = 1;
	break;

     /**** String values ****/
    case NDI_RPRODS_ID:
	*ierr =
	    ndi_c_get_string_val_n_h(state, NDI_RPRODS_ID, vec_num,
				     tmp_str, MAX_IN_STR_LEN);
	return_value = (int) strlen(tmp_str) + 1;
	break;
    case NDI_REACTANTS_ID:
	*ierr =
	    ndi_c_get_string_val_n_h(state, NDI_REACTANTS_ID, vec_num,
				     tmp_str, MAX_IN_STR_LEN);
	return_value = (int) strlen(tmp_str) + 1;
	break;
    case NDI_SEC_PART_TYPES:	/* Secondary particle types  */
	/* This gives the size of the string returned if the nth part type is asked for */
	if ((state->tn_stat.num_sec_parts > vec_num) &&
	    (state->sec_part_filled > vec_num) &&
	    (state->tn_stat.sec_part)) {
	    if (state->tn_stat.sec_part[vec_num].part_type >= 0) {
		char junk[MAX_IN_STR_LEN];
		return_value =
		    sprintf(junk, "%-ld",
			    state->tn_stat.sec_part[vec_num].part_type) +
		    1;
	    }
	} else {
	    if ((state->tn_stat.num_sec_parts <= 0)
		|| !(state->tn_stat.sec_part)) {
		*ierr = 9;
	    } else {
		*ierr = -5;
	    }
	}

	break;

      /***********  UNKNOWN KEYWORD  **************/

    default:
	*ierr = -4;
	break;
    }

    return (return_value);
}

/**********************************************************************/
/**********************************************************************/

static int get_tnreac_size(state_struct *state, int in_key, int vec_num,
			   int *ierr)
{


    int return_value = 0;	/* Default return value is '0' */

    *ierr = 0;			/* Default error value */

    if (!state) {
	*ierr = -2;
	return (0);
    }

    /* Check that isotope is set */
    if (state->tnreac_stat.reac_num < 0) {
	*ierr = -7;
	return (return_value);
    }

    switch (in_key) {

     /**** Single-valued data -- return "1" ****/
    case NDI_NUM_RPRODS:	/* Number of reaction products */
    case NDI_NUM_REACTANTS:	/* Number of reactants */
	return_value = 1;
	break;

     /****  Simple array values ****/

    case NDI_RPRODS_ID:
    case NDI_RPRODS_MLT:
	if ((state->tnreac_stat.reac_num > vec_num)
	    && (state->tnreac_stat.rprods)) {
	    return_value = state->tnreac_stat.rprods[vec_num].length;
	} else {
	    if ((state->tnreac_stat.reac_num <= 0)
		|| !(state->tnreac_stat.rprods)) {
		/* No data */
		*ierr = 9;
	    } else {
		/* Subscript out of bounds */
		*ierr = -5;
	    }
	}
	break;

    case NDI_REACTANTS_ID:
    case NDI_REACTANTS_MLT:
	if ((state->tnreac_stat.reactants)
	    && (state->tnreac_stat.reac_num > vec_num)) {
	    return_value = state->tnreac_stat.reactants[vec_num].length;
	} else {
	    if ((state->tnreac_stat.reac_num <= 0)
		|| !(state->tnreac_stat.reactants)) {
		/* No data */
		*ierr = 9;
	    } else {
		/* Subscript out of bounds */
		*ierr = -5;
	    }
	}
	break;

      /**** String values ****/
    case NDI_LIST_REAC:	/* List of reactions */
	if ((state->tnreac_stat.reac_num > vec_num)) {
	    return_value =
		(int) strlen(state->tnreac_stat.list_reac[vec_num]) + 1;
	} else {
	    if (state->tnreac_stat.reac_num <= 0) {
		/* No data */
		*ierr = 9;
	    } else {
		/* Subscript out of bounds */
		*ierr = -5;
	    }
	}

	break;

    /***********  UNKNOWN KEYWORD  **************/

    default:
	*ierr = -4;
	break;
    }

    return (return_value);
}

/**********************************************************************/
/**********************************************************************/

static int get_dedx_size_n(state_struct *state, int in_key, int vec_num,
                           int *ierr)
{

    int return_value = 0;       /* Default return value is '0' */

    if (!state) {
        *ierr = -2;
        return (0);
    }

    *ierr = 0;                  /* Default error value */

    /* Check that isotope is set */
    if (!state->dedx_stat.set_flag) {
        *ierr = -7;
        return (0);
    }

    /*************************/
    /*   Process input key   */
    /*************************/

    switch (in_key) {

    /****  Single-valued data -- return "1" ****/

    case NDI_TARGET_ZAID:        /* Total production Q for input product */
        return_value = 1;
        break;

    /****  Multi-dimensional array ****/

    case NDI_TARGET_DEDX:    /* dE/dx for target. Identical for all targets  */
        if (state->dedx_stat.target_dedx.num_objs > 0) {
            return_value = state->dedx_stat.num_grps*state->dedx_stat.num_densities
                           *state->dedx_stat.num_temps;
        } else {
            /* No target dedx data for this isotope */
            *ierr = 9;
        }
        break;

    /***********  UNKNOWN KEYWORD  **************/

    default:
        *ierr = -4;
        break;
    }

    return (return_value);
}

/**********************************************************************/
/**********************************************************************/
