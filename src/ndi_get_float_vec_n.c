
/*
 * File:	ndi_get_float_vec_.c
 * Description:	Source for libndi.a
 * Time-stamp:	<02/08/29 09:03:44 jomc>
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

static int NDI_DIAG2FULL(FLOAT_TYPE *low_diag, int num_ld, FLOAT_TYPE *up_diag, 
		  int num_ud, int num_row, int num_col, 
		  FLOAT_TYPE *return_vec);
static int GET_FULL_PN_MAT (FILE *stream, multi_stat full_mat, 
			    multi_stat down_mat, int num_grps, 
			    int vec_num, FLOAT_TYPE *return_mat, 
			    int return_size );
static int NDI_COLLAPSE_PROD_MAT(FLOAT_TYPE *old_mat, 
				 int old_in, int old_out,
				 int new_in,
				 FLOAT_TYPE *return_vec, int return_size );
static int NDI_FULL2DIAG( FLOAT_TYPE *full_mat, int num_grps, FLOAT_TYPE *return_vec,
			  int return_size );
static int GET_PN_DOWN_FULL(FILE *stream, FLOAT_TYPE *return_vec,
			    multi_stat *down_diag, multi_stat *full_mat,
			    int vec_num, int vec_size );
static int PN_DOWN_FULL_TRANS_CORR( FLOAT_TYPE *return_vec, int return_size );
static int PN_FULL_TRANS_CORR( FLOAT_TYPE *return_vec, int return_size );
static int GET_PN_PROD_FULL( FILE *stream, sec_part_mg sec_part,
			     int vec_num,
			     FLOAT_TYPE *return_vec, int return_size );
static int GET_DIAG_CORR( FLOAT_TYPE *return_vec, int vec_num );
static int GET_BHS_CORR( FLOAT_TYPE *return_vec, int vec_num );
static int GET_TRANSFER_MAT( FLOAT_TYPE *return_vec, int vec_num, 
			     int n_type );

/**********************************************************************/

