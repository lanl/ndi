
/*
 * File:	ndi_get_size.c
 * Description:	User function--return the "size" of a library value.
 * Time-stamp:	<1999-11-15 11:28:34 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static int get_mg_size( int in_key, int *ierr );
static int get_tn_size( int in_key, int *ierr );
static int get_tnreac_size( int in_key, int *ierr );

/**********************************************************************/

int ndi_c_get_size(int in_key, int *ierr) 

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
   if (!ndi_initialized) {
      size = 0;
      *ierr = -2;
      return(size); }

   /* Special case--library type need not be set if asking for version. */
   if ( in_key == NDI_VERSION ) {
      size = (int) strlen(NDI_VERSION_DATA) + 1;
   } else {
      if ( ( library_type == 'm' ) ||
	   ( library_type == 'd' )) {
	 size = get_mg_size( in_key, ierr );
      } else if (library_type == 's' ) {
	 size = get_tn_size( in_key, ierr );
      } else if (library_type == 'r' ) {
	 size = get_tnreac_size( in_key, ierr );
      } else if ( library_type == ' ' ) {
	/* Library type was not set--defaulting to multigroup n */
	library_type = 'm';
	if ( particle_type == ' ' ) {
	  particle_type = 'n';
	}
	size = get_mg_size( in_key, ierr );
	if ( *ierr >= 0 ) {
	  *ierr = 8;
	}
      } else {
	 *ierr = -16;
	 size = 0;
      }
   }

   
   if ( size < 0 ) {
     size = 0;
     if ( *ierr == 0 ) {
       *ierr = 9;
     }
   }
   return(size);
}


/**********************************************************************/
/**********************************************************************/

