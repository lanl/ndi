#include <stdio.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_var_type.h"


int ndi2_open(int *handle, const char *path)
{

    int ierr = 0;
    state_struct *new_state = NULL;

    ierr = ndi_c_init_h(&new_state, path);
    *handle = set_handle(new_state);

    if (*handle < 0) {
	ierr = -1;
    }

    return (ierr);
}

int ndi2_set_isotope(const int handle, const char *zaid)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_set_isotope_h(state_p, zaid));
}


int ndi2_set_zaid(const int handle, const char *zaid)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_set_zaid_h(state_p, zaid));
}

int ndi2_get_isotope(const int handle, const char *zaid, int *handle2)
{
    int i;
    int h;
    state_struct *parent;
    state_struct *child;

    parent = handle_to_state_p(handle);	/* get parent state */
    for (i = 0; i < MAX_HANDLE_NUM; i++) {
	if (!parent->handle_map[i]) {	/* use first empty element */
	    h = i;
	    ndi_c_clone_h(parent, &child);
	    if (child) {
		child->handle = h;	/* assign child local handle */
		parent->handle_map[i] = child;
	    }
	    break;
	}
    }
    *handle2 = (parent->handle + 1) * MAX_HANDLE_NUM + child->handle;
    return (ndi_c_set_zaid_h(child, zaid));
}

int ndi2_get_zaid(const int handle, const char *zaid, int *handle2)
{
    return (ndi2_get_isotope(handle, zaid, handle2));
}

int ndi2_set_reaction(const int handle, const char *zaid)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_set_reaction_h(state_p, zaid));
}

int ndi2_get_size(const int handle, int in_key, int *ierr)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_size_h(state_p, in_key, ierr));
}

int ndi2_get_size_n(const int handle, int in_key, int vec_num, int *ierr)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_size_n_h(state_p, in_key, vec_num, ierr));
}

int
ndi2_get_size_x(const int handle, int in_key, const char *in_str,
		int *ierr)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_size_x_h(state_p, in_key, in_str, ierr));
}

int ndi2_set_option(const int handle, int in_key, const char *value)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_set_option_h(state_p, in_key, value));
}

int
ndi2_set_float64_vec_option(const int handle, int in_key,
			    const ndi_float64 * option_array,
			    const int array_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_set_float64_vec_option_h
	    (state_p, in_key, option_array, array_size));
}

int
ndi2_check_option(const int handle, int in_key, char *out_value,
		  int string_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_check_option_h(state_p, in_key, out_value, string_size));
}

int ndi2_get_int_val(const int handle, int in_key, int *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_val_h(state_p, in_key, ret_val));
}

int
ndi2_get_int_val_n(const int handle, int in_key, int vec_num, int *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_val_n_h(state_p, in_key, vec_num, ret_val));
}

int
ndi2_get_int_vec(const int handle, int in_key, int *return_vec,
		 int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_vec_h(state_p, in_key, return_vec, vec_size));
}

int
ndi2_get_int_vec_n(const int handle, int in_key, int vec_num,
		   int *return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_vec_n_h
	    (state_p, in_key, vec_num, return_vec, vec_size));
}

int
ndi2_get_float32_val(const int handle, int in_key, ndi_float32 * ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float32_val_h(state_p, in_key, ret_val));
}

int
ndi2_get_float32_val_n(const int handle, int in_key, int vec_num,
		       ndi_float32 * ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float32_val_n_h(state_p, in_key, vec_num, ret_val));
}

int
ndi2_get_float32_vec(const int handle, int in_key,
		     ndi_float32 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float32_vec_h
	    (state_p, in_key, return_vec, vec_size));
}

int
ndi2_get_float32_vec_n(const int handle, int in_key, int vec_num,
		       ndi_float32 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float32_vec_n_h
	    (state_p, in_key, vec_num, return_vec, vec_size));
}

int
ndi2_get_float32_vec_x(const int handle, int in_key,
		       const char *in_str, ndi_float32 * return_vec,
		       int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float32_vec_x_h
	    (state_p, in_key, in_str, return_vec, vec_size));
}

int
ndi2_get_float64_val(const int handle, int in_key, ndi_float64 * ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float64_val_h(state_p, in_key, ret_val));
}

int
ndi2_get_float64_val_n(const int handle, int in_key, int vec_num,
		       ndi_float64 * ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float64_val_n_h(state_p, in_key, vec_num, ret_val));
}

int
ndi2_get_float64_vec(const int handle, int in_key,
		     ndi_float64 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float64_vec_h
	    (state_p, in_key, return_vec, vec_size));
}

int
ndi2_get_float64_vec_n(const int handle, int in_key, int vec_num,
		       ndi_float64 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float64_vec_n_h
	    (state_p, in_key, vec_num, return_vec, vec_size));
}

int
ndi2_get_float64_vec_x(const int handle, int in_key,
		       const char *in_str, ndi_float64 * return_vec,
		       int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float64_vec_x_h
	    (state_p, in_key, in_str, return_vec, vec_size));
}

int
ndi2_get_string_val(const int handle, int in_key, char *ret_val,
		    int str_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_string_val_h(state_p, in_key, ret_val, str_size));
}

int
ndi2_get_string_val_n(const int handle, int in_key, int vec_num,
		      char *ret_val, int str_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_string_val_n_h
	    (state_p, in_key, vec_num, ret_val, str_size));
}

int ndi2_close(const int handle)
{

    return (__ndi_close_handle(handle));
}
