
/*
 * File:	ndi_get_size_n.c
 * Description:	User function--returns size of an element of a multi-dim array.
 * Time-stamp:	<1999-11-18 13:09:22 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static int get_mg_size_n( int in_key, int vec_num, int *ierr );
static int get_tn_size( int in_key, int vec_num, int *ierr );
static int get_tnreac_size( int in_key, int vec_num, int *ierr );

/**********************************************************************/

int ndi_c_get_size_n(int in_key, int vec_num, int *ierr)

/* Purpose: Return the size of a "vector" in a multi-dimensional array.
   The vector is identified by vec_num (a zero-based index).

   Parameters:
   in_key -- in: key to specify type of data required
   ierr -- out: error flag, where 0=no error,pos=warning, neg=error
   vecnum -- in: indicates the index of the element (**zero-based**)

   Returns: Returns the integer indicating the number of elements
   in the array.  For single-valued data (temperature, sigma0, etc.)
   "1" is returned.  For string data the size of the string (number of
   characters plus the end of string character) is returned.  Returns 
   zero or negative as error.

**********************************************************************/

{
   int size = 0;              /* Default return value is '0' */
   *ierr = 0;                         /* Default error value */


   /* Check that ndi_init has been called. */
   if (!ndi_initialized) {
      *ierr = -2;
      return(size); }
   
   if ( ( library_type == 'm' ) ||
	( library_type == 'd' )) {
      size = get_mg_size_n( in_key, vec_num, ierr );
   } else if (library_type == 's' ) {
      size = get_tn_size( in_key, vec_num, ierr );
   } else if (library_type == 'r' ) {
      size = get_tnreac_size( in_key, vec_num, ierr );
   } else if ( library_type == ' ' ) {
     /* Library type was not set--defaulting to multigroup n */
     library_type = 'm';
     if ( particle_type == ' ' ) {
       particle_type = 'n';
     }
     size = get_mg_size_n( in_key, vec_num, ierr );
     if ( *ierr >= 0 ) {
       *ierr = 8;
     }
   } else {
      *ierr = -16;
      size = 0;
   }
   
   if ( size < 0 ) {
     size = 0;
     if ( ierr == 0 ) {
       *ierr = 9;
     }
   }
   return(size);
}
      
/**********************************************************************/
/**********************************************************************/

