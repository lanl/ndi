
/*
 * File:	ndi_get_float_val.c
 * Description:	User function--returns a real value.
 * Time-stamp:	<99/10/20 10:57:37 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_var_type.h"

static int GET_FLOAT_VAL_MG( int in_key, FLOAT_TYPE *ret_val );
static int GET_FLOAT_VAL_TN( int in_key, FLOAT_TYPE *ret_val );
static int GET_FLOAT_VAL_DOS( int in_key, FLOAT_TYPE *ret_val );
static int CALC_EAVG_OUT( FILE *lib_stream, FLOAT_TYPE *ret_val );

/**********************************************************************/

int NDI_GET_FLOAT_VAL( int in_key, FLOAT_TYPE *ret_val) 


/*********************************************************************
  
    Purpose: Return a single integer value which is an element of
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
   int ierr = 0;      /* Error flag: 0=no error,pos=warning, neg=error */

   *ret_val = 0.0;    /* Default return value is zero */

   /* Check that ndi_init has been called. */
   if (!ndi_initialized) {
      return(-2); }

#ifdef FLOAT32
   if ( sizeof(float) == 8 ) {
      /*  32-bit operation not implemented on 64-bit platform */
      return(-10);      
   }
#endif

   if ( library_type == 'm' ) {
      ierr = GET_FLOAT_VAL_MG( in_key, ret_val );
   } else if ( library_type == 's' ) {
      ierr = GET_FLOAT_VAL_TN( in_key, ret_val );
   } else if ( library_type == 'd' ) {
      ierr = GET_FLOAT_VAL_DOS( in_key, ret_val );
   } else if ( library_type == ' ' ) {
     /* Library type was not set--defaulting to multigroup n */
     library_type = 'm';
     if ( particle_type == ' ' ) {
       particle_type = 'n';
     }
     ierr = GET_FLOAT_VAL_MG( in_key, ret_val );
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

static int GET_FLOAT_VAL_MG( int in_key, FLOAT_TYPE *ret_val ) {

   int ierr = 0;
   
   /* Check that isotope is set */
   if (! iso_stat.set_flag) {
      return(-7);
   }

   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_TEMP:                           
      /*  Temperature  */
      if ( iso_stat.temp >= 0 ) {
	 *ret_val = (FLOAT_TYPE) iso_stat.temp;
      } else {
	 ierr = -9;
	 *ret_val = 0.0;
      }
      break;
    
   case NDI_SIG_0:                            
      /* Sigma zero  */
      if ( iso_stat.sig_0 >= 0 ) {
	 *ret_val = (FLOAT_TYPE) iso_stat.sig_0;
      } else {
	 ierr = -9;
	 *ret_val = 0.0;
      }
      break;
    
   case NDI_AT_WGT:                         
      /* Atomic weight  */
      if ( iso_stat.at_wgt >= 0 ) {
	 *ret_val = (FLOAT_TYPE) iso_stat.at_wgt;
      } else {
	 ierr = -9;
	 *ret_val = 0.0;
      }
      break;
    
   case NDI_AWR:                   
      /* Atomic weight ratio  */
      if ( iso_stat.awr >= 0 ) {
	 *ret_val = (FLOAT_TYPE) iso_stat.awr;
      } else {
	 ierr = -9;
	 *ret_val = 0.0;
      }
      break;
    
      /*******************  UNKNOWN KEYWORD  ********************/
    
   default:                                  
      ierr = -4;
      break;
   }
  
   return(ierr);
}

