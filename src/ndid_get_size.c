
/*
 * File:	ndid_get_size.c
 * Description:	User function--return size (in bytes) of return value.
 * Time-stamp:	<2000-02-03 15:00:51 jomc>
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

#include "ndi_bin_tree.h"


/*  Purpose: 
    
    Parameters: 

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/
int ndid_c_get_num(int in_key, const char *in_value, int *arr_num)
{
    int ierr = 0;
    struct ndi_bin_tree_node *root = NULL;

    /* Build a binary tree with ZAID as key */
    root = ndi_build_tree(NDI_ZAID, in_value);
    ierr = ndi_count_nodes(root, arr_num);
    ndi_close_tree(&root);
    return (ierr);

}

/**********************************************************************/

/*  Purpose: 
    
    Parameters: 

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/
int ndid_c_get_size(int in_key, const char *in_value, int *arr_size)
{
    int ierr = 0;
    struct ndi_bin_tree_node *root = NULL;

    /* Build a binary tree with ZAID as key */
    root = ndi_build_tree(NDI_ZAID, in_value);
    ierr = ndi_count_size(root, arr_size);
    ndi_close_tree(&root);
    return (ierr);

}

/**********************************************************************/
/*   FUNCTIONS INTERNAL TO FILE  */
/**********************************************************************/
