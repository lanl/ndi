
/*
 * File:	ndid_get_string_vec.c
 * Description:	User function--return list of multigroup-neutron zaids.
 * Time-stamp:	<2000-02-03 15:21:57 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ndi.h"
#include "ndi_internal.h"

#include "ndi_bin_tree.h"

/**********************************************************************/

int
ndid_c_get_string_vec(int in_key, const char *in_value,
		      char *ret_str_arr, int arr_size)

/*  Purpose: 
    
    Parameters: 

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/
{

    int ierr = 0;
    struct ndi_bin_tree_node *root = NULL;
    FILE *ndi_dir;
    char *path = NULL;

    /* Open gendir */
    ierr = ndi_open_gendir(&ndi_dir, "", &path);
    if (path)
        ndi_free((void **) &path);

    if (ierr < 0) {
	root = NULL;
    } else {
	 root = ndi_build_tree(ndi_dir, in_key, in_value);
	 fclose(ndi_dir);
    }

    /* Collect the strings into the return vec */
    if (ierr >= 0) {
	ierr = ndi_collect_keys(root, ret_str_arr, arr_size);
    }

    ndi_close_tree(&root);
    return (ierr);

}

/***********************************************************************/
