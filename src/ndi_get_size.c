
/*
 * File:        ndi_get_size.c
 * Description: User function--return the "size" of a library value.
 * Time-stamp:  <1999-11-15 11:28:34 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_bin_tree.h"

static int get_gendir_size(state_struct *state, int in_key, int *ierr);
static int get_mg_size(state_struct *state, int in_key, int *ierr);
static int get_tn_size(state_struct *state, int in_key, int *ierr);
static int get_tnreac_size(state_struct *state, int in_key, int *ierr);

/**********************************************************************/

int ndi_c_get_size_h(state_struct *state, int in_key, int *ierr)

/* Purpose: Return the size of the library value indicated by in_key.

Parameters:
in_key -- in: key to specify type of data required
ierr -- out: error flag, where 0=no error,pos=warning, neg=error

Returns: Returns the integer indicating the number of elements
in the array.  For single-valued data (temperature, sigma0, etc.)
"1" is returned.  For string data the size of the string (number of
characters plus the end of string character) is returned.  Returns 
zero as error.

**********************************************************************/
{
    int size = 0;

    /* Check that ndi_init has been called. */
    if ((!state) || (!state->ndi_initialized)) {
        *ierr = -2;
        return (size);
    }

    /* Special case--library type need not be set if asking for version. */
    if (in_key == NDI_VERSION) {
        size = (int) strlen(NDI_VERSION_DATA) + 1;
    } else if (in_key == NDI_GENDIR_PATH) {
        size = (int) strlen(state->open_gendir_file) + 1;
    } else {
        if (is_gendir(state)) {
            size = get_gendir_size(state, in_key, ierr);
        } else if ((state->library_type == 'm') || 
                   (state->library_type == 'd')) {
            size = get_mg_size(state, in_key, ierr);
        } else if (state->library_type == 's') {
            size = get_tn_size(state, in_key, ierr);
        } else if (state->library_type == 'r') {
            size = get_tnreac_size(state, in_key, ierr);
        } else if (state->library_type == 'p') {
            size = ndimp_get_size(state, in_key, ierr);
        } else if (state->library_type == ' ') {
            *ierr = -16;
            size = 0;
        } else {
            *ierr = -4;
            size = 0;
        }
    }


    if (size < 0) {
        size = 0;
        if (*ierr == 0) {
            *ierr = 9;
        }
    }
    return (size);
}


/**********************************************************************/
/**********************************************************************/

static int get_gendir_size(state_struct *state, int in_key, int *ierr)
{
    int return_value = 0;       /* default size */
    struct ndi_bin_tree_node *root = NULL;
    FILE *ndi_dir;
    char *path = NULL;
    char *in_value;

    *ierr = 0;                  /* default */

    if (!state) {               /* check state sanity */
        *ierr = -2;
        return(return_value);
    }

    in_value = ndi_get_section(state);
    if (in_value == 0) {
        *ierr = -16;            /* Library type not set */
        return(return_value);
    }
        
    /* Open gendir */
    *ierr = ndi_open_gendir(&ndi_dir, state->open_gendir_file, &path);
    if (path)
        ndi_free((void **) &path);

    if (*ierr < 0) {
        root = NULL;
    } else {
        /* Build a binary tree with ZAID as key */
        root = ndi_build_tree(ndi_dir, NDI_ZAID, in_value);
        fclose(ndi_dir);
    }

    switch (in_key) {
        /* String values--return size of string */
    case NDI_ZAID:              /*  ZAID  */
        *ierr = ndi_count_size(root, &return_value);
        break;
    case NDI_TEMP:              /* Temperature */
    case NDI_SIG_0:             /* sig_0 */
    case NDI_AT_WGT:            /* atomic weight */
    case NDI_AWR:               /* atomic weight ratio */
        *ierr = ndi_count_nodes(root, &return_value);
        break;
    default:      /**********  UNKNOWN KEYWORD  ***********/
        *ierr = -4;
        break;
    }
    ndi_close_tree(&root);
    return(return_value);
}

