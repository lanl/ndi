
/*
 * File:	ndi_read_string_block.c
 * Description:	Internal function--read a fixed number of strings.
 * Time-stamp:	<1998-12-18 17:48:09 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"


/**********************************************************************/


int ndi_read_string_block(FILE *stream, const int num_strings, char **p)

/*  Purpose: Skip over a specifed (num_strings) number of floating-point 
    numbers.

    Parameters:
    stream -- in:pointer to current open file.
    num_strings -- in:number of strings to skip over.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/
{
    char in_line[MAX_IN_STR_LEN];	/* Current input line */
    char *token;		/* Current data item */
    char seps[] = " ,\t\n";	/* Data delimiters in library file */
    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */
    int string_count = 0;	/* Current number of strings processed */


    /* Read next line and count strings */
    while (string_count < num_strings) {
	if (fgets(in_line, sizeof(in_line), stream) != NULL) {
	    token = strtok(in_line, seps);
	    while (token != NULL) {
		if (string_count < num_strings) {
		    p[string_count] =
			(char *) ndi_malloc((strlen(token) + 1) *
					    sizeof(char));
		    strcpy(p[string_count], token);
		    token = strtok(NULL, seps);
		    string_count++;
		} else {
		    /* More data than expected */
		    ierr = 6;
		    break;
		}
	    }
	} else {
	    /* Error reading file */
	    ierr = -11;
	    break;
	}
    }
    return (ierr);
}
