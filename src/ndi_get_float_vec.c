/*
 * $Id$
 * Description: Source for libndi.a
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
#include "ndi_bin_tree.h"
#include "ndi_sf.h"
#include "ndi_watt.h"

static int GET_FLOAT_VEC_GENDIR(state_struct *state, int in_key,
                                FLOAT_TYPE * return_vec, int return_size);
static int GET_FLOAT_VEC_MG(state_struct *state, int in_key,
                            FLOAT_TYPE * return_vec, int return_size);
static int GET_FLOAT_VEC_TN(state_struct *state, int in_key,
                            FLOAT_TYPE * return_vec, int return_size);
static int GET_CHI_MAT(state_struct *state, char chi_type, 
                       FLOAT_TYPE *return_mat, int return_size);
static int GET_SIG_XF_MAT(state_struct *state, char sig_xf_type, 
                          FLOAT_TYPE *return_mat, int return_size);
static int GET_E_BOUNDS_CURR_PART(state_struct *state,
                                  FLOAT_TYPE * return_vec,
                                  int return_size);
static int SIG_TOT_TRANS_CORR(state_struct *state,
                              FLOAT_TYPE * return_vec, int return_size);
static int NDI_CALC_E_AVG(state_struct *state, FLOAT_TYPE * return_vec,
                          int return_size);
static int NDI_CALC_V_AVG(state_struct *state, FLOAT_TYPE *return_vec,
                          int return_size);
static int NDI_CALC_E_N(state_struct *state, FLOAT_TYPE * return_vec,
                        int return_size);
static int NDI_CALC_E_AVAIL(state_struct *state, FLOAT_TYPE * return_vec,
                            int return_size);
static int NDI_GET_HEATING(state_struct *state, stat_flag * heating,
                           stat_flag * kerma, FLOAT_TYPE * return_vec,
                           int return_size);
static int NDI_GET_KERMA(state_struct *state, stat_flag * kerma,
                         stat_flag * heating, FLOAT_TYPE * return_vec,
                         int return_size);
static int NDI_GET_OLD_HEATING(state_struct *state,
                               FLOAT_TYPE * return_vec, int return_size);
static int NDI_GET_OLD_KERMA(state_struct *state,
                               FLOAT_TYPE * return_vec, int return_size);
static int NDI_GET_NEW_KERMA(state_struct *state,
                               FLOAT_TYPE * return_vec, int return_size);
static int NDI_GET_CHI_VEC(state_struct *state, char chi_type, 
                           FLOAT_TYPE *return_vec, int *return_size);
static int GET_DEPL_XS(state_struct *state, isotope_status * mg,
                       FLOAT_TYPE * return_vec, int return_size);
static int GET_EDIST_VEC(state_struct *state, FILE *lib_stream,
                         FLOAT_TYPE * return_vec, int return_size);
static int GET_EDIST_CONT(state_struct *state, char var,
                          FLOAT_TYPE *return_vec, int return_size);
static int CALC_CHI_WGT(state_struct *state, char nu_type,
                        FLOAT_TYPE * nu_avg);
static int NDI_GET_NUSIGF(state_struct *state, char nu_type,
                          FLOAT_TYPE * return_vec, int *return_size);
static int NDI_GET_NU(state_struct *state, isotope_status * mg,
                      char nu_type, FLOAT_TYPE * return_vec,
                      int *return_size);
static int COPY_SGNDF_E(int num_grps, FLOAT_TYPE * return_vec,
                        int return_size);
static int NEUTRON_E_AVG (state_struct *state, int num_grps, 
                          FLOAT_TYPE *return_vec, int return_size);
static int PHOTON_E_AVG (state_struct *state, int num_grps, 
                         FLOAT_TYPE *return_vec, int return_size);
static int MIDPT_ENERGY (state_struct *state, int num_grps, 
                         FLOAT_TYPE *return_vec, int return_size);
static int VELOCITY_CALC(state_struct *state, int num_grps, 
                         FLOAT_TYPE *return_vec, int return_size);
static int LEGACY_E_CALC(state_struct *state, int num_grps, 
                         FLOAT_TYPE *return_vec, int return_size);
static int FLAT_ENERGY (state_struct *state, int num_grps, 
                        FLOAT_TYPE *return_vec, int return_size);
static int TD_MOMENT(state_struct *state, char m, int num_grps, 
                     FLOAT_TYPE *return_vec, int return_size);

static int GET_SF_SPECTRA(state_struct *state, FLOAT_TYPE *return_vec, 
                          int return_size);

static int GET_QF(state_struct *state, FLOAT_TYPE *vec, int size);
static int GET_QSIGF(state_struct *state, FLOAT_TYPE *vec, int size);

static int is_partial_fission(int mt);

/**********************************************************************/

int NDI_GET_FLOAT_VEC_H(state_struct *state, int in_key,
                        FLOAT_TYPE * return_vec, int return_size)
{

    int ierr = 0;

    /* Initialize return vector with all zeroes */
    NDI_INIT_FLOAT_VEC(return_vec, return_size);

    /* Check that ndi_init has been called. */
    if ((!state) || (!state->ndi_initialized)) {
        return (-2);
    }
#ifdef FLOAT32
    if (sizeof(float) == 8) {
        ierr = -2;
        return (ierr);
    }
#endif

    if (is_gendir(state)) {
        ierr = GET_FLOAT_VEC_GENDIR(state, in_key, return_vec, return_size);
    } else if ((state->library_type == 'm') || 
               (state->library_type == 'd')) {
        ierr = GET_FLOAT_VEC_MG(state, in_key, return_vec, return_size);
    } else if (state->library_type == 's') {
        ierr = GET_FLOAT_VEC_TN(state, in_key, return_vec, return_size);
    } else if (state->library_type == 'p') {
        ierr = NDIMP_GET_FLOAT_VEC(state, in_key, return_vec, return_size);
    } else if (state->library_type == ' ') {
        /* Library type was not set */
        ierr = -16;
    } else {
        /* Incompatible library type  -- bad input key */
        ierr = -4;
    }

    return (ierr);
}


