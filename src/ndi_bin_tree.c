
/*
 * File:	ndi_bin_tree.c
 * Description:	Functions for manipulating a binary tree
 * Time-stamp:	<2000-02-02 17:57:16 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_bin_tree.h"

/**********************************************************************/

/***********************************************************************/

struct ndi_bin_tree_node *ndi_build_tree(FILE *ndi_dir, int in_key,
					 const char *in_value)
{
    /* Gendir section (e.g. neutron_multigroup) */
    char section[50];
    size_t sec_len;		/* Length of 'section' string */
    char in_line[MAX_IN_STR_LEN];	/* Current input line */
    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */
    int done = 0;		/* Flag indicating when processing of zaid is done */

    dir_entry mg_dir;
    char *str_of_interest;
    struct ndi_bin_tree_node *root = NULL;
    struct ndi_bin_tree_node *curr_node;

    strncpy(section, in_value, 50);
    sec_len = (int) strlen(section);

    while (done == 0) {

	/* Find correct section of directory file */
	if (fgets(in_line, sizeof(in_line), ndi_dir) != NULL) {
	    if (strncmp(in_line, section, sec_len) == 0) {

		/* Read each line in this section */
		while ((done == 0) && (ierr >= 0)) {

		    if (fgets(in_line, sizeof(in_line), ndi_dir) != NULL) {
			if (strncmp(in_line, "end", 3) != 0) {
			    ierr =
				read_dir_block(in_line, ndi_dir, &mg_dir);
			    if (ierr >= 0) {
				/* Add value to array */
				switch (in_key) {

				case NDI_ZAID:
				    str_of_interest =
					(char *) &(mg_dir.zaid);
				    break;

				default:
				    /* Invalid key */
				    return (NULL);
				}
				curr_node =
				    ndi_create_node(str_of_interest);
				ndi_insert_node(&root, curr_node);

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
		}		/* while */
	    }
	} else {
	    /* Error reading file */
	    ierr = -11;
	    done = -1;
	}
    }

    return (root);
}

/***********************************************************************/
/***********************************************************************/

struct ndi_bin_tree_node *ndi_create_node(char *instr)
{
    int instr_len = (int) strlen(instr) + 1;
    struct ndi_bin_tree_node *tmp_node = NULL;

    tmp_node = (struct ndi_bin_tree_node *)
	malloc(sizeof(struct ndi_bin_tree_node));
    tmp_node->key = (char *) malloc(instr_len * sizeof(char));
    strncpy(tmp_node->key, instr, instr_len);
    tmp_node->left = NULL;
    tmp_node->right = NULL;
    return (tmp_node);

}


struct ndi_bin_tree_node *ndi_create_inode(char *instr, int int_val)
{
    int instr_len = (int) strlen(instr) + 1;
    struct ndi_bin_tree_node *tmp_node = NULL;

    tmp_node = (struct ndi_bin_tree_node *)
	ndi_malloc(sizeof(struct ndi_bin_tree_node));
    tmp_node->key = (char *) ndi_malloc(instr_len * sizeof(char));
    strncpy(tmp_node->key, instr, instr_len);
    tmp_node->value.ival = int_val;
    tmp_node->left = NULL;
    tmp_node->right = NULL;
    return (tmp_node);

}

struct ndi_bin_tree_node *ndi_create_f32node(char *instr,
					     ndi_float32 ndi_float32_val)
{
    int instr_len = (int) strlen(instr) + 1;
    struct ndi_bin_tree_node *tmp_node = NULL;

    tmp_node = (struct ndi_bin_tree_node *)
	ndi_malloc(sizeof(struct ndi_bin_tree_node));
    tmp_node->key = (char *) ndi_malloc(instr_len * sizeof(char));
    strncpy(tmp_node->key, instr, instr_len);
    tmp_node->value.f32val = ndi_float32_val;
    tmp_node->left = NULL;
    tmp_node->right = NULL;
    return (tmp_node);

}

struct ndi_bin_tree_node *ndi_create_f64node(char *instr,
					     ndi_float64 float64_val)
{
    int instr_len = (int) strlen(instr) + 1;
    struct ndi_bin_tree_node *tmp_node = NULL;

    tmp_node = (struct ndi_bin_tree_node *)
	ndi_malloc(sizeof(struct ndi_bin_tree_node));
    tmp_node->key = (char *) ndi_malloc(instr_len * sizeof(char));
    strncpy(tmp_node->key, instr, instr_len);
    tmp_node->value.f64val = float64_val;
    tmp_node->left = NULL;
    tmp_node->right = NULL;
    return (tmp_node);

}

void ndi_insert_node(struct ndi_bin_tree_node **root_p,
		struct ndi_bin_tree_node *new_node)
{

    struct ndi_bin_tree_node **curr_node = root_p;

    while ((new_node != NULL) && (*curr_node != NULL)) {
	if (ndi_comp_zaids(new_node->key, (*curr_node)->key) < 0) {
/*     if ( strcmp( new_node->key, (*curr_node)->key ) < 0 ) { */
	    curr_node = &(*curr_node)->left;
/*     } else if ( strcmp( new_node->key,(*curr_node)->key ) > 0 ) { */
	} else if (ndi_comp_zaids(new_node->key, (*curr_node)->key) > 0) {
	    curr_node = &(*curr_node)->right;
	} else {

	    if (new_node->key) {
		free(new_node->key);
	    }
	    free((void *) new_node);
	    new_node = NULL;
	}
    }
    if (new_node) {
	*curr_node = new_node;
    }
}

int ndi_collect_keys(struct ndi_bin_tree_node *root, char *key_array_p,
		 int arr_size_p)
{
    /* Top level for collect keys, so that input parameters from top-level are not changed */

    int ierr = 0;
    char *tmp_array_p = key_array_p;
    int tmp_size_p = arr_size_p;

    ierr = _ndi_collect_keys(root, &tmp_array_p, &tmp_size_p);
    return (ierr);
}

int _ndi_collect_keys(struct ndi_bin_tree_node *root, char **key_array_p,
		  int *arr_size_p)
{

    int curr_size = 0;
    int ierr = 0;

    if (root != NULL) {
	/* Handle left branch -- less than */
	ierr = _ndi_collect_keys(root->left, key_array_p, arr_size_p);

	/* Handle current key */
	curr_size = (int) strlen(root->key) + 1;
	if (curr_size > *arr_size_p) {
	    curr_size = *arr_size_p;
	    ierr = 7;
	}
	strncpy(*key_array_p, root->key, curr_size);
	*arr_size_p -= (curr_size) * sizeof(char);
	*key_array_p += curr_size;

	/* Handle right branch -- greater than */
	ierr = _ndi_collect_keys(root->right, key_array_p, arr_size_p);
    }
    return (ierr);
}


int ndi_collect_ival(struct ndi_bin_tree_node *root, int *int_array_p,
		 int arr_size)
{
    /* Top level for collect ivals, so that input parameters from top-level are not changed */

    int ierr = 0;
    int tmp_size = arr_size;
    int *tmp_p = int_array_p;

    ierr = _ndi_collect_ival(root, &tmp_p, &tmp_size);
    return (ierr);
}

int _ndi_collect_ival(struct ndi_bin_tree_node *root, int **int_array_p,
		  int *arr_size_p)
{

    int ierr = 0;
    int *int_p;

    if (root != NULL) {
	/* Handle left branch -- less than */
	ierr = _ndi_collect_ival(root->left, int_array_p, arr_size_p);

	int_p = *int_array_p;
	/* Handle current int */
	if (*arr_size_p <= 0) {
	    ierr = 7;
	} else {
	    *int_p = root->value.ival;
	    *arr_size_p -= 1;
	    *int_array_p += 1;
	}

	/* Handle right branch -- greater than */
	ierr = _ndi_collect_ival(root->right, int_array_p, arr_size_p);
    }
    return (ierr);
}


int ndi_collect_f32val(struct ndi_bin_tree_node *root,
		   ndi_float32 * float32_array_p, int arr_size)
{
    /* Top level for collect f32vals, so that input parameters from top-level are not changed */

    int ierr = 0;
    int tmp_size = arr_size;
    ndi_float32 *tmp_p = float32_array_p;

    ierr = _ndi_collect_f32val(root, &tmp_p, &tmp_size);
    return (ierr);
}

int _ndi_collect_f32val(struct ndi_bin_tree_node *root,
		    ndi_float32 ** float32_array_p, int *arr_size_p)
{

    int ierr = 0;
    ndi_float32 *float32_p;

    if (root != NULL) {
	/* Handle left branch -- less than */
	ierr =
	    _ndi_collect_f32val(root->left, float32_array_p, arr_size_p);

	float32_p = *float32_array_p;
	/* Handle current float32 */
	if (*arr_size_p <= 0) {
	    ierr = 7;
	} else {
	    *float32_p = root->value.f32val;
	    *arr_size_p -= 1;
	    *float32_array_p += 1;
	}

	/* Handle right branch -- greater than */
	ierr =
	    _ndi_collect_f32val(root->right, float32_array_p, arr_size_p);
    }
    return (ierr);
}


int ndi_collect_f64val(struct ndi_bin_tree_node *root,
		   ndi_float64 * float64_array_p, int arr_size)
{
    /* Top level for collect f64vals, so that input parameters from top-level are not changed */

    int ierr = 0;
    int tmp_size = arr_size;
    ndi_float64 *tmp_p = float64_array_p;

    ierr = _ndi_collect_f64val(root, &tmp_p, &tmp_size);
    return (ierr);
}

int _ndi_collect_f64val(struct ndi_bin_tree_node *root,
		    ndi_float64 ** float64_array_p, int *arr_size_p)
{

    int ierr = 0;
    ndi_float64 *float64_p;

    if (root != NULL) {
	/* Handle left branch -- less than */
	ierr =
	    _ndi_collect_f64val(root->left, float64_array_p, arr_size_p);

	float64_p = *float64_array_p;
	/* Handle current float64 */
	if (*arr_size_p <= 0) {
	    ierr = 7;
	} else {
	    *float64_p = root->value.f64val;
	    *arr_size_p -= 1;
	    *float64_array_p += 1;
	}

	/* Handle right branch -- greater than */
	ierr =
	    _ndi_collect_f64val(root->right, float64_array_p, arr_size_p);
    }
    return (ierr);
}

int ndi_count_nodes(struct ndi_bin_tree_node *root, int *arr_size_p)
{

    *arr_size_p = 0;
    return (_ndi_count_nodes(root, arr_size_p));
}

int _ndi_count_nodes(struct ndi_bin_tree_node *root, int *arr_size_p)
{

    /* NOTE:  currently on the first call from the main function,
     *arr_size_p must be initialized to zero.
     */

    int ierr = 0;
    if (root != NULL) {
	/* Handle left branch -- less than */
	ierr = _ndi_count_nodes(root->left, arr_size_p);

	/* Handle current key */
	(*arr_size_p)++;

	/* Handle right branch -- greater than */
	ierr = _ndi_count_nodes(root->right, arr_size_p);
    }
    return (ierr);
}

int ndi_count_size(struct ndi_bin_tree_node *root, int *arr_size_p)
{

    *arr_size_p = 0;
    return (_ndi_count_size(root, arr_size_p));
}

int _ndi_count_size(struct ndi_bin_tree_node *root, int *arr_size_p)
{

    /* NOTE:  currently on the first call from the main function,
     *arr_size_p must be initialized to zero.
     */

    int ierr = 0;
    if (root != NULL) {
	/* Handle left branch -- less than */
	ierr = _ndi_count_size(root->left, arr_size_p);

	/* Handle current key */
	*arr_size_p += (int) strlen(root->key) + 1;

	/* Handle right branch -- greater than */
	ierr = _ndi_count_size(root->right, arr_size_p);
    }
    return (ierr);
}

int ndi_close_tree(struct ndi_bin_tree_node **root)
{

    int ierr = 0;
    if (*root != NULL) {
	struct ndi_bin_tree_node *tmp_node = *root;

	/* Handle left branch -- less than */
	ierr = ndi_close_tree(&(tmp_node->left));

	/* Handle right branch -- greater than */
	ierr = ndi_close_tree(&(tmp_node->right));

	/* Handle current node */
	if (tmp_node->key) {
	    free(tmp_node->key);
	}

	if (tmp_node) {
	    free(tmp_node);
	    *root = NULL;
	}

    }
    return (ierr);
}


/***********************************************************************/
/***********************************************************************/
