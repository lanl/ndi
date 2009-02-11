#include <stdio.h>
#include <string.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

#define ZAID_SIZE 25

/**********************************************************************/

int ndimp_set_zaid(state_struct *h, const char *zaid)
{

    int ierr = 0;
    int tmp_err = 0;
    int i;
    int z, a;
    char *dot_p = NULL;
    char *tmp_zaid = NULL;
    char std_zaid[ZAID_SIZE];
    state_struct *statep;
    state_struct **handle;
    int num_parts;

    handle = h->handle_map;
    num_parts = h->handle_free;

    /* Check each particle is multigroup */
    /* Check num_parts <= MAX_HANDLE_NUM */

    /* Translate ZAID to standard form */
    strncpy(std_zaid, zaid, ZAID_SIZE);
    tmp_err = ndi_std_zaid(h, std_zaid, ZAID_SIZE);
    if (tmp_err < 0) {
	return (tmp_err);
    }

    for (i = 0; i < num_parts; i++) {
	if ((statep = (handle[i]))) {
	    tmp_err = ndi_c_set_isotope_h(statep, std_zaid);
	    if (tmp_err < 0) {
		tmp_zaid = (char *) ndi_malloc(strlen(std_zaid) + 1
					       * sizeof(char));
		strncpy(tmp_zaid, std_zaid, strlen(std_zaid) + 1);
		/* Strip ZAID ending, if present */
		if ((dot_p = strchr(tmp_zaid, '.'))) {
		    *dot_p = '\0';
		    tmp_err = ndi_c_set_isotope_h(statep, tmp_zaid);
		    if (tmp_err >= 0) {
			/* check err */
			ndi_free((void **) &tmp_zaid);
			continue;
		    }
		}
		/* Try element */
		tmp_err = ndi_parse_mg_zaid(tmp_zaid, &z, &a);
		sprintf(tmp_zaid, "%d", z * 1000);
		tmp_err = ndi_c_set_isotope_h(statep, tmp_zaid);
		/* check err */
		ndi_free((void **) &tmp_zaid);
	    }
	    if ((tmp_err != 0) && ((ierr >= 0) || (tmp_err < 0)))
		ierr = tmp_err;
	} else {
	    ierr = -2;
	    break;
	}
    }

    return (ierr);

}

/**********************************************************************/
