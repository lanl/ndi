
/*
 * File:	ndi_state_handle.c
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

/**********************************************************************/

struct ndi_state_handle {
/*    GENERAL STATE VARIABLES -- not changed when lib_type changes */
    short ndi_initialized;	/*  ndi_init called: 0=false, 1=true */
    FILE *ndi_dir;		/*  Gendir (directory) file */
    FILE *lib_stream;		/*  Current library file */
    char *open_lib_file;	/*  Path for current (open) library file */
    short *ndi_elements;	/*  Lookup table for element symbols */
    char *library_vers;		/*  Default library type: 
				   (mendf5,mendf6,rmoxsct,"") */
    char particle_type;		/*  Incident particle type: 
				   n-neutron, p-photon,0-tn */
    char library_type;		/*  m-multigroup */
    char mat_order;		/*  Transpose return matrices? 
				   d=no(default), t=yes(transpose) */
    char reac_id;		/*  Reaction identifier: 
				   m=MT (default), c=c-value */
    int curr_reac_index;	/*  Obsolete? */
    char nuc_data_type[2];	/*  Obsolete? */

/*    GENERAL STATE VARIABLES -- initialized when lib_type changes */
    int curr_part_index;	/*  index of current particle */
    char n_group[MAX_NGROUP_STR_LEN];	/*  Type of group structure 
					   to collapse to */
    int grp_num_new;		/*  Number of groups to collapse to */


/* Multigroup data */
    isotope_status iso_stat;	/*  Struct containing isotope state */
    int *new_ebounds;		/*  Index of new ebound group structure, 
				   allocated in ndi_set_e_grp  */
    short collapse_flag;	/*  Collapsing required? 0=no; -1=yes  */
    ndi_float64 *lib_weight;	/*  Array of weights read in for current nuc */
    ndi_float64 *lib_ebounds;	/*  Array of ebounds read in for current nuc */
    char trans_corr;		/*  Transport correction: " "=no correction, 
				   d=diagonal, b=Bell-Hansen-Sandmeier */
    int trans_corr_order;	/*  Transport correction order */
    int no_norm;		/*  Normalize chi's and edists: 
				   0=normalize; -1=do not normalize */
    char def_fiss_prod_id[20];	/* Value of default fiss. prod. ZAID  */


/* TN data */
    tn_status tn_stat;
    int tn_num_grps;
    int sec_part_filled;	/*  Number of secondary particles 
				   which have edist data */
    double *tn_ebounds;
    double default_temp;

/* TN reaction data */
    tnreac_status tnreac_stat;

/* Chain data */
    chain_status chain_stat;

/* TN reaction data */
    char tn_dataset[60];	/* Name of current set of TN 
				   reaction data */
    char tn_reac_info[MAX_IN_STR_LEN];	/* Information about current TN 
					   reaction dataset */
    int tn_reac_num;		/* Number of reactions in TN 
				   reaction dataset */
/*    int sec_part_filled;              Number of secondary particles 
					which have edist data */
    char *tn_list_reac[MAX_TN_REAC];	/* List of current set of TN 
					   reaction zaids */
    stat_flag *tn_reactants;
    stat_flag *tn_rprods;

/* Depletion data */
    int ndi_depl_set;		/* Is depletion set? 
				   True = -1; False= 0 */
    char depl_dataset[60];	/* Name of current set of depletion 
				   data */
    char depl_info[MAX_IN_STR_LEN];	/* Information about current 
					   depletion dataset */
    int num_target;		/* Number of targets in 
				   depletion dataset */
    int chain_filled;		/* Number of depletion targets 
				   whose data has been read */
    depl_target_struct *depl_target;
};

