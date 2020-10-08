
/*
 * File:        ndi_skip_float_block.c
 * Description:        Internal function--skip float data to get to next keyword.
 * Time-stamp:        <1998-10-02 16:15:21 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"


/**********************************************************************/


int skip_float_block(FILE *stream, int *bsize, const int num_floats,
                     int binary)

/*  Purpose: Skip over a specifed (num_floats) number of floating-point 
    numbers.

    Parameters:
    stream -- in:pointer to current open file.
    bsize -- out:size (in bytes) of data to be skipped.
    num_floats -- in:number of floats to skip over.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/
{
    char in_line[MAX_IN_STR_LEN];        /* Current input line */
    char *token;                /* Current data item */
    char seps[] = " ,\t\n";        /* Data delimiters in library file */
    int ierr = 0;                /* Error flag: 0=no error,pos=warning, neg=error */
    int float_count = 0;        /* Current number of floats processed */
    double in_float;

    /* Calculate the array size needed to hold the skipped floats. */
    *bsize = num_floats * (int) sizeof(float);

    /* If the data is binary, use fread */
    if ( binary ) {
        /* Read next double and count floats */
        while (float_count < num_floats) {
            if (fread(&in_float, sizeof(double), 1, stream) > 0) {
                if (float_count < num_floats) {
                    float_count++;
                } else {
                    /* More data than expected */
                    ierr = 6;
                    break;
                }
            } else {
                /* Error reading file or end of file */
                ierr = -11;
            }
        }
    /* Otherwise use fgets to get an entire line of ascii data */
    } else {
        /* Read next line and count floats */
        while (float_count < num_floats) {
            if (fgets(in_line, sizeof(in_line), stream) != NULL) {
                token = strtok(in_line, seps);
                /* Work through line to get each float individually */
                while (token != NULL) {
                    if (float_count < num_floats) {
                        token = strtok(NULL, seps);
                        float_count++;
                    } else {
                        /* More data than expected */
                        ierr = 6;
                        break;
                    }
                }
            } else {
                /* Error reading file */
                ierr = -11;
            }
        }
    }
    return ierr;
}
