
/*
 * File:	ndi_get_float_vec_.c
 * Description:	Source for libndi.a
 * Time-stamp:	<02/08/29 09:03:44 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"

static int NDI_DIAG2FULL(FLOAT_TYPE * low_diag, int num_ld,
			 FLOAT_TYPE * up_diag, int num_ud, int num_row,
			 int num_col, FLOAT_TYPE * return_vec);
static int GET_FULL_PN_MAT(state_struct * state, FILE * stream,
			   multi_stat full_mat, multi_stat down_mat,
			   int num_grps, int vec_num,
			   FLOAT_TYPE * return_mat, int return_size);
static int NDI_FULL2DIAG(FLOAT_TYPE * full_mat, int num_grps,
			 FLOAT_TYPE * return_vec, int return_size);
static int GET_PN_DOWN_FULL(state_struct * state, FILE * stream,
			    FLOAT_TYPE * return_vec,
			    multi_stat * down_diag, multi_stat * full_mat,
			    int vec_num, int vec_size);
static int PN_DOWN_FULL_TRANS_CORR(state_struct * state,
				   FLOAT_TYPE * return_vec,
				   int return_size);
static int PN_FULL_TRANS_CORR(state_struct * state,
			      FLOAT_TYPE * return_vec, int return_size);
static int GET_PN_PROD_FULL(state_struct * state, FILE * stream,
			    sec_part_mg * sec_part, int vec_num,
			    FLOAT_TYPE * return_vec, int return_size);
static int GET_DIAG_CORR(state_struct * state, FLOAT_TYPE * return_vec,
			 int vec_num);
static int GET_BHS_CORR(state_struct * state, FLOAT_TYPE * return_vec,
			int vec_num);
static int GET_TRANSFER_MAT(state_struct * state, int n_type, int vec_num,
			    FLOAT_TYPE * return_vec, int vec_size);
static int GET_PNU_PR(state_struct * state, FLOAT_TYPE * return_vec,
		      int vec_num);
static int GET_SIG_PNU_PR(state_struct * state, FLOAT_TYPE * return_vec,
			  int vec_num);

/**********************************************************************/

