
/*
 * File:	ndi_misc_depl.c
 * Description:	Miscellaneous functions dealing with depletion (no floats)
 * Time-stamp:	<2000-02-08 15:43:38 jomc>
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

#define MAX_SZA_SIZE  16

/**********************************************************************/

int ndi_depletion( const char *in_zaid )
   /* The library type must already have been set to multigroup */

{ 
   int ierr = 0;
   FILE *depl_file = NULL;
   char library[20]  = "";   /* Name of file (e.g. mendf5) */
   char lib_path[250] = ""; /* Path of library file */
   char lib_file_type[] = "   ";  /* Currently only 'asc' is valid ascii */
   long int iso_start = 0; /* Line number in file where isotope data begins */
   char tmp_zaid[20] = "";
   char in_lib_type = ' ';
   double at_wgt = 0.0;

   if ( library_type != 'm' ) {
      /* Incompatible library type */
      return( -16 );
   }

   /* Initialize depletion data */
   ierr = ndi_init_state_chain();
   /* Read data from gendir */
   ierr = ndi_zaid_dir(particle_type, library_type, in_zaid, tmp_zaid,
		       library, lib_path, lib_file_type, &iso_start, 
		       &at_wgt, -1 );

   /* Check validity of data read from gendir */

   /* Set global variable to indicate the name of the new depl dataset */
   strcpy( depl_dataset, tmp_zaid);

   /* Open library file */
   if (( depl_file = ndi_open_lib_file(lib_path)) == NULL) {
     /* Error opening library file */
     ierr = -1;
   }

   /* Set new depletion state from library */
   in_lib_type = '0';
   ierr = ndi_init_iso_state( depl_file, in_lib_type, iso_start, tmp_zaid ); 

   /* Close library */
   if ( depl_file ) ierr = ndi_close_lib_file( depl_file );

   if ( ierr >= 0 ) {
      ndi_depl_set = -1;
   }
   return(ierr);
   
}
/**********************************************************************/
/**********************************************************************/

int ndi_set_default_depl( void ) {

  int ierr = 0;
  char tmp_zaid[MAX_IN_STR_LEN] = "";   /* This 'zaid' will match any zaid in the gendir */
  char library[20];   /* Name of file (e.g. mendf5) */
  char lib_path[250]; /* Path of library file */
  char lib_file_type[] = "   ";  /* Currently only 'asc' is valid ascii */
  long int iso_start; /* Line number in file where isotope data begins */
  double at_wgt;


  ierr = ndi_zaid_dir(particle_type, library_type, tmp_zaid, depl_dataset,
		      library, lib_path, lib_file_type, &iso_start, 
		      &at_wgt, -1 );
  ierr = ndi_depletion( depl_dataset );
  
  return( ierr );

}

/**********************************************************************/
/**********************************************************************/

int ndi_set_curr_target( depl_target_struct **targ_p, int *target_index ) 

     /* Set *targ_p to point at the target struct whose zaid is given by the global
	var 'zaid'.  The param target_index will be the index of the desired target in
	the targ_p  array.  If no such target is found, targ_p will be NULL and
	target_index will be set to -1.  On entry *targ_p will point to the beginning 
	of the target array.

        WARNING: the global variable depl_target should not be passed as the parameter
	*targ_p, since it will be changed within this function.

	*/

{

   int i;
   int ierr = 0;
   int curr_index = -1;
   int itarget;
   char *token;
   char tmp_zaid[15];
   depl_target_struct *curr_targ = *targ_p;


   /* Initialize output variables */
   *targ_p = (depl_target_struct *)NULL;
   *target_index = -1;

  /* Check that depletion is set */
  if ( ! ndi_depl_set ) {
    /* Depletion option not set */
    ierr = -30;
    return( ierr );
  }

  /* Change target zaid to integer */
  strcpy( tmp_zaid, iso_stat.zaid ); 
  token = strtok( tmp_zaid, "." );
  if ( token != NULL ) {
    itarget = (int) strtol( token, (char **)NULL, 10 );
  } else {
    itarget = (int) strtol( tmp_zaid, (char **)NULL, 10 );
  }

  if (!curr_targ) {
    return(-30);
  }

   /* Find target index in depl_target */
   for ( i = 0; i < num_target; i++ ) {
     if ( curr_targ->izaid == itarget ) {
       curr_index = i;
       break;
     } else {
       curr_targ++;
     }
   }
   if ( curr_index < 0 ) {
     /* Current zaid not found in targets */
     ierr = -31;
     return( ierr );
   } 

   *targ_p = curr_targ;
   *target_index = curr_index;

   return( ierr );

}

