
/*
 * File:	ndi_init_iso_state.c
 * Description:	Internal function--initialize isotope state.
 * Time-stamp:	<2000-02-03 14:50:33 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static int read_mg_data(FILE *stream, const char *zaid ); 
static int read_tn_data(FILE *stream, const char *zaid );
static int read_tn_reac_data(FILE *stream, const char *zaid );
static int read_depl_data(FILE *stream, const char *zaid );
static int set_simple_array (FILE *stream, stat_flag *sf, int data_num);
static int read_simple_array (FILE *stream, stat_flag *sf, int data_num,
			      ndi_float64 **global_p );
static int read_next_string (FILE *stream, char *str_array, int str_size );
static int set_string_array (FILE *stream, char **str_p, int data_num);
static int set_sec_part_types (FILE *stream, int_zaid_type **int_p, int data_num);
static int set_reaction_prod (FILE *stream, multi_stat *sf, int data_num);
static int set_pn_array (FILE *stream, multi_stat *sf, int pn_order,
			 int in_grps, int out_grps, 
			 int num_elements );
static int set_sig_reac (FILE *stream, multi_stat *sf, int num_reac,
			 int in_grps, int **in_e_mt, 
			 double **in_q_value, int *fission_flag );
static int ndi_setup_edist( char *keyword, FILE *stream, sec_part_distr *sec_part,
			    int num_parts, int *parts_filled );
static int fill_edist( FILE *stream, int ntemp, edist_data *dist );
static int ndi_read_interp_param( FILE *stream, int ninterp, 
				  interp_params *param );
static int ndi_create_tn_sec_part ( int num_parts, sec_part_distr **sec_part);
static int set_reaction_array (FILE *stream, char **str_p, stat_flag **reac_p,
			       stat_flag **rprods_p, int data_num);
static int set_depl_array (FILE *stream, depl_target_struct *target_p,
			       int data_num);
static int ndi_create_mg_sec_part( int num_parts, sec_part_mg **sec_part );
static int get_mg_part_type( const char *in_keyword, const char *prefix, 
			     mg_sec_part_type *part_type );
static int find_mg_sec_part_index( mg_sec_part_type part_type );
static int set_chain_array (FILE *stream, depl_target_struct *target,
			       int data_num);
static int skip_comment( FILE *stream );

/**********************************************************************/

int ndi_init_iso_state(FILE *stream, char lib_type, 
		       long int iso_start, const char *zaid) 

/*  Purpose:  Scan isotope data in library file and set initial values 
    of state variables.

    Parameters:
    stream -- file pointer to open library file.
    lib_type -- in:m=multigrp, s=TN data, r=TN reac, 0=depl, d=dosimetry
    iso_start -- line number of start of data for current isotope.
    zaid -- identifier for current isotope.

    Assumptions:  In reading arrays, it is assumed that the array
    number variable (num_grps, etc.) has been initialized to -1.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/

{
   char in_keyword[MAX_IN_STR_LEN];
   char izaid[MAX_IN_STR_LEN];
   int ierr = 0;
   int n;
  

   /*******************************************************/
   /* Find start of isotope data and check that it is the
      start of the correct data.                          */
   /*******************************************************/

   if ( !stream ) {
     /* Error opening file */
     return( -1 );
   }

   /* Use file pointers instead of line numbers */
   fseek( stream, iso_start, SEEK_SET );
   /*    (void)rewind(stream); 
	 if (iso_start > 1) {
	   if ( ( ierr = skiplines(stream, (iso_start - 1)) ) != 0 ) {
	     if ( ierr < 0 ) {
	        return(ierr);
	     }
           } 
         }
   */

   /* Read zaid */
   /*** ????  Can there be a blank space between keyword and zaid????
	????  Must the data start in column one  */
   n = fscanf(stream, "%s\n%s\n", in_keyword, izaid);
   if ( ( n == EOF ) || ( n < 2 ) ) {
      /* zaid wasn't read */
      return(-11);
   }
   if (strncmp(in_keyword, "zaid", 5) != 0) {
      /* zaid isn't where it should be */
      return(-12);
   }
   if (strncmp( izaid, zaid, (strlen(zaid)*sizeof(char))) != 0) {
      /* Not the correct zaid */
      return(-17);
   }

   if ( ( lib_type == 'm' ) ||
	( lib_type == 'd' ) ) {
      ierr = read_mg_data( stream, zaid );
   } else if ( lib_type == 's' ) {
      ierr = read_tn_data( stream, zaid );
   }else if ( lib_type == 'r' ) {                    /* TN reaction data */
      ierr = read_tn_reac_data( stream, zaid );
   }else if ( lib_type == '0' ) {                    /* Depletion data */
      ierr = read_depl_data( stream, zaid );
   } else {
      /* Internal error--invalid lib_type input */
      ierr = -13;
   }


   return(ierr);

}


/*************************************************************************/
/*************************************************************************/

