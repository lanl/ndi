
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
int ndi_c_init_h(state_struct **curr_state, const char *path)
{
    int i;
    int ierr = 0;		/* 0=no error,pos=warning, neg=error */
    state_struct *state;

    if (!curr_state) {
	return (-2);
    }

    *curr_state = NULL;
    state = (state_struct *) ndi_calloc(1, sizeof(state_struct));
    state->ndi_initialized = -1;

    /* Open directory file and initialize state variables */

    strcpy(state->open_gendir_file, path);
    ierr = ndi_open_gendir(&(state->ndi_dir), state->open_gendir_file);

    if (ierr < 0) {
	/* Fatal error opening gendir */
	state->ndi_initialized = 0;
	return (ierr);
    }

    /* Initialize variables */
    state->handle = -1;
    state->open_lib_file = (char *) ndi_malloc(sizeof(char));
    state->open_lib_file[0] = '\0';
    ndi_fill_elements(state);

    /*  Set default values */
    if (state->library_vers)
	ndi_free((void **) &state->library_vers);
    state->library_vers =
	(char *) ndi_calloc(MAX_LIB_VERS_LEN, sizeof(char));
    strncpy(state->library_vers, "", sizeof(state->library_vers));
    state->particle_type = -1;
    state->library_type = ' ';
    state->mat_order = 'd';	/* No transpose */
    state->reac_id = 'm';	/* Use MT numbers to identify reactions */
    state->curr_reac_index = -1;	/* OBSOLETE? */
    strncpy(state->nuc_data_type, "", sizeof(state->nuc_data_type));

    ierr = ndi_init_state_common(state);
    ierr = ndi_init_state_iso_state(state);
    ierr = ndi_init_state_tn(state);
    ierr = ndi_init_state_tnreac(state);
    ierr = ndi_init_state_chain(state);

    for (i = 0; i < MAX_HANDLE_NUM; i++)
	state->handle_map[i] = NULL;	/* handle pointers */

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
    state_struct *state;

    if (!child) {
	return (-2);
    }

    *child = NULL;
    /* allocate state, declare uninitialized */
    state = (state_struct *) ndi_calloc(1, sizeof(state_struct));
    state->ndi_initialized = -1;

    strcpy(state->open_gendir_file, parent->open_gendir_file);
    ierr = ndi_open_gendir(&(state->ndi_dir), state->open_gendir_file);
    if (ierr < 0) {
	/* Fatal error opening gendir */
	state->ndi_initialized = 0;
	return (ierr);
    }

    /* Initialize variables */
    state->handle = -1;
    state->open_lib_file = (char *) ndi_malloc(sizeof(char));
    state->open_lib_file[0] = '\0';
    ndi_fill_elements(state);

    /*  Set default values */
    state->library_vers =
	(char *) ndi_calloc(MAX_LIB_VERS_LEN, sizeof(char));
    strcpy(state->library_vers, parent->library_vers);
    state->particle_type = parent->particle_type;
    state->library_type = parent->library_type;
    state->mat_order = parent->mat_order;
    state->reac_id = parent->reac_id;
    state->curr_reac_index = parent->curr_reac_index;
    memcpy(state->nuc_data_type, parent->nuc_data_type, 2);

    ierr = ndi_init_state_common(state);
    ierr = ndi_init_state_iso_state(state);
    ierr = ndi_init_state_tn(state);
    ierr = ndi_init_state_tnreac(state);
    ierr = ndi_init_state_chain(state);

    for (i = 0; i < MAX_HANDLE_NUM; i++)
	state->handle_map[i] = NULL;	/* initialize handle pointers */

    *child = state;
    return (ierr);

}
/**********************************************************************/