int
NDI_GET_FLOAT_VEC_N_H(state_struct * state, int in_key, int vec_num,
		      FLOAT_TYPE * return_vec, int return_size)
{

    int ierr = 0;
    int tmp_err = 0;
    int num_elements;
    char in_str[6];


    /* Initialize return vector with zeroes */
    NDI_INIT_FLOAT_VEC(return_vec, return_size);

    if (!state) {
	return (-2);
    }

    /* Check that ndi_init has been called. */
    if (!state->ndi_initialized) {
	return (-2);
    }
#ifdef FLOAT32
    if (sizeof(float) == 8) {
	ierr = -2;
	return (ierr);
    }
#endif

    if (state->library_type == ' ') {
	/* Library type was not set */
	return (-16);
    } else if ((state->library_type != 'm') &&
	       (state->library_type != 'd')) {
	/* Incompatible library -- Bad input key */
	return (-4);
    }

    /* Check that isotope is set */
    if (!state->iso_stat.set_flag) {
	return (-7);
    }

   /*************************/
    /*   Process input key   */
   /*************************/

    switch (in_key) {

    case NDI_PN_DOWN_FULL:	/* Pn scattering matrix -- downscatter diagonal */

	num_elements = state->iso_stat.num_grps;

	ierr =
	    GET_PN_DOWN_FULL(state, state->lib_stream, return_vec,
			     &state->iso_stat.pn_down_full,
			     &state->iso_stat.pn_full, vec_num,
			     return_size);

	/* Apply transport correction, if necessary */
      /*** Test that scattering order is less than transport correction order ***/
	if ((ierr >= 0) && (state->trans_corr != ' ')) {
	    ierr = PN_DOWN_FULL_TRANS_CORR(state, return_vec, return_size);
	}
	/*  Transpose isn't applied to diagonal */

	break;

    case NDI_PN_FULL:		/* Full Pn scattering matrix  */
	/*  Full scattering matrices are not currently supported in the library,
	   so pn_full should not contain data  */

	num_elements = state->iso_stat.num_grps;
	ierr =
	    GET_FULL_PN_MAT(state, state->lib_stream,
			    state->iso_stat.pn_full,
			    state->iso_stat.pn_down_full, num_elements,
			    vec_num, return_vec, return_size);

	/* Apply transport correction, if necessary */
      /*** Test that scattering order is less than transport correction order ***/
	if ((ierr >= 0) && (state->trans_corr != ' ')) {
	    ierr = PN_FULL_TRANS_CORR(state, return_vec, return_size);
	}
	/* Transpose result, if necessary */
	if (state->collapse_flag != 0) {
	    num_elements = state->grp_num_new;
	} else {
	    num_elements = state->iso_stat.num_grps;
	}
	if ((state->mat_order == 't') && (ierr >= 0)) {
	 /***  CHECK THAT THE NUMBER OF ROWS AND COLUMNS INPUT ARE CORRECT!!!! */
	    ierr = NDI_TRANSPOSE_FLOAT(return_vec, num_elements,
				       num_elements);
	}
	break;

    case NDI_SIG_REAC:		/* Sigma edit cross sections */
	num_elements = return_size;
	ierr =
	    GET_SIG_REAC(state, &state->iso_stat, vec_num, return_vec,
			 &num_elements);
	break;


    case NDI_PNU_PR:		/* fission neutron mult probabilities */
	ierr = GET_PNU_PR(state, return_vec, vec_num);
	break;

    case NDI_SIG_PNU_PR:
	ierr = GET_SIG_PNU_PR(state, return_vec, vec_num);
	break;

    case NDI_PN_PROD_FULL_CURR_PART:	/* Full production matrix for current particle */

	(void) NDI_INIT_FLOAT_VEC(return_vec, return_size);


	if (state->curr_part_index >= 0) {
	    if (state->curr_part_index < state->iso_stat.num_sec_parts) {
		ierr =
		    GET_PN_PROD_FULL(state, state->lib_stream,
				     &state->iso_stat.sec_part[state->
							       curr_part_index],
				     vec_num, return_vec, return_size);

		/* Transpose result, if necessary */
		if (state->collapse_flag != 0) {
		    num_elements = state->grp_num_new;
		} else {
		    num_elements = state->iso_stat.num_grps;
		}

		if ((state->mat_order == 't') && (ierr >= 0)) {
	   /***  CHECK THAT THE NUMBER OF ROWS AND COLUMNS INPUT ARE CORRECT!!!! */
		    ierr =
			NDI_TRANSPOSE_FLOAT(return_vec,
					    state->iso_stat.
					    sec_part[state->
						     curr_part_index].
					    num_grps, num_elements);
		}
	    } else {
		ierr = ndi_check_array_err(state->iso_stat.num_sec_parts);
	    }
	} else {
	    /* Error -- bad particle? */
	    ierr = -999;
	}

	break;

    case NDI_TRANSFER_MAT_TOT:	/* Transfer matrix -- total */
    case NDI_TRANSFER_MAT_PR:	/* Transfer matrix -- prompt */
	ierr =
	    GET_TRANSFER_MAT(state, in_key, vec_num, return_vec, vec_num);

	/* Transpose result, if necessary */
	if (state->collapse_flag != 0) {
	    num_elements = state->grp_num_new;
	} else {
	    num_elements = state->iso_stat.num_grps;
	}

	if ((state->mat_order == 't') && (ierr >= 0)) {
	 /***  CHECK THAT THE NUMBER OF ROWS AND COLUMNS INPUT ARE CORRECT!!!! */
	    ierr = NDI_TRANSPOSE_FLOAT(return_vec, num_elements,
				       num_elements);
	}
	break;

    case NDI_CORR_DIAG:	/* Diagonal transport correction  */

	ierr = GET_DIAG_CORR(state, return_vec, vec_num);
	break;

    case NDI_CORR_BHS:		/* Bell-Hansen-Sandmeier transport correction */

	ierr = GET_BHS_CORR(state, return_vec, vec_num);
	break;

    case NDI_TOT_PROD_XS:	/* Total production x.s. for target given by 
				   global var. 'zaid' and product given by vec_num */
	sprintf(in_str, "%d", vec_num);
	ierr = GET_PROD_REAC(state, in_str, return_vec, return_size);
	break;


      /**********  UNKNOWN KEYWORD  ***********/

    default:
	ierr = -4;
	break;
    }
    if ((tmp_err != 0) && (ierr >= 0)) {
	ierr = tmp_err;
    }
    return (ierr);
}

/*********************************************************************/

