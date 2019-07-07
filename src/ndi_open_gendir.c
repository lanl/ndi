/*
 *
 * $Id$
 * Description:	Internal function--open the gendir file.
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
static FILE *fpath(const char *rel_path, const char *mode,
		   char *file, int len);

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
    char *def_file = "$NDIROOT/" NDI_VERSION_DATA "/share/gendir";

    int ierr = 0;
    char dir_file[MAX_IN_PATH_LEN];


    *dir_stream = NULL;

    if (in_path != NULL && !local_isblank(in_path)) {	/* in_path set */

	/* Try in_path */
	*dir_stream = fpath(in_path, "rb", dir_file, MAX_IN_PATH_LEN);
	if (*dir_stream == NULL)
	    ierr = 11;
    }

    if (*dir_stream == NULL) {

	/* Try "gendir" */
	*dir_stream = fpath("gendir", "rb", dir_file, MAX_IN_PATH_LEN);
    }

    if (*dir_stream == NULL && getenv("NDI_GENDIR_PATH")) {
	/* Try NDI_GENDIR_PATH */
	*dir_stream = fpath(getenv("NDI_GENDIR_PATH"), "rb",
			    dir_file, MAX_IN_PATH_LEN);
	if (*dir_stream == NULL)
	    ierr = (ierr) ? 11 : 12;
    }

    if (*dir_stream == NULL) {
	/* Try default file */
	*dir_stream = fpath(def_file, "rb", dir_file, MAX_IN_PATH_LEN);
    }

    if (*dir_stream == NULL) {
	ierr = -1;
    } else {
	if (open_path) {
	    *open_path = NULL;
	    if (ierr >= 0) {
		if ((*open_path =
		     (char *) ndi_malloc((strlen(dir_file) + 1) *
					 sizeof(char)))) {
                    strcpy(*open_path, dir_file);
		} else {
		    ierr = -14;	/* Out of Memory */
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
    for (i = 0; isspace(str[i]); i++);	/* skip leading whitespace */
    return str[i] == '\0';
}

/**********************************************************************/

/* 
 * if rel_path starts with prefix:
 *    Try to open abs_path[i]+rel_path with mode
 * else
 *    Try to open rel_path with mode
 *
 * Set file to path and return file pointer upon success or
 * Set file to "" and return NULL if no sucess.
 *
 */
FILE *fpath(const char *rel_path, const char *mode, char *file, int len)
{
    char *abs_path[] = { "/opt/local/codes/data/nuclear/ndi/",
    "/usr/local/codes/data/nuclear/ndi/",
	"/usr/projects/data/nuclear/ndi/",
	"/usr/gapps/lanl-data/nuclear/ndi/",
	"/projects/lanl-data/nuclear/ndi/",
	""
    };

    char prefix[] = "$NDIROOT/"; /* Abbrev for abs_path */
    char prefix2[] = "$NDIHOME/"; /* "$NDIROOT/" NDI_VERSION_DATA "share/" */

    FILE *f;
    int i;

    if (strncmp(rel_path, prefix, strlen(prefix)) == 0) {
	for (i = 0; strcmp(abs_path[i], ""); i++) {
	    strncpy(file, abs_path[i], len);
	    strncat(file, rel_path + strlen(prefix), len);
	    if ((f = fopen(file, mode)))
		return f;	/* success */
	}
        /* Reqt. 87*/
    } else if (strncmp(rel_path, prefix2, strlen(prefix2)) == 0) {
        for (i = 0; strcmp(abs_path[i], ""); i++) {
            strncpy(file, abs_path[i], len);
            strncat(file, NDI_VERSION_DATA "/share/", len);
            strncat(file, rel_path + strlen(prefix2), len);
	    if ((f = fopen(file, mode)))
		return f;	/* success */
        }
    } else {
	strncpy(file, rel_path, len);
	if ((f = fopen(file, mode)))
	    return f;		/* success */
    }

    strncpy(file, "", len);
    return NULL;		/* file not found */
}

/**********************************************************************/