/**********************************************************************/
static int get_mg_size(state_struct *state, int in_key, int *ierr)
{
    int return_value = 0;       /* Default return value is '0' */
    char tmp_str[MAX_IN_STR_LEN];

    if (!state) {
        *ierr = -2;
        return (0);
    }

    *ierr = 0;                  /* Default error value */

    /* Check that isotope is set */
    if ((in_key != NDI_DEPL_ZAID) && (in_key != NDI_DEPL_INFO)
        && (in_key != NDI_NUM_TARGET)) {
        /* Special case for depletion -- isotope need not be set */
        if (!state->iso_stat.set_flag) {
            *ierr = -7;
            return (0);
        }
    }

    switch (in_key) {

        /*************************/
        /*   Single-valued data  */
        /*************************/

        /* String values--return size of string */
    case NDI_ZAID:              /*  ZAID  */
        return_value = (int) strlen(state->iso_stat.zaid) + 1;
        break;
    case NDI_INFO:              /* Information */
        return_value = (int) strlen(state->iso_stat.info) + 1;
        break;
    case NDI_DATE_PROCESSED:    /* Date library was processed */
        return_value = (int) strlen(state->iso_stat.date_processed) + 1;
        break;
    case NDI_DATE_SOURCE:       /* Date of library source info */
        return_value = (int) strlen(state->iso_stat.date_source) + 1;
        break;
    case NDI_LIBRARY_NAME:      /* Name of current library */
        return_value = (int) strlen(state->iso_stat.library_name) + 1;
        break;
    case NDI_DEPL_ZAID: /*  Depletion ZAID  */
    case NDI_CHAIN:
    case NDI_DEPLETION:
        if (!state->ndi_depl_set) {
            *ierr = ndi_set_default_depl(state);
        }
        return_value = (int) strlen(state->depl_dataset) + 1;
        break;
    case NDI_DEPL_INFO: /* Depletion info */
        if (!state->ndi_depl_set) {
            *ierr = ndi_set_default_depl(state);
        }
        return_value = (int) strlen(state->depl_info) + 1;
        break;

    case NDI_SEC_PART_TYPES:    /* Secondary particle types  */
        if ((state->iso_stat.sec_part) &&
            (state->curr_part_index < state->iso_stat.num_sec_parts) &&
            (state->curr_part_index >= 0)) {
            sprintf(tmp_str, "%d",
                    state->iso_stat.sec_part[state->curr_part_index].
                    part_type);
            return_value = (int) strlen(tmp_str) + 1;
        } else {
            if (state->iso_stat.num_sec_parts >= state->curr_part_index) {
                /* Invalid state->curr_part_index */
                *ierr = -5;
            } else {
                /* No secondary particle data for this particle */
                *ierr = 9;
            }
        }
        break;
    case NDI_SEC_PART_ZAIDS:    /* Secondary particle ZAIDs  */
        if ((state->iso_stat.sec_part) &&
            (state->curr_part_index < state->iso_stat.num_sec_parts) &&
            (state->curr_part_index >= 0)) {
            return_value =
                (int) strlen(state->iso_stat.
                             sec_part[state->curr_part_index].zaid) + 1;
        } else {
            if (state->iso_stat.num_sec_parts >= state->curr_part_index) {
                /* Invalid curr_part_index  */
                *ierr = -5;
            } else {
                /* No secondary particle data for this particle */
                *ierr = 9;
            }
        }
        break;

        /* Single values--return "1" */
    case NDI_NUM_GRPS:          /* Number of groups */
    case NDI_TEMP:              /* Temperature */
    case NDI_SIG_0:             /* sig_0 */
    case NDI_UP_GRPS:           /* upscatter_groups */
    case NDI_DOWN_GRPS: /* downscatter_groups */
    case NDI_AT_WGT:            /* atomic weight */
    case NDI_AWR:               /* atomic weight ratio */
    case NDI_NUM_REAC:          /* number of edits */
    case NDI_NUM_RPRODS:        /* number of reaction products */
    case NDI_PN_ORDER:          /* number of scattering matrices */
    case NDI_NUM_SEC_PARTS:     /* number of secondary particles */
    case NDI_NUM_GRPS_CURR_PART:        /* number of groups -- current particle */
    case NDI_PN_ORDER_CURR_PART:        /* number of production matrices--current particle */
    case NDI_NUM_TARGET:        /* number of chain targets */
    case NDI_LAST_GRP:          /* Kludged ngk last neutron group */
    case NDI_FISSIONABLE:       /* Boolean: fission data? */
    case NDI_SPONTANEOUS_FISSIONABLE:  /* Boolean: spontaneous fission data? */
    case NDI_ATOMIC_NUM:        /* Z of the current zaid */
    case NDI_MASS_NUM:          /* A of the current zaid */
    case NDI_NUM_PNU_PR:        /* number of P(nu) vectors available */
        return_value = 1;
        break;

        /*************************/
        /*  Simple array values  */
        /*************************/


        /*  Arrays with num_grps number of elements */

    case NDI_E_AVG:             /* Average group energies */
    case NDI_ENERGY_N:          /* Energy given to neutrons */
    case NDI_ENERGY_AVAIL:      /* Energy available */
    case NDI_WGTS:              /* weights */
    case NDI_VEL:               /* Velocities */
    case NDI_HEATING:           /* Heating */
    case NDI_KERMA:             /* kerma */
    case NDI_SIG_TOT:           /* Sigma total */
    case NDI_NU_TOT:            /* nubar total */
    case NDI_NU_PR:             /* nubar prompt */
    case NDI_NU_DEL:            /* nubar delayed */
    case NDI_NU_SIG_F_TOT:      /* nubar total sigma fission */
    case NDI_NU_SIG_F_PR:       /* nubar prompt sigma fission */
    case NDI_NU_SIG_F_DEL:      /* nubar delayed sigma fission */
    case NDI_CHI_VEC_TOT:       /* chi total vector */
    case NDI_CHI_VEC_PR:        /* chi prompt vector */
    case NDI_CHI_VEC_DEL:       /* chi delayed vector */
    case NDI_HEATING_CURR_PART: /* Partial heating--current particle */
    case NDI_KERMA_CURR_PART:   /* Partial kerma--current particle */
    case NDI_TOT_PROD_XS:       /* Total production cross section */
    case NDI_TOT_DEPL_XS:       /* Total depletion cross section */
    case NDI_SF_SPECTRA:        /* Spontaneous fission spectra */
    case NDI_V_AVG:             /* Average group velocities */

        if (state->collapse_flag != 0) {
            return_value = state->grp_num_new;
        } else {
/*     if(state->iso_stat.wgts.length)  */
            return_value = state->iso_stat.num_grps;
        }
        break;

        /* Arrays with element number not equal to number of groups */


    case NDI_E_BOUNDS:
        /* energy boundaries */
        if (state->collapse_flag != 0) {
            return_value = state->grp_num_new + 1;
        } else {
/*       if(state->iso_stat.e_bounds.length)  */
            return_value = state->iso_stat.num_grps + 1;
        }
        break;

    case NDI_NORM:
        /* normalization factor */
/*       if(state->iso_stat.norm.length)  */
        return_value = NUM_NORM;
        break;

    case NDI_FISS_Q:
        /* fission q */
/*       if(state->iso_stat.fiss_q.length)  */
        return_value = NUM_FISSION_Q;
        break;

    case NDI_REAC_MT:
        /* edit reaction mt */
/*    if (state->iso_stat.reac_mt)  */
        return_value = state->iso_stat.num_reac;
        if (return_value < 0) {
            return_value = 0;
        }
        break;

    case NDI_REAC_Q:
        /*edit reaction q */
/*       if (state->iso_stat.reac_q)  */
        return_value = state->iso_stat.num_reac;
        if (return_value < 0) {
            return_value = 0;
        }
        break;

    case NDI_E_BOUNDS_CURR_PART:
        /* Energy boundaries for current particle */
/*   if(state->iso_stat.e_bounds_p.length)  */
        if ((state->iso_stat.sec_part) &&
            (state->curr_part_index < state->iso_stat.num_sec_parts) &&
            (state->curr_part_index >= 0)) {
            if (state->iso_stat.sec_part[state->curr_part_index].
                collapse_flag == 0) {
                return_value =
                    state->iso_stat.sec_part[state->curr_part_index].
                    num_grps + 1;
            } else {
                return_value =
                    state->iso_stat.sec_part[state->curr_part_index].
                    grp_num_new + 1;
            }
        } else {
            *ierr = 9;
        }
        break;

    case NDI_TEMPS:             /* multi-temperature (Reqt. 22) */
    {
        char sza[MAX_TN_REAC_LEN];

        if (!state->ndi_dir) {
            *ierr = -2;
            return (0);
        }

        sprintf(sza, "%d.", atoi(state->iso_stat.zaid));
        return_value = ndi_count_dir_block(state->ndi_dir, NDI_TEMP, 
                                           ndi_get_section(state), 
                                           state->library_vers, sza);
        if (return_value < 0) {
            *ierr = return_value;
            return_value = 0;
        }
    }
    break;

    /****************************/
    /*  Multi-dim array values  */
    /****************************/

    case NDI_RPROD_ALL: /* old reaction product format -- all */
        return_value = state->iso_stat.rprod_all.num_objs;
        break;

    case NDI_RPROD_RMO: /* sigma edit reaction products -- rmo */
        return_value = state->iso_stat.rprod_rmo.num_objs;
        break;

    case NDI_RPROD_FEW:
        /* sigma edit reaction products -- few */
        return_value = state->iso_stat.rprod_few.num_objs;
        break;

    case NDI_CHI_MAT_TOT:       /* chi total matrix */
    case NDI_CHI_MAT_PR:        /* chi prompt matrix */
    case NDI_CHI_MAT_DEL:       /* chi delayed matrix */
    case NDI_FISS_XFER_MAT_TOT:
    case NDI_FISS_XFER_MAT_PR:
    case NDI_FISS_XFER_MAT_DEL:

        if (state->collapse_flag != 0) {
            return_value = state->grp_num_new * state->grp_num_new;
        } else {
            return_value =
                state->iso_stat.num_grps * state->iso_stat.num_grps;
        }
/*       } */
        break;


        /***********  UNKNOWN KEYWORD  **************/

    default:
        *ierr = -4;
        return_value = 0;
        break;
    }

    return (return_value);
}

