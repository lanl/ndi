
/*
 * File:	ndi_close.c
 * Description:	User function--deallocate memory and close files.
 * Time-stamp:	<2000-02-04 16:09:47 jomc>
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

int ndi_c_close(void)

/*  Purpose:  Deallocate memory and close directory and library files.
    
    It is assumed that NDI has been initialized (ndi_initialized = -1).
    
    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok.
    
***********************************************************************/
   
{
   int ierr = 0;        /* Error flag: 0=no error,pos=warning, neg=error */
   int ret_code = 0;    /* Return code--returned error value */
   
   if (!ndi_initialized) {
      /* Warning: NDI close failed--already closed */
      return(2);
   }
   
   /* Close directory file and library file */
   if ( fclose(ndi_dir) != 0 ) {
     ret_code = 3;
   } else {
     ndi_dir = NULL;
   }
   ierr = ndi_close_lib_file(lib_stream);
   if ( open_lib_file ) { ndi_free( (void**)&open_lib_file); }
   if ( ierr < 0 ) {
      ret_code = ierr;
   } else if ( ( ret_code >= 0 ) && ( ierr != 0 ) ) {
      ret_code = ierr;
   }
   
   /* Initialize extern variables */
   if (ndi_elements) ndi_free((void**)&ndi_elements);
   if (library_vers) ndi_free((void**)&library_vers);
   particle_type = ' ';
   library_type = ' '; 
   mat_order = 'd';     /* No transpose */
   reac_id = 'm';       /* Use MT numbers to identify reactions */
   curr_reac_index = -1;
   strncpy(nuc_data_type, "", sizeof(nuc_data_type) );  /* OBSOLETE? */

   ierr = ndi_init_state_common();
   ierr = ndi_init_state_iso_state();
   ierr = ndi_init_state_tn();
   ierr = ndi_init_state_tnreac();
   ierr = ndi_init_state_chain();

   ndi_initialized = 0;

   return(ret_code);
   
}

/**********************************************************************/
