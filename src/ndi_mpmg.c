
/*
 * File:	ndi_mpmg.c
 * Description:	State handle ADT
 * Time-stamp:	<Wed Sep 24 15:20:14 MDT 2003>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_state_handle.h"
#include "ndi.h"
#include "ndi_mpmg.h"

/**********************************************************************/

struct ndi_mpmg {
    ndi_state_handle *n;	/* neutron state */
    ndi_state_handle *g;	/* gamma state */
};

/* 
   Allocate ndi_mpmg and initialize to same state as initial 
   global state in ndi_state.c.  Actually initialization of state by  
   calls to:

   ndi_init();
   ndi_set_option(NDI_LIB_TYPE...;
   ndi_set_option(NDI_LIBRARY...;
   ndi_set_isotope(zaid);

   when state is active
*/
ndi_mpmg *ndi_mpmg_new(char *zaid)
{
    ndi_mpmg *this;
    char photon_zaid[15];
    int ierr;

    this = (ndi_mpmg *) malloc(sizeof(ndi_mpmg));
    assert(this);

    this->n = ndi_state_handle_new();	/* create neutron state handle */
    ndi_state_handle_swap(this->n);	/* swap it into context */
    ierr = ndi_c_init();	/* initialize it */
    assert(ierr == 0);
    ierr = ndi_c_set_option(NDI_LIB_TYPE, "multigroup_neutron");
    assert(ierr == 0);
    ierr = ndi_c_set_option(NDI_COLLAPSE, "21_lanl");
    assert(ierr == 0);
    ierr = ndi_c_set_option(NDI_LIBRARY, "mendf6");
    assert(ierr == 0);
    ierr = ndi_c_set_isotope(zaid);
    assert(ierr == 0);
    ndi_state_handle_swap(this->n);	/* swap it out of context */

    this->g = ndi_state_handle_new();
    ndi_state_handle_swap(this->g);	/* swap it into context */
    ierr = ndi_c_init();	/* initialize it */
    assert(ierr == 0);
    ierr = ndi_c_set_option(NDI_LIB_TYPE, "multigroup_photon");
    assert(ierr == 0);
    ierr = ndi_c_set_option(NDI_COLLAPSE, "default");
    assert(ierr == 0);
    ierr = ndi_c_set_option(NDI_LIBRARY, "mendf6g");
    assert(ierr == 0);
    sprintf(photon_zaid, "%-ld000", strtol(zaid, NULL, 0) / 1000);
    ierr = ndi_c_set_isotope(photon_zaid);
    assert(ierr == 0);
    ndi_state_handle_swap(this->g);

    return this;
}

/*
  Deallocate ndi_mpmg.  Actual deallocation of state by call to:

  ndi_close();

  when state is active
*/
void ndi_mpmg_free(ndi_mpmg * this)
{
    int ierr;

    ndi_state_handle_swap(this->g);
    ierr = ndi_c_close();
    assert(ierr == 0);
    ndi_state_handle_swap(this->g);
    ndi_state_handle_free(this->g);

    ndi_state_handle_swap(this->n);
    ierr = ndi_c_close();
    assert(ierr == 0);
    ndi_state_handle_swap(this->n);
    ndi_state_handle_free(this->n);

    free((void *) this);
}

