
/*
 * File:	ndi_set0.c
 * Description:	Internal function--initialize iso_stat, lib_ebounds, lib_weight.
 * Time-stamp:	<1999-11-19 17:29:24 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static int set0_mg(void); 
static int set0_tn(void);
static int set0_tnreac(void);
static int set0_depl(void);

static int set0_interp_param ( interp_params *param, int reset_flag ); 
static int set0_edist_data ( edist_data *distr, int reset_flag ); 
/* static int set0_interp_param ( interp_params *param ); */
/* static int set0_edist_data ( edist_data *distr ); */

/**********************************************************************/

int	ndi_set0( char l_type)

/*  Purpose: Initialize iso_stat (isotope status) structure.  Initialize
    isotope-status values and free pointers. Initialize lib_ebounds and
    lib_weight.

    Parameters:
    l_type -- in: library type;'m'=multigroup, 's'=TNdata, 'd'=dosimetry.

    Return values:    Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok.  External
    pointer variables lib_ebounds and lib_weight is deallocated and set 
    to NULL.  Elements of iso_stat set to zero (numbers), empty string
    (character arrays), or deallocated and set to NULL (pointers).


***********************************************************************/

{
   int ierr = 0;   /* Error flag: 0=no error,pos=warning, neg=error */

   if ( ( l_type == 'm' ) ||
	( l_type == 'd' ) ) {
      ierr = set0_mg();
   } else if ( l_type == 's' ) {
      ierr = set0_tn();
   } else if ( l_type == 'r' ) {
      ierr = set0_tnreac();
   } else if ( l_type == '0' ) {
      ierr = set0_depl();
   } else {
      /* invalid library type */
      ierr = -13;
   }

   return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int set0_mg(void) {

   int ierr = 0;         /* Error flag: 0=no error,pos=warning, neg=error */
   int i;


   /* Initialize secondary particle data */
   if ( (iso_stat.num_sec_parts > 0) && ( iso_stat.sec_part != NULL ) ){
      for ( i = 0; i < iso_stat.num_sec_parts; i++ ) {
	ierr = ndi_set0_mg_sec_part( &(iso_stat.sec_part[i]) );
      }
   }


     /* Free and set to null secondary particle info, if present */
     if (iso_stat.sec_part) {
       ndi_free( (void**)&iso_stat.sec_part);
       iso_stat.sec_part = NULL;
     }
     iso_stat.num_sec_parts = -1;


   (void)strncpy(iso_stat.zaid, "", sizeof(iso_stat.zaid) );
   (void)strncpy(iso_stat.info, "", sizeof(iso_stat.info) );
   (void)strncpy(iso_stat.date_processed, "", sizeof(iso_stat.date_processed) );
   (void)strncpy(iso_stat.date_source, "", sizeof(iso_stat.date_source) );
   (void)strncpy(iso_stat.library_name, "", sizeof(iso_stat.library_name) );
   iso_stat.set_flag = 0;
   iso_stat.fissionable = 0;
   iso_stat.num_grps = -1;
   iso_stat.temp = -1.0;
   iso_stat.sig_0 = -1.0;
   iso_stat.at_wgt = -1.0;
   iso_stat.awr = -1.0;
   iso_stat.up_grps = -1;
   iso_stat.down_grps = -1;
   iso_stat.last_grp = -1;
   iso_stat.pn_order = -1;
   iso_stat.e_bounds.length = 0;
   iso_stat.e_bounds.file_ptr = 0;
   iso_stat.wgts.length = 0;
   iso_stat.wgts.file_ptr = 0;
   iso_stat.vel.length = 0;
   iso_stat.vel.file_ptr = 0;
   iso_stat.heat.length = 0;
   iso_stat.heat.file_ptr = 0;
   iso_stat.kerma.length = 0;
   iso_stat.kerma.file_ptr = 0;
   iso_stat.sig_tot.length = 0;
   iso_stat.sig_tot.file_ptr = 0;
   iso_stat.nu_tot.length = 0;
   iso_stat.nu_tot.file_ptr = 0;
   iso_stat.nu_pr.length = 0;
   iso_stat.nu_pr.file_ptr = 0;
   iso_stat.nu_del.length = 0;
   iso_stat.nu_del.file_ptr = 0;
   iso_stat.nu_sig_f_tot.length = 0;
   iso_stat.nu_sig_f_tot.file_ptr = 0;
   iso_stat.nu_sig_f_pr.length = 0;
   iso_stat.nu_sig_f_pr.file_ptr = 0;
   iso_stat.nu_sig_f_del.length = 0;
   iso_stat.nu_sig_f_del.file_ptr = 0;
   iso_stat.chi_vec_tot.length = 0;
   iso_stat.chi_vec_tot.file_ptr = 0;
   iso_stat.chi_vec_pr.length = 0;
   iso_stat.chi_vec_pr.file_ptr = 0;
   iso_stat.chi_vec_del.length = 0;
   iso_stat.chi_vec_del.file_ptr = 0;
   iso_stat.chi_mat_tot.length = 0;
   iso_stat.chi_mat_tot.file_ptr = 0;
   iso_stat.chi_mat_pr.length = 0;
   iso_stat.chi_mat_pr.file_ptr = 0;
   iso_stat.chi_mat_del.length = 0;
   iso_stat.chi_mat_del.file_ptr = 0;
   iso_stat.num_reac = -1;
   iso_stat.fiss_q.length = 0;
   iso_stat.fiss_q.file_ptr = 0;
   iso_stat.norm.length = 0;
   iso_stat.norm.file_ptr = 0;
   iso_stat.num_sec_parts = -1;
   iso_stat.pn_down_full.num_objs = -1;
   if (iso_stat.pn_down_full.stat) ndi_free((void**)&iso_stat.pn_down_full.stat);
   iso_stat.pn_down_full.stat = NULL;
   iso_stat.pn_full.num_objs = -1;
   if (iso_stat.pn_full.stat) ndi_free((void**)&iso_stat.pn_full.stat);
   iso_stat.pn_full.stat = NULL;
   iso_stat.sig_reac.num_objs = -1;
   if (iso_stat.sig_reac.stat) ndi_free((void**)&iso_stat.sig_reac.stat);
   iso_stat.sig_reac.stat = NULL;
   iso_stat.rprod_all.num_objs = -1;
   if (iso_stat.reac_mt) ndi_free((void**)&iso_stat.reac_mt);
   iso_stat.reac_mt = NULL;
   if (iso_stat.reac_q) ndi_free((void**)&iso_stat.reac_q);
   iso_stat.reac_q = NULL;
   if (iso_stat.rprod_all.stat) ndi_free((void**)&iso_stat.rprod_all.stat);
   iso_stat.rprod_all.stat = NULL;
   iso_stat.rprod_rmo.num_objs = -1;
   if (iso_stat.rprod_rmo.stat) ndi_free((void**)&iso_stat.rprod_rmo.stat);
   iso_stat.rprod_rmo.stat = NULL;
   iso_stat.rprod_few.num_objs = -1;
   if (iso_stat.rprod_few.stat) ndi_free((void**)&iso_stat.rprod_few.stat);
   iso_stat.rprod_few.stat = NULL;

   return(ierr);
  
}

/**********************************************************************/
/**********************************************************************/
int ndi_set0_mg_sec_part( sec_part_mg *part )
{

  int ierr = 0;

  if (part) {
    strcpy( part->part_type, "" );
    strcpy( part->zaid, "" );
    part->num_grps = -1;
    part->pn_order = -1;
    part->e_bounds.length = 0;
    part->e_bounds.file_ptr = 0;
    part->heating.length = 0;
    part->heating.file_ptr = 0;
    part->kerma.length = 0;
    part->kerma.file_ptr = 0;
    if ( part->pn_prod_full.num_objs > 0 ) {
      if ( part->pn_prod_full.stat ) {
	ndi_free((void**)&part->pn_prod_full.stat);
      }
    }
    part->pn_prod_full.num_objs = -1;
    part->pn_prod_full.stat = NULL;
  }

  return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int set0_tn(void) {
   
   int ierr = 0;         /* Error flag: 0=no error,pos=warning, neg=error */
   int i = 0;

   curr_part_index = -1;     /* Reset current part index */
   tn_stat.set_flag = 0;
   tn_stat.q_set = 0;
   tn_stat.q_value = 0.0;
   (void)strncpy(tn_stat.zaid, "", sizeof(tn_stat.zaid) );
   (void)strncpy(tn_stat.info, "", sizeof(tn_stat.info) );
   (void)strncpy(tn_stat.date_processed, "", sizeof(tn_stat.date_processed) );
   (void)strncpy(tn_stat.date_source, "", sizeof(tn_stat.date_source) );
   (void)strncpy(tn_stat.library_name, "", sizeof(tn_stat.library_name) );
   tn_stat.num_temps = -1;
   tn_stat.int_sigvbars.length = 0;
   tn_stat.int_sigvbars.file_ptr = 0;
   tn_stat.int_einbars.length = 0;
   tn_stat.int_einbars.file_ptr = 0;
   tn_stat.temps.length = 0;
   tn_stat.temps.file_ptr = 0;
   tn_stat.sigvbars.length = 0;
   tn_stat.sigvbars.file_ptr = 0;
   tn_stat.einbars.length = 0;
   tn_stat.einbars.file_ptr = 0;
   tn_stat.rprods.length = 0;
   tn_stat.rprods.file_ptr = 0;
   tn_stat.reactants.length = 0;
   tn_stat.reactants.file_ptr = 0;

   /* Initialize secondary particle data */
   if (tn_stat.num_sec_parts > 0) {
     for (i = 0; i < tn_stat.num_sec_parts; i++ ) {

       if ( ( i < sec_part_filled ) && ( sec_part_filled > 0 ) ) {
 	 ndi_set0_tn_sec_part( &(tn_stat.sec_part[i]), 1 ); 
       }

     }
     if (tn_stat.sec_part) ndi_free( (void**)&(tn_stat.sec_part)); 
     if (tn_stat.sec_part_types) ndi_free( (void**)&tn_stat.sec_part_types);
   }

   tn_stat.sec_part = NULL;
   tn_stat.sec_part_types = NULL;
   tn_stat.num_sec_parts = -1;
   sec_part_filled = 0;


   return(ierr);
  
}

/**********************************************************************/
/**********************************************************************/

static int set0_tnreac(void) {
   
   int ierr = 0;         /* Error flag: 0=no error,pos=warning, neg=error */
   int i = 0;

   curr_part_index = -1;     /* Reset current part index */
   tnreac_stat.set_flag = 0;
   (void)strncpy(tnreac_stat.dataset, "", sizeof(tnreac_stat.dataset) );
   (void)strncpy(tnreac_stat.info, "", sizeof(tnreac_stat.info) );
   (void)strncpy(tnreac_stat.date_processed, "", sizeof(tnreac_stat.date_processed) );
   (void)strncpy(tnreac_stat.date_source, "", sizeof(tnreac_stat.date_source) );
   (void)strncpy(tnreac_stat.library_name, "", sizeof(tnreac_stat.library_name) );

   for (i = 0; i < tnreac_stat.reac_num; i++ ) {
      tnreac_stat.reactants[i].length = 0;
      tnreac_stat.rprods[i].length = 0;
      tnreac_stat.reactants[i].file_ptr = 0;
      tnreac_stat.rprods[i].file_ptr = 0;
   }
   if (tnreac_stat.reactants) ndi_free( (void **)&tnreac_stat.reactants );
   if (tnreac_stat.rprods) ndi_free( (void **)&tnreac_stat.rprods );
   tnreac_stat.reac_num = -1;

   for (i = 0; i < MAX_TN_REAC; i++ ) {
      if ( tnreac_stat.list_reac[i] ) {
	 ndi_free((void**)&tnreac_stat.list_reac[i]);
	 tnreac_stat.list_reac[i] = NULL;
      }
   }
   
   return(ierr);
  
}

/**********************************************************************/
/**********************************************************************/

static int set0_depl(void) {
   
   int ierr = 0;         /* Error flag: 0=no error,pos=warning, neg=error */
   int i = 0;
   int j = 0;


   /* Free target data */
   if ( ndi_depl_set ) {

     /* Free reaction product data */
     for (i = 0; i < chain_filled; i++ ) {
       
       /* Free reaction data */
       for ( j = 0; j < depl_target[i].num_rprods; j++ ) {
	 if ( depl_target[i].rprod[j].mt ) {
	   ndi_free( (void **)&depl_target[i].rprod[j].mt ); }
	 if ( depl_target[i].rprod[j].mult ) {
	   ndi_free( (void **)&depl_target[i].rprod[j].mult ); }
       }
       
       ndi_free( (void **)&depl_target[i].rprod ); 
     }  /* for i */   
     
     if ( depl_target ) ndi_free( (void **)&depl_target );
   }

   curr_part_index = -1;     /* Reset current part index */
   (void)strncpy(depl_dataset, "", sizeof(depl_dataset) );
   (void)strncpy(depl_info, "", sizeof(depl_info) );

   ndi_depl_set = 0;
   num_target = 0;
   chain_filled = 0;

   depl_target = NULL;

   chain_stat.num_target = -1;
   if ( chain_stat.inc_part_types ) {
     ndi_free( (void **)&chain_stat.inc_part_types);
     chain_stat.inc_part_types = NULL;
   }
     
   return(ierr);
  
}

/**********************************************************************/
/**********************************************************************/

/* int ndi_set0_tn_sec_part ( sec_part_distr *part ) */
int ndi_set0_tn_sec_part ( sec_part_distr *part, int reset_flag ) 
{

  /* reset_flag indicates whether this particle has just been
     created and therefore has no array allocated, or whether it
     has been filled already needs to have arrays freed.
     reset_flag:  0=initial, 1=reset  (Yuck!)
     */

  int j;
  int ierr = 0;
  int num_temps = 0;
  edist_data *edist_ptr = part->edist;

  if ( reset_flag ) { 
    num_temps = part->temp_interp.npts;
    for ( j = 0; j < num_temps; j++ ) {
      /* 	  ierr = set0_edist_data( &(edist_ptr[j]) ); */
      ierr = set0_edist_data( &(edist_ptr[j]), reset_flag ); 
    }
    if (edist_ptr) ndi_free( (void**)&edist_ptr );
  }
  part->edist = NULL;
  ierr = set0_interp_param( &(part->temp_interp), reset_flag );
  part->part_type = -1;

  return(ierr);
}

/* static int set0_interp_param ( interp_params *param ) */
static int set0_interp_param ( interp_params *param, int reset_flag ) 
{
  /* reset_flag indicates whether this particle has just been
     created and therefore has no array allocated, or whether it
     has been filled already needs to have arrays freed.
     reset_flag:  0=initial, 1=reset
     */

  int ierr = 0;

  if ( reset_flag ) {
    if ( param->nbt ) {
      ndi_free( (void **) &(param->nbt) );
    }
    if ( param->int_type ) {
      ndi_free( (void **) &(param->int_type) );
    }
  }
  param->npts = 0;
  param->nint = 0;
  param->nbt = NULL;
  param->int_type = NULL;

  return(ierr);
}

/* static int set0_edist_data ( edist_data *distr ) */
static int set0_edist_data ( edist_data *distr, int reset_flag ) 
{
  /* reset_flag indicates whether this particle has just been
     created and therefore has no array allocated, or whether it
     has been filled already needs to have arrays freed.
     reset_flag:  0=initial, 1=reset
     */

  int ierr = 0;

    distr->temp = 0.0;
    ierr = set0_interp_param( &(distr->interp), reset_flag );
    distr->file_ptr = 0;

  return(ierr);
}


/**********************************************************************/
/**********************************************************************/