/**********************************************************************/
/**********************************************************************/
static int GET_FLOAT_VEC_GENDIR(state_struct *state, int in_key,
                                FLOAT_TYPE *return_vec, int return_size)
{
    int ierr = 0;
    struct ndi_bin_tree_node *root = NULL;
    FILE *ndi_dir;
    char *path = NULL;
    char *in_value;

    if (!state) {               /* check state sanity */
        return (-2);
    }

    in_value = ndi_get_section(state);
    if (in_value == 0) {
        return(-16);            /* Library type not set */
    }
        
    /* Open gendir */
    ierr = ndi_open_gendir(&ndi_dir, state->open_gendir_file, &path);
    if (path)
        ndi_free((void **) &path);

    if (ierr < 0) {
        root = NULL;
    } else {
        root = BUILD_FLOAT_TREE(ndi_dir, in_key, in_value);
        fclose(ndi_dir);
    }
    /* Collect the floats into the return vec */
    if (ierr >= 0) {
        ierr = COLLECT_FLOAT_VAL(root, return_vec, return_size);
    }
    ndi_close_tree(&root);
    return (ierr);
}
/**********************************************************************/
static int GET_FLOAT_VEC_MG(state_struct *state, int in_key,
                            FLOAT_TYPE *return_vec, int return_size)
{

    int ierr = 0;
    int num_elements;
    FLOAT_TYPE *curr_flt = return_vec;
    double *source_ptr = NULL;
    int n;

    if (!state) {
        return (-2);
    }


    /* Check that isotope is set */
    if (!state->iso_stat.set_flag) {
        return (-7);
    }

    /*************************/
    /*   Process input key   */
    /*************************/

    switch (in_key) {

    case NDI_E_BOUNDS:          /*  Energy group boundaries  */
        /*  CHECK DEPENDENCY ON EBOUND ORDER */
        /*  Check bounds */
        if (state->iso_stat.e_bounds.file_ptr) {
            if (state->collapse_flag) {
                for (n = 0; n <= state->grp_num_new; n++) {
                    if (n < return_size) {
                        return_vec[n] =
                            (FLOAT_TYPE) state->lib_ebounds[state->new_ebounds[n]];
                    } else {
                        ierr = 7;
                        break;
                    }
                }
            } else {
                num_elements = state->iso_stat.num_grps + 1;
                ierr =
                    NDI_READ_FLOAT_BLOCK(state->lib_stream,
                                         state->iso_stat.e_bounds.file_ptr,
                                         num_elements, return_vec,
                                         return_size);
            }
        } else {
            /* Error: data not available */
            ierr = -9;
        }
        break;

    case NDI_E_AVG:             /* Average group energies */
        /*  Check bounds */
        ierr = NDI_CALC_E_AVG(state, return_vec, return_size);
        break;

    case NDI_V_AVG:             /* Average group velocity (Reqt. 67) */
        ierr = NDI_CALC_V_AVG(state, return_vec, return_size);
        break;

    case NDI_ENERGY_N:          /* Energy given to neutrons */
        /*  Check bounds */
        ierr = NDI_CALC_E_N(state, return_vec, return_size);
        break;

    case NDI_ENERGY_AVAIL:      /* Energy available */
        /*  Check bounds */
        ierr = NDI_CALC_E_AVAIL(state, return_vec, return_size);
        break;

    case NDI_WGTS:              /* Weights */
        num_elements = state->iso_stat.num_grps;
        /*  Check bounds */
        ierr =
            GET_SIMPLE_FLOAT_VEC(state, state->lib_stream,
                                 state->iso_stat.wgts.file_ptr, 3,
                                 num_elements, return_vec, return_size);
        break;

    case NDI_VEL:               /* Velocities */
        num_elements = state->iso_stat.num_grps;
        /*  Check bounds */
        ierr =
            GET_SIMPLE_FLOAT_VEC(state, state->lib_stream,
                                 state->iso_stat.vel.file_ptr, 1,
                                 num_elements, return_vec, return_size);
        break;

    case NDI_HEATING:           /* Heating */
        if (state->kerma_calc == CALC) {
            ierr = NDI_GET_OLD_HEATING(state, return_vec, return_size);
        } else {
            ierr =
                NDI_GET_HEATING(state, &state->iso_stat.heat,
                                &state->iso_stat.kerma, return_vec,
                                return_size);
        }
        break;

    case NDI_KERMA:             /* kerma */
        /* Units are MeV*barn -- this is just heating * tot_sigma */
        if (state->kerma_calc == CALC) {
            ierr = NDI_GET_OLD_KERMA(state, return_vec, return_size);
        } else if(state->kerma_calc == LEGACY) {
            ierr = NDI_GET_NEW_KERMA(state, return_vec, return_size);
        } else {
            ierr =
                NDI_GET_KERMA(state, &state->iso_stat.kerma,
                              &state->iso_stat.heat, return_vec,
                              return_size);
        }
        break;

    case NDI_HEATING_CURR_PART: /* Partial heating for current particle */
        if (state->curr_part_index >= 0) {
            ierr =
                NDI_GET_HEATING(state,
                                &state->iso_stat.sec_part[state->
                                                          curr_part_index].
                                heating,
                                &state->iso_stat.sec_part[state->
                                                          curr_part_index].
                                kerma, return_vec, return_size);
        } else {
            /* Error -- bad particle? */
            ierr = -999;
        }
        break;

    case NDI_KERMA_CURR_PART:   /* Partial kerma for current particle */
        /* Units are MeV*barn -- this is just heating_curr_particle * tot_sigma */
        if (state->curr_part_index >= 0) {
            ierr =
                NDI_GET_KERMA(state,
                              &state->iso_stat.sec_part[state->
                                                        curr_part_index].
                              kerma,
                              &state->iso_stat.sec_part[state->
                                                        curr_part_index].
                              heating, return_vec, return_size);
        } else {
            /* Error -- bad particle? */
            ierr = -999;
        }
        break;

    case NDI_SIG_TOT:           /* Sigma total */
        num_elements = state->iso_stat.num_grps;
        /*  Check bounds */
        ierr =
            GET_SIMPLE_FLOAT_VEC(state, state->lib_stream,
                                 state->iso_stat.sig_tot.file_ptr, 0,
                                 num_elements, return_vec, return_size);

/*       printf("SIG_TOT RETURNED\n"); */
        if ((ierr >= 0) && (state->trans_corr != ' ')) {
            ierr = SIG_TOT_TRANS_CORR(state, return_vec, return_size);
        }
        break;

    case NDI_NU_TOT:            /* nubar total */
        num_elements = return_size;
        ierr =
            NDI_GET_NU(state, &state->iso_stat, 't', return_vec,
                       &num_elements);
        (void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
        break;

    case NDI_NU_PR:             /* nubar prompt */
        num_elements = return_size;
        ierr =
            NDI_GET_NU(state, &state->iso_stat, 'p', return_vec,
                       &num_elements);
        (void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
        break;

    case NDI_NU_DEL:            /* nubar delayed */
        num_elements = return_size;
        ierr =
            NDI_GET_NU(state, &state->iso_stat, 'd', return_vec,
                       &num_elements);
        (void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
        break;

    case NDI_NU_SIG_F_TOT:      /* nubar total sigma fission */
        num_elements = return_size;
        ierr = NDI_GET_NUSIGF(state, 't', return_vec, &num_elements);
        break;

    case NDI_NU_SIG_F_PR:       /* nubar prompt sigma fission */
        num_elements = return_size;
        ierr = NDI_GET_NUSIGF(state, 'p', return_vec, &num_elements);
        break;

    case NDI_NU_SIG_F_DEL:      /* nubar delayed sigma fission */
        num_elements = return_size;
        ierr = NDI_GET_NUSIGF(state, 'd', return_vec, &num_elements);
        break;

    case NDI_CHI_VEC_TOT:       /* chi total vector */
        num_elements = return_size;
        /*  Check bounds */
        ierr = NDI_GET_CHI_VEC(state, 't', return_vec, &num_elements);
        break;

    case NDI_CHI_VEC_PR:        /* chi prompt vector */
        num_elements = return_size;
        /*  Check bounds */
        ierr = NDI_GET_CHI_VEC(state, 'p', return_vec, &num_elements);
        break;

    case NDI_CHI_VEC_DEL:       /* chi delayed vector */
        num_elements = return_size;
        /*  Check bounds */
        ierr = NDI_GET_CHI_VEC(state, 'd', return_vec, &num_elements);
        break;

    case NDI_CHI_MAT_TOT:       /* chi total matrix */
        num_elements = return_size;
        /*  Check bounds */
        ierr = GET_CHI_MAT(state, 't', return_vec, num_elements);
        if (ierr == -9) {
            (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);
            ierr = 9;
        }
        break;

    case NDI_CHI_MAT_PR:        /* chi prompt matrix */
        num_elements = return_size;
        /*  Check bounds */
        ierr = GET_CHI_MAT(state, 'p', return_vec, num_elements);
        if (ierr == -9) {
            (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);
            ierr = 9;
        }
        break;

    case NDI_CHI_MAT_DEL:       /* chi delayed matrix */
        num_elements = return_size;
        /*  Check bounds */
        ierr = GET_CHI_MAT(state, 'd', return_vec, num_elements);
        if (ierr == -9) {
            (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);
            ierr = 9;
        }
        break;

    case NDI_FISS_XFER_MAT_TOT:
        ierr = GET_SIG_XF_MAT(state, 't', return_vec, return_size);
        break;

    case NDI_FISS_XFER_MAT_PR:
        ierr = GET_SIG_XF_MAT(state, 'p', return_vec, return_size);
        break;

    case NDI_FISS_XFER_MAT_DEL:
        ierr = GET_SIG_XF_MAT(state, 'd', return_vec, return_size);
        break;

    case NDI_NORM:              /* Normalization factors */
        num_elements = NUM_NORM;
        /*  Check bounds */
        if (state->iso_stat.norm.file_ptr) {
            ierr =
                NDI_READ_FLOAT_BLOCK(state->lib_stream,
                                     state->iso_stat.norm.file_ptr,
                                     NUM_NORM, return_vec, return_size);
        } else {
            /* Error: data not available -- make all values 1.0 */
            for (n = 0; n < 4; n++) {
                if (n < return_size) {
                    return_vec[n] = 1.0;
                }
            }
            ierr = 9;
        }
        break;

    case NDI_FISS_Q:            /* fission q */
        num_elements = NUM_FISSION_Q;
        /*  Check bounds */
        if (state->iso_stat.fiss_q.file_ptr) {
            ierr =
                NDI_READ_FLOAT_BLOCK(state->lib_stream,
                                     state->iso_stat.fiss_q.file_ptr,
                                     NUM_FISSION_Q, return_vec,
                                     return_size);
        } else {
            /* Error: data not available */
            ierr = -9;
        }
        (void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
        break;

    case NDI_REAC_Q:            /* edit reaction q */
        /*  Check bounds */
        if (state->iso_stat.reac_q) {
            source_ptr = state->iso_stat.reac_q;
            for (n = 0; n < state->iso_stat.num_reac; n++) {
                *curr_flt++ = (FLOAT_TYPE) * source_ptr++;
            }
        } else {
            /* Error: data not available */
            ierr = -9;
        }
        break;

    case NDI_E_BOUNDS_CURR_PART:        /* sec. particle energy boundaries */
        /*  Check bounds */
        ierr = GET_E_BOUNDS_CURR_PART(state, return_vec, return_size);
        break;

    case NDI_TOT_DEPL_XS:       /* Total depletion cross section */
        if (!state->ndi_depl_set) {
            ierr = ndi_set_default_depl(state);
        }

        ierr =
            GET_DEPL_XS(state, &state->iso_stat, return_vec, return_size);
        break;

    case NDI_TEMPS:             /* Multi-temperature (reqt. 22) */
    {
        double *vec = NULL;
        char sza[MAX_TN_REAC_LEN];
        int i;
        int size = return_size;
        if (!state->ndi_dir) {
            ierr = -2;
            return (ierr);
        }

        vec = (double *)ndi_malloc(return_size*sizeof(double));

        if (!vec) {
            ierr = -14;  /* Out of memory */
        } else {
            sprintf(sza, "%d.", atoi(state->iso_stat.zaid));
            ierr = ndi_float_dir_block(state->ndi_dir, NDI_TEMP, 
                                       ndi_get_section(state), 
                                       state->library_vers, sza, 
                                       vec, &size);

            /* insertion sort with type conversion (Dahl 02/02/09)*/
            for (i = 0; i < size; i++) {
                FLOAT_TYPE x;
                int j;

                x = (FLOAT_TYPE) vec[i];
                j = i;
                while ((j > 0) && (return_vec[j-1] > x)) {
                    return_vec[j] = return_vec[j-1]; /* shift up */
                    j = j - 1;
                }
                return_vec[j] = x; /* insert */
            }
        }
        ndi_free((void **) &vec);
    }
    break;

    case NDI_SF_SPECTRA:        /* Spontaneous Fission Spectra (reqt. 46) */
        ierr = GET_SF_SPECTRA(state, return_vec, return_size);
        break;

    case NDI_Q_SIG_F_PR:        /* Q*sigma_f for prompt neutrons (reqt 72) */
        ierr = GET_QSIGF(state, return_vec, return_size);
        break;

    /**********  UNKNOWN KEYWORD  ***********/

    default:
        ierr = -4;
        break;
    }

    return (ierr);
}


/**********************************************************************/
/**********************************************************************/
static int GET_FLOAT_VEC_TN(state_struct *state, int in_key,
                            FLOAT_TYPE * return_vec, int return_size)
{

    int ierr = 0;
    int num_elements;
    int i;
    FLOAT_TYPE *curr_p = return_vec;

    if (!state) {
        return (-2);
    }

    /* Check that isotope is set */
    if (!state->tn_stat.set_flag) {
        return (-7);
    }

    /*************************/
    /*   Process input key   */
    /*************************/

    switch (in_key) {

    case NDI_TEMPS:             /* Temperatures */
        num_elements = state->tn_stat.num_temps;
        ierr =
            GET_SIMPLE_FLOAT_VEC(state, state->lib_stream,
                                 state->tn_stat.temps.file_ptr, -1,
                                 num_elements, return_vec, return_size);
        break;

    case NDI_SIGVBARS:          /* Sigma Vbars */
        num_elements = state->tn_stat.num_temps;
        ierr =
            GET_SIMPLE_FLOAT_VEC(state, state->lib_stream,
                                 state->tn_stat.sigvbars.file_ptr, -1,
                                 num_elements, return_vec, return_size);
        break;

    case NDI_EINBAR:            /* Thermal-average of total inc. energy */
        num_elements = state->tn_stat.num_temps;
        ierr =
            GET_SIMPLE_FLOAT_VEC(state, state->lib_stream,
                                 state->tn_stat.einbars.file_ptr, -1,
                                 num_elements, return_vec, return_size);
        break;

    case NDI_EDIST:             /* Curr. particle's energy distribution at current temp. */
        ierr =
            GET_EDIST_VEC(state, state->lib_stream, return_vec,
                          return_size);
        break;

    case NDI_EDIST_TEMPS:
        /* Vector of interpolation type for current particle's interpolation 
           temperatures */
        if (state->grp_num_new <= 0) return (-21);
        if ((state->curr_part_index >= 0) ||
            (state->curr_part_index < state->sec_part_filled)) {
            sec_part_distr *part;
              
            part = &(state->tn_stat.sec_part[state->curr_part_index]);
            if (part) {
                int i;

                for (i = 0; i < part->temp_interp.npts; i++)
                    return_vec[i] = part->edist[i].temp;
            } else {
                return (-30);
            }
        } else {
            return (-30);
        }

        break;

    case NDI_EDIST_CONT_E:
        ierr = GET_EDIST_CONT(state, 'E', return_vec, return_size);
        break;

    case NDI_EDIST_CONT_P:
        ierr = GET_EDIST_CONT(state, 'P', return_vec, return_size);
        break;

    case NDI_E_BOUNDS:          /*  Multigroup energy boundaries for sec. part distr. */

        for (i = 0; i < (state->grp_num_new + 1); i++) {
            if (i >= return_size) {
                ierr = 7;
                break;
            } else {
                *curr_p = (FLOAT_TYPE) state->collapse_ebounds[i];
                curr_p++;
            }
        }
        break;

    case NDI_E_AVG:
        /* For TN this is just the mean of the two energy boundaries -- no flux to weight with */
        for (i = 0; i < state->grp_num_new; i++) {
            if (i < return_size) {
                return_vec[i] =
                    (FLOAT_TYPE) ((state->collapse_ebounds[i] +
                                   state->collapse_ebounds[i + 1]) / 2.0);
            } else {
                ierr = 7;
                break;
            }
        }
        break;

        /**********  UNKNOWN KEYWORD  ***********/

    default:
        ierr = -4;
        break;
    }

    return (ierr);
}

/*********************************************************************/
/*********************************************************************/

static int GET_CHI_MAT(state_struct *state, char chi_type, 
                       FLOAT_TYPE *return_mat, int return_size)
{
    int ierr = 0;
    int jerr = 0;

    int num_full_mat = 0;
    int row;
    int coll_num_grps = 0;

    long mat_ptr;
    long vec_ptr;
    long int chi_flg1;
    long int chi_ptr1;
    long int chi_flg2;
    long int chi_ptr2;
    char chi_type1;
    char chi_type2;
    FLOAT_TYPE s;

    int n;
    int in, out;
    char mat_order;
    int collapse_flag;
    int no_norm;
    FLOAT_TYPE norm;

    int lib_num_grps;
    FLOAT_TYPE *row_ptr;

    FLOAT_TYPE *chi_mat = NULL;
    FLOAT_TYPE *chi_vec = NULL;

    FLOAT_TYPE *nusigf = NULL;
    FLOAT_TYPE *nusigf1 = NULL;
    FLOAT_TYPE *chi_mat1 = NULL;
    FLOAT_TYPE *nusigf2 = NULL;
    FLOAT_TYPE *chi_mat2 = NULL;

    (void) NDI_INIT_FLOAT_VEC(return_mat, return_size);

    if (!state) NDI_THROW(ierr, -2);    /* State not initialized */

    switch(chi_type) {

    case 't':                  /* total neutrons */
        mat_ptr = state->iso_stat.chi_mat_tot.file_ptr;

        chi_type1 = 'p';
        chi_ptr1 = state->iso_stat.chi_mat_pr.file_ptr;
        s = +1.0;
        chi_type2 = 'd';
        chi_ptr2 = state->iso_stat.chi_mat_del.file_ptr;

        vec_ptr = state->iso_stat.chi_vec_tot.file_ptr;

        chi_flg1 = (state->iso_stat.chi_mat_pr.file_ptr ||
                    state->iso_stat.chi_vec_pr.file_ptr);
        chi_flg2 = (state->iso_stat.chi_mat_del.file_ptr ||
                    state->iso_stat.chi_vec_del.file_ptr);
        break;

    case 'p':                  /* prompt neutrons */
        mat_ptr = state->iso_stat.chi_mat_pr.file_ptr;

        chi_type1 = 't';
        chi_ptr1 = state->iso_stat.chi_mat_tot.file_ptr;
        s = -1.0;
        chi_type2 = 'd';
        chi_ptr2 = state->iso_stat.chi_mat_del.file_ptr;

        vec_ptr = state->iso_stat.chi_vec_pr.file_ptr;

        chi_flg1 = (state->iso_stat.chi_mat_tot.file_ptr ||
                    state->iso_stat.chi_vec_tot.file_ptr);
        chi_flg2 = (state->iso_stat.chi_mat_del.file_ptr ||
                    state->iso_stat.chi_vec_del.file_ptr);
        break;

    case 'd':                  /* delayed neutrons */
        mat_ptr = state->iso_stat.chi_mat_del.file_ptr;

        chi_type1 = 't';
        chi_ptr1 = state->iso_stat.chi_mat_tot.file_ptr;
        s = -1.0;
        chi_type2 = 'p';
        chi_ptr2 = state->iso_stat.chi_mat_pr.file_ptr;

        vec_ptr = state->iso_stat.chi_vec_del.file_ptr;

        chi_flg1 = (state->iso_stat.chi_mat_tot.file_ptr ||
                    state->iso_stat.chi_vec_tot.file_ptr);
        chi_flg2 = (state->iso_stat.chi_mat_pr.file_ptr ||
                    state->iso_stat.chi_vec_pr.file_ptr);
        break;

    default:
        NDI_THROW(ierr, -13);            /* Invalid input parameter */
    }

    if (state->iso_stat.fissionable == 0) goto catch; /* Not fissionable */

    lib_num_grps = state->iso_stat.num_grps;
    if (lib_num_grps <= 0) NDI_THROW(ierr, -9); /* Data not available */

    if (state->collapse_flag && !ndi_nusigf_avail(state, chi_type))
        NDI_THROW(ierr, -9); /* Data not available */

    num_full_mat = lib_num_grps * lib_num_grps;
    chi_mat = (FLOAT_TYPE *) ndi_malloc(num_full_mat*sizeof(FLOAT_TYPE));
    if (!chi_mat) NDI_THROW(ierr, -14);    /* Out of memory */
     
    if (mat_ptr) {
        /* (1) Read chi matrix from library  */

        jerr = NDI_READ_FLOAT_BLOCK(state->lib_stream, mat_ptr, num_full_mat,
                                    chi_mat, num_full_mat);
        if (jerr) NDI_THROW(ierr, jerr);

    } else if (chi_ptr1 && ndi_nusigf_avail(state, chi_type1) &&
               chi_ptr2 && ndi_nusigf_avail(state, chi_type2)) {
        /* (2) Calculate requested chi matrix from other two matricies */

        /* Get chi_mat1 */
        n = lib_num_grps * lib_num_grps;
        chi_mat1 = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!chi_mat1) NDI_THROW(ierr, -14); /* Out of memory */

        jerr = NDI_READ_FLOAT_BLOCK(state->lib_stream, chi_ptr1, 
                                    n, chi_mat1, n);
        if (jerr) NDI_THROW(ierr, jerr);

        /* Get nu_sig_f1 */
        n = lib_num_grps;
        nusigf1 = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!nusigf1) NDI_THROW(ierr, -14); /* Out of memory */

        collapse_flag = state->collapse_flag; /* remember user collapse */
        state->collapse_flag = 0; /* turn off collapse */
        jerr = NDI_GET_NUSIGF(state, chi_type1, nusigf1, &n);
        state->collapse_flag = collapse_flag; /* restore user collapse */
        if (jerr) NDI_THROW(ierr, jerr);

        /* Get chi_mat2 */
        n = lib_num_grps * lib_num_grps;
        chi_mat2 = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!chi_mat2) NDI_THROW(ierr, -14); /* Out of memory */

        jerr = NDI_READ_FLOAT_BLOCK(state->lib_stream, chi_ptr2, 
                                    n, chi_mat2, n);
        if (jerr) NDI_THROW(ierr, jerr);

        /* Get nusigf2 */
        n = lib_num_grps;
        nusigf2 = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!nusigf2) NDI_THROW(ierr, -14); /* Out of memory */

        collapse_flag = state->collapse_flag; /* remember user collapse */
        state->collapse_flag = 0; /* turn off collapse */
        jerr = NDI_GET_NUSIGF(state, chi_type2, nusigf2, &n);
        state->collapse_flag = collapse_flag; /* restore user collapse */
        if (jerr) NDI_THROW(ierr, jerr);

        n = lib_num_grps;
        /* Calculate fission transfer matrix */
        for (in = 0; in < n; in++) {
            norm = 0.0;
            for (out = 0; out < n; out++) {
                chi_mat[in*n+out] = (nusigf1[in] * chi_mat1[in*n+out]) + 
                    s * (nusigf2[in] * chi_mat2[in*n+out]);
                if (chi_mat[in*n+out] < 0.0) chi_mat[in*n+out] = 0.0;
                norm += chi_mat[in*n+out];
            }
            /* Divide fission transfer mat by nu_sigf to get chi matrix */
            if (norm != 0.0) {
                for (out = 0; out < n; out++) {
                    chi_mat[in*n+out] /= norm;
                }
            }
        }

    } else if (vec_ptr) { 
        /* Vector in library */

        chi_vec = (FLOAT_TYPE *) ndi_malloc(lib_num_grps * 
                                            sizeof(FLOAT_TYPE));
        if (!chi_vec) NDI_THROW(ierr, -14); /* Out of memory */
     
        jerr = NDI_READ_FLOAT_BLOCK(state->lib_stream, vec_ptr, lib_num_grps,
                                    chi_vec, lib_num_grps);
        if (jerr) NDI_THROW(ierr, jerr);

        /* Construct Matrix */
        jerr = NDI_VEC2MAT(chi_vec, lib_num_grps, chi_mat,
                           num_full_mat, num_full_mat);
        if (jerr) NDI_THROW(ierr, jerr);
        ierr = 10;            /* Matrix has been constructed */

    } else if (chi_flg1 && ndi_nusigf_avail(state, chi_type1) &&
               chi_flg2 && ndi_nusigf_avail(state, chi_type2)) {
        /* (2) Calculate chi matrix from other two */

        /* Get chi_mat1 */
        n = lib_num_grps * lib_num_grps;
        chi_mat1 = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!chi_mat1) NDI_THROW(ierr, -14); /* Out of memory */

        collapse_flag = state->collapse_flag; /* remember user collapse */
        state->collapse_flag = 0; /* turn off collapse */
        mat_order = state->mat_order; /* remember user matrix order */
        state->mat_order = 'd'; /* get matrix in "C" order */
        no_norm = state->no_norm; /* remember user norm */
        state->no_norm = 1;   /* turn normalization off */
        jerr = GET_CHI_MAT(state, chi_type1, chi_mat1, n);
        state->no_norm = no_norm; /* restore user normalization */
        state->mat_order = mat_order; /* restore user matrix order */
        state->collapse_flag = collapse_flag; /* restore user collapse */
        if (jerr) NDI_THROW(ierr, jerr);
        if (jerr == 10) ierr = jerr; /* Calculated matrix */

        /* Get nu_sig_f1 */
        n = lib_num_grps;
        nusigf1 = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!nusigf1) NDI_THROW(ierr, -14); /* Out of memory */

        collapse_flag = state->collapse_flag; /* remember user collapse */
        state->collapse_flag = 0; /* turn off collapse */
        jerr = NDI_GET_NUSIGF(state, chi_type1, nusigf1, &n);
        state->collapse_flag = collapse_flag; /* restore user collapse */
        if (jerr) NDI_THROW(ierr, jerr);

        /* Get chi_mat2 */
        n = lib_num_grps * lib_num_grps;
        chi_mat2 = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!chi_mat2) NDI_THROW(ierr, -14); /* Out of memory */

        collapse_flag = state->collapse_flag; /* remember user collapse */
        state->collapse_flag = 0; /* turn off collapse */
        mat_order = state->mat_order; /* remember user matrix order */
        state->mat_order = 'd'; /* get matrix in "C" order */
        no_norm = state->no_norm; /* remember user normalization */
        state->no_norm = 1;   /* turn normalization off */
        jerr = GET_CHI_MAT(state, chi_type2, chi_mat2, n);
        state->no_norm = no_norm; /* restore user normalization */
        state->mat_order = mat_order; /* restor user matrix order */
        state->collapse_flag = collapse_flag; /* restore user collapse */
        if (jerr) NDI_THROW(ierr, jerr);
        if (jerr == 10) ierr = jerr; /* Calculated matrix */

        /* Get nusigf2 */
        n = lib_num_grps;
        nusigf2 = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!nusigf2) NDI_THROW(ierr, -14); /* Out of memory */

        collapse_flag = state->collapse_flag; /* remember user collapse */
        state->collapse_flag = 0; /* turn off collapse */
        jerr = NDI_GET_NUSIGF(state, chi_type2, nusigf2, &n);
        state->collapse_flag = collapse_flag; /* restore user collapse */
        if (jerr) NDI_THROW(ierr, jerr);

        n = lib_num_grps;
        /* Calculate fission transfer matrix */
        for (in = 0; in < n; in++) {
            norm = 0.0;
            for (out = 0; out < n; out++) {
                chi_mat[in*n+out] = (nusigf1[in] * chi_mat1[in*n+out]) + 
                    s * (nusigf2[in] * chi_mat2[in*n+out]);
                if (chi_mat[in*n+out] < 0.0) chi_mat[in*n+out] = 0.0;
                norm += chi_mat[in*n+out];
            }
            /* Divide fission transfer mat by nu_sigf to get chi matrix */
            if (norm != 0.0) {
                for (out = 0; out < n; out++) {
                    chi_mat[in*n+out] /= norm;
                }
            }
        }

    } else {
        NDI_THROW(ierr, -9);                /* Data not available */
    }

    if (state->collapse_flag) {/* Collapsing required */

        /* 1) Create fission transfer matrix from chi matrix */

        /* Allocate library group sized nu_sigf */
        n = lib_num_grps;
        nusigf = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!nusigf) NDI_THROW(ierr, -14); /* Out of memory */

        /* Turn off collapsing and get full (nu sigma_f) */
        collapse_flag = state->collapse_flag;
        state->collapse_flag = 0;
        jerr = NDI_GET_NUSIGF(state, chi_type, nusigf, &n);
        state->collapse_flag = collapse_flag;
        if (jerr) NDI_THROW(ierr, jerr);

        /* mutiply (nu sigma_f) time chi for fission transfer matrix */
        for (in = 0; in < n; in++) {
            for (out = 0; out < n; out++) {
                chi_mat[in * n + out] *= nusigf[in];
            }
        }

        /* 2) Collapse fission transfer matrix */
        jerr = NDI_COLLAPSE_MAT(chi_mat, lib_num_grps, lib_num_grps,
                                state->grp_num_new, state->grp_num_new,
                                state->new_ebounds, state->new_ebounds,
                                state->lib_weight, return_mat,
                                return_size);
        if (jerr) NDI_THROW(ierr, jerr);

        /* 3) Create chi matrix from fission transfer matrix */
        coll_num_grps = state->grp_num_new;

        /* Allocate collapse group sized nu_sigf */
        n = coll_num_grps;
        ndi_free((void **) &nusigf);
        nusigf = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!nusigf) NDI_THROW(ierr, -14); /* Out of memory */

        /* Get collapse (nu sigma_f) */
        jerr = NDI_GET_NUSIGF(state, chi_type, nusigf, &n);
        if (jerr) NDI_THROW(ierr, jerr);

        /* Divide fission transfer mat by nu_sigmaf to get chi mat */
        for (in = 0; in < n; in++) {
            for (out = 0; out < n; out++) {
                if (nusigf[in] == 0.0)
                    return_mat[in*n + out] = 0.0;
                else
                    return_mat[in*n + out] /= nusigf[in];
            }
        }
    } else {                   /* Copy required */
        coll_num_grps = lib_num_grps;
        if (num_full_mat > return_size) {
            ierr = 7;        /*  Data truncated*/
            n = return_size;
        } else {
            n = num_full_mat;
        }

        for (in = 0; in < n; in++) {
            return_mat[in] = chi_mat[in];
        }
    }

    /* Normalize rows */
    if (!state->no_norm) {
        row_ptr = return_mat;
        for (row = 0; row < coll_num_grps; row++) {
            if (((row + 1) * coll_num_grps) > return_size) {
                break;
            } else {
                jerr = NORM_FLOAT_VEC(row_ptr, coll_num_grps, 1.0);
                if (jerr) NDI_THROW(ierr, jerr);
                row_ptr += coll_num_grps;
            }
        }
    }

    /*   Transpose matrix */
    if ((state->mat_order == 't') && (ierr >= 0)) {
        jerr = NDI_TRANSPOSE_FLOAT(return_mat, coll_num_grps, coll_num_grps);
        if (jerr) NDI_THROW(ierr, jerr);
    }

 catch:
    ndi_free((void **) &chi_mat2);
    ndi_free((void **) &nusigf2);
    ndi_free((void **) &chi_mat1);
    ndi_free((void **) &nusigf1);
    ndi_free((void **) &nusigf);
    ndi_free((void **) &chi_vec);
    ndi_free((void **) &chi_mat);

    return (ierr);
}

