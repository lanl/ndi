#include <stdio.h>
#include <string.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

/**********************************************************************/
int ndimp_get_string_val(state_struct *h, int in_key, char *ret_val,
			 int str_size)
{

    int ierr = 0;

    if (!h)
	return -2;

    if ((ret_val) && (str_size > 0))
	*ret_val = '\0';

   /*************************/
    /*   Process input key   */
   /*************************/
    switch (in_key) {

    case NDI_ZAID:		/*  NDI_ZAID  */
	/* Check that isotope is set */
	strncpy(ret_val, h->handle_map[0]->iso_stat.zaid, str_size);
	break;

      /**************  UNKNOWN KEYWORD  *****************/

    default:
	ierr = -4;
	break;
    }

    return (ierr);
}