/**********************************************************************/
/**********************************************************************/


static int get_tn_size(state_struct *state, int in_key, int *ierr)
{


    int return_value = 0;       /* Default return value is '0' */

    *ierr = 0;                  /* Default error value */

    if (!state) {
        *ierr = -2;
        return (0);
    }

    /* Check that isotope is set */
    if (!state->tn_stat.set_flag) {
        *ierr = -7;
        return (0);
    }

    switch (in_key) {

        /*************************/
        /*   Single-valued data  */
        /*************************/

        /* String values--return size of string */
    case NDI_ZAID:              /*  ZAID  */
        return_value = (int) strlen(state->tn_stat.zaid) + 1;
        break;
    case NDI_INFO:              /* Information */
        return_value = (int) strlen(state->tn_stat.info) + 1;
        break;
    case NDI_DATE_PROCESSED:    /* Date library was processed */
        return_value = (int) strlen(state->tn_stat.date_processed) + 1;
        break;
    case NDI_DATE_SOURCE:       /* Date of library source info */
        return_value = (int) strlen(state->tn_stat.date_source) + 1;
        break;
    case NDI_LIBRARY_NAME:      /* Name of current library */
        return_value = (int) strlen(state->tn_stat.library_name) + 1;
        break;

        /* Single values--return "1" */
    case NDI_NUM_TEMPS: /* Number of sigma tempuratures */
    case NDI_NUM_SIGVBAR_INTERP:        /* Number of sigvbar interpolation regions */
    case NDI_NUM_EINBAR_INTERP: /* Number of einbar interpolation regions */
    case NDI_NUM_SEC_PARTS:     /* Number of sec particles w/energy info  */
    case NDI_NUM_REAC:          /* Number of reactions  */
    case NDI_NUM_RPRODS:        /* Number of reaction products */
    case NDI_NUM_REACTANTS:     /* Number of reactants */
    case NDI_REAC_Q:            /* Q-value for current reaction */
    case NDI_NUM_GRPS:          /* Number of energy grps used in edist of sec. parts */
    case NDI_EAVG_OUT:          /* Average energy of outgoing secondary particle */
    case NDI_EOUTBAR:           /* Average energy of outgoing secondary particle */
        return_value = 1;
        break;


        /*************************/
        /*  Simple array values  */
        /*************************/

    case NDI_TEMPS:
        /* temperatures */
        return_value = state->tn_stat.num_temps;
        break;

    case NDI_SIGVBARS:
        /* sigma vbars */
        return_value = state->tn_stat.num_temps;
        break;

    case NDI_RPRODS_ID:
        /* reaction product ids */
        if (state->tn_stat.rprods.length)
            return_value = state->tn_stat.rprods.length;
        break;

    case NDI_RPRODS_MLT:
        /* reaction product multiplicities */
        if (state->tn_stat.rprods.length)
            return_value = state->tn_stat.rprods.length;
        break;

    case NDI_REACTANTS_ID:
        /* reactant ids */
        if (state->tn_stat.reactants.length)
            return_value = state->tn_stat.reactants.length;
        break;

    case NDI_REACTANTS_MLT:
        /* reactant ids */
        if (state->tn_stat.reactants.length)
            return_value = state->tn_stat.reactants.length;
        break;

    case NDI_SIGVBAR_INTERP_REG:
        if (state->tn_stat.int_sigvbars.length)
            return_value = state->tn_stat.int_sigvbars.length;
        break;

    case NDI_SIGVBAR_INTERP_TYPE:
        if (state->tn_stat.int_sigvbars.length)
            return_value = state->tn_stat.int_sigvbars.length;
        break;

    case NDI_EINBAR_INTERP_REG:
        if (state->tn_stat.int_einbars.length)
            return_value = state->tn_stat.int_einbars.length;
        break;

    case NDI_EINBAR_INTERP_TYPE:
        if (state->tn_stat.int_einbars.length)
            return_value = state->tn_stat.int_einbars.length;
        break;

    case NDI_SEC_PART_TYPES:
        /* Vector of secondary particle types */
        if (state->tn_stat.num_sec_parts >= 0) {
            return_value = state->sec_part_filled;
        }

    case NDI_EDIST:             /* Curr. particle's energy distribution at current temp. */
        /* The number returned is the number of groups in the currently-set group stucture */
        return_value = state->grp_num_new;
        break;

    case NDI_EDIST_INTERP_REG:
    case NDI_EDIST_INTERP_TYPE:
        /* Vector of interpolation type for current particle's temperature
           interpolation regions */
        if (state->grp_num_new <= 0) return (-21);
        if ((state->curr_part_index >= 0) ||
            (state->curr_part_index < state->sec_part_filled)) {
            sec_part_distr *part;
              
            part = &(state->tn_stat.sec_part[state->curr_part_index]);
            if (part) {
                return_value = part->temp_interp.nint;
            } else {
                return (-30);
            }
        } else {
            return (-30);
        }

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
                return_value = part->temp_interp.npts;
            } else {
                return (-30);
            }
        } else {
            return (-30);
        }

        break;

    case NDI_EDIST_CONT_E:
    case NDI_EDIST_CONT_P:
        if (state->grp_num_new <= 0) return (-21);
        if ((state->curr_part_index >= 0) ||
            (state->curr_part_index < state->sec_part_filled)) {
            sec_part_distr *part;
              
            part = &(state->tn_stat.sec_part[state->curr_part_index]);
            if (part) {
                int hi, lo, it;

                *ierr = ndi_find_temp_bound_index(state->default_temp,
                                                  part->edist, 
                                                  part->temp_interp.npts, 
                                                  &hi, &lo, &it);
                if (*ierr < 0) return -30;

                return_value = part->edist[it].interp.npts;
            } else {
                return (-30);
            }
        } else {
            return (-30);
        }

        break;

    case NDI_EDIST_CONT_INTERP_REG:
    case NDI_EDIST_CONT_INTERP_TYPE:
        if (state->grp_num_new <= 0) return (-21);
        if ((state->curr_part_index >= 0) ||
            (state->curr_part_index < state->sec_part_filled)) {
            sec_part_distr *part;
              
            part = &(state->tn_stat.sec_part[state->curr_part_index]);
            if (part) {
                int hi, lo, it;

                *ierr = ndi_find_temp_bound_index(state->default_temp,
                                                  part->edist, 
                                                  part->temp_interp.npts, 
                                                  &hi, &lo, &it);
                if (*ierr < 0) return -30;

                return_value = part->edist[it].interp.nint;
            } else {
                return (-30);
            }
        } else {
            return (-30);
        }

        break;

    case NDI_EINBAR:
        /* thermal-averaged total incoming energy for each temp. */
        return_value = state->tn_stat.num_temps;
        break;

    case NDI_E_BOUNDS:          /*  Multigroup energy boundaries for sec. part distr. */
        return_value = state->grp_num_new + 1;
        break;

    case NDI_E_AVG:             /* Mean of each multigroup bin used for sec. part edist */
        return_value = state->grp_num_new;
        break;

        /***********  UNKNOWN KEYWORD  **************/

    default:
        *ierr = -4;
        return_value = 0;
        break;
    }

    if (return_value < 0) {
        return_value = 0;
    }
    return (return_value);
}