/*********************************************************************/

static int GET_SIG_XF_MAT(state_struct *state, char sig_xf_type, 
                       FLOAT_TYPE *return_mat, int return_size)
{
    int ierr = 0;

    int collapse_flag = 0;
    char mat_order = 'd';
    int no_norm = 1;

    long int mat_ptr;
    char sig_xf_type1;
    FLOAT_TYPE s;
    char sig_xf_type2;
    int sig_xf_sum;
    long int vec_ptr;
    int sig_xf_mix;

    int lib_num_grps;
    int num_full_mat = 0;
    FLOAT_TYPE *tmp_mat = NULL;

    enum calc {MAT, SUM, VEC, MIX};
    enum calc calc_type;

    int jerr = 0;

    FLOAT_TYPE *sig_xf1 = NULL;
    FLOAT_TYPE *sig_xf2 = NULL;
    FLOAT_TYPE *chi_vec = NULL;
    FLOAT_TYPE *nusigf = NULL;
    int coll_num_grps = 0;

    int n;
    int in, out;

    (void) NDI_INIT_FLOAT_VEC(return_mat, return_size);

    if (!state) NDI_THROW(ierr, -2);    /* State not initialized */

    /* Remember user settings: collapse, order, normalization */
    collapse_flag = state->collapse_flag;
    mat_order = state->mat_order;
    no_norm = state->no_norm;

    /* Setup possible calculation parameters */
    switch(sig_xf_type) {

    case 't':                  /* total neutrons */
        mat_ptr = state->iso_stat.chi_mat_tot.file_ptr;

        sig_xf_type1 = 'p';
        s = +1.0;
        sig_xf_type2 = 'd';
        sig_xf_sum = (state->iso_stat.chi_mat_pr.file_ptr &&  
                      state->iso_stat.chi_mat_del.file_ptr);

        vec_ptr = state->iso_stat.chi_vec_tot.file_ptr;

        sig_xf_mix = ((state->iso_stat.chi_mat_pr.file_ptr ||
                       state->iso_stat.chi_vec_pr.file_ptr) &&
                      (state->iso_stat.chi_mat_del.file_ptr ||
                       state->iso_stat.chi_vec_del.file_ptr));
        break;

    case 'p':                  /* prompt neutrons */
        mat_ptr = state->iso_stat.chi_mat_pr.file_ptr;

        sig_xf_type1 = 't';
        s = -1.0;
        sig_xf_type2 = 'd';
        sig_xf_sum = (state->iso_stat.chi_mat_tot.file_ptr &&
                      state->iso_stat.chi_mat_del.file_ptr);

        vec_ptr = state->iso_stat.chi_vec_pr.file_ptr;

        sig_xf_mix = ((state->iso_stat.chi_mat_tot.file_ptr ||
                      state->iso_stat.chi_vec_tot.file_ptr) &&
                      (state->iso_stat.chi_mat_del.file_ptr ||
                       state->iso_stat.chi_vec_del.file_ptr));
        break;

    case 'd':                  /* delayed neutrons */
        mat_ptr = state->iso_stat.chi_mat_del.file_ptr;

        sig_xf_type1 = 't';
        s = -1.0;
        sig_xf_type2 = 'p';
        sig_xf_sum = (state->iso_stat.chi_mat_tot.file_ptr &&
                      state->iso_stat.chi_mat_pr.file_ptr);

        vec_ptr = state->iso_stat.chi_vec_del.file_ptr;

        sig_xf_mix = ((state->iso_stat.chi_mat_tot.file_ptr ||
                       state->iso_stat.chi_vec_tot.file_ptr) &&
                      (state->iso_stat.chi_mat_pr.file_ptr ||
                       state->iso_stat.chi_vec_pr.file_ptr));
        break;

    default:
        NDI_THROW(ierr, -13);            /* Invalid input parameter */
    }

    if (state->iso_stat.fissionable == 0) goto catch; /* Not fissionable */

    lib_num_grps = state->iso_stat.num_grps;
    if (lib_num_grps <= 0)
        NDI_THROW(ierr, -9); /* Data not available */

    /* Turn off collapse; select "C" matrix order; no normalization */
    state->collapse_flag = 0;
    state->mat_order = 'd';
    state->no_norm = 1;

    /* Allocate matrix */
    num_full_mat = lib_num_grps * lib_num_grps;
    tmp_mat = (FLOAT_TYPE *) ndi_malloc(num_full_mat*sizeof(FLOAT_TYPE));
    if (!tmp_mat) NDI_THROW(ierr, -14);    /* Out of memory */
    
 
    /* Decide which calculation to perform */

    if (mat_ptr && ndi_nusigf_avail(state, sig_xf_type)) {
        calc_type = MAT;
        ierr = 10;              /* Calculated value */
    } else if (sig_xf_sum && ndi_nusigf_avail(state, sig_xf_type1) &&
               ndi_nusigf_avail(state, sig_xf_type2)) {
        calc_type = SUM;
    } else if (vec_ptr && ndi_nusigf_avail(state, sig_xf_type)) {
        calc_type = VEC;
        ierr = 10;              /* Calculated value */
    } else if (sig_xf_mix && ndi_nusigf_avail(state, sig_xf_type1) &&
               ndi_nusigf_avail(state, sig_xf_type2)) {
        calc_type = MIX;
        ierr = 10;              /* Calculated value */
    } else {
        NDI_THROW(ierr, -9);
    }

    /* Get matrix data */
    switch(calc_type) {

    case MAT:                   /* Read chi matrix */

        jerr = NDI_READ_FLOAT_BLOCK(state->lib_stream, mat_ptr, num_full_mat,
                                    tmp_mat, num_full_mat);
        if (jerr) NDI_THROW(ierr, jerr);
        break;

    case SUM:                   /* Get other two sig_xf matrices */
    case MIX:

        n = lib_num_grps * lib_num_grps;
        /* Allocate sig_xf1 */
        sig_xf1 = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!sig_xf1) NDI_THROW(ierr, -14); /* Out of memory */

        /* Get sig_xf__mat1 */
        jerr = GET_SIG_XF_MAT(state, sig_xf_type1, sig_xf1, n);
        if (jerr) NDI_THROW(ierr, jerr);

        /* Allocate sig_xf2 */
        sig_xf2 = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!sig_xf2) NDI_THROW(ierr, -14); /* Out of memory */

        /* Get sig_xf_mat2 */
        jerr = GET_SIG_XF_MAT(state, sig_xf_type2, sig_xf2, n);
        if (jerr) NDI_THROW(ierr, jerr);

        break;
        
    case VEC:                   /* Calculate chi matrix from vector */

        chi_vec = (FLOAT_TYPE *) ndi_malloc(lib_num_grps * 
                                            sizeof(FLOAT_TYPE));
        if (!chi_vec) NDI_THROW(ierr, -14); /* Out of memory */
     
        jerr = NDI_READ_FLOAT_BLOCK(state->lib_stream, vec_ptr, lib_num_grps,
                                    chi_vec, lib_num_grps);
        if (jerr) NDI_THROW(ierr, jerr);

        jerr = NDI_VEC2MAT(chi_vec, lib_num_grps, tmp_mat,
                           num_full_mat, num_full_mat);
        if (jerr) NDI_THROW(ierr, jerr);
        ierr = 10;            /* Matrix has been constructed */
        break;

    }

    n = lib_num_grps;
    /* Calculate fission transfer matrix */
    switch (calc_type) {
        
    case MAT:                   /* from nusigf * chi */
    case VEC:

        nusigf = (FLOAT_TYPE *)ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!nusigf) NDI_THROW(ierr, -14); /* Out of memory */

        jerr = NDI_GET_NUSIGF(state, sig_xf_type, nusigf, &n);
        if (jerr) NDI_THROW(ierr, jerr);

        n = lib_num_grps;
        for (in = 0; in < n; in++)
            for (out = 0; out < n; out++)
                tmp_mat[in*n+out] *= nusigf[in];
        break;

    case SUM:                   /* from t = p+d */
    case MIX:

        for (in = 0; in < n; in++)
            for (out = 0; out < n; out++) {
                tmp_mat[in*n+out] = sig_xf1[in*n+out] + 
                    s * sig_xf2[in*n+out];
                if (tmp_mat[in*n+out] < 0.0) tmp_mat[in*n+out] = 0.0;
            }
        break;
    }

    if (collapse_flag) {        /* Collapsing required */
        coll_num_grps = state->grp_num_new;
        jerr = NDI_COLLAPSE_MAT(tmp_mat, lib_num_grps, lib_num_grps,
                                coll_num_grps, coll_num_grps,
                                state->new_ebounds, state->new_ebounds,
                                state->lib_weight, return_mat,
                                return_size);
    } else {                   /* Copy required */
        jerr = COPY_MATRIX(return_mat, return_size, 
                           tmp_mat, lib_num_grps*lib_num_grps);
    }
    if (jerr) NDI_THROW(ierr, jerr);

    /*   Transpose matrix */
    if ((mat_order == 't') && (ierr >= 0) && (ierr != 7)) {
        jerr = NDI_TRANSPOSE_FLOAT(return_mat, coll_num_grps, coll_num_grps);
        if (jerr) NDI_THROW(ierr, jerr);
    }

 catch:
    /* Restore user settings */
    if (state) {
        state->collapse_flag = collapse_flag;
        state->mat_order = mat_order;
        state->no_norm = no_norm;
    }

    /* Free resources */
    ndi_free((void **) &sig_xf2);
    ndi_free((void **) &sig_xf1);
    ndi_free((void **) &chi_vec);
    ndi_free((void **) &nusigf);
    ndi_free((void **) &tmp_mat);

    return (ierr);
}

/*********************************************************************/
/*********************************************************************/

static int NDI_CALC_E_AVG(state_struct *state, FLOAT_TYPE *return_vec,
                          int return_size)
{

    int num_grps;
    int ierr = 0;


    NDI_INIT_FLOAT_VEC(return_vec, return_size);

    if (state) {
        num_grps = state->iso_stat.num_grps;
        
        if ( num_grps <= 0 ) {
            ierr = 9;
            return(ierr);
        }
        
        if (state->collapse_flag) {
            num_grps = state->grp_num_new;
        }
        
        switch (state->particle_type) {
            
        case 0:        /* Photon */
            ierr = PHOTON_E_AVG(state, num_grps, return_vec, return_size);
            break;
            
        case 1:        /* Neutron */
            ierr = NEUTRON_E_AVG(state, num_grps, return_vec, return_size);
            break;
            
        default:       /* Other particle */
            /* Warning? */
            ierr = MIDPT_ENERGY(state, num_grps, return_vec, return_size);
            break;
            
        }
        
    } else {
        ierr = -2;
    }

    return(ierr);
}

