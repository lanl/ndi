
/*
 * File:	ndi_get_string_val.c
 * Description:	User function--return a string value.
 * Time-stamp:	<1999-11-18 18:19:39 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_bin_tree.h"

static int get_string_vec_gendir(state_struct *state, int in_key,
				 char *str_arr, int arr_size);

/**********************************************************************/

int ndi_c_get_string_vec_h(state_struct *state, int in_key, char *str_arr,
			   int arr_size)

/*  Purpose: Return a string array.
    
    Parameters: 
    in_key -- in: key to specify type of data required
    str_vec -- out: string value returned
    arr_size -- in: tells how large the string is (how many characters)

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/
{
    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */

    if ((str_arr) && (arr_size > 0)) {
	*str_arr = '\0';
    } else {
	ierr = 7;
    }

    /* Check that ndi_init has been called. */
    if ((!state) || (!state->ndi_initialized)) {
	return -2;
    }
    if (is_gendir(state)) {
	ierr = get_string_vec_gendir(state, in_key, str_arr, arr_size);
    } else if ((state->library_type == 'm') ||
	       (state->library_type == 'd')) {
	ierr = -2;
    } else if (state->library_type == 's') {
	ierr = -2;
    } else if (state->library_type == 'p') {
	ierr = -2;
    } else if (state->library_type == ' ') {
	/* Library type was not set */
	ierr = -16;
    } else {
	/* Incompatible library type  -- bad input key */
	ierr = -4;
    }
    return ierr;

}

/**********************************************************************/
/**********************************************************************/
static int get_string_vec_gendir(state_struct *state, int in_key,
				 char *str_arr, int arr_size)
{
    int ierr = 0;
    struct ndi_bin_tree_node *root = NULL;
    FILE *ndi_dir;
    char *path = NULL;
    char *in_value;

    if (!state) {		/* check state sanity */
	return -2;
    }

    in_value = ndi_get_section(state);
    if (in_value == 0) {
	return -16;		/* Library type not set */
    }

    /* Open gendir */
    ierr = ndi_open_gendir(&ndi_dir, state->open_gendir_file, &path);
    if (path)
	ndi_free((void **) &path);

    if (ierr < 0) {
	root = NULL;
    } else {
	root = ndi_build_tree(ndi_dir, in_key, in_value);
	fclose(ndi_dir);
    }
    /* Collect the floats into the return vec */
    if (ierr >= 0) {
	ierr = ndi_collect_keys(root, str_arr, arr_size);
    }
    ndi_close_tree(&root);
    return ierr;
}

/**********************************************************************/
/**********************************************************************/
