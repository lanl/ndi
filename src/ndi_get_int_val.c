
/*
 * File:	ndi_get_int_val.c
 * Description:	User function--return nuclear data (integer).
 * Time-stamp:	<1999-11-19 17:53:33 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

static int get_int_val_mg( state_struct *state, int in_key, int *ret_val );
static int get_int_val_tn( state_struct *state, int in_key, int *ret_val );
static int get_int_val_tnreac( state_struct *state, int in_key, int *ret_val );
static int ndi_get_num_rprods ( state_struct *state, int *num_rprods );

/**********************************************************************/
int ndi_c_get_int_val_h(state_struct *state, int in_key, int *ret_val) 


/*  Purpose:  Return a single integer value specified by 'in_key'.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 
    Ret_val contains the retrieved integer if no error has occurred.
    Ret_val will be zero if no data exists library.

***********************************************************************/

{

   int ierr = 0;

   /*  Default return value is 0 */
   *ret_val = 0;

   if ( (!state) || (!state->ndi_initialized)) {
     return(-2); }

   if ( ( state->library_type == 'm' ) ||
	( state->library_type == 'd' ) ) {
      ierr = get_int_val_mg( state, in_key, ret_val );
   } else if ( state->library_type == 's' ) {
      ierr = get_int_val_tn( state, in_key, ret_val );
   } else if ( state->library_type == 'r' ) {
      ierr = get_int_val_tnreac( state, in_key, ret_val );

      /*
	} else if ( state->library_type == '0' ) {
	ierr = get_int_val_chain( in_key, ret_val );
	*/

   } else if ( state->library_type == ' ' ) {
     /* Library type was not set--defaulting to multigroup n */
     state->library_type = 'm';
     if ( state->particle_type == ' ' ) {
       state->particle_type = 'n';
     }
     ierr = get_int_val_mg( state, in_key, ret_val );
     if ( ierr >= 0 ) {
       ierr = 8;
     }
   } else {
     /* Incompatible library type */
     ierr = -16;
   }

   return(ierr);
}