static int NDI_CALC_V_AVG(state_struct *state, FLOAT_TYPE *return_vec,
                          int return_size)
{
    int num_grps;
    int ierr = 0;

    NDI_INIT_FLOAT_VEC(return_vec, return_size);

    if (state) {
        num_grps = state->iso_stat.num_grps;
        
        if ( num_grps <= 0 ) {
            ierr = 9;
            return(ierr);
        }
        
        if (state->collapse_flag) {
            num_grps = state->grp_num_new;
        }
        
        switch (state->particle_type) {

        case 0:                 /* Photon */
            /*                            m/s     * cm/m * s/sh */ 
            ierr = NDI_CONST_FLOAT_VEC(V_PHOT_MPS * 100  * 1e-8, num_grps,
                                       return_vec, return_size);
            break;
            
        case 1:        /* Neutron */
            ierr = TD_MOMENT(state, 'v', num_grps, return_vec, return_size);
            break;
            
        default:       /* Other particle */
            ierr = 9;
            break;
        }
        
    } else {
        ierr = -2;
    }

    return(ierr);
}
/*********************************************************************/

static int NDI_CALC_E_N(state_struct *state, FLOAT_TYPE * return_vec,
                        int return_size)
{

    int i;                      /* counter over incident groups */
    int j;                      /* counter over outgoing groups */
    int num_grps;
    int ierr = 0;
    FLOAT_TYPE *eavg;
    FLOAT_TYPE *p0_vec;
    int p0_size;


    NDI_INIT_FLOAT_VEC(return_vec, return_size);

    if (state) {
        num_grps = state->iso_stat.num_grps;
    } else {
        return (-2);
    }

    if (num_grps <= 0) {
        ierr = 9;
        return (ierr);
    }

    if (state->collapse_flag) {
        num_grps = state->grp_num_new;
    }

    if (state->particle_type == 1) { /* Neutron */
        /*p0_size =  num_grps * num_grps; */
        p0_size = ndi_c_get_size_n_h(state, NDI_TRANSFER_MAT_PR, 0, &ierr);

        p0_vec = (FLOAT_TYPE *) ndi_malloc(p0_size * sizeof(FLOAT_TYPE));
        ierr = NDI_GET_FLOAT_VEC_N_H(state, NDI_TRANSFER_MAT_PR, 0, 
                                     p0_vec, p0_size);
        eavg = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
        ierr = NDI_GET_FLOAT_VEC_H(state, NDI_E_AVG, eavg, num_grps);
        for (i = 0; i < num_grps; i++) {
            return_vec[i] = 0;
            for (j = 0; j < num_grps; j++) {
                return_vec[i] += eavg[j] * p0_vec[(i * num_grps) + j];
            }
        }

        if (p0_vec)
            ndi_free((void **) &p0_vec);
        if (eavg)
            ndi_free((void **) &eavg);
    }
    return (ierr);
}

/*********************************************************************/

static int NDI_CALC_E_AVAIL(state_struct *state, FLOAT_TYPE * return_vec,
                            int return_size)
{

    int i;                      /* counter over reactions */
    int j;                      /* counter over energy groups */
    int num_grps;
    int ierr = 0;
    int *MT;
    int num_reac;
    FLOAT_TYPE Q;
    FLOAT_TYPE *eavg;
    FLOAT_TYPE *rx_cross_sec;


    NDI_INIT_FLOAT_VEC(return_vec, return_size);

    if (state) {
        num_grps = state->iso_stat.num_grps;
    } else {
        return (-2);
    }

    if (num_grps <= 0) {
        ierr = 9;
        return (ierr);
    }

    if (state->collapse_flag) {
        num_grps = state->grp_num_new;
    }

    ierr = ndi_c_get_int_val_h(state, NDI_NUM_REAC, &num_reac);
    MT = (int *) ndi_malloc(num_reac * sizeof(int));
    ierr = ndi_c_get_int_vec_h(state, NDI_REAC_MT, MT, num_reac);
    rx_cross_sec =
        (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
    eavg = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
    ierr = NDI_GET_FLOAT_VEC_H(state, NDI_E_AVG, eavg, num_grps);

    for (i = 0; i < num_reac; i++) {

        if (is_partial_fission(MT[i])) continue;

        ierr =
            NDI_GET_FLOAT_VEC_N_H(state, NDI_SIG_REAC, MT[i], rx_cross_sec,
                                  num_grps);
        ierr = NDI_GET_FLOAT_VAL_N_H(state, NDI_REAC_Q, MT[i], &Q);
        for (j = 0; j < num_grps; j++) {
            return_vec[j] += (eavg[j] + Q) * rx_cross_sec[j];
        }
    }

    if (rx_cross_sec)
        ndi_free((void **) &rx_cross_sec);
    if (eavg)
        ndi_free((void **) &eavg);
    if (MT)
        ndi_free((void **) &MT);

    return (ierr);
}

/*********************************************************************/

static int is_partial_fission(int mt)
{
    return mt == 38 || ((mt > 18) && (mt < 22));
}

/*********************************************************************/
static int NDI_GET_HEATING(state_struct *state, stat_flag * heating,
                           stat_flag * kerma, FLOAT_TYPE * return_vec,
                           int return_size)
{

    int ierr = 0;
    int num_elements;

    if (!state) {
        return (-2);
    }

    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);

    /* Units are MeV/collision */
    num_elements = state->iso_stat.num_grps;

    if (num_elements <= 0) {
        ierr = 9;
        return (ierr);
    }

    /*  Check bounds */
    if (heating->file_ptr > 0) {
        /* A heating vector exists in the library */
        ierr =
            GET_SIMPLE_FLOAT_VEC(state, state->lib_stream,
                                 heating->file_ptr, 0, num_elements,
                                 return_vec, return_size);
    } else if (kerma->file_ptr > 0) {
        /* A kerma exists in the library, try to calculate heating */
        ndi_float64 *temp_tot_sigma, *temp_kerma;
        temp_tot_sigma =
            (ndi_float64 *) ndi_malloc(num_elements * sizeof(ndi_float64));
        temp_kerma =
            (ndi_float64 *) ndi_malloc(num_elements * sizeof(ndi_float64));
        if ((!temp_kerma) || (!temp_tot_sigma)) {
            /* Out of memory */
            ierr = -14;
        } else {
            ierr =
                ndi_get_simple_float64_vec(state, state->lib_stream,
                                           kerma->file_ptr, -1,
                                           num_elements, temp_kerma,
                                           num_elements);
            if (ierr >= 0) {
                ierr =
                    ndi_get_simple_float64_vec(state, state->lib_stream,
                                               state->iso_stat.sig_tot.
                                               file_ptr, -1, num_elements,
                                               temp_tot_sigma,
                                               num_elements);
                ierr =
                    DIVIDE_FLOAT_VEC(state, temp_kerma, temp_tot_sigma,
                                     num_elements, 0, return_vec,
                                     return_size);
            }
        }
        if (temp_tot_sigma) {
            ndi_free((void **) &temp_tot_sigma);
        }
        if (temp_kerma) {
            ndi_free((void **) &temp_kerma);
        }
    } else if ((state->kerma_calc == DATA_CALC)
               && (state->iso_stat.num_reac > 0)) {
        ierr = NDI_GET_OLD_HEATING(state, return_vec, return_size);
        if (ierr >= 0) {
            ierr = 10;
        }
    } else {
        ierr = 9;
    }
    if (ierr == -9) {
        ierr = 9;
    }
    return (ierr);

}

/*********************************************************************/
/*********************************************************************/
static int NDI_GET_KERMA(state_struct *state, stat_flag * kerma,
                         stat_flag * heating, FLOAT_TYPE * return_vec,
                         int return_size)
{

    int ierr = 0;
    int num_elements;

    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);

    if (!state) {
        return (-2);
    }

    /* Units are MeV * barns */
    num_elements = state->iso_stat.num_grps;


    /*  Check bounds */
    if (num_elements <= 0) {
        ierr = 9;
        return (ierr);
    }


    if (kerma->file_ptr > 0) {
        /* A kerma vector exists in the library */
        ierr =
            GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, kerma->file_ptr,
                                 0, num_elements, return_vec, return_size);
    } else if (heating->file_ptr > 0) {
        /* A heating exists in the library, try to calculate kerma */
        ndi_float64 *temp_tot_sigma, *temp_heat;
        temp_tot_sigma =
            (ndi_float64 *) ndi_malloc(num_elements * sizeof(ndi_float64));
        temp_heat =
            (ndi_float64 *) ndi_malloc(num_elements * sizeof(ndi_float64));
        if ((!temp_heat) || (!temp_tot_sigma)) {
            /* Out of memory */
            ierr = -14;
        } else {
            ierr =
                ndi_get_simple_float64_vec(state, state->lib_stream,
                                           heating->file_ptr, -1,
                                           num_elements, temp_heat,
                                           num_elements);
            if (ierr >= 0) {
                ierr =
                    ndi_get_simple_float64_vec(state, state->lib_stream,
                                               state->iso_stat.sig_tot.
                                               file_ptr, -1, num_elements,
                                               temp_tot_sigma,
                                               num_elements);
                ierr =
                    MULT_FLOAT_VEC(state, temp_tot_sigma, temp_heat,
                                   num_elements, 0, return_vec,
                                   return_size);
            }
        }
        if (temp_tot_sigma) {
            ndi_free((void **) &temp_tot_sigma);
        }
        if (temp_heat) {
            ndi_free((void **) &temp_heat);
        }
    } else if ((state->kerma_calc == DATA_CALC)
               && (state->iso_stat.num_reac > 0)) {
        ierr = NDI_GET_OLD_KERMA(state, return_vec, return_size);
        if (ierr >= 0) {
            ierr = 10;
        }
    } else {
        ierr = 9;
    }
    if (ierr == -9) {
        ierr = 9;
    }
    return (ierr);

}

/*********************************************************************/
/*********************************************************************/
static int GET_E_BOUNDS_CURR_PART(state_struct *state,
                                  FLOAT_TYPE * return_vec, int return_size)
{

    int i;
    int ierr = 0;
    sec_part_mg *part;

    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);

    if ((state->iso_stat.sec_part) &&
        (0 <= state->curr_part_index) &&
        (state->curr_part_index < state->iso_stat.num_sec_parts)) {
        part = &state->iso_stat.sec_part[state->curr_part_index];
        if (part->collapse_flag) {
            /* User-specified collapse-group structure */
            for (i = 0; i <= part->grp_num_new; i++) {
                if (i < return_size) {
                    return_vec[i] = 
                        (FLOAT_TYPE) part->e_bounds[part->new_ebounds[i]];
                } else {
                    return (7);
                }
            }
        } else {
            /* Library ebounds */
            for (i = 0; i <= part->num_grps; i++) {
                if (i < return_size) {
                    return_vec[i] = (FLOAT_TYPE) part->e_bounds[i];
                } else {
                    return (7);
                }
            }
        }
    } else {
        /* Error -- bad particle? */
        ierr = -999;
    }

    return (ierr);
}

