/*
 * File: ndi_var_type.h
 * Description: Internal header--defines float type. */

/**********************************************************************/

#ifndef __NDI_VAR_TYPE_H
#define __NDI_VAR_TYPE_H

#ifdef FLOAT32
#define FLOAT_TYPE ndi_float32
#define GET_PN_DOWN_FULL ndi_get_pn_down_full32
#define GET_FULL_PN_MAT ndi_get_full_pn_mat32
#define NDI_CALC_E_AVG ndi_calc_e_avg32
#define NDI_CALC_E_N ndi_calc_e_n32
#define NDI_CALC_E_AVAIL ndi_calc_e_avail32
#define NDI_DIAG2FULL ndi_diag2full32
#define NDI_FULL2DIAG ndi_full2diag32
#define NDI_TRANSPOSE_FLOAT ndi_transpose_float32
#define NDI_COLLAPSE_FLOAT ndi_collapse_float32
#define NDI_COLLAPSE_MAT ndi_collapse_mat32
#define NDI_COLLAPSE_PROD ndi_collapse_prod32
#define NDI_READ_FLOAT_BLOCK ndi_read_float32_block
#define NDI_VEC2MAT ndi_vec2mat32
#define NDI_GET_FLOAT_VAL ndi_c_get_float32_val
#define NDI_GET_FLOAT_VAL_N ndi_c_get_float32_val_n
#define NDI_GET_FLOAT_VEC ndi_c_get_float32_vec
#define NDI_GET_FLOAT_VEC_N ndi_c_get_float32_vec_n
#define NDI_GET_FLOAT_VEC_X ndi_c_get_float32_vec_x
#define GET_FLOAT_VAL_MG ndi_get_float32_val_mg
#define GET_FLOAT_VAL_TN ndi_get_float32_val_tn
#define GET_FLOAT_VAL_DOS ndi_get_float32_val_dos
#define GET_FLOAT_VEC_MG ndi_get_float32_vec_mg
#define GET_FLOAT_VEC_TN ndi_get_float32_vec_tn
#define GET_EDIST_VEC ndi_get_edist_vec32
#define READ_FLOAT_PAIR ndi_read_float32_pair
#define NDI_LIN_LIN_COLLAPSE ndi_lin_lin_collapse32
#define NDI_CALC_E_AVG ndi_calc_e_avg32
#define GET_SIMPLE_FLOAT_VEC ndi_get_simple_float32_vec
#define NDI_GET_HEATING ndi_get_heating32
#define NDI_GET_KERMA ndi_get_kerma32
#define MULT_FLOAT_VEC ndi_mult_float32_vec
#define DIVIDE_FLOAT_VEC ndi_divide_float32_vec
#define GET_CHI_MAT ndi_get_chi_mat32
#define SIG_TOT_TRANS_CORR ndi_sig_tot_trans_corr32
#define PN_DOWN_FULL_TRANS_CORR ndi_pn_down_full_trans_corr32
#define PN_FULL_TRANS_CORR ndi_pn_full_trans_corr32
#define GET_TRANSFER_MAT ndi_get_transfer_mat32
#define NDI_INIT_FLOAT_VEC ndi_init_float32_vec
#define GET_SIG_REAC ndi_get_sig_reac32
#define GET_DEPL_XS ndi_get_depl_xs32
#define GET_PROD_REAC ndi_get_prod_reac32
#define GET_PROD_Q ndi_get_prod_q32
#define GET_PN_PROD_FULL ndi_get_pn_prod_full32
#define GET_DIAG_CORR ndi_get_diag_corr32
#define COPY_MATRIX ndi_copy_matrix32
#define NDI_GET_CHI_VEC  ndi_get_chi_vec32
#define NDID_ZAID_FLOAT_VEC  ndid_c_zaid_float32_vec
#define HEAP_SORT  ndi_heap_sort32
#define NORM_FLOAT_VEC  ndi_norm_float32_vec
#define GET_ABUNDANCE ndi_get_abundance32
#define CHI_MAT_TO_VEC ndi_chi_mat_to_vec32
#define CALC_CHI_WGT ndi_calc_chi_wgt32
#define CALC_EAVG_OUT  ndi_eavg_out32
#define BUILD_FLOAT_TREE ndi_build_tree_float32
#define COLLECT_FLOAT_VAL ndi_collect_f32val
#define CREATE_FLOAT_NODE ndi_create_f32node
#else
#define FLOAT_TYPE ndi_float64
#define GET_PN_DOWN_FULL ndi_get_pn_down_full64
#define GET_FULL_PN_MAT ndi_get_full_pn_mat64
#define NDI_CALC_E_AVG ndi_calc_e_avg64
#define NDI_CALC_E_N ndi_calc_e_n64
#define NDI_CALC_E_AVAIL ndi_calc_e_avail64
#define NDI_DIAG2FULL ndi_diag2full64
#define NDI_FULL2DIAG ndi_full2diag64
#define NDI_TRANSPOSE_FLOAT ndi_transpose_float64
#define NDI_COLLAPSE_FLOAT ndi_collapse_float64
#define NDI_COLLAPSE_MAT ndi_collapse_mat64
#define NDI_COLLAPSE_PROD ndi_collapse_prod64
#define NDI_READ_FLOAT_BLOCK ndi_read_float64_block
#define NDI_VEC2MAT ndi_vec2mat64
#define NDI_GET_FLOAT_VAL ndi_c_get_float64_val
#define NDI_GET_FLOAT_VAL_N ndi_c_get_float64_val_n
#define NDI_GET_FLOAT_VEC ndi_c_get_float64_vec
#define NDI_GET_FLOAT_VEC_N ndi_c_get_float64_vec_n
#define NDI_GET_FLOAT_VEC_X ndi_c_get_float64_vec_x
#define GET_FLOAT_VAL_MG ndi_get_float64_val_mg
#define GET_FLOAT_VAL_TN ndi_get_float64_val_tn
#define GET_FLOAT_VAL_DOS ndi_get_float64_val_dos
#define GET_FLOAT_VEC_MG ndi_get_float64_vec_mg
#define GET_FLOAT_VEC_TN ndi_get_float64_vec_tn
#define GET_EDIST_VEC ndi_get_edist_vec64
#define READ_FLOAT_PAIR ndi_read_float64_pair
#define NDI_LIN_LIN_COLLAPSE ndi_lin_lin_collapse64
#define NDI_CALC_E_AVG ndi_calc_e_avg64
#define GET_SIMPLE_FLOAT_VEC ndi_get_simple_float64_vec
#define NDI_GET_HEATING ndi_get_heating64
#define NDI_GET_KERMA ndi_get_kerma64
#define MULT_FLOAT_VEC ndi_mult_float64_vec
#define DIVIDE_FLOAT_VEC ndi_divide_float64_vec
#define GET_CHI_MAT ndi_get_chi_mat64
#define SIG_TOT_TRANS_CORR ndi_sig_tot_trans_corr64
#define PN_DOWN_FULL_TRANS_CORR ndi_pn_down_full_trans_corr64
#define PN_FULL_TRANS_CORR ndi_pn_full_trans_corr64
#define GET_SIG_REAC ndi_get_sig_reac64
#define GET_DEPL_XS ndi_get_depl_xs64
#define GET_PROD_REAC ndi_get_prod_reac64
#define GET_PROD_Q ndi_get_prod_q64
#define GET_PN_PROD_FULL_P ndi_get_pn_prod_full64
#define GET_TRANSFER_MAT gget_transfer_mat64
#define NDI_INIT_FLOAT_VEC ndi_init_float64_vec
#define GET_DIAG_CORR ndi_get_diag_corr64
#define COPY_MATRIX ndi_copy_matrix64
#define NDI_GET_CHI_VEC  ndi_get_chi_vec64
#define NDID_ZAID_FLOAT_VEC  ndid_c_zaid_float64_vec
#define HEAP_SORT  ndi_heap_sort64
#define NORM_FLOAT_VEC  ndi_norm_float64_vec
#define GET_ABUNDANCE ndi_get_abundance64
#define CHI_MAT_TO_VEC ndi_chi_mat_to_vec64
#define CALC_CHI_WGT ndi_calc_chi_wgt64
#define CALC_EAVG_OUT  ndi_eavg_out64
#define BUILD_FLOAT_TREE ndi_build_tree_float64
#define COLLECT_FLOAT_VAL ndi_collect_f64val
#define CREATE_FLOAT_NODE ndi_create_f64node
#endif


typedef float ndi_float32;
typedef double ndi_float64;

/**********************************************************************/

#endif	/* __NDI_VAR_TYPE_H */

