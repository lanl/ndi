/*
 * File:	ndi_get_float_vec.c
 * Description:	Source for libndi.a
 * Time-stamp:	<02/08/29 09:04:01 jomc>
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

static int GET_FLOAT_VEC_GENDIR(state_struct *state, int in_key,
			    FLOAT_TYPE * return_vec, int return_size);
static int GET_FLOAT_VEC_MG(state_struct *state, int in_key,
			    FLOAT_TYPE * return_vec, int return_size);
static int GET_FLOAT_VEC_TN(state_struct *state, int in_key,
			    FLOAT_TYPE * return_vec, int return_size);
static int GET_CHI_MAT(state_struct *state, FILE *stream, long mat_ptr,
		       long vec_ptr, FLOAT_TYPE * return_mat, int num_grps,
		       int return_size);
static int GET_E_BOUNDS_CURR_PART(state_struct *state,
				  FLOAT_TYPE * return_vec,
				  int return_size);
static int SIG_TOT_TRANS_CORR(state_struct *state,
			      FLOAT_TYPE * return_vec, int return_size);
static int NDI_CALC_E_AVG(state_struct *state, FLOAT_TYPE * return_vec,
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
static int NDI_GET_CHI_VEC(state_struct *state, isotope_status * mg,
			   char chi_type, FLOAT_TYPE * return_vec,
			   int *return_size);
static int GET_DEPL_XS(state_struct *state, isotope_status * mg,
		       FLOAT_TYPE * return_vec, int return_size);
static int GET_EDIST_VEC(state_struct *state, FILE *lib_stream,
			 FLOAT_TYPE * return_vec, int return_size);
static int CALC_CHI_WGT(state_struct *state, char nu_type,
			FLOAT_TYPE * nu_avg);
static int NDI_GET_NUSIGF(state_struct *state, char nu_type,
			  FLOAT_TYPE * return_vec, int *return_size);
static int NDI_GET_NU(state_struct *state, isotope_status * mg,
		      char nu_type, FLOAT_TYPE * return_vec,
		      int *return_size);
static int COPY_SGNDF_E(int num_grps, FLOAT_TYPE * return_vec,
			int return_size);
static int NEUTRON_E_AVG (state_struct *state, int num_grps, FLOAT_TYPE *return_vec, 
                          int return_size);
static int PHOTON_E_AVG (state_struct *state, int num_grps, FLOAT_TYPE *return_vec, 
                         int return_size);
static int MIDPT_ENERGY (state_struct *state, int num_grps, FLOAT_TYPE *return_vec, 
                         int return_size);
static int VELOCITY_CALC(state_struct *state, int num_grps, FLOAT_TYPE *return_vec, 
                         int return_size);
static int LEGACY_E_CALC(state_struct *state, int num_grps, FLOAT_TYPE *return_vec, 
                         int return_size);

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

    if (!state) {		/* check state sanity */
	return (-2);
    }

    in_value = ndi_get_section(state);
    if (in_value == 0) {
	return(-16);		/* Library type not set */
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

    case NDI_E_BOUNDS:		/*  Energy boundaries  */
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

    case NDI_E_AVG:		/* Average group energies */
	/*  Check bounds */
	ierr = NDI_CALC_E_AVG(state, return_vec, return_size);
	break;

    case NDI_ENERGY_N:		/* Energy given to neutrons */
	/*  Check bounds */
	ierr = NDI_CALC_E_N(state, return_vec, return_size);
	break;

    case NDI_ENERGY_AVAIL:	/* Energy available */
	/*  Check bounds */
	ierr = NDI_CALC_E_AVAIL(state, return_vec, return_size);
	break;

    case NDI_WGTS:		/* Weights */
	num_elements = state->iso_stat.num_grps;
	/*  Check bounds */
	ierr =
	    GET_SIMPLE_FLOAT_VEC(state, state->lib_stream,
				 state->iso_stat.wgts.file_ptr, 3,
				 num_elements, return_vec, return_size);
	break;

    case NDI_VEL:		/* Velocities */
	num_elements = state->iso_stat.num_grps;
	/*  Check bounds */
	ierr =
	    GET_SIMPLE_FLOAT_VEC(state, state->lib_stream,
				 state->iso_stat.vel.file_ptr, 1,
				 num_elements, return_vec, return_size);
	break;

    case NDI_HEATING:		/* Heating */
	if (state->kerma_calc == CALC) {
	    ierr = NDI_GET_OLD_HEATING(state, return_vec, return_size);
	} else {
	    ierr =
		NDI_GET_HEATING(state, &state->iso_stat.heat,
				&state->iso_stat.kerma, return_vec,
				return_size);
	}
	break;

    case NDI_KERMA:		/* kerma */
	/* Units are MeV*barn -- this is just heating * tot_sigma */
	if (state->kerma_calc == CALC) {
	    ierr = NDI_GET_OLD_KERMA(state, return_vec, return_size);
	} else {
	    ierr =
		NDI_GET_KERMA(state, &state->iso_stat.kerma,
			      &state->iso_stat.heat, return_vec,
			      return_size);
	}
	break;

    case NDI_HEATING_CURR_PART:	/* Partial heating for current particle */
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

    case NDI_KERMA_CURR_PART:	/* Partial kerma for current particle */
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

    case NDI_SIG_TOT:		/* Sigma total */
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

    case NDI_NU_TOT:		/* nubar total */
	num_elements = return_size;
	ierr =
	    NDI_GET_NU(state, &state->iso_stat, 't', return_vec,
		       &num_elements);
	(void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
	break;

    case NDI_NU_PR:		/* nubar prompt */
	num_elements = return_size;
	ierr =
	    NDI_GET_NU(state, &state->iso_stat, 'p', return_vec,
		       &num_elements);
	(void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
	break;

    case NDI_NU_DEL:		/* nubar delayed */
	num_elements = return_size;
	ierr =
	    NDI_GET_NU(state, &state->iso_stat, 'd', return_vec,
		       &num_elements);
	(void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
	break;

    case NDI_NU_SIG_F_TOT:	/* nubar total sigma fission */
	num_elements = return_size;
	ierr = NDI_GET_NUSIGF(state, 't', return_vec, &num_elements);
	(void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
	break;

    case NDI_NU_SIG_F_PR:	/* nubar prompt sigma fission */
	num_elements = return_size;
	ierr = NDI_GET_NUSIGF(state, 'p', return_vec, &num_elements);
	(void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
	break;

    case NDI_NU_SIG_F_DEL:	/* nubar delayed sigma fission */
	num_elements = return_size;
	ierr = NDI_GET_NUSIGF(state, 'd', return_vec, &num_elements);
	(void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
	break;

    case NDI_CHI_VEC_TOT:	/* chi total vector */
	num_elements = return_size;
	/*  Check bounds */
	ierr =
	    NDI_GET_CHI_VEC(state, &state->iso_stat, 't', return_vec,
			    &num_elements);
	(void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
	break;

    case NDI_CHI_VEC_PR:	/* chi prompt vector */
	num_elements = return_size;
	/*  Check bounds */
	ierr =
	    NDI_GET_CHI_VEC(state, &state->iso_stat, 'p', return_vec,
			    &num_elements);
	(void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
	break;

    case NDI_CHI_VEC_DEL:	/* chi delayed vector */
	num_elements = return_size;
	/*  Check bounds */
	ierr =
	    NDI_GET_CHI_VEC(state, &state->iso_stat, 'd', return_vec,
			    &num_elements);
	(void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
	break;

    case NDI_CHI_MAT_TOT:	/* chi total matrix */
	num_elements = state->iso_stat.num_grps;
	/*  Check bounds */
	ierr =
	    GET_CHI_MAT(state, state->lib_stream,
			state->iso_stat.chi_mat_tot.file_ptr,
			state->iso_stat.chi_vec_tot.file_ptr, return_vec,
			num_elements, return_size);
	if (ierr == -9) {
	    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);
	    ierr = 9;
	}
	(void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
	break;

    case NDI_CHI_MAT_PR:	/* chi prompt matrix */
	num_elements = state->iso_stat.num_grps;
	/*  Check bounds */
	ierr =
	    GET_CHI_MAT(state, state->lib_stream,
			state->iso_stat.chi_mat_pr.file_ptr,
			state->iso_stat.chi_vec_pr.file_ptr, return_vec,
			num_elements, return_size);
	if (ierr == -9) {
	    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);
	    ierr = 9;
	}
	(void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
	break;

    case NDI_CHI_MAT_DEL:	/* chi delayed matrix */
	num_elements = state->iso_stat.num_grps;
	/*  Check bounds */
	ierr =
	    GET_CHI_MAT(state, state->lib_stream,
			state->iso_stat.chi_mat_del.file_ptr,
			state->iso_stat.chi_vec_del.file_ptr, return_vec,
			num_elements, return_size);
	if (ierr == -9) {
	    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);
	    ierr = 9;
	}
	(void) check_fissionable_err(&ierr, state->iso_stat.fissionable);
	break;

    case NDI_NORM:		/* Normalization factors */
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

    case NDI_FISS_Q:		/* fission q */
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

    case NDI_REAC_Q:		/* edit reaction q */
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

    case NDI_E_BOUNDS_CURR_PART:	/* secondary particle energy boundaries */
	/*  Check bounds */
	ierr = GET_E_BOUNDS_CURR_PART(state, return_vec, return_size);
	break;

    case NDI_TOT_DEPL_XS:
	if (!state->ndi_depl_set) {
	    ierr = ndi_set_default_depl(state);
	}

	ierr =
	    GET_DEPL_XS(state, &state->iso_stat, return_vec, return_size);
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

    case NDI_TEMPS:		/* Temperatures */
	num_elements = state->tn_stat.num_temps;
	ierr =
	    GET_SIMPLE_FLOAT_VEC(state, state->lib_stream,
				 state->tn_stat.temps.file_ptr, -1,
				 num_elements, return_vec, return_size);
	break;

    case NDI_SIGVBARS:		/* Sigma Vbars */
	num_elements = state->tn_stat.num_temps;
	ierr =
	    GET_SIMPLE_FLOAT_VEC(state, state->lib_stream,
				 state->tn_stat.sigvbars.file_ptr, -1,
				 num_elements, return_vec, return_size);
	break;

    case NDI_EINBAR:		/* Thermal-average of total inc. energy */
	num_elements = state->tn_stat.num_temps;
	ierr =
	    GET_SIMPLE_FLOAT_VEC(state, state->lib_stream,
				 state->tn_stat.einbars.file_ptr, -1,
				 num_elements, return_vec, return_size);
	break;

    case NDI_EDIST:		/* Curr. particle's energy distribution at current temp. */
	ierr =
	    GET_EDIST_VEC(state, state->lib_stream, return_vec,
			  return_size);
	break;

    case NDI_E_BOUNDS:		/*  Multigroup energy boundaries for sec. part distr. */

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

static int GET_CHI_MAT(state_struct *state, FILE *stream, long mat_ptr,
		       long vec_ptr, FLOAT_TYPE * return_mat,
		       int lib_num_grps, int return_size)
{

    /* lib_num_grps is the number of groups--uncollapsed */

    int ierr = 0;
    int tmp_err = 0;
    int num_full_mat = 0;
    int row;
    int coll_num_grps = 0;
    int mat_size = return_size;
    FLOAT_TYPE *temp_mat = return_mat;
    FLOAT_TYPE *row_ptr = NULL;


    (void) NDI_INIT_FLOAT_VEC(return_mat, return_size);

    if (!state) {
	return (-2);
    }

    if (lib_num_grps <= 0) {
	ierr = 9;
	return (ierr);
    }

    num_full_mat = lib_num_grps * lib_num_grps;

    if (state->collapse_flag) {
	temp_mat =
	    (FLOAT_TYPE *) ndi_malloc(num_full_mat * sizeof(FLOAT_TYPE));
	mat_size = num_full_mat;
	if (!temp_mat) {
	    /* Out of memory */
	    return (-14);
	}
    }

    if (mat_ptr) {
	/*  Full matrix is in library  */
	ierr = NDI_READ_FLOAT_BLOCK(stream, mat_ptr, num_full_mat,
				    temp_mat, mat_size);
    } else {
	/* Matrix is constructed from vector */
	if (vec_ptr) {
	    FLOAT_TYPE *vec_array;
	    vec_array =
		(FLOAT_TYPE *) ndi_malloc(lib_num_grps *
					  sizeof(FLOAT_TYPE));
	    if (vec_array) {
		ierr = NDI_READ_FLOAT_BLOCK(stream, vec_ptr,
					    lib_num_grps,
					    vec_array, lib_num_grps);
		ierr =
		    NDI_VEC2MAT(vec_array, lib_num_grps, temp_mat,
				num_full_mat, mat_size);
		if (vec_array)
		    ndi_free((void **) &vec_array);
	    } else {
		/* Out of memory */
		ierr = -14;
	    }
	    if (tmp_err == 7) {
		/* Data was truncated so forget normalization and transpose */
		return (tmp_err);
	    }
	    /* Warning: matrix has been constructed */
	    if (ierr == 0) {
		ierr = 10;
	    }
	} else {
	    /* Error: data not available */
	    ierr = -9;
	}
    }


    /*   COLLAPSING REQUIRED   */
    if (state->collapse_flag) {
	if (ierr >= 0) {
	    tmp_err =
		NDI_COLLAPSE_MAT(temp_mat, lib_num_grps, lib_num_grps,
				 state->grp_num_new, state->grp_num_new,
				 state->new_ebounds, state->new_ebounds,
				 state->lib_weight, return_mat,
				 return_size);
	    if (tmp_err == 7) {
		/* return_mat is not large enough data was truncated so forget normalization and transpose */
		return (tmp_err);
	    }
	    coll_num_grps = state->grp_num_new;
	    /*        tmp_err = NDI_COLLAPSE_MAT( temp_mat, num_full_mat, return_mat ); */
	    if (tmp_err < 0) {
		ierr = tmp_err;
	    }
	}
	if (temp_mat)
	    ndi_free((void **) &temp_mat);
    } else {
	coll_num_grps = lib_num_grps;
    }


    /* NORMALIZE ROWS */
    if (!state->no_norm) {
	row_ptr = return_mat;
	for (row = 0; row < coll_num_grps; row++) {
	    if (((row + 1) * coll_num_grps) > return_size) {
		/*        printf ("*******************ERROR (row=%d, ng=%d,retsize=%d, lastele=%d)\n",
		   row, coll_num_grps, return_size, (row + 1) * coll_num_grps );  */
		break;
	    } else {
		tmp_err = NORM_FLOAT_VEC(row_ptr, coll_num_grps, 1.0);
		row_ptr += coll_num_grps;
	    }
	}
    }

    /*   TRANSPOSE MATRIX ORDER  */
    /* coll_num_grps currently set to current number of collapse groups */
    if ((state->mat_order == 't') && (ierr >= 0)) {
	tmp_err =
	    NDI_TRANSPOSE_FLOAT(return_mat, coll_num_grps, coll_num_grps);
    }
    if (tmp_err < 0) {
	ierr = tmp_err;
    }

    return (ierr);
}

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

/*********************************************************************/

static int NDI_CALC_E_N(state_struct *state, FLOAT_TYPE * return_vec,
			int return_size)
{

    int i;			/* counter over incident groups */
    int j;			/* counter over outgoing groups */
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

    /*p0_size =  num_grps * num_grps; */
    p0_size = ndi_c_get_size_n_h(state, NDI_TRANSFER_MAT_PR, 0, &ierr);

    p0_vec = (FLOAT_TYPE *) ndi_malloc(p0_size * sizeof(FLOAT_TYPE));
    ierr = NDI_GET_FLOAT_VEC_N_H(state, NDI_TRANSFER_MAT_PR, 0, p0_vec, p0_size);
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
    return (ierr);
}

/*********************************************************************/

static int NDI_CALC_E_AVAIL(state_struct *state, FLOAT_TYPE * return_vec,
			    int return_size)
{

    int i;			/* counter over reactions */
    int j;			/* counter over energy groups */
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
	if ((MT[i] > 18) && (MT[i] < 22)) {
	    continue;
	}
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
				     num_elements, -1, return_vec,
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
				   num_elements, -1, return_vec,
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

    if ((state->curr_part_index >= 0) &&
	(state->iso_stat.num_sec_parts > state->curr_part_index)) {
	part = &state->iso_stat.sec_part[state->curr_part_index];
	if (part->collapse_flag) {
	    /* User-specified collapse-group structure */
	    for (i = 0; i <= part->grp_num_new; i++) {
		if (i < return_size) {
		    return_vec[i] = (FLOAT_TYPE) part->collapse_ebounds[i];
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
    ndi_float64 *tot_sig;
    ndi_float64 *vec;

    ierr = ndi_get_int_val(NDI_NUM_GRPS, &num_grps);
    if (num_grps > 0) {
	tot_sig =
	    (ndi_float64 *) ndi_malloc(num_grps * sizeof(ndi_float64));
	vec = (ndi_float64 *) ndi_malloc(num_grps * sizeof(ndi_float64));
    } else {
	ierr = 5;
	return (ierr);
    }
    ierr = ndi_get_float64_vec(NDI_SIG_TOT, tot_sig, num_grps);
    ierr = ndi_get_old_kerma64(state, vec, num_grps);

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
int NDI_GET_OLD_KERMA(state_struct *state, FLOAT_TYPE * return_vec,
		      int return_size)
{

    int ierr = 0;
    int i, j;
    int num_elements;
    int num_reactions;
    int *mt;
    int mat_size;
    int old_reac_size = 3;
    char old_reac_id[3];
    ndi_float64 *tmp_vec = NULL;
    ndi_float64 *curr_xs = NULL;
    ndi_float64 fissq[NUM_FISSION_Q];
    ndi_float64 *eavg = NULL;
    ndi_float64 q;
    ndi_float64 *scat_mat = NULL;
    ndi_float64 *elastic_scat = NULL;
    ndi_float64 *inelastic_scat = NULL;


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
    ndi_init_float64_vec(tmp_vec, num_elements);
    ierr = ndi_get_float64_vec(NDI_E_AVG, eavg, num_elements);
    ierr = ndi_get_float64_vec(NDI_FISS_Q, fissq, NUM_FISSION_Q);

    for (i = 0; i < num_reactions; i++) {
	if ((mt[i] > 18) && (mt[i] < 22)) {
	    /* Partial fission cross sections contained in MT = 18 */
	    continue;
	}
	ierr =
	    ndi_get_float64_vec_n(NDI_SIG_REAC, mt[i], curr_xs,
				  num_elements);
	if (mt[i] == 18) {
	    q = fissq[0] - fissq[5];	/* Q(prompt fiss) - Q(prompt n) */
	} else {
	    ierr = ndi_get_float64_val_n(NDI_REAC_Q, mt[i], &q);
	}
	for (j = 0; j < num_elements; j++) {
	    tmp_vec[j] += curr_xs[j] * (eavg[j] + q);
/*            printf ("%d: %e   q=%e  e=%e  xs=%e\n", j, tmp_vec[j], q, eavg[j], curr_xs[j]); */
	}
    }

    /* Handle energy from scattering */
    mat_size = ndi_c_get_size_n_h(state, NDI_PN_FULL, 0, &ierr);
    scat_mat = (ndi_float64 *) ndi_malloc(mat_size * sizeof(ndi_float64));
    ierr = ndi_get_float64_vec_n(NDI_PN_FULL, 0, scat_mat, mat_size);
    for (i = 0; i < num_elements; i++) {	/* i = incident energy */
	for (j = 0; j < num_elements; j++) {	/* j = outgoing energy */
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

    if (tmp_vec) {
	ndi_free((void **) &tmp_vec);
    }
    if (curr_xs) {
	ndi_free((void **) &curr_xs);
    }
    if (eavg) {
	ndi_free((void **) &eavg);
    }
    if (scat_mat) {
	ndi_free((void **) &scat_mat);
    }
    if (elastic_scat) {
	ndi_free((void **) &scat_mat);
    }
    if (inelastic_scat) {
	ndi_free((void **) &scat_mat);
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
/* 	 printf("%d  %e\n", i, corr[i]); */
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

static int NDI_GET_CHI_VEC(state_struct *state, isotope_status * mg,
			   char chi_type, FLOAT_TYPE * return_vec,
			   int *return_size)
{

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

       (LATER: Calculate if other chi matrices are available. )


       Parameters:
       mg -- in:pointer to multigroup status structure, i.e. particle data.
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

    int ierr = 0;
    int n;
    int num_grps;
    char chi_type1;
    char chi_type2;
    long int chi_ptr;
    long int chi1_ptr;
    long int chi2_ptr;
    long int chi_mat_ptr;
    FLOAT_TYPE *chi1_vec = NULL;
    FLOAT_TYPE *chi2_vec = NULL;
    FLOAT_TYPE chi_wgt1 = 0.0;
    FLOAT_TYPE chi_wgt2 = 0.0;
    FLOAT_TYPE chi_value = 0.0;
    FLOAT_TYPE norm_sum = 0.0;


    (void) NDI_INIT_FLOAT_VEC(return_vec, *return_size);

    switch (chi_type) {

    case 't':
	chi_ptr = mg->chi_vec_tot.file_ptr;
	chi1_ptr = mg->chi_vec_pr.file_ptr;
	chi2_ptr = mg->chi_vec_del.file_ptr;
	chi_type1 = 'p';
	chi_type2 = 'd';
	chi_mat_ptr = mg->chi_mat_tot.file_ptr;
	break;

    case 'p':
	chi_ptr = mg->chi_vec_pr.file_ptr;
	chi1_ptr = mg->chi_vec_tot.file_ptr;
	chi2_ptr = mg->chi_vec_del.file_ptr;
	chi_type1 = 't';
	chi_type2 = 'd';
	chi_mat_ptr = mg->chi_mat_pr.file_ptr;
	break;

    case 'd':
	chi_ptr = mg->chi_vec_del.file_ptr;
	chi1_ptr = mg->chi_vec_tot.file_ptr;
	chi2_ptr = mg->chi_vec_pr.file_ptr;
	chi_type1 = 't';
	chi_type2 = 'p';
	chi_mat_ptr = mg->chi_mat_del.file_ptr;
	break;

    default:
	ierr = -13;
	return (ierr);
	break;
    }				/* switch */

    if (!state) {
	return (-2);
    }

    if (state->collapse_flag) {
	num_grps = state->grp_num_new;
    } else {
	num_grps = mg->num_grps;
    }

    if (chi_ptr) {
	/* (1) Read chi from library */
	ierr = GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, chi_ptr,
				    2, mg->num_grps,
				    return_vec, *return_size);

    } else if (chi_mat_ptr) {
	/* (2)  Form chi vector by flux-averaging chi matrix */
	ierr =
	    CHI_MAT_TO_VEC(state, state->lib_stream, chi_mat_ptr,
			   return_vec, *return_size, 2, mg->num_grps);

	/* Enough info available to calculate chi from other chi vectors */
    } else if (chi1_ptr && chi2_ptr && ndi_nusigf_avail(state, chi_type1)
	       && ndi_nusigf_avail(state, chi_type2)
	       && mg->wgts.file_ptr) {

	ierr = CALC_CHI_WGT(state, chi_type1, &chi_wgt1);
	ierr = CALC_CHI_WGT(state, chi_type2, &chi_wgt2);

/*      printf("factor1=%e  factor2=%e\n", chi_wgt1, chi_wgt2); */

	chi1_vec =
	    (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
	chi2_vec =
	    (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
	ierr =
	    GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, chi1_ptr, 2,
				 mg->num_grps, chi1_vec, num_grps);
	ierr =
	    GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, chi2_ptr, 2,
				 mg->num_grps, chi2_vec, num_grps);
	for (n = 0; n < num_grps; n++) {
	    if (chi_type == 't') {
		chi_value =
		    (chi_wgt1 * chi1_vec[n]) + (chi_wgt2 * chi2_vec[n]);
	    } else {
		chi_value =
		    (chi_wgt1 * chi1_vec[n]) - (chi_wgt2 * chi2_vec[n]);
	    }
	    norm_sum += chi_value;
	    if (n < *return_size) {
		return_vec[n] = chi_value;
	    } else {
		ierr = 7;
	    }
	}

	if (chi1_vec)
	    ndi_free((void **) &chi1_vec);
	if (chi2_vec)
	    ndi_free((void **) &chi2_vec);
	/* if ( chi_value < 0 ) chi_value = 0; */


    } else {
	/* Warning: No chi values in library */
	/* Array is zeroed below */
	(void) NDI_INIT_FLOAT_VEC(return_vec, *return_size);
	ierr = 9;
    }

    /* Normalize chi to one */
    if (!state->no_norm) {
	if (*return_size >= num_grps) {
	    ierr = NORM_FLOAT_VEC(return_vec, num_grps, 1.0);
	} else {
	    ierr = NORM_FLOAT_VEC(return_vec, *return_size, 1.0);
	    ierr = 7;
	}
    }
    return (ierr);
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

    for (i = 0; i < curr_index; i++) {	/* Loop over reaction products */
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
		    if (ierr != 9) {	/* No data warning takes precedence 
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
    ndi_float64 *wgt_ptr;	/* Weights are stored as float64 */
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
static int NDI_GET_NUSIGF(state_struct *state, char nu_type,
			  FLOAT_TYPE * return_vec, int *return_size)
{

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

    int ierr = 0;
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
    /*    FLOAT_TYPE *sigf = NULL; 
       FLOAT_TYPE *nu = NULL;  */

    if (!state) {
	return (-2);
    }

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
	ierr = -13;
	return (ierr);
	break;
    }

    if (state->collapse_flag) {
	num_elements = state->grp_num_new;
    } else {
	num_elements = state->iso_stat.num_grps;
    }

    if (nu_sigf_ptr) {
	/* Read nu_sigf from library */
	/* Check if state->iso_stat.num_grps is appropriate */
	ierr = GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, nu_sigf_ptr,
				    0, state->iso_stat.num_grps,
				    return_vec, *return_size);

    } else if (nu_sigf1_ptr && nu_sigf2_ptr) {
	/* Calculate nu_sigf from other two nu_sigf's */
	nu_sigf1_vec =
	    (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
	nu_sigf2_vec =
	    (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
	curr_size = num_elements;
	ierr = GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, nu_sigf1_ptr,
				    0, state->iso_stat.num_grps,
				    nu_sigf1_vec, num_elements);
	/* Check error */
	curr_size = num_elements;
	ierr = GET_SIMPLE_FLOAT_VEC(state, state->lib_stream, nu_sigf2_ptr,
				    0, state->iso_stat.num_grps,
				    nu_sigf2_vec, num_elements);
	/* Check error */
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
	if (nu_sigf1_vec)
	    ndi_free((void **) &nu_sigf1_vec);
	if (nu_sigf2_vec)
	    ndi_free((void **) &nu_sigf2_vec);

    } else if (ndi_nu_avail(state, nu_type) && ndi_sigf_avail(state)) {
	/* Calculate nu_sigf from nu and MT=18 cross section */
	nu = (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
	sigf =
	    (FLOAT_TYPE *) ndi_malloc(num_elements * sizeof(FLOAT_TYPE));
	curr_size = num_elements;
	ierr =
	    NDI_GET_NU(state, &state->iso_stat, nu_type, nu, &curr_size);
	/* Check error */
	curr_size = num_elements;
	ierr = GET_SIG_REAC(state, &state->iso_stat, 18, sigf, &curr_size);
	/*Check errors */
	for (n = 0; n < num_elements; n++) {
	    if (n < *return_size) {
		return_vec[n] = nu[n] * sigf[n];
	    } else {
		/* Bounds check */
		ierr = 7;
		break;
	    }
	}
	if (nu) {
	    ndi_free((void **) &nu);
	}
	if (sigf) {
	    ndi_free((void **) &sigf);
	}

    } else {
	/* No error or warning for no data--just return zeroes? */
	/* Set num_elements to either num_grps or grp_num_new */
	if (num_elements > *return_size) {
	    /*  ierr = 7; */
	    (void) NDI_INIT_FLOAT_VEC(return_vec, *return_size);
	} else {
	    (void) NDI_INIT_FLOAT_VEC(return_vec, num_elements);
	}
	ierr = 9;
	curr_size -= num_elements;
    }

    return (ierr);
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
    ndi_float64 _16_llnl_eavg[] =
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
	eavg_p = _16_llnl_eavg;
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
            calc_flag = -1;	/* Calculate from average velocity */
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
        
    } else {
        ierr = -13;
    }

    return(ierr);
}

/*********************************************************************/
/*********************************************************************/
static int PHOTON_E_AVG (state_struct *state, int num_grps, FLOAT_TYPE *return_vec, 
                         int return_size) {

    return(MIDPT_ENERGY(state, num_grps, return_vec, return_size));

}

/*********************************************************************/
/*********************************************************************/
static int MIDPT_ENERGY (state_struct *state, int num_grps, FLOAT_TYPE *return_vec, 
                         int return_size) {
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
