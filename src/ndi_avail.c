
/*
 * File:	ndi_avail.c
 * Description:	Internal functions--indicates is quantity is available.
 * Time-stamp:	<1999-06-28 13:37:09 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"

/*********************************************************************/

unsigned ndi_nusigf_avail(const state_struct *state, char nu_type)
{

    /* Introspection: Can ndi get nusigf? 

    Return: nusigf | nusigf1 & nusigf2 | sig_reac_18 & (nu | 
    nu1 & nusigf2 | nusigf1 & nu2 | nu1 & nu2) */

    unsigned flag = 0;

    long int nu_ptr;
    long int nu1_ptr;
    long int nu2_ptr;
    long int nusigf_ptr;
    long int nusigf1_ptr;
    long int nusigf2_ptr;

    assert(state);              /* REQUIRE */

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
	return flag;
	break;
    }

    if (nusigf_ptr)
        flag |= NUSIGF;
    if (nusigf1_ptr && nusigf2_ptr)
        flag |= NUSIGF1_NUSIGF2;
    if (ndi_sigf_avail(state)) {
        if (nu_ptr)
            flag |= NU;
        if (nu1_ptr && nusigf2_ptr)
            flag |= NU1_NUSIGF2;
        if (nusigf1_ptr && nu2_ptr)
            flag |= NUSIGF1_NU2;
        if (nu1_ptr && nu2_ptr)
            flag |= NU1_NU2;
    }
    return flag;
}

/*********************************************************************/
/*********************************************************************/

unsigned ndi_nu_avail(const state_struct *state, char nu_type)
{
    /* Introspection: Can ndi get nu?

    Return: nu | nu1 & nu2 | sig_reac_18 & (nusigf |
    | nu1 & nusigf2 | nusigf1 & nu2 | nusigf1 & nusigf2) */

    unsigned flag = 0;		/* Default is false */

    long int nu_ptr;
    long int nu1_ptr;
    long int nu2_ptr;
    long int nusigf_ptr;
    long int nusigf1_ptr;
    long int nusigf2_ptr;

    assert(state);              /* REQUIRE */

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
	return flag;
	break;
    }				/* switch */

    if (nu_ptr)                 /* nu in library */
        flag |= NU;
    if (nu1_ptr && nu2_ptr)      /* calculate from other nu's */
        flag |= NU1_NU2;
    if (ndi_sigf_avail(state)) {
        if (nusigf_ptr)
            flag |= NUSIGF;
        if (nu1_ptr && nusigf2_ptr)
            flag |= NU1_NUSIGF2;
        if (nusigf1_ptr && nu2_ptr)
            flag |= NUSIGF1_NU2;
        if (nusigf1_ptr && nusigf2_ptr)
            flag |= NUSIGF1_NUSIGF2;
    }
    return flag;
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

    assert(state);              /* REQUIRE */

    (void) ndi_find_reac_index(state->iso_stat.reac_mt, &reac_index,
			       mt, state->iso_stat.num_reac);
    if (reac_index >= 0) {
	return_val = -1;	/* True: sig_react_18 available */
    }

    return return_val;
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
