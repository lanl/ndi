
/*
 * File:	ndi_mpmg.c
 * Description:	State handle ADT
 * Time-stamp:	<Wed Sep 24 15:20:14 MDT 2003>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_state_handle.h"
#include "ndi_mpmg.h"

/**********************************************************************/

struct ndi_mpmg 
{
    ndi_state_handle *n;	/* neutron state */
    ndi_state_handle *g;	/* gamma state */
};

/* 
   Allocate ndi_mpmg and initialize to same state as initial 
   global state in ndi_state.c.  Actually initialization of state by  
   calls to:

   ndi_init();
   ndi_set_option(NDI_LIB_TYPE...;
   ndi_set_option(NDI_LIBRARY...;
   ndi_set_isotope(zaid);

   when state is active
*/
ndi_mpmg *ndi_mpmg_new(char *zaid)
{
    ndi_mpmg *this;
    char photon_zaid[15];

    this = (ndi_mpmg *)malloc(sizeof(ndi_mpmg));
    assert(this);

    this->n = ndi_state_handle_new(); /* create neutron state handle */
    ndi_state_handle_swap(this->n); /* swap it into context */
    ierr = ndi_init();		/* initialize it */
    ierr = ndi_set_option(NDI_LIB_TYPE, "multigroup_neutron");
    ierr = ndi_set_option(NDI_COLLAPSE, "21_lanl");
    ierr = ndi_set_option(NDI_LIBRARY, "mendf6");
    ierr = ndi_set_isotope(zaid);
    ndi_state_handle_swap(this->n); /* swap it out of context */
			  
    this->g = ndi_state_handle_new();
    ndi_state_handle_swap(this->g); /* swap it into context */
    ierr = ndi_init();		/* initialize it */
    ierr = ndi_set_option(NDI_LIB_TYPE, "multigroup_photon");
    ierr = ndi_set_option(NDI_COLLAPSE, "default");
    ierr = ndi_set_option(NDI_LIBRARY, "mendf6g");
    sprintf(photon_zaid, "%-ld000", strtol(zaid, NULL, 0)/1000);
    ierr = ndi_set_isotope(photon_zaid);
    ndi_state_handle_swap(this->g);

    return this;
}

/*
  Deallocate ndi_mpmg.  Actual deallocation of state by call to:

  ndi_close();

  when state is active
*/
void ndi_mpmg_free(ndi_mpmg *this)
{
    ndi_state_handle_swap(this->g);
    ierr = ndi_close();
    ndi_state_handle_swap(this->g);
    ndi_state_handle_free(this->g);

    ndi_state_handle_swap(this->n);
    ierr = ndi_close();
    ndi_state_handle_swap(this->n);
    ndi_state_handle_free(this->n);

    free((void *)this);
}

