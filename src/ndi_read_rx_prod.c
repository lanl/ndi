
/*
 * $Id$
 * Description: Internal function--read reaction product data.
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"


/**********************************************************************/


/*  Purpose: Read a block of data which consists of an integer specifying
    the number of pairs to follow (num), optionally an integer (representing
    an MT number), and then 'num' pairs of integers.  If the option MT is
    present, it must be on the line immediately following 'num'.
      e.g.
         ( with MT -- usually representing old-style reaction products)
      rprod_all
         2
         2
         1    1    1001    1

         ( without MT -- representing new-style rprods/reactants or interp params )
      rprods
         2
         1  1  2004  1
    
    Parameters: 
    stream -- in:library file pointer.
    file_pos -- in:absolute offset of data in file (for use with fseek).
    int_vec -- out:returned array of integers (length of num_prod).
    data_type -- in:return vec of first "0" or second "1" element of pairs.
    num_prod -- in/out:number of reactions products.
    mt_flag -- in:0=no MT number , -1=MT number present(multigroup). 

    Assumptions:
    Input lines have maximum MAX_IN_STR_LEN characters. Reaction id and 
    multiplicity come in pairs--id listed first.  File pointer points to 
    first ID.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

    Add: if num_prod >= 0 then check this value against the num_prod_in.

***********************************************************************/

int
ndi_read_rx_prod(FILE *stream, long file_pos, int **int_vec,
                 const int data_type, int *num_prod, short mt_flag)
{
    char in_line[MAX_IN_STR_LEN]; /* Line of input data (132-char maximum) */
    char *tok_str;                /*  String being parsed (input line) */
    char *token;                  /*  Current value from string being parsed */
    char seps[] = " ,\t\n";       /* Data delimiters--space, comma,tab, newline */
    int ierr = 0;                 /* Error flag: 0=no error,pos=warning, neg=error */
    int int_count = 0;            /* Count of integer values found from input */
    int curr_mt;                  /* MT of current reaction */
    int num_prod_in;              /* Number of products for current reaction */
    int *curr_int_ptr = *int_vec;


    /**********************************************/
    /*    Read reaction data from library file    */
    /**********************************************/
    fseek(stream, file_pos, SEEK_SET);

    /* Read MT and number of reaction products */
    if (mt_flag) {
        if (fscanf(stream, "%d\n%d\n", &curr_mt, &num_prod_in) == EOF) {
            /* Error reading file--reaction product data not read */
            return -11;
        }
    } else {
        if (fscanf(stream, "%d\n", &num_prod_in) == EOF) {
            /* Error reading file--reaction product data not read */
            return -11;
        }
    }

    /* if num_prod_in <= 0, there's no data */
    if (num_prod_in <= 0) {
        if (num_prod_in < 0) {
            ierr = 9;
        }
        return ierr;
    }

    if (fgets(in_line, sizeof(in_line), stream) == NULL) {
        /* Error reading file */
        ierr = -11;
        return ierr;
    }
    tok_str = in_line;

    /* check that the num_prod is equal to num_prod_in */

    if (*num_prod >= 0) {
        if (*num_prod != num_prod_in) {
            /* error and do what? */
            *num_prod = num_prod_in;
        }
    } else {
        *num_prod = num_prod_in;
        *int_vec = (int *) ndi_malloc(num_prod_in * sizeof(int));
        curr_int_ptr = *int_vec;
    }

    while (int_count < num_prod_in) {

        /*  Read reaction id, rprod id, or interpolation boudaries,
           i.e. first element of each pair  */
        if ((token = strtok(tok_str, seps)) != NULL) {
            tok_str = NULL;        /* ==>Next 'strtok' scans remaining characters */
            if (data_type == 0) {
                *curr_int_ptr = (int) strtol(token, NULL, 0);
            /*** Test returned value of strtol--unexpected data -12 ***/
                curr_int_ptr++;
                int_count++;
            }
        } else {
            /*  Reaction product info continued on next line -- read next line  */
            if (fgets(in_line, sizeof(in_line), stream) != NULL) {
                tok_str = in_line;
                continue;
            } else {
                /* Error: unexpected data (end) */
                ierr = -12;
                return ierr;
            }
        }                        /*  if ((token=... */

        /*  Read reaction product multiplicity or interpolation type
           i.e. second element of each pair  */
        if ((token = strtok(tok_str, seps)) != NULL) {
            if (data_type == 1) {
                *curr_int_ptr = (int) strtol(token, NULL, 0);
            /*** Test returned value of strtol--unexpected data -12 ***/
                curr_int_ptr++;
                int_count++;
            }
        } else {
            /* Error: multiplicity not found. */
            ierr = -12;
            break;
        }                        /*  if ((token=... */
    }                                /*  while  */

    return ierr;
}