static int
GET_PN_DOWN_FULL(state_struct * state, FILE * stream,
		 FLOAT_TYPE * return_vec,
		 multi_stat * down_diag, multi_stat * full_mat,
		 int vec_num, int return_size)
{

/* Will not construct a diagonal from a full matrix.  Is this needed? */

    int ierr = 0;
    int full = 0;		/*  Is scattering info in full matrix form, 0=no,-1=yes */
    int num_grps;
    int diag_size;
    int full_mat_size;
    int curr_vec_size = return_size;
    FLOAT_TYPE *curr_vec = return_vec;
    FLOAT_TYPE *curr_mat = NULL;
    FLOAT_TYPE *temp_vec = NULL;
    FLOAT_TYPE *temp_mat = NULL;

    NDI_INIT_FLOAT_VEC(return_vec, return_size);

    if (state) {
	num_grps = state->iso_stat.num_grps;
	diag_size = (num_grps * (num_grps + 1)) / 2;
	full_mat_size = num_grps * num_grps;
    } else {
	return (-2);
    }

    if (num_grps <= 0) {
	ierr = 9;
	return (ierr);
    }

    if (down_diag->num_objs > 0) {
	/* The diagonal is available in the library */
	if (down_diag->num_objs > vec_num) {
	    if (down_diag->stat[vec_num].file_ptr) {

		if (state->collapse_flag) {
		    temp_vec =
			(FLOAT_TYPE *) ndi_malloc(diag_size *
						  sizeof(FLOAT_TYPE));
		    curr_vec_size = diag_size;
		    if (!temp_vec)
			return (-14);
		    curr_vec = temp_vec;
		}
		ierr =
		    NDI_READ_FLOAT_BLOCK(stream,
					 down_diag->stat[vec_num].file_ptr,
					 diag_size, curr_vec,
					 curr_vec_size);
		if (state->collapse_flag) {
		    temp_mat =
			(FLOAT_TYPE *) ndi_malloc(full_mat_size *
						  sizeof(FLOAT_TYPE));
		    if (!temp_mat)
			return (-14);
		    ierr =
			NDI_DIAG2FULL(NULL, 0, curr_vec, diag_size,
				      num_grps, num_grps, temp_mat);
		    full = -1;
		    curr_mat = temp_mat;
		    if (temp_vec)
			ndi_free((void **) &temp_vec);
		} else {
		    /* Diagonal read in, no collapsing -- just return */
		}
	    } else {
		/* No file pointer set */
		ierr = 9;
	    }
	} else {
	    /* Error: index out of range */
	    ierr = ndi_check_array_err(down_diag->num_objs);
	}

    } else {
	/* Try constructing diagonal from full matrix */
	if (full_mat->num_objs > 0) {
	    if (full_mat->num_objs > vec_num) {
		if (full_mat->stat[vec_num].file_ptr) {
		    /*  The diagonal not in library,  construct it from full pn matrix */
		    temp_mat =
			(FLOAT_TYPE *) ndi_malloc(full_mat_size *
						  sizeof(FLOAT_TYPE));
		    if (!temp_mat)
			return (-14);
		    ierr =
			NDI_READ_FLOAT_BLOCK(stream,
					     full_mat->stat[vec_num].
					     file_ptr, full_mat_size,
					     temp_mat, full_mat_size);

	   /*** check return value ***/
		    curr_mat = temp_mat;
		    full = -1;
		} else {
		    /* File pointer not set -- data not available */
		    ierr = 9;
		}
	    } else {
		/* Error: index out of range */
		ierr = ndi_check_array_err(full_mat->num_objs);
	    }
	} else {
	    /* Neither the pn diagonal nor the full pn matrix in library */
	    ierr = 9;
	}

    }
    /* Collapse matrix */
    /* If the collapse flag is true, temp_mat will contain the full matrix at this
       point whether it was constructed from the diagonal or read from the file */
    if ((ierr >= 0) && (state->collapse_flag)) {
	temp_vec =
	    (FLOAT_TYPE *) ndi_malloc(state->grp_num_new *
				      state->grp_num_new *
				      sizeof(FLOAT_TYPE));
	ierr =
	    NDI_COLLAPSE_MAT(temp_mat, num_grps, num_grps,
			     state->grp_num_new, state->grp_num_new,
			     state->new_ebounds, state->new_ebounds,
			     state->lib_weight, temp_vec,
			     state->grp_num_new * state->grp_num_new);
/*      ierr = NDI_COLLAPSE_MAT( temp_mat, full_mat_size, temp_vec ); */
	num_grps = state->grp_num_new;
	curr_mat = temp_vec;
	if (temp_mat)
	    ndi_free((void **) &temp_mat);
    }

    /* Turn matrix into diagonal */
    if ((ierr >= 0) && (full != 0)) {
	ierr = NDI_FULL2DIAG(curr_mat, num_grps, return_vec, return_size);
    }

    if (temp_mat)
	ndi_free((void **) &temp_mat);
    if (temp_vec)
	ndi_free((void **) &temp_vec);
    return (ierr);

}

/*********************************************************************/