/*********************************************************************/
/*********************************************************************/
static int NDI_GET_OLD_HEATING(state_struct *state,
                               FLOAT_TYPE * return_vec, int return_size)
{
    int i;
    int ierr;
    int num_grps;
    FLOAT_TYPE *tot_sig;
    FLOAT_TYPE *vec;

    ierr = ndi_c_get_int_val_h(state, NDI_NUM_GRPS, &num_grps);
    if (num_grps > 0) {
        tot_sig =
            (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
        vec = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
    } else {
        ierr = 5;
        return (ierr);
    }
    ierr = NDI_GET_FLOAT_VEC_H(state, NDI_SIG_TOT, tot_sig, num_grps);
    ierr = NDI_GET_OLD_KERMA(state, vec, num_grps);

    for (i = 0; i < num_grps; i++) {
        if ( fabs(tot_sig[i]) > (vec[i] * DBL_EPSILON) ) {
            vec[i] = vec[i] / tot_sig[i];
        } else {
            vec[i] = 0.0;
            ierr = 5;
        }
        if (i < return_size) {
            return_vec[i] = (FLOAT_TYPE)vec[i];
        } else {
            ierr = 7;
        }
    }

    if (tot_sig) {
        ndi_free((void **) &tot_sig);
    }
    if (vec) {
        ndi_free((void **) &vec);
    }

    return (ierr);
}

/*********************************************************************/
/*********************************************************************/
static int NDI_GET_NEW_KERMA(state_struct *state, 
                             FLOAT_TYPE *return_vec, int return_size)
{
    int collapse_flag = 0;
    char reac_id = 'm';
    char mat_order = 'd';
    char trans_corr = ' ';
    int ierr = 0;
    int jerr = 0;
    int i, j;
    int num_elements;
    int num_reactions;
    int mat_size;
    FLOAT_TYPE q;

    FLOAT_TYPE *tmp_vec = NULL;
    FLOAT_TYPE *eavg = NULL;
    FLOAT_TYPE *curr_xs = NULL;
    int *mt = NULL;
    FLOAT_TYPE *qf = NULL;
    FLOAT_TYPE *xfer_mat = NULL;

    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);
    if (!state) NDI_THROW(ierr, -2); /* State not initialized */

    num_elements = state->iso_stat.num_grps;
    if (num_elements <= 0) NDI_THROW(ierr, -9); /* Bounds not initialized */

    /* Turn off collapsing
       for best approzimation of continuous products
       with multigroup quantities */
    collapse_flag = state->collapse_flag;
    state->collapse_flag = 0;

    /* Remember user setting and get reactions by mt */
    reac_id = state->reac_id;
    state->reac_id = 'm';

    /* Remember user settings and set matrix default order */
    mat_order = state->mat_order;
    state->mat_order = 'd';

    /* Remember user settings and turn transport corrections off */
    trans_corr = state->trans_corr;
    state->trans_corr = ' ';

    /* Allocate and zero result vector */
    tmp_vec = (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
    if (!tmp_vec) NDI_THROW(ierr, -14); /* Out of memory */
    (void) NDI_INIT_FLOAT_VEC(tmp_vec, num_elements);

    /* Allocate and get average energy */
    eavg = (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
    if (!eavg) NDI_THROW(ierr, -14); /* Out of memory */
    /* NB: Values returned depend on NDI_E_AVG option setting */
    jerr = GET_FLOAT_VEC_MG(state, NDI_E_AVG, eavg, num_elements);
    if (jerr) NDI_THROW(ierr, jerr);
    
    /* Allocate cross section */
    curr_xs = (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
    if (!curr_xs) NDI_THROW(ierr, -14); /* Out of memory */

    /* Allocate and get reaction list */
    jerr = ndi_c_get_int_val_h(state, NDI_NUM_REAC, &num_reactions);
    if (jerr) NDI_THROW(ierr, jerr);
    mt = (int *) ndi_malloc(num_reactions * sizeof(int));
    if (!mt) NDI_THROW(ierr, -14); /* Out of memory */
    jerr = ndi_c_get_int_vec_h(state, NDI_REAC_MT, mt, num_reactions);
    if (jerr) NDI_THROW(ierr, jerr);

    for (i = 0; i < num_reactions; i++) {

        /* Skip partial fission cross sections already in MT = 18 */
        if (is_partial_fission(mt[i])) continue;

        /* Get cross section */
        jerr = NDI_GET_FLOAT_VEC_N_H(state, NDI_SIG_REAC, mt[i], 
                                     curr_xs, num_elements);
        if (jerr) NDI_THROW(ierr, jerr);
        
        if (mt[i] == 18) {
            /* Allocage and get fission Q */
            qf = (FLOAT_TYPE *)ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
            if (!qf) NDI_THROW(ierr, -14); /* Out of memory */
            jerr = GET_QF(state, qf, num_elements);
            if (jerr) NDI_THROW(ierr, jerr);

            /* Add incident neutron and fission Q contribution to kerma */
            for (j = 0; j < num_elements; j++)
                tmp_vec[j] += curr_xs[j] * (eavg[j] + qf[j]); 
        } else {
            /* Get constant reaction Q */
            jerr = NDI_GET_FLOAT_VAL_N_H(state, NDI_REAC_Q, mt[i], &q);
            if (jerr) NDI_THROW(ierr, jerr);

            /* Add incident neutron and reaction Q contribution to kerma */
            for (j = 0; j < num_elements; j++)
                tmp_vec[j] += curr_xs[j] * (eavg[j] + q);
        }
    }

    /* Allocate and get transfer matrix */
    mat_size = ndi_c_get_size_n_h(state, NDI_TRANSFER_MAT_PR, 0, &jerr);
    if (jerr) NDI_THROW(ierr, jerr);
    xfer_mat = (FLOAT_TYPE *) ndi_malloc(mat_size * sizeof(FLOAT_TYPE));
    if (!xfer_mat) NDI_THROW(ierr, -14); /* Out of memory */
    jerr = NDI_GET_FLOAT_VEC_N_H(state, NDI_TRANSFER_MAT_PR, 0,
                                 xfer_mat, mat_size);
    if (jerr) NDI_THROW(ierr, jerr);

    /* Subtract outgoing neutron energies */
    for (i = 0; i < num_elements; i++) {        /* i = incident energy */
        for (j = 0; j < num_elements; j++) {    /* j = outgoing energy */
            tmp_vec[i] -= xfer_mat[j + (i * num_elements)] * eavg[j];
        }
    }

    /* Restore transport correction */
    state->trans_corr = trans_corr;
    trans_corr = ' ';

    /* Restore matrix order */
    state->mat_order = mat_order;
    mat_order = 'd';

    /* Restore reaction id */
    state-> reac_id = reac_id;
    reac_id = 'm';

    /* Restore collapsing */
    state->collapse_flag = collapse_flag;
    collapse_flag = 0;

    /* Collapse or copy result */
    if (state->collapse_flag) {
        jerr = NDI_COLLAPSE_FLOAT(state, tmp_vec, num_elements, 0, 
                            return_vec, return_size);
    } else {
        jerr = COPY_MATRIX(return_vec, return_size, tmp_vec, num_elements);
    }
    if (jerr) NDI_THROW(ierr, jerr);

 catch:                         /* Restore settings and free memory */
    if (trans_corr != ' ') state->trans_corr = trans_corr;
    if (mat_order != 'd') state->mat_order = mat_order;
    if (reac_id != 'm') state->reac_id = reac_id;
    if (collapse_flag) state->collapse_flag = collapse_flag;

    ndi_free((void **) &xfer_mat);
    ndi_free((void **) &qf);
    ndi_free((void **) &mt);
    ndi_free((void **) &curr_xs);
    ndi_free((void **) &eavg);
    ndi_free((void **) &tmp_vec);

    return ierr;
}
/*********************************************************************/
/*********************************************************************/
static int NDI_GET_OLD_KERMA(state_struct *state, FLOAT_TYPE * return_vec,
                      int return_size)
{
    char transpose_state;
    char corr_state;
    int ierr = 0;
    int i, j;
    int num_elements;
    int num_reactions;
    int mat_size;
    int old_reac_size = 3;
    char old_reac_id[3];
    ndi_float64 fissq[NUM_FISSION_Q];
    ndi_float64 q;

    int *mt = NULL;
    ndi_float64 *tmp_vec = NULL;
    ndi_float64 *curr_xs = NULL;
    ndi_float64 *eavg = NULL;
    ndi_float64 *scat_mat = NULL;

    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);

    /* Units are MeV * barns */
    num_elements = state->iso_stat.num_grps;

    /*  Check bounds */
    if (num_elements <= 0) {
        ierr = 9;
        return (ierr);
    }

    if (state->collapse_flag) {
        num_elements = state->grp_num_new;
    }

    ierr = ndi_c_check_option_h(state, NDI_REAC_ID, old_reac_id, old_reac_size);
    ierr = ndi_c_set_option_h(state, NDI_REAC_ID, "mt");
    ierr = ndi_c_get_int_val_h(state, NDI_NUM_REAC, &num_reactions);
    mt = (int *) ndi_malloc(num_reactions * sizeof(int));
    ierr = ndi_c_get_int_vec_h(state, NDI_REAC_MT, mt, num_reactions);

    tmp_vec = (ndi_float64 *) ndi_malloc(num_elements * sizeof(ndi_float64));
    curr_xs = (ndi_float64 *) ndi_malloc(num_elements * sizeof(ndi_float64));
    eavg = (ndi_float64 *) ndi_malloc(num_elements * sizeof(ndi_float64));
    (void) ndi_init_float64_vec(tmp_vec, num_elements);
    ierr = ndi_c_get_float64_vec_h(state, NDI_E_AVG, eavg, num_elements);
    ierr = ndi_c_get_float64_vec_h(state, NDI_FISS_Q, fissq, NUM_FISSION_Q);

    for (i = 0; i < num_reactions; i++) {

        /* Partial fission cross sections already contained in MT = 18 */
        if (is_partial_fission(mt[i])) continue;

        ierr =
            ndi_c_get_float64_vec_n_h(state, NDI_SIG_REAC, mt[i], curr_xs,
                                      num_elements);
        if (mt[i] == 18) {
            q = fissq[0] - fissq[5];    /* Q(prompt fiss) - Q(prompt n) */
        } else {
            ierr = ndi_c_get_float64_val_n_h(state, NDI_REAC_Q, mt[i], &q);
        }
        for (j = 0; j < num_elements; j++) {
            tmp_vec[j] += curr_xs[j] * (eavg[j] + q);
/*            printf ("%d: %e   q=%e  e=%e  xs=%e\n", j, tmp_vec[j], q, eavg[j], curr_xs[j]); */
        }
    }

    /* Handle energy from scattering */
    mat_size = ndi_c_get_size_n_h(state, NDI_PN_FULL, 0, &ierr);
    scat_mat = (ndi_float64 *) ndi_malloc(mat_size * sizeof(ndi_float64));

    /* Don't transpose or correct matrix,
       but save user-defined transpose & trans. cor. state */
    transpose_state = state->mat_order;
    state->mat_order = 'd';
    corr_state = state->trans_corr;
    state->trans_corr = ' ';
    ierr = ndi_c_get_float64_vec_n_h(state, NDI_PN_FULL, 0, scat_mat, mat_size);
    state->mat_order = transpose_state;
    state->trans_corr = corr_state;

    for (i = 0; i < num_elements; i++) {        /* i = incident energy */
        for (j = 0; j < num_elements; j++) {    /* j = outgoing energy */
            tmp_vec[i] -= scat_mat[j + (i * num_elements)] * eavg[j];
        }
    }

    for (i = 0; i < num_elements; i++) {
        if (i < return_size) {
            return_vec[i] = tmp_vec[i];
        } else {
            ierr = 7;
        }
    }

    if (scat_mat) {
        ndi_free((void **) &scat_mat);
    }
    if (eavg) {
        ndi_free((void **) &eavg);
    }
    if (curr_xs) {
        ndi_free((void **) &curr_xs);
    }
    if (tmp_vec) {
        ndi_free((void **) &tmp_vec);
    }
    if (mt) {
        ndi_free((void **) &mt);
    }

    ndi_c_set_option_h(state, NDI_REAC_ID, old_reac_id);

    return (ierr);

}

/*********************************************************************/
/*********************************************************************/
static int SIG_TOT_TRANS_CORR(state_struct *state,
                              FLOAT_TYPE * return_vec, int return_size)
{

    int i;
    int num_grps;
    int ierr = 0;
    FLOAT_TYPE *corr;


    if (state) {
        num_grps = state->iso_stat.num_grps;
    } else {
        return (-2);
    }

    /*  Check bounds */
    if (num_grps <= 0) {
        ierr = 9;
        return (ierr);
    }

    if (state->collapse_flag)
        num_grps = state->grp_num_new;
/*    printf("numgrps = %d\n", num_grps); */
    corr = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
    if (state->trans_corr == 'd') {
        ierr =
            NDI_GET_FLOAT_VEC_N_H(state, NDI_CORR_DIAG,
                                  state->trans_corr_order, corr, num_grps);
    } else if (state->trans_corr == 'b') {
/*       printf("D: %d\n", trans_corr_order); */
        ierr =
            NDI_GET_FLOAT_VEC_N_H(state, NDI_CORR_BHS,
                                  state->trans_corr_order, corr, num_grps);
    } else {
/*       printf("Error\n"); */
        ierr = -13;
    }
    /*** Bounds checking? ***/
/*    printf("correcting vector %d\n", ierr); */
    if (ierr >= 0) {
        for (i = 0; i < num_grps; i++) {
/*       printf("%d  %e\n", i, corr[i]); */
            if (i >= return_size) {
                ierr = 7;
                break;
            }
            return_vec[i] -= corr[i];
        }
    }
    if (corr)
        ndi_free((void **) &corr);
    return (ierr);
}

/*********************************************************************/
/*********************************************************************/

/*  Purpose:  Fill array 'return_vec' with appropriate chi vector
    (i.e. the energy distribution of neutrons produced after fission,
    assuming independence of incident neutron energy).  The vector
    is normalized to one (if total sum is not zero).

    (1) First, look in library for appropriate chi vector.
    (2) If chi matrix and wgts exist, flux-average chi matrix over
    incident energy to calculate chi vector:
    chi_vec(Eout) = sum over inc. energy grp i of [chi_mat(i,Eout) * wgt(i)]
    (3) if nu_sigfs, wgts, and other two chi vectors are available, 
    calculate appropriate chi vec using equation:
    chi_wgt_tot * chi_tot = chi_wgt_pr * chi_pr + chi_wgt_del * chi_del,
    where chi_wgt_tot = sum over groups (i) of (nu_sigf_tot(i) * wgt(i) ) 
    and similarly for other nu_avg values

    Parameters:
    state -- in:pointer to handle state
    chi_type -- in:t=total, p=prompt, d=delayed
    return_vec -- out:array to fill with chi values.
    return_size -- in/out:on input shows the size of the array; on output
    shows the size remaining in array ( i.e. original return_size 
    minus the number of elements filled ). May be less zero 
    on exit.

    NOTE:  RETURN_SIZE IS NOT CURRENTLY MODIFIED IN THIS SUBROUTINE.  

    Note: This function normalizes the vector to one.  If 'return_vec'
    is not large enough to contain the whole vector, it is truncated and
    will not be normalized to one--the full vector is normalized to one.

    ******************************************************************** */

static int NDI_GET_CHI_VEC(state_struct *state, char chi_type, 
                           FLOAT_TYPE *return_vec, int *return_size)
{
    int ierr = 0;
    int jerr = 0;

    int n;
    int num_grps;

    long int chi_vec_ptr;
    long int chi_mat_ptr;
    long int chi_flg1;
    long int chi_flg2;
    char chi_type1;
    char chi_type2;
    FLOAT_TYPE s;

    FLOAT_TYPE chi_wgt1 = 0.0;
    FLOAT_TYPE chi_wgt2 = 0.0;
    FLOAT_TYPE chi_value = 0.0;
    FLOAT_TYPE norm_sum = 0.0;

    int in, out;
    char mat_order;
    int no_norm;

    FLOAT_TYPE *chi_mat = NULL;
    FLOAT_TYPE *nu_sig_f = NULL;
    FLOAT_TYPE *wgts = NULL;
    FLOAT_TYPE *chi_vec1 = NULL;
    FLOAT_TYPE *chi_vec2 = NULL;

    (void) NDI_INIT_FLOAT_VEC(return_vec, *return_size);

    if (!state) NDI_THROW(ierr, -2); /* State not initialized */

    switch (chi_type) {

    case 't':                  /* total neutrons */
        chi_vec_ptr = state->iso_stat.chi_vec_tot.file_ptr;

        chi_mat_ptr = state->iso_stat.chi_mat_tot.file_ptr;

        chi_type1 = 'p';
        chi_flg1 = (state->iso_stat.chi_vec_pr.file_ptr ||
                    state->iso_stat.chi_mat_pr.file_ptr);
        s = +1.0;
        chi_type2 = 'd';
        chi_flg2 = (state->iso_stat.chi_vec_del.file_ptr ||
                    state->iso_stat.chi_mat_del.file_ptr);
        break;

    case 'p':                  /* prompt neutrons */
        chi_vec_ptr = state->iso_stat.chi_vec_pr.file_ptr;

        chi_mat_ptr = state->iso_stat.chi_mat_pr.file_ptr;

        chi_type1 = 't';
        chi_flg1 = (state->iso_stat.chi_vec_tot.file_ptr ||
                    state->iso_stat.chi_mat_tot.file_ptr);
        s = -1.0;
        chi_type2 = 'd';
        chi_flg2 = (state->iso_stat.chi_vec_del.file_ptr ||
                    state->iso_stat.chi_mat_del.file_ptr);
        break;

    case 'd':                  /* delayed neutrons */
        chi_vec_ptr = state->iso_stat.chi_vec_del.file_ptr;

        chi_mat_ptr = state->iso_stat.chi_mat_del.file_ptr;

        chi_type1 = 't';
        chi_flg1 = (state->iso_stat.chi_vec_tot.file_ptr ||
                    state->iso_stat.chi_mat_tot.file_ptr);
        s = -1.0;
        chi_type2 = 'p';
        chi_flg2 = (state->iso_stat.chi_vec_pr.file_ptr ||
                    state->iso_stat.chi_mat_pr.file_ptr);
        break;

    default:
        NDI_THROW(ierr, -13);           /* Invalid input parameter */
    }

    if (state->iso_stat.fissionable == 0) goto catch; /* Not fissionable */

    if (state->collapse_flag) {
        num_grps = state->grp_num_new;
    } else {
        num_grps = state->iso_stat.num_grps;
    }

    if (chi_vec_ptr) {
        /* (1) Read chi vector from library */
        jerr = GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, chi_vec_ptr,
                                    2, state->iso_stat.num_grps,
                                    return_vec, *return_size);
        if (jerr) NDI_THROW(ierr, jerr);

    } else if (chi_mat_ptr && ndi_nusigf_avail(state, chi_type) && 
               state->iso_stat.wgts.file_ptr) {
        /* (2)  Project chi vector from chi matrix */

        /* Get collapsed chi matrix */
        n = num_grps * num_grps;
        chi_mat = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!chi_mat) NDI_THROW(ierr, -14); /* Out of memory */
          
        mat_order = state->mat_order; /* remember user matrix order */
        state->mat_order = 'd'; /* get matrix in "C" order */
        jerr = GET_CHI_MAT(state, chi_type, chi_mat, n);
        state->mat_order = mat_order; /* restore user matrix order */
        if (jerr) NDI_THROW(ierr, jerr);

        /* Get collapsed nu_sig_f */
        n = num_grps;
        nu_sig_f = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!nu_sig_f) NDI_THROW(ierr, -14); /* Out of memory */

        jerr = NDI_GET_NUSIGF(state, chi_type, nu_sig_f, &n);
        if (jerr) NDI_THROW(ierr, jerr);
  
        /* Get collapsed weights */
        n = num_grps;
        wgts = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!wgts) NDI_THROW(ierr, -14); /* Out of memory */

        jerr = GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, 
                                    state->iso_stat.wgts.file_ptr, 3, 
                                    state->iso_stat.num_grps, wgts, n);
        if (jerr) NDI_THROW(ierr, jerr);

        /* Project matrix to vector */
        norm_sum = 0.0;
        for (out = 0; out < n; out++) {
            return_vec[out] = 0.0;
            for (in = 0; in < n; in++) {
                return_vec[out] += nu_sig_f[in] * wgts[in] * 
                    chi_mat[in * n + out];
            }
            norm_sum += return_vec[out];
        }

        if (norm_sum != 0.0) {
            for (out = 0; out < n; out++) {
                return_vec[out] /= norm_sum;
            }
        }

    } else if (chi_flg1 && ndi_nusigf_avail(state, chi_type1) && 
               chi_flg2 && ndi_nusigf_avail(state, chi_type2)) {
        /* (3) Calculate chi vector from other two */

        /* Get first chi vector */
        n = num_grps;
        chi_vec1 = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!chi_vec1) NDI_THROW(ierr, -14); /* Out of memory */

        no_norm = state->no_norm; /* remember user normalization */
        state->no_norm = 1;   /* turn off normalization */
        jerr = NDI_GET_CHI_VEC(state, chi_type1, chi_vec1, &n);
        state->no_norm = no_norm; /* restore user normalization */
        if (jerr) NDI_THROW(ierr, jerr);

        /* Get first chi weight */
        jerr = CALC_CHI_WGT(state, chi_type1, &chi_wgt1);
        if (jerr) NDI_THROW(ierr, jerr);
     
        /* Get second chi vector */
        n = num_grps;
        chi_vec2 = (FLOAT_TYPE *) ndi_malloc(n * sizeof(FLOAT_TYPE));
        if (!chi_vec2) NDI_THROW(ierr, -14); /* Out of memory */

        no_norm = state->no_norm; /* remember user normalization */
        state->no_norm = 1;   /* turn off normalization */
        jerr = NDI_GET_CHI_VEC(state, chi_type2, chi_vec2, &n);
        state->no_norm = no_norm; /* restore user normalization */
        if (jerr) NDI_THROW(ierr, jerr);

        /* Get second chi weight */
        jerr = CALC_CHI_WGT(state, chi_type2, &chi_wgt2);
        if (jerr) NDI_THROW(ierr, jerr);

        for (n = 0; n < num_grps; n++) {
            chi_value = (chi_wgt1 * chi_vec1[n]) + 
                s * (chi_wgt2 * chi_vec2[n]);
            if (chi_value < 0.0) chi_value = 0.0;
            norm_sum += chi_value;
            if (n < *return_size) {
                return_vec[n] = chi_value;
            } else {
                ierr = 7;
            }
        }

        if (norm_sum != 0.0) {
            for (n = 0; 
                 n < ((num_grps < *return_size)? num_grps: *return_size);
                 n++) { /* nomalize */
                return_vec[n] /= norm_sum;
            } 
        }

    } else {
        NDI_THROW(ierr, -9);            /* Data not available */
    }

    /* Normalize chi to one */
    if (!state->no_norm) {
        if (*return_size >= num_grps) {
            jerr = NORM_FLOAT_VEC(return_vec, num_grps, 1.0);
            if (jerr) NDI_THROW(ierr, jerr);
        } else {
            jerr = NORM_FLOAT_VEC(return_vec, *return_size, 1.0);
            if (jerr) NDI_THROW(ierr, jerr);
            ierr = 7;        /* Data truncated */
        }
    }

 catch:
    ndi_free((void **) &chi_vec2);
    ndi_free((void **) &chi_vec1);
    ndi_free((void **) &wgts);
    ndi_free((void **) &nu_sig_f);
    ndi_free((void **) &chi_mat);

    return ierr;
}

