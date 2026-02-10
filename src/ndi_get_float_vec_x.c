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

static int GET_NESTED_FLOAT_VEC_X_GENDIR(state_struct *state, int in_key,
                                         const char *in_str, FILE *stream,
                                         double *vec, int vec_size, int *count);

static int GET_FLOAT_VEC_X_DEDX(state_struct *state, int in_key,
                                const char *in_str,
                                FLOAT_TYPE *return_vec, int vec_size);

static int GET_TARGET_DEDX(state_struct *state, const char *zaid,
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
    } else if (state->library_type == 'x') {
        ierr = GET_FLOAT_VEC_X_DEDX(state, in_key, in_str,
                                    return_vec, vec_size);
    } else if (state->library_type == ' ') {
        NDI_THROW(ierr, -16);   /* Library type was not set */
    } else {
        NDI_THROW(ierr, -4);   /* Bad input key */
    }

 catch:
    return ierr;
}

/*********************************************************************/
/*********************************************************************/

static int GET_FLOAT_VEC_X_GENDIR(state_struct *state, int in_key,
                                  const char *in_str,
                                  FLOAT_TYPE *return_vec, int vec_size)
{
    int ierr = 0;
    int done = 0;
    double *vec = NULL;
    int i;
    int size = vec_size;
    int count = 0;
    char *section[30];
    char in_line[MAX_IN_STR_LEN];
    int particle_type = state->particle_type;
    char library_type = state->library_type;
    int reaction_flag = state->ndi_depl_set;

    fseek(state->ndi_dir, 0, SEEK_SET);

    if (particle_type == 1 && library_type == 'm') {
        if (reaction_flag == 0) {
            strcpy(section, "multigroup_neutron");
        } else {
            strcpy(section, "neutron_depletion");
        }
    } else if ((particle_type == 1) && (library_type == 'd')) {
        if (reaction_flag == 0) {
            strcpy(section, "dosimetry_neutron");
        } else {
            strcpy(section, "radchem_chain");
        }
    } else if ((particle_type == 1001) && (library_type == 'd')) { /* Reqt 93 */
        if (reaction_flag == 0) {
            strcpy(section, "dosimetry_proton");
        } else {
            strcpy(section, "radchem_chain");
        }
    } else if ((particle_type == 0) && (library_type == 'm')) {
        if (reaction_flag == 0) {
            strcpy(section, "multigroup_photon");
        } else {
            strcpy(section, "photon_depletion");
        }
    } else if ((particle_type == -1) && (library_type == 's')) {
        strcpy(section, "tndata");
    } else if ((particle_type == -1) && (library_type == 'r')) {
        strcpy(section, "tn_reactions");
    } else if ((particle_type == -1) && (library_type == 'x')) {
        strcpy(section, "dedx");
    } else if (library_type == ' ') {
        /* Error: library type not set */
        NDI_THROW(ierr, -16);
    } else {
        NDI_THROW(ierr, -5);
    }

    while (done == 0) {
        if (fgets(in_line, sizeof(in_line), state->ndi_dir) != NULL) {
            if (strncmp(in_line, section, strlen(section)) == 0) {
                /* We found the right section! */
                done = -1;
            }
        } else {
            /* Error reading file */
            NDI_THROW(ierr, -11);
        }
    }

    if (!state && !state->ndi_dir) /* check state sanity */
        NDI_THROW(ierr, -2);

    vec = (double *) ndi_malloc(vec_size*sizeof(double));
    if (!vec) NDI_THROW(ierr, -14); /* Out of memory */

    ierr = GET_NESTED_FLOAT_VEC_X_GENDIR(state, in_key, in_str,
                                         state->ndi_dir, vec, vec_size, &count);

    for (i = 0; i < size; i++) {
        return_vec[i] = (FLOAT_TYPE) vec[i];
    }

 catch:
    ndi_free((void **) &vec);
    return ierr;
}

/*********************************************************************/
/*                                                                   */
/* Get vector of floats from the gendir file corresponding to in_str */
/* If we come across a nested gendir, open it and see if in_str is   */
/* present there.                                                     */
/*                                                                   */
/*********************************************************************/

static int GET_NESTED_FLOAT_VEC_X_GENDIR(state_struct *state, int in_key,
                                         const char *in_str, FILE *stream,
                                         double *vec, int vec_size, int *count)
{
    int ierr = 0;
    FILE *temp_stream;
    dir_entry rec;
    dir_entry *d = NULL;
    long int len_diff = 0;
    char *path = NULL;
    char zaid[MAX_IN_STR_LEN];
    char *token;
    char seps[] = "ngz";
    char seps_dot[] = ".";

    while ((d = ndi_fget_dir_block(&rec,stream)) != NULL && ierr >= 0) {
        strcpy(zaid,d->zaid);
        /* If a nested gendir is found, check if it has the appropriate library
           and open it. */
        if (strncmp(d->zaid,"gendir",strlen("gendir")) == 0 &&
            strncmp(d->lib,state->library_vers,strlen(state->library_vers)) == 0 &&
            strlen(d->lib) == strlen(state->library_vers)) {

            ierr = ndi_open_gendir(&temp_stream, d->path, &path);
            if (ierr < 0) continue;
            ierr = GET_NESTED_FLOAT_VEC_X_GENDIR(state, in_key, in_str, temp_stream,
                                                 vec, vec_size, count);
            if (fclose(temp_stream) != 0) ierr = -3;

        /* If the gendir entry's zaid matches in_str, grab the appropriate float
           for that gendir entry. This requires either a matching zaid with 
           at least part of a zaid extension, or a matching zaid with matching
           library. */
        } else {
            token = strtok(zaid,seps_dot);
            if (token == NULL) continue;
            len_diff = strlen(in_str) - strlen(token);
            if (len_diff > 0) {
                strcpy(zaid,d->zaid);
                token = strtok(zaid,seps);
            }
            if ((len_diff > 1 && strncmp(token, in_str, strlen(in_str)) == 0) || /* Most of the zaid matches */
                (strncmp(d->zaid, in_str, strlen(d->zaid)) == 0) || /* All of the zaid matches */
                ((strncmp(d->zaid, in_str, strlen(in_str)) == 0) && /* The ZAID matches to the ., and... */
                 ((strcmp("", state->library_vers) == 0) ||         /* there is no default library */
                  (strcmp(d->lib, state->library_vers) == 0)))) {   /* or the default library matches */

            switch(in_key) {            /* Process keyword */
                case NDI_TEMP:
                    vec[*count] = d->temp;
                    break;
                case NDI_SIG_0:
                    vec[*count] = d->sig_0;
                    break;
                case NDI_AT_WGT:
                    vec[*count] = d->at_wgt;
                     break;
                case NDI_AWR:
                     vec[*count] = d->awr;
                     break;
                default:
                     NDI_THROW(ierr, -4);
            }

            *count = *count + 1;
            }
        }
    }
  catch:
    return ierr;
}

