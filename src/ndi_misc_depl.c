
/*
 * $Id$
 * Description:	Miscellaneous functions dealing with depletion (no floats)
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "ndi.h"
#include "ndi_internal.h"

#define MAX_SZA_SIZE  16

/**********************************************************************/

int ndi_depletion(state_struct *state, const char *in_zaid)
   /* The library type must already have been set to multigroup */
{
    int ierr = 0;
    int tmp_err = 0;
    FILE *depl_file = NULL;
    char library[20] = "";	/* Name of file (e.g. mendf5) */
    char lib_path[MAX_IN_PATH_LEN] = "";	/* Path of library file */
    char lib_file_type[] = "   ";	/* Currently only 'asc' is valid ascii */
    long int iso_start = 0;	/* Line number in file where isotope data begins */
    char tmp_zaid[20] = "";
    char in_lib_type = ' ';
    double at_wgt = 0.0;

    if (!state) {
	return (-2);
    }

    if (state->library_type != 'm') {
	if (state->library_type != ' ') {
	    return (-16);
	} else {
	    /* Incompatible library type */
	    return (-4);
	}
    }

    /* Initialize depletion data */
    ierr = ndi_init_state_chain(state);
    /* Read data from gendir */
    ierr =
	ndi_zaid_dir(state, state->particle_type, state->library_type,
		     in_zaid, tmp_zaid, library, lib_path, lib_file_type,
		     &iso_start, &at_wgt, -1);

    /* Check validity of data read from gendir */
    if (strcmp(lib_path, "") == 0 ) {
        ierr = -8;
        return(-8);
    }

    /* Set global variable to indicate the name of the new depl dataset */
    strcpy(state->depl_dataset, tmp_zaid);

    /* Open library file */
    if ((depl_file = ndi_open_lib_file(state, lib_path)) == NULL) {
	/* Error opening library file */
	ierr = -1;
    }

    /* Set new depletion state from library */
    in_lib_type = '0';
    ierr =
	ndi_init_iso_state(state, depl_file, in_lib_type, iso_start,
			   tmp_zaid);

    /* Close library */
    if (depl_file) {
	tmp_err = fclose(depl_file);
	if (tmp_err != 0)
	    ierr = -3;
    }

    if (ierr >= 0) {
	state->ndi_depl_set = -1;
    }
    return (ierr);

}

/**********************************************************************/
/**********************************************************************/

