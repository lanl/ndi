/*
 * $Id$
 * Description: User function--set isotope by temperature and background
 * cross section for processing.
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"

/**********************************************************************/

int NDI_SET_ZAID_FLOAT_TEMP_SIG_0(state_struct *state,
				  const char *in_zaid, FLOAT_TYPE temp,
				  FLOAT_TYPE sig0)
{
    int ierr = 0;
    long fpos = -1;
    dir_entry rec;
    char *section;
    char tmp_zaid[MAX_IN_STR_LEN];
    char zaid[MAX_TN_REAC_LEN];
    double diff_temp = DBL_MAX;
    double diff_sig0 = DBL_MAX;

    if (!state && !state->ndi_dir) 
        NDI_THROW(ierr, -2); /* Check state sanity */

    section = ndi_get_section(state);
    if (section == 0) NDI_THROW(ierr, -16); /* Lib type not set */

    /* Convert input zaid to standard zaid */
    strcpy(tmp_zaid, in_zaid);
    ierr = ndi_std_zaid(state, tmp_zaid, sizeof(tmp_zaid));

    /* tmp_zaid now contains SZA.(\0|.*\0) */ 
    if (*(strchr(tmp_zaid, '.') + 1) != '\0') { /* Qualified zaid override */

        ierr = ndi_c_set_zaid_h(state, tmp_zaid);

    } else {                    /* Multi-temp search */

        fpos = ftell(state->ndi_dir); /* Remember file position */

        if (ndi_fseek_section(state->ndi_dir, section, 0) != 0)
            NDI_THROW(ierr, -11);   /* Section not found */

        /* Find matching zaid: set if first or temperature closer than prev. */
        while (ndi_fget_next_dir(&rec, state->ndi_dir, 
                                 state->library_vers, tmp_zaid)) {
            if (fabs(rec.temp - temp) <= diff_temp &&
		fabs(rec.sig_0 - sig0) <= diff_sig0) {
                strncpy(zaid, rec.zaid, MAX_TN_REAC_LEN);
                diff_temp = fabs(rec.temp - temp);
		diff_sig0 = fabs(rec.sig_0 - sig0);
            }
        }

        /* Set matching zaid with smallest temp and sig0 diff */
        if (diff_temp < DBL_MAX && diff_sig0 < DBL_MAX)
            ierr = ndi_c_set_zaid_h(state, zaid);
        else
            NDI_THROW(ierr, -8);    /* Isotope not found */
    }

    if ( sig0 > 0.0 ) {
        if ( diff_sig0/sig0 > 1.0E-3 )
            NDI_THROW(ierr, -8); /* Relative error too large,
                                    particular isotope not found */
    } else {
        if ( diff_sig0 > 1.0E-3 )
            NDI_THROW(ierr, -8); /* Absolute error too large,
                                    particular isotope not found */
    }
    if ( temp > 0.0 ) {
        if ( diff_temp/temp > 1.0E-3 )
            NDI_THROW(ierr, -8); /* Relative error too large,
                                    particular isotope not found */
    } else {
        if ( diff_temp > 1.0E-3 )
            NDI_THROW(ierr, -8); /* Absolute error too large,
                                    particular isotope not found */
    }

 catch:
    if (fpos >= 0) fseek(state->ndi_dir, fpos, SEEK_SET);
    return ierr;
}

/**********************************************************************/
/* Set zaid by temperature and background cross section
   for multi-particle handle (Reqt. 75).
   
   For each particle in a multi-particle handle:
   if neutron set zaid by temperature and sig0
   else set zaid.

   In either case try first fully qualified zaid, then za.
   N.B. This routine works best if MP's have been set
   with appropriate libraries.

*/
int NDIMP_SET_ZAID_FLOAT_TEMP_SIG_0(state_struct *h, const char *zaid, 
                                    FLOAT_TYPE temp, FLOAT_TYPE sig0)
{
    int ierr = 0;
    int jerr;

    mg_zaid std_zaid;           /* Standard zaid */
    mg_zaid std_za;             /* Standard za */
    const int zaid_size = sizeof(mg_zaid);

    char *d;
    int i;
    int n;

    state_struct **m;           /* mp handle map */
    state_struct *p;            /* current child handle */

    /* Copy zaid, */
    strncpy(std_zaid, zaid, zaid_size);
    /* standardize it, */
    jerr = ndi_std_zaid(h, std_zaid, zaid_size);
    NDI_THROW(ierr, jerr);
    /* and extract za by truncating string after point*/
    strncpy(std_za, std_zaid, zaid_size);
    if ((d=strchr(std_za, '.'))) *(d+1) = '\0';

    m = h->handle_map;
    n = h->handle_free;
    for (i = 0; i < n; i++) {
        if ((p = m[i]) == 0) {
            NDI_THROW(ierr, -2); /* state not set */
        } else {
            if (p->particle_type == 1) { /* neutron; temp and sig0 dependent */
                /* Try setting full zaid, then za */
                if ((jerr = NDI_SET_ZAID_FLOAT_TEMP_SIG_0(p, std_zaid, temp, sig0)) < 0)
                    jerr = NDI_SET_ZAID_FLOAT_TEMP_SIG_0(p, std_za, temp, sig0);
                NDI_THROW(ierr, jerr);
            } else {            /* non-neutron; non-temp and non-sig0 dependent */
                /* Try setting full zaid, then za */
                if ((jerr = ndi_c_set_zaid_h(p, std_zaid)) < 0)
                    jerr = ndi_c_set_zaid_h(p, std_za);
                NDI_THROW(ierr, jerr);
            }
        }
    }
 catch:
    return ierr;
}

/**********************************************************************/
