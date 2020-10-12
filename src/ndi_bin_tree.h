/*
 * File:        ndi_bin_tree.h
 * Description: Header file for libndi
 * Time-stamp:  <2000-02-03 11:12:37 jomc>
 *
 * This header file is included in internal library code only.
 */

/**********************************************************************/

#ifndef NDI_BIN_TREE_H
#define NDI_BIN_TREE_H

/**********************************************************************/


/*
 *             DATA TYPES
 */


#include "ndi_var_type.h"

struct ndi_bin_tree_node {
    struct ndi_bin_tree_node *left;
    struct ndi_bin_tree_node *right;
    char *key;
    union {
        int ival;
        ndi_float32 f32val;
        ndi_float64 f64val;
        char *sval;
    } value;
};



/*
 *             FUNCTION PROTOTYPES
 */

#ifdef __cplusplus
extern "C" {
#endif

    struct ndi_bin_tree_node *ndi_build_tree_float32(FILE *ndi_dir, int in_key,
                                               const char *in_value);
    struct ndi_bin_tree_node *ndi_build_tree_float64(FILE *ndi_dir, int in_key,
                                               const char *in_value);
    struct ndi_bin_tree_node *ndi_build_tree(FILE *ndi_dir, int in_key,
                                             const char *in_value);
    struct ndi_bin_tree_node *ndi_create_node(char *instr);
    struct ndi_bin_tree_node *ndi_create_inode(char *instr, int int_val);
    struct ndi_bin_tree_node *ndi_create_f32node(char *instr,
                                                 ndi_float32
                                                 ndi_float32_val);
    struct ndi_bin_tree_node *ndi_create_f64node(char *instr,
                                                 ndi_float64 float64_val);
    void ndi_insert_node(struct ndi_bin_tree_node **root_p,
                         struct ndi_bin_tree_node *new_node);
    int ndi_collect_keys(struct ndi_bin_tree_node *root, char *key_array_p,
                         int arr_size_p);
    int ndi_collect_ival(struct ndi_bin_tree_node *root, int *int_array_p,
                         int arr_size);
    int ndi_collect_f32val(struct ndi_bin_tree_node *root,
                           ndi_float32 * float32_array_p, int arr_size);
    int ndi_collect_f64val(struct ndi_bin_tree_node *root,
                           ndi_float64 * float64_array_p, int arr_size);
    int _ndi_collect_keys(struct ndi_bin_tree_node *root,
                          char **key_array_p, int *arr_size_p);
    int _ndi_collect_ival(struct ndi_bin_tree_node *root,
                          int **int_array_p, int *arr_size_p);
    int _ndi_collect_f32val(struct ndi_bin_tree_node *root,
                            ndi_float32 ** float32_array_p,
                            int *arr_size_p);
    int _ndi_collect_f64val(struct ndi_bin_tree_node *root,
                            ndi_float64 ** float64_array_p,
                            int *arr_size_p);
    int ndi_count_nodes(struct ndi_bin_tree_node *root, int *arr_size_p);
    int _ndi_count_nodes(struct ndi_bin_tree_node *root, int *arr_size_p);
    int ndi_count_size(struct ndi_bin_tree_node *root, int *arr_size_p);
    int _ndi_count_size(struct ndi_bin_tree_node *root, int *arr_size_p);
    int ndi_close_tree(struct ndi_bin_tree_node **root);


#endif                                /* NDI_BIN_TREE_H */