/* 
   Allocate ndi_state_handle and initialize to same state as initial 
   global state in ndi_state.c.  Actually initialization of state by  
   calls to:

   ndi_init();
   ndi_set_option(NDI_LIB_TYPE...;
   ndi_set_option(NDI_LIBRARY...;
   ndi_set_isotope(zaid);

   when state is active
*/
ndi_state_handle *
ndi_state_handle_new(void)
{
    ndi_state_handle *this;

    this = (ndi_state_handle *) malloc(sizeof(ndi_state_handle));
    assert(this);

    this->ndi_initialized = 0;
    this->ndi_dir = NULL;
    this->lib_stream = NULL;
    this->open_lib_file = NULL;
    this->ndi_elements = NULL;
    this->library_vers = NULL;
    this->particle_type = ' ';
    this->library_type = ' ';
    this->mat_order = 'd';
    this->reac_id = 'm';
    this->curr_reac_index = -1;
    strcpy(this->nuc_data_type, "");

    this->curr_part_index = -1;
    strcpy(this->n_group, "default");
    this->grp_num_new = 0;

    this->iso_stat.sec_part = NULL;
    this->iso_stat.num_sec_parts = -1;
    strcpy(this->iso_stat.zaid, "");
    strcpy(this->iso_stat.info, "");
    strcpy(this->iso_stat.date_processed, "");
    strcpy(this->iso_stat.date_source, "");
    strcpy(this->iso_stat.library_name, "");
    this->iso_stat.set_flag = 0;
    this->iso_stat.fissionable = 0;
    this->iso_stat.num_grps = -1;
    this->iso_stat.temp = -1.0;
    this->iso_stat.sig_0 = -1.0;
    this->iso_stat.at_wgt = -1.0;
    this->iso_stat.awr = -1.0;
    this->iso_stat.up_grps = -1;
    this->iso_stat.down_grps = -1;
    this->iso_stat.last_grp = -1;
    this->iso_stat.pn_order = -1;
    this->iso_stat.e_bounds.length = 0;
    this->iso_stat.e_bounds.file_ptr = 0;
    this->iso_stat.wgts.length = 0;
    this->iso_stat.wgts.file_ptr = 0;
    this->iso_stat.vel.length = 0;
    this->iso_stat.vel.file_ptr = 0;
    this->iso_stat.heat.length = 0;
    this->iso_stat.heat.file_ptr = 0;
    this->iso_stat.kerma.length = 0;
    this->iso_stat.kerma.file_ptr = 0;
    this->iso_stat.sig_tot.length = 0;
    this->iso_stat.sig_tot.file_ptr = 0;
    this->iso_stat.nu_tot.length = 0;
    this->iso_stat.nu_tot.file_ptr = 0;
    this->iso_stat.nu_pr.length = 0;
    this->iso_stat.nu_pr.file_ptr = 0;
    this->iso_stat.nu_del.length = 0;
    this->iso_stat.nu_del.file_ptr = 0;
    this->iso_stat.nu_sig_f_tot.length = 0;
    this->iso_stat.nu_sig_f_tot.file_ptr = 0;
    this->iso_stat.nu_sig_f_pr.length = 0;
    this->iso_stat.nu_sig_f_pr.file_ptr = 0;
    this->iso_stat.nu_sig_f_del.length = 0;
    this->iso_stat.nu_sig_f_del.file_ptr = 0;
    this->iso_stat.chi_vec_tot.length = 0;
    this->iso_stat.chi_vec_tot.file_ptr = 0;
    this->iso_stat.chi_vec_pr.length = 0;
    this->iso_stat.chi_vec_pr.file_ptr = 0;
    this->iso_stat.chi_vec_del.length = 0;
    this->iso_stat.chi_vec_del.file_ptr = 0;
    this->iso_stat.chi_mat_tot.length = 0;
    this->iso_stat.chi_mat_tot.file_ptr = 0;
    this->iso_stat.chi_mat_pr.length = 0;
    this->iso_stat.chi_mat_pr.file_ptr = 0;
    this->iso_stat.chi_mat_del.length = 0;
    this->iso_stat.chi_mat_del.file_ptr = 0;
    this->iso_stat.num_reac = -1;
    this->iso_stat.fiss_q.length = 0;
    this->iso_stat.fiss_q.file_ptr = 0;
    this->iso_stat.norm.length = 0;
    this->iso_stat.norm.file_ptr = 0;
    this->iso_stat.num_sec_parts = -1;
    this->iso_stat.pn_down_full.num_objs = -1;
    this->iso_stat.pn_down_full.stat = NULL;
    this->iso_stat.pn_full.num_objs = -1;
    this->iso_stat.pn_full.stat = NULL;
    this->iso_stat.sig_reac.num_objs = -1;
    this->iso_stat.sig_reac.stat = NULL;
    this->iso_stat.rprod_all.num_objs = -1;
    this->iso_stat.reac_mt = NULL;
    this->iso_stat.reac_q = NULL;
    this->iso_stat.rprod_all.stat = NULL;
    this->iso_stat.rprod_rmo.num_objs = -1;
    this->iso_stat.rprod_rmo.stat = NULL;
    this->iso_stat.rprod_few.num_objs = -1;
    this->iso_stat.rprod_few.stat = NULL;

    this->new_ebounds = NULL;
    this->lib_weight = NULL;
    this->lib_ebounds = NULL;
    this->tn_ebounds = NULL;
    this->depl_target = NULL;

    return this;
}

