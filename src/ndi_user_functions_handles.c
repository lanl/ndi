#include <stdio.h>
#include <assert.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_var_type.h"

/*!
  \fn ndi2_open_gendir
  \brief initialize level 0 (gendir) handle
  \param handle: level 0 handle
  \param path: gendir path; "" for default
  \return 0 if successful; negative if error
*/
int ndi2_open_gendir(int *handle, const char *path)
{

    int ierr = 0;
    state_struct *new_state = NULL;

    ierr = ndi_c_init_h(&new_state, path);
    *handle = set_handle(new_state);

    if (*handle < 0) {
	ierr = -1;		/* handle array exhausted */
    }

    assert(is_gendir(*handle));
    return (ierr);
}

/*!
  \fn ndi2_set_option_gendir
  \brief set global option
  \param handle: gendir handle
  \param in_key: option key
  \param value: option value
  \return 0 if successful, negative if error
*/
int ndi2_set_option_gendir(int handle, int in_key, const char *value)
{
    state_struct *state_p;

    if (is_gendir(handle)) {
	state_p = handle_to_state_p(handle);
	return (ndi_c_set_option_h(state_p, in_key, value));
    } else {
	return -2;
    }
}

/*!
  \fn ndi2_check_option_gendir
  \brief check global options
  \param handle: gendir handle
  \param in_key: option key
  \param out_value: option value
  \param string_size: size of out_value
  \return 0 if successful, negative if error
*/
int ndi2_check_option_gendir(int handle, int in_key, char *out_value,
		  int string_size)
{
    state_struct *state_p;

    assert(is_gendir(handle));
    state_p = handle_to_state_p(handle);
    return (ndi_c_check_option_h(state_p, in_key, out_value, string_size));
}

int ndi2_get_handle_gendir(int handle0, int *handle1)
{
    assert(is_gendir(handle0));	/* require gendir handle */
    return ndi2_get_handle(handle0, handle1);
}

int ndi2_close_gendir(int handle)
{
    assert(is_gendir(handle));
    return (__ndi_close_handle(handle));
}

int ndi2_set_lib_type(int handle, const char *type)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_set_option_h(state_p, NDI_LIB_TYPE, type));
}

int ndi2_set_isotope(int handle, const char *zaid)
{
    state_struct *state_p;

    /* if (is_gendir(handle)) return -2; */

    state_p = handle_to_state_p(handle);
    return (ndi_c_set_isotope_h(state_p, zaid));
}

int ndi2_set_zaid(int handle, const char *zaid)
{
    state_struct *state_p;

    /* if (is_gendir(handle)) return -2; */

    state_p = handle_to_state_p(handle);
    return (ndi_c_set_zaid_h(state_p, zaid));
}

int ndi2_set_reaction(int handle, const char *zaid)
{
    state_struct *state_p;

    /* if (is_gendir(handle) return -2; */

    state_p = handle_to_state_p(handle);
    return (ndi_c_set_reaction_h(state_p, zaid));
}

int ndi2_get_size(int handle, int in_key, int *ierr)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);

    if (is_mpmg(state_p)) {
	return (ndimp_get_size(state_p->handle_map, 
			       state_p->handle_free, 
			       in_key, ierr));
    } else {
	return (ndi_c_get_size_h(state_p, 
				 in_key, ierr));
    }
}

int ndi2_get_size_n(int handle, int in_key, int vec_num, int *ierr)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);

    if (is_mpmg(state_p)) {
	return (ndimp_get_size_n(state_p->handle_map, 
				 state_p->handle_free, 
				 in_key, vec_num, ierr));
    } else {
	return (ndi_c_get_size_n_h(state_p, 
				   in_key, vec_num, ierr));
    }
}

int ndi2_get_size_x(int handle, int in_key, const char *in_str,
		int *ierr)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_size_x_h(state_p, in_key, in_str, ierr));
}

/*!
  \fn ndi2_set_option
  \brief set handle option
  \param handle: handle
  \param in_key: option key
  \param value: option value
  \return 0 if successful, negative if error
*/
int ndi2_set_option(int handle, int in_key, const char *value)
{
    state_struct *state_p;

    /* if (is_gendir(handle)) return -2; */
  
    state_p = handle_to_state_p(handle);

    if (is_mpmg(state_p)) {
	return (ndimp_set_option(state_p, in_key, value));
    } else {
        return (ndi_c_set_option_h(state_p, in_key, value));
    }
}

int ndi2_set_float64_vec_option(int handle, int in_key,
			    const ndi_float64 * option_array,
			    int array_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_set_float64_vec_option_h(state_p, in_key, option_array, 
					   array_size));
}

int ndi2_check_option(int handle, int in_key, char *out_value,
		  int string_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_check_option_h(state_p, in_key, out_value, string_size));
}

int ndi2_get_int_val(int handle, int in_key, int *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);

    if (is_mpmg(state_p)) {
	return (ndimp_get_int_val(state_p->handle_map, 
				  state_p->handle_free, 
				  in_key, ret_val));
    } else {
	return (ndi_c_get_int_val_h(state_p, 
				    in_key, ret_val));
    }
}