/**********************************************************************/
/**********************************************************************/
static int get_int_val_mg( state_struct *state, int in_key, int *ret_val ) {

   int ierr = 0;    /* Error flag: 0=no error,pos=warning, neg=error */
   int z, a;

        if (! state) {
            return(-2);
        }


   /*  Default return value is 0 */
   *ret_val = 0;

   /* Check that ndi_init has been called. */
   if (!state->ndi_initialized) {
      return(-2); }

  /* Check that isotope is set */
  if ( in_key != NDI_NUM_TARGET ) {
    /* Special case for no. of chain targets -- isotope need not be set */
    if (! state->iso_stat.set_flag) {
      return(-7);
    }
  }


   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_NUM_GRPS:                    /*  Number of groups  */
      if ( state->collapse_flag != 0 ) {
	 *ret_val =  state->grp_num_new;
      } else {
	    *ret_val = state->iso_stat.num_grps;
      }
      if ( *ret_val < 0 ) {
	*ret_val = 0;
	ierr = -9;
      }
      break;

   case NDI_FISSIONABLE:             /* Does this isotope have fission data?  */
      *ret_val = state->iso_stat.fissionable;
      break;

   case NDI_UP_GRPS:                 /* Number of upscatter groups  */
      if ( state->iso_stat.up_grps >= 0 ) {
	 *ret_val = state->iso_stat.up_grps;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_DOWN_GRPS :               /* Number of downscatter groups  */
      if ( state->iso_stat.down_grps >= 0 ) {
	 *ret_val = state->iso_stat.down_grps;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_LAST_GRP :               /* KLUDGE: Last neutron group  */
      if ( state->iso_stat.last_grp >= 0 ) {
	 *ret_val = state->iso_stat.last_grp;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_NUM_REAC:                     /* Number of edits  */
      if ( state->iso_stat.num_reac >= 0 ) {
	 *ret_val = state->iso_stat.num_reac;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_NUM_PNU_PR:
       *ret_val = 10;	/* number of fission neutron probabilities */
       break;

  case NDI_NUM_GRPS_CURR_PART:             /* Number of groups -- current particle */
    if ( state->iso_stat.sec_part &&
         /* && ( state->curr_part_index < state->sec_part_filled)) */
	 ( state->curr_part_index >= 0 ) ){ 
      if ( state->iso_stat.sec_part[state->curr_part_index].num_grps >= 0 ) {
	*ret_val = state->iso_stat.sec_part[state->curr_part_index].num_grps;
      } else {
	ierr = -9;    
      } 
    } else {
      /* Error -- bad particle? */
      ierr = -999;
    }

    break;

   case NDI_NUM_TARGET:             /* Number of targets on this depletion set */
     if ( ! state->ndi_depl_set ) {
       ierr = ndi_set_default_depl(state);
     }
     if ( state->num_target >= 0 ) {
       *ret_val = state->num_target;
     } else {
       *ret_val = 0;
       ierr = -9;
     }
     break;
      
   case NDI_NUM_RPRODS:         /* Number of products given for target given by current zaid */
     if ( ! state->ndi_depl_set ) {
       ierr = ndi_set_default_depl(state);
     }
     if ( state->num_target <= 0 ) {
       return(-9);
     }
     ierr =  ndi_get_num_rprods( state, ret_val );
     break;
      
   case NDI_ATOMIC_NUM:         /* Atomic number of current isotope -- Z */
     ierr = ndi_parse_mg_zaid( state->iso_stat.zaid, &z, &a );
     *ret_val = z;
     break;
      
   case NDI_MASS_NUM:           /* Mass number of current isotope -- A */
     ierr = ndi_parse_mg_zaid( state->iso_stat.zaid, &z, &a );
     *ret_val = a;

     break;


      /***********************************************************************/
      /* This section looks at non-standard isotope data (length & num_objs) */
      /***********************************************************************/

   case NDI_PN_ORDER:             
      /* Number of scattering matrices  */
      if ( state->iso_stat.pn_order >= 0 ) {
	 *ret_val = state->iso_stat.pn_order;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_NUM_SEC_PARTS:             
      /* Number of secondary particles  */
      if ( state->iso_stat.num_sec_parts > 0 ) {
	 *ret_val = state->iso_stat.num_sec_parts;
      } else {
	 ierr = -9;
      }
      break;


  case NDI_PN_ORDER_CURR_PART:      
    /* Number of production matrices--for current particle */
    if ( state->curr_part_index >= 0 ) {
      if ( state->iso_stat.sec_part[state->curr_part_index].pn_prod_full.num_objs > 0 ) {
	*ret_val = state->iso_stat.sec_part[state->curr_part_index].pn_prod_full.num_objs;
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
  
   return(ierr);
}

/**********************************************************************/
/**********************************************************************/
static int get_int_val_tn( state_struct *state, int in_key, int *ret_val ) {

   int ierr = 0;    /* Error flag: 0=no error,pos=warning, neg=error */


   /*  Default return value is 0 */
   *ret_val = 0;

   if (! state) {
       return(-2);
   }

   /* Check that isotope is set */
   if (! state->tn_stat.set_flag) {
      return(-7);
   }
   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_NUM_TEMPS:                    /*  Number of temperatures  */
      if ( state->tn_stat.num_temps >= 0 ) {
	 *ret_val = state->tn_stat.num_temps;
      } else {
	 /* Error: data not available in library for this reaction */
	 ierr = -9;
      }
      break;

   case NDI_NUM_SEC_PARTS:                    /*  Number of sec particles w/energy info  */
      if ( state->tn_stat.num_sec_parts >= 0 ) {
	 *ret_val = state->tn_stat.num_sec_parts;
      } else {
	 /* Error: data not available in library for this reaction */
	 ierr = -9;
      }
      break;
      
   case NDI_NUM_SIGVBAR_INTERP:       /* Number of sigvbar interpolation regions  */
      if ( state->tn_stat.int_sigvbars.length >= 0 ) {
	 *ret_val =  state->tn_stat.int_sigvbars.length;
      } else {
	 ierr = -9;
      }
      break;
      
   case NDI_NUM_EINBAR_INTERP:        /* Number of einbar interpolation regions  */
      if ( state->tn_stat.int_einbars.length >= 0 ) {
	 *ret_val =  state->tn_stat.int_einbars.length;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_NUM_REAC:                     /* Number of reactions  */
      /* TN data only describes one reaction for each zaid */
      if ( state->tn_stat.rprods.length >= 0 ) {
	 *ret_val = 1;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_NUM_RPRODS:             /* Number of reaction products*/
      if ( state->tn_stat.rprods.length >= 0 ) {
	 *ret_val =  state->tn_stat.rprods.length;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_NUM_REACTANTS:             /* Number of reactants*/
      if ( state->tn_stat.reactants.length >= 0 ) {
	 *ret_val =  state->tn_stat.reactants.length;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_NUM_GRPS:    /* Number of energy grps used in edist of sec. parts */
     *ret_val = state->tn_num_grps;
      if ( *ret_val < 0 ) {
	*ret_val = 0;
	ierr = -9;
      }
     break;

      /**************  UNKNOWN KEYWORD  *****************/

   default:
      ierr = -4;
      break;
   }
  
   return(ierr);
}
/**********************************************************************/
static int get_int_val_tnreac( state_struct *state, int in_key, int *ret_val ) {

   int ierr = 0;    /* Error flag: 0=no error,pos=warning, neg=error */

   /*  Default return value is 0 */
   *ret_val = 0;

   if (! state) {
       return(-2);
   }

   /* Check that isotope is set */
   if ( state->tnreac_stat.reac_num <= 0 ) {
      return(-7);
   }

   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_NUM_REAC:             /* Number of reactions on this TN dataset */

      if ( state->tnreac_stat.reac_num >= 0 ) {
	 *ret_val = state->tnreac_stat.reac_num;
      } else {
	 *ret_val = 0;
	 ierr = -9;
      }
      break;

      /**************  UNKNOWN KEYWORD  *****************/

   default:
      ierr = -4;
      break;
   }
  
   return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_get_num_rprods ( state_struct *state, int *num_rprods ) {
  
  int ierr = 0;
  int curr_index;
  depl_target_struct *curr_targ;

  
  if (state) {
      curr_targ = state->depl_target;
  } else {
       return(-2);
  }

  ierr = ndi_set_curr_target( state, &curr_targ, &curr_index );
  if ( ( ierr >= 0 ) && ( curr_targ != NULL ) ) {
    *num_rprods = curr_targ->num_rprods;
  } else {
    *num_rprods = 0;
  }
  
  return( ierr );
}

/**********************************************************************/