static int get_mg_size( int in_key, int *ierr ) {


   int return_value = 0;              /* Default return value is '0' */
   *ierr = 0;                          /* Default error value */

   /* Check that isotope is set */
   if ( ( in_key != NDI_DEPL_ZAID ) && ( in_key != NDI_DEPL_INFO ) 
	&& ( in_key != NDI_NUM_TARGET ) ){     
     /* Special case for depletion -- isotope need not be set */
     if (! iso_stat.set_flag) {
       *ierr = -7;
       return(0);
     }
   }

   switch (in_key) {

      /*************************/
      /*   Single-valued data  */
      /*************************/
 
      /* String values--return size of string */
   case NDI_ZAID:                               /*  ZAID  */
      return_value = (int) strlen(iso_stat.zaid) + 1;
      break;
   case NDI_INFO:                               /* Information */
      return_value = (int) strlen(iso_stat.info) + 1;
      break;
   case NDI_DATE_PROCESSED:                     /* Date library was processed */
      return_value = (int) strlen(iso_stat.date_processed) + 1;
      break;
   case NDI_DATE_SOURCE:                     /* Date of library source info */
      return_value = (int) strlen(iso_stat.date_source) + 1;
      break;
   case NDI_LIBRARY_NAME:                    /* Name of current library */
      return_value = (int) strlen(iso_stat.library_name) + 1;
      break;
   case NDI_DEPL_ZAID:                          /*  Depletion ZAID  */
     if ( ! ndi_depl_set ) {
       *ierr = ndi_set_default_depl();
     }
     return_value = (int) strlen(depl_dataset) + 1;
     break;
   case NDI_DEPL_INFO:                          /* Depletion info */
     if ( ! ndi_depl_set ) {
       *ierr = ndi_set_default_depl();
     }
     return_value = (int) strlen(depl_info) + 1;
     break;

   case NDI_SEC_PART_TYPES:                /* Secondary particle types  */
     if ( ( iso_stat.sec_part ) && 
	  ( curr_part_index < iso_stat.num_sec_parts ) &&
	  ( curr_part_index >= 0 ) ) {
       return_value = (int) strlen(iso_stat.sec_part[curr_part_index].part_type) + 1;
     } else {
       if ( iso_stat.num_sec_parts >= curr_part_index ) {
	 /* Invalid curr_part_index*/
	 *ierr = -5;
       } else {
	 /* No secondary particle data for this particle */
	 *ierr = 9;
       }
     } 
     break;
   case NDI_SEC_PART_ZAIDS:                /* Secondary particle ZAIDs  */
     if ( ( iso_stat.sec_part ) && 
	  ( curr_part_index < iso_stat.num_sec_parts ) &&
	  ( curr_part_index >= 0 ) ) {
       return_value = (int) strlen(iso_stat.sec_part[curr_part_index].zaid) + 1;
     } else {
       if ( iso_stat.num_sec_parts >= curr_part_index ) {
	 /* Invalid curr_part_index  */
	 *ierr = -5;
       } else {
	 /* No secondary particle data for this particle */
	 *ierr = 9;
       }
     } 
     break;

/*  Note:  version request handled specially above  */
/*   case NDI_VERSION:    */                        /* NDI VERSION */
/*      return_value = 30; */
/*       return_value = strlen(NDI_VERSION_DATA) + 1; */
/*      break;   */

      /* Single values--return "1" */
   case NDI_NUM_GRPS:         /* Number of groups */
   case NDI_TEMP:             /* Temperature */
   case NDI_SIG_0:            /* sig_0 */
   case NDI_UP_GRPS:          /* upscatter_groups */
   case NDI_DOWN_GRPS :       /* downscatter_groups */
   case NDI_AT_WGT:           /* atomic weight */
   case NDI_AWR:              /* atomic weight ratio */
   case NDI_NUM_REAC:         /* number of edits */
   case NDI_NUM_RPRODS:       /* number of reaction products */
   case NDI_PN_ORDER:         /* number of scattering matrices */
   case NDI_NUM_SEC_PARTS:    /* number of secondary particles */
   case NDI_NUM_GRPS_CURR_PART:       /* number of groups -- current particle */
   case NDI_PN_ORDER_CURR_PART:       /* number of production matrices--current particle */
   case NDI_NUM_TARGET:       /* number of chain targets */
      return_value = 1;
      break;

      /*************************/
      /*  Simple array values  */
      /*************************/
 

      /*  Arrays with num_grps number of elements */

   case NDI_E_AVG:                          /* Average group energies */
   case NDI_ENERGY_N:                       /* Energy given to neutrons */
   case NDI_ENERGY_AVAIL:                   /* Energy available */
   case NDI_WGTS:                           /* weights */
   case NDI_VEL:                            /* Velocities */
   case NDI_HEATING:                        /* Heating */
   case NDI_KERMA:                          /* kerma */
   case NDI_SIG_TOT:                        /* Sigma total */
   case NDI_NU_TOT:                         /* nubar total */
   case NDI_NU_PR:                          /* nubar prompt */
   case NDI_NU_DEL:                         /* nubar delayed */
   case NDI_NU_SIG_F_TOT:                   /* nubar total sigma fission */
   case NDI_NU_SIG_F_PR:                    /* nubar prompt sigma fission */
   case NDI_NU_SIG_F_DEL:                   /* nubar delayed sigma fission */
   case NDI_CHI_VEC_TOT:                    /* chi total vector */
   case NDI_CHI_VEC_PR:                     /* chi prompt vector */
   case NDI_CHI_VEC_DEL:                    /* chi delayed vector */
   case NDI_HEATING_CURR_PART:              /* Partial heating--current particle */
   case NDI_KERMA_CURR_PART:                /* Partial kerma--current particle */
   case NDI_TOT_PROD_XS:                    /* Total production cross section */
   case NDI_TOT_DEPL_XS:                    /* Total depletion cross section */

     if ( collapse_flag != 0 ) {
       return_value =  grp_num_new;
     } else {
/*     if(iso_stat.wgts.length)  */
       return_value = iso_stat.num_grps;
     }
     break;

     /* Arrays with element number not equal to number of groups */


   case NDI_E_BOUNDS:           
      /* energy boundaries */
      if ( collapse_flag != 0 ) {
	 return_value =  grp_num_new + 1;
      } else {
/* 	 if(iso_stat.e_bounds.length)  */
	    return_value = iso_stat.num_grps + 1;
      }
      break;

   case NDI_NORM:                  
      /* normalization factor */
/*       if(iso_stat.norm.length)  */
	 return_value =  NUM_NORM;
      break;

   case NDI_FISS_Q:                  
      /* fission q */
/*       if(iso_stat.fiss_q.length)  */
	 return_value =  NUM_FISSION_Q;
      break;

   case NDI_REAC_MT:    
     /* edit reaction mt */
/*    if (iso_stat.reac_mt)  */
     return_value = iso_stat.num_reac;
     if ( return_value < 0 ) { return_value = 0; }
     break;

   case NDI_REAC_Q:
      /*edit reaction q */
/*       if (iso_stat.reac_q)  */
     return_value = iso_stat.num_reac;
     if ( return_value < 0 ) { return_value = 0; }
     break;

   case NDI_E_BOUNDS_CURR_PART:   
     /* Energy boundaries for current particle */
/*   if(iso_stat.e_bounds_p.length)  */
     if ( ( iso_stat.sec_part ) && 
	  ( curr_part_index < iso_stat.num_sec_parts ) &&
	  ( curr_part_index >= 0 ) ) {
       return_value = ( (iso_stat.sec_part[curr_part_index].num_grps) + 1 ); 
     } else {
       *ierr = 9;
     }
     break;


      /****************************/
      /*  Multi-dim array values  */
      /****************************/
 
   case NDI_RPROD_ALL:          /* old reaction product format -- all */
      return_value = iso_stat.rprod_all.num_objs; 
      break; 

   case NDI_RPROD_RMO:          /* sigma edit reaction products -- rmo */
      return_value = iso_stat.rprod_rmo.num_objs; 
      break; 

   case NDI_RPROD_FEW:  
      /* sigma edit reaction products -- few */
      return_value = iso_stat.rprod_few.num_objs; 
      break; 

   case NDI_CHI_MAT_TOT:              /* chi total matrix */          
   case NDI_CHI_MAT_PR:               /* chi prompt matrix */     
   case NDI_CHI_MAT_DEL:              /* chi delayed matrix */          

/*       if((iso_stat.chi_mat_tot.length) || (iso_stat.chi_vec_tot.length)){ */
     if ( collapse_flag != 0 ) {
       return_value =  grp_num_new * grp_num_new;
     } else {
       return_value =  iso_stat.num_grps * iso_stat.num_grps;
     }
/*       } */
     break;
      

     /***********  UNKNOWN KEYWORD  **************/

   default:
     *ierr = -4;
      return_value = 0;
      break;
   }

   return(return_value);
}

