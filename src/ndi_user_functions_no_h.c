#include <stdio.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_var_type.h"

/**********************************************************************/

int
ndi_c_init(void)
{

    if (default_h) {
	/* ndi_close_h(&default_h); */
    }
    return ndi_c_init_h(&default_h, "");
}

/**********************************************************************/

int
ndi_c_set_zaid(const char *in_zaid)
{
    return ndi_c_set_zaid_h(default_h, in_zaid);
}

/**********************************************************************/

int
ndi_c_set_isotope(const char *in_zaid)
{
    return ndi_c_set_isotope_h(default_h, in_zaid);
}

/**********************************************************************/

int
ndi_c_set_reaction(const char *in_zaid)
{
    return ndi_c_set_reaction_h(default_h, in_zaid);
}

/**********************************************************************/

int
ndi_c_get_size(int in_key, int *ierr)
{
    return ndi_c_get_size_h(default_h, in_key, ierr);
}

/**********************************************************************/

int
ndi_c_get_size_n(int in_key, int vec_num, int *ierr)
{
    return ndi_c_get_size_n_h(default_h, in_key, vec_num, ierr);
}

/**********************************************************************/

int
ndi_c_get_size_x(int in_key, const char *in_str, int *ierr)
{
    return ndi_c_get_size_x_h(default_h, in_key, in_str, ierr);
}

/**********************************************************************/

int
ndi_c_set_option(int in_key, const char *in_value)
{
    return ndi_c_set_option_h(default_h, in_key, in_value);
}

/**********************************************************************/

int
ndi_c_check_option(int in_key, char *out_value, int string_size)
{
    return ndi_c_check_option_h(default_h, in_key, out_value, string_size);
}

/**********************************************************************/

int
ndi_c_get_int_val(int in_key, int *ret_val)
{
    return ndi_c_get_int_val_h(default_h, in_key, ret_val);
}

/**********************************************************************/

int
ndi_c_get_int_val_n(int in_key, int vec_num, int *ret_val)
{
    return ndi_c_get_int_val_n_h(default_h, in_key, vec_num, ret_val);
}

/**********************************************************************/

int
ndi_c_get_int_vec(int in_key, int *return_vec, int vec_size)
{
    return ndi_c_get_int_vec_h(default_h, in_key, return_vec, vec_size);
}

/**********************************************************************/

int
ndi_c_get_int_vec_n(int in_key, int vec_num, int *return_vec, int vec_size)
{
    return ndi_c_get_int_vec_n_h(default_h, in_key, vec_num, return_vec,
				 vec_size);
}

/**********************************************************************/

int
ndi_c_get_float32_val(int in_key, ndi_float32 * ret_val)
{
    return ndi_c_get_float32_val_h(default_h, in_key, ret_val);
}

/**********************************************************************/

int
ndi_c_get_float32_val_n(int in_key, int vec_num, ndi_float32 * ret_val)
{
    return ndi_c_get_float32_val_n_h(default_h, in_key, vec_num, ret_val);
}

/**********************************************************************/

int
ndi_c_get_float32_vec(int in_key, ndi_float32 * return_vec,
		      int return_size)
{
    return ndi_c_get_float32_vec_h(default_h, in_key, return_vec,
				   return_size);
}

/**********************************************************************/

int
ndi_c_get_float32_vec_n(int in_key, int vec_num,
			ndi_float32 * return_vec, int return_size)
{
    return ndi_c_get_float32_vec_n_h(default_h, in_key, vec_num,
				     return_vec, return_size);
}

/**********************************************************************/

int
ndi_c_get_float32_vec_x(int in_key, const char *in_str,
			ndi_float32 * return_vec, int vec_size)
{
    return ndi_c_get_float32_vec_x_h(default_h, in_key, in_str, return_vec,
				     vec_size);
}

/**********************************************************************/

int
ndi_c_get_float64_val(int in_key, ndi_float64 * ret_val)
{
    return ndi_c_get_float64_val_h(default_h, in_key, ret_val);
}

/**********************************************************************/

int
ndi_c_get_float64_val_n(int in_key, int vec_num, ndi_float64 * ret_val)
{
    return ndi_c_get_float64_val_n_h(default_h, in_key, vec_num, ret_val);
}

/**********************************************************************/

int
ndi_c_get_float64_vec(int in_key, ndi_float64 * return_vec,
		      int return_size)
{
    return ndi_c_get_float64_vec_h(default_h, in_key, return_vec,
				   return_size);
}

/**********************************************************************/

int
ndi_c_get_float64_vec_n(int in_key, int vec_num,
			ndi_float64 * return_vec, int return_size)
{
    return ndi_c_get_float64_vec_n_h(default_h, in_key, vec_num,
				     return_vec, return_size);
}

/**********************************************************************/

int
ndi_c_get_float64_vec_x(int in_key, const char *in_str,
			ndi_float64 * return_vec, int vec_size)
{
    return ndi_c_get_float64_vec_x_h(default_h, in_key, in_str, return_vec,
				     vec_size);
}

/**********************************************************************/

int
ndi_c_get_string_val(int in_key, char *ret_val, int str_size)
{
    return ndi_c_get_string_val_h(default_h, in_key, ret_val, str_size);
}

/**********************************************************************/

int
ndi_c_get_string_val_n(int in_key, int vec_num, char *ret_val,
		       int str_size)
{
    return ndi_c_get_string_val_n_h(default_h, in_key, vec_num, ret_val,
				    str_size);
}

/**********************************************************************/

int
ndi_c_close(void)
{
    return (ndi_c_close_h(&default_h));
}