int NDI_GET_FLOAT_VEC_N(int in_key, int vec_num, 
			FLOAT_TYPE *return_vec, int return_size) {

   int ierr = 0;
   int tmp_err = 0;
   int num_elements;
   char in_str[6];


   /* Initialize return vector with zeroes */
   NDI_INIT_FLOAT_VEC( return_vec, return_size );

   /* Check that ndi_init has been called. */
   if (!ndi_initialized) {
      return(-2); }

#ifdef FLOAT32
   if ( sizeof(float) == 8 ) {
      ierr = -2;
      return(ierr);      
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
   } else if ( ( library_type != 'm' )&&
	       (library_type != 'd' ) ) {
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

   case NDI_PN_DOWN_FULL:      /* Pn scattering matrix -- downscatter diagonal */

      num_elements = iso_stat.num_grps;

      ierr = GET_PN_DOWN_FULL(lib_stream, return_vec, &iso_stat.pn_down_full, 
			      &iso_stat.pn_full, vec_num, return_size );

      /* Apply transport correction, if necessary */
      /*** Test that scattering order is less than transport correction order ***/
      if ( (ierr >= 0) && (trans_corr != ' ') ) { 
	 ierr = PN_DOWN_FULL_TRANS_CORR( return_vec, return_size );
      }
      /*  Transpose isn't applied to diagonal */

      break;

   case NDI_PN_FULL:                       /* Full Pn scattering matrix  */
      /*  Full scattering matrices are not currently supported in the library,
	  so pn_full should not contain data  */
     
      num_elements = iso_stat.num_grps;
      ierr = GET_FULL_PN_MAT( lib_stream, iso_stat.pn_full, iso_stat.pn_down_full, 
			      num_elements, vec_num, return_vec, return_size );

      /* Apply transport correction, if necessary */
      /*** Test that scattering order is less than transport correction order ***/
      if ( (ierr >= 0) && (trans_corr != ' ') ) { 
	 ierr = PN_FULL_TRANS_CORR( return_vec, return_size );
      }
      /* Transpose result, if necessary */
      if ( collapse_flag != 0 ) {
	 num_elements =  grp_num_new;              
      } else {
	 num_elements = iso_stat.num_grps;
      }
      if ( ( mat_order == 't' ) && ( ierr >= 0 ) ) {
	 /***  CHECK THAT THE NUMBER OF ROWS AND COLUMNS INPUT ARE CORRECT!!!! */
	 ierr = NDI_TRANSPOSE_FLOAT( return_vec, num_elements, 
				     num_elements );
      }
      break;
     
   case NDI_SIG_REAC:                 /* Sigma edit cross sections */
     num_elements = return_size;
     ierr = GET_SIG_REAC( &iso_stat,  vec_num, return_vec, &num_elements );
     break;


   case NDI_PN_PROD_FULL_CURR_PART:        /* Full production matrix for current particle */

      /*** ADD PHOTON COLLAPSING!!?!  change input to transpose ***/
     /* Initialize to zeroes? */
     (void) NDI_INIT_FLOAT_VEC( return_vec, return_size );


     if ( curr_part_index >= 0 ) {
       if ( curr_part_index < iso_stat.num_sec_parts ) { 
	 ierr = GET_PN_PROD_FULL( lib_stream, iso_stat.sec_part[curr_part_index], 
				  vec_num,
				  return_vec, return_size );

	 /* Transpose result, if necessary */
	 if ( collapse_flag != 0 ) {
	   num_elements =  grp_num_new;              
	 } else {
	   num_elements = iso_stat.num_grps;
	 }
       
	 if ( ( mat_order == 't' ) && ( ierr >= 0 ) ) {
	   /***  CHECK THAT THE NUMBER OF ROWS AND COLUMNS INPUT ARE CORRECT!!!! */
	   ierr = NDI_TRANSPOSE_FLOAT( return_vec, iso_stat.sec_part[curr_part_index].num_grps, 
				       num_elements );
	 }
       } else {
	 ierr = ndi_check_array_err(iso_stat.num_sec_parts);
       }
     } else {
       /* Error -- bad particle? */
       ierr = -999;
     }
      
      break;

   case NDI_TRANSFER_MAT_TOT:                   /* Transfer matrix -- total */
   case NDI_TRANSFER_MAT_PR:                    /* Transfer matrix -- prompt */
      ierr = GET_TRANSFER_MAT( return_vec, vec_num, in_key );

      /* Transpose result, if necessary */
      if ( collapse_flag != 0 ) {
	 num_elements =  grp_num_new;              
      } else {
	 num_elements = iso_stat.num_grps;
      }

      if ( ( mat_order == 't' ) && ( ierr >= 0 ) ) {
	 /***  CHECK THAT THE NUMBER OF ROWS AND COLUMNS INPUT ARE CORRECT!!!! */
	 ierr = NDI_TRANSPOSE_FLOAT( return_vec, num_elements, 
				     num_elements );
      }
      break;

   case NDI_CORR_DIAG:        /* Diagonal transport correction  */

      ierr = GET_DIAG_CORR ( return_vec, vec_num );
      break;
     
   case NDI_CORR_BHS:        /* Bell-Hansen-Sandmeier transport correction */

      ierr = GET_BHS_CORR ( return_vec, vec_num );
      break;

   case NDI_TOT_PROD_XS:       /* Total production x.s. for target given by 
				  global var. 'zaid' and product given by vec_num */
     sprintf( in_str, "%d", vec_num );
     ierr = GET_PROD_REAC( in_str, return_vec, return_size );     
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

static int GET_PN_DOWN_FULL(FILE *stream, FLOAT_TYPE *return_vec,
			    multi_stat *down_diag, multi_stat *full_mat,
			    int vec_num, int return_size ) {

/* Will not construct a diagonal from a full matrix.  Is this needed? */

   int ierr = 0;
   int full = 0;         /*  Is scattering info in full matrix form, 0=no,-1=yes */
   int num_grps = iso_stat.num_grps;
   int diag_size = ( num_grps * ( num_grps + 1) ) / 2;
   int full_mat_size = num_grps * num_grps;
   int curr_vec_size = return_size;
   FLOAT_TYPE *curr_vec = return_vec;
   FLOAT_TYPE *curr_mat = NULL;
   FLOAT_TYPE *temp_vec = NULL;
   FLOAT_TYPE *temp_mat = NULL;

   NDI_INIT_FLOAT_VEC( return_vec, return_size );

   if ( num_grps <= 0 ) {
     ierr = 9;
     return(ierr);
   }

   if (down_diag->num_objs > 0 ) {
     /* The diagonal is available in the library */
     if (down_diag->num_objs > vec_num) {
       if (down_diag->stat[vec_num].file_ptr) {
	 
	 if ( collapse_flag ) {
	   temp_vec = (FLOAT_TYPE *)ndi_malloc( diag_size * sizeof(FLOAT_TYPE) );
	   curr_vec_size = diag_size;
	   if (!temp_vec) return(-14);
	   curr_vec = temp_vec;
	 }
	 ierr = NDI_READ_FLOAT_BLOCK(stream, down_diag->stat[vec_num].file_ptr,
				     diag_size,
				     curr_vec, curr_vec_size );
	 if ( collapse_flag ) {
	   temp_mat = (FLOAT_TYPE *)ndi_malloc( full_mat_size * sizeof(FLOAT_TYPE) );
	   if (!temp_mat) return(-14);
	   ierr = NDI_DIAG2FULL( NULL, 0, curr_vec, diag_size, num_grps, num_grps, 
				 temp_mat );
	   full = -1;
	   curr_mat = temp_mat;
	   if ( temp_vec ) ndi_free( (void**)&temp_vec );
	 } else {
	    /* Diagonal read in, no collapsing -- just return */
	 }
       } else {
	 /* No file pointer set */
	 ierr = 9;
       }
     } else {
       /* Error: index out of range */
 	 ierr = ndi_check_array_err(down_diag->num_objs);
     }

   } else {
     /* Try constructing diagonal from full matrix */
     if ( full_mat->num_objs > 0 ) {
       if ( full_mat->num_objs > vec_num ) {
	 if ( full_mat->stat[vec_num].file_ptr ) {
	   /*  The diagonal not in library,  construct it from full pn matrix */
	   temp_mat = (FLOAT_TYPE *)ndi_malloc( full_mat_size * sizeof(FLOAT_TYPE) );
	   if (!temp_mat) return(-14);
	   ierr = NDI_READ_FLOAT_BLOCK( stream, full_mat->stat[vec_num].file_ptr,
					full_mat_size,
					temp_mat, full_mat_size );
	   
	   /*** check return value ***/
	   curr_mat = temp_mat;
	   full = -1;
	 } else {
	   /* File pointer not set -- data not available */
	   ierr = 9;
	 }
       } else {
	 /* Error: index out of range */
 	 ierr = ndi_check_array_err(full_mat->num_objs);
       }
     } else {
       /* Neither the pn diagonal nor the full pn matrix in library */
       ierr = 9;
     }
   
   }
   /* Collapse matrix */
   /* If the collapse flag is true, temp_mat will contain the full matrix at this
      point whether it was constructed from the diagonal or read from the file */
   if ( ( ierr >= 0 ) && ( collapse_flag ) ) {
     temp_vec = (FLOAT_TYPE *)ndi_malloc( grp_num_new * grp_num_new  * sizeof(FLOAT_TYPE) );
     ierr = NDI_COLLAPSE_MAT( temp_mat, num_grps, num_grps, grp_num_new, grp_num_new,
			      new_ebounds, new_ebounds, lib_weight, temp_vec,
			      grp_num_new * grp_num_new );
/*      ierr = NDI_COLLAPSE_MAT( temp_mat, full_mat_size, temp_vec ); */
     num_grps = grp_num_new;
     curr_mat = temp_vec;
     if ( temp_mat ) ndi_free( (void**)&temp_mat );
   }
   
   /* Turn matrix into diagonal */
   if ( ( ierr >= 0 ) && ( full != 0 ) ) {
     ierr = NDI_FULL2DIAG( curr_mat, num_grps, return_vec, return_size );
   }

   if ( temp_mat ) ndi_free ( (void**)&temp_mat );
   if ( temp_vec ) ndi_free ( (void**)&temp_vec );
   return(ierr);
   
}

/*********************************************************************/

static int GET_FULL_PN_MAT (FILE *stream, multi_stat full_mat, 
			    multi_stat down_mat, int num_grps, 
			    int vec_num, FLOAT_TYPE *return_mat, 
			    int return_size ) {

  /* There's no check on the matrix size here. */
	
   int ierr = 0;
   int tmp_err = 0;
   int temp_mat_size;
   int num_full_mat =  num_grps * num_grps;
   int num_down = (num_grps * (num_grps + 1) ) / 2;
   FLOAT_TYPE *temp_mat = NULL;
    

     NDI_INIT_FLOAT_VEC( return_mat, return_size );

   if ( num_grps <= 0 ) {
     ierr = 9;
     return(ierr);
   }

   if ( collapse_flag ) {
      temp_mat = (FLOAT_TYPE *)ndi_malloc( num_full_mat * sizeof(FLOAT_TYPE) );
      temp_mat_size = num_full_mat;
      if ( !temp_mat ) {
	 /* Out of memory */
	 return(-14);
      }
   } else {
      temp_mat = return_mat;
      temp_mat_size = return_size;
   }
    
   if ( full_mat.num_objs > 0 ) { 
      /*  Full matrix is in library  */
      if ( full_mat.num_objs > vec_num) { 
	 if ( full_mat.stat[vec_num].file_ptr ) {
	    ierr = NDI_READ_FLOAT_BLOCK(stream, full_mat.stat[vec_num].file_ptr, 
					num_full_mat,
					temp_mat, temp_mat_size );	
	 } else {
	    /* No file pointer set */
	    ierr = 9;
	 }
      } else {
	 /* Error: index out of range */
 	 ierr = ndi_check_array_err(full_mat.num_objs);
      }
   } else {
      /* Full matrix is constructed from diagonal */
      /* When upscatter is added to library, should change this to include it */
      if ( down_mat.num_objs > 0 ) {
	 if ( down_mat.num_objs > vec_num ) {
	    if ( down_mat.stat[vec_num].file_ptr ) {
	       FLOAT_TYPE *diag_array;
	       diag_array = (FLOAT_TYPE *)ndi_malloc( num_down * sizeof(FLOAT_TYPE) );
	       if ( diag_array ) {
		  ierr = NDI_READ_FLOAT_BLOCK( stream, down_mat.stat[vec_num].file_ptr,
					       num_down,
					       diag_array, num_down );
		  ierr = NDI_DIAG2FULL( NULL, 0, diag_array, num_down, num_grps, 
					num_grps, temp_mat );
		  if ( diag_array ) ndi_free((void**)&diag_array);
		  if ( ierr >= 0 ) {
		    /* Warning: full scattering matrix is constructed from downscatter only */
		    ierr = 10;
		  }
	       } else {
		  /* Out of memory */
		  ierr = -14;
	       } 
	    } else {
	       /* No file pointer set */
	       ierr = 9;
	    }
	 } else {
	    /* Error: index out of bounds */
	   ierr = ndi_check_array_err(down_mat.num_objs);
	 }
      } else {
	 /* Error: data not available */
	 ierr = 9;
      }
   }
    
 
   /*   COLLAPSING REQUIRED   */
   if ( collapse_flag ) {
      if ( ierr >= 0 ) {
	 tmp_err = NDI_COLLAPSE_MAT( temp_mat, num_grps, num_grps, grp_num_new,
				     grp_num_new, new_ebounds, new_ebounds, lib_weight, 
				     return_mat, return_size );
/* 	 tmp_err = NDI_COLLAPSE_MAT( temp_mat, num_full_mat, return_mat ); */
	 num_grps = grp_num_new;
      }
      if (temp_mat) ndi_free((void**)&temp_mat);
      if ( tmp_err < 0 ) {
	ierr = tmp_err;
      }
   }
    
   return(ierr);
}

/*********************************************************************/

static int NDI_DIAG2FULL(FLOAT_TYPE *low_diag, int num_ld, FLOAT_TYPE *up_diag, 
			 int num_ud, int num_row, int num_col, 
			 FLOAT_TYPE *return_vec) {
   
   /* ASSUMPTION:  ud_diag matrix contains the diagonal!! 
      Should row == col always???                          */
   
   int ierr = 0;
   int row = 0;
   int col = 0;
   int ld_count = 0;
   int ud_count = 0;
   FLOAT_TYPE *ld_ptr = low_diag;
   FLOAT_TYPE *ud_ptr = up_diag;
   FLOAT_TYPE *matrix_ptr = return_vec;
   
   for ( row = 0; row < num_row; row++ ) {
      for ( col = 0; col < row; col++ ) {
	 if ( ld_count < num_ld ) {
	    *matrix_ptr = *ld_ptr;
	    matrix_ptr++;
	    ld_ptr++;
	    ld_count++;
	 } else {
	    *matrix_ptr = 0;
	    matrix_ptr++;
	 }
      }
      for ( col = row; col < num_col; col++ ) {
	 if ( ud_count < num_ud ) {
	    *matrix_ptr = *ud_ptr;
	    matrix_ptr++;
	    ud_ptr++;
	    ud_count++;
	 } else {
	    *matrix_ptr = 0;
	    matrix_ptr++;
	 }
      }
   }
   return(ierr);
}

/*********************************************************************/

static int NDI_COLLAPSE_PROD_MAT( FLOAT_TYPE *old_mat,
				  int old_in, int old_out,
				  int new_in,
				  FLOAT_TYPE *return_vec, int return_size ) 

     /*  NOTE:  Currently there is no way to specify collapsing
	 the secondary particle energy structure.  That'll have 
	 to wait for coupled functionality to be implemented.
	 So,   new_out = old_out

     */

{

   int i, k, l;
   int ierr = 0;      /* Error flag: 0=no error,pos=warning, neg=error */
   int *new_bound_in = new_ebounds;               /* New row boundaries */
   int new_out = old_out;
   int ret_vec_index;
   FLOAT_TYPE in_sum = 0;                         /* Summing rows */
   FLOAT_TYPE wt_sum = 0;                         /* Sum of weights */

   if ( grp_num_new <= 0 ) {
      /* Number of groups not set  */
      return(-21);
   }

   for (k = 0; k < new_in; k++) {
      for (l = 0; l < new_out; l++ ) {
	 in_sum = 0;
	 wt_sum = 0;
	 for ( i = new_bound_in[k]; i < new_bound_in[k + 1] ; i++ ) {
	    in_sum += old_mat[ ( i * old_out ) + l ] * lib_weight[i];
	    wt_sum += lib_weight[i];
	 }
	 if ( wt_sum > 0 ) {
	   ret_vec_index = (k * new_out) + l; 
	   if ( ret_vec_index >= return_size ) {
	     if ( ierr >= 0 ) ierr = 7;
	     return(ierr);
	   }
	   return_vec[ ret_vec_index ] = in_sum / wt_sum;
	 } else {
	    /* Error: unexpected library data--weight sum should not be 0 */
	    ierr = -12;
	 }
      }     /* for k */
    
   }       /* for l */
   return(ierr);
}


/*********************************************************************/

static int  NDI_FULL2DIAG( FLOAT_TYPE *full_mat, int num_grps,
			   FLOAT_TYPE *return_vec, int return_size )

{
   
   int ierr = 0;
   int row = 0;
   int col = 0;
   int num_row = num_grps;
   int num_col = num_grps;
   int ret_count = 0;
   FLOAT_TYPE *mat_ptr = full_mat;
   FLOAT_TYPE upscat_sum;

   if ( (full_mat) && (return_vec) ) {
      for ( row = 0; row < num_row; row++ ) {
	 upscat_sum = 0.0;
	 for ( col = 0; col < num_col; col++ ) {
	    if ( ( ( mat_ptr - full_mat ) < ( num_row * num_col ) ) 
		 && ( ret_count < return_size ) ) {
	       if ( col < row ) {
		  upscat_sum += *mat_ptr;
	       } else if ( col == row ) {
		  return_vec[ret_count] = *mat_ptr + upscat_sum;
		  ret_count++;
	       } else {
		  return_vec[ret_count] = *mat_ptr;
		  ret_count++;
	       }
	       mat_ptr++;
	    } else {
	       ierr = 7;
	       break; 
	    }
	 }
	 if ( ( ( mat_ptr - full_mat ) >= ( num_row * num_col ) ) 
	      || ( ret_count >= return_size ) ) {
	    break;
	 }
      }
   } else {
      if ( !full_mat ) {
	 /* Bad matrix -- internal NDI error */
	 ierr = -13;
      } else if ( !return_vec ) {
	 /* Bad vector input by user */
	 ierr = -25;
      }
   }
   return(ierr);
}

/*********************************************************************/

static int PN_FULL_TRANS_CORR( FLOAT_TYPE *return_vec, int return_size ) {

   int i;
   int num_grps = iso_stat.num_grps;
   int ierr = 0;
   FLOAT_TYPE *corr;


   if ( num_grps <= 0 ) {
     ierr = 9;
     return(ierr);
   }

   if (collapse_flag) num_grps = grp_num_new;
   corr = (FLOAT_TYPE *)ndi_malloc( num_grps * sizeof(FLOAT_TYPE) );

   if ( trans_corr == 'd' ) {
      ierr = NDI_GET_FLOAT_VEC_N( NDI_CORR_DIAG, trans_corr_order, corr,
				  num_grps );
   } else if ( trans_corr == 'b' ) {
      ierr = NDI_GET_FLOAT_VEC_N( NDI_CORR_BHS, trans_corr_order, corr,
				  num_grps);
   } else {
      ierr = -13;
   }
   /*** Bounds checking? ***/
   if (ierr >= 0 ) {
      for (i = 0; i < num_grps; i++ ) {
	 return_vec[ (i * num_grps) + i ] -= corr[i];
      }
   }
   if (corr) ndi_free((void**)&corr);
   return(ierr);

}

/*********************************************************************/

static int PN_DOWN_FULL_TRANS_CORR( FLOAT_TYPE *return_vec, int return_size ) {

   int i;
   int num_grps = iso_stat.num_grps;
   int ierr = 0;
   FLOAT_TYPE *corr;
   int curr_index = 0;


   if ( num_grps <= 0 ) {
     ierr = 9;
     return(ierr);
   }

   if (collapse_flag) num_grps = grp_num_new;
   corr = (FLOAT_TYPE *)ndi_malloc( num_grps * sizeof(FLOAT_TYPE) );

   if ( trans_corr == 'd' ) {
      ierr = NDI_GET_FLOAT_VEC_N( NDI_CORR_DIAG, trans_corr_order, corr,
				  num_grps);
   } else if ( trans_corr == 'b' ) {
      ierr = NDI_GET_FLOAT_VEC_N( NDI_CORR_BHS, trans_corr_order, corr, 
				  num_grps);
   } else {
      ierr = -13;
   }


   /* Traverse the diagonal elements of the triangular 'matrix' using
      the recurrance relation:
      index(i) = index(i-1) + (num_grps - i - 1),
      where index(0) = 0
   */

   /*** Bounds checking? ***/
   if (ierr >= 0 ) {
      for (i = 0; i < num_grps; i++ ) {
	 return_vec[curr_index] -= corr[i];
	 curr_index += num_grps - i;
      }
   }
   if (corr) ndi_free((void**)&corr);
   return(ierr);

}


/*********************************************************************/

static int GET_PN_PROD_FULL( FILE *stream, sec_part_mg sec_part,
			     int vec_num,
			     FLOAT_TYPE *return_vec, int return_size )
{
   int ierr = 0;
   FLOAT_TYPE *temp_vec = NULL;
   int matrix_num = sec_part.num_grps * iso_stat.num_grps;


   if (sec_part.pn_prod_full.num_objs > 0 ) {
      if (sec_part.pn_prod_full.num_objs > vec_num) {
	 if (sec_part.pn_prod_full.stat[vec_num].file_ptr) {

	    if ( collapse_flag ) {
	       temp_vec = (FLOAT_TYPE *)ndi_malloc( matrix_num * sizeof(FLOAT_TYPE) );
	       if ( temp_vec ) {
		  ierr = NDI_READ_FLOAT_BLOCK(stream,
					      sec_part.pn_prod_full.stat[vec_num].file_ptr,
					      matrix_num,
					      temp_vec, matrix_num );
		  if ( ierr >= 0 ) {
		     ierr = NDI_COLLAPSE_PROD_MAT( temp_vec, 
						   iso_stat.num_grps, sec_part.num_grps,
						   grp_num_new,
						   return_vec, return_size );
		  }
		  if ( temp_vec ) ndi_free( (void**)&temp_vec );
	       } else {
		  /* Out of memory */
		  ierr = -14;
	       }
	    } else {
	       ierr = NDI_READ_FLOAT_BLOCK(stream,
					   sec_part.pn_prod_full.stat[vec_num].file_ptr,
					   matrix_num,
					   return_vec, return_size );
	    }
	 } else {
	    /* Error reading file pointer */
	    ierr = -19;
	 }
      } else {
 	 ierr = ndi_check_array_err(sec_part.pn_prod_full.num_objs);
      }
   } else {
      /* Error: no data for this isotope */
      ierr = 9;
   }
   return(ierr);
}

/*********************************************************************/

static int GET_DIAG_CORR( FLOAT_TYPE *return_vec, int vec_num )

{
   char transpose_state;
   char corr_state;
   int ierr = 0;
   int i;
   int num = 0;
   int curr_index = 0;
   int array_size;
   FLOAT_TYPE *temp_vec = NULL;
   
	
   if (collapse_flag) {
      num = grp_num_new;
   } else {
      num = iso_stat.num_grps;
   }
   array_size = num * num;
   ierr = NDI_INIT_FLOAT_VEC( return_vec, num );
   temp_vec = (FLOAT_TYPE *)ndi_malloc( array_size * sizeof(FLOAT_TYPE) );
   if (!temp_vec) {
      /* Out of memory */
      return(-14);
   }

   /* Don't transpose matrix, but save user-defined transpose & trans. cor. state */
   transpose_state = mat_order;
   mat_order = 'd';
   corr_state = trans_corr;
   trans_corr = ' ';

   ierr = NDI_GET_FLOAT_VEC_N( NDI_PN_DOWN_FULL, vec_num + 1, temp_vec, array_size);
   mat_order = transpose_state;
   trans_corr = corr_state;
	 
   /* Traverse the diagonal elements of the triangular 'matrix' using
      the recurrance relation:
      index(i) = index(i-1) + (num_grps - i - 1),
      where index(0) = 0
   */
   for ( i = 0; i < num; i++ ) {
      *(return_vec + i) = *(temp_vec + curr_index);
      curr_index += num - i;
   }
   
   if (temp_vec) ndi_free((void**)&temp_vec);

   return(ierr);
}

/*********************************************************************/

static int GET_BHS_CORR( FLOAT_TYPE *return_vec, int vec_num )

{
   char transpose_state = ' ';
   char corr_state = ' ';
   int ierr = 0;
   int i, j;
   int num = 0;
   int array_size;
     /*  ndi_c_get_size_n( NDI_PN_FULL, vec_num, &ierr ); */
   FLOAT_TYPE *temp_vec = NULL;
   FLOAT_TYPE scat_sum = 0;


   if (collapse_flag) {
      num = grp_num_new;
   } else {
      num = iso_stat.num_grps;
   }

   /*
   **** array_size must be wrong ****
   for now just use array_size = num**2
    */
   array_size = num * num;

   ierr = NDI_INIT_FLOAT_VEC( return_vec, num );
   temp_vec = (FLOAT_TYPE *)ndi_malloc( array_size * sizeof(FLOAT_TYPE) );
   if (!temp_vec) {
      /* Out of memory */
      return(-14);
   }

   /*  Don't transpose matrix, but save user-defined transpose & trans. cor. state */
   transpose_state = mat_order;
   mat_order = 'd';
   corr_state = trans_corr;
   trans_corr = ' ';

   ierr = NDI_GET_FLOAT_VEC_N( NDI_PN_FULL, vec_num + 1, temp_vec, array_size );	 
   mat_order = transpose_state;
   trans_corr = corr_state;
   
   
   for ( i = 0; i < num; i++ ) {
      scat_sum = 0;
      for ( j = 0; j < num; j++ ) {
	 scat_sum += *(temp_vec + (i * num) + j );
      }
      *(return_vec + i) = scat_sum;
   }
   
   if (temp_vec) ndi_free((void**)&temp_vec);
   
   return(ierr);
}

/*********************************************************************/

static int GET_TRANSFER_MAT( FLOAT_TYPE *return_vec, int vec_num, 
			     int n_type )
{
   int i;
   int j;
   int ierr = 0;
   int array_size = 0;
   int nusigf_type = 0;
   int chi_type = 0;
   char transpose_state;
   FLOAT_TYPE *pn_ptr = return_vec;
   FLOAT_TYPE *nusigf = NULL;
   FLOAT_TYPE *chi_mat = NULL;
   FLOAT_TYPE *scat_mat = NULL;
   int num_grps;
   
   /*** Check for non-fissionable isotopes ***/

   switch ( n_type ) {
   
   case NDI_TRANSFER_MAT_PR:
      nusigf_type = NDI_NU_SIG_F_PR;
      chi_type = NDI_CHI_MAT_PR;
      break;
   case NDI_TRANSFER_MAT_TOT:
      nusigf_type = NDI_NU_SIG_F_PR;
      chi_type = NDI_CHI_MAT_PR;
      break;
   default:
      ierr = -13;
      break;
   }
      
   if ( ierr < 0 ) return ( ierr );

   /*  Don't transpose matrix, but save user-defined transpose & trans. cor. state */
   transpose_state = mat_order;
   mat_order = 'd';

   if (vec_num == 0 ) {
      /* Corrections are only applied to 0th order matrix */
      array_size = ndi_c_get_size( chi_type, &ierr );
      chi_mat = (FLOAT_TYPE *)ndi_malloc ( array_size * sizeof(FLOAT_TYPE) );
      /* Check for error */
      ierr = NDI_GET_FLOAT_VEC( NDI_CHI_MAT_TOT, chi_mat, array_size );

      array_size = ndi_c_get_size( nusigf_type, &ierr );
      nusigf = (FLOAT_TYPE *)ndi_malloc ( array_size * sizeof(FLOAT_TYPE) );
      /* Check for error */
      ierr = NDI_GET_FLOAT_VEC( NDI_NU_SIG_F_TOT, nusigf, array_size );

      array_size = ndi_c_get_size_n( NDI_PN_FULL, vec_num, &ierr );
      scat_mat = (FLOAT_TYPE *)ndi_malloc( array_size * sizeof(FLOAT_TYPE) );
      /* Check for error */
      pn_ptr = scat_mat;
   }

   array_size = ndi_c_get_size_n( NDI_PN_FULL, vec_num, &ierr );
   ierr = NDI_GET_FLOAT_VEC_N( NDI_PN_FULL, vec_num, pn_ptr, array_size );	
   mat_order = transpose_state;

   if (collapse_flag) {
      num_grps = grp_num_new;
   } else {
      num_grps = iso_stat.num_grps;
   }

   /*  Fill return_array */
   if ( vec_num == 0 ) {
      for (i = 0; i < num_grps; i++) {
	 for (j = 0; j < num_grps; j++ ) {
	    *(return_vec + ((i * num_grps) + j) ) = *(scat_mat + ((i * num_grps) + j) ) 
	       + *(nusigf + j) * *(chi_mat +  ((i * num_grps) + j));
	 }
      }
      if ( nusigf ) ndi_free( (void**)&nusigf );
      if ( chi_mat ) ndi_free( (void**)&chi_mat );
      if ( scat_mat ) ndi_free( (void**)&scat_mat );
   }
   return(ierr);
}

/*********************************************************************/
/*********************************************************************/
