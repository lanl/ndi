
/*
 * File:	ndi_zaid_dir.c
 * Description:	Internal function--lookup zaid in gendir file.
 * Time-stamp:	<2000-02-04 16:27:34 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static void ndi_find_zaid_string( char *in_line, int in_size, 
				  char *zaid_string, int string_size, 
				  char *seps );
/*
static void ndi_find_first_string( char *in_line, int in_size, 
				   char *first_string, int string_size, 
				   char *seps );
*/

/**********************************************************************/

int ndi_zaid_dir(char pt_type, char l_type, const char *in_zaid, 
		 char *real_zaid, char *library, char *lib_path, 
		 char *lib_file_type, long int *iso_start, double *at_wgt, 
		 int reaction_flag )

/*  Purpose: 
    
    Parameters: 
    pt_type -- in: incident particle type (e.g. 'n' neutron)
    l_type -- in: library type (e.g. 'm' multigroup)
    in_zaid -- in: zaid we're searching for on gendir
    real_zaid -- out: full zaid found on gendir
    library -- out: library (e.g. mendf5) read from gendir
    lib_path -- out: full pathname of library file read from gendir
    lib_file_type -- out: file type (asc) read from gendir
    iso_start -- out: byte offset in lib file of start of data (read from gendir)
    at_wgt -- out: atomic weight read from gendir
    reaction_flag -- Are we reading reaction data--depletion (True = -1, false=0)

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

    Notes:  If the in_zaid is the empty string, this will match any zaid. So
    if the first zaid is wanted, the empty string can be passed in.  However,
    this means that if the in_zaid is a substring of a zaid (i.e. the in_zaid
    is the first n characters of a zaid in the gendir) the zaids will match.
    THIS SHOULD BE FIXED.  It is fixed in the next release.

***********************************************************************/

{

   char section[30]; /* Gendir section (e.g. neutron_multigroup) */
   size_t sec_len;   /* Length of 'section' string */
   char in_line[MAX_IN_STR_LEN];  /* Current input line */
   char zaid_string[MAX_IN_STR_LEN];  /* First string on current line */
   char seps[] = " ,\t\n";
   int ierr = 0;     /* Error flag: 0=no error,pos=warning, neg=error */
   int done = 0;     /* Flag indicating when processing of zaid is done */
   char tmp_zaid[MAX_IN_STR_LEN];
   dir_entry dir_rec;


   /* Check if library type is set */
     
   if ( ( pt_type == 'n' ) && ( l_type == 'm' ) ) {
     if ( reaction_flag == 0 ) {
       strcpy( section, "multigroup_neutron" );
     } else {
       strcpy( section, "neutron_depletion" );
     }
   }else if ( ( pt_type == 'n' ) && ( l_type == 'd' ) ) {
     if ( reaction_flag == 0 ) {
       strcpy( section, "dosimetry_neutron" );
     } else {
       strcpy( section, "radchem_chain" );
     }
   } else if ( (pt_type == 'p') && ( l_type == 'm' ) ) {
      strcpy( section, "multigroup_photon" );
   } else if ( (pt_type == '0') && ( l_type == 's' ) ) {
      strcpy( section, "tndata" );
   } else if ( ( pt_type == '0' ) && ( l_type == 'r' ) ) {
      strcpy( section, "tn_reactions" );
      /*
   } else if ( ( pt_type == 'n' ) && ( l_type == '0' ) ) {
      strcpy( section, "neutron_depletion" );
      */
   } else {
      /* Error: library type not set */
      return(-16);
   }

   if ( ! ndi_dir ) {
     /* Error opening file */
     return( -1 );
   }

   (void)rewind(ndi_dir); 
   sec_len = strlen(section);
   
   while (done == 0) { 
      
      /* Find correct section of directory file */
      if (fgets(in_line, sizeof(in_line), ndi_dir) != NULL) {
	 if (strncmp(in_line, section, sec_len) == 0) {
	    
	    while ( (done == 0) && ( ierr >= 0 ) ) {
	       
	       /* Find correct zaid within section */
	       if (fgets(in_line, sizeof(in_line), ndi_dir) != NULL) {
		  
		  /* Check if current input line contains the desired zaid */
		 if ( strstr( in_line, "z=" ) ) {
		   (void) ndi_find_zaid_string( in_line, MAX_IN_STR_LEN, zaid_string, 
						MAX_IN_STR_LEN, seps );
		   if ( strncmp( zaid_string, in_zaid, strlen(in_zaid) ) == 0 ) {
		     ierr = read_dir_block( in_line, ndi_dir, &dir_rec );
		     strcpy( tmp_zaid, dir_rec.zaid );
		     strcpy( library, dir_rec.lib );
		     strcpy( lib_path, dir_rec.path );
		     strcpy( lib_file_type, dir_rec.lib_type );
		     *iso_start = dir_rec.offset;
		     *at_wgt = dir_rec.at_wgt;
		     
		     /*  Check current library against user-default library */
		     if ( ( reaction_flag != 0 ) || (
			 /* Chain libraries currently are undefined */
			 ( strchr(in_zaid, '.') != NULL ) &&
			  (isalnum( *(strchr(in_zaid, '.') + 1) ) ) ) ) {
			 /* A library is implied in the ZAID */
			 done = -1;
		     } else {
			 if ( strcmp(library_vers, "") != 0 ) {
			     if ( strcmp( library_vers, library ) == 0 ) {
				 /* Current library matches user-specified default */
				 done = -1;
			     }
			 } else {
			     /* No default library specified by user */
			     done = -1;
			 }
		     }
		   } /* strncmp*/
		     
		 } else if ( strncmp(in_line, "end", 3 ) == 0 ) {
		     /* Isotope not found in gendir */
		     ierr = -8;
		     done = -1;
		 } 
	       } else {
		   /* Error reading file */
		   ierr = -11;
		   done = -1;
	       }
	    }   /* while */ 
	 }      /* strncmp */
      } else {
	  /* Error reading file */
	  ierr = -11;
	  done = -1;
      }
   }  /* while */

   if ( ierr >= 0 ) {
     strcpy( real_zaid, tmp_zaid );
   }
   return(ierr);
}
	    
	    
/**********************************************************************/
/*   FUNCTIONS INTERNAL TO FILE  */
/**********************************************************************/

