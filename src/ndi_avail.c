
/*
 * File:	ndi_avail.c
 * Description:	Internal functions--indicates is quantity is available.
 * Time-stamp:	<1999-06-28 13:37:09 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"



/*********************************************************************/

int ndi_nusigf_avail(state_struct *state, char nu_type)
{

    /*  Purpose:  Determines if the indicated nusigf value for the current
       particle is available in the library.  

       Parameterus:
       nu_type -- in:t=total, p=prompt, d=delayed

       Returns: 0 if nusigf is not available and -1 if nusigf is available.

       ******************************************************************** */
    int return_val = 0;		/* Default is false */

    long int nusigf_ptr;
    long int nusigf1_ptr;
    long int nusigf2_ptr;

    if (!state) {
	return (-2);
    }

    switch (nu_type) {

    case 'p':
	nusigf_ptr = state->iso_stat.nu_sig_f_pr.file_ptr;
	nusigf1_ptr = state->iso_stat.nu_sig_f_tot.file_ptr;
	nusigf2_ptr = state->iso_stat.nu_sig_f_del.file_ptr;
	break;

    case 'd':
	nusigf_ptr = state->iso_stat.nu_sig_f_del.file_ptr;
	nusigf1_ptr = state->iso_stat.nu_sig_f_tot.file_ptr;
	nusigf2_ptr = state->iso_stat.nu_sig_f_pr.file_ptr;
	break;

    case 't':
	nusigf_ptr = state->iso_stat.nu_sig_f_tot.file_ptr;
	nusigf1_ptr = state->iso_stat.nu_sig_f_pr.file_ptr;
	nusigf2_ptr = state->iso_stat.nu_sig_f_del.file_ptr;
	break;

    default:
	return (return_val);
	break;

    }				/* switch */

    if (nusigf_ptr) {
	/* Nu_sigf is contained in library */
	return_val = -1;	/* True--the nu is available */
    } else if (ndi_nu_avail(state, nu_type) && ndi_sigf_avail(state)) {
	/* Nu_sigf can be calculated from nu and sigf */
	return_val = -1;
    } else if (nusigf1_ptr && nusigf2_ptr) {
	/* Nu_sigf can be calculated from other two nu_sigf's */
	return_val = -1;
    }

    /* Must add cases where nu is available--this data may be added to
       libraries some day.
     */


    return (return_val);
}

/*********************************************************************/
/*********************************************************************/

int ndi_nu_avail(state_struct *state, char nu_type)
{

    /*  Purpose:  Determines if the indicated nu value 
       is available in the library.  

       Parameters:
       nu_type -- in:t=total, p=prompt, d=delayed

       Returns: 0 if nu is not available and -1 if nu is available.


       ******************************************************************** */

    int return_val = 0;		/* Default is false */

    long int nu_ptr;
    long int nu1_ptr;
    long int nu2_ptr;
    long int nusigf_ptr;
    long int nusigf1_ptr;
    long int nusigf2_ptr;

    if (!state) {
	return (-2);
    }

    switch (nu_type) {

    case 't':
	nu_ptr = state->iso_stat.nu_tot.file_ptr;
	nu1_ptr = state->iso_stat.nu_pr.file_ptr;
	nu2_ptr = state->iso_stat.nu_del.file_ptr;
	nusigf_ptr = state->iso_stat.nu_sig_f_tot.file_ptr;
	nusigf1_ptr = state->iso_stat.nu_sig_f_pr.file_ptr;
	nusigf2_ptr = state->iso_stat.nu_sig_f_del.file_ptr;
	break;

    case 'p':
	nu_ptr = state->iso_stat.nu_pr.file_ptr;
	nu1_ptr = state->iso_stat.nu_tot.file_ptr;
	nu2_ptr = state->iso_stat.nu_del.file_ptr;
	nusigf_ptr = state->iso_stat.nu_sig_f_pr.file_ptr;
	nusigf1_ptr = state->iso_stat.nu_sig_f_tot.file_ptr;
	nusigf2_ptr = state->iso_stat.nu_sig_f_del.file_ptr;
	break;

    case 'd':
	nu_ptr = state->iso_stat.nu_del.file_ptr;
	nu1_ptr = state->iso_stat.nu_tot.file_ptr;
	nu2_ptr = state->iso_stat.nu_pr.file_ptr;
	nusigf_ptr = state->iso_stat.nu_sig_f_del.file_ptr;
	nusigf1_ptr = state->iso_stat.nu_sig_f_tot.file_ptr;
	nusigf2_ptr = state->iso_stat.nu_sig_f_pr.file_ptr;
	break;

    default:
	return (return_val);
	break;
    }				/* switch */

    if (nu_ptr) {
	/* Nu is contained in library */
	return_val = -1;
    } else if (nu1_ptr && nu2_ptr) {
	/* Calculate nu from other two nu's */
	return_val = -1;
    } else if (nusigf_ptr && ndi_sigf_avail(state)) {
	/* Calculate nu from nu_sigf and sigf */
	return_val = -1;
    } else if (nusigf1_ptr && nusigf2_ptr && ndi_sigf_avail(state)) {
	/* Calculate nu from other nu_sigf's and sigf */
	return_val = -1;
    }

    return (return_val);
}

/*********************************************************************/
/*********************************************************************/

int ndi_sigf_avail(state_struct *state)
{

    /*  Purpose:  Determines if the indicated fission cross-section is 
       available in the library.  Assumes that if MT = 18 (total fission) 
       is listed in the state->iso_stat.reac_mt list, that the cross-section is there.

       Parameters:
       None

       Returns: 0 if sigf is not available and -1 if sigf is available.

       ******************************************************************** */

    int return_val = 0;		/* Default is false */
    int reac_index = -1;
    int mt = 18;		/*  Total fission cross-section  */

    if (!state) {
	return (-2);
    }

    (void) ndi_find_reac_index(state->iso_stat.reac_mt, &reac_index,
			       mt, state->iso_stat.num_reac);
    if (reac_index >= 0) {
	return_val = -1;	/* True--the nu is available */
    }

    return (return_val);
}

/*********************************************************************/
/*********************************************************************/
