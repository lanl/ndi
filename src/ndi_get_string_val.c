
/*
 * File:	ndi_get_string_val.c
 * Description:	User function--return a string value.
 * Time-stamp:	<1999-11-18 18:19:39 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

int ndi_c_get_string_val_h( state_struct *state, int in_key, char *ret_val, int str_size );
static int get_string_mg(  state_struct *state, int in_key, char *ret_val, int str_size);
static int get_string_tn(  state_struct *state, int in_key, char *ret_val, int str_size);
static int get_string_tnreac(  state_struct *state, int in_key, char *ret_val, int str_size );

/**********************************************************************/

int ndi_c_get_string_val_h( state_struct *state, int in_key, char *ret_val, int str_size ) 

/*  Purpose: Return a single string value.
    
    Parameters: 
    in_key -- in: key to specify type of data required
    ret_val -- out: string value returned
    str_size -- in: tells how large the string is (how many characters)

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
   if (!state) {
       return(-2); 
   } else if ( in_key == NDI_VERSION ) {
       /* Special case--library type need not be set if asking for version. */
       strncpy( ret_val, NDI_VERSION_DATA, str_size );
   }  else if (!state->ndi_initialized) {
       return(-2);       
   } else if ( in_key == NDI_GENDIR_PATH ) {
       strncpy( ret_val, state->ndi_gendir_path, str_size );
   }
    

   if ( ( state->library_type == 'm' ) || 
        ( state->library_type == 'd' ) ) {
       ierr = get_string_mg( state, in_key, ret_val, str_size );
   } else if ( state->library_type == 's' ) {
       ierr = get_string_tn( state, in_key, ret_val, str_size );
   } else if ( state->library_type == 'r' ) {
       ierr = get_string_tnreac( state, in_key, ret_val, str_size );
       /*
         } else if ( state->library_type == '0' ) {
         ierr = get_string_depl( in_key, ret_val, str_size );
       */
   } else if ( state->library_type == ' ' ) {
       /* Library type was not set */
       ierr = -16;
   } else {
       /* Incompatible library type -- bad input key */ 
       ierr = -4;
   }

   return(ierr);

}

