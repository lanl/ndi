
/*
 *$Id$
 * Description:	User function--check defaults.
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ndi.h"
#include "ndi_internal.h"

static int ndi_get_lib_type(state_struct *state, char *value,
			    int string_size);
static int ndi_get_library(state_struct *state, char *value,
			   int string_size);
static int ndi_get_mat_order(state_struct *state, char *value,
			     int string_size);
static int ndi_get_collapse(state_struct *state, char *value,
			    int string_size);
static int ndi_get_trans_corr_diag(state_struct *state, char *value,
				   int string_size);
static int ndi_get_norm(state_struct *state, char *value, int string_size);
static int ndi_get_reac_id(state_struct *state, char *value,
			   int string_size);
static int ndi_get_e_avg(state_struct *state, char *value,
			 int string_size);
static int ndi_get_kerma_calc(state_struct *state, char *value,
			      int string_size);

/**********************************************************************/
int ndi_c_check_option_h(state_struct *state, int in_key, char *out_value,
			 int string_size)
/*  Purpose: Check options/defaults.

    Parameters:
    in_key -- in: key to specify option to set.
    out_value -- out: string specifying the value of the option.
    string_size -- in: number of characters in out_value

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok.

***********************************************************************/
{
    int ierr = 0;		/* Error flag: 0=no error, pos=warning, neg=error */

    if (!state) {
	return (-2);
    }

   /*** Check that NDI is initialized ***/

   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {

    case NDI_LIB_TYPE:
	/*  Set library type  */
	ierr = ndi_get_lib_type(state, out_value, string_size);
	break;

    case NDI_LIBRARY:
	/* Set source library */
	ierr = ndi_get_library(state, out_value, string_size);
	break;

    case NDI_MAT_ORDER:
	/* Set flag to transpose order of returned matrix */
	ierr = ndi_get_mat_order(state, out_value, string_size);
	break;

    case NDI_COLLAPSE:
	/* Set no. of energy groups to collapse to */
	ierr = ndi_get_collapse(state, out_value, string_size);
	break;

    case NDI_TRANS_CORR:
	/* Use transport corrections */
	ierr = ndi_get_trans_corr_diag(state, out_value, string_size);
	break;

    case NDI_NORM:
	/* Normalize chis and edists? */
	ierr = ndi_get_norm(state, out_value, string_size);
	break;

    case NDI_REAC_ID:
	/* Specify reaction identifiers--MT or C values */
	ierr = ndi_get_reac_id(state, out_value, string_size);
	break;

    case NDI_E_AVG:
	/* Specify how to calculate average bin energies */
	ierr = ndi_get_e_avg(state, out_value, string_size);
	break;

    case NDI_KERMA:
	/* Kerma and heating calculated if not found on library? */
	ierr = ndi_get_kerma_calc(state, out_value, string_size);
	break;

    case NDI_CHAIN:
	/* case NDI_DEPLETION:
	   case NDI_RADCHEM: */
	/*  Set chain data -- (depl for MG/radchem for dos) */
	if (state->library_type == 'm') {
	    if (state->chain_stat.set_flag) {
		strncpy(out_value, state->chain_stat.dataset, string_size);
	    } else {
		ierr = 9;
	    }
	} else {
	    /* Incompatible library type */
	    if (state->library_type == ' ') {
		ierr = -16;
	    } else {
		ierr = -4;
	    }
	}
	break;

    case NDI_CURR_PART:
	/* Which particle is 'current'  */
	if (state->curr_part_index >= 0) {
	    if (state->library_type == 's') {
		sprintf(out_value, "%-ld",
			state->tn_stat.sec_part[state->curr_part_index].
			part_type);
/* 	 strncpy( out_value, state->tn_stat.sec_part[state->curr_part_index].part_type, string_size ); */
	    } else if (state->library_type == 'm') {
		if (sprintf
		    (out_value, "%d",
		     state->iso_stat.sec_part[state->curr_part_index].
		     part_type)
		    > string_size) {
		    /*  ??? */
		}
	    } else {
		/* Operation is valid only for tn data */
		if (state->library_type == ' ') {
		    ierr = -16;
		} else {
		    ierr = -4;
		}
	    }
	} else {
	    ierr = 9;
	}
	break;

    case NDI_E_BOUNDS_CURR_PART:

	break;

    case NDI_TEMP:
	/*  Default temperature  */
	if (sprintf(out_value, "%e", state->default_temp) > string_size) {
	    /* ???? */
	}
	break;

    case NDI_FISS_PROD:
	/* Default fission product ZAID */
	strncpy(out_value, state->def_fiss_prod_id, string_size);
	break;

    case NDI_PARTICLE_TYPE:
	if (sprintf(out_value, "%d", state->particle_type) > string_size) {
	    /*  ??  */
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
/*   FUNCTIONS INTERNAL TO FILE  */
/**********************************************************************/

static int ndi_get_lib_type(state_struct *state, char *value,
			    int string_size)

/* Valid library type values:
   multigroup_neutron, tn, multigroup_photon
   
   Global variables used (and valid values):
   particle_type -- 1-neutorn, 0-photon, -1(tn)
   library_type -- m, s(tn)
   
-------------------------------------------------------------------------*/
{
    int ierr = 0;

    if (!state) {
	return (-2);
    }

    if ((state->particle_type == 1) && (state->library_type == 'm')) {
	strncpy(value, "multigroup_neutron", string_size);
    } else if ((state->particle_type == -1) && (state->library_type == 's')) {
	strncpy(value, "tn", string_size);
    } else if ((state->particle_type == -1) && (state->library_type == 'r')) {
	strncpy(value, "tnreactions", string_size);
    } else if ((state->particle_type == 0) && (state->library_type == 'm')) {
	strncpy(value, "multigroup_photon", string_size);
    } else if ((state->particle_type == 1) && (state->library_type == 'd')) {
	strncpy(value, "dosimetry_neutron", string_size);
    } else if ((state->particle_type == -1) && (state->library_type == 'p')) {
	strncpy(value, "multigroup_multi", string_size);
    } else {
         strncpy(value, "", string_size);
	ierr = 9;
    }

    return (ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_get_library(state_struct *state, char *value,
			   int string_size)

/* Valid library values:
   mendf5, mendf6, rmoxsct, default

   Global variables used (and valid values):
   library_vers --  mendf5, mendf6, rmoxsct, ""

-------------------------------------------------------------------------*/
{
    int ierr = 0;

    if (!state) {
	return (-2);
    }

   if (state->library_vers) {
       strncpy( value, state->library_vers, string_size );
   } 

    if (strncmp(value, "", string_size) == 0) {
	strncpy(value, "default", string_size);
    }

    return (ierr);
}

/**********************************************************************/
/**********************************************************************/

static int
ndi_get_mat_order(state_struct *state, char *value, int string_size)

/* Valid mat_order values:
   default, transpose

   Global variables used (and valid values):
   mat_order -- d, t

------------------------------------------------*/
{
    int ierr = 0;

    if (!state) {
	return (-2);
    }

    if (state->mat_order == 'd') {
	strncpy(value, "default", string_size);
    } else if (state->mat_order == 't') {
	strncpy(value, "transpose", string_size);
    } else {
	strncpy(value, "", string_size);
	ierr = 9;
    }

    return (ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_get_collapse(state_struct *state, char *value,
			    int string_size)

/* Valid collapse values:
   30_group, 21_group, 16_group, 14_group, 13_group, 12_group, 10_group,
   9_group, 8_group, 7_group, 6_group, 5_group, 4_group, 3_group, 
   2_group, 1_group, 16_LLNL, default

   Global variables used (and valid values):
   grp_num_new -- integer giving number of groups; 0 for default or error
   new_ebounds -- array of indices of lib_ebounds array with new energy 
                  boundaries (i.e. new boundaries subset of library bounds).
   collapse_flag -- 0 = no collapsing required, -1 = collapsing required
   n_group -- (same as valid input values)

-------------------------------------------------------------------------*/
{
    int ierr = 0;

    if (!state) {
	return (-2);
    }

    if (*state->n_group != '\0') {
	strncpy(value, (state->n_group), string_size);
    } else {
	strncpy(value, "default", string_size);
    }
    return (ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_get_trans_corr_diag(state_struct *state, char *value,
				   int string_size)

/* Valid  values:
   none, diag_n, bhs_n 
   (where 'n' is an integer indicating the correction order)

   Global variables used (and valid values):
   trans_corr -- " "=no correction, d=diagonal, b=Bell-Hansen-Sandmeier 
   trans_corr_order -- integer giving transport correction order

-------------------------------------------------------------------------*/
{
    int ierr = 0;
    char order_str[5];
    char temp_str[11] = "diag_";

    if (!state) {
	return (-2);
    }

    if (state->trans_corr == ' ') {
	strncpy(value, "none", string_size);
    } else {
	if (state->trans_corr_order < 100000) {
	    sprintf(order_str, "%d", state->trans_corr_order);
	}
	if (state->trans_corr == 'd') {
	    strncat(temp_str, order_str, 6);
	    strncpy(value, temp_str, string_size);
	} else if (state->trans_corr == 'b') {
	    strcpy(temp_str, "bhs_");
	    strncat(temp_str, order_str, 6);
	    strncpy(value, temp_str, string_size);
	} else {
	    strncpy(value, "", string_size);
	    ierr = -27;
	}
    }

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/

static int ndi_get_norm(state_struct *state, char *value, int string_size)

/* Valid  values:
   norm, no_norm

   Global variables used (and valid values):
   no_norm -- 0=normalize, -1=do not normalize

-------------------------------------------------------------------------*/
{
    int ierr = 0;

    if (!state) {
	return (-2);
    }

    if (state->no_norm == 0) {
	strncpy(value, "norm", string_size);
    } else if (state->no_norm != 0) {
	strncpy(value, "no_norm", string_size);
    } else {
	strncpy(value, "", string_size);
	ierr = 9;
    }

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/

static int ndi_get_reac_id(state_struct *state, char *value,
			   int string_size)

/* Valid  values:
   mt, c

   Global variables used (and valid values):
   reac_id --  Reaction identifier: m=MT (default), c=c-value

-------------------------------------------------------------------------*/
{
    int ierr = 0;

    if (!state) {
	return (-2);
    }

    if (state->reac_id == 'm') {
	strncpy(value, "mt", string_size);
    } else if (state->reac_id == 'c') {
	strncpy(value, "c", string_size);
    } else {
	strncpy(value, "", string_size);
	ierr = -27;
    }

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/

static int ndi_get_e_avg(state_struct *state, char *value, int string_size)

/* Valid  values:
  "from_vel", "from_e"

   Global variables used (and valid values):
   enum e_avg_calc --  VEL_CALC, E_MEAN, SGNDF

-------------------------------------------------------------------------*/
{
    int ierr = 0;

    if (state->e_avg_calc == VEL_CALC) {
	strncpy(value, "from_vel", string_size);
    } else if (state->e_avg_calc == E_MEAN) {
	strncpy(value, "from_e", string_size);
    } else if (state->e_avg_calc == SGNDF) {
	strncpy(value, "from_sgndf", string_size);
    } else if ( state->e_avg_calc == LEGACY_VEL_CALC ) {
        strncpy( value, "legacy_calc", string_size );
    } else if ( state->e_avg_calc == FLAT_CALC ) {
        strncpy( value, "from_flat", string_size );
    } else if ( state->e_avg_calc == TD_CALC ) {
        strncpy( value, "from_td", string_size );
    } else {
	strncpy(value, "", string_size);
	ierr = -27;
    }

    return (ierr);


}

/**********************************************************************/
/**********************************************************************/

static int ndi_get_kerma_calc(state_struct *state, char *value,
			      int string_size)

/* Valid  values:
  "use_calc", "no_calc"

   Global variables used (and valid values):
   enum kerma_calc_type kerma_calc -- DATA, CALC, DATA_CALC

-------------------------------------------------------------------------*/
{
    int ierr = 0;

    if (state->kerma_calc == CALC) {
	strncpy(value, "use_calc", string_size);
    } else if (state->kerma_calc == DATA_CALC) {
	strncpy(value, "use_data_calc", string_size);
    } else if (state->kerma_calc == DATA) {
	strncpy(value, "use_data", string_size);
    } else {
	strncpy(value, "", string_size);
	ierr = -27;
    }

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/
