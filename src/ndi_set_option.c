/*
 * $Id$
 * Description:	User function--set defaults.
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ndi.h"
#include "ndi_internal.h"

static int ndi_set_lib_type(state_struct *state, const char *value);
static int ndi_set_library(state_struct *state, const char *value);
static int ndi_set_mat_order(state_struct *state, const char *value);
static int ndi_set_ebounds_curr_part(state_struct *state,
				     const char *value);
static int ndi_set_norm(state_struct *state, const char *value);
static int ndi_set_trans_corr_diag(state_struct *state, const char *value);
static int ndi_set_reac_id(state_struct *state, const char *value);
static int ndi_set_e_avg(state_struct *state, const char *value);
static int ndi_set_kerma_calc(state_struct *state, const char *value);

static int get_order(const char *value);
static int ndi_lib_dir(state_struct *state, int pt_type, char l_type,
		       const char *value_type, const char *in_value,
		       int reaction_flag);
static void ndi_find_zaid_string(char *in_line, int in_size,
				 char *zaid_string, int string_size,
				 char *seps);

/**********************************************************************/

/*  Purpose: Set options/defaults.

    Parameters:
    in_key -- in: key to specify option to set.
    in_value -- in: string specifying the value of the option.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok.

***********************************************************************/
int
ndi_c_set_option_h(state_struct *state, int in_key, const char *in_value)
{
    int ierr = 0;		/* Error flag */
    int pindex = 0;		/* Secondary particle index */
    char *value;		/* Copy of input value of option */
    int in_value_len = 0;	/* Size of the in_value string */


   /*** Check that NDI is initialized ***/
    if ((!state) || (!state->ndi_initialized)) {
	return (-2);
    }

    in_value_len = (int) strlen(in_value) + 1;
    value = (char *) ndi_malloc(in_value_len * sizeof(char));
    ndi_niceStrCp(value, in_value, in_value_len);


   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {

    case NDI_LIB_TYPE:		/* Library type  */
	ierr = ndi_set_lib_type(state, value);
	break;

    case NDI_LIBRARY:		/* Library name */
	ierr = ndi_set_library(state, value);
	break;

    case NDI_MAT_ORDER:	/* Matrix transpose order */
	ierr = ndi_set_mat_order(state, value);
	break;

    case NDI_COLLAPSE:		/* Energy group structure */
	ierr = ndi_set_collapse(state, value);
	break;

    case NDI_E_BOUNDS_CURR_PART:	/* Current particle energy group structure */
	ierr = ndi_set_ebounds_curr_part(state, value);
	break;

    case NDI_TRANS_CORR:	/* Use transport corrections */
	ierr = ndi_set_trans_corr_diag(state, value);
	break;

    case NDI_NORM:		/* Normalize chi and e_dist */
	ierr = ndi_set_norm(state, value);
	break;

    case NDI_REAC_ID:		/* Specify reaction identifiers as MT or C values */
	ierr = ndi_set_reac_id(state, value);
	break;
    case NDI_E_AVG:		/* Specify average bin energy calculation */
	ierr = ndi_set_e_avg(state, value);
	break;

    case NDI_KERMA:		/* Specify kerma/heating calculation */
	ierr = ndi_set_kerma_calc(state, value);
	break;

    case NDI_DEPLETION:	/* Set depletion chain */
	ierr = ndi_depletion(state, value);
	break;

    case NDI_CHAIN:		/* Set radiochemistry chain */
	ierr = ndi_radchem(state, value);
	break;

    case NDI_CURR_PART:	/* Specify current particle */
	if (state->library_type == 's') {

	    pindex =
		ndi_find_tn_part_index(strtol(value, (char **) NULL, 10),
				       state->tn_stat.sec_part,
				       state->sec_part_filled);
	} else if (state->library_type == 'm') {
	    /* Check that isotope is set */
	    if (!state->iso_stat.set_flag) {
		return (-7);
	    }
	    pindex =
		ndi_find_mg_part_index(atoi(value),
				       state->iso_stat.sec_part,
				       state->iso_stat.num_sec_parts);
	} else if (state->library_type == ' ') {
	    ierr = -16;
	    return (ierr);
	} else {
	    ierr = -4;
	    return (ierr);
	}

	state->curr_part_index = pindex;
	if ((ierr < 0) || (pindex < 0)) {
	    /* Invalid particle was input -- not found */
	    ierr = -5;
	}

	break;

    case NDI_TEMP:		/* Set default temperature  */
	state->default_temp = strtod(value, (char **) NULL);
	break;

    case NDI_FISS_PROD:	/* Set default fission product ZAID */
	if (strncmp(value, "default", 8) == 0) {
	    strncpy(state->def_fiss_prod_id, DEF_FISSION_PROD,
		    sizeof(state->def_fiss_prod_id));
	} else {
	    strncpy(state->def_fiss_prod_id, value,
		    sizeof(state->def_fiss_prod_id));
	    /*  Shouldn't need this--no one should be inputting symbols
	       ierr =
	       ndi_std_zaid(state, state->def_fiss_prod_id,
	       sizeof(state->def_fiss_prod_id));
	     */
	}
	break;

      /**********  UNKNOWN KEYWORD  ***********/

    default:
	ierr = -4;
	break;
    }

    if (value)
	ndi_free((void **) &value);
    return (ierr);
}


