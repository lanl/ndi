#include <stdio.h>
#include <string.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

/**********************************************************************/
int
ndimp_get_string_val(const int *handle_arr, const int num_parts,
		     const int in_key, char *ret_val, const int str_size)
{

    int ierr = 0;


    if ((ret_val) && (str_size > 0)) {
	*ret_val = '\0';
    } else {
	ierr = 7;
	return (ierr);
    }

    if (in_key == NDI_VERSION) {
	strncpy(ret_val, NDI_VERSION_DATA, str_size);
    } else {
	ierr = -4;
    }

    return (ierr);
}
