
/*
 * File:	ndi_init_state.c
 * Description:	Internal function--used to initialize state variables
 * Time-stamp:	<01/10/11 19:42:34 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

/**********************************************************************/

int ndi_init_state_common( state_struct *state ) {

  int ierr = 0;

  if (!state) {
      return(-2);
  }

   state->curr_part_index = -1;
   strcpy( (state->n_group), "default" );
   state->grp_num_new = 0;

  return( ierr );
}

/**********************************************************************/

int ndi_init_state_iso_state( state_struct *state ) {

  int ierr = 0;

  if (!state) {
      return(-2);
  }

   ierr = ndi_set0(state, 'm');
   if (state->new_ebounds) ndi_free((void**)&(state->new_ebounds));
   state->new_ebounds = NULL;
   state->collapse_flag = 0;   /* No collapse */
   if (state->lib_weight) ndi_free((void**)&(state->lib_weight));
   state->lib_weight = NULL;
   if (state->lib_ebounds) ndi_free((void**)&(state->lib_ebounds));
   state->lib_ebounds = NULL;
   state->trans_corr = ' ';    /* No transport correction */
   state->trans_corr_order = -1; /* No transport correction */
   state->no_norm = 0;         /* Normalize chi and edist */
   /*  REPLACE USE OF SIZEOF */
   strncpy( state->def_fiss_prod_id, DEF_FISSION_PROD, sizeof(state->def_fiss_prod_id) );

  return( ierr) ;
}


/**********************************************************************/

int ndi_init_state_tn( state_struct *state ) {

  int ierr = 0;
  
  if (!state) {
      return(-2);
  }

  ierr = ndi_set0(state, 's');
  state->tn_num_grps = 0;
  state->sec_part_filled = 0;
  if ( state->tn_ebounds ) ndi_free((void**)&(state->tn_ebounds));
  state->tn_ebounds = NULL;
  state->default_temp = 0.0;

  return( ierr) ;
}

/**********************************************************************/

int ndi_init_state_tnreac( state_struct *state ) {

  int ierr = 0;

  if (!state) {
      return(-2);
  }

  ierr = ndi_set0(state, 'r');

  return( ierr) ;
}

/**********************************************************************/

int ndi_init_state_chain( state_struct *state ) {

  int ierr = 0;

  if (!state) {
      return(-2);
  }

  ierr = ndi_set0(state, '0');
   
  return( ierr) ;
}
/**********************************************************************/
