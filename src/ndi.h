/*
 * File:	ndi.h
 * Description:	User header file for NDI library.
 * Time-stamp:	<2000-02-02 15:31:03 jomc>
 *
 * This header file is included in user code and library code.
 */

/**********************************************************************/

/*
 * Defines
 */

#ifndef __NDI_H
#define __NDI_H

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

int NDI2_init(int *handle);
int NDI2_set_isotope(const int handle, const char *zaid );
int NDI2_set_zaid(const int handle, const char *zaid );
int NDI2_set_reaction(const int handle, const char *zaid );
int NDI2_get_size(const int handle, int in_key, int *ierr );
int NDI2_get_size_n(const int handle, int in_key, int vec_num, int *ierr );
int NDI2_get_size_x(const int handle, int in_key, const char *in_str, int *ierr);
int NDI2_set_option (const int handle,  int in_key, const char *value );
int NDI2_check_option (const int handle,  int in_key, char *out_value, int string_size ) ;
int NDI2_get_int_val(const int handle, int in_key, int *ret_val );
int NDI2_get_int_val_n(const int handle, int in_key, int vec_num, int *ret_val );
int NDI2_get_int_vec(const int handle, int in_key, int *return_vec, int vec_size );
int NDI2_get_int_vec_n(const int handle, int in_key, int vec_num, int *return_vec, 
			      int vec_size );
int NDI2_get_float32_val(const int handle, int in_key, ndi_float32 *ret_val);
int NDI2_get_float32_val_n(const int handle, int in_key, int vec_num, ndi_float32 *ret_val );
int NDI2_get_float32_vec(const int handle, int in_key, ndi_float32 *return_vec, int vec_size );
int NDI2_get_float32_vec_n(const int handle, int in_key, int vec_num, ndi_float32 *return_vec, 
				  int vec_size);
int NDI2_get_float32_vec_x(const int handle, int in_key, const char *in_str,  ndi_float32 *return_vec, 
				  int vec_size);
int NDI2_get_float64_val(const int handle, int in_key, ndi_float64 *ret_val);
int NDI2_get_float64_val_n(const int handle, int in_key, int vec_num, ndi_float64 *ret_val); 
int NDI2_get_float64_vec(const int handle, int in_key, ndi_float64 *return_vec, int vec_size);
int NDI2_get_float64_vec_n(const int handle, int in_key, int vec_num, ndi_float64 *return_vec, 
				  int vec_size);
int NDI2_get_float64_vec_x(const int handle, int in_key, const char *in_str,  ndi_float64 *return_vec, 
				  int vec_size); 
int NDI2_get_string_val(const int handle, int in_key, char *ret_val, int str_size);
int NDI2_get_string_val_n(const int handle, int in_key, int vec_num, char *ret_val, 
				 int str_size);
int NDI2_close(const int handle);

int ndimp_get_float32_vec(const int *handle_arr, const int num_parts, 
                         const int in_key,ndi_float32 *ret_vec, const int vec_size);
int ndimp_get_float64_vec(const int *handle_arr, const int num_parts, 
                         const int in_key, ndi_float64 *ret_vec, const int vec_size);
int ndimp_get_int_val(const int *handle_arr, const int num_parts, 
                       const int in_key, int *return_val);
int ndimp_get_int_val_n(const int *handle_arr, const int num_parts, 
                         const int in_key, const int vec_num, int *ret_val);
int ndimp_get_int_vec(const int *handle_arr, const int num_parts, 
                       const int in_key, int *ret_vec, const int vec_size);
int ndimp_get_size(const int *handle_arr, const int num_parts, 
                   int in_key, int *ierr);
int ndimp_get_string_val(const int *handle_arr, const int num_parts, 
                          const int in_key, char *ret_val, const int str_size);

/**********************************************************************/
     /* Backwards compatibility user functions */

