#include <stdio.h>

#include "ndi.h"
#include "ndi_internal.h"

/**********************************************************************/

int
ndimp_get_size_n(state_struct *h, int in_key, int vec_num, int *ierr)
{

    int return_val = 0;
    int tmp_int = 0;

    /* Check each particle is multigroup */
    /* Check num_parts <= MAX_HANDLE_NUM */

    switch (in_key) {

    case NDI_PN_FULL:
	/*  */
	ndimp_get_int_val(h, NDI_NUM_GRPS, &tmp_int);
	return_val = tmp_int * tmp_int;
	break;

    case NDI_SIG_REAC:
    case NDI_PNU_PR:
    case NDI_SIG_PNU_PR:
    case NDI_CORR_DIAG:
    case NDI_CORR_BHS:
        *ierr = ndimp_get_int_val(h, NDI_NUM_GRPS, &return_val);
        break;

    default:
	*ierr = -4;
	return_val = 0;
	break;
    }

    return (return_val);

}

/**********************************************************************/
