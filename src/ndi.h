/*
 * $Id$
 * Description:	User header file for NDI library.
 * This header file is included in user code and library code.
 */

/**********************************************************************/

/*
 * Defines
 */

#ifndef NDI_H
#define NDI_H

#include "ndi_keywords.h"
#include "ndi_var_type.h"

/**********************************************************************/

/*
 * Function prototypes
 */

#ifdef	__cplusplus
extern "C" {
#endif

/**********************************************************************/
    /* NDI 2.0 user functions with handles */

    /* Gendir functions */
    int NDI2_open_gendir(int *handle, const char *path);
    int NDI2_set_option_gendir(int handle, int in_key, const char *value);
    int NDI2_check_option_gendir(int handle, int in_key, char *out_value,
                                 int string_size);
    int NDI2_close_gendir(int handle);

    /* data handle functions */
    int NDI2_make_handle(int handle0, const char *type, int *handle1);
    int NDI2_set_isotope(int handle, const char *zaid);
    int NDI2_set_zaid(int handle, const char *zaid);
    int NDI2_set_zaid_float32_temp(int handle, const char *zaid, 
				   ndi_float32 temp);
    int NDI2_set_zaid_float64_temp(int handle, const char *zaid, 
				   ndi_float64 temp);
    int NDI2_set_zaid_float32_temp_sig_0(int handle, const char *zaid,
                                         ndi_float32 temp, ndi_float32 sig0);
    int NDI2_set_zaid_float64_temp_sig_0(int handle, const char *zaid,
                                         ndi_float64 temp, ndi_float64 sig0);
    int NDI2_set_reaction(int handle, const char *zaid);
    int NDI2_set_option(int handle, int in_key, const char *value);
    int NDI2_check_option(int handle, int in_key, char *out_value,
			  int string_size);

    int NDI2_get_size(int handle, int in_key, int *ierr);
    int NDI2_get_size_n(int handle, int in_key, int vec_num,
			int *ierr);
    int NDI2_get_size_x(int handle, int in_key, const char *in_str,
			int *ierr);
    int NDI2_set_float32_vec_option(const int handle, int in_key,
				    const ndi_float32 *option_array,
				    const int array_size);
    int NDI2_set_float64_vec_option(const int handle, int in_key,
				    const ndi_float64 *option_array,
				    const int array_size);
    int NDI2_get_int_val(int handle, int in_key, int *ret_val);
    int NDI2_get_int_val_n(int handle, int in_key, int vec_num,
			   int *ret_val);
    int NDI2_get_int_vec(int handle, int in_key, int *return_vec,
			 int vec_size);
    int NDI2_get_int_vec_n(int handle, int in_key, int vec_num,
			   int *return_vec, int vec_size);
    int NDI2_get_int_vec_x(int handle, int in_key, const char *in_str,
			   int *return_vec, int vec_size);
    int NDI2_get_float32_val(int handle, int in_key,
			     ndi_float32 * ret_val);
    int NDI2_get_float32_val_n(int handle, int in_key, int vec_num,
			       ndi_float32 * ret_val);
    int NDI2_get_float32_vec(int handle, int in_key,
			     ndi_float32 *return_vec, int vec_size);
    int NDI2_get_float32_vec_n(int handle, int in_key, int vec_num,
			       ndi_float32 *return_vec, int vec_size);
    int NDI2_get_float32_vec_x(int handle, int in_key,
			       const char *in_str,
			       ndi_float32 * return_vec, int vec_size);
    int NDI2_get_float64_val(int handle, int in_key,
			     ndi_float64 *ret_val);
    int NDI2_get_float64_val_n(int handle, int in_key, int vec_num,
			       ndi_float64 *ret_val);
    int NDI2_get_float64_vec(int handle, int in_key,
			     ndi_float64 *return_vec, int vec_size);
    int NDI2_get_float64_vec_n(int handle, int in_key, int vec_num,
			       ndi_float64 *return_vec, int vec_size);
    int NDI2_get_float64_vec_x(int handle, int in_key,
			       const char *in_str,
			       ndi_float64 *return_vec, int vec_size);
    int NDI2_get_string_val(int handle, int in_key, char *ret_val,
			    int str_size);
    int NDI2_get_string_val_n(int handle, int in_key, int vec_num,
			      char *ret_val, int str_size);
    int NDI2_get_string_vec(int handle, int in_key, 
			    char *ret_val, int str_size);
    int NDI2_get_string_vec_x(int handle, int in_key, 
			      const char *in_str, 
			      char *ret_val, int str_size);

/**********************************************************************/
    /* Backwards compatibility user functions */

    int ndid_c_get_size(int in_key, const char *in_value, int *arr_size);
    int ndid_c_get_num(int in_key, const char *in_value, int *arr_num);
    int ndid_c_get_string_vec(int in_key, const char *in_value, char *zaid,
			      int arr_size);
    int ndid_c_zaid_float32_vec(int in_key, const char *in_value,
				char *zaid, int arr_size,
				ndi_float32 * float_vec, int fl_arr_size);
    int ndid_c_zaid_float64_vec(int in_key, const char *in_value,
				char *zaid, int arr_size,
				ndi_float64 * float_vec, int fl_arr_size);

    int ndi_c_init(void);
    int ndi_c_set_isotope(const char *zaid);
    int ndi_c_set_zaid(const char *zaid);
    int ndi_c_set_zaid_float32_temp(const char *zaid, ndi_float32 temp);
    int ndi_c_set_zaid_float64_temp(const char *zaid, ndi_float64 temp);
    int ndi_c_set_zaid_float32_temp_sig_0(const char *zaid, ndi_float32 temp,
					  ndi_float32 sig0);
    int ndi_c_set_zaid_float64_temp_sig_0(const char *zaid, ndi_float64 temp,
					  ndi_float64 sig0);
    int ndi_c_set_reaction(const char *zaid);
    int ndi_c_get_size(int in_key, int *ierr);
    int ndi_c_get_size_n(int in_key, int vec_num, int *ierr);
    int ndi_c_get_size_x(int in_key, const char *in_str, int *ierr);
    int ndi_c_set_option(int in_key, const char *value);
    int ndi_c_check_option(int in_key, char *out_value, int string_size);
    int ndi_c_get_int_val(int in_key, int *ret_val);
    int ndi_c_get_int_val_n(int in_key, int vec_num, int *ret_val);
    int ndi_c_get_int_vec(int in_key, int *return_vec, int vec_size);
    int ndi_c_get_int_vec_n(int in_key, int vec_num, int *return_vec,
			    int vec_size);
    int ndi_c_get_int_vec_x(int in_key, const char *in_str, int *return_vec,
			    int vec_size);
    int ndi_c_get_float32_val(int in_key, ndi_float32 * ret_val);
    int ndi_c_get_float32_val_n(int in_key, int vec_num,
				ndi_float32 * ret_val);
    int ndi_c_get_float32_vec(int in_key, ndi_float32 * return_vec,
			      int vec_size);
    int ndi_c_get_float32_vec_n(int in_key, int vec_num,
				ndi_float32 * return_vec, int vec_size);
    int ndi_c_get_float32_vec_x(int in_key, const char *in_str,
				ndi_float32 * return_vec, int vec_size);
    int ndi_c_get_float64_val(int in_key, ndi_float64 * ret_val);
    int ndi_c_get_float64_val_n(int in_key, int vec_num,
				ndi_float64 * ret_val);
    int ndi_c_get_float64_vec(int in_key, ndi_float64 * return_vec,
			      int vec_size);
    int ndi_c_get_float64_vec_n(int in_key, int vec_num,
				ndi_float64 * return_vec, int vec_size);
    int ndi_c_get_float64_vec_x(int in_key, const char *in_str,
				ndi_float64 * return_vec, int vec_size);
    int ndi_c_get_string_val(int in_key, char *ret_val, int str_size);
    int ndi_c_get_string_val_n(int in_key, int vec_num, char *ret_val,
			       int str_size);
    int ndi_c_get_string_vec(int in_key, char *ret_val, int str_size);
    int ndi_c_get_string_vec_x(int in_key, const char *in_str, 
			      char *ret_val, int str_size);
    int ndi_c_close(void);

    int ndi_c_c_to_mt(int c_val);
    int ndi_c_mt_to_c(int mt);

/**********************************************************************/

    /* Rename functions */

#ifndef NDI_NO_NAME_SUB
#define ndid_get_size(x, y, z) ndid_c_get_size(x, y, z)
#define ndid_get_num(x, y, z) ndid_c_get_num(x, y, z)
#define ndid_get_string_vec(w, x, y, z) ndid_c_get_string_vec(w, x, y, z)
#define ndid_zaid_float32_vec(a, b, c, x, y, z) ndid_c_zaid_float32_vec(a, b, c, x, y, z)
#define ndid_zaid_float64_vec(a, b, c, x, y, z) ndid_c_zaid_float64_vec(a, b, c, x, y, z)


#define ndi_init() ndi_c_init()
#define ndi_set_isotope(x) ndi_c_set_isotope(x)
#define ndi_set_zaid(x) ndi_c_set_zaid(x)
#define ndi_set_zaid_float32_temp(x, y) ndi_c_set_float32_temp(x, y)
#define ndi_set_zaid_float64_temp(x, y) ndi_c_set_float64_temp(x, y)
#define ndi_set_zaid_float32_temp_sig_0(x, y) ndi_c_set_float32_temp_sig_0(x, y)
#define ndi_set_zaid_float64_temp_sig_0(x, y) ndi_c_set_float64_temp_sig_0(x, y)
#define ndi_set_reaction(x) ndi_c_set_reaction(x)
#define ndi_get_size(x, y) ndi_c_get_size(x, y)
#define ndi_get_size_n(x, y, z) ndi_c_get_size_n(x, y, z)
#define ndi_get_size_x(x, y, z) ndi_c_get_size_x(x, y, z)
#define ndi_set_option(x, y) ndi_c_set_option(x, y)
#define ndi_check_option(x, y, z) ndi_c_check_option(x, y, z)
#define ndi_get_int_val(x, y) ndi_c_get_int_val(x, y)
#define ndi_get_int_val_n(x, y, z) ndi_c_get_int_val_n(x, y, z)
#define ndi_get_int_vec(x, y, z) ndi_c_get_int_vec(x, y, z)
#define ndi_get_int_vec_n(w, x, y, z) ndi_c_get_int_vec_n(w, x, y, z)
#define ndi_get_int_vec_x(w, x, y, z) ndi_c_get_int_vec_x(w, x, y, z)
#define ndi_get_float32_val(x, y) ndi_c_get_float32_val(x, y)
#define ndi_get_float32_val_n(x, y, z) ndi_c_get_float32_val_n(x, y, z)
#define ndi_get_float32_vec(x, y, z) ndi_c_get_float32_vec(x, y, z)
#define ndi_get_float32_vec_n(w, x, y, z) ndi_c_get_float32_vec_n(w, x, y, z)
#define ndi_get_float32_vec_x(w, x, y, z) ndi_c_get_float32_vec_x(w, x, y, z)
#define ndi_get_float64_val(x, y) ndi_c_get_float64_val(x, y)
#define ndi_get_float64_val_n(x, y, z) ndi_c_get_float64_val_n(x, y, z)
#define ndi_get_float64_vec(x, y, z) ndi_c_get_float64_vec(x, y, z)
#define ndi_get_float64_vec_n(w, x, y, z) ndi_c_get_float64_vec_n(w, x, y, z)
#define ndi_get_float64_vec_x(w, x, y, z) ndi_c_get_float64_vec_x(w, x, y, z)
#define ndi_get_string_val(x, y, z) ndi_c_get_string_val(x, y, z)
#define ndi_get_string_val_n(w, x, y, z) ndi_c_get_string_val_n(w, x, y, z)
#define ndi_get_string_vec(x, y, z) ndi_c_get_string_vec(x, y, z)
#define ndi_get_string_vec_x(w, x, y, z) ndi_c_get_string_vec_x(w, x, y, z)
#define ndi_close() ndi_c_close()
#define ndi_c_to_mt(x)  ndi_c_c_to_mt(x)
#define ndi_mt_to_c(x) ndi_c_mt_to_c(x)

#endif				/* NDI_NO_NAME_SUB */

#ifdef	__cplusplus
}
#endif
#endif				/* NDI_H */
