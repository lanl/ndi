
/*
 * File:	ndid_zaid_float_vec.c
 * Description:	User function--return list of multigroup-neutron zaids and float values.
 * Time-stamp:	<2000-02-03 15:13:59 jomc>
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
#include "ndi_var_type.h"
#include "ndi_bin_tree.h"

static struct ndi_bin_tree_node *BUILD_FLOAT_TREE( int in_key, const char *in_value );

/**********************************************************************/

int NDID_ZAID_FLOAT_VEC(int in_key, const char *in_value, char *zaid_arr, 
			int arr_size, FLOAT_TYPE *float_arr, int fl_arr_size )

/*  Purpose: 
    
    Parameters: 

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/

{

  int ierr = 0;
  struct ndi_bin_tree_node *root = NULL;
  
  root = BUILD_FLOAT_TREE( in_key, in_value );
  
  /* Collect the strings into the return vec */
  if (ierr >= 0 )  {
    ierr = ndi_collect_keys( root, zaid_arr, arr_size );
  }
  
  /* Collect the floats into the return vec */
  if (ierr >= 0 )  {
    ierr = COLLECT_FLOAT_VAL( root, float_arr, fl_arr_size );
  }
  
  ndi_close_tree( &root );
  return(ierr);
  
}


/***********************************************************************/
/***********************************************************************/

static struct ndi_bin_tree_node *BUILD_FLOAT_TREE( int in_key, const char *in_value )
{
   /* Gendir section (e.g. neutron_multigroup) */
   char section[50];
   size_t sec_len;   /* Length of 'section' string */
   char in_line[MAX_IN_STR_LEN];  /* Current input line */
   int ierr = 0;     /* Error flag: 0=no error,pos=warning, neg=error */
   int done = 0;     /* Flag indicating when processing of zaid is done */
   FILE *ndi_dir;
   dir_entry mg_dir;
   char *zaid_p;
   FLOAT_TYPE curr_flt;
   struct ndi_bin_tree_node *root = NULL;
   struct ndi_bin_tree_node *curr_node;
   char path[250];


   /* Open gendir */

   strcpy(path, "");
   ierr = ndi_open_gendir(&ndi_dir, path); 

   if ( ierr < 0 ) {
      return(NULL);
   }
   
   strncpy(section, in_value, 50);
   sec_len = strlen(section);
   
   while (done == 0) { 
      
      /* Find correct section of directory file */
      if (fgets(in_line, sizeof(in_line), ndi_dir) != NULL) {
	 if (strncmp(in_line, section, sec_len) == 0) {
	    
	   /* Read each line in this section */
	    while ( (done == 0) && ( ierr >= 0 ) ) {
	       
	       if (fgets(in_line, sizeof(in_line), ndi_dir) != NULL) {
		  if ( strncmp( in_line, "end", 3 ) != 0 ) {
		    ierr = read_dir_block( in_line, ndi_dir, &mg_dir );
		    if (ierr >= 0) {
		      /* Add value to array */

		      zaid_p = (char *) &(mg_dir.zaid);
		      switch ( in_key ) {
					case NDI_TEMP:
			  curr_flt = (FLOAT_TYPE)mg_dir.temp;
			  break;
			  
			case NDI_SIG_0:
			  curr_flt = (FLOAT_TYPE)mg_dir.sig_0;
			  break;
			  
			case NDI_AT_WGT:
			  curr_flt = (FLOAT_TYPE)mg_dir.at_wgt;
			  break;
			  
			case NDI_AWR:
			  curr_flt = (FLOAT_TYPE)mg_dir.awr;
			  break;
			  
			  /**********  UNKNOWN KEYWORD  ***********/
			  
			default:      
			  ierr = -4;
			  return(NULL);
			  break;
			}	


		      curr_node = CREATE_FLOAT_NODE( zaid_p, curr_flt );
		      ndi_insert_node( &root, curr_node );
		      
		    } else {
		      /* Add a warning???? */
		    } 
		  } else {
		    /* End of section */
		    done = -1;
		  }
	       } else {
		 /* Error reading file */
		 ierr = -11;
		 done = -1;
	       }
	    }   /* while */ 
	 }
      } else  {
	/* Error reading file */
	ierr = -11;
	done = -1;
      }
   }

   fclose(ndi_dir);
   return(root);
}
