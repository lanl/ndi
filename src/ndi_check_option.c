
/*
 * File:	ndi_check_option.c
 * Description:	User function--check defaults.
 * Time-stamp:	<1999-05-06 07:55:53 jomc>
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

static int ndi_get_lib_type( char *value, int string_size );
static int ndi_get_library( char *value, int string_size );
static int ndi_get_mat_order( char *value, int string_size );
static int ndi_get_collapse( char *value, int string_size );
static int ndi_get_trans_corr_diag( char *value, int string_size );
static int ndi_get_norm( char *value, int string_size );
static int ndi_get_reac_id( char *value, int string_size );


/**********************************************************************/

int ndi_c_check_option ( int in_key, char *out_value, int string_size ) 

/*  Purpose: Check options/defaults.

    Parameters:
    in_key -- in: key to specify option to set.
    out_value -- out: string specifying the value of the option.
    string_size -- in: number of characters in out_value

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok.

***********************************************************************/

{
   int ierr = 0;       /* Error flag: 0=no error, pos=warning, neg=error */
   
   /*** Check that NDI is initialized ***/
   
   /*************************/
   /*   Process input key   */
   /*************************/
   switch (in_key) {
      
   case NDI_LIB_TYPE:                    
      /*  Set library type  */
      ierr = ndi_get_lib_type( out_value, string_size );
      break;

   case NDI_LIBRARY:                    
      /* Set source library */
      ierr = ndi_get_library( out_value, string_size );
      break;

   case NDI_MAT_ORDER:     
      /* Set flag to transpose order of returned matrix */
      ierr = ndi_get_mat_order( out_value, string_size );
      break;
      
   case NDI_COLLAPSE:       
      /* Set no. of energy groups to collapse to */
      ierr = ndi_get_collapse( out_value, string_size );
      break;

   case NDI_TRANS_CORR:
      /* Use transport corrections */
      ierr = ndi_get_trans_corr_diag( out_value, string_size );
      break;

   case NDI_NORM:
      /* Normalize chis and edists? */
      ierr = ndi_get_norm( out_value, string_size );
      break;

   case NDI_REAC_ID:
      /* Specify reaction identifiers--MT or C values */
      ierr = ndi_get_reac_id( out_value, string_size );
      break;

   case NDI_CHAIN:
     /* case NDI_DEPLETION:
	case NDI_RADCHEM: */
      /*  Set chain data -- (depl for MG/radchem for dos) */
     if ( library_type == 'm' )  {
       if ( chain_stat.set_flag ) {
	 strncpy( out_value, chain_stat.dataset, string_size );
       } else {
	 ierr = 9;
       }
     } else {
       /* Incompatible library type */
       ierr = -16;
     } 
     break;

   case NDI_CURR_PART:
      /* Which particle is 'current'  */
     if ( curr_part_index >= 0 ) {
       if ( library_type == 's' ) {
	 sprintf( out_value, "%-ld", tn_stat.sec_part[curr_part_index].part_type );
/* 	 strncpy( out_value, tn_stat.sec_part[curr_part_index].part_type, string_size ); */
       } else if ( library_type == 'm' ) {
	 strncpy( out_value, iso_stat.sec_part[curr_part_index].part_type, string_size );
       } else {
	 /* Operation is valid only for tn data */
	 ierr = -16;
       }
     } else {
       ierr = 9;
     }
     break;

   case NDI_TEMP:                    
     /*  Default temperature  */
     if ( sprintf( out_value, "%e", default_temp)  > string_size ) {
       /* ????*/
     }
     break;

   case NDI_FISS_PROD:
     /* Default fission product ZAID */
     strncpy( out_value, def_fiss_prod_id, string_size );
     break;


      /**********  UNKNOWN KEYWORD  ***********/

   default:
      ierr = -4;
      break;
   }

   return(ierr);
}


/**********************************************************************/
/*   FUNCTIONS INTERNAL TO FILE  */
/**********************************************************************/

static int ndi_get_lib_type ( char *value, int string_size )
   
/* Valid library type values:
   multigroup_neutron, tn, multigroup_photon
   
   Global variables used (and valid values):
   particle_type -- n, p, 0(tn)
   library_type -- m, s(tn)
   
-------------------------------------------------------------------------*/
   
