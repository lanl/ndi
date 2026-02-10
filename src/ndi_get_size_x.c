
/*
 * $Id$
 * Description: User function--returns size of the named element of a
 * multi-dim array.
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

static int get_size_x_gendir(state_struct *state, int in_key,
                             const char *in_str, int *ierr);
static int get_mg_size_x(state_struct *state, int in_key,
                         const char *in_str, int *ierr);
static int get_dedx_size_x(state_struct *state, int in_key,
                           const char *in_str, int *ierr);
static int get_reac_mt_size(state_struct *state, const char *rprod_in,
                            int *ierr);

/**********************************************************************/

int ndi_c_get_size_x_h(state_struct *state, int in_key,
                       const char *in_str, int *ierr)

/* Purpose: Return the size of a "vector" in a multi-dimensional array.
   The vector is identified by in_str (a zero-based index).

   Parameters:
   in_key -- in: key to specify type of data required
   ierr -- out: error flag, where 0=no error,pos=warning, neg=error
   in_str -- in: indicates the element for which to find the size.

   Returns: Returns the integer indicating the number of elements
   in the array.  For single-valued data (temperature, sigma0, etc.)
   "1" is returned.  For string data the size of the string (number of
   characters plus the end of string character) is returned.  Returns 
   zero or negative as error.

**********************************************************************/
{
    int size = 0;               /* Default return value is '0' */

    *ierr = 0;                  /* Default error value */

    if ((!state) || (!state->ndi_initialized)) {
        NDI_THROW(*ierr, -2);   /* State not initialized */
    }

    if (is_gendir(state)) {
        size = get_size_x_gendir(state, in_key, in_str, ierr);
    } else if ((state->library_type == 'm') ||
               (state->library_type == 'd')) {
        size = get_mg_size_x(state, in_key, in_str, ierr);
    } else if (state->library_type == 'x') {
        size = get_dedx_size_x(state, in_key, in_str, ierr);
    } else if (state->library_type == ' ') {
        NDI_THROW(*ierr, -16);  /* library type not set */
    } else {
        NDI_THROW(*ierr, -4);   /* Incompatible library type */
    }

    if (size < 0) {
        size = 0;
    }

 catch:
    return size;
}

/**********************************************************************/
/**********************************************************************/

static int get_size_x_gendir(state_struct *state, int in_key,
                             const char *in_str, int *ierr)
{
    int count = 0;

    if (!state && !state->ndi_dir) /* check state sanity */
        NDI_THROW(*ierr, -2);

    switch (in_key) {

    case NDI_ZAID:
    case NDI_TEMP:
    case NDI_SIG_0:
    case NDI_AT_WGT:
    case NDI_AWR:
        count = ndi_count_dir_block(state->ndi_dir, in_key, 
                                    ndi_get_section(state),
                                    state->library_vers, in_str);
        if (count < 0) {
            *ierr = count;
            count = 0;
        }
        if ( count == 0 ) *ierr = 19;
        break;
    default:
        NDI_THROW(*ierr, -4);      /* Unrecognized in_key */

    }

 catch:
    return count;
}

/**********************************************************************/
/**********************************************************************/

static int get_mg_size_x(state_struct *state, int in_key,
                         const char *in_str, int *ierr)
{
    int return_value = 0;       /* Default return value is '0' */


    if (!state->iso_stat.set_flag) {
        NDI_THROW(*ierr, -7);   /* Isotope not set */
    }

    /*************************/
    /*   Process input key   */
    /*************************/

    switch (in_key) {

    case NDI_TOT_PROD_XS:       /* Total production cross section */
        if (state->collapse_flag != 0) {
            return_value = state->grp_num_new;
        } else {
            return_value = state->iso_stat.num_grps;
        }
        break;

    case NDI_REAC_MT:
    case NDI_RPROD_MLT:
        return_value = get_reac_mt_size(state, in_str, ierr);
        break;

        /***********  UNKNOWN KEYWORD  **************/

    default:
        NDI_THROW(*ierr, -4);
        break;
    }

 catch:
    return return_value;
}

/**********************************************************************/
/**********************************************************************/

static int get_dedx_size_x(state_struct *state, int in_key, 
                           const char *in_str, int *ierr)
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

    /****    Single-valued data -- return "1" ****/

    case NDI_TARGET_ZAID:       /* Total production Q for input product */
        return_value = 1;
        break;

    /****  Multi-dimensional array ****/

    case NDI_TARGET_DEDX:       /* dE/dx for target. Identical for all targets  */
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
/*********************************************************************/

static int get_reac_mt_size(state_struct *state, const char *rprod_in,
                            int *ierr)
{
    int curr_index = -1;
    depl_target_struct *curr_targ;
    depl_rprod_struct *curr_prod;

    *ierr = 0;
    if (state) {
        curr_targ = state->depl_target;
    } else {
        *ierr = -2;
        return 0;
    }

    /* CHECK ERRORS */

    if (!state->ndi_depl_set) {
        *ierr = ndi_set_default_depl(state);
    }
    if (state->num_target <= 0) {
        *ierr = -7;
        return 0;
    }

    *ierr = ndi_set_curr_target(state, &curr_targ, &curr_index);
    if (curr_index == -1) {
        return 0;
    }

    curr_prod = curr_targ->rprod;
    curr_index = curr_targ->num_rprods;
    *ierr = ndi_set_curr_rprod(state, &curr_prod, rprod_in, &curr_index);
    if (curr_index == -1) {
        return 0;
    }

    return curr_prod->num_reac;

}
/*********************************************************************/
/**********************************************************************/