/**********************************************************************/
/**********************************************************************/


static int get_tn_size( int in_key, int *ierr ) {


   int return_value = 0;              /* Default return value is '0' */
   *ierr = 0;                         /* Default error value */

   /* Check that isotope is set */
   if (! tn_stat.set_flag) {
     *ierr = -7;
      return(0);
   }

   switch (in_key) {

      /*************************/
      /*   Single-valued data  */
      /*************************/
 
      /* String values--return size of string */
   case NDI_ZAID:                               /*  ZAID  */
      return_value = (int) strlen(tn_stat.zaid) + 1;
      break;
   case NDI_INFO:                               /* Information */
      return_value = (int) strlen(tn_stat.info) + 1;
      break;
   case NDI_VERSION:                            /* NDI VERSION */
      return_value = (int) strlen(NDI_VERSION_DATA) + 1;
      break;
      
      /* Single values--return "1" */
   case NDI_NUM_TEMPS:          /* Number of sigma tempuratures */
   case NDI_NUM_INTERP:         /* Number of interpolation regions */ 
   case NDI_NUM_SEC_PARTS:      /* Number of sec particles w/energy info  */
   case NDI_NUM_REAC:           /* Number of reactions  */
   case NDI_NUM_RPRODS:         /* Number of reaction products*/
   case NDI_NUM_REACTANTS:      /* Number of reactants*/
   case NDI_REAC_Q:             /* Q-value for current reaction */ 
   case NDI_NUM_GRPS:           /* Number of energy grps used in edist of sec. parts */
   case NDI_EOUTBAR:            /* Average energy of outgoing secondary particle */
      return_value = 1;
      break;


      /*************************/
      /*  Simple array values  */
      /*************************/
      
   case NDI_TEMPS:                 
      /* temperatures */
     return_value = tn_stat.num_temps;
     break;

   case NDI_SIGVBARS:                           
      /* sigma vbars */
     return_value = tn_stat.num_temps;
     break;
      
   case NDI_RPRODS_ID:                    
      /* reaction product ids */
      if ( tn_stat.rprods.length ) 
	 return_value =  tn_stat.rprods.length;
      break;
   
   case NDI_RPRODS_MLT:                  
      /* reaction product multiplicities */
      if ( tn_stat.rprods.length ) 
	 return_value =  tn_stat.rprods.length;
      break;

   case NDI_REACTANTS_ID:   
     /* reactant ids */
     if ( tn_stat.reactants.length ) 
       return_value =  tn_stat.reactants.length;
     break;

   case NDI_REACTANTS_MLT:   
     /* reactant ids */
     if ( tn_stat.reactants.length ) 
       return_value =  tn_stat.reactants.length;
     break;

   case NDI_SEC_PART_TYPES:  
      /* Vector of secondary particle types */
     if ( tn_stat.num_sec_parts >= 0 ) {
       return_value = sec_part_filled;
     }

   case NDI_EDIST:   /* Curr. particle's energy distribution at current temp. */
      /* The number returned is the number of groups in the currently-set group stucture */
     return_value = tn_num_grps;
     break;

   case NDI_INTERP_REG:                       
      /* interpolation regions */
      if(tn_stat.int_sigvbars.length) {
	 return_value =  tn_stat.int_sigvbars.length;
      }
      break;

   case NDI_INTERP_TYPE:        
      /* interpolation types */
      if (tn_stat.int_sigvbars.length) {
	 return_value =  tn_stat.int_sigvbars.length;
      }
      break;

   case NDI_EINBAR:
      /* thermal-averaged total incoming energy for each temp. */
     return_value = tn_stat.num_temps;
     break;
   
   case NDI_E_BOUNDS:                      /*  Multigroup energy boundaries for sec. part distr. */
     return_value = tn_num_grps + 1;
     break;

   case NDI_E_AVG:              /* Mean of each multigroup bin used for sec. part edist */
     return_value = tn_num_grps;
     break;

      /***********  UNKNOWN KEYWORD  **************/

   default:
     *ierr = -4;
      return_value = 0;
      break;
   }

   if ( return_value < 0 ) { return_value = 0; }
   return(return_value);
}

/**********************************************************************/
/**********************************************************************/

static int get_tnreac_size( int in_key, int *ierr ) {


   int return_value = 0;              /* Default return value is '0' */
   *ierr = 0;                         /* Default error value */

   /* Check that isotope is set */
   if ( tnreac_stat.reac_num > 0 ) {
     *ierr = -7;
     return(0);
   }

   switch (in_key) {

      /*************************/
      /*   Single-valued data  */
      /*************************/
 
      /* String values--return size of string */
   case NDI_ZAID:                               /*  ZAID  */
      return_value = (int) strlen(tnreac_stat.dataset) + 1;
      break;
   case NDI_INFO:                               /* Information */
      return_value = (int) strlen(tnreac_stat.info) + 1;
      break;

      /*   case NDI_VERSION:           */                 /* NDI VERSION */
      /* Special case -- handled above 
     return_value = strlen(NDI_VERSION) + 1;
      break;
      */
      
      /* Single values--return "1" */
   case NDI_NUM_REAC:       /* Number of reactions */
      return_value = 1;
      break;

      /***********  UNKNOWN KEYWORD  **************/

   default:
     *ierr = -4;
      return_value = 0;
      break;
   }

   return(return_value);
}

/**********************************************************************/
/**********************************************************************/