int ndi_set_default_depl(state_struct *state)
{

    int ierr = 0;
    char tmp_zaid[MAX_IN_STR_LEN] = "";	/* This 'zaid' will match any zaid in the gendir */
    char library[20];		/* Name of file (e.g. mendf5) */
    char lib_path[MAX_IN_PATH_LEN];	/* Path of library file */
    char lib_file_type[] = "   ";	/* Currently only 'asc' is valid ascii */
    long int iso_start;		/* Line number in file where isotope data begins */
    double at_wgt;

    if (!state) {
	return (-2);
    }

    ierr =
	ndi_zaid_dir(state, state->particle_type, state->library_type,
		     tmp_zaid, state->depl_dataset, library, lib_path,
		     lib_file_type, &iso_start, &at_wgt, -1);
    ierr = ndi_depletion(state, state->depl_dataset);

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/

int ndi_set_curr_target(state_struct *state, depl_target_struct **targ_p,
                        int *target_index)

     /* Set *targ_p to point at the target struct whose zaid is given by the global
        var 'zaid'.  The param target_index will be the index of the desired target in
        the targ_p  array.  If no such target is found, targ_p will be NULL and
        target_index will be set to -1.  On entry *targ_p will point to the beginning 
        of the target array.

        WARNING: the global variable depl_target should not be passed as the parameter
        *targ_p, since it will be changed within this function.

      */
{

    int i;
    int ierr = 0;
    int curr_index = -1;
    int itarget;
    char *token;
    char tmp_zaid[15];
    depl_target_struct *curr_targ = *targ_p;


    if (!state) {
	return (-2);
    }

    /* Initialize output variables */
    *targ_p = (depl_target_struct *) NULL;
    *target_index = -1;

    /* Check that depletion is set */
    if (!state->ndi_depl_set) {
	/* Depletion option not set */
	ierr = -30;
	return (ierr);
    }

    /* Change target zaid to integer */
    strcpy(tmp_zaid, state->iso_stat.zaid);
    token = strtok(tmp_zaid, ".");
    if (token != NULL) {
	itarget = (int) strtol(token, (char **) NULL, 10);
    } else {
	itarget = (int) strtol(tmp_zaid, (char **) NULL, 10);
    }

    if (!curr_targ) {
	return (-30);
    }

    /* Find target index in depl_target */
    for (i = 0; i < state->num_target; i++) {
	if (curr_targ->izaid == itarget) {
	    curr_index = i;
	    break;
	} else {
	    curr_targ++;
	}
    }
    if (curr_index < 0) {
	/* Current zaid not found in targets */
	ierr = -31;
	return (ierr);
    }

    *targ_p = curr_targ;
    *target_index = curr_index;

    return (ierr);

}

/**********************************************************************/
/**********************************************************************/

/* Set *rprod_p to point at the rprod struct whose zaid is given by
   the input rprod.  The param prod_index will be the index of the
   desired rprod in the rprod_p array.  If no such prod is found,
   rprod_p will be NULL and prod_index will be set to -1.  On entry
   *rprod_p will point to the beginning of the rprod array.
   
   On entering prod_index is the number of rprods in the current array.  
   On exit, it will be the index of the desired rprod.
*/
int ndi_set_curr_rprod(state_struct *state, depl_rprod_struct **rprod_p,
                       const char *rprod, int *prod_index)
{
    int i;
    int ierr = 0;
    int iprod;
    int num_rprods = *prod_index;
    depl_rprod_struct *curr_prod = *rprod_p;

    /* Initialize output variables */
    *rprod_p = (depl_rprod_struct *) NULL;
    *prod_index = -1;

    /* Check input */
    if (!state) {               /* State uninitialized error */
	return (-2);
    }

    if (!state->ndi_depl_set) {	/* Depletion uninitialized error */
	return (-30);
    }

    /* Change input rprod to integer */
    iprod = atoi(rprod);

    /* Search for iprod (or 99999 if fission product) in depletion set */
    for (i = 0; i < num_rprods; i++) {
	if (curr_prod[i].izaid == iprod  ||
             (atoi(state->def_fiss_prod_id) == iprod && 
              curr_prod[i].izaid == 99999)) {
	    break;
	}
    }

    if (i == num_rprods) { /* Reaction product not in depletion set error */
	return (-32);
    }

    *rprod_p = &curr_prod[i];
    *prod_index = i;

    return (ierr);
}

/**********************************************************************/
/**********************************************************************/
int ndi_radchem(state_struct *state, const char *in_zaid)
   /* The library type must already have been set to dosimetry */
{
    int ierr = 0;
    int tmp_err = 0;
    FILE *depl_file = NULL;
    char library[20] = "";	/* Name of file (e.g. mendf5) */
    char lib_path[MAX_IN_PATH_LEN] = "";	/* Path of library file */
    char lib_file_type[] = "   ";	/* Currently only 'asc' is valid ascii */
    long int iso_start = 0;	/* Line number in file where isotope data begins */
    char tmp_zaid[20] = "";
    char in_lib_type = ' ';
    double at_wgt = 0.0;

    if (!state) {
	return (-2);
    }

    if (state->library_type != 'd') {
	if (state->library_type != ' ') {
	    return (-16);
	} else {
	    /* Incompatible library type */
	    return (-4);
	}
    }

    /* Initialize depletion data? */
    ierr = ndi_set0(state, '0');
    /* Read data from gendir */
    ierr =
	ndi_zaid_dir(state, state->particle_type, state->library_type,
		     in_zaid, tmp_zaid, library, lib_path, lib_file_type,
		     &iso_start, &at_wgt, -1);

    /* Check validity of data read from gendir */

    /* Set global variable to indicate the name of the new depl dataset */
    strcpy(state->depl_dataset, tmp_zaid);

    /* Open library file */
    if ((depl_file = ndi_open_lib_file(state, lib_path)) == NULL) {
	/* Error opening library file */
	ierr = -1;
    }

    /* Set new depletion state from library */
    in_lib_type = '0';
    ierr =
	ndi_init_iso_state(state, depl_file, in_lib_type, iso_start,
			   tmp_zaid);

    /* Close library */
    if (depl_file) {
	tmp_err = fclose(depl_file);
	if (tmp_err != 0)
	    ierr = -3;
    }

    if (ierr >= 0) {
	state->ndi_depl_set = -1;
    }
    return (ierr);

}

/**********************************************************************/
/**********************************************************************/
