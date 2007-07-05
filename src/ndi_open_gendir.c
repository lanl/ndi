/*
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
                   unsigned int flag, char *file, int len);

enum { PLAIN = 01, PREFIX = 02 };

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
    char *def_file = NDI_VERSION_DATA "/share/gendir";

    int ierr = 0;
    char dir_file[MAX_IN_PATH_LEN];

    if (in_path != NULL && !local_isblank(in_path)) {
	/* in_path set */
	if ((*dir_stream = fpath(in_path, "rb", PLAIN | PREFIX, 
                                 dir_file, MAX_IN_PATH_LEN)) == NULL) {
	    /* in_path file unreadable */
	    ierr = 11;
	    if ((*dir_stream = 
                 fpath("gendir", "rb", PLAIN, 
                       dir_file, MAX_IN_PATH_LEN)) == NULL) {
		/* gendir unreadable */
		if (getenv("NDI_GENDIR_PATH")) {
                     if ((*dir_stream = fpath(getenv("NDI_GENDIR_PATH"), "rb",
                                              PLAIN | PREFIX, 
                                              dir_file, MAX_IN_PATH_LEN)) ==
                          NULL) {
			/* NDI_GENDIR_PATH file unreadable */
			ierr = 11;
		    }
		}
	    }		
	}
    } else {
	if ((*dir_stream = fpath("gendir", "rb", PLAIN, 
                                 dir_file, MAX_IN_PATH_LEN)) == NULL) {
	    /* gendir unreadable */
	    if (getenv("NDI_GENDIR_PATH")) {
                 if ((*dir_stream = fpath(getenv("NDI_GENDIR_PATH"), "rb", 
                                          PLAIN | PREFIX,
                                         dir_file, MAX_IN_PATH_LEN)) == 
                     NULL) {
		    /* NDI_GENDIR_PATH file unreadable */
		    ierr = 12;
	        }
	    }		
	}
    }

    if (*dir_stream == NULL) {
	/* Check for the default file */
         *dir_stream = fpath(def_file, "rb", PREFIX, 
                             dir_file, MAX_IN_PATH_LEN);
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
		    ierr = -14; /* Out of Memory */
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

/* 
 * Try to open rel_path with mode if flag PLAIN set, then
 * try to open abs_path[i]+rel_path with mode if flag PREFIX is set.
 *
 * set file to path and return file pointer upon success or
 * set file to "" and return NULL if no sucess.
 *
 */
FILE *fpath(const char *rel_path, const char *mode, 
            unsigned int flag, char *file, int len)
{
     char *abs_path[] = {"/usr/local/codes/data/nuclear/ndi/",
                         "/usr/projects/data/nuclear/ndi/",
                         "/usr/gapps/lanl-data/nuclear/ndi/",
                         "/projects/lanl-data/nuclear/ndi/",
                         ""};

     FILE *f;
     int i;

     if (flag & PLAIN) {            /* try relative path first */
          strncpy(file, rel_path, len);
          f = fopen(file, mode);
          if (f) return f;      /* success */
     }

     if ((flag & PREFIX) && rel_path[0] != '/') { /* try absolute paths */
          for (i = 0; strcmp(abs_path[i], ""); i++) {
               strncpy(file, abs_path[i], len);
               strncat(file, rel_path, len);
               f = fopen(file, mode);
               if (f) return f;      /* success */
          }
     }

     strncpy(file, "", len);
     return NULL;               /* file not found */
}
/**********************************************************************/