static int get_mg_size_n( int in_key, int vec_num, int *ierr ) {

   int return_value = 0;              /* Default return value is '0' */
   int rprod_index;

   *ierr = 0;                          /* Default error value */

   /* Check that isotope is set */
   if ( in_key != NDI_TARGET ) {     
     /* Special case for targets -- isotope need not be set */
     if (! iso_stat.set_flag) {
       *ierr = -7;
       return(0);
     }
   }

   /*************************/
   /*   Process input key   */
   /*************************/       
   switch (in_key) {

     /****    Single-valued data -- return "1" ****/

   case NDI_RPROD_ALL:             /* sigma edit reaction products -- all */
   case NDI_RPROD_RMO:             /* sigma edit reaction products -- rmo */
   case NDI_RPROD_FEW:             /* sigma edit reaction products -- few */
   case NDI_REAC_MT:               /* Reaction mt for reaction indexed by vec_num */ 
   case NDI_REAC_Q:                /* Reaction Q value for reaction indexed by vec_num */ 
     return_value = 1;
     break;


     /****    String values ****/
   case NDI_SEC_PART_TYPES:                /* Secondary particle types  */
     if ( ( iso_stat.sec_part ) && ( vec_num < iso_stat.num_sec_parts ) ) {
       return_value = (int) strlen(iso_stat.sec_part[vec_num].part_type) + 1;
     } else {
       if ( iso_stat.num_sec_parts <= vec_num ) {
	 /* Invalid vec_num */
	 *ierr = -5;
       } else {
	 /* No secondary particle data for this particle */
	 *ierr = 9;
       }
     } 
     break;

   case NDI_SEC_PART_ZAIDS:                /* Secondary particle ZAIDs  */
     if ( ( iso_stat.sec_part ) && ( vec_num < iso_stat.num_sec_parts ) ) {
       return_value = (int) strlen(iso_stat.sec_part[vec_num].zaid) + 1;
     } else {
       if ( iso_stat.num_sec_parts <= vec_num ) {
	 /* Invalid vec_num */
	 *ierr = -5;
       } else {
	 /* No secondary particle data for this particle */
	 *ierr = 9;
       }
     } 
     break;

   case NDI_TARGET:
     if ( ! ndi_depl_set ) {
       *ierr = ndi_set_default_depl();
     }

     if ( ( depl_target) && ( num_target > vec_num ) ) {
       char temp_id[MAX_IN_STR_LEN];
       if ( depl_target[vec_num].izaid != 99999 ) {
	 sprintf( temp_id, "%d", depl_target[vec_num].izaid ); 
       } else {
	 /* Translate fission product identifier 99999 to default f.p. ZAID */
	 strcpy( temp_id, def_fiss_prod_id );
       }

       return_value = (int) strlen( temp_id ) + 1;
     } else {
       if ( ( num_target <= 0 ) || !(depl_target) ) {
	 *ierr = 9;
       } else {
	 *ierr = -5;
       }
     } 
     break;

   case NDI_RPRODS_ID:
     if ( ! ndi_depl_set ) {
       *ierr = ndi_set_default_depl();
     }
     if ( num_target > vec_num ) {
       char temp_id[MAX_IN_STR_LEN];
       *ierr = ndi_get_rprod_id_n( vec_num, temp_id, MAX_IN_STR_LEN );
       if ( strcmp(temp_id, "99999" ) == 0 ) {
	 /* Translate fission product identifier 99999 to default f.p. ZAID */
	 strcpy( temp_id, def_fiss_prod_id );
       }
       return_value = (int) strlen(temp_id) + 1;
     } else {
       if ( ( num_target <= 0 ) ) {
	 *ierr = 9;
       } else {
	 *ierr = -5;
       }
     } 
     break;


     /**** Simple array values ****/


     /* Size of the vector is the number of groups */
   case NDI_TOT_PROD_XS:     /* Total production cross section */
   case NDI_SIG_REAC:        /* reaction cross section */      
   case NDI_CORR_DIAG:       /* Diagonal transport correction  */
   case NDI_CORR_BHS:        /* Bell-Hansen-Sandmeier transport correction */
     if ( collapse_flag != 0 ) {
       return_value =  grp_num_new;              
     } else {
       return_value = iso_stat.num_grps;
     }
     break;

     /* Others */
   case NDI_RPROD_ID_ALL:             /* reaction product ids -- all */             
   case NDI_RPROD_MLT_ALL:            /* reaction product multiplicities--all */            
     *ierr = ndi_find_rprod_index(lib_stream, &iso_stat.rprod_all, 
				     &rprod_index, vec_num);
     if ( *ierr >= 0 ) {
       return_value =  iso_stat.rprod_all.stat[rprod_index].length;
     }
     break;
   
   case NDI_RPROD_ID_RMO:              /* reaction product ids -- rmo */     
   case NDI_RPROD_MLT_RMO:             /* reaction product multiplicities--rmo */
     *ierr = ndi_find_rprod_index(lib_stream, &iso_stat.rprod_rmo, 
				     &rprod_index, vec_num);
     if ( *ierr >= 0 ) {
       return_value =  iso_stat.rprod_rmo.stat[rprod_index].length;
     }
     break;


   case NDI_RPROD_ID_FEW:            /* reaction product ids -- few */  
   case NDI_RPROD_MLT_FEW:            /* reaction product ids -- few */
     *ierr = ndi_find_rprod_index(lib_stream, &iso_stat.rprod_few, 
				     &rprod_index, vec_num);
     if ( *ierr >= 0 ) {
       return_value =  iso_stat.rprod_few.stat[rprod_index].length;
     }
     break;
      


     /**** Multi-dimensional arrays ****/

   case NDI_PN_DOWN_FULL:           
      /* pn scattering -- full lower diagonal */
      /***Assumption for pn_down_full: down_grps == number of groups - 1 ***/
     if ( collapse_flag != 0 ) {
       return_value = ( grp_num_new * (grp_num_new + 1) ) / 2;
     } else {
       return_value =  ( ( iso_stat.num_grps ) *
			 ( iso_stat.num_grps + 1 ) ) / 2;
     } 
     break;
      
   case NDI_PN_FULL:                      /* pn scattering -- full matrix */                    
   case NDI_TRANSFER_MAT_TOT:             /* Transfer matrix -- total */
   case NDI_TRANSFER_MAT_PR:              /* Transfer matrix -- prompt */
      if ( collapse_flag != 0 ) {
	    return_value =  grp_num_new * grp_num_new;
     } else {
       return_value =  iso_stat.num_grps * iso_stat.num_grps;
     }
     break;

   case NDI_PN_PROD_FULL_CURR_PART:         
      /* pn scattering photon -- full */
      /* Assumption: photon groups are not collapsed */
      /*** Are photon production matrices collapsed????? ****/
   
     if ( ( iso_stat.sec_part ) && 
	  ( curr_part_index < iso_stat.num_sec_parts ) &&
	  ( curr_part_index >= 0 )) {
       /* Current particle has secondary particle info */
       
       if ( collapse_flag != 0 ) {
	 return_value = iso_stat.sec_part[curr_part_index].num_grps * grp_num_new;
       } else {
	 return_value = iso_stat.sec_part[curr_part_index].num_grps * iso_stat.num_grps;
       }
     } else {
       if ( iso_stat.num_sec_parts >= vec_num ) {
	 /* Invalid vec_num */
	 *ierr = -5;
       } else {
	 /* No secondary particle data for this particle */
	 *ierr = 9;
       }
     }
     break;

      
      /***********  UNKNOWN KEYWORD  **************/
      
   default:
      *ierr = -4;
      break;
   }

   return(return_value);
}

