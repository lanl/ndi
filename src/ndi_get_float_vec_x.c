/*
 * $Id$
 * Description: User function--returns vector from multi-dim array
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"

static int GET_FLOAT_VEC_X_MG(state_struct *state, int in_key, 
                              const char *in_str,
                              FLOAT_TYPE *return_vec, int vec_size);

static int GET_FLOAT_VEC_X_GENDIR(state_struct *state, int in_key,
                                  const char *in_str,
                                  FLOAT_TYPE *return_vec, int vec_size);
/**********************************************************************/

int NDI_GET_FLOAT_VEC_X_H(state_struct *state, int in_key,
                          const char *in_str, 
                          FLOAT_TYPE *return_vec, int vec_size)
{

    int ierr = 0;

    /* Initialize return_vec with zeroes */
    ierr = NDI_INIT_FLOAT_VEC(return_vec, vec_size);

    if (!state || !state->ndi_initialized)
        NDI_THROW(ierr, -2);

#ifdef FLOAT32
    if (sizeof(float) == 8) {
        NDI_THROW(ierr, -2);
    }
#endif

    if (is_gendir(state)) {
        ierr = GET_FLOAT_VEC_X_GENDIR(state, in_key, in_str, 
                                      return_vec, vec_size);
    } else if ((state->library_type == 'm') ||
               (state->library_type == 'd')) {
        ierr = GET_FLOAT_VEC_X_MG(state, in_key, in_str, 
                                  return_vec, vec_size);
    } else if (state->library_type == ' ') {
        NDI_THROW(ierr, -16);   /* Library type was not set */
    } else {
        NDI_THROW(ierr, -4);   /* Bad input key */
    }

 catch:
    return (ierr);
}

/*********************************************************************/
/*********************************************************************/

static int GET_FLOAT_VEC_X_GENDIR(state_struct *state, int in_key,
                                  const char *in_str,
                                  FLOAT_TYPE *return_vec, int vec_size)
{
    int ierr = 0;
    double *vec = NULL;
    int i;
    int size = vec_size;

    if (!state && !state->ndi_dir) /* check state sanity */
        NDI_THROW(ierr, -2);

    switch(in_key) {            /* Process keyword */
    case NDI_TEMP:
    case NDI_SIG_0:
    case NDI_AT_WGT:
    case NDI_AWR:
        vec = (double *)ndi_malloc(vec_size*sizeof(double));
        if (!vec) NDI_THROW(ierr, -14); /* Out of memory */

        ierr = ndi_float_dir_block(state->ndi_dir, in_key, 
                                   ndi_get_section(state),
                                   state->library_vers, in_str, 
                                   vec, &size);

        for(i = 0; i < size; i++) return_vec[i] = (FLOAT_TYPE) vec[i];
        break;
    default:
        NDI_THROW(ierr, -4);   /* Unrecognized input key */
    }


 catch:
    ndi_free((void **) &vec);
    return ierr;
}

/**********************************************************************/
/*********************************************************************/

static int GET_FLOAT_VEC_X_MG(state_struct *state, int in_key, 
                              const char *in_str,
                              FLOAT_TYPE *return_vec, int vec_size)
{
    int ierr = 0;

    if (!state->iso_stat.set_flag)
        NDI_THROW(ierr, -7);   /* Isotope not set */

    /*************************/
    /*   Process input key   */
    /*************************/

    switch (in_key) {

    case NDI_TOT_PROD_XS:       /* Total production x.s. for target
                                   given by global var. 'zaid' and
                                   product given by in_str */
        ierr = GET_PROD_REAC(state, in_str, return_vec, vec_size);
        break;

    case NDI_RPROD_MLT:         /* Reaction multiplicity */
        ierr = GET_RPROD_MLT(state, in_str, return_vec, vec_size);
        break;

    default:     /**********  UNKNOWN KEYWORD  ***********/
        NDI_THROW(ierr, -4);
        break;
    }

 catch:
    return ierr;
}

/*********************************************************************/
/*********************************************************************/

