
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

static int local_isblank(const char *str);

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
    static char *default_file[] = {"/usr/local/codes/data/nuclear/ndi/"
				   NDI_VERSION_DATA "/share/gendir",
				   "/usr/projects/data/nuclear/ndi/"
				   NDI_VERSION_DATA "/share/gendir",
				   "/usr/gapps/lanl-data/nuclear/ndi/"
				   NDI_VERSION_DATA "/share/gendir",
				   "/projects/lanl-data/nuclear/ndi/"
				   NDI_VERSION_DATA "/share/gendir",
				   ""};
    int i;

    if (in_path != NULL && !local_isblank(in_path)) {
	/* in_path set */
	strcpy(dir_file, in_path);
	if ((*dir_stream = fopen(dir_file, "rb")) == NULL) {
	    /* in_path file unreadable */
	    ierr = 11;
	    strcpy(dir_file, "gendir");
	    if ((*dir_stream = fopen(dir_file, "rb")) == NULL) {
		/* gendir unreadable */
		if (getenv("NDI_GENDIR_PATH")) {
		    strcpy(dir_file, getenv("NDI_GENDIR_PATH"));
		    if ((*dir_stream = fopen(dir_file, "rb")) == NULL) {
			/* NDI_GENDIR_PATH file unreadable */
			ierr = 11;
		    }
		}
	    }		
	}
    } else {
	strcpy(dir_file, "gendir");
	if ((*dir_stream = fopen(dir_file, "rb")) == NULL) {
	    /* gendir unreadable */
	    if (getenv("NDI_GENDIR_PATH")) {
		strcpy(dir_file, getenv("NDI_GENDIR_PATH"));
		if ((*dir_stream = fopen(dir_file, "rb")) == NULL) {
		    /* NDI_GENDIR_PATH file unreadable */
		    ierr = 12;
	        }
	    }		
	}
    }

    if (*dir_stream == NULL) {
	/* Check for the default file */
	for (i = 0; strcmp(default_file[i], ""); i++) {
	    strcpy(dir_file, default_file[i]);
	    if ((*dir_stream = fopen(dir_file, "rb")) != NULL)
		break;
	}
    }

    if (*dir_stream == NULL) {
	ierr = -1;
    } else {
	if (open_path) {
	    *open_path = NULL;
	    if (ierr >= 0) {
		if ((*open_path = 
		     (char *)ndi_malloc((strlen(dir_file) + 1) * 
					sizeof(char)))) {
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
    }

    return (ierr);
}

static int local_isblank(const char *str)
{

    int i;
    for (i = 0; isspace(str[i]); i++)
	;			/* skip leading whitespace */
    return str[i] == '\0';
}

/**********************************************************************/
