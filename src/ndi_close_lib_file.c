
/*
 * File:	ndi_close_lib_file.c
 * Description:	Internal function--close open library file.
 * Time-stamp:	<1998-10-02 16:19:50 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"


/**********************************************************************/

int ndi_close_lib_file(FILE *stream) 

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


{
   int ret_code = 0;       /* Return code--returned error value */

  
   /* Close library file */
   if (stream) {
      if ( fclose(stream) != 0 ) ret_code = -3;
   } else {
      ret_code = 4;
   }


   if (ret_code >= 0) {
    
      /* Deinitialize state */
      if ( open_lib_file ) ndi_free((void**)&open_lib_file);
      open_lib_file = (char*)ndi_malloc(sizeof(char));
      open_lib_file[0] = '\0';
   }
  
   return (ret_code);
 
}


/**********************************************************************/