int ndi2_get_int_val_n(int handle, int in_key, int vec_num, int *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);

    if (is_mpmg(state_p)) {
	return (ndimp_get_int_val_n(state_p->handle_map, 
				    state_p->handle_free, 
				    in_key, vec_num, ret_val));
    } else {
	return (ndi_c_get_int_val_n_h(state_p, 
				      in_key, vec_num, ret_val));
    }
}

int ndi2_get_int_vec(int handle, int in_key, int *return_vec,
		 int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);

    if (is_mpmg(state_p)) {
	return (ndimp_get_int_vec(state_p->handle_map, 
				  state_p->handle_free, 
				  in_key, return_vec, vec_size));
    } else {
	return (ndi_c_get_int_vec_h(state_p, 
				    in_key, return_vec, vec_size));
    }
}

int ndi2_get_int_vec_n(int handle, int in_key, int vec_num,
		   int *return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_vec_n_h(state_p, in_key, vec_num, 
				  return_vec, vec_size));
}

int ndi2_get_float32_val(int handle, int in_key, ndi_float32 * ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float32_val_h(state_p, in_key, ret_val));
}

int ndi2_get_float32_val_n(int handle, int in_key, int vec_num,
		       ndi_float32 * ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float32_val_n_h(state_p, in_key, vec_num, ret_val));
}

int ndi2_get_float32_vec(int handle, int in_key,
		     ndi_float32 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    
    if (is_mpmg(state_p)) {
	return (ndimp_get_float32_vec(state_p->handle_map, 
				      state_p->handle_free, 
				      in_key, return_vec, vec_size));
    } else {
	return (ndi_c_get_float32_vec_h(state_p, 
					in_key, return_vec, vec_size));
    }
}

int ndi2_get_float32_vec_n(int handle, int in_key, int vec_num,
		       ndi_float32 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);

    if (is_mpmg(state_p)) {
	return (ndimp_get_float32_vec_n(state_p->handle_map, 
					state_p->handle_free, 
					in_key, vec_num, 
					return_vec, vec_size));
    } else {
	return (ndi_c_get_float32_vec_n_h(state_p, 
					  in_key, vec_num, 
					  return_vec, vec_size));
    }
}

int ndi2_get_float32_vec_x(int handle, int in_key,
		       const char *in_str, ndi_float32 * return_vec,
		       int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float32_vec_x_h(state_p, in_key, in_str, 
				      return_vec, vec_size));
}

int ndi2_get_float64_val(int handle, int in_key, ndi_float64 * ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float64_val_h(state_p, in_key, ret_val));
}

int ndi2_get_float64_val_n(int handle, int in_key, int vec_num,
		       ndi_float64 * ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float64_val_n_h(state_p, in_key, vec_num, ret_val));
}

int ndi2_get_float64_vec(int handle, int in_key,
		     ndi_float64 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    
if (is_mpmg(state_p)) {
	return (ndimp_get_float64_vec(state_p->handle_map, 
				      state_p->handle_free, 
				      in_key, return_vec, vec_size));
    } else {
	return (ndi_c_get_float64_vec_h(state_p, 
					in_key, return_vec, vec_size));
    }
}

int ndi2_get_float64_vec_n(int handle, int in_key, int vec_num,
		       ndi_float64 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);

    if (is_mpmg(state_p)) {
	return (ndimp_get_float64_vec_n(state_p->handle_map, 
					state_p->handle_free, 
					in_key, vec_num, 
					return_vec, vec_size));
    } else {
	return (ndi_c_get_float64_vec_n_h(state_p, 
					  in_key, vec_num, 
					  return_vec, vec_size));
    }
}

int ndi2_get_float64_vec_x(int handle, int in_key,
		       const char *in_str, ndi_float64 * return_vec,
		       int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float64_vec_x_h(state_p, in_key, in_str, 
				      return_vec, vec_size));
}

int ndi2_get_string_val(int handle, int in_key, char *ret_val,
		    int str_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);

    if (is_mpmg(state_p)) {
	return (ndimp_get_string_val(state_p->handle_map, 
				     state_p->handle_free, 
				     in_key, ret_val, str_size));
    } else {
	return (ndi_c_get_string_val_h(state_p, 
				       in_key, ret_val, str_size));
    }
}

int ndi2_get_string_val_n(int handle, int in_key, int vec_num,
		      char *ret_val, int str_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_string_val_n_h(state_p, in_key, vec_num, 
				     ret_val, str_size));
}

int ndi2_get_handle_mpmg(int handle0, int *handle1)
{
    return ndi2_get_handle(handle0, handle1);
}

/*!
  \fn ndi2_get_handle
  \brief get child handle from parent handle
  \param handle0: parent handle
  \param handle1: child handle
  \return 0 if successful, negative if error
*/
int ndi2_get_handle(int handle0, int *handle1)
{
    state_struct *parent;

    parent = handle_to_state_p(handle0);	/* get parent state */
    *handle1 = (handle0 + 1) * MAX_HANDLE_NUM + parent->handle_free;

    return(__ndi_add_child_handle(parent));
    

}

/* obsolete: replace with above */
int ndi2_open(int *handle, const char *path)
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

/* obsolete */
int ndi2_close(int handle)
{
    return (__ndi_close_handle(handle));
}


