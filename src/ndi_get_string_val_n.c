
/*
 * File:	ndi_get_string_val_n.c
 * Description:	User function--return a string element from a list.
 * Time-stamp:	<2003-03-24 18:41:12 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static int get_string_n_mg( int in_key, char *ret_val, 
			    int vec_num, int str_size );
static int get_string_n_tn( int in_key, char *ret_val, 
			    int vec_num, int str_size );
static int get_string_n_tnreac( int in_key, char *ret_val, 
				int vec_num, int str_size );

/**********************************************************************/

int ndi_c_get_string_val_n(int in_key, int vec_num, char *ret_val, 
			   int str_size) 

/*  Purpose: Return a single string value.
    
    Parameters: 
    in_key -- in: key to specify type of data required
    ret_val -- out: string value returned
    str_size -- in:number of characters in string

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/

{
   int ierr = 0;      /* Error flag: 0=no error,pos=warning, neg=error */

   if ( ( ret_val ) && ( str_size > 0 ) ) {
      *ret_val = '\0';
   } else {
     ierr = 7;
   }

   /* Check that ndi_init has been called. */
   if (!ndi_initialized) {
      return(-2); }

   if ( in_key == NDI_ZAID ) {
       ierr = ndi_translate_sza_to_symbol( vec_num, ret_val, str_size );
   } else {
     if ( ( library_type == 'm' ) ||
	  ( library_type == 'd' ) ) {
	 ierr = get_string_n_mg( in_key, ret_val, vec_num, str_size );
     } else if ( library_type == 's' ) {
	 ierr = get_string_n_tn( in_key, ret_val, vec_num, str_size );
     } else if ( library_type == 'r' ) {
	 ierr = get_string_n_tnreac( in_key, ret_val, vec_num, str_size );
     } else if ( library_type == ' ' ) {
	 /* Library type was not set--defaulting to multigroup n */
	 library_type = 'm';
	 if ( particle_type == ' ' ) {
	     particle_type = 'n';
	 }
	 ierr = get_string_n_mg( in_key, ret_val, vec_num, str_size );
	 if ( ierr >= 0 ) {
	     ierr = 8;
	 }
     } else {
	 /* Incompatible library type */
	 ierr = -16;
     }   
   }

   return(ierr);

}

