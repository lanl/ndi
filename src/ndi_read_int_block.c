
/*
 * File:	ndi_read_int_block.c
 * Description:	Internal function--read a given number of ints from file.
 * Time-stamp:	<2003-01-12 16:02:26 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"


/**********************************************************************/

int
ndi_read_int_block(FILE *stream, const int num_ints,
		   long *return_vec, const int return_size)

/*********************************************************************
    
    Purpose:  Read from the current stream 'num_ints' integer numbers.

    Parameters: 
    stream -- in: file to read from (open library file positioned at start of block).
    return_vec -- out: long integer array containing values read from stream.
    num_ints -- in: number of integer values to read from stream.
    return_size -- in: size of the return_vec array.

    Return values:  
    Returns negative value for error condition, positive value for 
    warning, and a '0' if everything's ok.  return_vec points to array of
    values read in.

***********************************************************************/
{
    int int_count = 0;		/* Current number of ints processed */
    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */
    char in_line[MAX_IN_STR_LEN];	/* Current input line */
    char *token;		/* Current data item */
    char seps[] = " ,\t\n";	/* Data delimiters in library file */
    long *cur_int_ptr = return_vec;


    /* current position in stream is assumed to be at the start of int block */

    while (int_count < num_ints) {

	/* Check to see if we've reached the end of return_vec */
	if (int_count >= return_size) {
	    ierr = 7;
	    break;
	}

	if (fgets(in_line, sizeof(in_line), stream) != NULL) {
	    token = strtok(in_line, seps);
	    while (token != NULL) {
		if (int_count < num_ints) {
		    *cur_int_ptr = strtol(token, (char **) NULL, 10);
		    token = strtok(NULL, seps);
		    int_count++;
		    cur_int_ptr++;
		} else {
		    /* More data than expected */
		    ierr = 6;
		    break;
		}
	    }
	} else {
	    /* Error reading file */
	    ierr = -11;
	    return (ierr);
	}
    }

    return (ierr);
}