/**********************************************************************/
/**********************************************************************/
static int GET_FLOAT_VAL_TN( int in_key, FLOAT_TYPE *ret_val ) {

   int ierr = 0;
  
   /* Check that isotope is set */
   if (! tn_stat.set_flag) {
      return(-7);
   }

   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

   case NDI_REAC_Q:       
      /* Reaction Q-value */
      if ( tn_stat.q_value >= 0 ) {
	 *ret_val = (FLOAT_TYPE) tn_stat.q_value;
      } else {
	 ierr = -9;
	 *ret_val = 0.0;
      }
      break;

   case NDI_EAVG_OUT:
   case NDI_EOUTBAR:
     ierr = CALC_EAVG_OUT( lib_stream, ret_val );
     break;

   
      /*******************  UNKNOWN KEYWORD  ********************/
    
   default:                                  
      ierr = -4;
      break;
   }
  
   return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int GET_FLOAT_VAL_DOS( int in_key, FLOAT_TYPE *ret_val ) {

   int ierr = 0;
   
   /* Check that isotope is set */
   if (! iso_stat.set_flag) {
      return(-7);
   }

   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {

/*    case NDI_ABUNDANCE:                            */
      /*  Abundance  */
     /*
      if ( iso_stat.abundance >= 0 ) {
	 *ret_val = (FLOAT_TYPE) iso_stat.abundance;
      } else {
	 ierr = -9;
	 *ret_val = 0.0;
      }
      break;
      */

   case NDI_TEMP:                           
      /*  Temperature  */
      if ( iso_stat.temp >= 0 ) {
	 *ret_val = (FLOAT_TYPE) iso_stat.temp;
      } else {
	 ierr = -9;
	 *ret_val = 0.0;
      }
      break;
    
   case NDI_SIG_0:                            
      /* Sigma zero  */
      if ( iso_stat.sig_0 >= 0 ) {
	 *ret_val = (FLOAT_TYPE) iso_stat.sig_0;
      } else {
	 ierr = -9;
	 *ret_val = 0.0;
      }
      break;
    
   case NDI_AT_WGT:                         
      /* Atomic weight  */
      if ( iso_stat.at_wgt >= 0 ) {
	 *ret_val = (FLOAT_TYPE) iso_stat.at_wgt;
      } else {
	 ierr = -9;
	 *ret_val = 0.0;
      }
      break;
    
   case NDI_AWR:                   
      /* Atomic weight ratio  */
      if ( iso_stat.awr >= 0 ) {
	 *ret_val = (FLOAT_TYPE) iso_stat.awr;
      } else {
	 ierr = -9;
	 *ret_val = 0.0;
      }
      break;
    
      /*******************  UNKNOWN KEYWORD  ********************/
    
   default:                                  
      ierr = -4;
      break;
   }
  
   return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int CALC_EAVG_OUT( FILE *lib_stream, FLOAT_TYPE *ret_val )
     /* Calculates the average outgoing energy of the "current" secondary 
	particle (given by global var. curr_part_index) by integrating
	over that particles energy distribution
	*/
     
{
  int ierr = 0;
  int num_temps;
  int hi, lo;
  int curr_temp_index = -1;
  long int curr_file_p;
  double *distr_E = NULL;
  double *distr_prob = NULL;
  double Eavg;
  sec_part_distr *part;
  interp_params curr_interp;


  /* INITIALIZE */

  /* Set current particle */
  if ( ( curr_part_index < 0 ) ||
       ( curr_part_index >= sec_part_filled ) )
    return( -30 ); 

  part = &(tn_stat.sec_part[curr_part_index]);
  if ( !part ) return( -30 );

  /* Find closest temperature */
  num_temps = part->temp_interp.npts;
  ierr = ndi_find_temp_bound_index( part->edist, num_temps, &hi,&lo, &curr_temp_index );
  if ( ierr < 0 )  return( -30 );
   
        
  /* Initialize distribution variables */
  curr_interp = part->edist[curr_temp_index].interp;
  curr_file_p = part->edist[curr_temp_index].file_ptr;
  distr_E = (double *)ndi_malloc( curr_interp.npts * sizeof(double) );
  distr_prob = (double *)ndi_malloc( curr_interp.npts * sizeof(double) );
  ierr = ndi_read_float_pair( lib_stream, curr_file_p, 
			      curr_interp.npts, distr_E, distr_prob,
			      curr_interp.npts );

  /* FIND AVERAGE ENERGY */
  ierr = find_average_E( distr_E, distr_prob, &curr_interp, &Eavg );

  /* Check error */
  if ( ierr < 0 )  {
    if (distr_E) ndi_free( (void **) &distr_E);
    if (distr_prob) ndi_free( (void **) &distr_prob);
    return( -30 );
  }


  if (distr_E) ndi_free( (void **) &distr_E);
  if (distr_prob) ndi_free( (void **) &distr_prob);
   
  /* Check error */

  /* Change from double to correct float type */
  *ret_val = (FLOAT_TYPE)Eavg;

  return(ierr);
}

/**********************************************************************/
/**********************************************************************/