/*********************************************************************/
static int GET_DEPL_XS(state_struct *state, isotope_status * mg,
                       FLOAT_TYPE * return_vec, int return_size)
{

    int ierr = 0;
    int sig_reac_err = 0;
    int i, j, k;
    int found = 0;
    int mtindex = 0;
    int curr_index = -1;
    int num_grps = 0;
    int num_elements = 0;
    int MTarray[MAX_DEPL_REAC];
    int old_reac_size = 3;
    char old_reac_id[3];
    depl_target_struct *curr_targ;
    depl_rprod_struct *curr_prod;
    FLOAT_TYPE *curr_xs = NULL;

    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);

    if (state) {
        curr_targ = state->depl_target;
    } else {
        return (-2);
    }

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


    /* Allocate space for a temporary array to hold cross sections */

    if (state->collapse_flag) {
        num_grps = state->grp_num_new;
    } else {
        num_grps = mg->num_grps;
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


    /* Collect a list of MTs for the current target -- no duplicates! */

    for (i = 0; i < curr_index; i++) {  /* Loop over reaction products */
        for (j = 0; j < curr_prod[i].num_reac; j++) {
            /* check that this mt is not already in the list */
            found = 0;
            for (k = 0; k < mtindex; k++) {
                if (MTarray[k] == curr_prod[i].mt[j]) {
                    found = -1;
                    break;
                }
            }
            if (!found) {
                MTarray[mtindex] = curr_prod[i].mt[j];
                mtindex++;
            }
        }
    }

    ierr = ndi_c_check_option_h(state, NDI_REAC_ID, old_reac_id, old_reac_size);
    ierr = ndi_c_set_option_h(state, NDI_REAC_ID, "mt");

    /* For each MT, get the x.s. and add to running total */

    for (i = 0; i < mtindex; i++) {
        num_elements = num_grps;
        sig_reac_err = GET_SIG_REAC(state, mg, MTarray[i], curr_xs, &num_elements);
        if (sig_reac_err == 9) {
            ierr = 9;
        } else {
            for (j = 0; j < num_grps; j++) {
                if (j < return_size) {
                    return_vec[j] = return_vec[j] + curr_xs[j];
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

    (void) ndi_c_set_option_h(state, NDI_REAC_ID, old_reac_id);
    if (curr_xs)
        ndi_free((void **) &curr_xs);
    return (ierr);

}

/*********************************************************************/
static int GET_EDIST_VEC(state_struct *state, FILE *lib_stream,
                         FLOAT_TYPE * return_vec, int return_size)
{
    /* It is assumed that the temperature data will be given in ASCENDING order!!!! */


    int ierr = 0;
    int i;
    int num_temps;
    int curr_temp_index = -1;
    int hi, lo;
    long int curr_file_p;
    double *distr_E = NULL;
    double *distr_prob = NULL;
    double *edist_array = NULL;
    interp_params curr_interp;
    sec_part_distr *part;


    /**************************** INITIALIZE ****************************/
    /********************************************************************/
    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);

    if (!state) {
        return (-2);
    }

    /* Check that collapse flag is set */
    if (state->grp_num_new <= 0) {
        return (-21);
    }

    /* Set current particle */
    if ((state->curr_part_index < 0) ||
        (state->curr_part_index >= state->sec_part_filled))
        return (-30);

    part = &(state->tn_stat.sec_part[state->curr_part_index]);
    if (!part)
        return (-30);

    /* Find closest temperature */
    num_temps = part->temp_interp.npts;
    ierr =
        ndi_find_temp_bound_index(state->default_temp, part->edist,
                                  num_temps, &hi, &lo, &curr_temp_index);
    if (ierr < 0)
        return (-30);


    /* Initialize distribution variables */
    curr_interp = part->edist[curr_temp_index].interp;
    curr_file_p = part->edist[curr_temp_index].file_ptr;
    distr_E = (double *) ndi_malloc(curr_interp.npts * sizeof(double));
    distr_prob = (double *) ndi_malloc(curr_interp.npts * sizeof(double));
    ierr = ndi_read_float_pair(lib_stream, curr_file_p,
                               curr_interp.npts, distr_E, distr_prob,
                               curr_interp.npts);
    /* Check error */
    if (ierr < 0) {
        if (distr_E)
            ndi_free((void **) &distr_E);
        if (distr_prob)
            ndi_free((void **) &distr_prob);
        return (-30);
    }

    /**************************** INTEGRATE  ****************************/
    /********************************************************************/

    /* Integrate into bins  -- Note: function returns only array of double */
    edist_array =
        (double *) ndi_malloc(state->grp_num_new * sizeof(double));
    ierr =
        ndi_integrate_bins(distr_E, distr_prob, &curr_interp,
                           state->collapse_ebounds, state->grp_num_new,
                           edist_array, state->grp_num_new);
    if (distr_E)
        ndi_free((void **) &distr_E);
    if (distr_prob)
        ndi_free((void **) &distr_prob);

    /* Check error */
    if (ierr < 0) {
        if (edist_array)
            ndi_free((void **) &edist_array);
        return (-30);
    }

    /* Transfer integrals to return_vec */
    /* Copy here */
    if (state->grp_num_new <= return_size) {
        for (i = 0; i < state->grp_num_new; i++) {
            return_vec[i] = (FLOAT_TYPE) edist_array[i];
        }
    } else {
        /* Error */
    }
    if (edist_array)
        ndi_free((void **) &edist_array);

    /**************************** NORMALIZE  ****************************/
    /********************************************************************/
    /* !!ADD!!!!! Special normalize for discrete energies */
    if (state->grp_num_new <= return_size) {
        ierr = NORM_FLOAT_VEC(return_vec, state->grp_num_new, 1.0);
    } else {
        /* Error */
    }


    return (ierr);
}

/*********************************************************************/
static int GET_EDIST_CONT(state_struct *state, char var,
                          FLOAT_TYPE *return_vec, int return_size)
{
    int ierr=0;
    int i, n;
    int lo, hi;
    int it = -1;
    double *E;
    double *P;
    interp_params interp;
    sec_part_distr *part;

    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);
     
    /* check that state, secondary particle selected */
    if (!state) return -2;
    if ((state->curr_part_index < 0) || 
        (state->curr_part_index >= state->sec_part_filled))
        return -30;

    part = &(state->tn_stat.sec_part[state->curr_part_index]);
    if (!part) return -30;

    /* find nearest temperature index */
    ierr = ndi_find_temp_bound_index(state->default_temp, part->edist,
                                     part->temp_interp.npts, &hi, &lo, &it);

    if (ierr < 0) return -30;

    interp = part->edist[it].interp;

    /* allocate E and P */
    n = interp.npts;
    E = (double *) ndi_malloc(n * sizeof(double));
    if (!E) return -14;        /* Out of memory */
    P = (double *) ndi_malloc(n * sizeof(double));
    if (!P) {
        ndi_free((void**) &E);
        return -14;           /* Out of memory */
    }

    /* read E and P */
    ierr = ndi_read_float_pair(state->lib_stream, part->edist[it].file_ptr,
                               n, E, P, n);

    if (ierr < 0) {
        if (E) ndi_free((void **) &E);
        if (P) ndi_free((void **) &P);
        return -30;
    }

    if (n > return_size) {
        n = return_size;
        ierr = 7;             /* Data truncated */
    }

    /* Copy data to return_vec */
    for (i = 0; i < n; i++) {
        if (var == 'E') {
            return_vec[i] = E[i];
        } else if (var == 'P') {
            return_vec[i] = P[i];
        } else {
            ierr = -13;      /* Invalid parameter */
        }
    }

    if (E) ndi_free((void **) &E);
    if (P) ndi_free((void **) &P);
    return ierr;     
}

/*********************************************************************/

static int CALC_CHI_WGT(state_struct *state, char nu_type,
                        FLOAT_TYPE * nu_avg)
{

/* Purpose: Calculates the weight to give to chi when summing to form third chi.

Chi weight calculates nu_avg * sum over groups i ( sigf(i) * wgt(i) )

*/

    int ierr = 0;
    int n = 0;
    int num_elements;
    int curr_size;
    ndi_float64 *wgt_ptr;       /* Weights are stored as float64 */
    ndi_float64 *wgts;
    FLOAT_TYPE *nusigf = NULL;
    FLOAT_TYPE *nusigf_ptr = NULL;
    FLOAT_TYPE nu_sum = 0.0;

    if (!state) {
        return (-2);
    }


    if (ndi_nusigf_avail(state, nu_type) && state->iso_stat.wgts.file_ptr) {
        if (state->collapse_flag) {
            num_elements = state->grp_num_new;
        } else {
            num_elements = state->iso_stat.num_grps;
        }
        curr_size = num_elements;
        nusigf =
            (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));

        ierr = NDI_GET_NUSIGF(state, nu_type, nusigf, &curr_size);

        /* Check errors */
        nusigf_ptr = nusigf;
        wgts =
            (ndi_float64 *) ndi_malloc(num_elements * sizeof(ndi_float64));
        curr_size = num_elements;

        ierr =
            ndi_get_simple_float64_vec(state, state->lib_stream,
                                       state->iso_stat.wgts.file_ptr, 2,
                                       state->iso_stat.num_grps, wgts,
                                       num_elements);

        /* Check errors */
        wgt_ptr = wgts;
        for (n = 0; n < num_elements; n++) {
            nu_sum += *nusigf_ptr * *wgt_ptr;
            nusigf_ptr++;
            wgt_ptr++;
        }
        *nu_avg = nu_sum;
        if (nusigf)
            ndi_free((void **) &nusigf);
        if (wgts)
            ndi_free((void **) &wgts);
    } else {
        ierr = -9;
        *nu_avg = 0.0;
    }

    return (ierr);

}

/*********************************************************************/
/*********************************************************************/
/*  Purpose:  Return the appropriate nu*sigmaf (Nu * fission cross-section)
    for current particle.  Vector will be collapsed, if necessary.

    (1) First, look in library for appropriate nusigf data.
    (2) if other two nusigf's are available, calculate appropriate nusigf using:
    nusigf_tot = nusigf_del + nusigf_pr.
    (3) if nu and reaction MT=18 (total fission) are available,
    (or can be calculated), calulate using equation: 
    nu_sig_f = nu * sigma(MT=18). 


    Parameters:
    nu_type -- in:t=total, p=prompt, d=delayed
    return_vec -- out:array to fill with nu*sigf values.
    return_size -- in/out:on input shows the size of the array; on output
    shows the size remaining in array ( i.e. original return_size 
    minus the number of elements filled ). May be less zero 
    on exit.

    ******************************************************************** */
static int NDI_GET_NUSIGF(state_struct *state, char nu_type,
                          FLOAT_TYPE * return_vec, int *return_size)
{
    int ierr = 0;
    int jerr = 0;
    int n;
    int num_elements;
    int curr_size = *return_size;
    long int nu_sigf_ptr;
    long int nu_sigf1_ptr;
    long int nu_sigf2_ptr;
    FLOAT_TYPE *nu_sigf1_vec = NULL;
    FLOAT_TYPE *nu_sigf2_vec = NULL;
    FLOAT_TYPE *nu = NULL;
    FLOAT_TYPE *sigf = NULL;

    if (!state) NDI_THROW(ierr, -2); /* State not initialized */

    (void) NDI_INIT_FLOAT_VEC(return_vec, *return_size);

    switch (nu_type) {

    case 't':
        nu_sigf_ptr = state->iso_stat.nu_sig_f_tot.file_ptr;
        nu_sigf1_ptr = state->iso_stat.nu_sig_f_pr.file_ptr;
        nu_sigf2_ptr = state->iso_stat.nu_sig_f_del.file_ptr;
        break;

    case 'p':
        nu_sigf_ptr = state->iso_stat.nu_sig_f_pr.file_ptr;
        nu_sigf1_ptr = state->iso_stat.nu_sig_f_tot.file_ptr;
        nu_sigf2_ptr = state->iso_stat.nu_sig_f_del.file_ptr;
        break;

    case 'd':
        nu_sigf_ptr = state->iso_stat.nu_sig_f_del.file_ptr;
        nu_sigf1_ptr = state->iso_stat.nu_sig_f_tot.file_ptr;
        nu_sigf2_ptr = state->iso_stat.nu_sig_f_pr.file_ptr;
        break;

    default:
        NDI_THROW(ierr, -13);  /* Invalid argument */
    }

    if (state->iso_stat.fissionable == 0) goto catch;

    if (state->collapse_flag) {
        num_elements = state->grp_num_new;
    } else {
        num_elements = state->iso_stat.num_grps;
    }

    if (nu_sigf_ptr) {
        /* Read nu_sigf from library */
        /* Check if state->iso_stat.num_grps is appropriate */
        jerr = GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, nu_sigf_ptr,
                                    0, state->iso_stat.num_grps,
                                    return_vec, *return_size);
        if (jerr) NDI_THROW(ierr, jerr);

    } else if (nu_sigf1_ptr && nu_sigf2_ptr) {
        /* Calculate nu_sigf from other two nu_sigf's */
        nu_sigf1_vec =
            (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
        if (!nu_sigf1_vec) NDI_THROW(ierr, -14); /* Out of memory */

        nu_sigf2_vec =
            (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
        if (!nu_sigf2_vec) NDI_THROW(ierr, -14); /* Out of memory */

        curr_size = num_elements;
        jerr = GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, nu_sigf1_ptr,
                                    0, state->iso_stat.num_grps,
                                    nu_sigf1_vec, num_elements);
        if (jerr) NDI_THROW(ierr, jerr);

        curr_size = num_elements;
        jerr = GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, nu_sigf2_ptr,
                                    0, state->iso_stat.num_grps,
                                    nu_sigf2_vec, num_elements);
        if (jerr) NDI_THROW(ierr, jerr);

        for (n = 0; n < num_elements; n++) {
            /* Check bounds */
            if (n < *return_size) {
                if (nu_type == 't') {
                    return_vec[n] = nu_sigf1_vec[n] + nu_sigf2_vec[n];
                } else {
                    return_vec[n] = nu_sigf1_vec[n] - nu_sigf2_vec[n];
                }
            } else {
                /* Bounds check */
                ierr = 7;
                break;
            }
        }
    } else if (ndi_nu_avail(state, nu_type) && ndi_sigf_avail(state)) {
        /* Calculate nu_sigf from nu and MT=18 cross section */

        int collapse_flag;

        num_elements = state->iso_stat.num_grps;

        nu_sigf1_vec = (FLOAT_TYPE *) ndi_malloc(num_elements * 
                                                 sizeof(FLOAT_TYPE));
        if (!nu_sigf1_vec) NDI_THROW(ierr, -14); /* Out of memory */

        /* Get nu */
        nu = (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
        if (!nu) NDI_THROW(ierr, -14);    /* Out of memory */

        /* Turn off collapsing: get full nu */
        curr_size = num_elements;
        collapse_flag = state->collapse_flag;
        state->collapse_flag = 0;
        jerr = NDI_GET_NU(state, &state->iso_stat, nu_type, nu, &curr_size);
        state->collapse_flag = collapse_flag;
        if (jerr) NDI_THROW(ierr, jerr);

        /* Get sigf */
        sigf = (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
        if (!sigf) NDI_THROW(ierr, -14); /* Out of memory */

        /* Turn of collapsing: get fill sigf */
        curr_size = num_elements;
        collapse_flag = state->collapse_flag;
        state->collapse_flag = 0;
        jerr = GET_SIG_REAC(state, &state->iso_stat, 18, sigf, &curr_size);
        state->collapse_flag = collapse_flag;
        if (jerr) NDI_THROW(ierr, jerr);

        /* Calculate uncollapsed nu_sigf */
        for (n = 0; n < num_elements; n++) {
            nu_sigf1_vec[n] = nu[n] * sigf[n];
        }

        if (state->collapse_flag) {
            jerr = NDI_COLLAPSE_FLOAT(state, nu_sigf1_vec, num_elements, 
                                      0, return_vec, *return_size);
            if (jerr) NDI_THROW(ierr, jerr);
        } else {
            if (num_elements > *return_size) ierr = 7;  /* Data truncated */
            for (n = 0; n < ((num_elements < *return_size) ? 
                             num_elements: *return_size); n++) {
                return_vec[n] = nu_sigf1_vec[n];
            }
        }

    } else {
        ierr = 9;
        curr_size -= num_elements;
    }

 catch:
    ndi_free((void **) &nu);
    ndi_free((void **) &sigf);
    ndi_free((void **) &nu_sigf2_vec);
    ndi_free((void **) &nu_sigf1_vec);

    return ierr;
}

/*********************************************************************/
/*********************************************************************/
static int NDI_GET_NU(state_struct *state, isotope_status * mg,
                      char nu_type, FLOAT_TYPE * return_vec,
                      int *return_size)
{

    /*  Purpose:  Return the appropriate nu value (average number of particles
        released in fission as a function of incident energy) for particle 'mg'.

        (1) First, look in library for appropriate nu data.
        (2) if other two nu's are available, calculate appropriate nu using:
        nu_tot = nu_del + nu_pr.
        (3) if nu_sig_f and reaction MT=18 (total fission) are available,
        (or can be calculated), calulate using equation: 
        nu = nu_sig_f / sigma(MT=18). 


        Parameters:
        mg -- in:pointer to multigroup status structure, i.e. particle data.
        nu_type -- in:t=total, p=prompt, d=delayed
        return_vec -- out:array to fill with nu values.
        return_size -- in/out:on input shows the size of the array; on output
        shows the size remaining in array ( i.e. original return_size 
        minus the number of elements filled ). May be less zero 
        on exit.

        ******************************************************************** */

    int ierr = 0;
    int n;
    int num_grps;
    int curr_size = *return_size;
    long int nu_ptr;
    long int nu1_ptr;
    long int nu2_ptr;
    FLOAT_TYPE *nu1_vec = NULL;
    FLOAT_TYPE *nu2_vec = NULL;
    FLOAT_TYPE *nusigf = NULL;
    FLOAT_TYPE *sigf = NULL;


    switch (nu_type) {

    case 't':
        nu_ptr = mg->nu_tot.file_ptr;
        nu1_ptr = mg->nu_pr.file_ptr;
        nu2_ptr = mg->nu_del.file_ptr;
        break;

    case 'p':
        nu_ptr = mg->nu_pr.file_ptr;
        nu1_ptr = mg->nu_tot.file_ptr;
        nu2_ptr = mg->nu_del.file_ptr;
        break;

    case 'd':
        nu_ptr = mg->nu_del.file_ptr;
        nu1_ptr = mg->nu_tot.file_ptr;
        nu2_ptr = mg->nu_pr.file_ptr;
        break;

    default:
        ierr = -13;
        return (ierr);
        break;
    }

    if (!state) {
        return (-2);
    }

    if (state->collapse_flag) {
        num_grps = state->grp_num_new;
    } else {
        num_grps = mg->num_grps;
    }

    if (nu_ptr) {
        /* Read nu from library */
        /* Check if mg->num_grps is appropriate */
        ierr = GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, nu_ptr,
                                    0, mg->num_grps,
                                    return_vec, *return_size);
    } else if (nu1_ptr && nu2_ptr) {
        /* Calculate nu from other two nu's */
        nu1_vec = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
        nu2_vec = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
        curr_size = num_grps;
        ierr = GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, nu1_ptr,
                                    0, mg->num_grps, nu1_vec, num_grps);
        /* Check errors */
        curr_size = num_grps;
        ierr = GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, nu2_ptr,
                                    0, mg->num_grps, nu2_vec, num_grps);
        /* Check errors */
        for (n = 0; n < num_grps; n++) {
            /* Check bounds */
            if (nu_type == 't') {
                return_vec[n] = nu1_vec[n] + nu2_vec[n];
            } else {
                return_vec[n] = nu1_vec[n] - nu2_vec[n];
            }
        }
        if (nu1_vec)
            ndi_free((void **) &nu1_vec);
        if (nu2_vec)
            ndi_free((void **) &nu2_vec);

    } else if (ndi_nu_avail(state, nu_type) && ndi_sigf_avail(state)) {
        /* Calculate nu from nu_sigf and sigf */
        nusigf = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
        sigf = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
        curr_size = num_grps;
        ierr = NDI_GET_NUSIGF(state, nu_type, nusigf, &curr_size);
        /* Check errors */
        curr_size = num_grps;
        ierr = GET_SIG_REAC(state, mg, 18, sigf, &curr_size);
        /*Check errors */
        for (n = 0; n < num_grps; n++) {
            if (n < *return_size) {
                if (sigf[n] != 0) {
                    return_vec[n] = nusigf[n] / sigf[n];
                } else {
                    return_vec[n] = 0.0;
                }
            } else {
                /* Bounds check */
                ierr = 7;
                break;
            }
        }
        if (nusigf)
            ndi_free((void **) &nusigf);
        if (sigf)
            ndi_free((void **) &sigf);
    } else {
        /* No error or warning for no data--just return zeroes? */
        /* Set num_grps to either num_grps or grp_num_new */
        if (num_grps > *return_size) {
            /*  ierr = 7; */
            (void) NDI_INIT_FLOAT_VEC(return_vec, *return_size);
        } else {
            (void) NDI_INIT_FLOAT_VEC(return_vec, num_grps);
        }
        ierr = 9;
    }
    curr_size -= num_grps;
    return (ierr);
}