static int
GET_FULL_PN_MAT(state_struct * state, FILE * stream,
		multi_stat full_mat, multi_stat down_mat,
		int num_grps, int vec_num,
		FLOAT_TYPE * return_mat, int return_size)
{

    /* There's no check on the matrix size here. */

    int ierr = 0;
    int tmp_err = 0;
    int temp_mat_size;
    int num_full_mat = num_grps * num_grps;
    int num_down = (num_grps * (num_grps + 1)) / 2;
    FLOAT_TYPE *temp_mat = NULL;


    NDI_INIT_FLOAT_VEC(return_mat, return_size);

    if (!state) {
	return (-2);
    }

    if (num_grps <= 0) {
	ierr = 9;
	return (ierr);
    }

    if (state->collapse_flag) {
	temp_mat =
	    (FLOAT_TYPE *) ndi_malloc(num_full_mat * sizeof(FLOAT_TYPE));
	temp_mat_size = num_full_mat;
	if (!temp_mat) {
	    /* Out of memory */
	    return (-14);
	}
    } else {
	temp_mat = return_mat;
	temp_mat_size = return_size;
    }

    if (full_mat.num_objs > 0) {
	/*  Full matrix is in library  */
	if (full_mat.num_objs > vec_num) {
	    if (full_mat.stat[vec_num].file_ptr) {
		ierr =
		    NDI_READ_FLOAT_BLOCK(stream,
					 full_mat.stat[vec_num].file_ptr,
					 num_full_mat, temp_mat,
					 temp_mat_size);
	    } else {
		/* No file pointer set */
		ierr = 9;
	    }
	} else {
	    /* Error: index out of range */
	    ierr = ndi_check_array_err(full_mat.num_objs);
	}
    } else {
	/* Full matrix is constructed from diagonal */
	/* When upscatter is added to library, should change this to include it */
	if (down_mat.num_objs > 0) {
	    if (down_mat.num_objs > vec_num) {
		if (down_mat.stat[vec_num].file_ptr) {
		    FLOAT_TYPE *diag_array;
		    diag_array =
			(FLOAT_TYPE *) ndi_malloc(num_down *
						  sizeof(FLOAT_TYPE));
		    if (diag_array) {
			ierr =
			    NDI_READ_FLOAT_BLOCK(stream,
						 down_mat.stat[vec_num].
						 file_ptr, num_down,
						 diag_array, num_down);
			ierr =
			    NDI_DIAG2FULL(NULL, 0, diag_array, num_down,
					  num_grps, num_grps, temp_mat);
			if (diag_array)
			    ndi_free((void **) &diag_array);
			if (ierr >= 0) {
			    /* Warning: full scattering matrix is constructed from downscatter only */
			    /* ierr = 10;  ??? */
			}
		    } else {
			/* Out of memory */
			ierr = -14;
		    }
		} else {
		    /* No file pointer set */
		    ierr = 9;
		}
	    } else {
		/* Error: index out of bounds */
		ierr = ndi_check_array_err(down_mat.num_objs);
	    }
	} else {
	    /* Error: data not available */
	    ierr = 9;
	}
    }


    /*   COLLAPSING REQUIRED   */
    if (state->collapse_flag) {
	if (ierr >= 0) {
	    tmp_err =
		NDI_COLLAPSE_MAT(temp_mat, num_grps, num_grps,
				 state->grp_num_new, state->grp_num_new,
				 state->new_ebounds, state->new_ebounds,
				 state->lib_weight, return_mat,
				 return_size);
/* 	 tmp_err = NDI_COLLAPSE_MAT( temp_mat, num_full_mat, return_mat ); */
	    num_grps = state->grp_num_new;
	}
	if (temp_mat)
	    ndi_free((void **) &temp_mat);
	if (tmp_err < 0) {
	    ierr = tmp_err;
	}
    }

    return (ierr);
}

/*********************************************************************/

static int
NDI_DIAG2FULL(FLOAT_TYPE * low_diag, int num_ld,
	      FLOAT_TYPE * up_diag, int num_ud, int num_row,
	      int num_col, FLOAT_TYPE * return_vec)
{

    /* ASSUMPTION:  ud_diag matrix contains the diagonal!! 
       Should row == col always???                          */

    int ierr = 0;
    int row = 0;
    int col = 0;
    int ld_count = 0;
    int ud_count = 0;
    FLOAT_TYPE *ld_ptr = low_diag;
    FLOAT_TYPE *ud_ptr = up_diag;
    FLOAT_TYPE *matrix_ptr = return_vec;

    for (row = 0; row < num_row; row++) {
	for (col = 0; col < row; col++) {
	    if (ld_count < num_ld) {
		*matrix_ptr = *ld_ptr;
		matrix_ptr++;
		ld_ptr++;
		ld_count++;
	    } else {
		*matrix_ptr = 0;
		matrix_ptr++;
	    }
	}
	for (col = row; col < num_col; col++) {
	    if (ud_count < num_ud) {
		*matrix_ptr = *ud_ptr;
		matrix_ptr++;
		ud_ptr++;
		ud_count++;
	    } else {
		*matrix_ptr = 0;
		matrix_ptr++;
	    }
	}
    }
    return (ierr);
}

/*********************************************************************/

