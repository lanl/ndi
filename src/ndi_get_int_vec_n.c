
/*
 * File:	ndi_get_int_vec_n.c
 * Description:	User function--return a vector which is an element of a matrix.
 * Time-stamp:	<2000-02-01 18:37:48 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static int get_int_vec_n_mg(int in_key, int vec_num, 
			    int *return_vec, int vec_size);
static int get_int_vec_n_tnreac(int in_key, int vec_num, 
				int *return_vec, int vec_size);

/**********************************************************************/

int ndi_c_get_int_vec_n(int in_key, int vec_num, int *return_vec, 
		      int vec_size ) 

/*  Purpose: Return an integer array which is an element of
    a matrix.
    
    Parameters: 
    in_key -- in: key to specify type of data required
    ret_val -- out: integer value returned
    vec_num -- in: integer specifying which element of matrix to 
                    return (zero-based)

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/

{
   int ierr = 0;            /* Error flag: 0=no error,pos=warning, neg=error */

   ierr = ndi_init_int_vec( return_vec, vec_size );

   /* Check that ndi_init has been called. */
   if (!ndi_initialized) {
      return(-2); }
  

   /* Currently, there is no instance where a negative vec_num can be
      valid.  Check here for that, and return if found. */
   if ( vec_num < 0 ) {
     ierr = -6;
     return (ierr);
   }

   if ( library_type == 'm' ) {
      ierr = get_int_vec_n_mg( in_key, vec_num, return_vec, vec_size );
   } else if (library_type == 'r' ) {
      ierr = get_int_vec_n_tnreac( in_key, vec_num, return_vec, vec_size );
   } else if ( library_type == ' ' ) {
     /* Library type was not set--defaulting to multigroup n */
     library_type = 'm';
     if ( particle_type == ' ' ) {
       particle_type = 'n';
     }
     ierr = get_int_vec_n_mg( in_key, vec_num, return_vec, vec_size );
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
static int get_int_vec_n_mg(int in_key, int vec_num, 
			    int *return_vec, int vec_size) {

   int ierr = 0;     /* Error flag: 0=no error,pos=warning, neg=error */
   int rprod_index;
   int mt = 0;
   int num_elements;        /* Number of vector elements to return */
   stat_flag curr_state;    /* Status indicator of current reaction */


   /*  Check if isotope is set  */
   if (! iso_stat.set_flag) {
      return(-7);
   }
   
   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {   

   case NDI_RPROD_ID_ALL:   
      /* Reac. prod. id for products of reaction indicated by vec_num -- all */
      if ( reac_id == 'c' ) {
	 mt = ndi_c_c_to_mt( vec_num );
	 if ( mt <= 0 ) {
	    ierr = -6;
	    return(-24);     /* Invalid MT or reaction identifier */
	 }
      } else {
	 mt = vec_num;
      }

      ierr = ndi_find_rprod_index(lib_stream, &iso_stat.rprod_all, 
				  &rprod_index, vec_num);
      if (ierr >= 0) {
	if ( rprod_index < iso_stat.rprod_all.num_objs ) {
	  curr_state = iso_stat.rprod_all.stat[rprod_index];
	  num_elements =  curr_state.length;
	  if (curr_state.file_ptr) {
	    ierr = ndi_read_rx_prod(lib_stream, curr_state.file_ptr, &return_vec,
				    "id", &num_elements, READ_MT);
	    (void) ndi_translate_fiss_prod( return_vec, num_elements );

	 } else {
	    /* Error: data not available for this isotope */
	    ierr = -9;
	  }
	} else {
	  ierr = ndi_check_array_err(iso_stat.rprod_all.num_objs);
	}
      } else {
	 /* Error: data not available for this isotope */
	 ierr = -9;
      }
      break;
    
   case NDI_RPROD_MLT_ALL: 
      /* Reac. prod. mults for products of reaction indicated by vec_num--all */
      if ( reac_id == 'c' ) {
	 mt = ndi_c_c_to_mt( vec_num );
	 if ( mt <= 0 ) {
	    ierr = -6;
	    return(-24);     /* Invalid MT or reaction identifier */
	 }
      } else {
	 mt = vec_num;
      }

      ierr = ndi_find_rprod_index(lib_stream, &iso_stat.rprod_all, 
				  &rprod_index, vec_num);
      if (ierr >= 0) {
	if ( rprod_index < iso_stat.rprod_all.num_objs ) {
	  curr_state = iso_stat.rprod_all.stat[rprod_index];
	  num_elements =  curr_state.length;
	  if (curr_state.file_ptr) {
	    ierr = ndi_read_rx_prod(lib_stream, curr_state.file_ptr, &return_vec,
				    "mult", &num_elements, READ_MT);
	 } else {
	    /* Error: data not available for this isotope */
	    ierr = -9;
	  }
	} else {
	  ierr = ndi_check_array_err(iso_stat.rprod_all.num_objs);
	}
      } else {
	 /* Error: data not available for this isotope */
	 ierr = -9;
      }
      
      break;
      
      
   case NDI_RPROD_ID_RMO:  
      /* Reac. prod. id for products of reaction indicated by vec_num -- rmo */
      if ( reac_id == 'c' ) {
	 mt = ndi_c_c_to_mt( vec_num );
	 if ( mt <= 0 ) {
	    ierr = -6;
	    return(-24);     /* Invalid MT or reaction identifier */
	 }
      } else {
	 mt = vec_num;
      }

     ierr = ndi_find_rprod_index(lib_stream, &iso_stat.rprod_rmo, 
				 &rprod_index, vec_num);
     if (ierr >= 0) {
       if ( rprod_index < iso_stat.rprod_rmo.num_objs ) {      
	 curr_state = iso_stat.rprod_rmo.stat[rprod_index];
	 num_elements =  curr_state.length;
	 if (curr_state.file_ptr) {
	    ierr = ndi_read_rx_prod(lib_stream, curr_state.file_ptr, &return_vec,
				    "id", &num_elements, READ_MT );
	    (void) ndi_translate_fiss_prod( return_vec, num_elements );
	 } else {
	   /* Error: data not available for this isotope */
	   ierr = -9;
	   memset(return_vec, 0, num_elements * sizeof(int) );
	 }
       } else {
	 ierr = ndi_check_array_err(iso_stat.rprod_rmo.num_objs);
       }
      } else {
	 /* Error: data not available for this isotope */
	 ierr = -9;
	 /*** Check this ***/
/*	 memset(return_vec, 0, sizeof(return_vec) ); */
      }
      break;
      

   case NDI_RPROD_MLT_RMO: 
      /* Reac. prod. mults for products of reaction indicated by vec_num -- rmo*/
      if ( reac_id == 'c' ) {
	 mt = ndi_c_c_to_mt( vec_num );
	 if ( mt <= 0 ) {
	    ierr = -6;
	    return(-24);     /* Invalid MT or reaction identifier */
	 }
      } else {
	 mt = vec_num;
      }

      ierr = ndi_find_rprod_index(lib_stream, &iso_stat.rprod_rmo, 
				  &rprod_index, vec_num);
      if (ierr >= 0) {
	if ( rprod_index < iso_stat.rprod_rmo.num_objs ) {
	  curr_state = iso_stat.rprod_rmo.stat[rprod_index];
	  num_elements =  curr_state.length;
	  if (curr_state.file_ptr) {
	    ierr = ndi_read_rx_prod(lib_stream, curr_state.file_ptr, &return_vec,
				    "mult", &num_elements, READ_MT );
	 } else {
	    /* Error: data not available for this isotope */
	    ierr = -9;
	    memset(return_vec, 0, num_elements * sizeof(int) );
	  }
	} else {
	  ierr = ndi_check_array_err(iso_stat.rprod_rmo.num_objs);
	}
      }  else {
	 /* Error: data not available for this isotope */
	 ierr = -9;
      }
      
      break;
  

   case NDI_RPROD_ID_FEW:  
      /* Reac. prod. id for products of reaction indicated by vec_num -- few */
      if ( reac_id == 'c' ) {
	 mt = ndi_c_c_to_mt( vec_num );
	 if ( mt <= 0 ) {
	    ierr = -6;
	    return(-24);    /* Invalid MT or reaction identifier */
	 }
      } else {
	 mt = vec_num;
      }

      ierr = ndi_find_rprod_index(lib_stream, &iso_stat.rprod_few, 
				  &rprod_index, vec_num);
      if (ierr >= 0) {
	if ( rprod_index < iso_stat.rprod_few.num_objs ) {
	  curr_state = iso_stat.rprod_few.stat[rprod_index];
	  num_elements =  curr_state.length;
	  if (curr_state.file_ptr) {
	    ierr = ndi_read_rx_prod(lib_stream, curr_state.file_ptr, &return_vec,
				    "id", &num_elements, READ_MT );
	    (void) ndi_translate_fiss_prod( return_vec, num_elements );

	 } else {
	    /* Error: data not available for this isotope */
	    ierr = -9;
	    memset(return_vec, 0, num_elements * sizeof(int) );
	  }
	} else {
	  ierr = ndi_check_array_err(iso_stat.rprod_few.num_objs);
	}
      }  else {
	 /* Error: data not available for this isotope */
	 ierr = -9;
      }
      
      break;


   case NDI_RPROD_MLT_FEW: 
      /* Reac. prod. mult for products of reaction indicated by vec_num -- few */
      if ( reac_id == 'c' ) {
	 mt = ndi_c_c_to_mt( vec_num );
	 if ( mt <= 0 ) {
	    ierr = -6;
	    return(-24);    /* Invalid MT or reaction identifier */
	 }
      } else {
	 mt = vec_num;
      }

      ierr = ndi_find_rprod_index(lib_stream, &iso_stat.rprod_few, 
				  &rprod_index, vec_num);
      if (ierr >= 0) {
	if ( rprod_index < iso_stat.rprod_few.num_objs ) {
	  curr_state = iso_stat.rprod_few.stat[rprod_index];
	  num_elements =  curr_state.length;
	  if (curr_state.file_ptr) {
	    ierr = ndi_read_rx_prod(lib_stream, curr_state.file_ptr, &return_vec,
				    "mult", &num_elements, READ_MT );
	 } else {
	    /* Error: data not available for this isotope */
	    ierr = -9;
	    memset(return_vec, 0, num_elements * sizeof(int) );
	  }
	} else {
	  ierr = ndi_check_array_err(iso_stat.rprod_few.num_objs);
	}
      }  else {
	 /* Error: data not available for this isotope */
	 ierr = -9;
      }

      break;

      /***********  UNKNOWN KEYWORD  **************/

   default:
      ierr = -4;
      break;
   }

   return(ierr);

}



/**********************************************************************/
/**********************************************************************/
static int get_int_vec_n_tnreac(int in_key, int vec_num, 
				int *return_vec, int vec_size) {

   int ierr = 0;     /* Error flag: 0=no error,pos=warning, neg=error */
   int num_elements;

   /*  Check if isotope is set  */
   if ( tnreac_stat.reac_num <= 0 ) {
      return(-7);
   }
   
   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {   

   case NDI_RPRODS_ID:  
      /* The reaction product ID's for reaction specified by the index 'vec_num' */
      if ( tnreac_stat.reac_num > vec_num) {
	num_elements = tnreac_stat.rprods[vec_num].length;
	 ierr = ndi_read_rx_prod( lib_stream, tnreac_stat.rprods[vec_num].file_ptr,
				  &return_vec, "id", &num_elements, 
				  NO_MT );
      } else {
	 ierr = ndi_check_array_err(tnreac_stat.reac_num);
      }
      break;
    
   case NDI_RPRODS_MLT:  
      /* The reaction product multiplicities for reaction specified by vec_num  */
      if ( tnreac_stat.reac_num > vec_num) {
	num_elements = tnreac_stat.rprods[vec_num].length;
	 ierr = ndi_read_rx_prod( lib_stream, tnreac_stat.rprods[vec_num].file_ptr,
				  &return_vec, "mult", &num_elements, 
				  NO_MT );
      } else {
	 ierr = ndi_check_array_err(tnreac_stat.reac_num);
      }
      break;
    
   case NDI_REACTANTS_ID:  
      /* The reactant ID's for reaction specified by the index 'vec_num' */
      if ( tnreac_stat.reac_num > vec_num) {
	num_elements = tnreac_stat.reactants[vec_num].length;
	 ierr = ndi_read_rx_prod( lib_stream, tnreac_stat.reactants[vec_num].file_ptr,
				  &return_vec, "id", &num_elements, 
				  NO_MT );
      } else {
	 ierr = ndi_check_array_err(tnreac_stat.reac_num);
      }
      break;
    
   case NDI_REACTANTS_MLT:  
      /* The reactant multiplicities for reaction specified by vec_num  */
      if ( tnreac_stat.reac_num > vec_num) {
	num_elements = tnreac_stat.reactants[vec_num].length;
	 ierr = ndi_read_rx_prod( lib_stream, tnreac_stat.reactants[vec_num].file_ptr,
				  &return_vec, "mult", 
				  &num_elements, 
				  NO_MT );
      } else {
	 ierr = ndi_check_array_err(tnreac_stat.reac_num);
      }
      break;
      
      /***********  UNKNOWN KEYWORD  **************/
      
   default:
      ierr = -4;
      break;
   }

   return(ierr);

}

/**********************************************************************/
/**********************************************************************/