/**********************************************************************/
/**********************************************************************/


static int get_tn_size( int in_key, int vec_num, int *ierr ) {



   int return_value = 0;              /* Default return value is '0' */
   *ierr = 0;                         /* Default error value */

   /* Check that isotope is set */
   if (! tn_stat.set_flag) {
     *ierr = -7;
      return(return_value);
   }

   switch (in_key) {

     /****  Single-valued data -- return "1" ****/
   case NDI_RPRODS_ID:  
   case NDI_RPRODS_MLT:  
   case NDI_REACTANTS_ID:  
   case NDI_REACTANTS_MLT:  
     return_value = 1;
     break;

     /**** String values ****/
   case NDI_SEC_PART_TYPES:                /* Secondary particle types  */
     /* This gives the size of the string returned if the nth part type is asked for */
       if ( ( tn_stat.num_sec_parts > vec_num ) && 
	    ( sec_part_filled > vec_num ) &&
	    ( tn_stat.sec_part ) ) {
	 if ( tn_stat.sec_part[vec_num].part_type >= 0 ) {
	   char junk[MAX_IN_STR_LEN];
	   return_value = sprintf( junk, "%-ld", tn_stat.sec_part[vec_num].part_type) + 1;
	 }
       } else {
	 if ( ( tn_stat.num_sec_parts <= 0 ) || !( tn_stat.sec_part ) ) {
	   *ierr = 9;
	 } else {
	   *ierr = -5;
	 }
       } 
 
      break;

      /***********  UNKNOWN KEYWORD  **************/

   default:
      *ierr = -4;
      break;
   }

   return(return_value);
}

/**********************************************************************/
/**********************************************************************/

static int get_tnreac_size( int in_key, int vec_num, int *ierr ) {


   int return_value = 0;              /* Default return value is '0' */
   *ierr = 0;                         /* Default error value */

   /* Check that isotope is set */
   if ( tnreac_stat.reac_num < 0 ) {
     *ierr = -7;
     return(return_value);
   }

   switch (in_key) {

     /**** Single-valued data -- return "1" ****/
   case NDI_NUM_RPRODS:             /* Number of reaction products*/
   case NDI_NUM_REACTANTS:             /* Number of reactants*/
     return_value = 1;
     break;

     /****  Simple array values ****/

   case NDI_RPRODS_ID:  
   case NDI_RPRODS_MLT:  
     if ( ( tnreac_stat.reac_num > vec_num) && (tnreac_stat.rprods) ){
       return_value =  tnreac_stat.rprods[vec_num].length;
     } else {
       if ( ( tnreac_stat.reac_num <= 0 ) || !( tnreac_stat.rprods ) ) {
	 /* No data */
	 *ierr = 9;
       } else {
	 /* Subscript out of bounds */
	 *ierr = -5;
       }
     }
     break;
    
   case NDI_REACTANTS_ID:  
   case NDI_REACTANTS_MLT:  
      if ( (tnreac_stat.reactants) && ( tnreac_stat.reac_num > vec_num ) ) {
	 return_value =  tnreac_stat.reactants[vec_num].length;
      } else {
	if ( ( tnreac_stat.reac_num <= 0 ) || !( tnreac_stat.reactants ) ) {
	  /* No data */
	  *ierr = 9;
	} else {
	  /* Subscript out of bounds */
	  *ierr = -5;
	}
      }
      break;
      
      /**** String values ****/
   case NDI_LIST_REAC:     /* List of reactions */
      if ( ( tnreac_stat.reac_num > vec_num ) && (tnreac_stat.list_reac) )  {
	 return_value = (int) strlen( tnreac_stat.list_reac[vec_num] ) + 1;
      } else {
	if ( ( tnreac_stat.reac_num <= 0 ) || !( tnreac_stat.list_reac ) ) {
	  /* No data */
	  *ierr = 9;
	} else {
	  /* Subscript out of bounds */
	  *ierr = -5;
	}
      } 
 
      break;

    /***********  UNKNOWN KEYWORD  **************/

   default:
      *ierr = -4;
      break;
   }

   return(return_value);
}

/**********************************************************************/
/**********************************************************************/
