#include <stdio.h>
#include <assert.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_var_type.h"

/*!
  \brief create gendir handle
  \param handle: level 0 handle
  \param path: gendir path string; "" for default path
  \return 0 if successful; negative if error
*/
int NDI2_open_gendir(int *handle, const char *path)
{

    int ierr = 0;
    state_struct *new_state = NULL;

    ierr = ndi_c_init_h(&new_state, path);
    *handle = set_handle(new_state);

    if (*handle < 0) {
	ierr = -1;		/* handle array exhausted */
    }

    return (ierr);
}

/*!
  \brief set global option
  \param handle: gendir handle
  \param in_key: option key
  \param value: option value
  \return 0 if successful, negative if error
*/
int NDI2_set_option_gendir(int handle, int in_key, const char *value)
{
    int new_inkey;
    state_struct *state_p;

    state_p = handle_to_state_p(handle);

    if (is_gendir(state_p)) {
        if ((in_key >= 1006) && (in_key <= 1100)) {
            new_inkey = in_key - 1000;
            return (ndi_c_set_option_h(state_p, new_inkey, value));
        } else {
            return -4;
        }
    } else {
	return -2;
    }
}

/*!
  \brief check global options
  \param handle: gendir handle
  \param in_key: option key
  \param out_value: option value
  \param string_size: size of out_value
  \return 0 if successful, negative if error
*/
int NDI2_check_option_gendir(int handle, int in_key, char *out_value,
                             int string_size)
{
    int new_inkey;
    state_struct *state_p;

    if ((out_value) && (string_size > 0))
        *out_value = '\0';

    state_p = handle_to_state_p(handle);

    if (is_gendir(state_p)) {
        if ((in_key >= 1006) && (in_key <= 1100)) {
            new_inkey = in_key - 1000;
            return (ndi_c_check_option_h(state_p, new_inkey, out_value, 
					 string_size));
        } else {
            return -4;
        }
    } else {
	return -2;
    }

}

/*!
  \brief release gendir handle
  \param handle: gendir handle
  \return 0 if successful, negative if error
*/
int NDI2_close_gendir(int handle)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);

    if (is_gendir(state_p)) {
        return (__ndi_close_handle(handle));
    } else {
	return -2;
    }
}

/*!
  \brief make child handle of type from parent handle
  \param handle0: parent handle
  \param type: library type
  \param handle1: child handle
  \return 0 if successful, negative if error
*/
int NDI2_make_handle(int handle0, const char *type, int *handle1)
{
    state_struct *parent;
    state_struct *child;

    *handle1 = -1;		/* invalid handle */
    parent = handle_to_state_p(handle0); /* get parent state */
    if (parent)			/* calculate child handle */
        *handle1 = (handle0 + 1) * MAX_HANDLE_NUM + parent->handle_free;
    
    if(__ndi_add_child_handle(parent) != 0) /* add child handle */
	return -1;

    child = handle_to_state_p(*handle1);	/* get child state */
    if (ndi_is_library_type(child, type))
	return (0);
    else
	return (ndi_c_set_option_h(child, NDI_LIB_TYPE, type));
}

/*!
  \brief set zaid for multi-group transport data handle
  \param handle: handle
  \param zaid: Z, A id
  \return 0 if successful, negative if error
*/
int NDI2_set_isotope(int handle, const char *zaid)
{
    return NDI2_set_zaid(handle, zaid);
}

/*!
  \brief set zaid for multi-group transport data handle
  \param handle: handle
  \param zaid: Z, A id
  \return 0 if successful, negative if error
*/
int NDI2_set_zaid(int handle, const char *zaid)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    
    if (is_gendir(state_p)) {
        return -2;
    } else if (is_mpmg(state_p)) {
        return (ndimp_set_zaid(state_p, zaid ));
    } else {
        return (ndi_c_set_zaid_h(state_p, zaid));
    }
}

/*!
  \brief set zaid for handle
  \param handle: handle
  \param zaid: Z, A id
  \return 0 if successful, negative if error
*/
int NDI2_set_reaction(int handle, const char *zaid)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    if (is_gendir(state_p)) {
        return -2;
    } else if (is_mpmg(state_p)) {
        return -4;
    } else {
        return (ndi_c_set_reaction_h(state_p, zaid));
    }
}

/*!
  \brief get size for keyword data from handle
  \param handle: handle
  \param in_key: keyword
  \param ierr: 0 if successful, negative if error
  \return size
*/
int NDI2_get_size(int handle, int in_key, int *ierr)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_size_h(state_p, in_key, ierr));
}

/*!
  \brief get size for n-th keyword data from handle
  \param handle: handle
  \param in_key: keyword
  \param vec_num: index
  \param ierr: 0 if successful, negative if error
  \return size
*/
int NDI2_get_size_n(int handle, int in_key, int vec_num, int *ierr)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_size_n_h(state_p, in_key, vec_num, ierr));
}

