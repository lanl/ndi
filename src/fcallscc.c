/*
 * $Id$
 * Description:        Discover Fortran-C name mangling 
 *
 */

#include <stdio.h>

void ndi_print_fmangle_body(void);


void ndi_isitu()
{

    printf("#define NDI_NO_NAME_SUB\n");
    printf("#define LOWER_WITHOUT \n");
    ndi_print_fmangle_body();

}

void ndi_isitu_()
{

    printf("#define NDI_NO_NAME_SUB\n");
    printf("#define LOWER_WITH \n\n\n");
    ndi_print_fmangle_body();

}

void ndi_isitu__()
{

    printf("#define NDI_NO_NAME_SUB\n");
    printf("#define LOWER_WITH2 \n\n\n");
    ndi_print_fmangle_body();

}

void NDI_ISITU()
{

    printf("#define NDI_NO_NAME_SUB\n");
    printf("#define UPPER_WITHOUT \n");
    ndi_print_fmangle_body();

}

void NDI_ISITU_()
{

    printf("#define NDI_NO_NAME_SUB\n");
    printf("#define UPPER_WITH \n");
    ndi_print_fmangle_body();

}


void ndi_print_fmangle_body()
{

    printf("#ifdef UPPER_WITHOUT\n");
    printf("#define F_NDID_GET_SIZE          NDID_GET_SIZE\n");
    printf("#define F_NDID_GET_NUM           NDID_GET_NUM\n");
    printf("#define F_NDID_GET_STRING_VEC    NDID_GET_STRING_VEC\n");
    printf("#define F_NDI_INIT               NDI_INIT\n");
    printf("#define F_NDI_CLOSE              NDI_CLOSE\n");
    printf("#define F_NDI_SET_OPTION         NDI_SET_OPTION\n");
    printf("#define F_NDI_CHECK_OPTION       NDI_CHECK_OPTION\n");
    printf("#define F_NDI_SET_ZAID           NDI_SET_ZAID\n");
    printf("#define F_NDI_SET_ZAID_FLOAT32_TEMP NDI_SET_ZAID_FLOAT32_TEMP\n");
    printf("#define F_NDI_SET_ZAID_FLOAT64_TEMP NDI_SET_ZAID_FLOAT64_TEMP\n");
    printf("#define F_NDI_SET_ZAID_FLOAT32_TEMP_SIG_0 NDI_SET_ZAID_FLOAT32_TEMP_SIG_0\n");
    printf("#define F_NDI_SET_ZAID_FLOAT64_TEMP_SIG_0 NDI_SET_ZAID_FLOAT64_TEMP_SIG_0\n");
    printf("#define F_NDI_SET_ISOTOPE        NDI_SET_ISOTOPE\n");
    printf("#define F_NDI_SET_REACTION       NDI_SET_REACTION\n");
    printf("#define F_NDI_GET_SIZE           NDI_GET_SIZE\n");
    printf("#define F_NDI_GET_SIZE_N         NDI_GET_SIZE_N\n");
    printf("#define F_NDI_GET_SIZE_X         NDI_GET_SIZE_X\n");
    printf("#define F_NDI_GET_INT_VAL        NDI_GET_INT_VAL\n");
    printf("#define F_NDI_GET_INT_VAL_N      NDI_GET_INT_VAL_N\n");
    printf("#define F_NDI_GET_INT_VEC        NDI_GET_INT_VEC\n");
    printf("#define F_NDI_GET_INT_VEC_N      NDI_GET_INT_VEC_N\n");
    printf("#define F_NDI_GET_INT_VEC_X      NDI_GET_INT_VEC_X\n");
    printf("#define F_NDI_GET_FLOAT64_VAL    NDI_GET_FLOAT64_VAL\n");
    printf("#define F_NDI_GET_FLOAT64_VAL_N  NDI_GET_FLOAT64_VAL_N\n");
    printf("#define F_NDI_GET_FLOAT64_VEC    NDI_GET_FLOAT64_VEC\n");
    printf("#define F_NDI_GET_FLOAT64_VEC_N  NDI_GET_FLOAT64_VEC_N\n");
    printf("#define F_NDI_GET_FLOAT64_VEC_X  NDI_GET_FLOAT64_VEC_X\n");
    printf("#define F_NDI_GET_FLOAT32_VAL    NDI_GET_FLOAT32_VAL\n");
    printf("#define F_NDI_GET_FLOAT32_VAL_N  NDI_GET_FLOAT32_VAL_N\n");
    printf("#define F_NDI_GET_FLOAT32_VEC    NDI_GET_FLOAT32_VEC\n");
    printf("#define F_NDI_GET_FLOAT32_VEC_N  NDI_GET_FLOAT32_VEC_N\n");
    printf("#define F_NDI_GET_FLOAT32_VEC_X  NDI_GET_FLOAT32_VEC_X\n");
    printf("#define F_NDI_GET_STRING_VAL     NDI_GET_STRING_VAL\n");
    printf("#define F_NDI_GET_STRING_VAL_N   NDI_GET_STRING_VAL_N\n");
    printf("#define F_NDI_GET_STRING_VEC     NDI_GET_STRING_VEC\n");
    printf("#define F_NDI_GET_STRING_VEC_X   NDI_GET_STRING_VEC_X\n");
    printf("#define F_NDID_ZAID_FLOAT64_VEC  NDID_ZAID_FLOAT64_VEC\n");
    printf("#define F_NDID_ZAID_FLOAT32_VEC  NDID_ZAID_FLOAT32_VEC\n");
    printf("#define F_NDI2_OPEN_GENDIR NDI2_OPEN_GENDIR\n");
    printf("#define F_NDI2_SET_OPTION_GENDIR NDI2_SET_OPTION_GENDIR\n");
    printf("#define F_NDI2_CHECK_OPTION_GENDIR NDI2_CHECK_OPTION_GENDIR\n");
    printf("#define F_NDI2_CLOSE_GENDIR NDI2_CLOSE_GENDIR\n");
    printf("#define F_NDI2_SET_ZAID NDI2_SET_ZAID\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT32_TEMP NDI2_SET_ZAID_FLOAT32_TEMP\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT64_TEMP NDI2_SET_ZAID_FLOAT64_TEMP\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT32_TEMP_SIG_0 NDI2_SET_ZAID_FLOAT32_TEMP_SIG_0\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT64_TEMP_SIG_0 NDI2_SET_ZAID_FLOAT64_TEMP_SIG_0\n");
    printf("#define F_NDI2_SET_ISOTOPE NDI2_SET_ISOTOPE\n");
    printf("#define F_NDI2_SET_REACTION NDI2_SET_REACTION\n");
    printf("#define F_NDI2_MAKE_HANDLE NDI2_MAKE_HANDLE\n");
    printf("#define F_NDI2_GET_SIZE NDI2_GET_SIZE\n");
    printf("#define F_NDI2_GET_SIZE_N NDI2_GET_SIZE_N\n");
    printf("#define F_NDI2_GET_SIZE_X NDI2_GET_SIZE_X\n");
    printf("#define F_NDI2_SET_OPTION NDI2_SET_OPTION\n");
    printf("#define F_NDI2_SET_FLOAT32_VEC_OPTION NDI2_SET_FLOAT32_VEC_OPTION\n");
    printf("#define F_NDI2_SET_FLOAT64_VEC_OPTION NDI2_SET_FLOAT64_VEC_OPTION\n");
    printf("#define F_NDI2_CHECK_OPTION NDI2_CHECK_OPTION\n");
    printf("#define F_NDI2_GET_INT_VAL NDI2_GET_INT_VAL\n");
    printf("#define F_NDI2_GET_INT_VAL_N NDI2_GET_INT_VAL_N\n");
    printf("#define F_NDI2_GET_INT_VEC NDI2_GET_INT_VEC\n");
    printf("#define F_NDI2_GET_INT_VEC_N NDI2_GET_INT_VEC_N\n");
    printf("#define F_NDI2_GET_INT_VEC_X NDI2_GET_INT_VEC_X\n");
    printf("#define F_NDI2_GET_FLOAT32_VAL NDI2_GET_FLOAT32_VAL\n");
    printf("#define F_NDI2_GET_FLOAT32_VAL_N NDI2_GET_FLOAT32_VAL_N\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC NDI2_GET_FLOAT32_VEC\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC_N NDI2_GET_FLOAT32_VEC_N\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC_X NDI2_GET_FLOAT32_VEC_X\n");
    printf("#define F_NDI2_GET_FLOAT64_VAL NDI2_GET_FLOAT64_VAL\n");
    printf("#define F_NDI2_GET_FLOAT64_VAL_N NDI2_GET_FLOAT64_VAL_N\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC NDI2_GET_FLOAT64_VEC\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC_N NDI2_GET_FLOAT64_VEC_N\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC_X NDI2_GET_FLOAT64_VEC_X\n");
    printf("#define F_NDI2_GET_STRING_VAL NDI2_GET_STRING_VAL\n");
    printf("#define F_NDI2_GET_STRING_VAL_N NDI2_GET_STRING_VAL_N\n");
    printf("#define F_NDI2_GET_STRING_VEC NDI2_GET_STRING_VEC\n");
    printf("#define F_NDI2_GET_STRING_VEC_X NDI2_GET_STRING_VEC_X\n");
    printf("#endif\n\n");

    printf("#ifdef UPPER_WITH\n");
    printf("#define F_NDID_GET_SIZE          NDID_GET_SIZE_\n");
    printf("#define F_NDID_GET_NUM           NDID_GET_NUM_\n");
    printf("#define F_NDID_GET_STRING_VEC    NDID_GET_STRING_VEC_\n");
    printf("#define F_NDI_INIT               NDI_INIT_\n");
    printf("#define F_NDI_CLOSE              NDI_CLOSE_\n");
    printf("#define F_NDI_SET_OPTION         NDI_SET_OPTION_\n");
    printf("#define F_NDI_CHECK_OPTION       NDI_CHECK_OPTION_\n");
    printf("#define F_NDI_SET_ZAID           NDI_SET_ZAID_\n");
    printf("#define F_NDI_SET_ZAID_FLOAT32_TEMP NDI_SET_ZAID_FLOAT32_TEMP_\n");
    printf("#define F_NDI_SET_ZAID_FLOAT64_TEMP NDI_SET_ZAID_FLOAT64_TEMP_\n");
    printf("#define F_NDI_SET_ZAID_FLOAT32_TEMP_SIG_0 NDI_SET_ZAID_FLOAT32_TEMP_SIG_0_\n");
    printf("#define F_NDI_SET_ZAID_FLOAT64_TEMP_SIG_0 NDI_SET_ZAID_FLOAT64_TEMP_SIG_0_\n");
    printf("#define F_NDI_SET_ISOTOPE        NDI_SET_ISOTOPE_\n");
    printf("#define F_NDI_SET_REACTION       NDI_SET_REACTION_\n");
    printf("#define F_NDI_GET_SIZE           NDI_GET_SIZE_\n");
    printf("#define F_NDI_GET_SIZE_N         NDI_GET_SIZE_N_\n");
    printf("#define F_NDI_GET_SIZE_X         NDI_GET_SIZE_X_\n");
    printf("#define F_NDI_GET_INT_VAL        NDI_GET_INT_VAL_\n");
    printf("#define F_NDI_GET_INT_VAL_N      NDI_GET_INT_VAL_N_\n");
    printf("#define F_NDI_GET_INT_VEC        NDI_GET_INT_VEC_\n");
    printf("#define F_NDI_GET_INT_VEC_N      NDI_GET_INT_VEC_N_\n");
    printf("#define F_NDI_GET_INT_VEC_X      NDI_GET_INT_VEC_X_\n");
    printf("#define F_NDI_GET_FLOAT64_VAL    NDI_GET_FLOAT64_VAL_\n");
    printf("#define F_NDI_GET_FLOAT64_VAL_N  NDI_GET_FLOAT64_VAL_N_\n");
    printf("#define F_NDI_GET_FLOAT64_VEC    NDI_GET_FLOAT64_VEC_\n");
    printf("#define F_NDI_GET_FLOAT64_VEC_N  NDI_GET_FLOAT64_VEC_N_\n");
    printf("#define F_NDI_GET_FLOAT64_VEC_X  NDI_GET_FLOAT64_VEC_X_\n");
    printf("#define F_NDI_GET_FLOAT32_VAL    NDI_GET_FLOAT32_VAL_\n");
    printf("#define F_NDI_GET_FLOAT32_VAL_N  NDI_GET_FLOAT32_VAL_N_\n");
    printf("#define F_NDI_GET_FLOAT32_VEC    NDI_GET_FLOAT32_VEC_\n");
    printf("#define F_NDI_GET_FLOAT32_VEC_N  NDI_GET_FLOAT32_VEC_N_\n");
    printf("#define F_NDI_GET_FLOAT32_VEC_X  NDI_GET_FLOAT32_VEC_X_\n");
    printf("#define F_NDI_GET_STRING_VAL     NDI_GET_STRING_VAL_\n");
    printf("#define F_NDI_GET_STRING_VAL_N   NDI_GET_STRING_VAL_N_\n");
    printf("#define F_NDI_GET_STRING_VEC     NDI_GET_STRING_VEC_\n");
    printf("#define F_NDI_GET_STRING_VEC_X   NDI_GET_STRING_VEC_X_\n");
    printf("#define F_NDID_ZAID_FLOAT64_VEC  NDID_ZAID_FLOAT64_VEC_\n");
    printf("#define F_NDID_ZAID_FLOAT32_VEC  NDID_ZAID_FLOAT32_VEC_\n");
    printf("#define F_NDID_ZAID_FLOAT64_VEC  NDID_ZAID_FLOAT64_VEC_\n");
    printf("#define F_NDID_ZAID_FLOAT32_VEC  NDID_ZAID_FLOAT32_VEC_\n");
    printf("#define F_NDI2_OPEN_GENDIR NDI2_OPEN_GENDIR_\n");
    printf("#define F_NDI2_SET_OPTION_GENDIR NDI2_SET_OPTION_GENDIR_\n");
    printf("#define F_NDI2_CHECK_OPTION_GENDIR NDI2_CHECK_OPTION_GENDIR_\n");
    printf("#define F_NDI2_CLOSE_GENDIR NDI2_CLOSE_GENDIR_\n");
    printf("#define F_NDI2_SET_LIB_TYPE NDI2_SET_LIB_TYPE_\n");
    printf("#define F_NDI2_SET_ZAID NDI2_SET_ZAID_\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT32_TEMP NDI2_SET_ZAID_FLOAT32_TEMP_\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT64_TEMP NDI2_SET_ZAID_FLOAT64_TEMP_\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT32_TEMP_SIG_0 NDI2_SET_ZAID_FLOAT32_TEMP_SIG_0_\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT64_TEMP_SIG_0 NDI2_SET_ZAID_FLOAT64_TEMP_SIG_0_\n");
    printf("#define F_NDI2_SET_ISOTOPE NDI2_SET_ISOTOPE_\n");
    printf("#define F_NDI2_SET_REACTION NDI2_SET_REACTION_\n");
    printf("#define F_NDI2_MAKE_HANDLE NDI2_MAKE_HANDLE_\n");
    printf("#define F_NDI2_GET_SIZE NDI2_GET_SIZE_\n");
    printf("#define F_NDI2_GET_SIZE_N NDI2_GET_SIZE_N_\n");
    printf("#define F_NDI2_GET_SIZE_X NDI2_GET_SIZE_X_\n");
    printf("#define F_NDI2_SET_OPTION NDI2_SET_OPTION_\n");
    printf("#define F_NDI2_SET_FLOAT32_VEC_OPTION NDI2_SET_FLOAT32_VEC_OPTION_\n");
    printf("#define F_NDI2_SET_FLOAT64_VEC_OPTION NDI2_SET_FLOAT64_VEC_OPTION_\n");
    printf("#define F_NDI2_CHECK_OPTION NDI2_CHECK_OPTION_\n");
    printf("#define F_NDI2_GET_INT_VAL NDI2_GET_INT_VAL_\n");
    printf("#define F_NDI2_GET_INT_VAL_N NDI2_GET_INT_VAL_N_\n");
    printf("#define F_NDI2_GET_INT_VEC NDI2_GET_INT_VEC_\n");
    printf("#define F_NDI2_GET_INT_VEC_N NDI2_GET_INT_VEC_N_\n");
    printf("#define F_NDI2_GET_INT_VEC_X NDI2_GET_INT_VEC_X_\n");
    printf("#define F_NDI2_GET_FLOAT32_VAL NDI2_GET_FLOAT32_VAL_\n");
    printf("#define F_NDI2_GET_FLOAT32_VAL_N NDI2_GET_FLOAT32_VAL_N_\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC NDI2_GET_FLOAT32_VEC_\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC_N NDI2_GET_FLOAT32_VEC_N_\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC_X NDI2_GET_FLOAT32_VEC_X_\n");
    printf("#define F_NDI2_GET_FLOAT64_VAL NDI2_GET_FLOAT64_VAL_\n");
    printf("#define F_NDI2_GET_FLOAT64_VAL_N NDI2_GET_FLOAT64_VAL_N_\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC NDI2_GET_FLOAT64_VEC_\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC_N NDI2_GET_FLOAT64_VEC_N_\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC_X NDI2_GET_FLOAT64_VEC_X_\n");
    printf("#define F_NDI2_GET_STRING_VAL NDI2_GET_STRING_VAL_\n");
    printf("#define F_NDI2_GET_STRING_VAL_N NDI2_GET_STRING_VAL_N_\n");
    printf("#define F_NDI2_GET_STRING_VEC NDI2_GET_STRING_VEC_\n");
    printf("#define F_NDI2_GET_STRING_VEC_X NDI2_GET_STRING_VEC_X_\n");
    printf("#endif\n\n");

    printf("#ifdef LOWER_WITHOUT\n");
    printf("#define F_NDID_GET_SIZE          ndid_get_size\n");
    printf("#define F_NDID_GET_NUM           ndid_get_num\n");
    printf("#define F_NDID_GET_STRING_VEC    ndid_get_string_vec\n");
    printf("#define F_NDI_INIT               ndi_init\n");
    printf("#define F_NDI_CLOSE              ndi_close\n");
    printf("#define F_NDI_SET_OPTION         ndi_set_option\n");
    printf("#define F_NDI_CHECK_OPTION       ndi_check_option\n");
    printf("#define F_NDI_SET_ZAID           ndi_set_zaid\n");
    printf("#define F_NDI_SET_ZAID_FLOAT32_TEMP ndi_set_zaid_float32_temp\n");
    printf("#define F_NDI_SET_ZAID_FLOAT64_TEMP ndi_set_zaid_float64_temp\n");
    printf("#define F_NDI_SET_ZAID_FLOAT32_TEMP_SIG_0 ndi_set_zaid_float32_temp_sig_0\n");
    printf("#define F_NDI_SET_ZAID_FLOAT64_TEMP_SIG_0 ndi_set_zaid_float64_temp_sig_0\n");
    printf("#define F_NDI_SET_ISOTOPE        ndi_set_isotope\n");
    printf("#define F_NDI_SET_REACTION       ndi_set_reaction\n");
    printf("#define F_NDI_GET_SIZE           ndi_get_size\n");
    printf("#define F_NDI_GET_SIZE_N         ndi_get_size_n\n");
    printf("#define F_NDI_GET_SIZE_X         ndi_get_size_x\n");
    printf("#define F_NDI_GET_INT_VAL        ndi_get_int_val\n");
    printf("#define F_NDI_GET_INT_VAL_N      ndi_get_int_val_n\n");
    printf("#define F_NDI_GET_INT_VEC        ndi_get_int_vec\n");
    printf("#define F_NDI_GET_INT_VEC_N      ndi_get_int_vec_n\n");
    printf("#define F_NDI_GET_INT_VEC_X      ndi_get_int_vec_x\n");
    printf("#define F_NDI_GET_FLOAT64_VAL    ndi_get_float64_val\n");
    printf("#define F_NDI_GET_FLOAT64_VAL_N  ndi_get_float64_val_n\n");
    printf("#define F_NDI_GET_FLOAT64_VEC    ndi_get_float64_vec\n");
    printf("#define F_NDI_GET_FLOAT64_VEC_N  ndi_get_float64_vec_n\n");
    printf("#define F_NDI_GET_FLOAT64_VEC_X  ndi_get_float64_vec_x\n");
    printf("#define F_NDI_GET_FLOAT32_VAL    ndi_get_float32_val\n");
    printf("#define F_NDI_GET_FLOAT32_VAL_N  ndi_get_float32_val_n\n");
    printf("#define F_NDI_GET_FLOAT32_VEC    ndi_get_float32_vec\n");
    printf("#define F_NDI_GET_FLOAT32_VEC_N  ndi_get_float32_vec_n\n");
    printf("#define F_NDI_GET_FLOAT32_VEC_X  ndi_get_float32_vec_x\n");
    printf("#define F_NDI_GET_STRING_VAL     ndi_get_string_val\n");
    printf("#define F_NDI_GET_STRING_VAL_N   ndi_get_string_val_n\n");
    printf("#define F_NDI_GET_STRING_VEC     ndi_get_string_vec\n");
    printf("#define F_NDI_GET_STRING_VEC_X   ndi_get_string_vec_x\n");
    printf("#define F_NDID_ZAID_FLOAT64_VEC  ndid_zaid_float64_vec\n");
    printf("#define F_NDID_ZAID_FLOAT32_VEC  ndid_zaid_float32_vec\n");
    printf("#define F_NDI2_OPEN_GENDIR ndi2_open_gendir\n");
    printf("#define F_NDI2_SET_OPTION_GENDIR ndi2_set_option_gendir\n");
    printf("#define F_NDI2_CHECK_OPTION_GENDIR ndi2_check_option_gendir\n");
    printf("#define F_NDI2_CLOSE_GENDIR ndi2_close_gendir\n");
    printf("#define F_NDI2_OPEN_GENDIR ndi2_open_gendir\n");
    printf("#define F_NDI2_SET_OPTION_GENDIR ndi2_set_option_gendir\n");
    printf("#define F_NDI2_CHECK_OPTION_GENDIR ndi2_check_option_gendir\n");
    printf("#define F_NDI2_CLOSE_GENDIR ndi2_close_gendir\n");
    printf("#define F_NDI2_MAKE_HANDLE ndi2_make_handle\n");
    printf("#define F_NDI2_SET_LIB_TYPE ndi2_set_lib_type\n");
    printf("#define F_NDI2_SET_ZAID ndi2_set_zaid\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT32_TEMP ndi2_set_zaid_float32_temp\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT64_TEMP ndi2_set_zaid_float64_temp\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT32_TEMP_SIG_0 ndi2_set_zaid_float32_temp_sig_0\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT64_TEMP_SIG_0 ndi2_set_zaid_float64_temp_sig_0\n");

    printf("#define F_NDI2_SET_ISOTOPE ndi2_set_isotope\n");
    printf("#define F_NDI2_SET_REACTION ndi2_set_reaction\n");
    printf("#define F_NDI2_GET_SIZE ndi2_get_size\n");
    printf("#define F_NDI2_GET_SIZE_N ndi2_get_size_n\n");
    printf("#define F_NDI2_GET_SIZE_X ndi2_get_size_x\n");
    printf("#define F_NDI2_SET_OPTION ndi2_set_option\n");
    printf("#define F_NDI2_SET_FLOAT32_VEC_OPTION ndi2_set_float32_vec_option\n");
    printf("#define F_NDI2_SET_FLOAT64_VEC_OPTION ndi2_set_float64_vec_option\n");
    printf("#define F_NDI2_CHECK_OPTION ndi2_check_option\n");
    printf("#define F_NDI2_GET_INT_VAL ndi2_get_int_val\n");
    printf("#define F_NDI2_GET_INT_VAL_N ndi2_get_int_val_n\n");
    printf("#define F_NDI2_GET_INT_VEC ndi2_get_int_vec\n");
    printf("#define F_NDI2_GET_INT_VEC_N ndi2_get_int_vec_n\n");
    printf("#define F_NDI2_GET_INT_VEC_X ndi2_get_int_vec_x\n");
    printf("#define F_NDI2_GET_FLOAT32_VAL ndi2_get_float32_val\n");
    printf("#define F_NDI2_GET_FLOAT32_VAL_N ndi2_get_float32_val_n\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC ndi2_get_float32_vec\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC_N ndi2_get_float32_vec_n\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC_X ndi2_get_float32_vec_x\n");
    printf("#define F_NDI2_GET_FLOAT64_VAL ndi2_get_float64_val\n");
    printf("#define F_NDI2_GET_FLOAT64_VAL_N ndi2_get_float64_val_n\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC ndi2_get_float64_vec\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC_N ndi2_get_float64_vec_n\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC_X ndi2_get_float64_vec_x\n");
    printf("#define F_NDI2_GET_STRING_VAL ndi2_get_string_val\n");
    printf("#define F_NDI2_GET_STRING_VAL_N ndi2_get_string_val_n\n");
    printf("#define F_NDI2_GET_STRING_VEC ndi2_get_string_vec\n");
    printf("#define F_NDI2_GET_STRING_VEC_X ndi2_get_string_vec_x\n");
    printf("#endif\n\n");

    printf("#ifdef LOWER_WITH\n");
    printf("#define F_NDID_GET_SIZE          ndid_get_size_\n");
    printf("#define F_NDID_GET_NUM           ndid_get_num_\n");
    printf("#define F_NDID_GET_STRING_VEC    ndid_get_string_vec_\n");
    printf("#define F_NDI_INIT               ndi_init_\n");
    printf("#define F_NDI_CLOSE              ndi_close_\n");
    printf("#define F_NDI_SET_ZAID           ndi_set_zaid_\n");
    printf("#define F_NDI_SET_ZAID_FLOAT32_TEMP ndi_set_zaid_float32_temp_\n");
    printf("#define F_NDI_SET_ZAID_FLOAT64_TEMP ndi_set_zaid_float64_temp_\n");
    printf("#define F_NDI_SET_ZAID_FLOAT32_TEMP_SIG_0 ndi_set_zaid_float32_temp_sig_0_\n");
    printf("#define F_NDI_SET_ZAID_FLOAT64_TEMP_SIG_0 ndi_set_zaid_float64_temp_sig_0_\n");
    printf("#define F_NDI_SET_ISOTOPE        ndi_set_isotope_\n");
    printf("#define F_NDI_SET_REACTION       ndi_set_reaction_\n");
    printf("#define F_NDI_SET_OPTION         ndi_set_option_\n");
    printf("#define F_NDI_CHECK_OPTION       ndi_check_option_\n");
    printf("#define F_NDI_GET_SIZE           ndi_get_size_\n");
    printf("#define F_NDI_GET_SIZE_N         ndi_get_size_n_\n");
    printf("#define F_NDI_GET_SIZE_X         ndi_get_size_x_\n");
    printf("#define F_NDI_GET_INT_VAL        ndi_get_int_val_\n");
    printf("#define F_NDI_GET_INT_VAL_N      ndi_get_int_val_n_\n");
    printf("#define F_NDI_GET_INT_VEC        ndi_get_int_vec_\n");
    printf("#define F_NDI_GET_INT_VEC_N      ndi_get_int_vec_n_\n");
    printf("#define F_NDI_GET_INT_VEC_X      ndi_get_int_vec_x_\n");
    printf("#define F_NDI_GET_FLOAT64_VAL    ndi_get_float64_val_\n");
    printf("#define F_NDI_GET_FLOAT64_VAL_N  ndi_get_float64_val_n_\n");
    printf("#define F_NDI_GET_FLOAT64_VEC    ndi_get_float64_vec_\n");
    printf("#define F_NDI_GET_FLOAT64_VEC_N  ndi_get_float64_vec_n_\n");
    printf("#define F_NDI_GET_FLOAT64_VEC_X  ndi_get_float64_vec_x_\n");
    printf("#define F_NDI_GET_FLOAT32_VAL    ndi_get_float32_val_\n");
    printf("#define F_NDI_GET_FLOAT32_VAL_N  ndi_get_float32_val_n_\n");
    printf("#define F_NDI_GET_FLOAT32_VEC    ndi_get_float32_vec_\n");
    printf("#define F_NDI_GET_FLOAT32_VEC_N  ndi_get_float32_vec_n_\n");
    printf("#define F_NDI_GET_FLOAT32_VEC_X  ndi_get_float32_vec_x_\n");
    printf("#define F_NDI_GET_STRING_VAL     ndi_get_string_val_\n");
    printf("#define F_NDI_GET_STRING_VAL_N   ndi_get_string_val_n_\n");
    printf("#define F_NDI_GET_STRING_VEC     ndi_get_string_vec_\n");
    printf("#define F_NDI_GET_STRING_VEC_X   ndi_get_string_vec_x_\n");
    printf("#define F_NDID_ZAID_FLOAT64_VEC  ndid_zaid_float64_vec_\n");
    printf("#define F_NDID_ZAID_FLOAT32_VEC  ndid_zaid_float32_vec_\n");
    printf("#define F_NDI2_OPEN_GENDIR ndi2_open_gendir_\n");
    printf("#define F_NDI2_SET_OPTION_GENDIR ndi2_set_option_gendir_\n");
    printf("#define F_NDI2_CHECK_OPTION_GENDIR ndi2_check_option_gendir_\n");
    printf("#define F_NDI2_CLOSE_GENDIR ndi2_close_gendir_\n");
    printf("#define F_NDI2_OPEN_GENDIR ndi2_open_gendir_\n");
    printf("#define F_NDI2_SET_OPTION_GENDIR ndi2_set_option_gendir_\n");
    printf("#define F_NDI2_CHECK_OPTION_GENDIR ndi2_check_option_gendir_\n");
    printf("#define F_NDI2_CLOSE_GENDIR ndi2_close_gendir_\n");
    printf("#define F_NDI2_MAKE_HANDLE ndi2_make_handle_\n");
    printf("#define F_NDI2_SET_LIB_TYPE ndi2_set_lib_type_\n");
    printf("#define F_NDI2_SET_ZAID ndi2_set_zaid_\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT32_TEMP ndi2_set_zaid_float32_temp_\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT64_TEMP ndi2_set_zaid_float64_temp_\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT32_TEMP_SIG_0 ndi2_set_zaid_float32_temp_sig_0_\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT64_TEMP_SIG_0 ndi2_set_zaid_float64_temp_sig_0_\n");
    printf("#define F_NDI2_SET_ISOTOPE ndi2_set_isotope_\n");
    printf("#define F_NDI2_SET_REACTION ndi2_set_reaction_\n");
    printf("#define F_NDI2_GET_SIZE ndi2_get_size_\n");
    printf("#define F_NDI2_GET_SIZE_N ndi2_get_size_n_\n");
    printf("#define F_NDI2_GET_SIZE_X ndi2_get_size_x_\n");
    printf("#define F_NDI2_SET_OPTION ndi2_set_option_\n");
    printf("#define F_NDI2_SET_FLOAT32_VEC_OPTION ndi2_set_float32_vec_option_\n");
    printf("#define F_NDI2_SET_FLOAT64_VEC_OPTION ndi2_set_float64_vec_option_\n");
    printf("#define F_NDI2_CHECK_OPTION ndi2_check_option_\n");
    printf("#define F_NDI2_GET_INT_VAL ndi2_get_int_val_\n");
    printf("#define F_NDI2_GET_INT_VAL_N ndi2_get_int_val_n_\n");
    printf("#define F_NDI2_GET_INT_VEC ndi2_get_int_vec_\n");
    printf("#define F_NDI2_GET_INT_VEC_N ndi2_get_int_vec_n_\n");
    printf("#define F_NDI2_GET_INT_VEC_X ndi2_get_int_vec_x_\n");
    printf("#define F_NDI2_GET_FLOAT32_VAL ndi2_get_float32_val_\n");
    printf("#define F_NDI2_GET_FLOAT32_VAL_N ndi2_get_float32_val_n_\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC ndi2_get_float32_vec_\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC_N ndi2_get_float32_vec_n_\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC_X ndi2_get_float32_vec_x_\n");
    printf("#define F_NDI2_GET_FLOAT64_VAL ndi2_get_float64_val_\n");
    printf("#define F_NDI2_GET_FLOAT64_VAL_N ndi2_get_float64_val_n_\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC ndi2_get_float64_vec_\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC_N ndi2_get_float64_vec_n_\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC_X ndi2_get_float64_vec_x_\n");
    printf("#define F_NDI2_GET_STRING_VAL ndi2_get_string_val_\n");
    printf("#define F_NDI2_GET_STRING_VAL_N ndi2_get_string_val_n_\n");
    printf("#define F_NDI2_GET_STRING_VEC ndi2_get_string_vec_\n");
    printf("#define F_NDI2_GET_STRING_VEC_X ndi2_get_string_vec_x_\n");
    printf("#endif\n\n");

    printf("#ifdef LOWER_WITH2\n");
    printf("#define F_NDID_GET_SIZE          ndid_get_size__\n");
    printf("#define F_NDID_GET_NUM           ndid_get_num__\n");
    printf("#define F_NDID_GET_STRING_VEC    ndid_get_string_vec__\n");
    printf("#define F_NDI_INIT               ndi_init__\n");
    printf("#define F_NDI_CLOSE              ndi_close__\n");
    printf("#define F_NDI_SET_ZAID           ndi_set_zaid__\n");
    printf("#define F_NDI_SET_ZAID_FLOAT32_TEMP ndi_set_zaid_float32_temp__\n");
    printf("#define F_NDI_SET_ZAID_FLOAT64_TEMP ndi_set_zaid_float64_temp__\n");
    printf("#define F_NDI_SET_ZAID_FLOAT32_TEMP_SIG_0 ndi_set_zaid_float32_temp_sig_0__\n");
    printf("#define F_NDI_SET_ZAID_FLOAT64_TEMP_SIG_0 ndi_set_zaid_float64_temp_sig_0__\n");
    printf("#define F_NDI_SET_ISOTOPE        ndi_set_isotope__\n");
    printf("#define F_NDI_SET_REACTION       ndi_set_reaction__\n");
    printf("#define F_NDI_SET_OPTION         ndi_set_option__\n");
    printf("#define F_NDI_CHECK_OPTION       ndi_check_option__\n");
    printf("#define F_NDI_GET_SIZE           ndi_get_size__\n");
    printf("#define F_NDI_GET_SIZE_N         ndi_get_size_n__\n");
    printf("#define F_NDI_GET_SIZE_X         ndi_get_size_x__\n");
    printf("#define F_NDI_GET_INT_VAL        ndi_get_int_val__\n");
    printf("#define F_NDI_GET_INT_VAL_N      ndi_get_int_val_n__\n");
    printf("#define F_NDI_GET_INT_VEC        ndi_get_int_vec__\n");
    printf("#define F_NDI_GET_INT_VEC_N      ndi_get_int_vec_n__\n");
    printf("#define F_NDI_GET_INT_VEC_X      ndi_get_int_vec_x__\n");
    printf("#define F_NDI_GET_FLOAT64_VAL    ndi_get_float64_val__\n");
    printf("#define F_NDI_GET_FLOAT64_VAL_N  ndi_get_float64_val_n__\n");
    printf("#define F_NDI_GET_FLOAT64_VEC    ndi_get_float64_vec__\n");
    printf("#define F_NDI_GET_FLOAT64_VEC_N  ndi_get_float64_vec_n__\n");
    printf("#define F_NDI_GET_FLOAT64_VEC_X  ndi_get_float64_vec_x__\n");
    printf("#define F_NDI_GET_FLOAT32_VAL    ndi_get_float32_val__\n");
    printf("#define F_NDI_GET_FLOAT32_VAL_N  ndi_get_float32_val_n__\n");
    printf("#define F_NDI_GET_FLOAT32_VEC    ndi_get_float32_vec__\n");
    printf("#define F_NDI_GET_FLOAT32_VEC_N  ndi_get_float32_vec_n__\n");
    printf("#define F_NDI_GET_FLOAT32_VEC_X  ndi_get_float32_vec_x__\n");
    printf("#define F_NDI_GET_STRING_VAL     ndi_get_string_val__\n");
    printf("#define F_NDI_GET_STRING_VAL_N   ndi_get_string_val_n__\n");
    printf("#define F_NDI_GET_STRING_VEC     ndi_get_string_vec__\n");
    printf("#define F_NDI_GET_STRING_VEC_X   ndi_get_string_vec_x__\n");
    printf("#define F_NDID_ZAID_FLOAT64_VEC  ndid_zaid_float64_vec__\n");
    printf("#define F_NDID_ZAID_FLOAT32_VEC  ndid_zaid_float32_vec__\n");
    printf("#define F_NDI2_OPEN_GENDIR ndi2_open_gendir__\n");
    printf("#define F_NDI2_SET_OPTION_GENDIR ndi2_set_option_gendir__\n");
    printf("#define F_NDI2_CHECK_OPTION_GENDIR ndi2_check_option_gendir__\n");
    printf("#define F_NDI2_CLOSE_GENDIR ndi2_close_gendir__\n");
    printf("#define F_NDI2_OPEN_GENDIR ndi2_open_gendir__\n");
    printf("#define F_NDI2_SET_OPTION_GENDIR ndi2_set_option_gendir__\n");
    printf("#define F_NDI2_CHECK_OPTION_GENDIR ndi2_check_option_gendir__\n");
    printf("#define F_NDI2_CLOSE_GENDIR ndi2_close_gendir__\n");
    printf("#define F_NDI2_MAKE_HANDLE ndi2_make_handle__\n");
    printf("#define F_NDI2_SET_LIB_TYPE ndi2_set_lib_type__\n");
    printf("#define F_NDI2_SET_ZAID ndi2_set_zaid__\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT32_TEMP ndi2_set_zaid_float32_temp__\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT64_TEMP ndi2_set_zaid_float64_temp__\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT32_TEMP_SIG_0 ndi2_set_zaid_float32_temp_sig_0__\n");
    printf("#define F_NDI2_SET_ZAID_FLOAT64_TEMP_SIG_0 ndi2_set_zaid_float64_temp_sig_0__\n");
    printf("#define F_NDI2_SET_ISOTOPE ndi2_set_isotope__\n");
    printf("#define F_NDI2_SET_REACTION ndi2_set_reaction__\n");
    printf("#define F_NDI2_GET_SIZE ndi2_get_size__\n");
    printf("#define F_NDI2_GET_SIZE_N ndi2_get_size_n__\n");
    printf("#define F_NDI2_GET_SIZE_X ndi2_get_size_x__\n");
    printf("#define F_NDI2_SET_OPTION ndi2_set_option__\n");
    printf("#define F_NDI2_SET_FLOAT32_VEC_OPTION ndi2_set_float32_vec_option__\n");
    printf("#define F_NDI2_SET_FLOAT64_VEC_OPTION ndi2_set_float64_vec_option__\n");
    printf("#define F_NDI2_CHECK_OPTION ndi2_check_option__\n");
    printf("#define F_NDI2_GET_INT_VAL ndi2_get_int_val__\n");
    printf("#define F_NDI2_GET_INT_VAL_N ndi2_get_int_val_n__\n");
    printf("#define F_NDI2_GET_INT_VEC ndi2_get_int_vec__\n");
    printf("#define F_NDI2_GET_INT_VEC_N ndi2_get_int_vec_n__\n");
    printf("#define F_NDI2_GET_INT_VEC_X ndi2_get_int_vec_x__\n");
    printf("#define F_NDI2_GET_FLOAT32_VAL ndi2_get_float32_val__\n");
    printf("#define F_NDI2_GET_FLOAT32_VAL_N ndi2_get_float32_val_n__\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC ndi2_get_float32_vec__\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC_N ndi2_get_float32_vec_n__\n");
    printf("#define F_NDI2_GET_FLOAT32_VEC_X ndi2_get_float32_vec_x__\n");
    printf("#define F_NDI2_GET_FLOAT64_VAL ndi2_get_float64_val__\n");
    printf("#define F_NDI2_GET_FLOAT64_VAL_N ndi2_get_float64_val_n__\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC ndi2_get_float64_vec__\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC_N ndi2_get_float64_vec_n__\n");
    printf("#define F_NDI2_GET_FLOAT64_VEC_X ndi2_get_float64_vec_x__\n");
    printf("#define F_NDI2_GET_STRING_VAL ndi2_get_string_val__\n");
    printf("#define F_NDI2_GET_STRING_VAL_N ndi2_get_string_val_n__\n");
    printf("#define F_NDI2_GET_STRING_VEC ndi2_get_string_vec__\n");
    printf("#define F_NDI2_GET_STRING_VEC_X ndi2_get_string_vec_x__\n");
    printf("#endif\n\n");

}