static void ndi_find_zaid_string( char *in_line, int in_size, 
				  char *zaid_string, int string_size, 
				  char *seps ) {
  
  /* This function assumes that the string is terminated by '\0'.
     There is no other test to prevent the function from going
     right off the end of the string.

     Also assumes that zaid is of the form z=92235 (no spaces).
     */

   char *curr_char = NULL;
   char *target_char = zaid_string;
   char *tmpstr;
   int i;
   int str_length;
   int curr_size = in_size;
   int target_count = 0;

   *zaid_string = '\0';

   if ( ( curr_char = strstr( in_line, "z=" ) ) ) {
     /* Start of zaid should be two ( 'z' and '=') past curr_char */
     curr_char += 2;
     str_length = (int)( curr_char - in_line );
     curr_size = in_size - str_length/((int)sizeof(char));
     if ( curr_size <= 1 ) {
       return;
     }

     for ( i = 0; i < curr_size; i++ ) {
       tmpstr = strchr( seps, *curr_char );       
       if ( ( *curr_char != '\0' ) && 
	    ( tmpstr == NULL ) &&
	    ( target_count < string_size ) ) {
	 /* The current character is not is the seperation characters */
	 *target_char = *curr_char;
	 target_char++;
	 target_count++;
	 curr_char++;
       } else {
	 *target_char = '\0';
	 break;
       }
     }
   }

   return;
}

/***********************************************************************/
/***********************************************************************/
/*
static void ndi_find_first_string( char *in_line, int in_size, 
				   char *first_string, int string_size, 
				   char *seps ) {
   int i;
   int data_start = 0;
   int target_count = 0;
   char *curr_char = in_line;
   char *target_char = first_string;
   char *junk = NULL;

   for ( i = 0; i < in_size; i++ ) {
     junk = strchr( seps, *curr_char );
     if ( ( junk == NULL ) &&
	  ( target_count < string_size ) ) {
	  * Go past beginning white space *
	 if ( data_start == 0 ) {
	    data_start = -1;
	 }
	 *target_char = *curr_char;
	 target_char++;
	 target_count++;
	 curr_char++;
	 * Check that we haven't gone off the end of the string *
      } else {
	 if ( data_start != 0 ) {
	    *target_char = '\0';
	    break;
	 }
	 curr_char++;
      }
   }
   * what if i == in_size? *

   return;
}
*/
/***********************************************************************/
/***********************************************************************/