/**********************************************************************/
/**********************************************************************/

int ndi_set_curr_rprod( depl_rprod_struct **rprod_p, const char *rprod, 
			int *prod_index ) 

     /* Set *rprod_p to point at the rprod struct whose zaid is given by the input
	rprod.  The param prod_index will be the index of the desired rprod in the 
	rprod_p array.  If no such prod is found, rprod_p will be NULL and prod_index
	will be set to -1.  On entry *rprod_p will point to the beginning of the rprod
	array.

	On entering prod_index is the number of rprods in the current array.  On exit,
	it will be the index of the desired rprod.
	*/

{

   int i;
   int ierr = 0;
   int curr_index = -1;
   int iprod;
   int num_rprods = *prod_index;
   int zaid_size = MAX_SZA_SIZE;
   char *token;
   char tmp_zaid[MAX_SXA_SIZE];
   char *curr_zaid = tmp_zaid;

   depl_rprod_struct *curr_prod = *rprod_p;

   /* Initialize output variables */
   *rprod_p = (depl_rprod_struct *)NULL;
   *prod_index = -1;

   /* Check that depletion is set */
   if ( ! ndi_depl_set ) {
     /* Depletion option not set */
     ierr = -30;
     return( ierr );
   }

   /* Change input rprod to integer */
   strcpy( tmp_zaid, rprod );
   if ( tmp_zaid[0] == '-' ) {
       curr_zaid = &tmp_zaid[1];
       zaid_size -= 1;
   }
   ierr = ndi_std_zaid( curr_zaid, zaid_size );

   token = strtok( tmp_zaid, "." );
   if ( token != NULL ) {
     iprod = (int) strtol( token, (char **)NULL, 10 );
   } else {
     iprod = (int) strtol( tmp_zaid, (char **)NULL, 10 );
   }
   
    /* Find rprod index in rprod array */
   for ( i = 0; i < num_rprods; i++ ) {
     if ( curr_prod->izaid == iprod ) {
       curr_index = i;
       break;
     } else if ( ( curr_prod->izaid == 99999 ) && 
		 ( iprod == (int) strtol(def_fiss_prod_id, (char **)NULL, 10 ) ) ) {
       /* Special check for fission products 
	  The curr_prod is still '99999', while iprod has been changed
	  to the default fission product */
       curr_index = i;
       break;
     } else {
       curr_prod++;
     }
   }
   if ( curr_index < 0 ) {
     /* Reaction product not found in current depletion set */
     ierr = -32;
     return( ierr );
   } 

   *rprod_p = curr_prod;
   *prod_index = curr_index;

   return( ierr );

}

/**********************************************************************/
/**********************************************************************/
int ndi_radchem ( const char *in_zaid )
   /* The library type must already have been set to dosimetry*/

{ 
   int ierr = 0;
   FILE *depl_file = NULL;
   char library[20]  = "";   /* Name of file (e.g. mendf5) */
   char lib_path[250] = ""; /* Path of library file */
   char lib_file_type[] = "   ";  /* Currently only 'asc' is valid ascii */
   long int iso_start = 0; /* Line number in file where isotope data begins */
   char tmp_zaid[20] = "";
   char in_lib_type = ' ';
   double at_wgt = 0.0;

   if ( library_type != 'd' ) {
      /* Incompatible library type */
      return( -16 );
   }

   /* Initialize depletion data? */
   ierr = ndi_set0('0');
   /* Read data from gendir */
   ierr = ndi_zaid_dir(particle_type, library_type, in_zaid, tmp_zaid,
		       library, lib_path, lib_file_type, &iso_start, 
		       &at_wgt, -1 );

   /* Check validity of data read from gendir */

   /* Set global variable to indicate the name of the new depl dataset */
   strcpy( depl_dataset, tmp_zaid);

   /* Open library file */
   if (( depl_file = ndi_open_lib_file(lib_path)) == NULL) {
     /* Error opening library file */
     ierr = -1;
   }

   /* Set new depletion state from library */
   in_lib_type = '0';
   ierr = ndi_init_iso_state( depl_file, in_lib_type, iso_start, tmp_zaid ); 

   /* Close library */
   if ( depl_file ) ierr = ndi_close_lib_file( depl_file );

   if ( ierr >= 0 ) {
      ndi_depl_set = -1;
   }
   return(ierr);
   
}
/**********************************************************************/
/**********************************************************************/
