
/*
 * File:	ndi_get_float_vec_x.c
 * Description:	Source for libndi.a
 * Time-stamp:	<2000-02-03 15:26:44 jomc>
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


/**********************************************************************/

int NDI_GET_FLOAT_VEC_X(int in_key, const char *in_str, 
			FLOAT_TYPE *return_vec, int vec_size) {

   int ierr = 0;
   int tmp_err = 0;

    /* Initialize return_vec with zeroes */
   ierr = NDI_INIT_FLOAT_VEC( return_vec, vec_size );

  /* Check that ndi_init has been called. */
   if (!ndi_initialized) {
      return(-2); }

#ifdef FLOAT32
   if ( sizeof(float) == 8 ) {
      ierr = -2;
      return(ierr);      
   }
#endif

   if ( library_type == ' ' ) {
     /* Library type was not set--defaulting to multigroup n */
     library_type = 'm';
     if ( particle_type == ' ' ) {
       particle_type = 'n';
     }
     tmp_err = 8;
   } else if ( ( library_type != 'm' ) &&
	       ( library_type != 'd' ) ) {
      /* Incompatible library */
      return(-16);
   }

   /* Check that isotope is set */
   if (! iso_stat.set_flag) {
      return(-7);
   }
  
   /*************************/
   /*   Process input key   */
   /*************************/
    
   switch (in_key) {

   case NDI_TOT_PROD_XS:       /* Total production x.s. for target given by 
				  global var. 'zaid' and product given by in_str */
     if ( strcmp(in_str, def_fiss_prod_id ) == 0 ) {
        ierr = GET_PROD_REAC( "99999", return_vec, vec_size ); 
     } else {
       ierr = GET_PROD_REAC( in_str, return_vec, vec_size );     
     }
     break;



      /**********  UNKNOWN KEYWORD  ***********/
     
   default:    
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

int GET_PROD_REAC( const char *rprod_in, FLOAT_TYPE *return_vec, int vec_size )

{
   int i, j;
   int ierr = 0;
   int curr_index = -1;
   int num_grps = 0;
   int num_elements;
   int curr_mt;
   int sig_reac_err = 0;
   FLOAT_TYPE *curr_xs = NULL;
   depl_target_struct *curr_targ = depl_target;
   depl_rprod_struct *curr_prod;


   /* CHECK ERRORS */


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
   ierr = ndi_set_curr_rprod( &curr_prod, rprod_in, &curr_index );
   if ( curr_index == -1 ) {
     return( ierr );
   }


   /* Determine no. of groups and create temporary x.s. array */
   if ( collapse_flag ) {
     num_grps = grp_num_new;
   } else {
     num_grps = iso_stat.num_grps;
   }
   if ( num_grps <= 0 ) {
     /* Bad energy group structure */
     ierr = 5;    
     return( ierr );
   }
   curr_xs = (FLOAT_TYPE *)ndi_malloc( num_grps * sizeof(FLOAT_TYPE) );
   if (!curr_xs) {
     /* Out of memory */
     return(-14);
   }


   /* For each reaction, read MT and mult */
   /*     find sig_reac for MT, multiply by mult. and  and to total */
   
   for ( i = 0; i < curr_prod->num_reac; i++ ) {
     curr_mt = curr_prod->mt[i];
     /* Must change get_sig_reac to not convert to c values */
     num_elements = num_grps;
     sig_reac_err = GET_SIG_REAC( &iso_stat, curr_mt, curr_xs, &num_elements );
     if ( sig_reac_err == 9 ) {
       ierr = 9;
     } else {
       for ( j = 0; j < num_grps; j++ ) {
	 if ( j < vec_size ) {
	   return_vec[j] = return_vec[j] + curr_xs[j] * curr_prod->mult[i];
	 } else {
	   /* Warning--array size not large enough */
	   if ( ierr != 9 ) {     /* No data warning takes precedence 
				     over too small array warning */
	     ierr = 7;
	   }
	 }
       }
     }
   }

   if ( curr_xs ) ndi_free( (void **)&curr_xs );
   return(ierr);
}

/*********************************************************************/
/*********************************************************************/