int GET_PROD_REAC(state_struct *state, const char *rprod_in,
                  FLOAT_TYPE * return_vec, int vec_size)
{
    int i, j;
    int ierr = 0;
    int curr_index = -1;
    int num_grps = 0;
    int num_elements;
    int curr_mt;
    int sig_reac_err = 0;
    int old_reac_size = 3;
    char old_reac_id[3];
    FLOAT_TYPE *curr_xs = NULL;
    depl_target_struct *curr_targ;
    depl_rprod_struct *curr_prod;


    if (state) {
        curr_targ = state->depl_target;
    } else {
        return (-2);
    }

    /* CHECK ERRORS */

    if (!state->ndi_depl_set) {
        ierr = ndi_set_default_depl(state);
    }
    if (state->num_target <= 0) {
        return (-7);
    }

    ierr = ndi_set_curr_target(state, &curr_targ, &curr_index);
    if (curr_index == -1) {
        return (ierr);
    }

    curr_prod = curr_targ->rprod;
    curr_index = curr_targ->num_rprods;
    ierr = ndi_set_curr_rprod(state, &curr_prod, rprod_in, &curr_index);
    if (curr_index == -1) {
        return (ierr);
    }


    /* Determine no. of groups and create temporary x.s. array */
    if (state->collapse_flag) {
        num_grps = state->grp_num_new;
    } else {
        num_grps = state->iso_stat.num_grps;
    }
    if (num_grps <= 0) {
        /* Bad energy group structure */
        ierr = 5;
        return (ierr);
    }
    curr_xs = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
    if (!curr_xs) {
        /* Out of memory */
        return (-14);
    }


    /* For each reaction, read MT and mult */
    /*     find sig_reac for MT, multiply by mult. and  and to total */

    ierr = ndi_c_check_option_h(state, NDI_REAC_ID, old_reac_id, old_reac_size);
    ierr = ndi_c_set_option_h(state, NDI_REAC_ID, "mt");
    for (i = 0; i < curr_prod->num_reac; i++) {
        curr_mt = curr_prod->mt[i];
        /* Must change get_sig_reac to not convert to c values */
        num_elements = num_grps;
        sig_reac_err = GET_SIG_REAC(state, &state->iso_stat, curr_mt, curr_xs,
                                    &num_elements);
        if (sig_reac_err == 9) {
            ierr = 9;
        } else {
            for (j = 0; j < num_grps; j++) {
                if (j < vec_size) {
                    return_vec[j] =
                        return_vec[j] + curr_xs[j] * curr_prod->mult[i];
                } else {
                    /* Warning--array size not large enough */
                    if (ierr != 9) {    /* No data warning takes precedence 
                                           over too small array warning */
                        ierr = 7;
                    }
                }
            }
        }
    }

    ndi_c_set_option_h(state, NDI_REAC_ID, old_reac_id);
    if (curr_xs)
        ndi_free((void **) &curr_xs);
    return (ierr);

}

/*********************************************************************/
/*********************************************************************/

int GET_RPROD_MLT(state_struct *state, const char *rprod_in,
                  FLOAT_TYPE *return_vec, int vec_size)
{
    int i;
    int ierr = 0;
    int curr_index = -1;
    depl_target_struct *curr_targ;
    depl_rprod_struct *curr_prod;


    if (state) {
        curr_targ = state->depl_target;
    } else {
        return (-2);
    }

    /* CHECK ERRORS */

    if (!state->ndi_depl_set) {
        ierr = ndi_set_default_depl(state);
    }
    if (state->num_target <= 0) {
        return (-7);
    }

    ierr = ndi_set_curr_target(state, &curr_targ, &curr_index);
    if (curr_index == -1) {
        return (ierr);
    }

    curr_prod = curr_targ->rprod;
    curr_index = curr_targ->num_rprods;
    ierr = ndi_set_curr_rprod(state, &curr_prod, rprod_in, &curr_index);
    if (curr_index == -1) {
        return (ierr);
    }

    if (vec_size < curr_prod->num_reac) {
        return (7);
    }

    for (i = 0; i < curr_prod->num_reac; i++) {
        return_vec[i] = curr_prod->mult[i];
    }

    return (ierr);

}

/*********************************************************************/
