
/*
 * File:	ndi_set_option.c
 * Description:	User function--set defaults.
 * Time-stamp:	<2000-02-03 14:56:24 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static int ndi_set_lib_type( const char *value );
static int ndi_set_library( const char *value );
static int ndi_set_mat_order( const char *value );
static int ndi_set_collapse( const char *value );
static int ndi_set_norm ( const char *value );
static int ndi_set_trans_corr_diag( const char *value );
static int ndi_set_reac_id( const char *value );

static int get_order( const char *value );
static int ndi_niceStrCp( char *new_str, const char *old_str, int new_len );

/**********************************************************************/

int ndi_c_set_option ( int in_key, const char *in_value ) 

/*  Purpose: Set options/defaults.

    Parameters:
    in_key -- in: key to specify option to set.
    in_value -- in: string specifying the value of the option.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok.

***********************************************************************/

{
   int ierr = 0;       /* Error flag: 0=no error, pos=warning, neg=error */
   int pindex= 0;      /* Secondary particle index */
   char *value;        /* Copy of input value of option */
   int in_value_len = 0;   /* Size of the in_value string */
   
   /* Check that ndi_init has been called. */
   if (!ndi_initialized) {
      return(-2); }

   in_value_len = (int) strlen(in_value) + 1;
   value = (char *)ndi_malloc( in_value_len * sizeof(char));
   ndi_niceStrCp( value, in_value, in_value_len );
   
   /*** Check that NDI is initialized ***/
   
   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {
      
   case NDI_LIB_TYPE:                    
      /*  Set library type  */
      ierr = ndi_set_lib_type( value );
      break;

   case NDI_LIBRARY:                    
      /* Set source library */
      ierr = ndi_set_library( value );
      break;

   case NDI_MAT_ORDER:     
      /* Set flag to transpose order of returned matrix */
      ierr = ndi_set_mat_order( value );
      break;
      
   case NDI_COLLAPSE:       
      /* Set no. of energy groups to collapse to */
      ierr = ndi_set_collapse( value );
      break;

   case NDI_TRANS_CORR:
      /* Use transport corrections */
      ierr = ndi_set_trans_corr_diag( value );
      break;

   case NDI_NORM:
      /* Specify whether to normalize chis and edist -- default is normalize */
      ierr = ndi_set_norm( value );
      break;

   case NDI_REAC_ID:
      /* Specify reaction identifiers--MT or C values */
      ierr = ndi_set_reac_id( value );
      break;

   case NDI_DEPLETION:
      /* Set depletion chain -- for multigroup only */
      ierr = ndi_depletion( value );
      break;

   case NDI_CHAIN:  /* Same as NDI_RADCHEM  */
      /* Set radchem chain -- for dosimetry only */
      ierr = ndi_radchem( value );
      break;

   case NDI_CURR_PART:
      /* Specify which particle is 'current'  */
      if ( library_type == 's' ) {
	 pindex = ndi_find_tn_part_index( value, tn_stat.sec_part, 
				       sec_part_filled );
      } else if ( library_type == 'm' ) {
	pindex = ndi_find_mg_part_index( value, iso_stat.sec_part, iso_stat.num_sec_parts );
	
      } else {
	 /* Operation is valid only for tn data */
	 ierr = -16;
      }

      curr_part_index = pindex;
      if ( ( ierr <0 ) || ( pindex < 0 ) ) {
	/* Invalid particle was input -- not found */
	ierr = -5;
      }

      break;

/*    case NDI_CURR_ISO: */
      /* specify which zaid (isotope) is current */
    /*
      if  ( library_type == '0' ) {
	 int pindex;
	 pindex = ndi_find_iso_index( value, depl_targets, num_target );
	 if ( pindex >= 0 ) {
	    curr_reac_index = pindex;
	 } else {
	    *//* Invalid zaid was input -- not found *//*
	    ierr = -5;
	 }
	 
      } else {
	 *//* Operation is valid only for depletion data *//*
	 ierr = -16;
      }

      break;
     */

   case NDI_TEMP:                    
      /*  Set default temperature  */
      default_temp = strtod( value, (char **)NULL );
      break;

   case NDI_FISS_PROD:
     /* Set default fission product ZAID */
     if ( strncmp( value, "default", 8 ) == 0 ) {
       strncpy( def_fiss_prod_id, DEF_FISSION_PROD, sizeof(def_fiss_prod_id) );
     } else {
       strncpy( def_fiss_prod_id, value, sizeof( def_fiss_prod_id ) );
       ierr = ndi_std_zaid( def_fiss_prod_id );
/*        printf ("FISS PROD:>>>>%s<<<<<\n",def_fiss_prod_id ); */
     }
     break;

      /**********  UNKNOWN KEYWORD  ***********/

   default:
      ierr = -4;
      break;
   }

   if ( value ) ndi_free( (void**)&value );
   return(ierr);
}