int NDI2_get_size_x(int handle, int in_key, const char *in_str,
                    int *ierr)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_size_x_h(state_p, in_key, in_str, ierr));
}

/*!
  \brief set string option for handle
  \param handle: handle
  \param in_key: option key
  \param value: option value
  \return 0 if successful, negative if error
*/
int NDI2_set_option(int handle, int in_key, const char *value)
{
    state_struct *state_p;

    /* if (is_gendir(handle)) return -2; */
  
    state_p = handle_to_state_p(handle);

    if (is_mpmg(state_p) && in_key == NDI_LIBRARY) {
	return (ndimp_set_option(state_p, in_key, value));
    } else {
        return (ndi_c_set_option_h(state_p, in_key, value));
    }
}

/*!
  \brief set float64 vector option for handle
  \param handle: handle
  \param in_key: option key
  \param option_array: option values
  \param array_size: size of option array
  \return 0 if successful, negative if error
*/
int NDI2_set_float64_vec_option(int handle, int in_key,
			    const ndi_float64 * option_array,
			    int array_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    if (is_gendir(state_p)) {
        /* Case not handled currently */
        return -2;
    } else {
        return (ndi_c_set_float64_vec_option_h(state_p, in_key, option_array, 
                                               array_size));
    }
}

/*!
  \brief get string option value for handle
  \param handle: handle
  \param in_key: option key
  \param out_value: option value
  \param string_size: maximum size of option string
  \return 0 if successful, negative if error
*/
int NDI2_check_option(int handle, int in_key, char *out_value, int string_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_check_option_h(state_p, in_key, out_value, string_size));
}

/*!
  \brief get integer value from handle
  \param handle: handle
  \param in_key: option key
  \param ret_val: return value
  \return 0 if successful, negative if error
*/
int NDI2_get_int_val(int handle, int in_key, int *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_val_h(state_p, in_key, ret_val));
}

/*!
  \brief get n-th integer value from handle
  \param handle: handle
  \param in_key: option key
  \param vec_num: index of requested value
  \param ret_val: return value
  \return 0 if successful, negative if error
*/
int NDI2_get_int_val_n(int handle, int in_key, int vec_num, int *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_val_n_h(state_p, in_key, vec_num, ret_val));
}

/*!
  \brief get integer vector of values from handle
  \param handle: handle
  \param in_key: option key
  \param return_vec: vector of values
  \param vec_size: maximum size of integer vector
  \return 0 if successful, negative if error
*/
int NDI2_get_int_vec(int handle, int in_key, int *return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_vec_h(state_p, in_key, return_vec, vec_size));
}

/*!
  \brief get n-th integer vector of values from handle
  \param handle: handle
  \param in_key: option key
  \param vec_num: index of vector
  \param return_vec: vector of values
  \param vec_size: maximum size of integer vector
  \return 0 if successful, negative if error
*/
int NDI2_get_int_vec_n(int handle, int in_key, int vec_num,
		   int *return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_vec_n_h(state_p, in_key, vec_num, 
                                      return_vec, vec_size));
}

/*!
  \brief get float value from handle
  \param handle: handle
  \param in_key: option key
  \param ret_val: float value
  \return 0 if successful, negative if error
*/
int NDI2_get_float32_val(int handle, int in_key, ndi_float32 *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float32_val_h(state_p, in_key, ret_val));
}

/*!
  \brief get n-th float value from handle
  \param handle: handle
  \param in_key: option key
  \param vec_num: index value
  \param ret_val: float value
  \return 0 if successful, negative if error
*/
int NDI2_get_float32_val_n(int handle, int in_key, int vec_num,
		       ndi_float32 *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    if (is_gendir(state_p)) {
        /* Case not handled currently */
        *ret_val = 0.0;
        return -2;
    } else {
        return (ndi_c_get_float32_val_n_h(state_p, in_key, vec_num, ret_val));
    }
}

/*!
  \brief get float vector values from handle
  \param handle: handle
  \param in_key: option key
  \param return_vec: float vector
  \param vec_size: maximum size of float vector
  \return 0 if successful, negative if error
*/
int NDI2_get_float32_vec(int handle, int in_key,
		     ndi_float32 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float32_vec_h(state_p, in_key, return_vec, vec_size));
}

/*!
  \brief get n-th float vector from handle
  \param handle: handle
  \param in_key: option key
  \param vec_num: index
  \param return_vec: float vector
  \param vec_size: maximum size of float vector
  \return 0 if successful, negative if error
*/
int NDI2_get_float32_vec_n(int handle, int in_key, int vec_num,
		       ndi_float32 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    if (is_mpmg(state_p)) {
	return (ndimp_get_float32_vec_n(state_p,
					in_key, vec_num, 
					return_vec, vec_size));
    } else if (is_gendir(state_p)) {
        /* Case not handled currently */
        (void)ndi_init_float32_vec(return_vec, vec_size);
        return -2;
    } else {
	return (ndi_c_get_float32_vec_n_h(state_p, 
					  in_key, vec_num, 
					  return_vec, vec_size));
    }
}