/**********************************************************************/
/*********************************************************************/

static int GET_FLOAT_VEC_X_MG(state_struct *state, int in_key, 
                              const char *in_str,
                              FLOAT_TYPE *return_vec, int vec_size)
{
    int binary = state->binary;
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
        ierr = GET_PROD_REAC(state, in_str, return_vec, vec_size,
                             binary);
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

/**********************************************************************/
/*********************************************************************/

static int GET_FLOAT_VEC_X_DEDX(state_struct *state, int in_key,
                                const char *in_str,
                                FLOAT_TYPE *return_vec, int vec_size)
{
    int ierr = 0;

    if (!state->dedx_stat.set_flag)
        NDI_THROW(ierr, -7);   /* Isotope not set */

    /*************************/
    /*   Process input key   */
    /*************************/

    switch (in_key) {

    case NDI_TARGET_DEDX:       /* Get dE/dx for a target given
                                   by the ZAID in in_str */
        ierr = GET_TARGET_DEDX(state, in_str, return_vec, vec_size);
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
                  FLOAT_TYPE * return_vec, int vec_size, int binary)
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
        return -2;
    }

    /* CHECK ERRORS */

    if (!state->ndi_depl_set) {
        ierr = ndi_set_default_depl(state);
    }
    if (state->num_target <= 0) {
        return -7;
    }

    ierr = ndi_set_curr_target(state, &curr_targ, &curr_index);
    if (curr_index == -1) {
        return ierr;
    }

    curr_prod = curr_targ->rprod;
    curr_index = curr_targ->num_rprods;
    ierr = ndi_set_curr_rprod(state, &curr_prod, rprod_in, &curr_index);
    if (curr_index == -1) {
        return ierr;
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
        return ierr;
    }
    curr_xs = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
    if (!curr_xs) {
        /* Out of memory */
        return -14;
    }


    /* For each reaction, read MT and mult */
    /* find sig_reac for MT, multiply by mult. and  and to total */

    ierr = ndi_c_check_option_h(state, NDI_REAC_ID, old_reac_id, old_reac_size);
    ierr = ndi_c_set_option_h(state, NDI_REAC_ID, "mt");
    for (i = 0; i < curr_prod->num_reac; i++) {
        curr_mt = curr_prod->mt[i];
        /* Must change get_sig_reac to not convert to c values */
        num_elements = num_grps;
        sig_reac_err = GET_SIG_REAC(state, &state->iso_stat, curr_mt, curr_xs,
                                    &num_elements, binary);
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
    return ierr;

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
        return -2;
    }

    /* CHECK ERRORS */

    if (!state->ndi_depl_set) {
        ierr = ndi_set_default_depl(state);
    }
    if (state->num_target <= 0) {
        return -7;
    }

    ierr = ndi_set_curr_target(state, &curr_targ, &curr_index);
    if (curr_index == -1) {
        return ierr;
    }

    curr_prod = curr_targ->rprod;
    curr_index = curr_targ->num_rprods;
    ierr = ndi_set_curr_rprod(state, &curr_prod, rprod_in, &curr_index);
    if (curr_index == -1) {
        return ierr;
    }

    if (vec_size < curr_prod->num_reac) {
        return 7;
    }

    for (i = 0; i < curr_prod->num_reac; i++) {
        return_vec[i] = curr_prod->mult[i];
    }

    return ierr;

}

/*********************************************************************/
/*********************************************************************/

int GET_TARGET_DEDX(state_struct *state, const char *zaid_in,
                    FLOAT_TYPE *return_vec, int vec_size)
{
    int i;
    int ierr = 0;
    int curr_index = -1;
    int zaid = atoi(zaid_in);
    int binary;

    if (!state) {
        return (-2);
    }

    /* CHECK ERRORS */

    if (state->dedx_stat.num_targets <= 0) {
        return (-7);
    }

    binary = state->binary;

    for (i = 0; i < state->dedx_stat.num_targets; ++i) {
        if ( zaid == state->dedx_stat.targets[i] ) {
            curr_index = i;
            break;
        }
    }


    if ( curr_index < 0 ) {
        ierr = -9;
    } else { 
        ierr = NDI_READ_FLOAT_BLOCK(state->lib_stream,
             state->dedx_stat.target_dedx.stat[curr_index].file_ptr,
             vec_size, return_vec, vec_size, binary);
    }

    return (ierr);

}

/*********************************************************************/
