
/*
 * File:	ndi_open_gendir.c
 * Description:	Internal function--open the gendir file.
 * Time-stamp:	<1999-01-14 08:38:28 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ndi.h"

/*include ndi_internal.h only for ndi_malloc and ndi_free functions */
#include "ndi_internal.h"

#include "ndi_state.h"

/**********************************************************************/
int ndi_open_gendir(FILE **dir_stream, char *path)

/**********************************************************************

  Purpose:  Look in the appropriate places for the gendir file and
  open it for reading.

  Parameter:
  dir_stream -- out:FILE ptr to the beginning of the gendir file.

  Return value: Will return a negative value on error, a positive
  value for warning, and zero if everything is ok.

**********************************************************************/
{
  
  int ierr = 0;
  char *dir_file;
  int str_size;
  
  /* Open directory file and initialize state variables */
  dir_file = (char *)ndi_malloc(250 * sizeof(char));

  if (path != NULL && strcmp(path, "")) {
      strcpy(dir_file, path);
  } else {
      /*  Look first for directory file "gendir" in current dir */
      strcpy( dir_file, "gendir");  
  }

  if ( (*dir_stream = fopen(dir_file, "rb") ) == NULL ) {

      /* Then check for directory file given 
	 by environment variable NDI_GENDIR_PATH */
      if (getenv("NDI_GENDIR_PATH")) {
	  strcpy( dir_file, getenv("NDI_GENDIR_PATH") );
      }
      if ( (*dir_stream = fopen(dir_file, "rb") ) == NULL ) {

	  /* Then check for directory file given 
	     by environment variable SNXSPATH */
	  if (getenv("SNXSPATH")) {
	      strcpy( dir_file, getenv("SNXSPATH") );
	      strcat( dir_file, "/gendir" );
	  }
	  if ( (*dir_stream = fopen(dir_file, "rb") ) == NULL ) {
	
	      /* Check for file /usr/local/codes/data/sn/gendir
		 ---  the standard file on the X-div LAN ----    */
	      strcpy( dir_file, "/usr/local/codes/data/nuclear/ndi/" NDI_VERSION_DATA "/share/gendir" ); 
	      if ( ( *dir_stream = fopen( dir_file, "rb" ) ) == NULL ) {
	  
		  /* Check for file /usr/projects/data/nuclear/ndi/gendir
		     ---  the standard file on the sgi open and secure ----    */
		  strcpy( dir_file, "/usr/projects/data/nuclear/ndi/" NDI_VERSION_DATA "/share/gendir" ); 
		  if ( ( *dir_stream = fopen( dir_file, "rb" ) ) == NULL ) {
	    
		      /* Check for file /usr/local/udata/ndi/gendir
			 ---  the standard file on the secure crays ----    */
		      strcpy( dir_file, "/usr/local/udata/ndi/" NDI_VERSION_DATA "/share/gendir" ); 
		      if ( ( *dir_stream = fopen( dir_file, "rb" ) ) == NULL ) {
			  
			  /* Check for file /usr/gapps/lanl-data/nuclear/ndi/gendir
			     ---  the standard file on LLNL asci white ----    */
			  strcpy( dir_file, "/usr/gapps/lanl-data/nuclear/ndi/" NDI_VERSION_DATA "/share/gendir" ); 
			  if ( ( *dir_stream = fopen( dir_file, "rb" ) ) == NULL ) {
			      
			      /* Error: cannot open directory file. */
			      ierr = -1;
			  }
		      }
		  }
	      }
	  }
      }
  }

  strcpy(path, dir_file);
  ndi_free((void**)&dir_file);
  return(ierr);

}
  
/**********************************************************************/
