
/*
 * File:	ndi_get_float_val_n.c
 * Description:	User function--return an element of a vector (float).
 * Time-stamp:	<1999-04-07 15:05:24 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_var_type.h"

static int GET_PROD_Q( const int rprod_in, FLOAT_TYPE *return_val );

/**********************************************************************/


int NDI_GET_FLOAT_VAL_N(int in_key, int vec_num, 
			FLOAT_TYPE *ret_val) 

/**********************************************************************/

/*  Purpose: Return a single real value which is an element of
    a vector.
    
    Parameters: 
    in_key -- in: key to specify type of data required
    ret_val -- out: integer value returned
    vec_num -- in: integer specifying which element of array to 
                    return (zero-based)

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/

{
   int ierr = 0;    /* Error flag: 0=no error,pos=warning, neg=error */
   int reac_index;
   int mt = 0;
   int tmp_err = 0;


   *ret_val = 0.0;    /* Default return value is zero */

   /* Check that ndi_init has been called. */
   if (!ndi_initialized) {
      return(-2); }

#ifdef FLOAT32
   if (  sizeof(float) == 8 ) {
      /*  32-bit operation not implemented on 64-bit platform */
      return(-10);      
   }
#endif


   /* Currently, there is no instance where a negative vec_num can be
      valid.  Check here for that, and return if found. */
   if ( vec_num < 0 ) {
     ierr = -6;
     return (ierr);
   }

   if ( library_type == ' ' ) {
     /* Library type was not set--defaulting to multigroup n */
     library_type = 'm';
     if ( particle_type == ' ' ) {
       particle_type = 'n';
     }
     tmp_err = 8;
   } else if ( ( library_type != 'm' ) &&
	       ( library_type != 'd' ) ) {
      /* Incompatible library type */
      return(-16);
   }

   /* Check that isotope is set */
   if (! iso_stat.set_flag) {
      return(-7);
   }

   /*  Process input key  */
   switch (in_key) {

      /***********************************************************************/
      /*           This section looks at non-standard isotope data           */
      /***********************************************************************/

   case NDI_REAC_Q:                      
      /* Reaction Q for reaction indexed by vec_num */
      /* Change this test */
      if ( reac_id == 'c' ) {
	 mt = ndi_c_c_to_mt( vec_num );
	 if ( mt <= 0 ) {
	    ierr = -6;
	    return(-24);   /* Invalid MT or reaction identifier */
	 }
      } else {
	 mt = vec_num;
      }

      ierr = ndi_find_reac_index( iso_stat.reac_mt, &reac_index, 
				  mt, iso_stat.num_reac );
      if ( ( ierr >= 0 ) && ( iso_stat.reac_q ) ){
	if ( reac_index < iso_stat.num_reac ) {
	  *ret_val = (FLOAT_TYPE) iso_stat.reac_q[reac_index];
	} else {
	  ierr = ndi_check_array_err(iso_stat.num_reac);
	}
      } else {
	 /* Data not in library. */
	 *ret_val = 0.0;
	 ierr = 9;
      }
      break;

   case NDI_ABUNDANCE:
     ierr = GET_ABUNDANCE( vec_num, ret_val );
     break;

   case NDI_TOT_PROD_Q:

     /* Check if incoming rprod zaid is the default fission product */
     if ( vec_num == atoi(def_fiss_prod_id) ) {
       ierr = GET_PROD_Q(99999, ret_val);
     } else {
       ierr = GET_PROD_Q(vec_num, ret_val);
     }

     break;


      /**********  UNKNOWN KEYWORD  ***********/

   default:               
      *ret_val = 0.0;
      ierr = -4;
      break;
   }
  
   if ( ( tmp_err != 0 ) && ( ierr >= 0 ) ) {
     ierr = tmp_err;
   }
   return(ierr);
}


/*********************************************************************/
/*********************************************************************/

static int GET_PROD_Q( const int rprod_in, FLOAT_TYPE *return_val )

{
   int i;
   int ierr = 0;
   int curr_index = -1;
   int curr_mt;
   int q_err = 0;
   int reac_index;
   FLOAT_TYPE max_q = 0.0;
   depl_target_struct *curr_targ = depl_target;
   depl_rprod_struct *curr_prod;
   mg_sec_part_type rprod_str;
   

   /* CHECK ERRORS */

   *return_val = 0.0;

   if ( ! ndi_depl_set ) {
     ierr = ndi_set_default_depl(); 
   }
   if ( num_target <= 0 ) {
     return(-7);
   }

   ierr = ndi_set_curr_target( &curr_targ, &curr_index );
   if ( curr_index == -1 ) {
     return( ierr );
   }
   

   curr_prod = curr_targ->rprod;
   curr_index = curr_targ->num_rprods;

   
   sprintf( rprod_str, "%d", rprod_in );
   ierr = ndi_set_curr_rprod( &curr_prod, rprod_str, &curr_index );
   if ( curr_index == -1 ) {
     return( ierr );
   }


   /* For each reaction, read MT and mult */
   /*     find maximum Q value */
   
   for ( i = 0; i < curr_prod->num_reac; i++ ) {
     curr_mt = curr_prod->mt[i];
     /* Must change get_sig_reac to not convert to c values */

     q_err = ndi_find_reac_index( iso_stat.reac_mt, &reac_index,
				  curr_mt, iso_stat.num_reac );
     if ( q_err >= 0 ) {
       if ( reac_index < iso_stat.num_reac ) {
	 /*  What about negative Q values ? */
	 if ( fabs(max_q) < fabs(iso_stat.reac_q[reac_index]) ) {
	   max_q = iso_stat.reac_q[reac_index];
	 }
       } else {
	 /* Error -- subscript out of range */
	 ierr = -31;
       }
     } else {
       ierr = q_err;
     }
   }

   *return_val = max_q;
   if ( (q_err < 0) && ( ierr >= 0) ) {
     ierr = q_err;
   } else if ((q_err > 0) && ( ierr == 0)) {
     ierr = q_err;
   }

   if ( ierr == -9 ) {
     ierr = 9;
   }

   return(ierr);
}

/*********************************************************************/
/*********************************************************************/