/**********************************************************************/
/*   FUNCTIONS INTERNAL TO FILE  */
/**********************************************************************/

static int ndi_set_lib_type ( const char *value )

/* Valid input values:
   multigroup_neutron

   Global variables changed (and valid values):
   particle_type -- n
   library_type -- m

-------------------------------------------------------------------------*/

{
   int ierr = 0;

   if ( strncmp( value, "multigroup_neutron", (19 * sizeof(char) )) == 0 ) {
      /**** make section a global variable = multigroup_neutron? ****/
      
      particle_type = 'n';
      library_type = 'm';
   } else if ( strncmp( value, "tn", ( 3 * sizeof(char) ) ) == 0 ) {
      particle_type = '0';
      library_type = 's';
   } else if ( strncmp( value, "tnreactions", ( 12 * sizeof(char) ) ) == 0 ) {
      particle_type = '0';
      library_type = 'r';
   } else if ( strncmp( value, "multigroup_photon", ( 18 * sizeof(char) ) ) == 0 ) {
      particle_type = 'p';
      library_type = 'm';

   } else if ( strncmp( value, "dosimetry_neutron", ( 18 * sizeof(char) ) ) == 0 ) {
      particle_type = 'n';
      library_type = 'd';

      /*
   } else if ( strncmp( value, "neutron_depl", ( 13 * sizeof(char) ) ) == 0 ) {
      particle_type = 'n';
      library_type = '0';
      */

   } else {
      /* "Error:  invalid NDI input argument. */
      ierr = -5;
   }
   
   if ( ierr >= 0 ) {
     ierr = ndi_init_state_common();
     ierr = ndi_init_state_iso_state();
     ierr = ndi_init_state_tn();
     ierr = ndi_init_state_tnreac();
     ierr = ndi_init_state_chain();
   }
 
/*    if ( ierr >= 0 ) ierr = ndi_set0(library_type); */

   
   return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_set_library( const char *value )

/* Valid input values:
   mendf5, mendf6, rmoxsct, default

   Global variables changed (and valid values):
   library_vers --  mendf5, mendf6, rmoxsct, ""

-------------------------------------------------------------------------*/

{
   int ierr = 0;
   int val_length = (int) strlen( value );

/*** Check the !=; it sure doesn't seem right 1!!! */

   if ( strncmp( value, "default", 8 ) == 0 ) {
      strncpy( library_vers, "", sizeof(library_vers) );
   } else {
      if ( ( val_length + 1 ) < 26 ) {   /* Size of library_vers is 13 in ndi_init */
	 strncpy( library_vers, value, (val_length + 1) * sizeof(char) );
      } else {
	 strncpy( library_vers, value, ( 26 * sizeof(char) ) );
      }
   }


/*    if ( strncmp( value, "mendf5", 7 ) == 0 ) { */
/*       strncpy( library_vers, value, 7 ); */
/*    } else if ( strncmp( value, "mendf6", 7 ) == 0 ) { */
/*       strncpy( library_vers, value, 7  ); */
/*    } else if ( strncmp( value, "rmoxsct", 8 ) == 0 ) { */
/*       strncpy( library_vers, value, 8 ); */
/*    } else if ( strncmp( value, "default", 8 ) == 0 ) { */
/*       strncpy( library_vers, "", sizeof(library_vers) ); */
/*    } else { */
      /* "Error:  invalid NDI input argument. */ 
/*       ierr = -5; */
/*    } */


   return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_set_mat_order ( const char *value ) 

/* Valid input values:
   default, transpose

   Global variables changed (and valid values):
   mat_order -- d, t

------------------------------------------------*/
 
{
   int ierr = 0;

   if ( strstr( value, "default" ) != NULL ) {
      mat_order = 'd';
   } else if ( strstr( value, "transpose" ) ) {
      mat_order = 't';
   } else if ( strstr( value, "column" ) ) {
      mat_order = 't';
   } else if ( strstr( value, "row" ) ) {
      mat_order = 'd';
   } else {
      /* "Error:  invalid NDI input argument. */
      ierr = -5;
   }
   return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_set_collapse( const char *value ) 

/* Valid input values:
   30_group, 21_group, 16_group, 14_group, 13_group, 12_group, 10_group,
   9_group, 8_group, 7_group, 6_group, 5_group, 4_group, 3_group, 
   2_group, 1_group, 16_LLNL, default

   Global variables changed (and valid values):
   grp_num_new -- integer giving number of groups; 0 for default or error
   new_ebounds -- array of indices of lib_ebounds array with new energy 
                  boundaries (i.e. new boundaries subset of library bounds).
   collapse_flag -- 0 = no collapsing required, -1 = collapsing required
   n_group -- (same as valid input values)

-------------------------------------------------------------------------*/
   
{
   int ierr = 0;
   int num_grps = 0;

   if ( ( library_type == 'm' ) ||
	( library_type == 'd' ) ) {
      /* Multigroup or dosimetry */
      num_grps = iso_stat.num_grps;
   }

   ierr = ndi_set_e_grps( &new_ebounds, value, &grp_num_new, num_grps,
			  &collapse_flag );

   if ( library_type == 's' ) {
      /* Multigroup */
      tn_num_grps = grp_num_new;
   }

   if ( ierr >= 0 ) {
      strcpy( n_group, value );
   } else {
      strcpy( n_group, "default" );
   }
   return(ierr);
}


/**********************************************************************/
/**********************************************************************/

static int ndi_set_norm ( const char *value ) 

/* Flag to determine whether to normalize chis and edist values

   Valid input values:
   norm, no_norm, default

   Global variables changed (and valid values):
   no_norm -- 0 (normalize); -1 (do not normalize)

------------------------------------------------*/
 
{
   int ierr = 0;

   if ( strstr( value, "default" ) != NULL ) {
      no_norm = 0;
   } else if ( strstr( value, "no_norm" ) ) {
      no_norm = -1;
   } else {
      no_norm = 0;
   }

   return(ierr);
}


/**********************************************************************/
/**********************************************************************/

static int ndi_set_trans_corr_diag( const char *value )

{ 
   int ierr = 0;
   int tmp_order;

   if ( strncmp( value, "none", 4 ) == 0 ) {
      /* No transport correction */
      trans_corr = ' ';               
      trans_corr_order = -1;       
   } else if ( strncmp( value, "diag_", 5 ) == 0 ) {
      tmp_order = get_order( value );;
      if ( ( tmp_order < 0 ) || ( tmp_order > 20 ) ) {
	 ierr = -5;
      } else {
	 trans_corr = 'd';
	 trans_corr_order = (int)tmp_order;
      }
   } else if ( strncmp( value, "bhs_", 4 ) == 0 ) {
      tmp_order = get_order( value );
      if ( ( tmp_order < 0 ) || ( tmp_order > 20 ) ) {
	 ierr = -5;
      } else {
	 trans_corr = 'b';
	 trans_corr_order = (int)tmp_order;
      }
   } else {
      /* "Error:  invalid NDI input argument. */
      ierr = -5;
   }

   return(ierr);

}
/**********************************************************************/
/**********************************************************************/

static int ndi_set_reac_id( const char *value )

{ 
   int ierr = 0;

   if ( strncmp( value, "mt", 3 ) == 0 ) {
      /* Use reaction MT's */
      reac_id = 'm';               
   } else if ( strncmp( value, "c", 2 ) == 0 ) {
      reac_id = 'c';
   } else {
      /* "Error:  invalid NDI input argument. */
      ierr = -5;
   }

   return(ierr);
   
}
/**********************************************************************/
/**********************************************************************/
static int get_order( const char *value ) {

   char *under_score = NULL;
   int return_value = -1;
   
   under_score = strchr( value, '_' );
   if ( under_score ) {
      /*** Check value ***/
      return_value = (int)strtol( ( under_score + 1), NULL, 10 );
   }
   return(return_value);   
}

/**********************************************************************/
/**********************************************************************/

static int ndi_niceStrCp( char *new_str, const char *old_str, int new_len )
   /* Copies old_str to new_str making all characters lowercase 
      and stripping starting and ending whitespace.  The string
      cannot contain any "internal" whitespace.  The first whitespace
      found after characters will cause an end of string to be inserted.
      
   */
{
   int old_len = (int) strlen(old_str);
   int oldindx;
   int newindx = 0;
   int ierr = 0;
   
   for (oldindx = 0; oldindx <= old_len; oldindx++) {
      if ( isspace(old_str[oldindx]) ) {
	 if ( newindx > 0 ) {
	    new_str[newindx] = '\0';
	    break;
	 } 
      } else {
	 new_str[newindx] = tolower(old_str[oldindx]);
	 newindx++;
	 if ( newindx >= new_len ) {
	    new_str[newindx - 1] = '\0';
	    ierr = 7;
	    break;
	 }
      }
   }
   
   /* Handle 'empty' strings */
   if ( newindx == 0 ) new_str[0] = '\0';
   return(ierr);
}


/**********************************************************************/
/**********************************************************************/