/*********************************************************************/
/*********************************************************************/
static int COPY_SGNDF_E(int num_grps, FLOAT_TYPE * return_vec,
                        int return_size)
{

    int ierr = 0;
    int i;
    ndi_float64 *eavg_p;
    ndi_float64 _16_lanl2_eavg[] =
        { 14.149974, 12.900168, 11.025994, 8.7271099, 6.8058861,
          4.5529926, 3.2272797, 1.9317643, 0.87432668, 0.32892994,
          0.11622681, 4.26904e-2, 1.567259e-2, 5.7608294e-3,
          1.4494968e-3, 2.8695381e-4
        };
    ndi_float64 _30_lanl_eavg[30] =
        { 15.741605, 14.069769, 12.900168, 11.025994, 8.7271099,
          6.8058861, 4.5529926, 3.2272797, 2.5168648, 1.9670423, 1.5314896,
          1.0662404, 0.65543265, 0.40027916, 0.23876023, 0.11622681,
          4.26904e-2, 1.567259e-2, 5.7608294e-3, 2.1196273e-3, 7.8051786e-4,
          2.8695381e-4, 1.0553574e-4, 3.8819478e-5, 1.4288539e-5,
          5.259767e-6,
          1.9375072e-6, 7.1294634e-7, 2.6175908e-7, 4.7190667e-8
        };

    if (num_grps == 16) {
        eavg_p = _16_lanl2_eavg;
    } else if (num_grps == 30) {
        eavg_p = _30_lanl_eavg;
    } else {
        ierr = -13;
        return (ierr);
    }
    for (i = 0; i < num_grps; i++) {
        if (i < return_size) {
            return_vec[i] = (FLOAT_TYPE) * eavg_p;
            eavg_p++;
        } else {
            ierr = 7;
            break;
        }
    }
    return (ierr);
}

/*********************************************************************/
/*********************************************************************/
static int NEUTRON_E_AVG (state_struct *state, int num_grps, FLOAT_TYPE *return_vec, 
                          int return_size) {

    int ierr;
    int calc_flag = 0;


    if (state->e_avg_calc == SGNDF) {
        if (ndi_use_sgndf_values(state, num_grps) != 0) {
            /* Use hard-wired values */
            ierr = COPY_SGNDF_E(num_grps, return_vec, return_size);
            return (ierr);
        } else {
            calc_flag = -1;     /* Calculate from average velocity */
        }
    }
    
    if ((state->e_avg_calc == VEL_CALC) || (calc_flag != 0)) {
        /* E(avg) = 1/2 m vel(avg)^2 */
        ierr = VELOCITY_CALC(state, num_grps, return_vec, return_size);

    } else if (state->e_avg_calc == LEGACY_VEL_CALC) {
        ierr = LEGACY_E_CALC(state, num_grps, return_vec, return_size);
        
    } else if (state->e_avg_calc == E_MEAN) {
        /*   Use bin midpoint energy */
        ierr = MIDPT_ENERGY(state, num_grps, return_vec, return_size);
        
    } else if (state->e_avg_calc == FLAT_CALC) {
        ierr = FLAT_ENERGY(state, num_grps, return_vec, return_size);
        
    } else if (state->e_avg_calc == TD_CALC) {
        ierr = TD_MOMENT(state, 'e', num_grps, return_vec, return_size);
        
    } else {
        ierr = -13;
    }

    return(ierr);
}

/*********************************************************************/
/*********************************************************************/
static int PHOTON_E_AVG (state_struct *state, int num_grps, 
                         FLOAT_TYPE *return_vec, int return_size) 
{
    return(MIDPT_ENERGY(state, num_grps, return_vec, return_size));
}

/*********************************************************************/
/*********************************************************************/
static int MIDPT_ENERGY (state_struct *state, int num_grps, 
                         FLOAT_TYPE *return_vec, int return_size) 
{
    /*   Arithmetic mean energy */

    int ierr = 0;
    int i, j;
    double wt_sum, e_sum;
    
    for ( i = 0; i < num_grps; i++ ) {
        wt_sum = 0;
        e_sum = 0;
        if ( state->collapse_flag ) {
            for ( j = state->new_ebounds[i]; j < state->new_ebounds[ i + 1 ] ; j++ ) {
                e_sum += (ndi_float64) fabs( (double) 
                                             state->lib_ebounds[j+1] * state->lib_ebounds[j+1] 
                                             - state->lib_ebounds[j] * state->lib_ebounds[j] ) 
                    * state->lib_weight[j];
                wt_sum += state->lib_weight[j] * fabs( state->lib_ebounds[j+1] - state->lib_ebounds[j] );
            }
        } else {
            e_sum = state->lib_ebounds[i+1] + state->lib_ebounds[i];
            wt_sum = 1;
        }
        return_vec[i] = (FLOAT_TYPE) e_sum /(2 *  wt_sum);
    }

    return(ierr);
}

/*********************************************************************/
/*********************************************************************/
static int VELOCITY_CALC(state_struct *state, int num_grps, FLOAT_TYPE *return_vec, 
                         int return_size) {
    /* Energy calculation from velocity */

    int ierr = 0;
    int i, j;
    int low_bound, up_bound;
    ndi_float64 *vel;
    double numer, denom;

    /* Assuming inc part = neutrons and that vel units are cm/shake */
    double v2_to_e_factor = (M_NEUT_KG) / 2 * 1e12 / (J_TO_MEV);
    
    
    if (fabs(v2_to_e_factor) < DBL_EPSILON) {
        ierr = 5;
        return (ierr);
    }
    
    vel = (ndi_float64 *) ndi_malloc(state->iso_stat.num_grps * sizeof(ndi_float64));
    ierr = ndi_get_simple_float64_vec(state, state->lib_stream,
                                      state->iso_stat.vel.file_ptr, -1,
                                      state->iso_stat.num_grps, vel, state->iso_stat.num_grps);
    for (i = 0; i < num_grps; i++) {
        if (i < return_size) {
            numer = 0.0;
            denom = 0.0;
            if (state->collapse_flag) {             /* Collapse */
                low_bound = state->new_ebounds[i];
                up_bound = state->new_ebounds[i+1];
            } else {
                low_bound = i;
                up_bound = i + 1;
            }
            for (j = low_bound; j < up_bound; j++) {
                numer += vel[j] * vel[j] * state->lib_weight[j];
                denom += state->lib_weight[j];
            }
            if (denom > (FLOAT_EPSILON * numer)) {
                return_vec[i] = (FLOAT_TYPE)v2_to_e_factor * (numer / denom);
            } else {
                ierr = 5;
                break;
            }
        } else {
            ierr = 7;
            break;
        }
    }
    if (vel)
        ndi_free((void **) &vel);
    

    return(ierr);
}

/*********************************************************************/
/*********************************************************************/
static int LEGACY_E_CALC(state_struct *state, int num_grps, FLOAT_TYPE *return_vec, 
                         int return_size) {
    /* Legacy energy calculation */

    int ierr = 0;
    int i,j;
    int low_bound, up_bound;
    ndi_float64 *vel;
    double numer, denom;

    /* Assuming inc part = neutrons and that vel units are cm/shake */
    double legacy_v2_to_e_factor = 0.0052271; /* Converts to MeV; originally hardwired by Eldon Linnebur? */
    
    vel = (ndi_float64 *)ndi_malloc(state->iso_stat.num_grps * sizeof(ndi_float64));
    ierr = ndi_get_simple_float64_vec( state, state->lib_stream, 
                                       state->iso_stat.vel.file_ptr,
                                       -1, state->iso_stat.num_grps,
                                       vel, state->iso_stat.num_grps );
    for (i = 0; i < num_grps; i++ ) {
        if (i < return_size) {
            numer = 0.0;
            denom = 0.0;
            if (state->collapse_flag) {             /* Collapse */
                low_bound = state->new_ebounds[i];
                up_bound = state->new_ebounds[i+1];
            } else {
                low_bound = i;
                up_bound = i + 1;
            }
            for (j = low_bound; j < up_bound; j++) {
                numer += vel[j] * state->lib_weight[j];
                if (vel[j] > (DBL_EPSILON * state->lib_weight[j])) {
                    denom += state->lib_weight[j] / vel[j];
                } else {
                    ierr = 5;
                    return(ierr);
                }
            }
            if (denom > (FLOAT_EPSILON * numer)) {
                return_vec[i] = (FLOAT_TYPE)legacy_v2_to_e_factor * (numer / denom);
            } else {
                ierr = 5;
                return(ierr);                
            }
        } else {
            ierr = 7;
        }
    }
    if (vel) ndi_free((void **)&vel);
    
    return(ierr);
}
/*********************************************************************/
/*********************************************************************/