/**********************************************************************/
/**********************************************************************/
static int get_string_n_mg( int in_key, char *ret_val, 
			    int vec_num, int str_size ) {

   int ierr = 0;      /* Error flag: 0=no error,pos=warning, neg=error */

   /* Default return value is empty string */
   *ret_val = '\0';

   /* Check that isotope is set */
   if ( ( in_key != NDI_TARGET ) &&
	( in_key != NDI_ZAID ) )
   {     
     /* Special case for targets -- isotope need not be set */
     if (! iso_stat.set_flag) {
       return(-7);
     }
   }


   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_ZAID:                   /* Translate input SZA to string */
       ierr = ndi_translate_sza_to_symbol( vec_num, ret_val, str_size );
       break;

   case NDI_SEC_PART_TYPES:                /* Secondary particle types  */
      if ( ( iso_stat.num_sec_parts > vec_num ) && ( iso_stat.sec_part ) &&
	   ( strcmp(iso_stat.sec_part[vec_num].part_type, "") != 0 ) ) {
	 strncpy( ret_val, iso_stat.sec_part[vec_num].part_type, str_size );
      } else {
	if ( iso_stat.num_sec_parts > vec_num ) {
	  ierr = ndi_check_array_err(iso_stat.num_sec_parts);
	} else {
	  ierr = -9;
	}
      }
      break;

   case NDI_SEC_PART_ZAIDS:                /* Secondary particle ZAIDs  */
      if ( ( iso_stat.num_sec_parts > vec_num ) && ( iso_stat.sec_part ) &&
	   ( strcmp(iso_stat.sec_part[vec_num].zaid, "") != 0 ) ) {
	 strncpy( ret_val, iso_stat.sec_part[vec_num].zaid, str_size );
      } else {
	if ( iso_stat.num_sec_parts > vec_num ) {
	  ierr = ndi_check_array_err(iso_stat.num_sec_parts);
	} else {
	  ierr = 9;
	}
      }
      break;

   case NDI_TARGET:     /* Zaid of target specified by vec_num */
     
     /* check vec_num is less than num of targets */
     if ( ! ndi_depl_set ) {
       ierr = ndi_set_default_depl();
     }
     if ( num_target <= 0 ) {
       return(-7);
     }
     if ( num_target > vec_num ) {
       if ( sprintf( ret_val, "%d", depl_target[vec_num].izaid )  > str_size ) {
	 /* Error */
	 ierr = -5;
       }
     } else {
       ierr = ndi_check_array_err(chain_stat.num_target);
     } 
     break;

   case NDI_RPRODS_ID:
      /* Return the id of the reaction product whose index is vec_num 
         (for the target given by the global variable 'zaid' ) */
      
     if ( ! ndi_depl_set ) {
       ierr = ndi_set_default_depl();
     }
     if ( num_target <= 0 ) {
       return(-7);
     }
     ierr = ndi_get_rprod_id_n( vec_num, ret_val, str_size );
     break;


   case NDI_INC_PART_TYPES:          /* Incident particle types */
       ierr = ndi_get_nth_inc_part_type( vec_num, ret_val, str_size );
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
static int get_string_n_tn( int in_key, char *ret_val, 
			    int vec_num, int str_size ) {

  int ierr = 0;      /* Error flag: 0=no error,pos=warning, neg=error */
  int return_int;

  if ( str_size > 0 ) {
    *ret_val = '\0';
  } else {
    return(7);
  }

   /* Check that isotope is set */
   if (! tn_stat.set_flag) {
      return(-7);
   }

   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_SEC_PART_TYPES:                /* Secondary particle types  */
      /* Check vec_num is within bounds */
      if ( ( tn_stat.num_sec_parts > vec_num ) && 
	   ( sec_part_filled > vec_num ) &&
	   ( tn_stat.sec_part ) ) {
	if ( tn_stat.sec_part[vec_num].part_type >= 0 ) {
	  sprintf( ret_val, "%-ld", tn_stat.sec_part[vec_num].part_type );
	} else {
	  ierr = 5;
	}
      } else {
	if ( tn_stat.sec_part) {
	  ierr = ndi_check_array_err(sec_part_filled);
	} else {
	  ierr = -9;
	}
      }
      break;

   case NDI_RPRODS_ID:  
      /* The reaction product ID specified by the index 'vec_num' */
     ierr = ndi_c_get_int_val_n( NDI_RPRODS_ID, vec_num, &return_int );
     sprintf( ret_val, "%-d", return_int );
     break;

   case NDI_REACTANTS_ID:  
      /* The reactant ID specified by the index 'vec_num' */
     ierr = ndi_c_get_int_val_n( NDI_REACTANTS_ID, vec_num, &return_int );
     sprintf( ret_val, "%-d", return_int );
     break;

     /**************  UNKNOWN KEYWORD  *****************/

   default:                                  
      ierr = -4;
      break;
   }

   return(ierr);
}

/**********************************************************************/
static int get_string_n_tnreac( int in_key, char *ret_val, 
				int vec_num, int str_size ) {

   int ierr = 0;      /* Error flag: 0=no error,pos=warning, neg=error */

   /* Default return value is empty string */
   *ret_val = '\0';

   /* Check that isotope is set */
   if ( tnreac_stat.reac_num <= 0 ) {
      return(-7);
   }

   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_LIST_REAC:     /* List of reactions */

      /* check vec_num is less than num of reac */
      if ( tnreac_stat.reac_num > vec_num ) {
	 strncpy( ret_val, tnreac_stat.list_reac[vec_num], str_size );
      } else {
	ierr = ndi_check_array_err(tnreac_stat.reac_num);
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
