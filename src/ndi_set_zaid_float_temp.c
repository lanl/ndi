/*
 * $Id$
 * Description: User function--set isotope for processing.
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

static int NDI_NESTED_SET_ZAID_FLOAT_TEMP(state_struct *state, FILE *stream,
                                         char *zaid, char *out_zaid, FLOAT_TYPE temp,
                                         int num_grps, double *diff);

/**********************************************************************/

int NDI_SET_ZAID_FLOAT_TEMP(state_struct *state, const char *in_zaid, 
                            FLOAT_TYPE temp)
{
    int ierr = 0;
    long fpos = -1;
    char *section;
    char tmp_zaid[MAX_IN_STR_LEN];
    char *zaid;
    char *out_zaid;
    double diff = DBL_MAX;
    char seps_dot[] = ".";
    int num_grps;

    out_zaid = (char *) ndi_calloc(MAX_IN_STR_LEN, sizeof(char));

    if (!state && !state->ndi_dir) 
        NDI_THROW(ierr, -2); /* Check state sanity */

    section = ndi_get_section(state);
    if (section == 0) NDI_THROW(ierr, -16); /* Lib type not set */

    /* Make sure the isotope found is compatible with the set number of groups */
    num_grps = state->iso_stat.num_grps;

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

        zaid = strtok(tmp_zaid, seps_dot);
        /* Find matching zaid: set if first or temperature closer than prev.
           This now allows nested gendirs; if a new gendir is opened, then
           state->ndi_dir is modified. */
        ierr = NDI_NESTED_SET_ZAID_FLOAT_TEMP(state, state->ndi_dir, zaid, out_zaid, temp, num_grps, &diff);

        /* Set matching zaid with smallest temp diff */
        if (diff < DBL_MAX)
            ierr = ndi_c_set_zaid_h(state, out_zaid);
        else
            NDI_THROW(ierr, -8);    /* Isotope not found */
    }

 catch:
    if (fpos >= 0) fseek(state->ndi_dir, fpos, SEEK_SET);
    return ierr;
}

/**********************************************************************/
/* Set zaid by temperature, allowing for nested gendirs. If a nested
 * gendir is found, open it and look for the zaid there. This function
 * will find the zaid with the closest temperature, even if it does not
 * match exactly. If there are multiple zaids with the same temperature
 * from the same library, this will return the first one. */

static int NDI_NESTED_SET_ZAID_FLOAT_TEMP(state_struct *state, FILE *stream,
                                         char *zaid, char *out_zaid, FLOAT_TYPE temp,
                                         int num_grps, double *diff) {
    int ierr = 0;
    FILE *temp_stream = NULL;
    dir_entry rec;
    char *path = NULL;

    /* Find a gendir entry with a matching zaid and library */
    while (ndi_fget_next_dir(&rec, stream, state->library_vers, zaid) != NULL) {
        if (strncmp(rec.zaid,"gendir",strlen("gendir")) == 0) {
            /* Found a gendir with the correct library, open it up */
            ierr = ndi_open_gendir(&temp_stream, rec.path, &path);
            if (ierr < 0) break;
            /* Call NDI_NESTED_SET_ZAID_FLOAT_TEMP recursively for nested gendirs */
            ierr = NDI_NESTED_SET_ZAID_FLOAT_TEMP(state, temp_stream, zaid,
                                                  out_zaid, temp, num_grps, diff);
            if (ierr < 0) break;
            /* Close the nested gendir */
            if (fclose(temp_stream) != 0) ierr = -3;
        } else if (fabs(rec.temp - temp) < *diff && // Check if the temperature is closer than the previously found zaid
            (rec.num_grps == num_grps || num_grps == 0)) { // Make sure it has the correct number of groups, if the number of groups has been set
            /* Store the current best zaid and temperature differential */
            strcpy(out_zaid,rec.zaid);
            *diff = fabs(rec.temp - temp);
        }
    }

    return ierr;

}


/**********************************************************************/
/* Set zaid by temperature for multi-particle handle (Reqt. 75).
   
   For each particle in a multi-particle handle:
   if neutron set zaid by temperature
   else set zaid.

   In either case try first fully qualified zaid, then za.
   N.B. This routine works best if MP's have been set
   with appropriate libraries.

*/
int NDIMP_SET_ZAID_FLOAT_TEMP(state_struct *h, const char *zaid, 
                              FLOAT_TYPE temp)
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
    strcpy(std_zaid, zaid);
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
            if (p->particle_type == 1) { /* neutron; temp dependent */
                /* Try setting full zaid, then za */
                if ((jerr = NDI_SET_ZAID_FLOAT_TEMP(p, std_zaid, temp)) < 0)
                    jerr = NDI_SET_ZAID_FLOAT_TEMP(p, std_za, temp);
                NDI_THROW(ierr, jerr);
            } else {            /* non-neutron; non-temp dependent */
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
