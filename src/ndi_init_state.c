
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
#include "ndi_state.h"

/**********************************************************************/

int ndi_init_state_common( void ) {

  int ierr = 0;


   curr_part_index = -1;
   strcpy( n_group, "default" );
   grp_num_new = 0;

  return( ierr );
}

/**********************************************************************/

int ndi_init_state_iso_state( void ) {

  int ierr = 0;

   ierr = ndi_set0('m');
   if (new_ebounds) ndi_free((void**)&new_ebounds);
   new_ebounds = NULL;
   collapse_flag = 0;   /* No collapse */
   if (lib_weight) ndi_free((void**)&lib_weight);
   lib_weight = NULL;
   if (lib_ebounds) ndi_free((void**)&lib_ebounds);
   lib_ebounds = NULL;
   trans_corr = ' ';    /* No transport correction */
   trans_corr_order = -1; /* No transport correction */
   no_norm = 0;         /* Normalize chi and edist */
   /*  REPLACE USE OF SIZEOF */
   strncpy( def_fiss_prod_id, DEF_FISSION_PROD, sizeof(def_fiss_prod_id) );

  return( ierr) ;
}


/**********************************************************************/

int ndi_init_state_tn( void ) {

  int ierr = 0;
  
  ierr = ndi_set0('s');
  tn_num_grps = 0;
  sec_part_filled = 0;
  if ( tn_ebounds ) ndi_free((void**)&tn_ebounds);
  tn_ebounds = NULL;
  default_temp = 0.0;

  return( ierr) ;
}

/**********************************************************************/

int ndi_init_state_tnreac( void ) {

  int ierr = 0;

  ierr = ndi_set0('r');

  return( ierr) ;
}

/**********************************************************************/

int ndi_init_state_chain( void ) {

  int ierr = 0;

  ierr = ndi_set0('0');
   
  return( ierr) ;
}
/**********************************************************************/