int     ndid_c_get_size(int in_key, const char *in_value, int *arr_size);
int     ndid_c_get_num(int in_key, const char *in_value, int *arr_num);
int     ndid_c_get_string_vec(int in_key, const char *in_value, char *zaid, int arr_size);
int     ndid_c_zaid_float32_vec(int in_key, const char *in_value, char *zaid, int arr_size,
				  ndi_float32 *float_vec, int fl_arr_size );
int     ndid_c_zaid_float64_vec(int in_key, const char *in_value, char *zaid, int arr_size, 
				  ndi_float64 *float_vec, int fl_arr_size );

int     ndi_c_init(void);
int     ndi_c_set_isotope(const char *zaid );
int     ndi_c_set_zaid(const char *zaid );
int     ndi_c_set_reaction(const char *zaid );
int     ndi_c_get_size(int in_key, int *ierr );
int     ndi_c_get_size_n(int in_key, int vec_num, int *ierr );
int     ndi_c_get_size_x(int in_key, const char *in_str, int *ierr);
int     ndi_c_set_option ( int in_key, const char *value );
int     ndi_c_check_option ( int in_key, char *out_value, int string_size ) ;
int     ndi_c_get_int_val(int in_key, int *ret_val );
int     ndi_c_get_int_val_n(int in_key, int vec_num, int *ret_val );
int     ndi_c_get_int_vec(int in_key, int *return_vec, int vec_size );
int     ndi_c_get_int_vec_n(int in_key, int vec_num, int *return_vec, 
			      int vec_size );
int     ndi_c_get_float32_val(int in_key, ndi_float32 *ret_val);
int     ndi_c_get_float32_val_n(int in_key, int vec_num, ndi_float32 *ret_val );
int     ndi_c_get_float32_vec(int in_key, ndi_float32 *return_vec, int vec_size );
int     ndi_c_get_float32_vec_n(int in_key, int vec_num, ndi_float32 *return_vec, 
				  int vec_size);
int     ndi_c_get_float32_vec_x(int in_key, const char *in_str,  ndi_float32 *return_vec, 
				  int vec_size);
int     ndi_c_get_float64_val(int in_key, ndi_float64 *ret_val);
int     ndi_c_get_float64_val_n(int in_key, int vec_num, ndi_float64 *ret_val); 
int     ndi_c_get_float64_vec(int in_key, ndi_float64 *return_vec, int vec_size);
int     ndi_c_get_float64_vec_n(int in_key, int vec_num, ndi_float64 *return_vec, 
				  int vec_size);
int     ndi_c_get_float64_vec_x(int in_key, const char *in_str,  ndi_float64 *return_vec, 
				  int vec_size); 
int     ndi_c_get_string_val(int in_key, char *ret_val, int str_size);
int     ndi_c_get_string_val_n(int in_key, int vec_num, char *ret_val, 
				 int str_size);
int     ndi_c_close(void);

int     ndi_c_c_to_mt (int c_val);
int     ndi_c_mt_to_c (int mt);

/**********************************************************************/

  /* Rename functions */

#ifndef __NDI_NO_NAME_SUB
#define ndid_get_size(x, y, z) ndid_c_get_size(x, y, z)
#define ndid_get_num(x, y, z) ndid_c_get_num(x, y, z)
#define ndid_get_string_vec(w, x, y, z) ndid_c_get_string_vec(w, x, y, z)
#define ndid_zaid_float32_vec(a, b, c, x, y, z) ndid_c_zaid_float32_vec(a, b, c, x, y, z)
#define ndid_zaid_float64_vec(a, b, c, x, y, z) ndid_c_zaid_float64_vec(a, b, c, x, y, z)


#define ndi_init() ndi_c_init()
#define ndi_set_isotope(x) ndi_c_set_isotope(x)
#define ndi_set_zaid(x) ndi_c_set_zaid(x)
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
#define ndi_close() ndi_c_close()
#define ndi_c_to_mt(x)  ndi_c_c_to_mt(x)
#define ndi_mt_to_c(x) ndi_c_mt_to_c(x)

#endif /* __NDI_NO_NAME_SUB */

#ifdef	__cplusplus
}
#endif

#endif	/* __NDI_H */