static int
NDI_FULL2DIAG(FLOAT_TYPE * full_mat, int num_grps,
	      FLOAT_TYPE * return_vec, int return_size)
{

    int ierr = 0;
    int row = 0;
    int col = 0;
    int num_row = num_grps;
    int num_col = num_grps;
    int ret_count = 0;
    FLOAT_TYPE *mat_ptr = full_mat;
    FLOAT_TYPE upscat_sum;

    if ((full_mat) && (return_vec)) {
	for (row = 0; row < num_row; row++) {
	    upscat_sum = 0.0;
	    for (col = 0; col < num_col; col++) {
		if (((mat_ptr - full_mat) < (num_row * num_col))
		    && (ret_count < return_size)) {
		    if (col < row) {
			upscat_sum += *mat_ptr;
		    } else if (col == row) {
			return_vec[ret_count] = *mat_ptr + upscat_sum;
			ret_count++;
		    } else {
			return_vec[ret_count] = *mat_ptr;
			ret_count++;
		    }
		    mat_ptr++;
		} else {
		    ierr = 7;
		    break;
		}
	    }
	    if (((mat_ptr - full_mat) >= (num_row * num_col))
		|| (ret_count >= return_size)) {
		break;
	    }
	}
    } else {
	if (!full_mat) {
	    /* Bad matrix -- internal NDI error */
	    ierr = -13;
	} else if (!return_vec) {
	    /* Bad vector input by user */
	    ierr = -25;
	}
    }
    return (ierr);
}

/*********************************************************************/

static int
PN_FULL_TRANS_CORR(state_struct * state,
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

    if (num_grps <= 0) {
	ierr = 9;
	return (ierr);
    }

    if (state->collapse_flag)
	num_grps = state->grp_num_new;
    corr = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));

    if (state->trans_corr == 'd') {
	ierr =
	    NDI_GET_FLOAT_VEC_N_H(state, NDI_CORR_DIAG,
				  state->trans_corr_order, corr, num_grps);
    } else if (state->trans_corr == 'b') {
	ierr =
	    NDI_GET_FLOAT_VEC_N_H(state, NDI_CORR_BHS,
				  state->trans_corr_order, corr, num_grps);
    } else {
	ierr = -13;
    }
   /*** Bounds checking? ***/
    if (ierr >= 0) {
	for (i = 0; i < num_grps; i++) {
	    return_vec[(i * num_grps) + i] -= corr[i];
	}
    }
    if (corr)
	ndi_free((void **) &corr);
    return (ierr);

}

/*********************************************************************/

static int
PN_DOWN_FULL_TRANS_CORR(state_struct * state,
			FLOAT_TYPE * return_vec, int return_size)
{

    int i;
    int num_grps;
    int ierr = 0;
    FLOAT_TYPE *corr;
    int curr_index = 0;


    if (state) {
	num_grps = state->iso_stat.num_grps;
    } else {
	return (-2);
    }

    if (num_grps <= 0) {
	ierr = 9;
	return (ierr);
    }

    if (state->collapse_flag)
	num_grps = state->grp_num_new;
    corr = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));

    if (state->trans_corr == 'd') {
	ierr =
	    NDI_GET_FLOAT_VEC_N_H(state, NDI_CORR_DIAG,
				  state->trans_corr_order, corr, num_grps);
    } else if (state->trans_corr == 'b') {
	ierr =
	    NDI_GET_FLOAT_VEC_N_H(state, NDI_CORR_BHS,
				  state->trans_corr_order, corr, num_grps);
    } else {
	ierr = -13;
    }


    /* Traverse the diagonal elements of the triangular 'matrix' using
       the recurrance relation:
       index(i) = index(i-1) + (num_grps - i - 1),
       where index(0) = 0
     */

   /*** Bounds checking? ***/
    if (ierr >= 0) {
	for (i = 0; i < num_grps; i++) {
	    return_vec[curr_index] -= corr[i];
	    curr_index += num_grps - i;
	}
    }
    if (corr)
	ndi_free((void **) &corr);
    return (ierr);

}


/*********************************************************************/

