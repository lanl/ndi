
/*
 * File:	ndi_init.c
 * Description:	User function--open gendir file and initialize state.
 * Time-stamp:	<2000-02-04 16:07:55 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

/**********************************************************************/

int	ndi_c_init(void)

/*  Purpose:  Open the 'gendir' directory file and assign to variable
    ndi_dir.  Initialize state variables and set default values.

    It is assumed that the gendir file exists either 1) in the current
    directory and is name 'gendir'or 2) the path (including file name)
    is given by the environment variable NDI_GENDIR_PATH or 3) is
    named gendir and is in the directory '/usr/local/codes/data/sn'.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. The 
    external file pointer variable ndi_dir is set to the open gendir
    file.  State variables are set: ndi_initialized,library_vers, 
    mat_order,...
***********************************************************************/

{
   int ierr = 0;           /* Error flag: 0=no error,pos=warning, neg=error */
  
   if ( ndi_initialized ) {
      /*  NDI already initialized */
      return(1);
   } else {
      ndi_initialized = -1;
   }

   /* Open directory file and initialize state variables */
 
   ierr = ndi_open_gendir( &ndi_dir );
   if ( ierr < 0 ) {
     /* Fatal error opening gendir */
     ndi_initialized = 0;
     return(ierr);
   }

   /* Initialize variables */
   lib_stream = NULL;
   if (open_lib_file) ndi_free((void**)&open_lib_file);
   open_lib_file = (char*)ndi_malloc(sizeof(char));
   open_lib_file[0] = '\0';
   ndi_fill_elements();

   /*  Set default values */
   if (library_vers)  ndi_free((void**)&library_vers);
   library_vers = (char *)ndi_calloc(MAX_LIB_VERS_LEN, sizeof(char));
   strncpy( library_vers, "", sizeof(library_vers) );
   particle_type = ' ';
   library_type = ' ';
   mat_order = 'd';     /* No transpose */
   reac_id = 'm';       /* Use MT numbers to identify reactions */
   curr_reac_index = -1;                                /* OBSOLETE? */
   strncpy(nuc_data_type, "", sizeof(nuc_data_type) );  /* OBSOLETE? */

   ierr = ndi_init_state_common();
   ierr = ndi_init_state_iso_state();
   ierr = ndi_init_state_tn();
   ierr = ndi_init_state_tnreac();
   ierr = ndi_init_state_chain();
   return(ierr);
  
}

/**********************************************************************/

