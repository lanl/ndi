
/*
 * $Id$
 * Description:	Internal function--open library file and set global var.
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

static void dir_name(const char *file, char *path);
static FILE *fpath2(state_struct *state, const char *path, 
		    const char *mode, char *file, int len);

/**********************************************************************

    Purpose:  Open library file for read-only and set global 
    variable 'open_lib_file' to be the path of the current open
    library file.

    Parameters:  
    path -- string containing path of library file to be opened.

    Return values:  Returns file pointer to open file (returns NULL
    if open fails). Sets global variable 'open_lib_file' to input 
    parameter 'path'.

***********************************************************************/

FILE *ndi_open_lib_file(state_struct *state, const char *path)
{

    FILE *temp_file_p = NULL;
    char name[MAX_IN_PATH_LEN];

    if (!state) {
	return (NULL);
    }

    temp_file_p = fpath2(state, path, "rb", name, MAX_IN_PATH_LEN);

    /* Set global variable 'open_lib_file' to the pathname of the open file. */
    if (state->open_lib_file)
	ndi_free((void **) &state->open_lib_file);

    if (temp_file_p) {
	if ((state->open_lib_file =
	     (char *) ndi_malloc(strlen(name) + sizeof(char)))
	    != NULL) {
	    strcpy(state->open_lib_file, name);
	}
    }

    return (temp_file_p);
}

/**********************************************************************/
/**********************************************************************/


FILE *fpath2(state_struct *state, const char *path, 
	     const char *mode, char *name, int len)
{
    char prefix[] = "$GENDIRPATH";

    if (strncmp(path, prefix, strlen(prefix)) == 0) {
	dir_name(state->open_gendir_file, name);
	strncat(name, path+strlen(prefix), len);
    } else {
	strncpy(name, path, len);
    }

    return fopen(name, mode);
}

/**********************************************************************/
/**********************************************************************/

void dir_name(const char *file, char *path)
{
    char *p;

    strcpy(path, file);
    p = path + strlen(path);
    
    while (p > path && p[-1] == '/') p--; /* remove trailing slashes */
    while (p > path && p[-1] != '/') p--; /* remove trailing non-slashes */
    while (p > path && p[-1] == '/') p--; /* remove trainling slashes */

    if (p == path) {
	if (path[0] == '/')
	    strcpy(path, "/");
	else
	    strcpy(path, ".");
    } else {
	*p = '\0';
    }
}