/*
  Return a shallow copy of that
*/
ndi_state_handle *
ndi_state_handle_copy(ndi_state_handle * that)
{
    ndi_state_handle *this;
    int i;

    this = ndi_state_handle_new();

    this->ndi_initialized = that->ndi_initialized;
    this->ndi_dir = that->ndi_dir;
    this->lib_stream = that->lib_stream;
    this->open_lib_file = that->open_lib_file;
    this->ndi_elements = that->ndi_elements;
    this->library_vers = that->library_vers;
    this->particle_type = that->particle_type;
    this->library_type = that->library_type;
    this->mat_order = that->mat_order;
    this->reac_id = that->reac_id;
    this->curr_reac_index = that->curr_reac_index;
    for (i = 0; i < 2; i++)
	this->nuc_data_type[i] = that->nuc_data_type[i];

    this->curr_part_index = that->curr_part_index;
    for (i = 0; i < MAX_NGROUP_STR_LEN; i++)
	this->n_group[i] = that->n_group[i];
    this->grp_num_new = that->grp_num_new;

    this->iso_stat = that->iso_stat;
    this->new_ebounds = that->new_ebounds;
    this->collapse_flag = that->collapse_flag;
    this->lib_weight = that->lib_weight;
    this->lib_ebounds = that->lib_ebounds;
    this->trans_corr = that->trans_corr;
    this->trans_corr_order = that->trans_corr_order;
    this->no_norm = that->no_norm;
    for (i = 0; i < 20; i++)
	this->def_fiss_prod_id[i] = that->def_fiss_prod_id[i];

    this->tn_stat = that->tn_stat;
    this->tn_num_grps = that->tn_num_grps;
    this->sec_part_filled = that->sec_part_filled;
    this->tn_ebounds = that->tn_ebounds;
    this->default_temp = that->default_temp;

    this->tnreac_stat = that->tnreac_stat;

    this->chain_stat = that->chain_stat;

    for (i = 0; i < 60; i++)
	this->tn_dataset[i] = that->tn_dataset[i];
    for (i = 0; i < MAX_IN_STR_LEN; i++)
	this->tn_reac_info[i] = that->tn_reac_info[i];
    this->tn_reac_num = that->tn_reac_num;
    this->sec_part_filled = that->sec_part_filled;
    for (i = 0; i < MAX_TN_REAC; i++)
	this->tn_list_reac[i] = that->tn_list_reac[i];
    this->tn_reactants = that->tn_reactants;
    this->tn_rprods = that->tn_rprods;

    this->ndi_depl_set = that->ndi_depl_set;
    for (i = 0; i < 60; i++)
	this->depl_dataset[i] = that->depl_dataset[i];
    for (i = 0; i < MAX_IN_STR_LEN; i++)
	this->depl_info[i] = that->depl_info[i];
    this->num_target = that->num_target;
    this->chain_filled = that->chain_filled;
    this->depl_target = that->depl_target;

    return this;
}

/*
  Deallocate ndi_state_handle.  Actual deallocation of state by call to:

  ndi_close();

  when state is active
*/
void
ndi_state_handle_free(ndi_state_handle * this)
{
    free((void *) this);
}

