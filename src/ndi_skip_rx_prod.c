
/*
 * File:	ndi_skip_rx_prod.c
 * Description:	Internal function--skip over reaction products in file.
 * Time-stamp:	<1998-11-03 15:45:39 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"


/**********************************************************************/

int ndi_skip_rx_prod(FILE *stream, stat_flag * rx_prod, short mt_flag,
		     int binary)

/*  Purpose:  Skip over reaction product data in library file.

    Paramaters:
    stream -- in:pointer to current open file.
    rx_prod -- out:struct containing reaction product info.
    mt_flag -- in:0=MT not given in library(tn data), -1=mt given(multigroup). 

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 
    Changes rx_prod.length to be the number of reaction products
    and rx_prod.file_ptr to point to beginning of reaction product data.

***********************************************************************/
{
    char in_line[MAX_IN_STR_LEN];	/* Current input line */
    char *tok_str = NULL;	/* Input string being parsed */
    char *token;		/* Current data item */
    char seps[] = " ,\t\n";	/* Data delimiters in library file */
    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */
    int num_prod;		/* Number of products for current reaction */
    int curr_mt;		/* MT of current reaction */
    int prod_count;		/* Running total of products processed */
    int num;                    /* Miscellaneous read in integer */

    /* Read reaction mt and number of reaction products, then
       set reaction product state. */
    /*  Assumption:  reaction product format:
	Reaction MT, Number of Products, [ID quantity]x# of products */

    if ((rx_prod->file_ptr = ftell(stream)) == -1L) {
	/* Error setting pointer */
	ierr = -19;
    }

    /* Read MT and number of reaction products */
    if (mt_flag) {
	if (binary) {
	    if (fread(&curr_mt, sizeof(int), 1, stream) == EOF) {
	        /* Error reading file--reaction product data not read */
	        return (-11);
	    }
            if (fread(&num_prod, sizeof(int), 1, stream) == EOF) {
                /* Error reading file--reaction product data not read */
                return (-11);
            }
	} else {
            if (fscanf(stream, "%d\n%d\n", &curr_mt, &num_prod) == EOF) {
                /* Error reading file--reaction product data not read */
                return (-11);
            }
	}
    } else {
	if (binary) {
	    if (fread(&num_prod, sizeof(int), 1, stream) == EOF) {
	        /* Error reading file--reaction product data not read */
	        return (-11);
	    }
	} else {
            if (fscanf(stream, "%d\n", &num_prod) == EOF) {
                /* Error reading file--reaction product data not read */
                return (-11);
            }
	}
    }

    rx_prod->length = num_prod;

    /*  Skip through rx product data to leave file stream at beginning
        of next reaction or at next keyword  */

    /*  Num_prod = 0   ---> no more data to read for this reaction  */
    if (num_prod > 0) {
	if (binary) {
	    /* Cycle through products. Each product has a particle ID and number
	       of particles released for this reaction */
	    for (int i = 0; i < num_prod; ++i) {
	        /* Product ID */
	        if (fread(&num, sizeof(int), 1, stream) == EOF) {
		    return (-11);
	        }
	        /* Number of product */
                if (fread(&num, sizeof(int), 1, stream) == EOF) {
                    return (-11);
                }
	    }
	} else {
	    if (fgets(in_line, sizeof(in_line), stream) == NULL) {
                /* Error reading file */
                return (-11);
            }
            tok_str = in_line;

	    prod_count = 0;
	    while (prod_count < num_prod) {
        	/*  Read reaction product id from string  */
        	token = strtok(tok_str, seps);
        	while (token == NULL) {
            	    if (fgets(in_line, sizeof(in_line), stream) != NULL) {
                	tok_str = in_line;
                	token = strtok(tok_str, seps);
            	    } else {
                	/* Error reading file */
                	return (-11);
            	    }
        	}
        	tok_str = NULL;

        	/*  Read reaction product multiplicity  */
        	token = strtok(tok_str, seps);
        	while (token == NULL) {
            	    if (fgets(in_line, sizeof(in_line), stream) != NULL) {
                	tok_str = in_line;
                	token = strtok(tok_str, seps);
                	tok_str = NULL;
            	    } else {
                        /* Error reading file */
                        return (-11);
                    }
                }
                ++prod_count;
            }
	}
    }

    return (ierr);
}