static int read_mg_data(FILE *stream, const char *zaid ) {

   char in_keyword[MAX_IN_STR_LEN];
   int ierr = 0;
   int num_elements;
   mg_sec_part_type part_type;
   int part_index;


   /***** ??? Should this be done at the end ?? ***/
   strcpy(iso_stat.zaid, zaid); 
   iso_stat.set_flag = 1;
   
   /*******************************************************/
   /*                  Read isotope data                  */
   /*******************************************************/
   while (1) {
      
      /* Read next keyword */
      if (fscanf(stream, "%s\n", in_keyword) == EOF) {
	 /* Error reading file */
	 return(-11);
      }
      
      
      /******            Process keywords            ******/
      
      /*** Single-valued data  ***/
      
      /* Information */
      if (strncmp (in_keyword, "info", 5) == 0) {
	 if (fgets(iso_stat.info, sizeof(iso_stat.info), stream) == NULL) {
	    /* Error reading file */
	    return(-11);
	 }	 
      }
      /* Data type */
      else if ( strncmp( in_keyword, "data_type", 10 ) == 0 ) {
	char junk[MAX_IN_STR_LEN];
	 if (fscanf( stream, "%s\n", junk ) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* date_processed */
      else if ( strncmp( in_keyword, "date_processed", 15 ) == 0 ) {
	ierr = read_next_string( stream, iso_stat.date_processed, 
				 sizeof(iso_stat.date_processed));
      }
      /* date_source */
      else if ( strncmp( in_keyword, "date_source", 12 ) == 0 ) {
	ierr = read_next_string( stream, iso_stat.date_source,
				 sizeof(iso_stat.date_source));
      }
      /* Library name*/
      else if ( strncmp( in_keyword, "library_name", 13 ) == 0 ) {
	ierr = read_next_string( stream, iso_stat.library_name,
				 sizeof(iso_stat.library_name) );
      }
      /* Number of groups */
      else if ( strncmp( in_keyword, "num_grps", 9 ) == 0 ) {
	 if (fscanf( stream, "%d\n", &iso_stat.num_grps ) == EOF) {
	    /* ???? Could this return 0???? */
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* Last neutron group */
      else if ( strncmp( in_keyword, "last_n_grp", 11 ) == 0 ) {
	 if (fscanf( stream, "%d\n", &iso_stat.last_grp ) == EOF) {
	    /* ???? Could this return 0???? */
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* Temperature */
      else if (strncmp (in_keyword, "temp", 5 ) == 0) {
	 if ( fscanf(stream, "%le\n", &iso_stat.temp) == EOF ) {
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* sig_0 */
      else if ( strncmp( in_keyword, "sig_0", 6 ) == 0 ) {
	 if (fscanf(stream, "%lf\n", &iso_stat.sig_0) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      } 
      /* upscatter_groups */
      else if ( strncmp( in_keyword, "up_grps", 8 ) == 0 ) {
	 if (fscanf(stream, "%d\n", &iso_stat.up_grps) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* downscatter_groups */
      else if ( strncmp( in_keyword, "down_grps", 10 ) == 0 ) {
	 if ( fscanf(stream, "%d\n", &iso_stat.down_grps) == EOF ) {
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* atomic weight */
      else if ( strncmp(in_keyword, "at_wgt", 7) == 0 ) {
	 if (fscanf(stream, "%lf\n", &iso_stat.at_wgt) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* atomic weight ratio */
      else if ( strncmp(in_keyword, "awr", 4) == 0 ) {
	 if (fscanf(stream, "%lf\n", &iso_stat.awr) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* number of reactions which have cross-section data */
      else if ( strncmp(in_keyword, "num_reac", 9) == 0 ) {
	 if (fscanf(stream, "%d\n", &iso_stat.num_reac) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* number of scattering matrices */
      else if ( strncmp(in_keyword, "pn_order", 9) == 0 ) {
	 if (fscanf(stream, "%d\n", &iso_stat.pn_order) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* number of secondary particles */

      /*
      else if ( strncmp(in_keyword, "num_sec_parts", 14) == 0 ) {
	 if ( fscanf(stream, "%d\n", &iso_stat.num_sec_parts ) == EOF ) {
	    * Error reading file *
	    return(-11);
	 }    
      }
      */
      
      /*** Simple array data  ***/

      /* energy boundaries */
      else if ( strncmp(in_keyword, "e_bounds", 9) == 0 ) {
	 ierr = read_simple_array(stream, &iso_stat.e_bounds, 
				  (iso_stat.num_grps + 1), &lib_ebounds);
      }
      /* wgts */
      else if (strncmp(in_keyword, "wgts", 5 ) == 0) {
	 ierr = read_simple_array(stream, &iso_stat.wgts, 
				  iso_stat.num_grps, &lib_weight);
      }
      /* vel */
      else if (strncmp(in_keyword, "vel", 4 ) == 0) {
	 ierr = set_simple_array (stream, &iso_stat.vel, iso_stat.num_grps);
      }
      /* heating */
      else if (strncmp(in_keyword, "heating", 8 ) == 0) {
	 ierr = set_simple_array (stream, &iso_stat.heat, iso_stat.num_grps);
      }
      /* kerma */
      else if (strncmp(in_keyword, "kerma", 6 ) == 0) {
	 ierr = set_simple_array (stream, &iso_stat.kerma, iso_stat.num_grps);
      }
      /* sig_tot */
      else if (strncmp(in_keyword, "sig_tot", 8 ) == 0) {
	 ierr = set_simple_array (stream, &iso_stat.sig_tot, iso_stat.num_grps);
      }
      /* nubar total */
      else if  (strncmp(in_keyword, "nu_tot", 7 ) == 0) {
	 ierr = set_simple_array (stream, &iso_stat.nu_tot, 
				  iso_stat.num_grps);
	 iso_stat.fissionable = 1;
      }
      /* nubar prompt */
      else if  (strncmp(in_keyword, "nu_pr", 6 ) == 0) {
	 ierr = set_simple_array (stream, &iso_stat.nu_pr, 
				  iso_stat.num_grps);
	 iso_stat.fissionable = 1;
      }
      /* nubar delayed */
      else if  (strncmp(in_keyword, "nu_del", 7 ) == 0) {
	 ierr = set_simple_array (stream, &iso_stat.nu_del, 
				  iso_stat.num_grps);
	 iso_stat.fissionable = 1;
      }
      /* nubar total sigma fission */
      else if  (strncmp(in_keyword, "nu_sig_f_tot", 13 ) == 0) {
	 ierr = set_simple_array (stream, &iso_stat.nu_sig_f_tot, 
				  iso_stat.num_grps);
	 iso_stat.fissionable = 1;
      }
      /* nubar prompt sigma fission */
      else if  (strncmp(in_keyword, "nu_sig_f_pr", 12 ) == 0) {
	 ierr = set_simple_array (stream, &iso_stat.nu_sig_f_pr, 
				  iso_stat.num_grps);
	 iso_stat.fissionable = 1;
      }
      /* nubar delayed sigma fission */
      else if  (strncmp(in_keyword, "nu_sig_f_del", 13 ) == 0) {
	 ierr = set_simple_array (stream, &iso_stat.nu_sig_f_del, 
				  iso_stat.num_grps);
	 iso_stat.fissionable = 1;
      }
      /* chi total vector */
      else if ( strncmp(in_keyword, "chi_vec_tot", 12) == 0 ) {
	 ierr = set_simple_array (stream, &iso_stat.chi_vec_tot, 
				  iso_stat.num_grps);
	 iso_stat.fissionable = 1;
      }
      /* chi prompt vector */
      else if  (strncmp(in_keyword, "chi_vec_pr", 11 ) == 0) {
	 ierr = set_simple_array (stream, &iso_stat.chi_vec_pr, 
				  iso_stat.num_grps);
	 iso_stat.fissionable = 1;
      }
      /* chi delayed vector */
      else if  (strncmp(in_keyword, "chi_vec_del", 12 ) == 0) {
	 ierr = set_simple_array (stream, &iso_stat.chi_vec_del, 
				  iso_stat.num_grps);
	 iso_stat.fissionable = 1;
      }
      /* chi total matrix */
      else if ( strncmp(in_keyword, "chi_mat_tot", 12) == 0 ) {
	 num_elements = iso_stat.num_grps * iso_stat.num_grps;
	 ierr = set_simple_array (stream, &iso_stat.chi_mat_tot, 
				  num_elements);
	 iso_stat.fissionable = 1;
      }
      /* chi prompt matrix */
      else if  (strncmp(in_keyword, "chi_mat_pr", 11 ) == 0) {
	 num_elements = iso_stat.num_grps * iso_stat.num_grps;
	 ierr = set_simple_array (stream, &iso_stat.chi_mat_pr, 
				  num_elements);
	 iso_stat.fissionable = 1;
      }
      /* chi delayed matrix */
      else if  (strncmp(in_keyword, "chi_mat_del", 12 ) == 0) {
	 num_elements = iso_stat.num_grps * iso_stat.num_grps;
	 ierr = set_simple_array (stream, &iso_stat.chi_mat_del, 
				  num_elements);
	 iso_stat.fissionable = 1;
      }
      /* fission q */
      else if ( strncmp(in_keyword, "fiss_q", 7 ) == 0 ) {
	 ierr = set_simple_array (stream, &iso_stat.fiss_q, 
				  NUM_FISSION_Q);
	 iso_stat.fissionable = 1;
      }
      /* Normalization factors */
      else if ( strncmp(in_keyword, "norm", 5 ) == 0 ) {
	 ierr = set_simple_array ( stream, &iso_stat.norm, 
				   NUM_NORM );
      }      


      /* Multi-dim array data */

      /* pn scattering -- full lower diagonal */
      else if (strncmp(in_keyword, "pn_down_full", 13 ) == 0) {
	 iso_stat.pn_down_full.num_objs = iso_stat.pn_order;	 
	 /*** Check that num_grps == down_grps + 1 ???  ***/
	 num_elements = ( ( iso_stat.num_grps ) * 
			  ( iso_stat.num_grps + 1 ) ) / 2;
	 ierr = set_pn_array( stream, &iso_stat.pn_down_full, 
			      iso_stat.pn_down_full.num_objs,
			      iso_stat.num_grps, iso_stat.num_grps, 
			      num_elements );
      }
 
      /* pn scattering -- full matrix */
      else if (strncmp(in_keyword, "pn_full", 8 ) == 0) {
	 iso_stat.pn_full.num_objs = iso_stat.pn_order;	 
	 num_elements =  iso_stat.num_grps * iso_stat.num_grps;
	 ierr = set_pn_array( stream, &iso_stat.pn_full, 
			      iso_stat.pn_full.num_objs,
			      iso_stat.num_grps, iso_stat.num_grps, 
			      num_elements );
/*	 printf("setting pnfull ierr=%d\n", ierr); */
      }
   
      /* sigma edit cross section */
      else if (strncmp(in_keyword, "sig_reac", 9) == 0 ) {
	 ierr = set_sig_reac (stream, &iso_stat.sig_reac, iso_stat.num_reac,
			      iso_stat.num_grps, &iso_stat.reac_mt, 
			      &iso_stat.reac_q, &iso_stat.fissionable );
      }

      /* sigma edit reaction products -- all */
      else if (strncmp(in_keyword, "rprod_all", 24 ) == 0) {
	 ierr = set_reaction_prod (stream, &iso_stat.rprod_all, 
				   iso_stat.num_reac);
      }

      /* sigma edit reaction products -- rmo */
      else if (strncmp(in_keyword, "rprod_rmo", 24 ) == 0) {
	 ierr = set_reaction_prod (stream, &iso_stat.rprod_rmo, 
				   iso_stat.num_reac);
      }

      /* sigma edit reaction products -- few */
      else if (strncmp(in_keyword, "rprod_few", 24 ) == 0) {
	 ierr = set_reaction_prod (stream, &iso_stat.rprod_few, 
				   iso_stat.num_reac);
      }

      /*********  SECONDARY PARTICLE DATA  *************/

      /* number of secondary particles */
      else if ( strncmp(in_keyword, "num_sec_parts", 14) == 0 ) {
	 if ( fscanf(stream, "%d\n", &iso_stat.num_sec_parts ) == EOF ) {
	    /* Error reading file */
	    return(-11);
	 } else {
	   ierr = ndi_create_mg_sec_part( iso_stat.num_sec_parts, &iso_stat.sec_part );
	 }
      }


      /* secondary particle types */
      else if (strncmp(in_keyword, "sec_part_types", 15 ) == 0) {
	char **temp_part_types = NULL;
	int min_len;
	int i;

	temp_part_types = (char **)ndi_calloc( iso_stat.num_sec_parts, 
					       sizeof(char *) );
	ierr = set_string_array (stream, temp_part_types, iso_stat.num_sec_parts);
	for ( i = 0; i < iso_stat.num_sec_parts; i++ ) {
	  /*  Check this out ! */
	  if ( (strlen(iso_stat.sec_part[i].part_type) * sizeof(mg_sec_part_type) )
		< sizeof(temp_part_types[i]) ) {
	    min_len = sizeof(iso_stat.sec_part[i].part_type);
	  } else {
	    min_len = sizeof(temp_part_types[i]);
	  }
	  strncpy(iso_stat.sec_part[i].part_type, temp_part_types[i], min_len);
	  if ( temp_part_types[i] ) {
	    ndi_free( (void**)&(temp_part_types[i]) );
	  }
	}
	if ( temp_part_types ) {
	  ndi_free( (void**)&temp_part_types);
	}
      }

      /* secondary particle zaids */
      else if (strncmp(in_keyword, "sec_part_zaids", 15 ) == 0) {
	char **temp_part_zaids = NULL;
	int min_len;
	int i;

	temp_part_zaids = (char **)ndi_calloc( iso_stat.num_sec_parts, 
					       sizeof(mg_sec_part_type *) );
	ierr = set_string_array (stream, temp_part_zaids, iso_stat.num_sec_parts );

	for ( i = 0; i < iso_stat.num_sec_parts; i++ ) {
	  /* CHECK THAT ZAID MATCHES PART TYPE */

	  if ( (strlen(iso_stat.sec_part[i].zaid) * sizeof(mg_zaid) )
	       < (strlen(temp_part_zaids[i]) + 1) ) {
	    min_len = (int) strlen(iso_stat.sec_part[i].zaid);
	  } else {
	    min_len = (int) strlen(temp_part_zaids[i]) + 1;
	  }
	  strncpy(iso_stat.sec_part[i].zaid, temp_part_zaids[i], min_len);
	  if ( temp_part_zaids[i] ) {
	    ndi_free( (void**)&(temp_part_zaids[i]) );
	  }
	}
	if ( temp_part_zaids ) {
	  ndi_free( (void**)&temp_part_zaids);
	}
      }

      /* number of groups -- secondary particle */
      else if (strncmp(in_keyword, "num_grps_", 9 ) == 0) {
	int curr_int;

	if (fscanf(stream, "%d\n", &curr_int) == EOF) {
	  /* Error reading file */
	  return(-11);
	} else {
	  ierr = get_mg_part_type( in_keyword, "num_grps_", &part_type );
	  part_index = find_mg_sec_part_index(part_type);
	  if ( ( part_index >= 0 ) && ( part_index < iso_stat.num_sec_parts ) ) {
	    iso_stat.sec_part[part_index].num_grps = curr_int;
	  } else {
	    /* Error -- bad part type? */
	    ierr = -15;
	  }
	}
      }

      /* number of production matrices -- secondary particle */
      else if ( strncmp(in_keyword, "pn_order_", 9 ) == 0 ) {
	int curr_int;

	 if (fscanf(stream, "%d\n", &curr_int) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }  else {
	   ierr = get_mg_part_type( in_keyword, "pn_order_", &part_type );
	   part_index = find_mg_sec_part_index(part_type);
	   if ( ( part_index >= 0 ) && ( part_index < iso_stat.num_sec_parts ) ) {
	     iso_stat.sec_part[part_index].pn_order = curr_int;
	   } else {
	     /* Error -- bad part type? */
	    ierr = -15;
	   }
	 }
      }
      
       /* partial heating -- secondary particle */
      else if (strncmp(in_keyword, "heating_", 8 ) == 0) {

	ierr = get_mg_part_type( in_keyword, "heating_", &part_type );
	part_index = find_mg_sec_part_index(part_type);
	if ( ( part_index >= 0 ) && ( part_index < iso_stat.num_sec_parts ) ) {
	  ierr = set_simple_array (stream, &iso_stat.sec_part[part_index].heating, 
				   iso_stat.num_grps);
	} else {
	  /* Error -- bad particle? */
	  ierr = -15;
	}
      }

      /* partial kerma -- secondary particle */
      else if (strncmp(in_keyword, "kerma_", 6 ) == 0) {

	ierr = get_mg_part_type( in_keyword, "kerma_", &part_type );
	part_index = find_mg_sec_part_index(part_type);
	if ( ( part_index >= 0 ) && ( part_index < iso_stat.num_sec_parts ) ) {
	  ierr = set_simple_array (stream, &iso_stat.sec_part[part_index].kerma, 
				   iso_stat.num_grps);
	} else {
	  /* Error -- bad particle? */
	  ierr = -15;
	}
      }

      /* energy boundaries -- secondary particle */
      else if ( strncmp(in_keyword, "e_bounds_", 9) == 0 ) {

	ierr = get_mg_part_type( in_keyword, "e_bounds_", &part_type );
	part_index = find_mg_sec_part_index(part_type);
	if ( ( part_index >= 0 ) && ( part_index < iso_stat.num_sec_parts ) ) {
	  ierr = set_simple_array (stream, &iso_stat.sec_part[part_index].e_bounds, 
				   ( iso_stat.sec_part[part_index].num_grps + 1 ) );
	} else {
	  /* Error -- bad particle? */
	  ierr = -999;
	}
      }
 
      /* full pn production matrix -- secondary particle */
      else if (strncmp(in_keyword, "pn_prod_full_", 13 ) == 0) {

	ierr = get_mg_part_type( in_keyword, "pn_prod_full_", &part_type );
	part_index = find_mg_sec_part_index(part_type);
	if ( ( part_index >= 0 ) && ( part_index < iso_stat.num_sec_parts ) ) {
	  iso_stat.sec_part[part_index].pn_prod_full.num_objs = iso_stat.sec_part[part_index].pn_order;
	  num_elements =  iso_stat.sec_part[part_index].num_grps * iso_stat.num_grps;
	  ierr = set_pn_array( stream, &iso_stat.sec_part[part_index].pn_prod_full, 
			       iso_stat.sec_part[part_index].pn_prod_full.num_objs,
			       iso_stat.num_grps, iso_stat.sec_part[part_index].num_grps, 
			       num_elements );

	  assert( iso_stat.sec_part[part_index].pn_prod_full.num_objs <= 0 ||
		  iso_stat.sec_part[part_index].pn_prod_full.stat != NULL );

	} else {
	  /* Error -- bad particle? */
	  ierr = -999;
	}
      }

      /*********  COMMENTS  *************/

      /* comment */
      else if (strncmp(in_keyword, "comment", 8) == 0) {
	ierr = skip_comment( stream );
	return(ierr);
      } 

      /*********  END  *************/

      /* end */
      else if (strncmp(in_keyword, "end", 4) == 0) {
	 return(ierr);
      } 

      /**************  UNKNOWN KEYWORD  *****************/
    
      else {
	 /* Unknown keyword in library */
	 ierr = 15;
      }
   }
}

/*************************************************************************/

static int read_tn_data(FILE *stream, const char *zaid ) {

   char in_keyword[MAX_IN_STR_LEN];
   int ierr = 0;

   /***** ??? Should this be done at the end ?? ***/
   strcpy(tn_stat.zaid, zaid); 
   tn_stat.set_flag = 1;
   
   /*******************************************************/
   /*                     Read TN data                    */
   /*******************************************************/
   while (1) {
      
      /* Read next keyword */
      if (fscanf(stream, "%s\n", in_keyword) == EOF) {
	 /* Error reading file */
	 return(-11);
      }
      
      
      /******************PROCESS KEYWORDS*********************/
      
      /*** Single-valued data  ***/
      
      /* Information */
      if (strncmp (in_keyword, "info", 5) == 0) {
	 if (fgets(tn_stat.info, sizeof(tn_stat.info), stream) == NULL) {
	    /* Error reading file */
	    return(-11);
	 }	 
      }
      /* date_processed */
      else if ( strncmp( in_keyword, "date_processed", 15 ) == 0 ) {
	ierr = read_next_string( stream, tn_stat.date_processed,
				 sizeof(tn_stat.date_processed) );
      }
      /* date_source */
      else if ( strncmp( in_keyword, "date_source", 12 ) == 0 ) {
	ierr = read_next_string( stream, tn_stat.date_source,
				 sizeof(tn_stat.date_source));
      }
      /* Library name*/
      else if ( strncmp( in_keyword, "library_name", 13 ) == 0 ) {
	ierr = read_next_string( stream, tn_stat.library_name,
				 sizeof(tn_stat.library_name) );
      }
      /* Number of temps */
      else if ( strncmp( in_keyword, "num_temps", 9 ) == 0 ) {
	 if (fscanf( stream, "%d\n", &tn_stat.num_temps ) == EOF) {
	    /* ???? Could this return 0???? */
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* Number of secondary particles for which there is energy distribution info */
      else if ( strncmp( in_keyword, "num_sec_parts", 14 ) == 0 ) {
	 if (fscanf( stream, "%d\n", &tn_stat.num_sec_parts ) == EOF) {
	    /* ???? Could this return 0???? */
	    /* Error reading file */
	    return(-11);
	 } else {
	 }
      }
      /* Q-value */
      else if ( strncmp( in_keyword, "reac_q", 7 ) == 0 ) {
	 if (fscanf( stream, "%lf\n", &tn_stat.q_value ) == EOF) {
	    /* ???? Could this return 0???? */
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* Number of interpolation regions */
/*       else if ( strncmp( in_keyword, "num_int_sigvbars", 17 ) == 0 ) { */
/* 	 if (fscanf( stream, "%d\n", &tn_stat.num_int_sigvbars ) == EOF) { */
      /* ???? Could this return 0???? */ 
      /* Error reading file */ 
/* 	    return(-11); */
/* 	 }     */
/*       } */

      /*** Simple array data  ***/
      
      /* temperatures */
      else if ( strncmp(in_keyword, "temps", 6) == 0 ) {
	 ierr = set_simple_array(stream, &tn_stat.temps, tn_stat.num_temps);
      }
      /* sigma vbar */
      else if (strncmp(in_keyword, "sigvbars", 9 ) == 0) {
	 ierr = set_simple_array (stream, &tn_stat.sigvbars, tn_stat.num_temps);
      }
      /* sigma vbar interpolation data */
      else if (strncmp(in_keyword, "int_sigvbars", 13 ) == 0) {
	 ierr = ndi_skip_rx_prod (stream, &tn_stat.int_sigvbars, NO_MT );
      }
      /* einbar interpolation data */
      else if (strncmp(in_keyword, "int_einbars", 12 ) == 0) {
	 ierr = ndi_skip_rx_prod (stream, &tn_stat.int_einbars, NO_MT );
      }
      /* average incident energy */
      else if ( strncmp(in_keyword, "einbars", 8 ) == 0 ) {
	 ierr = set_simple_array(stream, &tn_stat.einbars, tn_stat.num_temps);
      }
      /* reaction products */
      else if  (strncmp(in_keyword, "rprods", 7 ) == 0) {
	 ierr = ndi_skip_rx_prod( stream, &tn_stat.rprods, NO_MT );
      }
      /* reactants */
      else if  (strncmp(in_keyword, "reactants", 10 ) == 0) {
	 ierr = ndi_skip_rx_prod( stream, &tn_stat.reactants, NO_MT );
      }

      /* secondary particle types */
      else if (strncmp(in_keyword, "sec_part_types", 15 ) == 0) {
	 /* Check for num_sec_parts > 0 */
	if ( tn_stat.num_sec_parts > 0 ) {
	  tn_stat.sec_part_types = (int_zaid_type*)ndi_malloc( tn_stat.num_sec_parts * 
							       sizeof(int_zaid_type) );
	  ierr = set_sec_part_types( stream, &(tn_stat.sec_part_types),
				     tn_stat.num_sec_parts );
	} else {
	  ierr =-18 ;
	}
      }

      /* Compound data */
      else if  (strncmp(in_keyword, "e_dist_", 7 ) == 0) {
	/* Allocate sec_part array -- energy distr info */
	if (!  tn_stat.sec_part ) {
	  ierr = ndi_create_tn_sec_part( tn_stat.num_sec_parts, &tn_stat.sec_part );
	}
	ierr = ndi_setup_edist( in_keyword, stream, tn_stat.sec_part, tn_stat.num_sec_parts,
				&sec_part_filled );
      }


      /*********  COMMENTS  *************/

      /* comment */
      else if (strncmp(in_keyword, "comment", 8) == 0) {
	ierr = skip_comment( stream );
	return(ierr);
      } 

      /*********  END  *************/
      
      /* end */
      else if (strncmp(in_keyword, "end", 4) == 0) {
	 return(ierr);
      } 

      /**************  UNKNOWN KEYWORD  *****************/
    
      else {
	 /* Unknown keyword in library */
	 ierr = 15;
      }
   }
}

/**********************************************************************/
/*************************************************************************/

static int read_tn_reac_data(FILE *stream, const char *zaid ) {

   char in_keyword[MAX_IN_STR_LEN];
   int ierr = 0;


   /* Initialize data--set all to zero or null-string */

   /***** ??? Should this be done at the end ?? ***/
   strcpy( tnreac_stat.dataset, zaid ); 
   
   /*******************************************************/
   /*                     Read TN data                    */
   /*******************************************************/
   while (1) {
      
      /* Read next keyword */
      if (fscanf(stream, "%s\n", in_keyword) == EOF) {
	 /* Error reading file */
	 return(-11);
      }
      
      
      /******************PROCESS KEYWORDS*********************/
      
      /*** Single-valued data  ***/
      
      /* Information */
      if (strncmp (in_keyword, "info", 5) == 0) {
	 if (fgets(tnreac_stat.info, sizeof(tnreac_stat.info), stream) == NULL) {
	    /* Error reading file */
	    return(-11);
	 }	 
      }
      /* date_processed */
      else if ( strncmp( in_keyword, "date_processed", 15 ) == 0 ) {
	ierr = read_next_string( stream, tnreac_stat.date_processed,
				 sizeof(tnreac_stat.date_processed) );
      }
      /* date_source */
      else if ( strncmp( in_keyword, "date_source", 12 ) == 0 ) {
	ierr = read_next_string( stream, tnreac_stat.date_source,
				 sizeof(tnreac_stat.date_source) );
      }
      /* Library name*/
      else if ( strncmp( in_keyword, "library_name", 13 ) == 0 ) {
	ierr = read_next_string( stream, tnreac_stat.library_name,
				 sizeof(tnreac_stat.library_name) );
      }
      /* Number of reactions */
      else if ( strncmp( in_keyword, "num_reactions", 14 ) == 0 ) {
	 if (fscanf( stream, "%d\n", &tnreac_stat.reac_num ) == EOF) {
	    /* ???? Could this return 0???? */
	    /* Error reading file */
	    return(-11);
	 }    
      }

      /*** Simple array data  ***/
      
      /* List of reactions */
      else if ( strncmp(in_keyword, "list_reactions", 15 ) == 0 ) {
	 ierr = set_reaction_array ( stream, tnreac_stat.list_reac, &tnreac_stat.reactants, 
				     &tnreac_stat.rprods, tnreac_stat.reac_num );
      }

      /*********  COMMENTS  *************/

      /* comment */
      else if (strncmp(in_keyword, "comment", 8) == 0) {
	ierr = skip_comment( stream );
	return(ierr);
      } 

      /*********  END  *************/
      
      /* end */
      else if (strncmp(in_keyword, "end", 4) == 0) {
	 return(ierr);
      } 

      /**************  UNKNOWN KEYWORD  *****************/
    
      else {
	 /* Unknown keyword in library */
	 ierr = 15;
      }
   }
}

/**********************************************************************/

static int read_depl_data(FILE *stream, const char *zaid ) {

   char in_keyword[MAX_IN_STR_LEN];
   int ierr = 0;


   /* Initialize data--set all to zero or null-string */

   /***** ??? Should this be done at the end ?? ***/
   strcpy( depl_dataset, zaid ); 
   
   /*******************************************************/
   /*              Read depletion data                    */
   /*******************************************************/
   while (1) {
      
      /* Read next keyword */
      if (fscanf(stream, "%s\n", in_keyword) == EOF) {
	 /* Error reading file */
	 return(-11);
      }
      
      
      /******************PROCESS KEYWORDS*********************/
      
      /*** Single-valued data  ***/
      
      /* Information */
      if (strncmp (in_keyword, "info", 5) == 0) {
	 if (fgets(depl_info, sizeof(depl_info), stream) == NULL) {
	    /* Error reading file */
	    return(-11);
	 }	 
      }
      /* date_processed */
      else if ( strncmp( in_keyword, "date_processed", 15 ) == 0 ) {
	char junk[MAX_IN_STR_LEN];
	 if (fscanf( stream, "%s\n", junk ) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* date_source */
      else if ( strncmp( in_keyword, "date_source", 12 ) == 0 ) {
	char junk[MAX_IN_STR_LEN];
	 if (fscanf( stream, "%s\n", junk ) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      }
      /* Library name*/
      else if ( strncmp( in_keyword, "library_name", 13 ) == 0 ) {
	char junk[MAX_IN_STR_LEN];
	 if (fscanf( stream, "%s\n", junk ) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      }

      /* Unique targets*/
      else if ( strncmp( in_keyword, "num_unique_targets", 19 ) == 0 ) {
	char junk[MAX_IN_STR_LEN];
	 if (fscanf( stream, "%s\n", junk ) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      }

      /* Number of incident particles*/
      else if ( strncmp( in_keyword, "num_inc_parts", 14 ) == 0 ) {
	char junk[MAX_IN_STR_LEN];
	 if (fscanf( stream, "%s\n", junk ) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      }

      /* Incident particles*/
      else if ( strncmp( in_keyword, "list_inc_parts", 14 ) == 0 ) {
	char junk[MAX_IN_STR_LEN];
	 if (fscanf( stream, "%s\n", junk ) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      }

      /* Current incident particle*/
      else if ( strncmp( in_keyword, "inc_part", 9 ) == 0 ) {
	char junk[MAX_IN_STR_LEN];
	 if (fscanf( stream, "%s\n", junk ) == EOF) {
	    /* Error reading file */
	    return(-11);
	 }    
      }

      /* Number of depletion targets */
      else if ( strncmp( in_keyword, "num_targets", 12 ) == 0 ) {

	if (fscanf( stream, "%d\n", &num_target ) == EOF) { 
	  /* ???? Could this return 0???? */
	  /* Error reading file */
	  return(-11);
	}  else {
	  depl_target = ( depl_target_struct *)ndi_malloc( num_target * sizeof(depl_target_struct) );
	}
      }
      
      /*** Simple array data  ***/
      
      /* List of reactions */
      else if ( strncmp(in_keyword, "list_targets", 13 ) == 0 ) {
	ierr = set_depl_array ( stream, depl_target, num_target );
      }

      else if ( strncmp(in_keyword, "target", 7 ) == 0 ) {

	/* Read target zaid */
/* 	if ( chain_stat.target_filled < num_target ) { */
	ierr = set_chain_array ( stream, depl_target, num_target );
	  /*	  chain_stat.target_filled++;  
	}
	*/  

      }

      /*********  COMMENTS  *************/

      /* comment */
      else if (strncmp(in_keyword, "comment", 8) == 0) {
	ierr = skip_comment( stream );
	return(ierr);
      } 

      /*********  END  *************/
      
      /* end */
      else if (strncmp(in_keyword, "end", 4) == 0) {
	 return(ierr);
      } 

      /**************  UNKNOWN KEYWORD  *****************/
    
      else {
	 /* Unknown keyword in library */
	 ierr = 15;
      }
   }
}


/**********************************************************************/
/*************************************************************************/

static int set_simple_array (FILE *stream, stat_flag *sf, int data_num) {

   int block_size = 0;
   int ierr = 0;
   
   sf->file_ptr = 0;
   if ( data_num < 0 ) {
      /* Library error:  incorrect data order. */
      ierr = -18;
   } else if ( data_num == 0 ) {
      /* No energy groups ! */
      ierr = 5;
   } else {
      sf->file_ptr = ftell(stream);
      ierr = skip_float_block(stream, &block_size, data_num);
      sf->length = block_size;
   }
   return(ierr);
}

/**********************************************************************/

static int read_simple_array (FILE *stream, stat_flag *sf, 
			      int data_num, ndi_float64 **global_p ) {


   int ierr = 0;
   
   /* Initialize */
   sf->file_ptr = 0;
   if (*global_p) ndi_free((void**)global_p);
   
   /* Error checks and read data */
   if ( data_num < 0 ) {
      /* Library error:  incorrect data order. */
      ierr = -18;
   } else if ( data_num == 0 ) {
      /* No energy groups ! */
      ierr = 5;
   } else {
      sf->file_ptr = ftell(stream);
      *global_p = (ndi_float64 *)ndi_malloc( data_num * sizeof(ndi_float64) );
      if (*global_p) {
	 ierr = ndi_read_float64_block(stream, sf->file_ptr, 
				       data_num,
				       *global_p, data_num );
	 sf->length =  data_num;
      } else {
	 /* Out of memory */
	 ierr = -14;
      }
   }
   return(ierr);
}

/**********************************************************************/
static int read_next_string (FILE *stream, char *str_array, int str_size )

     /* Reads into str_array the next non-blank line.  Str_array
	is assumed to be a character array, rather than dynamically
	allocated.????
     */

{
   int ierr = 0;

   *str_array = '\0';

   while ( ndi_empty_string( str_array ) &&
	   ! feof( stream ) ) {
     if (fgets(str_array, str_size-1, stream) == NULL) {
       /* Error reading file */
       ierr = -11;
       break;
     }	
   }

   return(ierr);
}

/**********************************************************************/
static int set_string_array (FILE *stream, char **str_p, int data_num) 

{
   int ierr = 0;
   
   if ( data_num < 0 ) {
      /* Library error:  incorrect data order. */
      ierr = -18;
   } else if ( data_num == 0 ) {
      /* No data! */
      ierr = 5;
   } else {
      ierr = ndi_read_string_block( stream, data_num, str_p );
   }
   return(ierr);
}

/**********************************************************************/
/**********************************************************************/
static int set_sec_part_types (FILE *stream, int_zaid_type **int_p, int data_num) 

{
   int ierr = 0;
   
   if ( data_num < 0 ) {
      /* Library error:  incorrect data order. */
      ierr = -18;
   } else if ( data_num == 0 ) {
      /* No data! */
      ierr = 5;
   } else {
     *int_p = (int_zaid_type *)ndi_malloc( data_num * sizeof(int_zaid_type) );
     ierr = ndi_read_int_block( stream, data_num, *int_p, data_num );
   }
   return(ierr);
}

/**********************************************************************/
/**********************************************************************/
static int set_reaction_array (FILE *stream, char **str_p, stat_flag **reac_p,
			       stat_flag **rprods_p, int data_num) 

{
   int ierr = 0;
   int reac_count = 0;
   char in_line[MAX_IN_STR_LEN];        /* Current input line */
   char *token;                  /* Current data item */
   char seps[] = " ,\t\n";       /* Data delimiters in library file */
   stat_flag *reactants;
   stat_flag *rprods;

   
   if ( data_num < 0 ) {
      /* Library error:  incorrect data order. */
      ierr = -18;
   } else if ( data_num == 0 ) {
      /* No data! */
      ierr = 5;
   } else {
      reactants = (stat_flag *)ndi_malloc( data_num * sizeof(stat_flag) );
      rprods = (stat_flag *)ndi_malloc( data_num * sizeof(stat_flag) );
      /* Read next line and count strings */
      while ( reac_count < data_num ) {
	 if (fgets(in_line, sizeof(in_line), stream) != NULL) {
	    token  = strtok( in_line, seps );
	    if ( ( strlen(token) == 0 ) ||
		 ( strncmp( token, "reaction", strlen(token) ) == 0 ) ) {
	      continue;
	    }
	    if (reac_count < data_num) {
	      
	       str_p[reac_count] = (char *)ndi_malloc( (strlen(token) + 1) *
						       sizeof(char) );
	       strcpy( str_p[reac_count], token );
	       ierr = ndi_skip_rx_prod( stream, &(reactants[reac_count]), NO_MT);
	       ierr = ndi_skip_rx_prod( stream, &(rprods[reac_count]), NO_MT);
	       reac_count++;
	    } else {
	       /* More data than expected */
	       ierr = 6;
	       break;
	    }
	 } else {
	    /* Error reading file */
	    ierr = -11;
	    break;
	 }
      }
      *reac_p = reactants;
      *rprods_p = rprods;

   }
   return(ierr);
}

/**********************************************************************/
static int set_reaction_prod (FILE *stream, multi_stat *sf, int data_num)
			      
{
   int n;
   int ierr = 0;
   
   if ( data_num < 0 ) {
      /* Library error:  incorrect data order. */
      ierr = -18;
   } else if ( data_num == 0 ) {
      /* No reactions ! */
      sf->num_objs = data_num;
      ierr = 5;
   } else {
      sf->num_objs = data_num;
      sf->stat = (stat_flag *)ndi_malloc(data_num * sizeof(stat_flag));
      if ( sf->stat ) {
	 for (n = 0; n < data_num; ++n) {	
	    ierr = ndi_skip_rx_prod(stream, &(sf->stat[n]), READ_MT );
	 }
      } else {
	 /* Out of memory */
	 ierr = -14;
      }
   }
   return(ierr);
}


/**********************************************************************/
static int set_depl_array (FILE *stream, depl_target_struct *target,
			   int data_num) 

{
   int ierr = 0;
   int target_count = chain_filled;
   char in_line[MAX_IN_STR_LEN];        /* Current input line */
   char *token;                  /* Current data item */
   char seps[] = " ,\t\n";       /* Data delimiters in library file */
   int i;
   depl_target_struct *target_p;
   long int tmp_fptr;
   
   if ( data_num < 0 ) {
     /* Library error:  incorrect data order. */
     ierr = -18;
   } else if ( data_num == 0 ) {
     /* No data! */
     ierr = 5;
   } else {
     /* Read next line and count strings */
     
     while ( target_count < data_num ) { 
       
       target_p = &(target[target_count]);
       /* Read target zaid */
       if ( fscanf( stream, "%s\n", in_line ) == EOF ) {
	 /* Error reading file */
	 ierr = -11;    
	 break;
       } else {
	 token  = strtok( in_line, seps );
	 target_p->izaid = (int) strtol( token, (char **)NULL, 10 );
       }

       /* Read number of products for the current target */
       if ( fscanf( stream,  "%s\n", in_line ) == EOF ) {
	 /* Error reading file */
	 ierr = -11;
	 break;
       } else {
	 token  = strtok( in_line, seps );
	 target_p->num_rprods = (int) strtol(token, (char **)NULL, 10);
	 target_p->rprod = ( depl_rprod_struct *)ndi_malloc( target_p->num_rprods 
							     * sizeof(depl_rprod_struct) ); 
       }
       for ( i = 0; i < target_p->num_rprods; i++ ) {
	 /* Read product zaid */
	 if ( fscanf( stream,  "%s\n", in_line ) == EOF ) {
	   /* Error reading file */
	   ierr = -11;
	   break;
	 } else {
	   token  = strtok( in_line, seps );
	   target_p->rprod[i].izaid = (int)strtol( token, (char **)NULL, 10 );
	   tmp_fptr = ftell(stream);
	   
	   /* Read reaction product mt */
	   target_p->rprod[i].num_reac = -1;
	   ierr = ndi_read_rx_prod( stream, tmp_fptr, &(target_p->rprod[i].mt),
				    ID_PARAM, &(target_p->rprod[i].num_reac), NO_MT );
	   
	   /* Read reaction product multiplicities */
	   target_p->rprod[i].mult = (int *)ndi_malloc( target_p->rprod[i].num_reac 
							* sizeof(int) );
	   ierr = ndi_read_rx_prod( stream, tmp_fptr, &(target_p->rprod[i].mult),
				    MULT_PARAM, &(target_p->rprod[i].num_reac), NO_MT );
	 }
       }
       
       target_count++;
     }
     
   }

   chain_filled = target_count;
   return(ierr);
}

/**********************************************************************/
/**********************************************************************/
static int set_chain_array (FILE *stream, depl_target_struct *target,
			       int data_num) 

{
   int ierr = 0;
   int target_count = chain_filled;
   char in_line[MAX_IN_STR_LEN];        /* Current input line */
   char *token;                  /* Current data item */
   char seps[] = " ,\t\n";       /* Data delimiters in library file */
   char junk[20];
   int i;
   depl_target_struct *target_p;
   long int tmp_fptr;
   

   if ( data_num < 0 ) {
     /* Library error:  incorrect data order. */
     ierr = -18;
   } else if ( data_num == 0 ) {
     /* No data! */
     ierr = 5;
   } else {  /* Read info for current target */
     /* Read next line and count strings */
     
       
       target_p = &(target[target_count]);
       /* Read target zaid */
       if ( fscanf( stream, "%s\n", in_line ) == EOF ) {
	 /* Error reading file */
	 ierr = -11;   
	 return(ierr);
       } else {
	 token  = strtok( in_line, seps );
	 target_p->izaid = (int) strtol( token, (char **)NULL, 10 );
       }


       fscanf( stream, "%s\n", junk );
       if (strncmp( junk, "num_products", 12) == 0 ) {
	 /* Read number of products for the current target */
	 if ( fscanf( stream,  "%s\n", in_line ) == EOF ) {
	   /* Error reading file */
	   ierr = -11;   
	   return(ierr);
	 } else {
	   token  = strtok( in_line, seps );
	   target_p->num_rprods = (int) strtol(token, (char **)NULL, 10);
	   target_p->rprod = ( depl_rprod_struct *)ndi_malloc( target_p->num_rprods 
							       * sizeof(depl_rprod_struct) ); 
	 }
	 
	 for ( i = 0; i < target_p->num_rprods; i++ ) {
	   fscanf( stream, "%s\n", junk );
	   if (strncmp( junk, "product", 7) == 0 ) {
	     /* Read product zaid */
	     if ( fscanf( stream,  "%s\n", in_line ) == EOF ) {
	       /* Error reading file */
	       ierr = -11;    
	       return (ierr);
	     } else {
	       token  = strtok( in_line, seps );
	       target_p->rprod[i].izaid = (int) strtol( token, (char **)NULL, 10 );
	       tmp_fptr = ftell(stream);
	   
	       /* Read reaction product mt */


	   
	       target_p->rprod[i].num_reac = -1;
	       ierr = ndi_read_rx_prod( stream, tmp_fptr, &(target_p->rprod[i].mt),
					ID_PARAM, &(target_p->rprod[i].num_reac), NO_MT );
	   
	       /* Read reaction product multiplicities */
	       target_p->rprod[i].mult = (int *)ndi_malloc( target_p->rprod[i].num_reac 
							    * sizeof(int) );
	       ierr = ndi_read_rx_prod( stream, tmp_fptr, &(target_p->rprod[i].mult),
					MULT_PARAM, &(target_p->rprod[i].num_reac), NO_MT );

	       /*  COMMENTED OUT
	       fscanf( stream, "%d\n",  &(target_p->rprod[i].num_reac));
	       target_p->rprod[i].mt = (int *)ndi_malloc( target_p->rprod[i].num_reac * sizeof(int) );
	       target_p->rprod[i].mult = (int *)ndi_malloc( target_p->rprod[i].num_reac * sizeof(int) );
	       curr_mt = target_p->rprod[i].mt;
	       curr_mult = target_p->rprod[i].mult;
	       for ( i = 0; i < target_p->rprod[i].num_reac; i++ ) {
		 fscanf( stream, "%d", curr_mt);
		 curr_mt++;
		 
		 fscanf( stream, "%d", curr_mult);
		 curr_mult++;
	       }
	       */

	     }

	   } else {
	     /* product keyword not found */
	     ierr = -15;
	   }
	     
       	 }
   
	 chain_filled++;

       } else {
	 /* num_products keyword not found */
	 ierr = -15;
       }
    
   }
   return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int set_pn_array (FILE *stream, multi_stat *sf, int pn_order,
			 int in_grps, int out_grps, int num_elements )
   
/* It is assumed that 'num_objs' was set when the pn number was read */
/* The number of incoming and outgoing energy groups must be set */
   
{
   int n;
   int ierr = 0;
   int pn_num;
   int block_size;
   

   /*** get rid of this stupidity--set num_objs here ***/
   if (sf->num_objs < 0 ) {
      /* Library error:  incorrect data order. */
      ierr = -18;
   } else {
      if ( ( in_grps < 0 ) || ( out_grps < 0 ) ) {
	 /* Library error:  incorrect data order. */
	 ierr = -18;
      } else if ( ( in_grps == 0 ) || ( out_grps == 0 ) ) {
	 /* No energy groups! */
	 ierr = 5;	  
      } else {
	 sf->stat = (stat_flag *)ndi_malloc( pn_order * sizeof(stat_flag) );
	 if ( sf->stat ) {
	    for (n = 0; n < pn_order; ++n) {
	       if ( fscanf(stream, "%d\n", &pn_num) == EOF ) {
		  /* Error reading file--pn order not read */
		  return (-11);
	       }
	       if (n != pn_num) {
		  /* Unexpected data--pn out of order */
		  return (-12);
	       }
	       sf->stat[n].file_ptr = ftell(stream);
	       ierr = skip_float_block(stream, &block_size, num_elements);
	       sf->stat[n].length = block_size;
	    }
	 } else {
	    /* Out of memory */
	    ierr = -14;
	 }
      }
   }
   return(ierr);
}

/**********************************************************************/

static int set_sig_reac (FILE *stream, multi_stat *sf, int num_reac,
			 int in_grps, int **in_e_mt, 
			 double **in_q_value, int *fission_flag )
	
/* It is assumed that 'num_objs' has not been set */
/* The number of  energy groups must be set */
      
{
   int n;
   int ierr = 0;
   int *e_mt;
   int block_size;
   double *q_value;
   
   sf->num_objs = num_reac;
   if (sf->num_objs < 0 ) {
      /* Library error:  incorrect data order. */
      ierr = -18;
   } else {
      if ( in_grps < 0 )  {
	 /* Library error:  incorrect data order. */
	 ierr = -18;
      } else if ( in_grps == 0 ) {
	 /* No energy groups! */
	 ierr = 5;	  
      } else {
	 sf->stat = (stat_flag *)ndi_malloc( num_reac * sizeof(stat_flag) );
	 *in_e_mt = (int *)ndi_malloc( num_reac * sizeof(int) ); 
	 *in_q_value = (double *)ndi_malloc( num_reac * 
						  sizeof(double) );
	 if ( ( sf->stat ) && ( *in_e_mt ) && ( *in_q_value ) ) {
	    e_mt = *in_e_mt;
	    q_value = *in_q_value;
	    for ( n = 0; n < num_reac; ++n ) {
	       if (fscanf(stream, "%d  %lf\n", e_mt, q_value) == EOF) {
		  /* Error reading file--MT and Q-value not read */
		  if ( e_mt ) ndi_free( (void**)&e_mt );
		  if ( q_value ) ndi_free( (void**)&q_value );
		  if ( sf->stat ) ndi_free( (void**)&sf->stat );
		  return (-11);
	       }

	       /* Check for fission reaction */
	       /* ASSUMPTION: Checking only for MT=18 (total f) and MT=19 (direct f),
		  since it is assumed that MT=19 will be present if any other partial 
		  fission reactions are present (MT=20,21,38)
	       */
	       if ( (*e_mt == 18) || (*e_mt == 19) ) {
		   *fission_flag = 1;
	       }

	       /*** check e_mt[n] == curr_e_mt  and store q_value */
	       e_mt++;
	       q_value++;
	       sf->stat[n].file_ptr = ftell(stream);
	       ierr = skip_float_block( stream, &block_size, in_grps );
	       sf->stat[n].length = block_size;
	    }
	 } else {
	    /* Out of memory */
	    ierr = -14;
	 }
      }
   }
   return(ierr);
}

/**********************************************************************/
static int ndi_setup_edist( char *keyword, FILE *stream, sec_part_distr *sec_part,
			    int num_parts, int *parts_filled ) 

{
   int ierr = 0;
   int tmp_err = 0;
   int ntemp, ninterp;
   int num_particles_read = *parts_filled;
   char *particle_str = &(keyword[7]); /* The particle id starts at char 7 */
   sec_part_distr *part;
   	
   /* Find next part to fill */
   if ( num_particles_read < num_parts ) {
     part = &sec_part[num_particles_read];
     num_particles_read++;
   } else {
     /* Error */
     return(-7);
   }

   /* Fill particle type */
   part->part_type = strtol(particle_str, (char **)NULL, 10 );

   /* Fill temperature interpolation parameters */
   if (fscanf(stream, "%d  %d\n", &ntemp, &ninterp) != EOF) {
     if ( part ) {
       part->temp_interp.npts = ntemp;
       part->temp_interp.nint = ninterp;
       if ( ninterp > 0 ) {
	 ierr = ndi_read_interp_param( stream, ninterp, &(part->temp_interp) );
       }
     } else {
        if ( ninterp > 0 ) {
	 ierr = ndi_read_interp_param( stream, ninterp, NULL );
       }
     }      
     
     /* Fill in energy distribution data for each temperature */
     if ( ntemp > 0 )  {
       if ( part ) {
	 part->edist = (edist_data *)ndi_malloc( ntemp * sizeof(edist_data) );
	 if ( !( part->edist ) ) {
	   ierr = -14;
	 } else {
	   tmp_err = fill_edist( stream, ntemp, part->edist );
	 }
       } /*else {
	 tmp_err = fill_edist( stream, ntemp, NULL );
	 } */
     } else {
     }
       
   } else  {
     /* Error reading file */
     ierr = -11;
   }
   
   if ( ( ( tmp_err < 0 ) && ( ierr >= 0 ) ) ||
	( ( tmp_err > 0 ) && ( ierr == 0 ) ) ) {
     ierr = tmp_err;
   }
   
   *parts_filled = num_particles_read;
   return (ierr);
}

/**********************************************************************/
static int fill_edist( FILE *stream, int ntemp, edist_data *dist ) 
{
  int ierr = 0;
  int i;
  int npts, ninterp;
  int block_size;
  double temp;

  for ( i = 0; i < ntemp; i++ ) {
    if (fscanf(stream, "%lf", &temp ) != EOF) {
      if ( dist ) {
	dist[i].temp = temp;
      }
      if (fscanf(stream, "%d  %d\n", &npts, &ninterp ) != EOF) {
	dist[i].interp.npts = npts;
	dist[i].interp.nint = ninterp;
	ierr = ndi_read_interp_param( stream, ninterp, &(dist[i].interp) );
	dist[i].file_ptr = ftell(stream);
	ierr = skip_float_block( stream, &block_size, npts * 2 );
      } else {
	/* Error reading file */
	ierr = -11;
	break;
      }
    } else  {
      /* Error reading file */
      ierr = -11;
      break;
    }
  }
   
   return(ierr);
   
}

/**********************************************************************/
static int ndi_read_interp_param( FILE *stream, int ninterp, interp_params *param ) 

   /* Malloc's the space for nbt's and int_types.
   */

{    
   int ierr = 0;                 /* Error flag: 0=no error,pos=warning, neg=error */
   int param_count = 0;          /* Current number of inter parameter pairs processed */
   char in_line[MAX_IN_STR_LEN];        /* Current input line */
   char *token;                  /* Current data item */
   char seps[] = " ,\t\n";       /* Data delimiters in library file */
   int *curr_nbt;
   int *curr_int_type;


   if ( param ) {
     param->nbt = (int *)ndi_malloc( ninterp * sizeof(int) );
     param->int_type = (int *)ndi_malloc( ninterp * sizeof(int) );
     if ( !(param->nbt) || !(param->int_type) ) {
       /* Out of memory */
       return(-14);
     }
     curr_nbt = param->nbt;
     curr_int_type = param->int_type;
   } else {
     curr_nbt = NULL;
     curr_int_type = NULL;
   }

   while ( param_count < ninterp ) {
      if (fgets(in_line, sizeof(in_line), stream) != NULL) {
	 token  = strtok( in_line, seps );
	 while (token != NULL) {
	   if ( param_count < ninterp ) {
	     if ( curr_nbt ) {
	       *curr_nbt = (int)atoi( token );
	       curr_nbt++;
	     }
	     token = strtok( NULL, seps );
	     if ( curr_int_type ) {
	       *curr_int_type = (int)atoi( token );
	       curr_int_type++;
	     }
	     param_count++;
	     token = strtok( NULL, seps );
	   } else {
	     /* More data than expected */
	     ierr = 6;
	     break;
	   }
	 }
      } else {
	 /* Error reading file */
	 ierr = -11;
	 break;
/* 	 return(ierr); */
      }
   }

   return(ierr);

}


/**********************************************************************/
static int ndi_create_tn_sec_part ( int num_parts, sec_part_distr **sec_part)
{

  int i;
  int ierr = 0;
  sec_part_distr *part;


/*   part = (sec_part_distr *)calloc(  num_parts, sizeof( sec_part_distr ) ); */

  part = (sec_part_distr *)ndi_malloc( num_parts * sizeof( sec_part_distr ) );
  for (i = 0; i < num_parts; i++ ) {
    ndi_set0_tn_sec_part( &(part[i]), 0 ); 
  }
  
  *sec_part = part;
  return(ierr);
}


/**********************************************************************/
static int ndi_create_mg_sec_part( int num_parts, sec_part_mg **sec_part )
{

  int i;
  int ierr = 0;
  sec_part_mg *part;

  part = (sec_part_mg *)ndi_calloc( num_parts, sizeof( sec_part_mg ) );
  for (i = 0; i < num_parts; i++ ) {
    ierr = ndi_set0_mg_sec_part( &(part[i]) );
  }
  
  *sec_part = part;
  return(ierr);
}

/**********************************************************************/
static int get_mg_part_type( const char *in_keyword, const char *prefix, 
			     mg_sec_part_type *part_type)
     /* 
	Strip prefix from the front of in_keyword and return part_type.
	Returns error value 
      */

{
  int ierr = 0;
  char *part_type_start = NULL;

  strncpy( *part_type, "", 1);

  if ( strstr( in_keyword, prefix ) ) {
    part_type_start = (char *)&(in_keyword[strlen(prefix)]);
    strcpy( *part_type, part_type_start );
/*     printf( ">>>>>>PART TYPE: %s\n", part_type ); */

    /*Check that part_type is an integer */
    /*
       part_num = atoi(*part_type);
       sprintf( *part_type, "%d", part_num);
       */


  } else {
    /* Internal NDI error */
    ierr = -13;
  }
  return(ierr);
}
/**********************************************************************/
static int find_mg_sec_part_index( mg_sec_part_type part_type ) 
     /* Returns the index within iso_stat.sec_parts of the particle of type
	"part_type".
	*/
{
  int i;
  int return_val = -1;

  for ( i = 0; i < iso_stat.num_sec_parts; i++ ) {
/*     if ( strncmp(iso_stat.sec_part[i].part_type, part_type, (strlen(part_type) + 1) ) == 0 ) {  */
    if ( strncmp(iso_stat.sec_part[i].part_type, part_type, (strlen(part_type) ) ) == 0 ) { 
      return_val = i;
      break;
    }
  }
  return(return_val);
}
/**********************************************************************/
static int skip_comment( FILE *stream )

{
  char seps[] = " ,\t\n"; 
  int ierr = 0;
  char comment[MAX_IN_STR_LEN];

  ierr = -11;  /* Default error -- error reading file */

  while ( fgets(comment, sizeof(comment), stream) != NULL ) {

    if ( strncmp (strtok( comment, seps ), "end_comment", 12) == 0) {
      /* Check that there is nothing following?? */
      ierr = 0;  /* No error */
      break;
    }
  }

  return( ierr );
}

/**********************************************************************/
