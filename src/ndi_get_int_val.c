
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
#include "ndi_state.h"

static int get_int_val_mg( int in_key, int *ret_val );
static int get_int_val_tn( int in_key, int *ret_val );
static int get_int_val_tnreac( int in_key, int *ret_val );
static int ndi_get_num_rprods ( int *num_rprods );

/**********************************************************************/

int ndi_c_get_int_val(int in_key, int *ret_val) 

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

   if (!ndi_initialized) {
     return(-2); }

   if ( ( library_type == 'm' ) ||
	( library_type == 'd' ) ) {
      ierr = get_int_val_mg( in_key, ret_val );
   } else if ( library_type == 's' ) {
      ierr = get_int_val_tn( in_key, ret_val );
   } else if ( library_type == 'r' ) {
      ierr = get_int_val_tnreac( in_key, ret_val );

      /*
	} else if ( library_type == '0' ) {
	ierr = get_int_val_chain( in_key, ret_val );
	*/

   } else if ( library_type == ' ' ) {
     /* Library type was not set--defaulting to multigroup n */
     library_type = 'm';
     if ( particle_type == ' ' ) {
       particle_type = 'n';
     }
     ierr = get_int_val_mg( in_key, ret_val );
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
static int get_int_val_mg( int in_key, int *ret_val ) {

   int ierr = 0;    /* Error flag: 0=no error,pos=warning, neg=error */
   int z, a;


   /*  Default return value is 0 */
   *ret_val = 0;

   /* Check that ndi_init has been called. */
   if (!ndi_initialized) {
      return(-2); }

  /* Check that isotope is set */
  if ( in_key != NDI_NUM_TARGET ) {
    /* Special case for no. of chain targets -- isotope need not be set */
    if (! iso_stat.set_flag) {
      return(-7);
    }
  }


   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_NUM_GRPS:                    /*  Number of groups  */
      if ( collapse_flag != 0 ) {
	 *ret_val =  grp_num_new;
      } else {
	    *ret_val = iso_stat.num_grps;
      }
      if ( *ret_val < 0 ) {
	*ret_val = 0;
	ierr = -9;
      }
      break;

   case NDI_UP_GRPS:                 /* Number of upscatter groups  */
      if ( iso_stat.up_grps >= 0 ) {
	 *ret_val = iso_stat.up_grps;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_DOWN_GRPS :               /* Number of downscatter groups  */
      if ( iso_stat.down_grps >= 0 ) {
	 *ret_val = iso_stat.down_grps;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_LAST_GRP :               /* KLUDGE: Last neutron group  */
      if ( iso_stat.last_grp >= 0 ) {
	 *ret_val = iso_stat.last_grp;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_NUM_REAC:                     /* Number of edits  */
      if ( iso_stat.num_reac >= 0 ) {
	 *ret_val = iso_stat.num_reac;
      } else {
	 ierr = -9;
      }
      break;


  case NDI_NUM_GRPS_CURR_PART:             /* Number of groups -- current particle */
    if ( iso_stat.sec_part &&
	 ( curr_part_index >= 0 ) &&
	 ( curr_part_index < sec_part_filled)) {
      if ( iso_stat.sec_part[curr_part_index].num_grps >= 0 ) {
	*ret_val = iso_stat.sec_part[curr_part_index].num_grps;
      } else {
	ierr = -9;    
      } 
    } else {
      /* Error -- bad particle? */
      ierr = -999;
    }

    break;

   case NDI_NUM_TARGET:             /* Number of targets on this depletion set */
     if ( ! ndi_depl_set ) {
       ierr = ndi_set_default_depl();
     }
     if ( num_target >= 0 ) {
       *ret_val = num_target;
     } else {
       *ret_val = 0;
       ierr = -9;
     }
     break;
      
   case NDI_NUM_RPRODS:         /* Number of products given for target given by current zaid */
     if ( ! ndi_depl_set ) {
       ierr = ndi_set_default_depl();
     }
     if ( num_target <= 0 ) {
       return(-9);
     }
     ierr =  ndi_get_num_rprods( ret_val );
     break;
      
   case NDI_ATOMIC_NUM:         /* Atomic number of current isotope -- Z */
     ierr = ndi_parse_mg_zaid( iso_stat.zaid, &z, &a );
     *ret_val = z;
     break;
      
   case NDI_MASS_NUM:           /* Mass number of current isotope -- A */
     ierr = ndi_parse_mg_zaid( iso_stat.zaid, &z, &a );
     *ret_val = a;

     break;


      /***********************************************************************/
      /* This section looks at non-standard isotope data (length & num_objs) */
      /***********************************************************************/

   case NDI_PN_ORDER:             
      /* Number of scattering matrices  */
      if ( iso_stat.pn_order >= 0 ) {
	 *ret_val = iso_stat.pn_order;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_NUM_SEC_PARTS:             
      /* Number of secondary particles  */
      if ( iso_stat.num_sec_parts > 0 ) {
	 *ret_val = iso_stat.num_sec_parts;
      } else {
	 ierr = -9;
      }
      break;


  case NDI_PN_ORDER_CURR_PART:      
    /* Number of production matrices--for current particle */
    if ( curr_part_index >= 0 ) {
      if ( iso_stat.sec_part[curr_part_index].pn_prod_full.num_objs > 0 ) {
	*ret_val = iso_stat.sec_part[curr_part_index].pn_prod_full.num_objs;
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
static int get_int_val_tn( int in_key, int *ret_val ) {

   int ierr = 0;    /* Error flag: 0=no error,pos=warning, neg=error */


   /*  Default return value is 0 */
   *ret_val = 0;

   /* Check that isotope is set */
   if (! tn_stat.set_flag) {
      return(-7);
   }
   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_NUM_TEMPS:                    /*  Number of temperatures  */
      if ( tn_stat.num_temps >= 0 ) {
	 *ret_val = tn_stat.num_temps;
      } else {
	 /* Error: data not available in library for this reaction */
	 ierr = -9;
      }
      break;

   case NDI_NUM_SEC_PARTS:                    /*  Number of sec particles w/energy info  */
      if ( tn_stat.num_sec_parts >= 0 ) {
	 *ret_val = tn_stat.num_sec_parts;
      } else {
	 /* Error: data not available in library for this reaction */
	 ierr = -9;
      }
      break;

   case  NDI_NUM_INTERP:               /* Number of interpolation regions  */
      if ( tn_stat.int_sigvbars.length >= 0 ) {
	 *ret_val =  tn_stat.int_sigvbars.length;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_NUM_REAC:                     /* Number of reactions  */
      /* TN data only describes one reaction for each zaid */
      if ( tn_stat.rprods.length >= 0 ) {
	 *ret_val = 1;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_NUM_RPRODS:             /* Number of reaction products*/
      if ( tn_stat.rprods.length >= 0 ) {
	 *ret_val =  tn_stat.rprods.length;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_NUM_REACTANTS:             /* Number of reactants*/
      if ( tn_stat.reactants.length >= 0 ) {
	 *ret_val =  tn_stat.reactants.length;
      } else {
	 ierr = -9;
      }
      break;

   case NDI_NUM_GRPS:    /* Number of energy grps used in edist of sec. parts */
     *ret_val = tn_num_grps;
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
static int get_int_val_tnreac( int in_key, int *ret_val ) {

   int ierr = 0;    /* Error flag: 0=no error,pos=warning, neg=error */


   /*  Default return value is 0 */
   *ret_val = 0;

   /* Check that isotope is set */
   if ( tnreac_stat.reac_num <= 0 ) {
      return(-7);
   }

   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_NUM_REAC:             /* Number of reactions on this TN dataset */

      if ( tnreac_stat.reac_num >= 0 ) {
	 *ret_val = tnreac_stat.reac_num;
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

static int ndi_get_num_rprods ( int *num_rprods ) {
  
  int ierr = 0;
  int curr_index;
  depl_target_struct *curr_targ = depl_target;
  
  ierr = ndi_set_curr_target( &curr_targ, &curr_index );
  if ( ( ierr >= 0 ) && ( curr_targ != NULL ) ) {
    *num_rprods = curr_targ->num_rprods;
  } else {
    *num_rprods = 0;
  }
  
  return( ierr );
}

/**********************************************************************/
