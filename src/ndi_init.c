
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

/**********************************************************************/

int ndi_c_init_h(state_struct **curr_state)

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
   state_struct *state;

   if ( ! curr_state ) {
       return(-2);
   }

   *curr_state = NULL;
   state = (state_struct *)ndi_calloc(1, sizeof(state_struct));
   state->ndi_initialized = -1;

   /* Open directory file and initialize state variables */
 
   ierr = ndi_open_gendir( &(state->ndi_dir) );
   if ( ierr < 0 ) {
     /* Fatal error opening gendir */
     state->ndi_initialized = 0;
     return(ierr);
   }

   /* Initialize variables */
   state->handle =-1;
/*
   state->ndi_elements = NULL;
   state->new_ebounds = NULL;
   state->lib_weight = NULL;
   state->lib_ebounds = NULL;
   state->tn_ebounds = NULL;

   state->iso_stat.set_flag = 0;
   state->iso_stat.num_sec_parts = 0;
   state->iso_stat.sec_part = NULL;
   state->iso_stat.pn_down_full.stat = NULL;
   state->iso_stat.pn_full.stat = NULL;
   state->iso_stat.sig_reac.stat = NULL;
   state->iso_stat.rprod_all.stat = NULL;
   state->iso_stat.rprod_rmo.stat = NULL;
   state->iso_stat.rprod_few.stat = NULL;
   state->iso_stat.reac_mt = NULL;
   state->iso_stat.reac_q = NULL;

   state->tn_stat.set_flag = 0;
   state->tn_stat.num_sec_parts = 0;
   state->tnreac_stat.set_flag = 0;
   state->tnreac_stat.reactants = NULL;
   state->tnreac_stat.rprods = NULL;
   state->chain_stat.set_flag = 0;
   state->ndi_depl_set = 0;

   state->lib_stream = NULL;
*/
   state->open_lib_file = (char*)ndi_malloc(sizeof(char));
   state->open_lib_file[0] = '\0';
   ndi_fill_elements(state);

   /*  Set default values */
   state->library_vers = (char *)ndi_calloc(MAX_LIB_VERS_LEN, sizeof(char));
   strncpy( state->library_vers, "", sizeof(state->library_vers) );
   state->particle_type = ' ';
   state->library_type = ' ';
   state->mat_order = 'd';     /* No transpose */
   state->reac_id = 'm';       /* Use MT numbers to identify reactions */
   state->curr_reac_index = -1;                                /* OBSOLETE? */
   strncpy(state->nuc_data_type, "", sizeof(state->nuc_data_type) );  /* OBSOLETE? */

   ierr = ndi_init_state_common(state);
   ierr = ndi_init_state_iso_state(state);
   ierr = ndi_init_state_tn(state);
   ierr = ndi_init_state_tnreac(state);
   ierr = ndi_init_state_chain(state);

   *curr_state = state;
   return(ierr);
  
}

/**********************************************************************/