/**********************************************************************/
/*   FUNCTIONS INTERNAL TO FILE  */
/**********************************************************************/

static int ndi_set_lib_type(state_struct *state, const char *value)

/* Valid input values:
   multigroup_neutron

   Global variables changed (and valid values):
   particle_type -- 1-neutron, 0-photon, -1-tn
   library_type -- m-multigroup, s-tn, r-tnreactions, d-dosimetry,
                   p-multiparticle multigroup  

-------------------------------------------------------------------------*/
{
    int ierr = 0;

    if (!state) {
	return (-2);
    }

    if (strncmp(value, "multigroup_neutron", (19 * sizeof(char))) == 0) {
      /**** make section a global variable = multigroup_neutron? ****/

	state->particle_type = 1;
	state->library_type = 'm';
    } else if (strncmp(value, "tn", (3 * sizeof(char))) == 0) {
	state->particle_type = -1;
	state->library_type = 's';
    } else if (strncmp(value, "tnreactions", (12 * sizeof(char))) == 0) {
	state->particle_type = -1;
	state->library_type = 'r';
    } else if (strncmp(value, "multigroup_photon", (18 * sizeof(char))) ==
	       0) {
	state->particle_type = 0;
	state->library_type = 'm';

    } else if (strncmp(value, "dosimetry_neutron", (18 * sizeof(char))) ==
	       0) {
	state->particle_type = 1;
	state->library_type = 'd';

    } else if (strncmp(value, "multigroup_multi", (17 * sizeof(char))) ==
	       0) {
	state->particle_type = -1;
	state->library_type = 'p';

    } else {
	/* "Error:  invalid NDI input argument. */
	ierr = -5;
    }

    if (ierr >= 0) {
	ierr = ndi_init_state_common(state);
	ierr = ndi_init_state_iso_state(state);
	ierr = ndi_init_state_tn(state);
	ierr = ndi_init_state_tnreac(state);
	ierr = ndi_init_state_chain(state);
    }

/*    if ( ierr >= 0 ) ierr = ndi_set0(library_type); */


    return (ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_set_library(state_struct *state, const char *value)

/* Valid input values:
   mendf5, mendf6, rmoxsct, default

   Global variables changed (and valid values):
   library_vers --  mendf5, mendf6, rmoxsct, ""

-------------------------------------------------------------------------*/
{
    int ierr = 0;
    int val_length = (int) strlen(value);

/*** Check the !=; it sure doesn't seem right 1!!! */

    if (!state) {
	return (-2);
    }

    if (strncmp(value, "default", 8) == 0) {
	strncpy(state->library_vers, "", sizeof(state->library_vers));
    } else {
	if (ndi_lib_dir
	    (state, state->particle_type, state->library_type, "l", value,
	     0) == 1) {
	    if ((val_length + 1) < MAX_LIB_VERS_LEN) {	/* Size of library_vers is 13 in ndi_init */
		strncpy(state->library_vers, value,
			(val_length + 1) * sizeof(char));
	    } else {
		strncpy(state->library_vers, value,
			(MAX_LIB_VERS_LEN * sizeof(char)));
	    }
	} else {
	    ierr = 54;
	}
    }

    return (ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_set_mat_order(state_struct *state, const char *value)

/* Valid input values:
   default, transpose

   Global variables changed (and valid values):
   mat_order -- d, t

------------------------------------------------*/
{
    int ierr = 0;

    if (!state) {
	return (-2);
    }

    if (strstr(value, "default") != NULL) {
	state->mat_order = 'd';
    } else if (strstr(value, "transpose")) {
	state->mat_order = 't';
    } else if (strstr(value, "column")) {
	state->mat_order = 't';
    } else if (strstr(value, "row")) {
	state->mat_order = 'd';
    } else {
	/* "Error:  invalid NDI input argument. */
	ierr = -5;
    }
    return (ierr);
}

/**********************************************************************/
/**********************************************************************/

int ndi_set_collapse(state_struct *state, const char *value)

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

    if (!state) {
	return (-2);
    }

    if ((state->library_type == 'm') || (state->library_type == 'd')) {
	/* Multigroup or dosimetry */
	ierr =
	    ndi_set_e_grps(state->iso_stat.num_grps, state->lib_ebounds,
			   &state->grp_num_new, &state->collapse_ebounds,
			   value, state->library_type, &state->new_ebounds,
			   &state->collapse_flag);

    } else if (state->library_type == 's') {
	/* TN */
	if (state->new_ebounds)
	    ndi_free((void **) &state->new_ebounds);
	ierr =
	    ndi_set_e_grps(0, NULL, &state->grp_num_new,
			   &state->collapse_ebounds, value,
			   state->library_type, NULL,
			   &state->collapse_flag);
    } else {
	return (-23);
    }

    if (ierr >= 0) {
	strcpy(state->n_group, value);
    } else {
	strcpy(state->n_group, "default");
    }
    return (ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_set_ebounds_curr_part(state_struct *state,
				     const char *value)

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
    sec_part_mg *part;

    if (!state) {
	return (-2);
    }

    if ((state->library_type == 'm') || (state->library_type == 'd')) {
	/* Multigroup or dosimetry */
	if (state->iso_stat.sec_part &&
	    (state->curr_part_index >= 0) &&
	    (state->curr_part_index < state->iso_stat.num_sec_parts)) {
	    part = &state->iso_stat.sec_part[state->curr_part_index];
	    ierr =
		ndi_set_e_grps(part->num_grps, part->e_bounds,
			       &part->grp_num_new, &part->collapse_ebounds,
			       value, state->library_type,
			       &part->new_ebounds, &part->collapse_flag);
	    if (ierr >= 0) {
		strcpy(part->n_group, value);
	    } else {
		strcpy(part->n_group, "default");
	    }
	} else {
	    ierr = -999;
	}
    } else {
	/* Bad input for current library type */
	return (-4);
    }

    return (ierr);
}


/**********************************************************************/
/**********************************************************************/

static int ndi_set_norm(state_struct *state, const char *value)

/* Flag to determine whether to normalize chis and edist values

   Valid input values:
   norm, no_norm, default

   Global variables changed (and valid values):
   no_norm -- 0 (normalize); -1 (do not normalize)

------------------------------------------------*/
{
    int ierr = 0;

    if (!state) {
	return (-2);
    }

    if (strstr(value, "default") != NULL) {
	state->no_norm = 0;
    } else if (strstr(value, "no_norm")) {
	state->no_norm = -1;
    } else {
	state->no_norm = 0;
	ierr = -5;
    }

    return (ierr);
}


/**********************************************************************/
/**********************************************************************/

static int ndi_set_trans_corr_diag(state_struct *state, const char *value)
{
    int ierr = 0;
    int tmp_order;

    if (!state) {
	return (-2);
    }

    if (strncmp(value, "none", 4) == 0) {
	/* No transport correction */
	state->trans_corr = ' ';
	state->trans_corr_order = -1;
    } else if (strncmp(value, "diag_", 5) == 0) {
	tmp_order = get_order(value);;
	if ((tmp_order < 0) || (tmp_order > 20)) {
	    ierr = -5;
	} else {
	    state->trans_corr = 'd';
	    state->trans_corr_order = (int) tmp_order;
	}
    } else if (strncmp(value, "bhs_", 4) == 0) {
	tmp_order = get_order(value);
	if ((tmp_order < 0) || (tmp_order > 20)) {
	    ierr = -5;
	} else {
	    state->trans_corr = 'b';
	    state->trans_corr_order = (int) tmp_order;
	}
    } else {
	/* "Error:  invalid NDI input argument. */
	ierr = -5;
    }

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/

static int ndi_set_reac_id(state_struct *state, const char *value)
{
    int ierr = 0;

    if (!state) {
	return (-2);
    }

    if (strncmp(value, "mt", 3) == 0) {
	/* Use reaction MT's */
	state->reac_id = 'm';
    } else if (strncmp(value, "c", 2) == 0) {
	state->reac_id = 'c';
    } else {
	/* "Error:  invalid NDI input argument. */
	ierr = -5;
    }

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/

static int ndi_set_e_avg(state_struct *state, const char *value)
{
    int ierr = 0;

    if (strncmp(value, "from_vel", 9) == 0) {
	/* E = 1/2 m * v^2  */
	state->e_avg_calc = VEL_CALC;
    } else if (strncmp(value, "from_e", 7) == 0) {
	state->e_avg_calc = E_MEAN;
    } else if (strncmp(value, "from_sgndf", 11) == 0) {
	state->e_avg_calc = SGNDF;
    } else if (strncmp(value, "legacy_calc", 12) == 0) {
	state->e_avg_calc = LEGACY_VEL_CALC;
    } else if (strncmp(value, "from_flat", 10) == 0) {
	state->e_avg_calc = FLAT_CALC;
    } else if (strncmp(value, "from_td", 8) == 0) {
	state->e_avg_calc = TD_CALC;
    } else {
	/* "Error:  invalid NDI input argument. */
	ierr = -5;
    }

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/

static int ndi_set_kerma_calc(state_struct *state, const char *value)
{
    int ierr = 0;

    if (strncmp(value, "use_data", 9) == 0) {
	/* If kerma/heating not available on library, use calculation */
	state->kerma_calc = DATA;
    } else if (strncmp(value, "use_calc", 9) == 0) {
	state->kerma_calc = CALC;
    } else if (strncmp(value, "use_data_calc", 14) == 0) {
	state->kerma_calc = DATA_CALC;
    } else {
	/* "Error:  invalid NDI input argument. */
	ierr = -5;
    }

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/
static int get_order(const char *value)
{

    char *under_score = NULL;
    int return_value = -1;

    under_score = strchr(value, '_');
    if (under_score) {
      /*** Check value ***/
	return_value = (int) strtol((under_score + 1), NULL, 10);
    }
    return (return_value);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_lib_dir(state_struct *state, int pt_type, char l_type,
		       const char *value_type, const char *in_value,
		       int reaction_flag)

/*  Purpose: 
    
    Parameters: 
    pt_type -- in: incident particle type (e.g. 1=neutron)
    l_type -- in: library type (e.g. 'm'=multigroup)
    in_value -- in: zaid we're searching for on gendir
    reaction_flag -- Are we reading reaction data--depletion (True = -1, false=0)

    Return values:  Returns -1 on error, 0 if the value was not found, and 1 if
    the value was found.

***********************************************************************/
{

    char section[30];		/* Gendir section (e.g. neutron_multigroup) */
    size_t sec_len;		/* Length of 'section' string */
    char in_line[MAX_IN_STR_LEN];	/* Current input line */
    char zaid_string[MAX_IN_STR_LEN];	/* First string on current line */
    char seps[] = " ,\t\n";
    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */
    int done = 0;		/* Flag indicating when processing of zaid is done */
    char tmp_str[MAX_IN_STR_LEN];
    dir_entry dir_rec;


    /* Check if library type is set */

    if ((pt_type == 1) && (l_type == 'm')) {
	if (reaction_flag == 0) {
	    strcpy(section, "multigroup_neutron");
	} else {
	    strcpy(section, "neutron_depletion");
	}
    } else if ((pt_type == 1) && (l_type == 'd')) {
	if (reaction_flag == 0) {
	    strcpy(section, "dosimetry_neutron");
	} else {
	    strcpy(section, "radchem_chain");
	}
    } else if ((pt_type == 0) && (l_type == 'm')) {
	strcpy(section, "multigroup_photon");
    } else if ((pt_type == -1) && (l_type == 's')) {
	strcpy(section, "tndata");
    } else if ((pt_type == -1) && (l_type == 'r')) {
	strcpy(section, "tn_reactions");
	/*
	   } else if ( ( pt_type == 'n' ) && ( l_type == '0' ) ) {
	   strcpy( section, "neutron_depletion" );
	 */
    } else {
	/* Error: library type not set */
	return (-16);
    }

    if (!state->ndi_dir) {
	/* Error opening file */
	return (-1);
    }

    (void) rewind(state->ndi_dir);
    sec_len = strlen(section);

    while (done == 0) {

	/* Find correct section of directory file */
	if (fgets(in_line, sizeof(in_line), state->ndi_dir) != NULL) {
	    if (strncmp(in_line, section, sec_len) == 0) {

		while ((done == 0) && (ierr >= 0)) {

		    /* Search section */
		    if (fgets(in_line, sizeof(in_line), state->ndi_dir) !=
			NULL) {

			/* Check if current input line contains the desired value */
			if (strstr(in_line, "z=")) {
			    (void) ndi_find_zaid_string(in_line,
							MAX_IN_STR_LEN,
							zaid_string,
							MAX_IN_STR_LEN,
							seps);
			    ierr =
				read_dir_block(in_line, state->ndi_dir,
					       &dir_rec);
			    if (strncmp(value_type, "z", 2) == 0) {
				if (strncmp
				    (in_value, dir_rec.zaid,
				     strlen(in_value) + 1) == 0) {
				    return (1);
				}
			    } else if (strncmp(value_type, "l", 2) == 0) {
				if (strncmp
				    (in_value, dir_rec.lib,
				     strlen(in_value) + 1) == 0) {
				    return (1);
				}
			    } else if (strncmp(value_type, "f", 2) == 0) {
				if (strncmp
				    (in_value, dir_rec.path,
				     strlen(in_value) + 1) == 0) {
				    return (1);
				}
			    } else if (strncmp(value_type, "ft", 3) == 0) {
				if (strncmp
				    (in_value, dir_rec.lib_type,
				     strlen(in_value) + 1) == 0) {
				    return (1);
				}
			    } else if (strncmp(value_type, "o", 2) == 0) {
				sprintf(tmp_str, "%ld", dir_rec.offset);
				if (strncmp
				    (in_value, tmp_str,
				     strlen(in_value) + 1) == 0) {
				    return (1);
				}
			    } else if (strncmp(value_type, "aw", 3) == 0) {
				sprintf(tmp_str, "%lf", dir_rec.at_wgt);
				if (strncmp
				    (in_value, tmp_str,
				     strlen(in_value) + 1) == 0) {
				    return (1);
				}
			    } else {
				return (-1);
			    }
			}
		    } else {
			/* Error reading file */
			ierr = -11;
			done = -1;
		    }
		}		/* while */
	    }			/* strncmp */
	} else {
	    /* Error reading file */
	    ierr = -11;
	    done = -1;
	}
    }				/* while */

    return (ierr);
}

/**********************************************************************/

static void ndi_find_zaid_string(char *in_line, int in_size,
				 char *zaid_string, int string_size,
				 char *seps)
{

    /* This function assumes that the string is terminated by '\0'.
       There is no other test to prevent the function from going
       right off the end of the string.

       Also assumes that zaid is of the form z=92235 (no spaces).
     */

    char *curr_char = NULL;
    char *target_char = zaid_string;
    char *tmpstr;
    int i;
    int str_length;
    int curr_size = in_size;
    int target_count = 0;

    *zaid_string = '\0';

    if ((curr_char = strstr(in_line, "z="))) {
	/* Start of zaid should be two ( 'z' and '=') past curr_char */
	curr_char += 2;
	str_length = (int) (curr_char - in_line);
	curr_size = in_size - str_length / ((int) sizeof(char));
	if (curr_size <= 1) {
	    return;
	}

	for (i = 0; i < curr_size; i++) {
	    tmpstr = strchr(seps, *curr_char);
	    if ((*curr_char != '\0') &&
		(tmpstr == NULL) && (target_count < string_size)) {
		/* The current character is not is the seperation characters */
		*target_char = *curr_char;
		target_char++;
		target_count++;
		curr_char++;
	    } else {
		*target_char = '\0';
		break;
	    }
	}
    }

    return;
}

/***********************************************************************/
/***********************************************************************/