/*!
  \brief get named float vector values from handle
  \param handle: handle
  \param in_key: option key
  \param in_str: name
  \param return_vec: float vector
  \param vec_size: maximum size of float vector
  \return 0 if successful, negative if error
*/
int NDI2_get_float32_vec_x(int handle, int in_key,
		       const char *in_str, ndi_float32 * return_vec,
		       int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    if (is_gendir(state_p)) {
        /* Case not handled currently */
        (void)ndi_init_float32_vec(return_vec, vec_size);
        return -2;
   } else {
        return (ndi_c_get_float32_vec_x_h(state_p, in_key, in_str, 
                                          return_vec, vec_size));
    }
}

/*!
  \brief get float value from handle
  \param handle: handle
  \param in_key: option key
  \param ret_val: float value
  \return 0 if successful, negative if error
*/
int NDI2_get_float64_val(int handle, int in_key, ndi_float64 *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float64_val_h(state_p, in_key, ret_val));
}

/*!
  \brief get n-th float value from handle
  \param handle: handle
  \param in_key: option key
  \param vec_num: index value
  \param ret_val: float value
  \return 0 if successful, negative if error
*/
int NDI2_get_float64_val_n(int handle, int in_key, int vec_num,
                           ndi_float64 *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    if (is_gendir(state_p)) {
        /* Case not handled currently */
        *ret_val = 0.0;
        return -2;
    } else {
        return (ndi_c_get_float64_val_n_h(state_p, in_key, vec_num, ret_val));
    }
}

/*!
  \brief get float vector values from handle
  \param handle: handle
  \param in_key: option key
  \param return_vec: float vector
  \param vec_size: maximum size of float vector
  \return 0 if successful, negative if error
*/
int NDI2_get_float64_vec(int handle, int in_key,
		     ndi_float64 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float64_vec_h(state_p, in_key, return_vec, vec_size));
}

/*!
  \brief get n-th float vector from handle
  \param handle: handle
  \param in_key: option key
  \param vec_num: index
  \param return_vec: float vector
  \param vec_size: maximum size of float vector
  \return 0 if successful, negative if error
*/
int NDI2_get_float64_vec_n(int handle, int in_key, int vec_num,
		       ndi_float64 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    if (is_mpmg(state_p)) {
	return (ndimp_get_float64_vec_n(state_p,
					in_key, vec_num, 
					return_vec, vec_size));
    } else if (is_gendir(state_p)) {
        /* Case not handled currently */
        (void)ndi_init_float64_vec(return_vec, vec_size);
        return -2;
    } else {
	return (ndi_c_get_float64_vec_n_h(state_p, 
					  in_key, vec_num, 
					  return_vec, vec_size));
    }
}

/*!
  \brief get named float vector values from handle
  \param handle: handle
  \param in_key: option key
  \param in_str: name
  \param return_vec: float vector
  \param vec_size: maximum size of float vector
  \return 0 if successful, negative if error
*/
int NDI2_get_float64_vec_x(int handle, int in_key,
		       const char *in_str, ndi_float64 * return_vec,
		       int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    if (is_gendir(state_p)) {
        /* Case not handled currently */
        (void)ndi_init_float64_vec(return_vec, vec_size);
        return -2;
    } else {
        return (ndi_c_get_float64_vec_x_h(state_p, in_key, in_str, 
                                          return_vec, vec_size));
    }
}

/*!
  \brief get string value from handle
  \param handle: handle
  \param in_key: option key
  \param ret_val: string value
  \param str_size: maximum size of string
  \return 0 if successful, negative if error
*/
int NDI2_get_string_val(int handle, int in_key, char *ret_val,
                        int str_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_string_val_h(state_p, in_key, ret_val, str_size));
}

/*!
  \brief get n-th string value from handle
  \param handle: handle
  \param in_key: option key
  \param vec_num: index
  \param ret_val: string
  \param str_size: maximum size of string
  \return 0 if successful, negative if error
*/
int NDI2_get_string_val_n(int handle, int in_key, int vec_num,
                          char *ret_val, int str_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_string_val_n_h(state_p, in_key, vec_num, 
				     ret_val, str_size));
}

/*!
  \brief get string vector of values from handle
  \param handle: handle
  \param in_key: option key
  \param ret_val: vector of strings
  \param string_size: maximum size of strings
  \return 0 if successful, negative if error
*/
int NDI2_get_string_vec(int handle, int in_key, char *ret_val,
                        int str_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_string_vec_h(state_p, in_key, ret_val, str_size));
}

