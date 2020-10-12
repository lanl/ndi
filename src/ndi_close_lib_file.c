
/*
 * File:        ndi_close_lib_file.c
 * Description: Internal function--close open library file.
 * Time-stamp:  <1998-10-02 16:19:50 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"


/**********************************************************************/

/*  Purpose:  Close library file 'stream' and re-initialize extern 
    variable open_lib_file.

    Parameters: FILE *stream  file pointer to library file

    It is assumed that library file is open and that its pathname is
    given by the open_lib_file variable.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. The 
    external character string pointer open_lib_file is set to the
    end-of-string character.

***********************************************************************/
int ndi_close_lib_file(state_struct *state)
{
    int ret_code = 0;                /* Return code--returned error value */

    if (!state) {
        return -2;
    }

    /* Close library file */
    if (state->lib_stream) {
        if (fclose(state->lib_stream) != 0)
            ret_code = -3;
    }

    /* Deinitialize state */
    if (state->open_lib_file)
        ndi_free((void **) &state->open_lib_file);

    return ret_code;

}

/**********************************************************************/
