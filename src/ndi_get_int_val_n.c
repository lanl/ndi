

/*
 * File:	ndi_get_int_val_n.c
 * Description:	User function--return an element of a vector (integer).
 * Time-stamp:	<99/11/03 11:13:09 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

static int get_int_val_n_mg(state_struct *state, int in_key, int vec_num,
			    int *ret_val);
static int get_int_val_n_tn(state_struct *state, int in_key, int vec_num,
			    int *ret_val);
static int get_nth_int(state_struct *state, stat_flag sf, int in_key,
		       int *return_val, int vec_num);
static int get_int_val_n_tnreac(state_struct *state, int in_key,
				int vec_num, int *ret_val);

/**********************************************************************/

int ndi_c_get_int_val_n_h(state_struct *state, int in_key, int vec_num,
			  int *ret_val)

/*  Purpose: Return a single integer value which is an element of
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

    /* Default return value is 0 */
    *ret_val = 0;

    /* Check that ndi_init has been called. */
    if ((!state) || (!state->ndi_initialized)) {
	return (-2);
    }

    if (is_gendir(state)) {
	ierr = -2;		/* case not currently handled */
    } else if ((state->library_type == 'm') || 
	       (state->library_type == 'd')) {
	ierr = get_int_val_n_mg(state, in_key, vec_num, ret_val);
    } else if (state->library_type == 's') {
	ierr = get_int_val_n_tn(state, in_key, vec_num, ret_val);
    } else if (state->library_type == 'r') {
	ierr = get_int_val_n_tnreac(state, in_key, vec_num, ret_val);
    } else if (state->library_type == 'p') {
	ierr = ndimp_get_int_val_n(state, in_key, vec_num, ret_val);
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
static int get_int_val_n_mg(state_struct *state, int in_key, int vec_num,
			    int *ret_val)
{

    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */
    int rprod_index;
    int mt = 0;

    if (!state) {
	return (-2);
    }

    /*  Default (error) return value is 0  */
    *ret_val = 0;

    /* Check that isotope is set */
    if (in_key != NDI_TARGET) {
	/* Special case for targets -- isotope need not be set */
	if (!state->iso_stat.set_flag) {
	    return (-7);
	}
    }

   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {


   case NDI_RPROD_ALL:  
      /* No. of products for reaction  MT 'vec_num' -- all */
      if ( state->reac_id == 'c' ) {
	 mt = ndi_c_c_to_mt( vec_num );
	 if ( mt <= 0 ) {
	    ierr = -24;
	    return(ierr);  /* Invalid MT or reaction identifier */
	 }
      } else {
	 mt = vec_num;
      }

      ierr = ndi_find_rprod_index(state->lib_stream, 
                                  &state->iso_stat.rprod_all, 
				  &rprod_index, mt);
      if ( ierr >= 0 ) {
	if ( rprod_index < state->iso_stat.rprod_all.num_objs ) {
	  *ret_val =  state->iso_stat.rprod_all.stat[rprod_index].length;
	} else {
	  ierr = ndi_check_array_err(state->iso_stat.rprod_all.num_objs);
	}
      }  else {
	 /* Data not in library */
	 ierr = 9;
	 *ret_val = 0;
      }
      break;
    
   case NDI_RPROD_RMO:  
      /* No. of products for reaction indexed by vec_num -- rmo  */
      if ( state->reac_id == 'c' ) {
	 mt = ndi_c_c_to_mt( vec_num );
	 if ( mt <= 0 ) {
	    ierr = -24;
	    return(ierr);    /* Invalid MT or reaction identifier */
	 }
      } else {
	 mt = vec_num;
      }

      ierr = ndi_find_rprod_index(state->lib_stream, 
                                  &state->iso_stat.rprod_rmo, 
				  &rprod_index, mt );
      if ( ierr >= 0 ) {
	if ( rprod_index < state->iso_stat.rprod_rmo.num_objs ) {
	  *ret_val =  state->iso_stat.rprod_rmo.stat[rprod_index].length;
	} else {
	  ierr = ndi_check_array_err(state->iso_stat.rprod_rmo.num_objs);
	}
      } else {
	 /* Data not in library */
	 ierr = 9;
	 *ret_val = 0;
      }
      break;
    
   case NDI_RPROD_FEW:  
      /* No. of products for reaction indexed by vec_num -- few  */
      if ( state->reac_id == 'c' ) {
	 mt = ndi_c_c_to_mt( vec_num );
	 if ( mt <= 0 ) {
	    ierr = -24;
	    return(ierr);    /* Invalid MT or reaction identifier */
	 }
      } else {
	 mt = vec_num;
      }

      ierr = ndi_find_rprod_index(state->lib_stream, 
                                  &state->iso_stat.rprod_few, 
				  &rprod_index, mt );
      if ( ierr >= 0 ) {
	if ( rprod_index < state->iso_stat.rprod_few.num_objs ) {
	  *ret_val =  state->iso_stat.rprod_few.stat[rprod_index].length;
	} else {
	  ierr = ndi_check_array_err(state->iso_stat.rprod_few.num_objs);
	}
      } else {
	 /* Data not in library */
	 ierr = 9;
	 *ret_val = 0;
      }
      break;

      /***********************************************************************/
	/*           This section looks at non-standard isotope data           */
      /***********************************************************************/

    case NDI_REAC_MT:
	/* Reaction mt for reaction indexed by vec_num */
	if (state->iso_stat.reac_mt) {
	    if (state->iso_stat.num_reac > vec_num) {
                if (state->reac_id == 'c') {
                    *ret_val = ndi_c_mt_to_c(state->iso_stat.reac_mt[vec_num]);
                    if (*ret_val <= 0) {
                        ierr = -24;
                    }
                } else {
                    *ret_val = state->iso_stat.reac_mt[vec_num];
                }
	    } else {
		/* Index out of range */
		ierr = ndi_check_array_err(state->iso_stat.num_reac);
	    }
	} else {
	    /* No data in library */
	    ierr = -9;
	}
	break;


    case NDI_INC_PART_TYPES:	/* Incident particle types */
	if (!state->chain_stat.set_flag) {
	    ierr = ndi_set_default_depl(state);
	}
	if (state->chain_stat.num_inc_parts <= 0) {
	    *ret_val = 1;	/* Default neutron */
	    ierr = 9;
	} else {
	    if (state->chain_stat.num_inc_parts > vec_num) {
		*ret_val = state->chain_stat.inc_part_types[vec_num];
	    } else {
		ierr =
		    ndi_check_array_err(state->chain_stat.num_inc_parts);
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
	    *ret_val = state->depl_target[vec_num].izaid;
	} else {
	    ierr = ndi_check_array_err(state->iso_stat.num_reac);
	}
	break;

    case NDI_RPRODS_ID:
	/* Return the id of the reaction product whose index is vec_num 
	   (for the target given by the global variable 'zaid'  */

	if (!state->ndi_depl_set) {
	    ierr = ndi_set_default_depl(state);
	}
	if (state->num_target <= 0) {
	    return (-7);
	}
	ierr = ndi_get_rprod_int_n(state, vec_num, ret_val);
	break;



      /*******************  UNKNOWN KEYWORD  ********************/

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}



/**********************************************************************/
/**********************************************************************/
static int get_int_val_n_tn(state_struct *state, int in_key, int vec_num,
			    int *ret_val)
{

    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */

    /*  Default (error) return value is 0  */
    *ret_val = 0;

    if (!state) {
	return (-2);
    }

    /*  Check if isotope is set  */
    if (!state->tn_stat.set_flag) {
	return (-7);
    }

   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {

    case NDI_RPRODS_ID:
	/* The reaction product ID specified by the index 'vec_num' */
	if (state->tn_stat.rprods.length > vec_num) {
	    ierr =
		get_nth_int(state, state->tn_stat.rprods, NDI_RPRODS_ID,
			    ret_val, vec_num);
	} else {
	    ierr = ndi_check_array_err(state->tn_stat.rprods.length);
	}
	break;

    case NDI_RPRODS_MLT:
	/* The reaction product multiplicity for product specified by vec_num  */
	if (state->tn_stat.rprods.length > vec_num)
	    ierr =
		get_nth_int(state, state->tn_stat.rprods, NDI_RPRODS_MLT,
			    ret_val, vec_num);
	else {
	    ierr = ndi_check_array_err(state->tn_stat.rprods.length);
	}
	break;

    case NDI_REACTANTS_ID:
	/* The reactant ID specified by the index 'vec_num' */
	if (state->tn_stat.reactants.length > vec_num) {
	    ierr =
		get_nth_int(state, state->tn_stat.reactants,
			    NDI_REACTANTS_ID, ret_val, vec_num);
	} else {
	    ierr = ndi_check_array_err(state->tn_stat.reactants.length);
	}
	break;

    case NDI_REACTANTS_MLT:
	/* The reactant multiplicity for product specified by vec_num  */
	if (state->tn_stat.reactants.length > vec_num)
	    ierr =
		get_nth_int(state, state->tn_stat.reactants,
			    NDI_REACTANTS_MLT, ret_val, vec_num);
	else {
	    ierr = ndi_check_array_err(state->tn_stat.reactants.length);
	}
	break;

       /*******************  UNKNOWN KEYWORD  ********************/

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}

/**********************************************************************/

static int get_int_val_n_tnreac(state_struct *state, int in_key,
				int vec_num, int *ret_val)
{

    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */

    /*  Default (error) return value is 0  */
    *ret_val = 0;

    if (!state) {
	return (-2);
    }

    /*  Check if isotope is set  */
    if (state->tnreac_stat.reac_num <= 0) {
	return (-7);
    }

   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {

    case NDI_NUM_RPRODS:	/* Number of reaction products */
	if (vec_num < state->tnreac_stat.reac_num) {
	    if (state->tnreac_stat.rprods[vec_num].length >= 0) {
		*ret_val = state->tnreac_stat.rprods[vec_num].length;
	    } else {
		ierr = -9;
	    }
	} else {
	    ierr = ndi_check_array_err(state->tnreac_stat.reac_num);
	}
	break;

    case NDI_NUM_REACTANTS:	/* Number of reactants */
	if (vec_num < state->tnreac_stat.reac_num) {
	    if (state->tnreac_stat.reactants[vec_num].length >= 0) {
		*ret_val = state->tnreac_stat.reactants[vec_num].length;
	    } else {
		ierr = -9;
	    }
	} else {
	    ierr = ndi_check_array_err(state->tnreac_stat.reac_num);
	}
	break;

       /*******************  UNKNOWN KEYWORD  ********************/

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}


/**********************************************************************/
/**********************************************************************/

static int get_nth_int(state_struct *state, stat_flag sf, int in_key,
		       int *return_val, int vec_num)
{
    int ierr = 0;
    int *return_vec;
    int vec_size;

    vec_size = ndi_c_get_size_h(state, in_key, &ierr);
    if (vec_size > 0) {
	return_vec = (int *) ndi_malloc(vec_size * sizeof(int));
	if (!return_vec) {
	    /* Out of memory */
	    return (-14);
	}
	ierr = ndi_c_get_int_vec_h(state, in_key, return_vec, vec_size);
	*return_val = return_vec[vec_num];
	if (return_vec)
	    ndi_free((void **) &return_vec);
    } else {
	ierr = 5;
    }

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/