/* Flat weighted average energy */
static int FLAT_ENERGY(state_struct *state, int num_grps, 
                       FLOAT_TYPE *return_vec, int return_size) 
{
    int ierr = 0;
    int i, j;
    double ebar, wt_sum, e_sum;
    
    for ( i = 0; i < num_grps; i++ ) {
        wt_sum = 0.0;
        e_sum = 0.0;
        if ( state->collapse_flag ) {
            for ( j = state->new_ebounds[i]; 
                  j < state->new_ebounds[i+1] ; j++ ) {
                ebar = (state->lib_ebounds[j+1] + state->lib_ebounds[j])/2.0;
                e_sum += ebar * state->lib_weight[j];
                wt_sum += state->lib_weight[j];
            }
        } else {
            e_sum = (state->lib_ebounds[i+1] + state->lib_ebounds[i])/2.0;
            wt_sum = 1;
        }
        return_vec[i] = (FLOAT_TYPE) e_sum / wt_sum;
    }

    return(ierr);
}
/*********************************************************************/
/*********************************************************************/
/* 10 point Gauss-Legendre Quadrature
   See: M. Abramowitz and I. A. Stegun,
   Handbook of Mathematical Functions
   Dover Publications (1972), p.916 */
static double gl10(double (*f)(double), double a, double b)
{
    static double z[] = {
        0.148874338981631, 
        0.433395394129247, 
        0.679409568299024,
        0.865063366688985,
        0.973906528517172
    };
    static double w[] = {
        0.295524224714753,
        0.269266719309996,
        0.219086362515982,
        0.149451349150581,
        0.066671344108688
    };

    int i;
    double xmid, xrange, dx, sum;

    xmid = (b+a)/2.0;
    xrange = (b-a)/2.0;
    sum = 0.0;

    for (i = 0; i < sizeof(z)/sizeof(z[0]); i++) {
        dx = xrange * z[i];
        sum += ((*f)(xmid+dx) + (*f)(xmid-dx)) * w[i];
    }
    return sum *= xrange;
}

/* The TD weight function.
   See: M. C. White
   The TD Weight Function Revisited 
   X-5:MCW-05-24(u)
   March 2005 */
static double td(double x)
{

    static double E[] = {              /* Energy grid (eV) */
        1.000000e-05, 1.390000e-04, 5.000000e-04, 1.000000e-03, 5.000000e-03,
        1.000000e-02, 2.500000e-02, 4.000000e-02, 5.000000e-02, 1.000000e-01,
        1.400000e-01, 1.500000e-01, 4.140000e-01, 1.130000e+00, 3.060000e+00,
        8.320000e+00, 2.260000e+01, 6.140000e+01, 1.670000e+02, 4.540000e+02,
        1.235000e+03, 3.350000e+03, 9.120000e+03, 2.480000e+04, 6.760000e+04,
        1.840000e+05, 3.030000e+05, 5.000000e+05, 8.230000e+05, 1.353000e+06,
        1.738000e+06, 2.232000e+06, 2.865000e+06, 3.680000e+06, 6.070000e+06,
        7.790000e+06, 1.000000e+07, 1.200000e+07, 1.300000e+07, 1.350000e+07,
        1.400000e+07, 1.410000e+07, 1.420000e+07, 1.430000e+07, 1.460000e+07,
        1.500000e+07, 1.600000e+07, 1.700000e+07, 2.000000e+07};

    static double w[] = {              /* weight @ E[i] */
        2.239100e+05, 3.019000e+06, 1.070000e+07, 2.098000e+07, 8.939000e+07,
        1.463800e+08, 2.008000e+08, 1.763500e+08, 1.478000e+08, 4.000000e+07,
        1.130000e+07, 7.600000e+06, 2.790000e+06, 1.020000e+06, 3.770000e+05,
        1.390000e+05, 5.110000e+04, 1.880000e+04, 6.910000e+03, 2.540000e+03,
        9.350000e+02, 3.450000e+02, 1.266000e+02, 4.650000e+01, 1.710000e+01,
        6.270000e+00, 3.880000e+00, 3.600000e+00, 2.870000e+00, 1.750000e+00,
        1.130000e+00, 7.300000e-01, 4.000000e-01, 2.050000e-01, 3.900000e-02,
        1.630000e-02, 6.500000e-03, 7.600000e-03, 1.230000e-02, 2.640000e-02,
        1.140000e-01, 1.140000e-01, 1.010000e-01, 6.500000e-02, 1.490000e-02,
        4.000000e-03, 1.540000e-03, 8.500000e-04, 1.727900e-04};

    static double lnw[] = {            /* log (w[i+1]/w[i])/log(E[i+1]/E[i]) */
         9.884296e-01, 9.884262e-01, 9.714039e-01, 9.005872e-01, 7.115331e-01,
         3.449816e-01,-2.762505e-01,-7.914665e-01,-1.885574e+00,-3.756853e+00,
        -5.749210e+00,-9.870729e-01,-1.002123e+00,-9.991121e-01,-9.975245e-01,
        -1.001403e+00,-1.000468e+00,-1.000303e+00,-1.000702e+00,-9.986448e-01,
        -9.991109e-01,-1.001001e+00,-1.001206e+00,-9.976161e-01,-1.001972e+00,
        -9.621974e-01,-1.495408e-01,-4.547460e-01,-9.951175e-01,-1.746723e+00,
        -1.746575e+00,-2.409498e+00,-2.670140e+00,-3.315940e+00,-3.496828e+00,
        -3.681218e+00, 8.575293e-01, 6.014927e+00, 2.023736e+01, 4.022351e+01,
         0.000000e+00,-1.713246e+01,-6.280423e+01,-7.094821e+01,-4.865451e+01,
        -1.478980e+01,-9.802970e+00,-9.802916e+00,-9.802916e+00};

    int l, u, m;

    x = x * 1.0e6;              /* convert MeV to eV */

    /* find x in E grid using binary search */

    l = 0;
    u = sizeof(E)/sizeof(E[0]) - 1;

    if (x < E[l]) {             /* off lower end */
        u = l;
    } else if (E[u] < x) {      /* off upper end */
        l = u;
    }
    while ((u-l) > 1) {         /* find x in table */
        m = (l+u)/2;
        if (E[m] < x) {
            l = m;
        } else if (E[m] == x) {
            l = m;
            u = m+1;
        } else if (E[m] > x) {
            u = m;
        }
    }

    /* return log-log interpolation */
    return w[l] * pow(x/E[l], lnw[l]);
}

static double tde(double x)
{
    return x * td(x);
}

static double tdv(double x)
{
    double vc;

    /* Since x in MeV, conversion is:
          J/MeV    *  (m/s)^2/J  * (cm/m)^2 * (s/sh)^2 */
    vc = J_TO_MEV  * 2/M_NEUT_KG *    1e4   *   1e-16; 
    /* to (cm/sh)^2 */

    return sqrt(vc * x) * td(x);
}

/* TD weighted average moment */
static int TD_MOMENT(state_struct *state, char m,  int num_grps, 
                     FLOAT_TYPE *return_vec, int return_size) 
{
    int ierr = 0;
    int i, j;
    double fbar, wt_sum, f_sum;

    double (*f)(double);

    switch (m) {
    case 'e':                   /* Energy average */
        f = tde;
        break;
    case 'v':                   /* Velocity average */
        f = tdv;
        break;
    default:                    /* Unrecognized average */
        return -5;
        break;
    }
    
    for (i = 0; i < num_grps; i++) {
        wt_sum = 0.0;
        f_sum = 0.0;
        if (state->collapse_flag) {
            for ( j = state->new_ebounds[i]; 
                  j < state->new_ebounds[i+1] ; j++ ) {
                fbar = gl10(f, state->lib_ebounds[j+1], 
                            state->lib_ebounds[j]) /
                    gl10(td, state->lib_ebounds[j+1], 
                         state->lib_ebounds[j]);

                f_sum += fbar * state->lib_weight[j];
                wt_sum += state->lib_weight[j];
            }
        } else {
            f_sum = gl10(f, state->lib_ebounds[i+1], state->lib_ebounds[i]) /
                gl10(td, state->lib_ebounds[i+1], state->lib_ebounds[i]);
            wt_sum = 1;
        }
        if (i < return_size) {
            return_vec[i] = (FLOAT_TYPE) f_sum / wt_sum;
        } else {
            ierr = 7;          /* array bounds exceeded */
            break;
        }
    }

    return(ierr);
}
/*********************************************************************/
/*********************************************************************/

/* Spontaneous fission neutrons per sh per energy bin */
static int GET_SF_SPECTRA(state_struct *state, FLOAT_TYPE *return_vec, 
                          int return_size) {

    int ierr = 0;
    int num_grps;
    int i;
    int n;
    double C;
    double R;
    double E;
    spont_fiss *sf;

    if (!state) NDI_THROW(ierr, -2); /* State not initialized */

    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);

    if (state->iso_stat.fissionable == 0) goto catch; /* Not fissionable */

    num_grps = (state->collapse_flag != 0) ? state->grp_num_new :
        state->iso_stat.num_grps;
    if (num_grps <= 0) NDI_THROW(ierr, -9);
    
    sf = ndi_get_sf_data(state->iso_stat.zaid); /* Lookup data */
    if (!sf) {
        ierr = 9;               /* Data not available */
        goto catch;
    }

    /* [decay/(atom*sh)][fissions/decay][neutrons/fission] */
    R = sf->lambda * sf->ratio * sf->nubar;

    if (return_size < num_grps) {
        n = return_size;
        ierr = 7;               /* Data truncated */
    } else {
        n = num_grps;
    }

    C = 1.0;                    /* Expand boundary to include all neutrons */
    for (i = 0; i < n; i++) {
        return_vec[i] = (FLOAT_TYPE) R*C; /* Upper boundary */

        E = state->lib_ebounds[(state->collapse_flag !=0) ? 
                               state->new_ebounds[i+1] : i+1];
        C = (i+1 != num_grps) ? ndi_int_watt(E, sf->a, sf->b): 0.0;

        return_vec[i] -= (FLOAT_TYPE) R*C; /* Lower boundary */
        if (return_vec[i] < 0.0) return_vec[i] = 0.0; /* negative fixup */
    }

 catch:
    return ierr;
}

/**********************************************************************/
int GET_QSIGF(state_struct *state, FLOAT_TYPE *vec, int size)
{
    int ierr = 0;
    int jerr = 0;
    int n;
    int i;
    int collapse_flag = 0;

    FLOAT_TYPE *qsf = NULL;
    FLOAT_TYPE *sigf = NULL;

    if (!state) NDI_THROW(ierr, -2); /* State not initialized */

    if (state->collapse_flag) {
        n = state->grp_num_new;
    } else {
        n = state->iso_stat.num_grps;
    }
    ierr = NDI_CONST_FLOAT_VEC(0.0, n, vec, size);
    if (state->iso_stat.fissionable == 0) goto catch; /* We're done */

    if (!ndi_sigf_avail(state)) NDI_THROW(ierr, -9); /* Data not available */

    /* Turn off collapsing 
       for best approximation of continuous product 
       with multigroup quantities */
    collapse_flag = state->collapse_flag;
    state->collapse_flag = 0;

    n = state->iso_stat.num_grps;
    /* Allocate and get fission Q */
    qsf = (FLOAT_TYPE *)ndi_malloc(n*sizeof(FLOAT_TYPE));
    if (!qsf) NDI_THROW(ierr, -14); /* Out of memory */
    jerr = GET_QF(state, qsf, n);
    if (jerr) NDI_THROW(ierr, jerr);

    /* Allocate and get fission cross section */
    sigf = (FLOAT_TYPE *)ndi_malloc(n*sizeof(FLOAT_TYPE));
    if (!sigf) NDI_THROW(ierr, -14); /* Out of memory */
    jerr = GET_SIG_REAC(state, &state->iso_stat, 18, sigf, &n);
    if (jerr) NDI_THROW(ierr, jerr);

    /* Calculate qsf */
    n = state->iso_stat.num_grps; /* May have been changed above */
    for (i = 0; i < n; i++) {
        qsf[i] *= sigf[i];
    }

    /* Restore collapsing */
    state->collapse_flag = collapse_flag;
    collapse_flag = 0;

    /* Collapse or copy result */
    if (state->collapse_flag) {
        jerr = NDI_COLLAPSE_FLOAT(state, qsf, n, 0, vec, size);
    } else {
        jerr = COPY_MATRIX(vec, size, qsf, n);
    }
    if (jerr) NDI_THROW(ierr, jerr);

 catch:
    if (collapse_flag) state->collapse_flag = collapse_flag;

    ndi_free((void **)&sigf);
    ndi_free((void **)&qsf);

    return ierr;
}

/**********************************************************************/
int GET_QF(state_struct *state, FLOAT_TYPE *vec, int size)
{
    int ierr = 0;
    int jerr = 0;
    int n;
    int i;
    int collapse_flag = 0;
    FLOAT_TYPE fissq[NUM_FISSION_Q];
    FLOAT_TYPE a;
    FLOAT_TYPE b;

    FLOAT_TYPE *nu = NULL;
    FLOAT_TYPE *eavg = NULL;
    FLOAT_TYPE *q = NULL;

    if (!state) NDI_THROW(ierr, -2); /* State not initialized */

    if (state->collapse_flag) {
        n = state->grp_num_new;
    } else {
        n = state->iso_stat.num_grps;
    }
    ierr = NDI_CONST_FLOAT_VEC(0.0, n, vec, size);
    if (state->iso_stat.fissionable == 0) goto catch; /* We're done */

    /* Get (constant) fission Q */
    jerr = GET_FLOAT_VEC_MG(state, NDI_FISS_Q, fissq, NUM_FISSION_Q);
    if (jerr) NDI_THROW(ierr, jerr);

    switch(state->qsigf_calc) {
        
    case Q_LEGACY:              /* Constant Q fission prompt */
        ierr = NDI_CONST_FLOAT_VEC(fissq[0], n, vec, size);
        goto catch;

    case P_CALC:                /* Q fission prompt in NJOY99 R0 */
        a = 8.07;
        b = 0.043;
        break;

    case Q_CALC:                /* Q fission prompt in NJOY99 R336 */
        a = 8.07;
        b = -0.307;
        break;

    default:
        NDI_THROW(ierr, -13);   /* Invalid flag */
    }

    if (!ndi_nu_avail(state, 'p')) NDI_THROW(ierr, -9); /* Data not avail */

    /* Turn off collapsing
       for best approximation of continuous nubar(0) 
       by multigroup nu[n-1] */
    collapse_flag = state->collapse_flag;
    state->collapse_flag = 0;

    n = state->iso_stat.num_grps;

    /* Allocate result */
    q = (FLOAT_TYPE *)ndi_malloc(n*sizeof(FLOAT_TYPE));
    if (!q) NDI_THROW(ierr, -14); /* Out of memory */

    /* Allocate and get average neutron incident energy per group */
    eavg = (FLOAT_TYPE *)ndi_malloc(n*sizeof(FLOAT_TYPE));
    if (!eavg) NDI_THROW(ierr, -14); /* Out of memory */
    /* NB: Values returned depend on NDI_E_AVG option setting */
    jerr = GET_FLOAT_VEC_MG(state,  NDI_E_AVG, eavg, n);
    if (jerr) NDI_THROW(ierr, jerr);

    /* Allocate and get average number of prompt neutrons emitted per group */
    nu = (FLOAT_TYPE *)ndi_malloc(n*sizeof(FLOAT_TYPE));
    if (!nu) NDI_THROW(ierr, -14); /* Out of memory */
    jerr = GET_FLOAT_VEC_MG(state, NDI_NU_PR, nu, n);
    if (jerr) NDI_THROW(ierr, jerr);

    /* Calculate incident neutron energy dependent prompt fission Q */
    /* See: "Incident Neutron Energy Dependent Fission Data in NDI"
       XCP-5:10-021(U), July 2010 */
    for (i = 0; i < n; i++)
        q[i] = fissq[0] - a * (nu[i] - nu[n-1]) - b * eavg[i];

    /* Restore collapsing */
    state->collapse_flag = collapse_flag;
    collapse_flag = 0;

    /* Collapse or copy result */
    if (state->collapse_flag) {
        jerr = NDI_COLLAPSE_FLOAT(state, q, n, 0, vec, size);
    } else {
        jerr = COPY_MATRIX(vec, size, q, n);
    }
    if (jerr) NDI_THROW(ierr, jerr);

 catch:
    if (collapse_flag) state->collapse_flag = collapse_flag;

    ndi_free((void **)&nu);
    ndi_free((void **)&eavg);
    ndi_free((void **)&q);

    return ierr;
}
/**********************************************************************/
/*********************************************************************/
