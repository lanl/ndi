
/*
 * File:	ndi_open_lib_file.c
 * Description:	Internal function--open library file and set global var.
 * Time-stamp:	<1998-10-02 16:19:25 jomc>
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

FILE *ndi_open_lib_file(const char *path) 

/*  Purpose:  Open library file for read-only and set global 
    variable 'open_lib_file' to be the path of the current open
    library file.

    Parameters:  
    path -- string containing path of library file to be opened.

    Return values:  Returns file pointer to open file (returns NULL
    if open fails). Sets global variable 'open_lib_file' to input 
    parameter 'path'.

***********************************************************************/

{
  
   FILE *temp_file_p = NULL;

   temp_file_p = fopen( path, "rb" );

   /*  Question: what to do if 'open_lib_file' can't be set???? */

   /* Set global variable 'open_lib_file' to the pathname of the open file. */
   if (open_lib_file) ndi_free((void**)&open_lib_file);
   if (temp_file_p) {
      if ((open_lib_file = (char *)ndi_malloc(strlen(path) + sizeof(char)))
	  != NULL) {
	 strcpy( open_lib_file, path);
      }
   } else {
      open_lib_file = (char*)ndi_malloc(sizeof(char));
      if (open_lib_file) open_lib_file[0] = '\0';
   }

   return(temp_file_p);
}

/**********************************************************************/

