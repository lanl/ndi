
/*
 * File:	ndi_read_float_block.c
 * Description:	Internal function--read a given number of floats.
 * Time-stamp:	<2000-02-01 10:02:26 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_var_type.h"


/**********************************************************************/

#ifdef FLOAT32
int  ndi_read_float32_block(FILE *stream, long file_pos, FLOAT_TYPE *flt_vec, 
			  const int num_floats) 
#else
int  ndi_read_float64_block(FILE *stream, long file_pos, FLOAT_TYPE *flt_vec, 
			   const int num_floats)
#endif
/*********************************************************************
    
    Purpose:  Read from the current stream 'num_floats' floating-point
    numbers.

    Parameters: 
    stream -- in: file to read from (open library file).
    flt_vec -- out: floating point array containing values read from stream.
    num_floats -- in: number of floating-point values to read from stream.

    Return values:  
    Returns negative value for error condition, positive value for 
    warning, and a '0' if everything's ok.  flt_vec points to array of
    values read in.

***********************************************************************/

{
   int float_count = 0;          /* Current number of floats processed */
   int ierr = 0;      /* Error flag: 0=no error,pos=warning, neg=error */
   char in_line[132 + 1];        /* Current input line */
   char *token;                  /* Current data item */
   char seps[] = " ,\t\n";       /* Data delimiters in library file */
   FLOAT_TYPE *cur_flt_ptr = flt_vec;
  

   /* Go to start of data, read line and count floats */
   /*  Check for data not available */
   fseek( stream, file_pos, SEEK_SET );
   while (float_count < num_floats) {
      if (fgets(in_line, sizeof(in_line), stream) != NULL) {
/*	 printf("\n>>%s\n", in_line);  */
	 token  = strtok( in_line, seps );
	 while (token != NULL) {
	    if (float_count < num_floats) {
	       *cur_flt_ptr = (FLOAT_TYPE) strtod( token, NULL);

/* 	       if ( ( line_cnt == 6) || ( junk_count % 16 == 0) ) { */
/* 		  printf("\n"); */
/* 		  line_cnt = 0; */
/* 	       } */
/* 	       line_cnt++; */
/* 	       junk_count++; */
/* 	       printf("%11.5e   ", *cur_flt_ptr ); */


	       token = strtok( NULL, seps );
	       float_count++;
	       cur_flt_ptr++;
	    } else {
	       /* More data than expected */
	       ierr = 6;
	       break;
	    }
	 }
      } else {
	 /* Error reading file */
	 ierr = -11;
	 return(ierr);
      }
   }

   return(ierr);
}