/**********************************************************************/
/**********************************************************************/
static int get_string_mg(  state_struct *state, int in_key, char *ret_val, int str_size ) {

   int ierr = 0;      /* Error flag: 0=no error,pos=warning, neg=error */

        if (! state) {
            return(-2);
        }

   /* Default return value is empty string */
   if ( (ret_val) && ( str_size > 0 ))
       *ret_val = '\0';

   if ( (in_key != NDI_DEPL_INFO) &&
        (in_key != NDI_DEPL_ZAID) ) {
       if ( (! state->iso_stat.set_flag)) {
           return(-7);
       }
   }

   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_ZAID:                                  /*  NDI_ZAID  */
     /* Check that isotope is set */
       strncpy( ret_val, state->iso_stat.zaid, str_size );
       break;
     
       /***  Change these to use strncpy when bounds checking is implemented */

   case NDI_INFO:                                  /*  Information  */
     /* Check that isotope is set */
     strncpy( ret_val, state->iso_stat.info, str_size );
     break;

   case NDI_DATE_PROCESSED:                           /* Date library was processed */
     /* Check that isotope is set */
     strncpy( ret_val, state->iso_stat.date_processed, str_size );
     break;

   case NDI_DATE_SOURCE:                              /* Date of library source info */
     /* Check that isotope is set */
     strncpy( ret_val, state->iso_stat.date_source, str_size );
     break;

   case NDI_LIBRARY_NAME:                    /* Name of current library */
     /* Check that isotope is set */
     strncpy( ret_val, state->iso_stat.library_name, str_size );
     break;

   case NDI_DEPL_ZAID:                                  /* Depletion ZAID  */
       if ( ! state->ndi_depl_set ) {
           ierr = ndi_set_default_depl(state);
       }

     if ( state->num_target <= 0 ) {
       return(-7);
     }
     strncpy( ret_val, state->depl_dataset, str_size );
     break;

      /***  Change these to use strncpy when bounds checking is implemented */

   case NDI_DEPL_INFO:                                  /*  Depletion nformation  */
       if ( ! state->ndi_depl_set ) {
           ierr = ndi_set_default_depl(state);
       }

     if ( state->num_target <= 0 ) {
       return(-7);
     }
     strncpy( ret_val, state->depl_info, str_size );
     break;


   case NDI_SEC_PART_TYPES:                /* Type of current particle */
     /* Check also curr_part_index < num_parts?  */
     if ( state->curr_part_index >= 0 ) {
         if ( sprintf ( ret_val, "%d", state->iso_stat.sec_part[state->curr_part_index].part_type) > str_size ) {
             /** ??? **/
         }
     } else {
       /* Error -- bad particle? */
       ierr = -999;
     }
     break;

   case NDI_SEC_PART_ZAIDS:                /* ZAID of current particle */
     if ( state->curr_part_index >= 0 ) {
     strncpy( ret_val, state->iso_stat.sec_part[state->curr_part_index].zaid, str_size );
     } else {
       /* Error -- bad particle? */
       ierr = -999;
     }
     break;


/*  Note:  version request handled specially above */
/*   case NDI_VERSION:     */                      /* Interface version data */
/*      strncpy( ret_val, NDI_VERSION_DATA, str_size );
      break;
*/
      /**************  UNKNOWN KEYWORD  *****************/

   default:                                  
      ierr = -4;
      break;
   }

   return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int get_string_tn(  state_struct *state, int in_key, char *ret_val, int str_size ) {

   int ierr = 0;      /* Error flag: 0=no error,pos=warning, neg=error */

   if (! state) {
       return(-2);
   }

   /* Default return value is empty string */
   if ( (ret_val) && ( str_size > 0 ))
       *ret_val = '\0';

   /* Check that isotope is set */
   if (! state->tn_stat.set_flag) {
      return(-7);
   }

   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_ZAID:                                  /*  NDI_ZAID  */
      strncpy( ret_val, state->tn_stat.zaid, str_size );
      break;

      /***  Change these to use strncpy when bounds checking is implemented */

   case NDI_INFO:                                  /*  Information  */
      strncpy( ret_val, state->tn_stat.info, str_size );
      break;

   case NDI_DATE_PROCESSED:                           /* Date library was processed */
     strncpy( ret_val, state->tn_stat.date_processed, str_size );
     break;

   case NDI_DATE_SOURCE:                              /* Date of library source info */
     strncpy( ret_val, state->tn_stat.date_source, str_size );
     break;

   case NDI_LIBRARY_NAME:                    /* Name of current library */
     strncpy( ret_val, state->tn_stat.library_name, str_size );
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

static int get_string_tnreac(  state_struct *state, int in_key, char *ret_val, 
			      int str_size ) {

   int ierr = 0;      /* Error flag: 0=no error,pos=warning, neg=error */

   if (! state) {
        return(-2);
   }

   /* Default return value is empty string */
   if ( (ret_val) && ( str_size > 0 ))
       *ret_val = '\0';

   /* Check that isotope is set */
   if ( state->tnreac_stat.reac_num <= 0 ) {
      return(-7);
   }

   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_ZAID:                                  /*  NDI_ZAID  */
      strncpy( ret_val, state->tnreac_stat.dataset, str_size );
      break;

      /***  Change these to use strncpy when bounds checking is implemented */

   case NDI_INFO:                                  /*  Information  */
      if ( strncmp(state->tnreac_stat.info, "\0", 1) != 0 ) { 
	 strncpy( ret_val, state->tnreac_stat.info, str_size );
      } else {
	 strncpy( ret_val, "\0", 1);
	 ierr = -9;
      }
      break;


   case NDI_DATE_PROCESSED:                           /* Date library was processed */
     strncpy( ret_val, state->tnreac_stat.date_processed, str_size );
     break;

   case NDI_DATE_SOURCE:                              /* Date of library source info */
     /* Check that isotope is set */
     strncpy( ret_val, state->tnreac_stat.date_source, str_size );
     break;

   case NDI_LIBRARY_NAME:                    /* Name of current library */
     strncpy( ret_val, state->tnreac_stat.library_name, str_size );
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