/*
  Swap state in this with global state.  
*/
void
ndi_state_handle_swap(ndi_state_handle * this)
{

    ndi_state_handle *temp;
    int i;

    /* shallow copy this to temp */
    temp = ndi_state_handle_copy(this);

    /* shallow copy global to this */
    this->ndi_initialized = ndi_initialized;
    this->ndi_dir = ndi_dir;
    this->lib_stream = lib_stream;
    this->open_lib_file = open_lib_file;
    this->ndi_elements = ndi_elements;
    this->library_vers = library_vers;
    this->particle_type = particle_type;
    this->library_type = library_type;
    this->mat_order = mat_order;
    this->reac_id = reac_id;
    this->curr_reac_index = curr_reac_index;
    for (i = 0; i < 2; i++)
	this->nuc_data_type[i] = nuc_data_type[i];

    this->curr_part_index = curr_part_index;
    for (i = 0; i < MAX_NGROUP_STR_LEN; i++)
	this->n_group[i] = n_group[i];
    this->grp_num_new = grp_num_new;

    this->iso_stat = iso_stat;
    this->new_ebounds = new_ebounds;
    this->collapse_flag = collapse_flag;
    this->lib_weight = lib_weight;
    this->lib_ebounds = lib_ebounds;
    this->trans_corr = trans_corr;
    this->trans_corr_order = trans_corr_order;
    this->no_norm = no_norm;
    for (i = 0; i < 20; i++)
	this->def_fiss_prod_id[i] = def_fiss_prod_id[i];

    this->tn_stat = tn_stat;
    this->tn_num_grps = tn_num_grps;
    this->sec_part_filled = sec_part_filled;
    this->tn_ebounds = tn_ebounds;
    this->default_temp = default_temp;

    this->tnreac_stat = tnreac_stat;

    this->chain_stat = chain_stat;

    for (i = 0; i < 60; i++)
	this->tn_dataset[i] = tn_dataset[i];
    for (i = 0; i < MAX_IN_STR_LEN; i++)
	this->tn_reac_info[i] = tn_reac_info[i];
    this->tn_reac_num = tn_reac_num;
    this->sec_part_filled = sec_part_filled;
    for (i = 0; i < MAX_TN_REAC; i++)
	this->tn_list_reac[i] = tn_list_reac[i];
    this->tn_reactants = tn_reactants;
    this->tn_rprods = tn_rprods;

    this->ndi_depl_set = ndi_depl_set;
    for (i = 0; i < 60; i++)
	this->depl_dataset[i] = depl_dataset[i];
    for (i = 0; i < MAX_IN_STR_LEN; i++)
	this->depl_info[i] = depl_info[i];
    this->num_target = num_target;
    this->chain_filled = chain_filled;
    this->depl_target = depl_target;

    /* shallow copy temp to global */
    ndi_initialized = temp->ndi_initialized;
    ndi_dir = temp->ndi_dir;
    lib_stream = temp->lib_stream;
    open_lib_file = temp->open_lib_file;
    ndi_elements = temp->ndi_elements;
    library_vers = temp->library_vers;
    particle_type = temp->particle_type;
    library_type = temp->library_type;
    mat_order = temp->mat_order;
    reac_id = temp->reac_id;
    curr_reac_index = temp->curr_reac_index;
    for (i = 0; i < 2; i++)
	nuc_data_type[i] = temp->nuc_data_type[i];

    curr_part_index = temp->curr_part_index;
    for (i = 0; i < MAX_NGROUP_STR_LEN; i++)
	n_group[i] = temp->n_group[i];
    grp_num_new = temp->grp_num_new;

    iso_stat = temp->iso_stat;
    new_ebounds = temp->new_ebounds;
    collapse_flag = temp->collapse_flag;
    lib_weight = temp->lib_weight;
    lib_ebounds = temp->lib_ebounds;
    trans_corr = temp->trans_corr;
    trans_corr_order = temp->trans_corr_order;
    no_norm = temp->no_norm;
    for (i = 0; i < 20; i++)
	def_fiss_prod_id[i] = temp->def_fiss_prod_id[i];

    tn_stat = temp->tn_stat;
    tn_num_grps = temp->tn_num_grps;
    sec_part_filled = temp->sec_part_filled;
    tn_ebounds = temp->tn_ebounds;
    default_temp = temp->default_temp;

    tnreac_stat = temp->tnreac_stat;

    chain_stat = temp->chain_stat;

    for (i = 0; i < 60; i++)
	tn_dataset[i] = temp->tn_dataset[i];
    for (i = 0; i < MAX_IN_STR_LEN; i++)
	tn_reac_info[i] = temp->tn_reac_info[i];
    tn_reac_num = temp->tn_reac_num;
    sec_part_filled = temp->sec_part_filled;
    for (i = 0; i < MAX_TN_REAC; i++)
	tn_list_reac[i] = temp->tn_list_reac[i];
    tn_reactants = temp->tn_reactants;
    tn_rprods = temp->tn_rprods;

    ndi_depl_set = temp->ndi_depl_set;
    for (i = 0; i < 60; i++)
	depl_dataset[i] = temp->depl_dataset[i];
    for (i = 0; i < MAX_IN_STR_LEN; i++)
	depl_info[i] = temp->depl_info[i];
    num_target = temp->num_target;
    chain_filled = temp->chain_filled;
    depl_target = temp->depl_target;

    /* delete temp */
    ndi_state_handle_free(temp);
}
