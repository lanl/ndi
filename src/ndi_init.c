
/*
 * File:	ndi_init.c
 * Description:	User function--open gendir file and initialize state.
 * Time-stamp:	<2000-02-04 16:07:55 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
/**********************************************************************/

/*  Purpose:  Open the 'gendir' directory file and assign to variable
    ndi_dir.  Initialize state variables and set default values.

    It is assumed that the gendir file exists either 1) in the current
    directory and is name 'gendir'or 2) the path (including file name)
    is given by the environment variable NDI_GENDIR_PATH or 3) is
    named gendir and is in the directory '/usr/local/codes/data/sn'.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. The 
    external file pointer variable ndi_dir is set to the open gendir
    file.  State variables are set: ndi_initialized,library_vers, 
    mat_order,...
***********************************************************************/
int ndi_c_init_h(state_struct **curr_state, const char *gendir_path)
{
    int i;
    int ierr = 0;		/* 0=no error,pos=warning, neg=error */
    int jerr;
    char *path = NULL;
    FILE *tmp_gendirp;
    state_struct *state;

    if (!curr_state) {
	return (-2);
    }
    *curr_state = NULL;

    /* Open directory file and initialize state variables */
    ierr = ndi_open_gendir(&tmp_gendirp, gendir_path, &path);

    if (ierr < 0) {
	/* Fatal error opening gendir */
	if (path)
	    ndi_free((void **) &path);
	return (ierr);
    }
    state = (state_struct *) ndi_calloc(1, sizeof(state_struct));
    state->ndi_initialized = -1;
    strcpy(state->open_gendir_file, path);
    state->ndi_dir = tmp_gendirp;
    if (path)
	ndi_free((void **) &path);

    /* Initialize variables */
    state->handle = -1;
    state->open_lib_file = NULL;
    ndi_fill_elements(state);

    /*  Set default values */
    if (state->library_vers)
	ndi_free((void **) &state->library_vers);
    state->library_vers =
	(char *) ndi_calloc(MAX_LIB_VERS_LEN, sizeof(char));
    strcpy(state->library_vers, "");
    state->particle_type = -1;
    state->library_type = ' ';
    state->mat_order = 'd';	/* No transpose */
    state->reac_id = 'm';	/* Use MT numbers to identify reactions */
    state->curr_reac_index = -1;	/* OBSOLETE? */
    strncpy(state->nuc_data_type, "", sizeof(state->nuc_data_type));	/* OBSOLETE */

    jerr = ndi_init_state_common(state);
    NDI_CURR_ERR(jerr, ierr);
    jerr = ndi_init_state_iso_state(state);
    NDI_CURR_ERR(jerr, ierr);
    jerr = ndi_init_state_tn(state);
    NDI_CURR_ERR(jerr, ierr);
    jerr = ndi_init_state_tnreac(state);
    NDI_CURR_ERR(jerr, ierr);
    jerr = ndi_init_state_chain(state);
    NDI_CURR_ERR(jerr, ierr);
    jerr = ndi_init_state_dedx(state);
    NDI_CURR_ERR(jerr, ierr);

    for (i = 0; i < MAX_HANDLE_NUM; i++)
	state->handle_map[i] = NULL;	/* handle pointers */
    state->handle_free = 0;

    *curr_state = state;

    return (ierr);

}

/**********************************************************************/

/*  Purpose:  Clone parent state to child state

***********************************************************************/
int ndi_c_clone_h(state_struct *parent, state_struct **child)
{
    int i;
    int ierr = 0;		/* 0=no error,pos=warning, neg=error */
    char *path = NULL;
    FILE *tmp_gendirp;
    state_struct *state;

    if (!child) {
	return (-2);
    }

    *child = NULL;

    ierr = ndi_open_gendir(&tmp_gendirp, parent->open_gendir_file, &path);

    if (ierr < 0) {
	/* Fatal error opening gendir */
	if (path)
	    ndi_free((void **) &path);
	return (ierr);
    }
    /* allocate state, declare uninitialized */
    state = (state_struct *) ndi_calloc(1, sizeof(state_struct));
    state->ndi_initialized = -1;
    strcpy(state->open_gendir_file, parent->open_gendir_file);
    state->ndi_dir = tmp_gendirp;


    /* Initialize variables */
    state->handle = -1;
    state->open_lib_file = NULL;
    ndi_fill_elements(state);

    /*  Set default values */
    if (state->library_vers)
	ndi_free((void **) &state->library_vers);
    state->library_vers =
	(char *) ndi_calloc(MAX_LIB_VERS_LEN, sizeof(char));
    strcpy(state->library_vers, "");
    state->particle_type = -1;
    state->library_type = ' ';
    state->mat_order = 'd';	/* No transpose */
    state->reac_id = 'm';	/* Use MT numbers to identify reactions */
    state->curr_reac_index = -1;	/* OBSOLETE? */
    strncpy(state->nuc_data_type, "", sizeof(state->nuc_data_type));	/* OBSOLETE */

    ierr = ndi_init_state_common(state);
    ierr = ndi_init_state_iso_state(state);
    ierr = ndi_init_state_tn(state);
    ierr = ndi_init_state_tnreac(state);
    ierr = ndi_init_state_chain(state);
    ierr = ndi_init_state_dedx(state);

    if (is_gendir(parent)) {
	state->particle_type = parent->particle_type;	/* lib_type option */
	state->library_type = parent->library_type;
	strcpy(state->library_vers, parent->library_vers);	/* library option */
	state->mat_order = parent->mat_order;	/* mat_order option */
	(void) ndi_set_collapse(state, parent->n_group);	/* collapse option */
	state->trans_corr = parent->trans_corr;	/* trans. corr. option */
	state->trans_corr_order = parent->trans_corr_order;
	state->no_norm = parent->no_norm;	/* norm option */
	state->reac_id = parent->reac_id;	/* reaction identifiers */
	state->e_avg_calc = parent->e_avg_calc;	/* avg. grp energy calc */
	state->kerma_calc = parent->kerma_calc;	/* default kerma calc */
	state->default_temp = parent->default_temp;	/* temperature option */
	strcpy(state->def_fiss_prod_id, parent->def_fiss_prod_id);	/* default fiss prod */
	/* Currently does not copy options:
	   E_BOUNDS_CURR_PART
	   DEPLETION
	   CHAIN
	   CURR_PART
	 */

    }

    for (i = 0; i < MAX_HANDLE_NUM; i++)
	state->handle_map[i] = NULL;	/* initialize handle pointers */
    state->handle_free = 0;

    *child = state;
    return (ierr);

}

/**********************************************************************/
