#include <stdio.h>
#include <string.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"


/**********************************************************************/

int ndimp_set_zaid(state_struct *handle[], const int num_parts,
                   const char *zaid )
{

    int ierr = 0;
    int tmp_err = 0;
    int i;
    int z, a;
    char *dot_p = NULL;
    char *tmp_zaid = NULL;
    state_struct *statep;

    /* Check each particle is multigroup */
    /* Check num_parts <= MAX_HANDLE_NUM */

    for (i = 0; i < num_parts; i++ ) {
        if ((statep = (handle[i]))) {
            tmp_err = ndi_c_set_isotope_h(statep, zaid);
            if (tmp_err < 0 ) {
                tmp_zaid = (char *)ndi_malloc(strlen(zaid) + 1
                                              * sizeof(char));
                strncpy(tmp_zaid, zaid, strlen(zaid) + 1);
                /* Strip ZAID ending, if present */
                if ((dot_p = strchr(tmp_zaid, '.' ))) {
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
                sprintf(tmp_zaid, "%d", z*1000);
                tmp_err = ndi_c_set_isotope_h(statep, tmp_zaid);
                /* check err */
                ndi_free((void **) &tmp_zaid);
            }
            if ( (tmp_err != 0) &&
                 ((ierr >= 0) || (tmp_err < 0 )))
                ierr = tmp_err;
        } else {
            ierr = -2;
            break;
        }
    }

    return (ierr);

}

/**********************************************************************/