/**********************************************************************/
/**********************************************************************/

static int get_tnreac_size(state_struct *state, int in_key, int *ierr)
{


    int return_value = 0;       /* Default return value is '0' */

    *ierr = 0;                  /* Default error value */

    if (!state) {
        *ierr = -2;
        return (0);
    }
    /* Check that isotope is set */
    if (state->tnreac_stat.reac_num > 0) {
        *ierr = -7;
        return (0);
    }

    switch (in_key) {

        /*************************/
        /*   Single-valued data  */
        /*************************/

        /* String values--return size of string */
    case NDI_ZAID:              /*  ZAID  */
        return_value = (int) strlen(state->tnreac_stat.dataset) + 1;
        break;
    case NDI_INFO:              /* Information */
        return_value = (int) strlen(state->tnreac_stat.info) + 1;
        break;
    case NDI_DATE_PROCESSED:    /* Date library was processed */
        return_value = (int) strlen(state->tnreac_stat.date_processed) + 1;
        break;
    case NDI_DATE_SOURCE:       /* Date of library source info */
        return_value = (int) strlen(state->tnreac_stat.date_source) + 1;
        break;
    case NDI_LIBRARY_NAME:      /* Name of current library */
        return_value = (int) strlen(state->tnreac_stat.library_name) + 1;
        break;

        /* Single values--return "1" */
    case NDI_NUM_REAC:          /* Number of reactions */
        return_value = 1;
        break;

        /***********  UNKNOWN KEYWORD  **************/

    default:
        *ierr = -4;
        return_value = 0;
        break;
    }

    return (return_value);
}

/**********************************************************************/
/**********************************************************************/
