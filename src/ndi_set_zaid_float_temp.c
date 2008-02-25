/*
 * $Id$
 * Description:	User function--set isotope for processing.
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"

/**********************************************************************/

int NDI_SET_ZAID_FLOAT_TEMP(state_struct *state, const char *in_zaid, 
			    FLOAT_TYPE temp)
{
    int ierr = 0;
    long fpos = -1;
    dir_entry rec;
    char *section;
    char zaid[MAX_TN_REAC_LEN] = "";
    double diff = 1.0e38;

    if (!state) NDI_THROW(ierr, -2); /* Check state sanity */

    section = ndi_get_section(state);
    if (section == 0) NDI_THROW(ierr, -16); /* Lib type not set */

    fpos = ftell(state->ndi_dir); /* Remember file position */

    if (ndi_fseek_section(state->ndi_dir, section, 0) != 0)
	NDI_THROW(ierr, -11);	/* Section not found */

    while (ndi_fget_next_dir(&rec, state->ndi_dir, 
			     state->library_vers, in_zaid)) {
	if (diff > abs(rec.temp - temp)) {
	    strncpy(zaid, rec.zaid, MAX_TN_REAC_LEN);
	    diff = abs(rec.temp - temp);
	}
    }

    if (strlen(zaid) > 0)
	ierr = ndi_c_set_zaid_h(state, zaid);

 catch:
    if (fpos >= 0) fseek(state->ndi_dir, fpos, SEEK_SET);
    return ierr;
}

/**********************************************************************/
