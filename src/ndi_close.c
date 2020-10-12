
/*
 * File:        ndi_close.c
 * Description: User function--deallocate memory and close files.
 * Time-stamp:  <2000-02-04 16:09:47 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

/**********************************************************************/

/*  Purpose:  Deallocate memory and close directory and library files.
    
    It is assumed that NDI has been initialized (ndi_initialized = -1).
    
    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok.
    
***********************************************************************/
int ndi_c_close_h(state_struct **curr_state)
{
    int i;
    int ierr = 0;                /* 0=no error,pos=warning, neg=error */
    int ret_code = 0;            /* Return code--returned error value */
    state_struct *state;

    if (curr_state) {
        state = *curr_state;
        if (!state) {
            return -2;
        }
    } else {
        return -2;
    }

    if (!state->ndi_initialized) {
        /* Warning: NDI close failed--already closed */
        return -2;
    }

    for (i = 0; i < MAX_HANDLE_NUM; i++)        /* close children */
        if (state->handle_map[i] != NULL)
            ierr = ndi_c_close_h(&(state->handle_map[i]));
    state->handle_free = 0;

    /* Close directory file and library file */
    if (state->ndi_dir) {
        if (fclose(state->ndi_dir) != 0) {
            ret_code = 3;
        } else {
            state->ndi_dir = NULL;
            strcpy(state->open_gendir_file, "");
        }
    }

    ierr = ndi_close_lib_file(state);
    if (state->open_lib_file) {
        ndi_free((void **) &state->open_lib_file);
    }

    if (ierr < 0) {
        ret_code = ierr;
    } else if ((ret_code >= 0) && (ierr != 0)) {
        ret_code = ierr;
    }

    /* Initialize extern variables */
    if (state->ndi_elements)
        ndi_free((void **) &state->ndi_elements);
    if (state->library_vers)
        ndi_free((void **) &state->library_vers);
    state->particle_type = -1;
    state->library_type = ' ';
    state->mat_order = 'd';        /* No transpose */
    state->reac_id = 'm';          /* Use MT numbers to identify reactions */
    state->e_avg_calc = VEL_CALC;
    state->kerma_calc = DATA;
    state->curr_reac_index = -1;
    strncpy(state->nuc_data_type, "", sizeof(state->nuc_data_type));        /* OBSOLETE? */

    ierr = ndi_init_state_common(state);
    ierr = ndi_init_state_iso_state(state);
    ierr = ndi_init_state_tn(state);
    ierr = ndi_init_state_tnreac(state);
    ierr = ndi_init_state_chain(state);

    ndi_free((void **) curr_state);

    return ret_code;

}

/**********************************************************************/