static int
GET_PN_PROD_FULL(state_struct * state, FILE * stream,
		 sec_part_mg * sec_part, int vec_num,
		 FLOAT_TYPE * return_vec, int return_size)
{
    int ierr = 0;
    FLOAT_TYPE *temp_vec = NULL;
    int full_mat_size;
    int new_grp_num_in;
    int new_grp_num_out;
    int *new_ebounds_in;
    int *new_ebounds_out;


    if (state) {
	full_mat_size = sec_part->num_grps * state->iso_stat.num_grps;
    } else {
	return (-2);
    }


    if (sec_part->pn_prod_full.num_objs > 0) {
	if (sec_part->pn_prod_full.num_objs > vec_num) {
	    if (sec_part->pn_prod_full.stat[vec_num].file_ptr) {

		if ((state->collapse_flag) || (sec_part->collapse_flag)) {
		    temp_vec =
			(FLOAT_TYPE *) ndi_malloc(full_mat_size *
						  sizeof(FLOAT_TYPE));
		    if (temp_vec) {
			ierr = NDI_READ_FLOAT_BLOCK(stream,
						    sec_part->pn_prod_full.
						    stat[vec_num].file_ptr,
						    full_mat_size,
						    temp_vec,
						    full_mat_size);
			if (ierr >= 0) {
			    if (state->collapse_flag) {
				new_grp_num_in = state->grp_num_new;
				new_ebounds_in = state->new_ebounds;
			    } else {
				new_grp_num_in = state->iso_stat.num_grps;
				new_ebounds_in = NULL;
			    }
			    if (sec_part->collapse_flag) {
				new_grp_num_out = sec_part->grp_num_new;
				new_ebounds_out = sec_part->new_ebounds;
			    } else {
				new_grp_num_out = sec_part->num_grps;
				new_ebounds_out = NULL;
			    }
			    ierr =
				NDI_COLLAPSE_MAT(temp_vec,
						 state->iso_stat.num_grps,
						 sec_part->num_grps,
						 new_grp_num_in,
						 new_grp_num_out,
						 new_ebounds_in,
						 new_ebounds_out,
						 state->lib_weight,
						 return_vec, return_size);
			}
			if (temp_vec)
			    ndi_free((void **) &temp_vec);
		    } else {
			/* Out of memory */
			ierr = -14;
		    }
		} else {
		    ierr = NDI_READ_FLOAT_BLOCK(stream,
						sec_part->pn_prod_full.
						stat[vec_num].file_ptr,
						full_mat_size, return_vec,
						return_size);
		}
	    } else {
		/* Error reading file pointer */
		ierr = -19;
	    }
	} else {
	    ierr = ndi_check_array_err(sec_part->pn_prod_full.num_objs);
	}
    } else {
	/* Error: no data for this isotope */
	ierr = 9;
    }
    return (ierr);
}

/*********************************************************************/

static int
GET_DIAG_CORR(state_struct * state, FLOAT_TYPE * return_vec, int vec_num)
{
    char transpose_state;
    char corr_state;
    int ierr = 0;
    int i;
    int num = 0;
    int curr_index = 0;
    int array_size;
    FLOAT_TYPE *temp_vec = NULL;

    if (!state) {
	return (-2);
    }

    if (state->collapse_flag) {
	num = state->grp_num_new;
    } else {
	num = state->iso_stat.num_grps;
    }
    array_size = num * num;
    ierr = NDI_INIT_FLOAT_VEC(return_vec, num);
    temp_vec = (FLOAT_TYPE *) ndi_malloc(array_size * sizeof(FLOAT_TYPE));
    if (!temp_vec) {
	/* Out of memory */
	return (-14);
    }

    /* Don't transpose matrix, but save user-defined transpose & trans. cor. state */
    transpose_state = state->mat_order;
    state->mat_order = 'd';
    corr_state = state->trans_corr;
    state->trans_corr = ' ';

    ierr = NDI_GET_FLOAT_VEC_N_H(state, NDI_PN_DOWN_FULL, vec_num + 1,
				 temp_vec, array_size);
    state->mat_order = transpose_state;
    state->trans_corr = corr_state;

    /* Traverse the diagonal elements of the triangular 'matrix' using
       the recurrance relation:
       index(i) = index(i-1) + (num_grps - i - 1),
       where index(0) = 0
     */
    for (i = 0; i < num; i++) {
	*(return_vec + i) = *(temp_vec + curr_index);
	curr_index += num - i;
    }

    if (temp_vec)
	ndi_free((void **) &temp_vec);

    return (ierr);
}

/*********************************************************************/

static int
GET_BHS_CORR(state_struct * state, FLOAT_TYPE * return_vec, int vec_num)
{
    char transpose_state = ' ';
    char corr_state = ' ';
    int ierr = 0;
    int i, j;
    int num = 0;
    int array_size;
    /*  ndi_c_get_size_n( NDI_PN_FULL, vec_num, &ierr ); */
    FLOAT_TYPE *temp_vec = NULL;
    FLOAT_TYPE scat_sum = 0;


    if (!state) {
	return (-2);
    }

    if (state->collapse_flag) {
	num = state->grp_num_new;
    } else {
	num = state->iso_stat.num_grps;
    }

    /*
       **** array_size must be wrong ****
       for now just use array_size = num**2
     */
    array_size = num * num;

    ierr = NDI_INIT_FLOAT_VEC(return_vec, num);
    temp_vec = (FLOAT_TYPE *) ndi_malloc(array_size * sizeof(FLOAT_TYPE));
    if (!temp_vec) {
	/* Out of memory */
	return (-14);
    }

    /*  Don't transpose matrix, but save user-defined transpose & trans. cor. state */
    transpose_state = state->mat_order;
    state->mat_order = 'd';
    corr_state = state->trans_corr;
    state->trans_corr = ' ';

    ierr =
	NDI_GET_FLOAT_VEC_N_H(state, NDI_PN_FULL, vec_num + 1, temp_vec,
			      array_size);
    state->mat_order = transpose_state;
    state->trans_corr = corr_state;


    for (i = 0; i < num; i++) {
	scat_sum = 0;
	for (j = 0; j < num; j++) {
	    scat_sum += *(temp_vec + (i * num) + j);
	}
	*(return_vec + i) = scat_sum;
    }

    if (temp_vec)
	ndi_free((void **) &temp_vec);

    return (ierr);
}