/*********************************************************************/


/* ndi_read_rx_prod2 -- read real reaction multiplicities from library file
**
**  Purpose: Read a block of data which consists of an integer specifying
**  the number of pairs to follow (num), optionally an integer (representing
**  an MT number), and then 'num' pairs of integers,doubles.  If the option 
**  MT is present, it must be on the line immediately following 'num'.
**    e.g.
**       ( with MT -- usually representing old-style reaction products)
**    rprod_all
**         2
**         2
**         1    1.0    1001    1.0
**
**         ( without MT -- representing new-style rprods/reactants or interp 
**         params )
**    rprods
**         2
**         1  1.0  2004  1.0
**  
**  Parameters: 
**  stream -- in:library file pointer.
**  file_pos -- in:absolute offset of data in file (for use with fseek).
**  vec -- out:returned array of doubles (length of num_prod).
**  num_prod -- in/out:number of reactions products.
**  mt_flag -- in:0=no MT number , -1=MT number present(multigroup). 
**
**  Assumptions:
**  Input lines have maximum MAX_IN_STR_LEN characters. Reaction id and 
**  multiplicity come in pairs--id listed first.  File pointer points to 
**  first ID.
**
**  Return values:  Returns negative value for error condition,
**  positive value for warning, and a '0' if everything's ok. 
**
**  Add: if num_prod >= 0 then check this value against the num_prod_in.
*/
int
ndi_read_rx_prod2(FILE *stream, long file_pos, double **vec,
                  const int data_type, int *num_prod, short mt_flag)
{
    char in_line[MAX_IN_STR_LEN]; /* Line of input data (132-char maximum) */
    char *tok_str;                /* String being parsed (input line) */
    char *token;                  /* Current value from string being parsed */
    char seps[] = " ,\t\n";       /* Data delimiters--space,comma,tab,newline */

    int count;                    /* Count of integer values found from input */
    int curr_mt;                  /* MT of current reaction */
    int num_prod_in;              /* Number of products for current reaction */
    double *curr_ptr = *vec;      /* Return data */

    /* Check input parameters */
    if (data_type != MULT_PARAM)
        return -13;

    /* Go to data block */
    fseek(stream, file_pos, SEEK_SET);

    /* Read optional MT ID */
    if (mt_flag) {
        if (fscanf(stream, "%d\n", &curr_mt) == EOF)
            return -11;
    }

    /* Read number of reaction products */
    if (fscanf(stream, "%d\n", &num_prod_in) == EOF)
        return -11;

    /* if num_prod_in <= 0 then there's no data */
    if (num_prod_in < 0)
        return 9;                 /* Warn: bad data on file */
    if (num_prod_in == 0)
        return 0;

    /* allocate return vector (bad style!) */
    *num_prod = num_prod_in;
    *vec = (double *) ndi_malloc(num_prod_in * sizeof(double));
    curr_ptr = *vec;

    count = 0;
    do {

        /* read line */
        if (fgets(in_line, sizeof(in_line), stream) == NULL)
            return -11;
        tok_str = in_line;

        /* parse line */
        for (token = strtok(tok_str, seps); token != NULL; token = strtok(NULL, seps)) {        /* first of pair */

            if ((token = strtok(NULL, seps)) != NULL) {        /* second of pair */
                *curr_ptr++ = strtod(token, NULL);
                if (++count == num_prod_in)
                    break;
            } else {
                return -12;       /* Error: second of pair not found. */
            }
        }

    } while (count < num_prod_in);

    return 0;
}