{
   int ierr = 0;
   
   if ( (particle_type == 'n') && (library_type == 'm') ) { 
      strncpy( value, "multigroup_neutron", string_size );
   } else if( (particle_type == '0') && (library_type == 's') ) {
      strncpy( value, "tn", string_size );
   } else if ( (particle_type == 'p') && (library_type == 'm') ) {
      strncpy( value, "multigroup_photon", string_size );
   } else {
      strncpy( value, "", string_size);
      ierr = 9;
   }

   return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_get_library( char *value, int string_size )

/* Valid library values:
   mendf5, mendf6, rmoxsct, default

   Global variables used (and valid values):
   library_vers --  mendf5, mendf6, rmoxsct, ""

-------------------------------------------------------------------------*/

{
   int ierr = 0;


   strncpy( value, library_vers, string_size );

   if ( strncmp( value, "", string_size ) == 0 ) {
      strncpy( value, "default", string_size );
   }

   return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_get_mat_order ( char *value, int string_size ) 

/* Valid mat_order values:
   default, transpose

   Global variables used (and valid values):
   mat_order -- d, t

------------------------------------------------*/
 
{
   int ierr = 0;

   if (mat_order == 'd') {
      strncpy( value, "default", string_size );
   } else if (mat_order == 't') {
      strncpy( value, "transpose", string_size );
   } else {
      strncpy( value, "", string_size);
      ierr = 9;
   }

   return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_get_collapse( char *value, int string_size ) 

/* Valid collapse values:
   30_group, 21_group, 16_group, 14_group, 13_group, 12_group, 10_group,
   9_group, 8_group, 7_group, 6_group, 5_group, 4_group, 3_group, 
   2_group, 1_group, 16_LLNL, default

   Global variables used (and valid values):
   grp_num_new -- integer giving number of groups; 0 for default or error
   new_ebounds -- array of indices of lib_ebounds array with new energy 
                  boundaries (i.e. new boundaries subset of library bounds).
   collapse_flag -- 0 = no collapsing required, -1 = collapsing required
   n_group -- (same as valid input values)

-------------------------------------------------------------------------*/
   
{
   int ierr = 0;

   
   if ( *n_group != '\0' ) {
     strncpy( value, n_group, string_size );
   } else {
     strncpy( value, "default", string_size );
   }
   return(ierr);
}

/**********************************************************************/
/**********************************************************************/

static int ndi_get_trans_corr_diag( char *value, int string_size )

/* Valid  values:
   none, diag_n, bhs_n 
   (where 'n' is an integer indicating the correction order)

   Global variables used (and valid values):
   trans_corr -- " "=no correction, d=diagonal, b=Bell-Hansen-Sandmeier 
   trans_corr_order -- integer giving transport correction order

-------------------------------------------------------------------------*/

{ 
   int ierr = 0;
   char order_str[5];
   char temp_str[11] = "diag_";

   if ( trans_corr == ' ' ) {
      strncpy( value, "none", string_size );
   } else {
      if (trans_corr_order < 100000) {
	 sprintf( order_str, "%d", trans_corr_order ); }
      if ( trans_corr == 'd' ) {
	 strncat( temp_str, order_str, 6 );
	 strncpy( value, temp_str, string_size );
      } else if ( trans_corr == 'b' ) {
	 strcpy( temp_str, "bhs_" );
	 strncat( temp_str, order_str, 6);
	 strncpy( value, temp_str, string_size );
      } else {
	 strncpy( value, "", string_size );
	 ierr = -27;
      }
   }

   return(ierr);

}

/**********************************************************************/
/**********************************************************************/

static int ndi_get_norm( char *value, int string_size )

/* Valid  values:
   norm, no_norm

   Global variables used (and valid values):
   no_norm -- 0=normalize, -1=do not normalize

-------------------------------------------------------------------------*/

{ 
  int ierr = 0;
  
  
  if (no_norm == 0) {
    strncpy( value, "norm", string_size );
  } else if (no_norm != 0) {
    strncpy( value, "no_norm", string_size );
  } else {
    strncpy( value, "", string_size);
    ierr = 9;
  }
  
  return(ierr);

}
/**********************************************************************/
/**********************************************************************/

static int ndi_get_reac_id( char *value, int string_size )

/* Valid  values:
   mt, c

   Global variables used (and valid values):
   reac_id --  Reaction identifier: m=MT (default), c=c-value

-------------------------------------------------------------------------*/

{ 
   int ierr = 0;

   if ( reac_id == 'm' ) {
      strncpy( value, "mt", string_size );
   } else if ( reac_id == 'c' ) {
      strncpy( value, "c", string_size );
   } else {
      strncpy( value, "", string_size );
      ierr = -27;
   }

   return(ierr);
   
}
/**********************************************************************/
/**********************************************************************/