/*********************************************************************/

static int
GET_TRANSFER_MAT(state_struct * state, int n_type, int vec_num,
		 FLOAT_TYPE * return_vec, int vec_size)
{
    int i;
    int j;
    int ierr = 0;
    int array_size = vec_size;
    int nusigf_type = 0;
    int chi_type = 0;
    char transpose_state;
    FLOAT_TYPE *pn_ptr = return_vec;
    FLOAT_TYPE *nusigf = NULL;
    FLOAT_TYPE *chi_mat = NULL;
    FLOAT_TYPE *scat_mat = NULL;
    int num_grps;

   /*** Check for non-fissionable isotopes ***/

    if (!state) {
	return (-2);
    }

    switch (n_type) {

    case NDI_TRANSFER_MAT_PR:
	nusigf_type = NDI_NU_SIG_F_PR;
	chi_type = NDI_CHI_VEC_PR;
	break;
    case NDI_TRANSFER_MAT_TOT:
	nusigf_type = NDI_NU_SIG_F_TOT;
	chi_type = NDI_CHI_VEC_TOT;
	break;
    default:
	ierr = -13;
	break;
    }

    if (ierr < 0)
	return (ierr);

    /*  Don't transpose matrix, but save user-defined transpose & trans. cor. state */
    transpose_state = state->mat_order;
    state->mat_order = 'd';

    if (vec_num == 0) {
	/* Corrections are only applied to 0th order matrix */
	array_size = ndi_c_get_size_h(state, chi_type, &ierr);
	chi_mat =
	    (FLOAT_TYPE *) ndi_malloc(array_size * sizeof(FLOAT_TYPE));
	/* Check for error */
	ierr = NDI_GET_FLOAT_VEC_H(state, chi_type, chi_mat, array_size);

	array_size = ndi_c_get_size_h(state, nusigf_type, &ierr);
	nusigf =
	    (FLOAT_TYPE *) ndi_malloc(array_size * sizeof(FLOAT_TYPE));
	/* Check for error */
	ierr = NDI_GET_FLOAT_VEC_H(state, nusigf_type, nusigf, array_size);

	array_size =
	    ndi_c_get_size_n_h(state, NDI_PN_FULL, vec_num, &ierr);
	scat_mat =
	    (FLOAT_TYPE *) ndi_malloc(array_size * sizeof(FLOAT_TYPE));
	/* Check for error */
	pn_ptr = scat_mat;
    }

    array_size = ndi_c_get_size_n_h(state, NDI_PN_FULL, vec_num, &ierr);
    ierr =
	NDI_GET_FLOAT_VEC_N_H(state, NDI_PN_FULL, vec_num, pn_ptr,
			      array_size);
    state->mat_order = transpose_state;

    if (state->collapse_flag) {
	num_grps = state->grp_num_new;
    } else {
	num_grps = state->iso_stat.num_grps;
    }

    /*  Fill return_array */
    if (vec_num == 0) {
	for (i = 0; i < num_grps; i++) {
	    for (j = 0; j < num_grps; j++) {
		if (((i * num_grps) + j) < vec_size) {
		    *(return_vec + ((i * num_grps) + j)) =
			*(scat_mat + ((i * num_grps) + j))
/*                     + *(nusigf + i) * *(chi_mat +  ((i * num_grps) + j)); */
			+ *(nusigf + i) * *(chi_mat + j);
		} else {
		    ierr = 7;
		}
	    }
	}
	if (nusigf)
	    ndi_free((void **) &nusigf);
	if (chi_mat)
	    ndi_free((void **) &chi_mat);
	if (scat_mat)
	    ndi_free((void **) &scat_mat);
    }
    return (ierr);
}

/*********************************************************************/

/*!
  \fn ndi_erf
  \brief Error function

  See M. Abamowitz and I. A. Stegun
  'Handbook of Mathematical Functions'
  Dover (1972), p. 297, Eq. 7.1.1
  Accurate to within 2.5E-5
  \param x
  \return 2/sqrt(pi) int_0^x exp(-t^2) dt

 */
static FLOAT_TYPE
ndi_erf(FLOAT_TYPE x)
{
    FLOAT_TYPE p = 0.47047;	/* ibid. p. 299, Eq. 7.1.25 */
    FLOAT_TYPE a[] = { 0.3480242, -0.0958798, 0.7478556 };
    FLOAT_TYPE t;

    if (x < 0.0)
	return -ndi_erf(-x);
    else {
	t = 1.0 / (1.0 + p * x);
	return 1.0 - t * (a[0] + t * (a[1] + t * a[2])) * exp(-(x * x));
    }
}

#define MAX_NU 10

