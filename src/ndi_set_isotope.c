
/*
 * $Id$
 * Description: User function--set isotope for processing.
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

/**********************************************************************/

/*  Purpose: Set the isotope which all future processing will work on.
    This will also open the library file and read in isotope info.

    Parameters: 
    in_zaid -- input specification of isotope to process. This can be
               as little as an element specification (e.g. U or 92).

    Assumptions:  Isotope state will be initialized when function is
    entered.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 
    May change the value of extern variable lib_stream.

***********************************************************************/
int ndi_c_set_zaid_h(state_struct *state, const char *in_zaid)
{
    int ierr = 0;                  /* Error flag: 0=no error,pos=warning, neg=error */
    int tmp_err = 0;
    char library[20];              /* Name of file (e.g. mendf5) */
    char lib_path[MAX_IN_PATH_LEN];/* Path of library file */
    char lib_file_type[] = "   ";  /* Currently only 'asc' is valid ascii */
    long int iso_start;            /* Line number in file where isotope data begins */
    char tmp_zaid[MAX_IN_STR_LEN];
    double at_wgt;
    char real_zaid[MAX_IN_STR_LEN];

   /*** Check that library type has been set ***/

   /*** Check that ndi_init has been called. */
    if ((!state) || (!state->ndi_initialized)) {
        return -2;
    }

    if (state->library_type == ' ') {
        /* Library type was not set */
        return -16;
    }

    /* Initialize (zero) isotope state */
    ierr = ndi_set0(state, state->library_type);

   /*** check returned error value ***/

    /* Convert input zaid to standard zaid and read data from gendir */
    strcpy(tmp_zaid, in_zaid);
    if ((state->library_type == 'm') || (state->library_type == 'd') ||
        (state->library_type == 'x')) {
        if (!(strstr(in_zaid, "-999") || strstr(in_zaid, "99999"))) {
            ierr = ndi_std_zaid(state, tmp_zaid, sizeof(tmp_zaid));
        }
        /* printf("tmp_zaid = %s \n", tmp_zaid ); */
      /*** Check returned error value  ***/
        if (ierr < 0)
            return ierr;
    }

    ierr =
        ndi_zaid_dir(state, state->particle_type, state->library_type,
                     tmp_zaid, real_zaid, library, lib_path, lib_file_type,
                     &iso_start, &at_wgt, 0);

    if (ierr < 0) {
        if (ierr == -8 && state->particle_type == 'd') ierr = -37;
        return ierr;
    }

   /*** Check returned error value ***/

    /* Check validity of data read from gendir */
    if (strcmp(lib_file_type, "bin") == 0) {
        state->binary = 1;
    } else if (strcmp(lib_file_type, "asc") == 0) {
        state->binary = 0;
    } else {
        /* Only ASCII and binary library files are currently handled. */
        if (ierr >= 0) {
            ierr = -15;
        }
        return ierr;
    }

    /* Open library file if needed */
    if (state->open_lib_file) {
        if (strcmp(lib_path, state->open_lib_file) != 0) {
            /* Close current lib file and open new one */
            if (state->lib_stream)
                ierr = ndi_close_lib_file(state);
            if ((state->lib_stream =
                 ndi_open_lib_file(state, lib_path)) == NULL) {
                /* Error opening library file */
                ierr = -1;
            }
        }
    } else {
        /* No library file is currently open */
        if ((state->lib_stream =
             ndi_open_lib_file(state, lib_path)) == NULL) {
            /* Error opening library file */
            ierr = -1;
        }
    }

   /*** check returned error value ****/

    /* Set new isotope state */
    ierr =
        ndi_init_iso_state(state, state->lib_stream, state->library_type,
                           iso_start, real_zaid);

    if ((state->library_type == 'm') || (state->library_type == 'd')) {
      /*** Check returned error value ***/
        /*  Set energy bounds for collapsing */
        if (strcmp(state->n_group, "default") != 0) {
            tmp_err =
                ndi_set_e_grps(state->iso_stat.num_grps,
                               state->lib_ebounds, &state->grp_num_new,
                               &state->collapse_ebounds, state->n_group,
                               state->library_type, &state->new_ebounds,
                               &state->collapse_flag);
        }
    }

    if ((tmp_err != 0) && (ierr >= 0)) {
        ierr = tmp_err;
    }

    return ierr;

}

/**********************************************************************/

/*  Purpose: Set the isotope which all future processing will work on.
    This will also open the library file and read in isotope info.

    Parameters: 
    in_zaid -- input specification of isotope to process. This can be
               as little as an element specification (e.g. U or 92).

    Assumptions:  Isotope state will be initialized when function is
    entered.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 
    May change the value of extern variable lib_stream.

***********************************************************************/
int ndi_c_set_isotope_h(state_struct *state, const char *in_zaid)
{

    int ierr = 0;

    ierr = ndi_c_set_zaid_h(state, in_zaid);

    return ierr;
}

/**********************************************************************/

/*  Purpose: Set the isotope which all future processing will work on.
    This will also open the library file and read in isotope info.

    Parameters: 
    in_zaid -- input specification of isotope to process. This can be
               as little as an element specification (e.g. U or 92).

    Assumptions:  Isotope state will be initialized when function is
    entered.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 
    May change the value of extern variable lib_stream.

***********************************************************************/
int ndi_c_set_reaction_h(state_struct *state, const char *in_zaid)
{

    int ierr = 0;

    ierr = ndi_c_set_zaid_h(state, in_zaid);

    return ierr;
}


/**********************************************************************/
