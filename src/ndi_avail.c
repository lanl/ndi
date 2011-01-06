
/*
 * File:	ndi_avail.c
 * Description:	Internal functions--indicates is quantity is available.
 * Time-stamp:	<1999-06-28 13:37:09 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"



/*********************************************************************/

int ndi_nusigf_avail(const state_struct *state, char nu_type)
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

    if (state->iso_stat.fissionable == 0)
	return -1;

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

int ndi_nu_avail(const state_struct *state, char nu_type)
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

int ndi_sigf_avail(const state_struct *state)
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
/* Introspection routine: Does table have sig_xf data either
   explicitly or implicitly?

   Returns flag > 0 if available, 
   where bits indicate how available;
           SIG_XF_MAT: Through type chi matrix and nu_sig_f
           SIG_XF_MMC: Through other chi matricies and nu_sig_f's
           SIG_XF_VEC: Through type chi vector and nu_sig_f
           SIG_XF_MIX: Through other chi matrix/vector and nu_sig_f's
           SIG_XF_VVC: Through other chi vectors and nu_sig_f's
           flag = 0 if unavailable.
*/
unsigned ndi_sigxf_avail(const state_struct *state, char type)
{
    unsigned flag = 0;
    long int mat_off[3];
    long int vec_off[3];
    int has_nusigf[3];
    char option[] = "tpd";
    char *p;
    int i, j, k;

    /* Register all the pertinent data */
    mat_off[0] = state->iso_stat.chi_mat_tot.file_ptr;
    mat_off[1] = state->iso_stat.chi_mat_pr.file_ptr;
    mat_off[2] = state->iso_stat.chi_mat_del.file_ptr;

    vec_off[0] = state->iso_stat.chi_vec_tot.file_ptr;
    vec_off[1] = state->iso_stat.chi_vec_pr.file_ptr;
    vec_off[2] = state->iso_stat.chi_vec_del.file_ptr;

    has_nusigf[0] = ndi_nusigf_avail(state, 't');
    has_nusigf[1] = ndi_nusigf_avail(state, 'p');
    has_nusigf[2] = ndi_nusigf_avail(state, 'd');

    /* Index type using option set */
    p = strchr(option, type);
    if (!p) return 0;           /* Illegal option */
    i = p - option;             /* Index of type in option */
    j = (i==0) ? 1 : 0;         /* Index of other in option */
    k = (i==2) ? 1 : 2;         /* Index of other in option */

    /* Matrix? */
    if (mat_off[i] && has_nusigf[i])
        flag |= SIG_XF_MAT;

    /* Matrix calculation? */
    if (mat_off[j] && has_nusigf[j] && mat_off[k] && has_nusigf[k])
        flag |= SIG_XF_MMC;

    /* Vector? */
    if (vec_off[i] && has_nusigf[i])
        flag |= SIG_XF_VEC;

    /* Mixed calculation? */
    if ((mat_off[j] || vec_off[j]) && has_nusigf[j] && 
        (mat_off[k] || vec_off[k]) && has_nusigf[k])
        flag |= SIG_XF_MIX;

    /* Vector calculation? */
    if (vec_off[j] && has_nusigf[j] && 
        vec_off[k] && has_nusigf[k])
        flag |= SIG_XF_VVC;


    return flag;
}

/*********************************************************************/
