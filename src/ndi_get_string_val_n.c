
/*
 * File:	ndi_get_string_val_n.c
 * Description:	User function--return a string element from a list.
 * Time-stamp:	<2003-03-24 18:41:12 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

int ndi_c_get_string_val_n_h(state_struct *state, int in_key, int vec_num,
			     char *ret_val, int str_size);
static int get_string_n_mg(state_struct *state, int in_key, char *ret_val,
			   int vec_num, int str_size);
static int get_string_n_tn(state_struct *state, int in_key, char *ret_val,
			   int vec_num, int str_size);
static int get_string_n_tnreac(state_struct *state, int in_key,
			       char *ret_val, int vec_num, int str_size);

/**********************************************************************/

int ndi_c_get_string_val_n_h(state_struct *state, int in_key, int vec_num,
			     char *ret_val, int str_size)

/*  Purpose: Return a single string value.
    
    Parameters: 
    in_key -- in: key to specify type of data required
    ret_val -- out: string value returned
    str_size -- in:number of characters in string

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/
{
    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */

    if ((ret_val) && (str_size > 0)) {
	*ret_val = '\0';
    } else {
	return (7);
    }

    /* Check that ndi_init has been called. */
    if ((!state) || (!state->ndi_initialized)) {
	return (-2);
    }

    if (in_key == NDI_ZAID) {
	ierr = ndi_translate_sza_to_symbol(vec_num, ret_val, str_size);
    } else {
	if (is_gendir(state)) {
	    ierr = -2;		/* case not handled currently */
	} else if ((state->library_type == 'm') || 
		   (state->library_type == 'd')) {
	    ierr = get_string_n_mg(state, in_key, ret_val, vec_num, str_size);
	} else if (state->library_type == 's') {
	    ierr = get_string_n_tn(state, in_key, ret_val, vec_num, str_size);
	} else if (state->library_type == 'r') {
	    ierr =get_string_n_tnreac(state, in_key, ret_val, vec_num,
				    str_size);
	} else if (state->library_type == 'p') {
	    ierr = -4;
	} else if (state->library_type == ' ') {
	    /* Library type was not set */
	    ierr = -16;
	} else {
	    /* Incompatible library type -- bad input key */
	    ierr = -4;
	}
    }

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/
static int get_string_n_mg(state_struct *state, int in_key, char *ret_val,
			   int vec_num, int str_size)
{

    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */

    if (!state) {
	return (-2);
    }

    /* Default return value is empty string */
    *ret_val = '\0';

    /* Check that isotope is set */
    if ((in_key != NDI_TARGET) && (in_key != NDI_ZAID)) {
	/* Special case for targets -- isotope need not be set */
	if (!state->iso_stat.set_flag) {
	    return (-7);
	}
    }


   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {

    case NDI_ZAID:		/* Translate input SZA to string */
	ierr = ndi_translate_sza_to_symbol(vec_num, ret_val, str_size);
	break;

    case NDI_SEC_PART_TYPES:	/* Secondary particle types  */
	if ((state->iso_stat.num_sec_parts > vec_num)
	    && (state->iso_stat.sec_part)
	    && (state->iso_stat.sec_part[vec_num].part_type >= 0)) {
	    if (sprintf
		(ret_val, "%d",
		 state->iso_stat.sec_part[vec_num].part_type) > str_size);
	} else {
	    if (state->iso_stat.num_sec_parts > vec_num) {
		ierr = ndi_check_array_err(state->iso_stat.num_sec_parts);
	    } else {
		ierr = -9;
	    }
	}
	break;

    case NDI_SEC_PART_ZAIDS:	/* Secondary particle ZAIDs  */
	if ((state->iso_stat.num_sec_parts > vec_num)
	    && (state->iso_stat.sec_part)
	    && (strcmp(state->iso_stat.sec_part[vec_num].zaid, "") != 0)) {
	    strncpy(ret_val, state->iso_stat.sec_part[vec_num].zaid,
		    str_size);
	} else {
	    if (state->iso_stat.num_sec_parts > vec_num) {
		ierr = ndi_check_array_err(state->iso_stat.num_sec_parts);
	    } else {
		ierr = 9;
	    }
	}
	break;

    case NDI_TARGET:		/* Zaid of target specified by vec_num */

	/* check vec_num is less than num of targets */
	if (!state->ndi_depl_set) {
	    ierr = ndi_set_default_depl(state);
	}
	if (state->num_target <= 0) {
	    return (-7);
	}
	if (state->num_target > vec_num) {
	    if (sprintf(ret_val, "%d", state->depl_target[vec_num].izaid) >
		str_size) {
		/* Error */
		ierr = -5;
	    }
	} else {
	    ierr = ndi_check_array_err(state->chain_stat.num_target);
	}
	break;

    case NDI_RPRODS_ID:
	/* Return the id of the reaction product whose index is vec_num 
	   (for the target given by the global variable 'zaid' ) */

	if (!state->ndi_depl_set) {
	    ierr = ndi_set_default_depl(state);
	}
	if (state->num_target <= 0) {
	    return (-7);
	}
	ierr = ndi_get_rprod_id_n(state, vec_num, ret_val, str_size);
	break;


    case NDI_INC_PART_TYPES:	/* Incident particle types */
	ierr =
	    ndi_get_nth_inc_part_type(state, vec_num, ret_val, str_size);
	break;

     /**************  UNKNOWN KEYWORD  *****************/

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}

/**********************************************************************/
/**********************************************************************/
static int get_string_n_tn(state_struct *state, int in_key, char *ret_val,
			   int vec_num, int str_size)
{

    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */
    int return_int;

    if (!state) {
	return (-2);
    }

    if (str_size > 0) {
	*ret_val = '\0';
    } else {
	return (7);
    }

    /* Check that isotope is set */
    if (!state->tn_stat.set_flag) {
	return (-7);
    }

   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {

    case NDI_SEC_PART_TYPES:	/* Secondary particle types  */
	/* Check vec_num is within bounds */
	if ((state->tn_stat.num_sec_parts > vec_num) &&
	    (state->sec_part_filled > vec_num) &&
	    (state->tn_stat.sec_part)) {
	    if (state->tn_stat.sec_part[vec_num].part_type >= 0) {
		sprintf(ret_val, "%-ld",
			state->tn_stat.sec_part[vec_num].part_type);
	    } else {
		ierr = 5;
	    }
	} else {
	    if (state->tn_stat.sec_part) {
		ierr = ndi_check_array_err(state->sec_part_filled);
	    } else {
		ierr = -9;
	    }
	}
	break;

    case NDI_RPRODS_ID:
	/* The reaction product ID specified by the index 'vec_num' */
	ierr =
	    ndi_c_get_int_val_n_h(state, NDI_RPRODS_ID, vec_num,
				  &return_int);
	sprintf(ret_val, "%-d", return_int);
	break;

    case NDI_REACTANTS_ID:
	/* The reactant ID specified by the index 'vec_num' */
	ierr =
	    ndi_c_get_int_val_n_h(state, NDI_REACTANTS_ID, vec_num,
				  &return_int);
	sprintf(ret_val, "%-d", return_int);
	break;

     /**************  UNKNOWN KEYWORD  *****************/

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}

/**********************************************************************/
static int get_string_n_tnreac(state_struct *state, int in_key,
			       char *ret_val, int vec_num, int str_size)
{

    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */

    if (!state) {
	return (-2);
    }

    /* Default return value is empty string */
    *ret_val = '\0';

    /* Check that isotope is set */
    if (state->tnreac_stat.reac_num <= 0) {
	return (-7);
    }

   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {

    case NDI_LIST_REAC:	/* List of reactions */

	/* check vec_num is less than num of reac */
	if (state->tnreac_stat.reac_num > vec_num) {
	    strncpy(ret_val, state->tnreac_stat.list_reac[vec_num],
		    str_size);
	} else {
	    ierr = ndi_check_array_err(state->tnreac_stat.reac_num);
	}
	break;


     /**************  UNKNOWN KEYWORD  *****************/

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}

/**********************************************************************/
/**********************************************************************/
