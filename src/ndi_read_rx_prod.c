
/*
 * File:	ndi_read_rx_prod.c
 * Description:	Internal function--read reaction product data.
 * Time-stamp:	<2000-02-01 17:37:33 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"


/**********************************************************************/

int ndi_read_rx_prod(FILE *stream, long file_pos, int **int_vec,
		     const char *data_type, int *num_prod, short mt_flag)

/*  Purpose: Return an integer array which is an element of
    a matrix.
    
    Parameters: 
    stream -- in:library file pointer.
    file_pos -- in:absolute offset of data in file (for use with fseek).
    int_vec -- out:returned array of integers (length of num_prod).
    data_type -- in:reaction data to return--"id" or "mult"(multiplicity).
    num_prod -- in/out:number of reactions products.
    mt_flag -- in:0=MT given in library(tn data), -1=mt given(multigroup). 

    Assumptions:
    Input lines have maximum 132-characters. Reaction id and multiplicity
    come in pairs--id listed first.  File pointer points to first ID.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

    Add: if num_prod >= 0 then check this value against the num_prod_in.

***********************************************************************/

{
   char in_line[132 + 1];    /* Line of input data (132-char maximum) */
   char *tok_str;            /*  String being parsed (input line) */
   char *token;              /*  Current value from string being parsed */
   char seps[] = " ,\t\n";   /* Data delimiters--space, comma,tab, newline */
   int ierr = 0;        /* Error flag: 0=no error,pos=warning, neg=error */
   int int_count = 0;        /* Count of integer values found from input */
   int curr_mt;              /* MT of current reaction */
   int num_prod_in;          /* Number of products for current reaction*/
   int *curr_int_ptr = *int_vec;



   /* Check input parameters */
   if ( (strncmp(data_type, "id", 3 ) != 0) &&
	(strncmp(data_type, "mult", 5 ) != 0) ) {
      /* Internal error: invalid input parameter. */
      return(-13);
   }

   /**********************************************/
   /*    Read reaction data from library file    */
   /**********************************************/
   fseek( stream, file_pos, SEEK_SET );

   /* Read MT and number of reaction products */
   if ( mt_flag ) {
      if (fscanf(stream, "%d\n%d\n", &curr_mt, &num_prod_in) == EOF) {
	 /* Error reading file--reaction product data not read */
	 return (-11);
      }
   } else {
      if (fscanf(stream, "%d\n", &num_prod_in) == EOF) {
	 /* Error reading file--reaction product data not read */
	 return (-11);
      }
   }

   /* if num_prod_in <= 0, there's no data */
   if ( num_prod_in <= 0 ) {
     if ( num_prod_in < 0 ) {
       ierr = 9;
     }
     return(ierr);
   }

   if (fgets(in_line, sizeof(in_line), stream) == NULL) {
      /* Error reading file */
      ierr = -11;
   }
   tok_str = in_line;

   /* check that the num_prod is equal to num_prod_in */

   if ( *num_prod >= 0 ) {
     if ( *num_prod != num_prod_in ) {
       /* error and do what? */
       *num_prod = num_prod_in;
     }
   } else {
       *num_prod = num_prod_in;
       *int_vec = (int *)ndi_malloc( num_prod_in * sizeof(int) );
       curr_int_ptr = *int_vec;
   }
     
   while (int_count < num_prod_in) {
    
      /*  Read reaction product id from string  */
      if ((token = strtok(tok_str, seps)) != NULL) {
	 tok_str = NULL; /* ==>Next 'strtok' scans remaining characters */
	 if (strncmp(data_type, "id", 3 ) == 0) {
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
	 }      
      }    /*  if ((token=... */
    
      /*  Read reaction product multiplicity  */
      if ((token = strtok(tok_str, seps)) != NULL) {
	 if (strncmp(data_type, "mult", 5 ) == 0) {
	    *curr_int_ptr = (int) strtol(token, NULL, 0 );
	    /*** Test returned value of strtol--unexpected data -12 ***/
	    curr_int_ptr++;
	    int_count++;
	 }
      } else {
	 /* Error: multiplicity not found. */
	 ierr = -12;
      }      /*  if ((token=... */
   }     /*  while  */

   return(ierr);
}