/*!
  \fn GET_PNU_PR
  \param return_vec: array of probability per group
  \param vec_num: number of fission neutrons

  See:
  J. Terrell
  "Distribution of Fission Neutron Numbers"
  Phys. Rev. 108, 738 (1957)
*/
static int
GET_PNU_PR(state_struct * state, FLOAT_TYPE * return_vec, int vec_num)
{
    FLOAT_TYPE *nu = NULL;
    int num_grps;
    int ierr;

    FLOAT_TYPE sigma;
    FLOAT_TYPE b;
    int g;

    assert(0 <= vec_num && vec_num < MAX_NU);

    if (!state) {
	return (-2);
    }
    /* get number of groups */
    if (state->collapse_flag)
	num_grps = state->grp_num_new;
    else
	num_grps = state->iso_stat.num_grps;

    /* get nu */
    nu = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
    if (!nu)
	return (-14);		/* out of memory */

    ierr = NDI_GET_FLOAT_VEC_H(state, NDI_NU_PR, nu, num_grps);
    if (ierr < 0)
	return (ierr);

    sigma = sqrt(2.0) * 1.08;	/* Terrell's fit value */

    for (g = 0; g < num_grps; g++) {
	b = 0.5 * (1.0 - ndi_erf((nu[g] + 0.5) / sigma));
	return_vec[g] = 0.5 * (1.0 + ndi_erf((vec_num - nu[g] + 0.5 + b) /
					     sigma));
	if (0 < vec_num)
	    return_vec[g] -=
		0.5 * (1.0 + ndi_erf((vec_num - nu[g] - 0.5 + b) / sigma));
    }

    if (nu)
	ndi_free((void **) &nu);
    return (ierr);
}

/*********************************************************************/

/*!
  \fn ndi_fact
  \brief Factorial function
  \param n
  \pre 0 <= i
  \return n!
 */
static long
ndi_fact(long n)
{

    long j;
    long k;

    assert(0 <= n);

    j = 1;
    if (n > 1)
	for (k = 1; k <= n; k++)
	    j *= k;

    return j;
}

/*!
  \fn ndi_perm
  \brief Permutation function
  \param n, m
  \pre 0 <= m && m <= n
  \return i!
 */
static long
ndi_perm(long n, long m)
{
    long j;
    long k;

    assert(0 <= m && m <= n);

    j = 1;
    if (m == 1)
	j = n;
    else
	for (k = n - m + 1; k <= n; k++)
	    j *= k;
    return j;
}

/*!
  \fn GET_SIG_PNU_PR
  \param sig_pnu: array of transport fission cross section multiplicities
  \param i: number of fission neutrons


  See:
  G. I. Bell and C. E. Lee
  "On the Probability of Initiating a Persistent Fission Chain"
  LA-2608 (1976)
*/
static int
GET_SIG_PNU_PR(state_struct * state, FLOAT_TYPE * sig_pnu, int i)
{

    FLOAT_TYPE *nu = NULL;
    FLOAT_TYPE *nu_sig_f = NULL;
    FLOAT_TYPE *p = NULL;

    int num_grps;
    int ierr;

    long j;
    long k;

    assert(0 <= i && i < MAX_NU);

    if (!state) {
	return (-2);
    }
    /* get number of groups */
    if (state->collapse_flag)
	num_grps = state->grp_num_new;
    else
	num_grps = state->iso_stat.num_grps;

    /* get nu */
    nu = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
    if (!nu)
	return (-14);		/* out of memory */

    ierr = NDI_GET_FLOAT_VEC_H(state, NDI_NU_PR, nu, num_grps);
    if (ierr < 0)
	return (ierr);

    /* get sig_f */
    nu_sig_f = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
    if (!nu_sig_f)
	return (-14);		/* out of memory */

    ierr = NDI_GET_FLOAT_VEC_H(state, NDI_NU_SIG_F_PR, nu_sig_f, num_grps);
    if (ierr < 0)
	return (ierr);

    p = (FLOAT_TYPE *) ndi_malloc(num_grps * sizeof(FLOAT_TYPE));
    if (!p)
	return (-14);		/* out of memory */

    ierr = NDI_INIT_FLOAT_VEC(sig_pnu, num_grps);
    if (ierr < 0)
	return (ierr);

    for (j = i; j < MAX_NU; j++) {
	GET_PNU_PR(state, p, j);	/* get probabilities */
	for (k = 0; k < num_grps; k++)	/* add to sum */
	    sig_pnu[k] += ndi_perm(j, i) * p[k];
    }

    for (k = 0; k < num_grps; k++)	/* normalize */
	sig_pnu[k] *= nu_sig_f[k] / (ndi_fact(i) * nu[k]);


    if (nu)
	ndi_free((void **) &nu);
    if (nu_sig_f)
	ndi_free((void **) &nu_sig_f);
    if (p)
	ndi_free((void **) &p);

    return (ierr);
}

/*********************************************************************/
/*********************************************************************/