#ifdef foo
static int get_int_val(ndi_mpmg * this, int key, int *val)
{
    int ierr = 0;		/* fatal < 0 < warning */
    ndi_state_handle *n;
    ndi_state_handle *g;

    *val = 0;			/* default return value */
    n = this->n;
    g = this->g;


    /* check ndi_mpmg_new called */
    if (n->ndi_initialized == 0 || g->ndi_initialized == 0)
	return (-2);

    /* check isotope set */
    if (key != NDI_NUM_TARGET && n->iso_stat.set_flag == 0)
	return (-7);

    switch (key) {
    case NDI_NUM_GRPS:		/* number of groups */
	if (n->collapse_flag != 0)
	    *val = n->grp_num_new;
	else
	    *val = n->iso_stat.num_grps;

	if (g->collapse_flag != 0)
	    *val += g->grp_num_new;
	else
	    *val += g->iso_stat.num_grps;
	if (*val < 0) {
	    *val = 0;
	    ierr = -9;
	}
	break;

    case NDI_FISSIONABLE:	/* fissionable data? */

	*val = n->iso_stat.fissionable;
	break;

    case NDI_UP_GRPS:		/* number of upscatter groups */

	if (n->iso_stat.up_grps >= 0)
	    *val = n->iso_stat.up_grps;	/* correct? */
	else
	    ierr = -9;
	break;

    case NDI_DOWN_GRPS:	/* number of downscatter groups */

	if (n->iso_stat.down_grps >= 0)
	    *val = n->iso_stat.down_grps;	/* correct? */
	else
	    ierr = -9;
	break;

    case NDI_LAST_GRP:		/* NGK kludge */
	if (n->iso_stat.last_grp >= 0)
	    *val = n->iso_stat.last_grp;
	else
	    ierr = -9;
	break;

    case NDI_NUM_REAC:		/* Number of edits  */
	if (n->iso_stat.num_reac >= 0)
	    *val = n->iso_stat.num_reac;
	else
	    ierr = -9;
	break;

    case NDI_NUM_PNU:
	*val = 10;		/* number of fission neutron probabilities */
	break;

    case NDI_NUM_GRPS_CURR_PART:	/* Number of cp groups */
	if (n->iso_stat.sec_part &&
	    (n->curr_part_index >= 0) &&
	    (n->curr_part_index < n->sec_part_filled)) {
	    if (n->iso_stat.sec_part[curr_part_index].num_grps >= 0) {
		*val = n->iso_stat.sec_part[curr_part_index].num_grps;
	    } else {
		ierr = -9;
	    }
	} else {
	    ierr = -999;	/* Error -- bad particle? */
	}

	break;

    case NDI_NUM_TARGET:	/* 
				   if ( ! ndi_depl_set ) {
				   ierr = ndi_set_default_depl();
				   }
				   if ( num_target >= 0 ) {
				   *ret_val = num_target;
				   } else {
				   *ret_val = 0;
				   ierr = -9;
				   } 
				 */
	break;

    case NDI_NUM_RPRODS:	/*
				   if ( ! ndi_depl_set ) {
				   ierr = ndi_set_default_depl();
				   }
				   if ( num_target <= 0 ) {
				   return(-9);
				   }
				   ierr =  ndi_get_num_rprods( ret_val );
				 */
	break;

    case NDI_ATOMIC_NUM:	/* Atomic number of current isotope -- Z */
	ierr = ndi_parse_mg_zaid(n->iso_stat.zaid, &z, &a);
	*val = z;
	break;

    case NDI_MASS_NUM:		/* Mass number of current isotope -- A */
	ierr = ndi_parse_mg_zaid(n->iso_stat.zaid, &z, &a);
	*val = a;

	break;


      /***********************************************************************/
	/* This section looks at non-standard isotope data (length & num_objs) */
      /***********************************************************************/

    case NDI_PN_ORDER:
	/* Number of scattering matrices  */
	if (n->iso_stat.pn_order >= 0)
	    *val = iso_stat.pn_order;
	else
	    ierr = -9;
	break;

    case NDI_NUM_SEC_PARTS:
	/* Number of secondary particles  */
	if (n->iso_stat.num_sec_parts > 0) {
	    *val = n->iso_stat.num_sec_parts;
	} else {
	    ierr = -9;
	}
	break;


    case NDI_PN_ORDER_CURR_PART:
	/* Number of production matrices--for current particle */
	if (n->curr_part_index >= 0) {
	    if (n->iso_stat.sec_part[curr_part_index].pn_prod_full.
		num_objs > 0) {
		*val =
		    n->iso_stat.sec_part[curr_part_index].pn_prod_full.
		    num_objs;
	    } else {
		ierr = -9;
	    }
	} else {
	    /* Error -- bad particle? */
	    ierr = -999;
	}

	break;

      /**************  UNKNOWN KEYWORD  *****************/

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}

static int GET_FLOAT_VAL(ndi_mpmg * this, int key, FLOAT_TYPE * val)
{

    int ierr = 0;
    ndi_state_handle *n;
    ndi_state_handle *g;

    *val = 0.0			/* default value */
	n = this->n;
    g = this->g;


    /* Check that isotope is set */
    if (n->iso_stat.set_flag == 0 || g->iso_stat.set_flag == 0) {
	return (-7);
    }

    /*************************/
    /*   Process input key   */
    /*************************/
    switch (key) {

    case NDI_TEMP:		/*  Temperature  */
	if (n->iso_stat.temp >= 0)
	    *val = (FLOAT_TYPE) n->iso_stat.temp;
	else
	    ierr = -9;
	break;

    case NDI_SIG_0:		/* Sigma zero  */

	if (iso_stat.sig_0 >= 0)
	    *val = (FLOAT_TYPE) n->iso_stat.sig_0;
	else
	    ierr = -9;
	break;

    case NDI_AT_WGT:		/* Atomic weight  */
	if (n->iso_stat.at_wgt >= 0)
	    *val = (FLOAT_TYPE) n->iso_stat.at_wgt;
	else
	    ierr = -9;
	break;

    case NDI_AWR:		/* Atomic weight ratio  */

	if (n->iso_stat.awr >= 0)
	    *val = (FLOAT_TYPE) n->iso_stat.awr;
	else
	    ierr = -9;
	break;

      /*******************  UNKNOWN KEYWORD  ********************/

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}

static int GET_FLOAT_VEC(ndi_mpmg * this, int key, FLOAT_TYPE * vec,
			 int vec_size)
{
    int ierr = 0;
    int num_elements;
    FLOAT_TYPE *curr_flt = vec;
    double *source_ptr = NULL;
    int n;
    ndi_state_handle *n;
    ndi_state_handle *g;
    int n_size;
    int g_size;


    /* Check that isotope is set */
    if (n->iso_stat.set_flag == 0 || g->iso_stat.set_flag == 0)
	return (-7);

    /*************************/
    /*   Process input key   */
    /*************************/

    switch (key) {

    case NDI_E_BOUNDS:		/*  Energy boundaries  */
	ndi_state_handle_swap(n);
	n_size = ndi_get_size(NDI_E_BOUNDS, &i);
	assert(n_size <= vec_size);
	ierr = NDI_GET_FLOAT_VEC(NDI_E_BOUNDS, vec, n_size);
	ndi_state_handle_swap(n);

	ndi_state_handle_swap(g);
	g_size = ndi_get_size(NDI_E_BOUNDS, &i);
	assert(nsize + g_size <= vec_size);
	ierr += NDI_GET_FLOAT_VEC(NDI_E_BOUNDS, vec + n_size, g_size);
	ndi_state_handle_swap(g);
	break;

    case NDI_E_AVG:		/* Average group energies */
	/*  Check bounds */
	ierr = NDI_CALC_E_AVG(return_vec, return_size);
	break;

    case NDI_ENERGY_N:		/* Energy given to neutrons */
	/*  Check bounds */
	ierr = NDI_CALC_E_N(return_vec, return_size);
	break;

    case NDI_ENERGY_AVAIL:	/* Energy available */
	/*  Check bounds */
	ierr = NDI_CALC_E_AVAIL(return_vec, return_size);
	break;

    case NDI_WGTS:

	ndi_state_handle_swap(n);
	n_size = iso_stat.num_grps;
	/*  Check bounds */
	ierr = GET_SIMPLE_FLOAT_VEC(lib_stream, iso_stat.wgts.file_ptr,
				    3, n_size, vec, n_size);
	ndi_state_handle_swap(n);

	ndi_state_handle_swap(g);
	g_size = iso_stat.num_grps;
	/*  Check bounds */
	ierr = GET_SIMPLE_FLOAT_VEC(lib_stream, iso_stat.wgts.file_ptr,
				    3, g_size, vec + n_size, g_size);
	ndi_state_handle_swap(n);
	break;

    case NDI_VEL:		/* Velocities */
	num_elements = iso_stat.num_grps;
	/*  Check bounds */
	ierr = GET_SIMPLE_FLOAT_VEC(lib_stream, iso_stat.vel.file_ptr,
				    1, num_elements,
				    return_vec, return_size);
	break;

    case NDI_HEATING:		/* Heating */
	ierr =
	    NDI_GET_HEATING(&iso_stat.heat, &iso_stat.kerma, return_vec,
			    return_size);
	break;

    case NDI_KERMA:		/* kerma */
	/* Units are MeV*barn -- this is just heating * tot_sigma */
	ierr =
	    NDI_GET_KERMA(&iso_stat.kerma, &iso_stat.heat, return_vec,
			  return_size);
	break;

    case NDI_HEATING_CURR_PART:	/* Partial heating for current particle */
	if (curr_part_index >= 0) {
	    ierr =
		NDI_GET_HEATING(&iso_stat.sec_part[curr_part_index].
				heating,
				&iso_stat.sec_part[curr_part_index].kerma,
				return_vec, return_size);
	} else {
	    /* Error -- bad particle? */
	    ierr = -999;
	}
	break;

    case NDI_KERMA_CURR_PART:	/* Partial kerma for current particle */
	/* Units are MeV*barn -- this is just heating_curr_particle * tot_sigma */
	if (curr_part_index >= 0) {
	    ierr = NDI_GET_KERMA(&iso_stat.sec_part[curr_part_index].kerma,
				 &iso_stat.sec_part[curr_part_index].
				 heating, return_vec, return_size);
	} else {
	    /* Error -- bad particle? */
	    ierr = -999;
	}
	break;

    case NDI_SIG_TOT:		/* Sigma total */
	num_elements = iso_stat.num_grps;
	/*  Check bounds */
	ierr = GET_SIMPLE_FLOAT_VEC(lib_stream, iso_stat.sig_tot.file_ptr,
				    0, num_elements,
				    return_vec, return_size);

/*       printf("SIG_TOT RETURNED\n"); */
	if ((ierr >= 0) && (trans_corr != ' ')) {
	    ierr = SIG_TOT_TRANS_CORR(return_vec, return_size);
	}
	break;

    case NDI_NU_TOT:		/* nubar total */
	num_elements = return_size;
	ierr = NDI_GET_NU(&iso_stat, 't', return_vec, &num_elements);
	(void) check_fissionable_err(&ierr, iso_stat.fissionable);
	break;

    case NDI_NU_PR:		/* nubar prompt */
	num_elements = return_size;
	ierr = NDI_GET_NU(&iso_stat, 'p', return_vec, &num_elements);
	(void) check_fissionable_err(&ierr, iso_stat.fissionable);
	break;

    case NDI_NU_DEL:		/* nubar delayed */
	num_elements = return_size;
	ierr = NDI_GET_NU(&iso_stat, 'd', return_vec, &num_elements);
	(void) check_fissionable_err(&ierr, iso_stat.fissionable);
	break;

    case NDI_NU_SIG_F_TOT:	/* nubar total sigma fission */
	num_elements = return_size;
	ierr = NDI_GET_NUSIGF('t', return_vec, &num_elements);
	(void) check_fissionable_err(&ierr, iso_stat.fissionable);
	break;

    case NDI_NU_SIG_F_PR:	/* nubar prompt sigma fission */
	num_elements = return_size;
	ierr = NDI_GET_NUSIGF('p', return_vec, &num_elements);
	(void) check_fissionable_err(&ierr, iso_stat.fissionable);
	break;

    case NDI_NU_SIG_F_DEL:	/* nubar delayed sigma fission */
	num_elements = return_size;
	ierr = NDI_GET_NUSIGF('d', return_vec, &num_elements);
	(void) check_fissionable_err(&ierr, iso_stat.fissionable);
	break;

    case NDI_CHI_VEC_TOT:	/* chi total vector */
	num_elements = return_size;
	/*  Check bounds */
	ierr = NDI_GET_CHI_VEC(&iso_stat, 't', return_vec, &num_elements);
	(void) check_fissionable_err(&ierr, iso_stat.fissionable);
	break;

    case NDI_CHI_VEC_PR:	/* chi prompt vector */
	num_elements = return_size;
	/*  Check bounds */
	ierr = NDI_GET_CHI_VEC(&iso_stat, 'p', return_vec, &num_elements);
	(void) check_fissionable_err(&ierr, iso_stat.fissionable);
	break;

    case NDI_CHI_VEC_DEL:	/* chi delayed vector */
	num_elements = return_size;
	/*  Check bounds */
	ierr = NDI_GET_CHI_VEC(&iso_stat, 'd', return_vec, &num_elements);
	(void) check_fissionable_err(&ierr, iso_stat.fissionable);
	break;

    case NDI_CHI_MAT_TOT:	/* chi total matrix */
	num_elements = iso_stat.num_grps;
	/*  Check bounds */
	ierr = GET_CHI_MAT(lib_stream, iso_stat.chi_mat_tot.file_ptr,
			   iso_stat.chi_vec_tot.file_ptr, return_vec,
			   num_elements, return_size);
	if (ierr == -9) {
	    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);
	    ierr = 9;
	}
	(void) check_fissionable_err(&ierr, iso_stat.fissionable);
	break;

    case NDI_CHI_MAT_PR:	/* chi prompt matrix */
	num_elements = iso_stat.num_grps;
	/*  Check bounds */
	ierr = GET_CHI_MAT(lib_stream, iso_stat.chi_mat_pr.file_ptr,
			   iso_stat.chi_vec_pr.file_ptr, return_vec,
			   num_elements, return_size);
	if (ierr == -9) {
	    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);
	    ierr = 9;
	}
	(void) check_fissionable_err(&ierr, iso_stat.fissionable);
	break;

    case NDI_CHI_MAT_DEL:	/* chi delayed matrix */
	num_elements = iso_stat.num_grps;
	/*  Check bounds */
	ierr = GET_CHI_MAT(lib_stream, iso_stat.chi_mat_del.file_ptr,
			   iso_stat.chi_vec_del.file_ptr, return_vec,
			   num_elements, return_size);
	if (ierr == -9) {
	    (void) NDI_INIT_FLOAT_VEC(return_vec, return_size);
	    ierr = 9;
	}
	(void) check_fissionable_err(&ierr, iso_stat.fissionable);
	break;

    case NDI_NORM:		/* Normalization factors */
	num_elements = NUM_NORM;
	/*  Check bounds */
	if (iso_stat.norm.file_ptr) {
	    ierr = NDI_READ_FLOAT_BLOCK(lib_stream, iso_stat.norm.file_ptr,
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
	if (iso_stat.fiss_q.file_ptr) {
	    ierr =
		NDI_READ_FLOAT_BLOCK(lib_stream, iso_stat.fiss_q.file_ptr,
				     NUM_FISSION_Q, return_vec,
				     return_size);
	} else {
	    /* Error: data not available */
	    ierr = -9;
	}
	(void) check_fissionable_err(&ierr, iso_stat.fissionable);
	break;

    case NDI_REAC_Q:		/* edit reaction q */
	/*  Check bounds */
	if (iso_stat.reac_q) {
	    source_ptr = iso_stat.reac_q;
	    for (n = 0; n < iso_stat.num_reac; n++) {
		*curr_flt++ = (FLOAT_TYPE) * source_ptr++;
	    }
	} else {
	    /* Error: data not available */
	    ierr = -9;
	}
	break;

    case NDI_E_BOUNDS_CURR_PART:	/* secondary particle energy boundaries */
	/*  Check bounds */

	if (curr_part_index >= 0) {
	    num_elements = iso_stat.sec_part[curr_part_index].num_grps + 1;

	    if (iso_stat.sec_part[curr_part_index].e_bounds.file_ptr) {
		ierr = NDI_READ_FLOAT_BLOCK(lib_stream,
					    iso_stat.
					    sec_part[curr_part_index].
					    e_bounds.file_ptr,
					    num_elements, return_vec,
					    return_size);
	    } else {
		/* Error: data not available */
		ierr = -9;
	    }
	} else {
	    /* Error -- bad particle? */
	    ierr = -999;
	}
	break;

    case NDI_TOT_DEPL_XS:
	ierr = GET_DEPL_XS(&iso_stat, return_vec, return_size);
	break;

      /**********  UNKNOWN KEYWORD  ***********/

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}
#endif
