
/*
 * File:	ndi_open_gendir.c
 * Description:	Internal function--open the gendir file.
 * Time-stamp:	<1999-01-14 08:38:28 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"


/**********************************************************************

  Purpose:  Look in the appropriate places for the gendir file and
  open it for reading.

  Parameter:
  dir_stream -- out:FILE ptr to the beginning of the gendir file.

  Return value: Will return a negative value on error, a positive
  value for warning, and zero if everything is ok.

**********************************************************************/
int ndi_open_gendir(FILE **dir_stream, const char *in_path, 
                    char **open_path)
{
    int ierr = 0;
    char dir_file[MAX_IN_PATH_LEN];

    if (in_path != NULL && strcmp(in_path, "")) {
	/* Look first for the user specified gendir, if any */
	strcpy(dir_file, in_path);
    } else {
	/*  Look next for a local gendir, if any */
	strcpy(dir_file, "gendir");
    }

    if ((*dir_stream = fopen(dir_file, "rb")) == NULL) {

	/* Then check for directory file given 
	   by environment variable NDI_GENDIR_PATH */
	if (getenv("NDI_GENDIR_PATH")) {
	    strcpy(dir_file, getenv("NDI_GENDIR_PATH"));
	}
	if ((*dir_stream = fopen(dir_file, "rb")) == NULL) {

            /* Check for the default file on the X-div LAN */
            strcpy(dir_file,
                   "/usr/local/codes/data/nuclear/ndi/"
                   NDI_VERSION_DATA "/share/gendir");
            if ((*dir_stream = fopen(dir_file, "rb")) == NULL) {
                
                /* Check for the default file on the sgi */
                strcpy(dir_file,
                       "/usr/projects/data/nuclear/ndi/"
                       NDI_VERSION_DATA "/share/gendir");
                if ((*dir_stream = fopen(dir_file, "rb")) == NULL) {
                    
                    /* Check for the default file on the secure crays */
                    strcpy(dir_file,
                           "/usr/local/udata/ndi/" NDI_VERSION_DATA
                           "/share/gendir");
                    if ((*dir_stream = fopen(dir_file, "rb")) == NULL) {
                        
                        /* Check for the default file on LLNL asci white */
                        strcpy(dir_file,
                               "/usr/gapps/lanl-data/nuclear/ndi/"
                               NDI_VERSION_DATA "/share/gendir");
                        if ((*dir_stream =
                             fopen(dir_file, "rb")) == NULL) {
                            
                            /* Error: cannot open directory file. */
                            ierr = -1;
                        }
                    }
                }
            }
        }
    }

    if (open_path) {
        *open_path = NULL;
        if (ierr >= 0) {
            if ((*open_path = 
                 (char *)ndi_malloc((strlen(dir_file) + 1) * sizeof(char)))) {
                strncpy(*open_path, dir_file, strlen(dir_file) + 1);
            } else {
                ierr = -14;
            }
        }
    } else {
        /*  ???  */
        if (ierr >= 0)
            ierr = 7;
    }

    return (ierr);
}

/**********************************************************************/
