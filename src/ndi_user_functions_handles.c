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
  \brief Set the string option indicated by \a value for \a handle

  \par Fortran example
  \code
  ! Set default library to "mendf6"
  call ndi2_set_option(handle, NDI_LIBRARY, "mendf6", ndi_err)
  \endcode

  \param handle: integer input state handle
  \param in_key: integer input key indicating the option set
  \param value: string input containing the option value
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only)

  \par Valid input keys:
  ======================

  - All \a handles:
      - \c NDI_LIB_TYPE
      - \c NDI_LIBRARY
      - \c NDI_MAT_ORDER
      - \c NDI_COLLAPSE
      - \c NDI_E_BOUNDS_CURR_PART
      - \c NDI_TRANS_CORR
      - \c NDI_NORM
      - \c NDI_REAC_ID
      - \c NDI_E_AVG
      - \c NDI_KERMA
      - \c NDI_DEPLETION
      - \c NDI_CHAIN
      - \c NDI_CURR_PART
      - \c NDI_TEMP
      - \c NDI_FISS_PROD

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

  \par Valid input keys:
  ======================

  - All \a handles:
      - \c NDI_COLLAPSE
      - \c NDI_E_BOUNDS_CURR_PART
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
  \brief Get the integer value indicated by  \a in_key 
  from \a handle.

  \par Fortran Example:
  \code
  ! Get the number of energy groups
  call ndi2_get_int_val(handle, NDI_NUM_GRPS, ret_val, ndi_err)
  \endcode

  \param handle: integer input state handle
  \param in_key: integer input key indicating the data sought
  \param ret_val: integer output containing the value returned by NDI
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 

  \par Valid input keys:
  ======================

  - Multigroup Transport \& Dosimetry \a handle:
      - \c NDI_NUM_GRPS
      - \c NDI_FISSIONABLE
      - \c NDI_UP_GRPS
      - \c NDI_DOWN_GRPS
      - \c NDI_LAST_GRP
      - \c NDI_NUM_REAC
      - \c NDI_NUM_PNU_PR
      - \c NDI_NUM_GRPS_CURR_PART
      - \c NDI_NUM_TARGET
      - \c NDI_NUM_RPRODS
      - \c NDI_ATOMIC_NUM
      - \c NDI_MASS_NUM
      - \c NDI_PN_ORDER
      - \c NDI_NUM_SEC_PARTS
      - \c NDI_PN_ORDER_CURR_PART
  - Thermonuclear burn \a handle:
      - \c NDI_NUM_TEMPS
      - \c NDI_NUM_SEC_PARTS
      - \c NDI_NUM_SIGVBAR_INTERP
      - \c NDI_NUM_EINBAR_INTERP
      - \c NDI_NUM_REAC
      - \c NDI_NUM_RPRODS
      - \c NDI_NUM_REACTANTS
      - \c NDI_NUM_GRPS
  - Thermonuclear reaction \a handle:
      - \c NDI_NUM_REAC

*/
int NDI2_get_int_val(int handle, int in_key, int *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_val_h(state_p, in_key, ret_val));
}

/*!
  \brief Get the integer value indicated by \a in_key and \a vec_num 
  from \a handle. 

  \par Fortran example:
  \code
  ! Get the integer identifier of the first reaction
  call ndi2_get_int_val_n(handle, NDI_REAC_MT, 0, ret_val, ndi_err)
  \endcode

  \param handle: integer input handle
  \param in_key: integer input key indicating the data sought
  \param vec_num: integer input index of requested element
  \param ret_val: integer output containing the value returned by NDI
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 

  \par Valid input keys:
  ======================

  - Multigroup Transport \& Dosimetry \a handle:
      - \c NDI_RPROD_ALL
      - \c NDI_RPROD_RMO
      - \c NDI_RPROD_FEW
      - \c NDI_REACT_MT
      - \c NDI_INC_PART_TYPES
      - \c NDI_TARGET
      - \c NDI_RPRODS_ID
  - Thermonuclear burn \a handle:
      - \c NDI_RPRODS_ID
      - \c NDI_RPRODS_MLT
      - \c NDI_REACTANTS_ID
      - \c NDI_REACTANTS_MLT
  - Thermonuclear reaction \a handle:
      - \c NDI_NUM_RPRODS
      - \c NDI_NUM_REACTANTS

*/
int NDI2_get_int_val_n(int handle, int in_key, int vec_num, int *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_val_n_h(state_p, in_key, vec_num, ret_val));
}

/*!
  \brief Get the integer array indicated by \a in_key from \a handle.

  \par Fortran example:
  \code
  ! Get the array of reaction identifiers
  call ndi2_get_int_vec(handle, NDI_REAC_MT, return_vec, vec_size, ndi_err)
  \endcode

  \param handle: integer input handle
  \param in_key: integer input key indicating the data sought.
  \param return_vec: integer output array, allocated by the user, 
  containing the values returned by NDI.
  \param vec_size: integer input indicating the allocated size of return_vec.
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 

  \par Valid input keys:
  ======================

  - Multi-particle Multigroup Transport \a handle:
      - \c NDI_REAC_MT
  - Multigroup Transport \& Dosimetry \a handle:
      - \c NDI_RPRODS_ID
      - \c NDI_RPROD_ALL
      - \c NDI_RPROD_RMO
      - \c NDI_RPROD_FEW
      - \c NDI_REAC_MT
  - Thermonuclear burn \a handle:
      - \c NDI_RPRODS_ID
      - \c NDI_RPRODS_MLT
      - \c NDI_REACTANTS_ID
      - \c NDI_REACTANTS_MLT
      - \c NDI_SIGVBAR_INTERP_REG
      - \c NDI_SIGVBAR_INTERP_TYPE
      - \c NDI_EINBAR_INTERP_REG
      - \c NDI_EINBAR_INTERP_TYPE
      - \c NDI_SEC_PART_TYPES

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
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 
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
  \brief Get the 32-bit floating-point value indicated by \a in_key 
  from \a handle.
  
  \par Fortran example:
  \code
  ! Get the atomic weight
  call ndi2_get_float32_val(handle, NDI_AT_WGT, ret_val, ndi_err)
  \endcode

  \param handle: integer input handle
  \param in_key: integer input key indicating the data sought
  \param ret_val: float32 output containing the value returned by NDI
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 

  \par Valid input keys:
  ======================

  - Multigroup Transport \a handle:
      - \c NDI_TEMP
      - \c NDI_SIG_0
      - \c NDI_AT_WGT
      - \c NDI_AWR
  - Thermonuclear burn \a handle:
      - \c NDI_REAC_Q
      - \c NDI_EAVG_OUT
      - \c NDI_EOUTBAR


*/
int NDI2_get_float32_val(int handle, int in_key, ndi_float32 *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float32_val_h(state_p, in_key, ret_val));
}

/*!
  \brief Get the 32-bit floating-point value indicated by \a in_key 
  and \a vec_num from \a handle.

  \par Fortran example:
  \code
  ! Get the Q-value for the first reaction
  call ndi2_get_float32_val_n(handle, NDI_REAC_Q, 0,  ret_val, ndi_err)
  \endcode

  \param handle: integer input handle
  \param in_key: integer input key indicating the data sought
  \param vec_num: integer input index of element sought
  \param ret_val: float32 output containing the value returned by NDI
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 

  \par Valid input keys:
  ======================

  - All \a handles:
      - \c NDI_ABUNDANCE
  - Thermonuclear burn \a handle:
      - \c NDI_REAC_Q
      - \c NDI_TOT_PROD_Q

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
  \brief Get the 32-bit floating-point array of values indicated by \a in_key
  from \a handle.

  \par Fortran example:
  \code
  ! Get the energy boundaries
  call ndi2_get_float32_vec(handle, NDI_E_BOUNDS,  return_vec, vec_size, ndi_err)
  \endcode

  \param handle: integer input handle
  \param in_key: integer input key indicating the data sought.
  \param return_vec: float32 output array, allocated by the user, 
  containing the values returned by NDI.
  \param vec_size: integer input indicating the allocated size of return_vec.
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 

  \par Valid input keys:
  ======================

  - Gendir \a handle:
      - \c NDI_TEMP
      - \c NDI_SIG_0
      - \c NDI_AT_WGT
      - \c NDI_AWR
  - Multigroup Transport \& Dosimetry \a handle:
      - \c NDI_E_BOUNDS
      - \c NDI_E_AVG
      - \c NDI_ENERGY_N
      - \c NDI_ENERGY_AVAIL
      - \c NDI_WGTS
      - \c NDI_VEL
      - \c NDI_HEATING
      - \c NDI_KERMA
      - \c NDI_HEATING_CURR_PART
      - \c NDI_KERMA_CURR_PART
      - \c NDI_SIG_TOT
      - \c NDI_NU_TOT
      - \c NDI_NU_PR
      - \c NDI_NU_DEL
      - \c NDI_NU_SIG_F_TOT
      - \c NDI_NU_SIG_F_PR
      - \c NDI_NU_SIG_F_DEL
      - \c NDI_CHI_VEC_TOT
      - \c NDI_CHI_VEC_PR
      - \c NDI_CHI_VEC_DEL
      - \c NDI_CHI_MAT_TOT
      - \c NDI_CHI_MAT_DEL
      - \c NDI_CHI_MAT_PR
      - \c NDI_NORM
      - \c NDI_FISS_Q
      - \c NDI_REAC_Q
      - \c NDI_E_BOUNDS_CURR_PART
      - \c NDI_TOT_DEPL_XS
  - Thermonuclear burn \a handle:
      - \c NDI_TEMPS
      - \c NDI_SIGVBARS
      - \c NDI_EINBAR
      - \c NDI_EDIST
      - \c NDI_E_BOUNDS
      - \c NDI_E_AVG

*/
int NDI2_get_float32_vec(int handle, int in_key,
		     ndi_float32 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float32_vec_h(state_p, in_key, return_vec, vec_size));
}

/*!
  \brief Get the 32-bit floating-point array indicated by \a in_key 
  and \a vec_num from \a handle.

  \par Fortran example:
  \code
  ! Get the cross section for the (n,2n) reaction (MT=16)
  call ndi2_get_float32_vec_n(handle, NDI_SIG_REAC, 16,  return_vec, vec_size, ndi_err)
  \endcode

  \param handle: integer input handle.
  \param in_key: integer input key indicating the data sought.
  \param vec_num: integer input index of requested element.
  \param return_vec: float32 output array, allocated by the user,
  containing the values returned by NDI.
  \param vec_size: integer input indicating the allocated size of \a return_vec.
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 
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
  \brief Get the 32-bit floating-point array indicated by \a in_key and 
  \a in_str from \a handle

  \par Fortran example:
  \code
  ! Get the total production cross sections for Y-87
  call ndi2_get_float32_vec_x(handle, NDI_TOT_PROD_XS, "39087", return_vec, vec_size, ndi_err)
  \endcode

  \param handle: integer input handle
  \param in_key: integer input key indicating the data sought
  \param in_str: string input name indicating the element sought
  \param return_vec: float32 output array, allocated by the user,
  containing the values returned by NDI
  \param vec_size: integer input indicating the allocated size of \a return_vec
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 

  \par Valid input keys:
  ======================

  - All \a handles:
      - \c NDI_TOT_PROD_XS

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
  \brief Get the 64-bit floating-point value indicated by \a in_key 
  from \a handle.
  
  \par Fortran example:
  \code
  ! Get the atomic weight
  call ndi2_get_float64_val(handle, NDI_AT_WGT, ret_val, ndi_err)
  \endcode

  \param handle: integer input handle
  \param in_key: integer input key indicating the data sought
  \param ret_val: float64 output containing the value returned by NDI
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 

  \par Valid input keys:
  ======================

  - Multigroup Transport \a handle:
      - \c NDI_TEMP
      - \c NDI_SIG_0
      - \c NDI_AT_WGT
      - \c NDI_AWR
  - Thermonuclear burn \a handle:
      - \c NDI_REAC_Q
      - \c NDI_EAVG_OUT
      - \c NDI_EOUTBAR

*/
int NDI2_get_float64_val(int handle, int in_key, ndi_float64 *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float64_val_h(state_p, in_key, ret_val));
}

/*!
  \brief Get the 64-bit floating-point value indicated by \a in_key 
  and \a vec_num from \a handle.

  \par Fortran example:
  \code
  ! Get the Q-value for the first reaction
  call ndi2_get_float64_val_n(handle, NDI_REAC_Q, 0,  ret_val, ndi_err)
  \endcode

  \param handle: integer input handle
  \param in_key: integer input key indicating the data sought
  \param vec_num: integer input index of element sought
  \param ret_val: float64 output containing the value returned by NDI
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 

  \par Valid input keys:
  ======================

  - All \a handles:
      - \c NDI_ABUNDANCE
  - Thermonuclear burn \a handle:
      - \c NDI_REAC_Q
      - \c NDI_TOT_PROD_Q

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
  \brief Get the 64-bit floating-point array of values indicated by \a in_key
  from \a handle.

  \par Fortran example:
  \code
  ! Get the energy boundaries
  call ndi2_get_float64_vec(handle, NDI_E_BOUNDS,  return_vec, vec_size, ndi_err)
  \endcode

  \param handle: integer input handle
  \param in_key: integer input key indicating the data sought.
  \param return_vec: float64 output array, allocated by the user, 
  containing the values returned by NDI.
  \param vec_size: integer input indicating the allocated size of return_vec.
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 

  \par Valid input keys:
  ======================

  - Gendir \a handle:
      - \c NDI_TEMP
      - \c NDI_SIG_0
      - \c NDI_AT_WGT
      - \c NDI_AWR
  - Multigroup Transport \& Dosimetry \a handle:
      - \c NDI_E_BOUNDS
      - \c NDI_E_AVG
      - \c NDI_ENERGY_N
      - \c NDI_ENERGY_AVAIL
      - \c NDI_WGTS
      - \c NDI_VEL
      - \c NDI_HEATING
      - \c NDI_KERMA
      - \c NDI_HEATING_CURR_PART
      - \c NDI_KERMA_CURR_PART
      - \c NDI_SIG_TOT
      - \c NDI_NU_TOT
      - \c NDI_NU_PR
      - \c NDI_NU_DEL
      - \c NDI_NU_SIG_F_TOT
      - \c NDI_NU_SIG_F_PR
      - \c NDI_NU_SIG_F_DEL
      - \c NDI_CHI_VEC_TOT
      - \c NDI_CHI_VEC_PR
      - \c NDI_CHI_VEC_DEL
      - \c NDI_CHI_MAT_TOT
      - \c NDI_CHI_MAT_DEL
      - \c NDI_CHI_MAT_PR
      - \c NDI_NORM
      - \c NDI_FISS_Q
      - \c NDI_REAC_Q
      - \c NDI_E_BOUNDS_CURR_PART
      - \c NDI_TOT_DEPL_XS
  - Thermonuclear burn \a handle:
      - \c NDI_TEMPS
      - \c NDI_SIGVBARS
      - \c NDI_EINBAR
      - \c NDI_EDIST
      - \c NDI_E_BOUNDS
      - \c NDI_E_AVG
*/
int NDI2_get_float64_vec(int handle, int in_key,
		     ndi_float64 * return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float64_vec_h(state_p, in_key, return_vec, vec_size));
}

/*!
  \brief Get the 64-bit floating-point array indicated by \a in_key 
  and \a vec_num from \a handle.

  \par Fortran example:
  \code
  ! Get the cross section for the (n,2n) reaction (MT=16)
  call ndi2_get_float64_vec_n(handle, NDI_SIG_REAC, 16,  return_vec, vec_size, ndi_err)
  \endcode

  \param handle: integer input handle.
  \param in_key: integer input key indicating the data sought.
  \param vec_num: integer input index of requested element.
  \param return_vec: float64 output array, allocated by the user,
  containing the values returned by NDI.
  \param vec_size: integer input indicating the allocated size of return_vec.
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 
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
  \brief Get the string value indicated by \a in_key from \a handle.

  \par Fortran example:
  \code
  ! Get the NDI version string
  call ndi2_get_string_val(handle, NDI_VERSION, ret_val, ndi_err)
  \endcode

  \param handle: integer input handle
  \param in_key: integer input key indicating the data sought.
  \param ret_val: character output array, allocated by the user, 
  containing the values returned by NDI.
  \param str_size: integer input indicating the allocated size of \a ret_val 
  (C only) 
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 

  \par Valid input keys:
  ======================

  - Any \a handle:
      - \c NDI_VERSION
      - \c NDI_GENDIR_PATH
  - Multigroup Transport \& Dosimetry \a handle:
      - \c NDI_ZAID
      - \c NDI_INFO
      - \c NDI_DATE_PROCESSED
      - \c NDI_DATE_SOURCE
      - \c NDI_LIBRARY_NAME
      - \c NDI_DEPL_ZAID
      - \c NDI_DEPL_INFO
      - \c NDI_SEC_PART_TYPES
      - \c NDI_SEC_PART_ZAIDS
  - Thermonuclear burn \a handle:
      - \c NDI_ZAID
      - \c NDI_INFO
      - \c NDI_DATE_PROCESSED
      - \c NDI_DATE_SOURCE
      - \c NDI_LIBRARY_NAME
  - Thermonuclear burn reaction \a handle:
      - \c NDI_ZAID
      - \c NDI_INFO
      - \c NDI_DATE_PROCESSED
      - \c NDI_DATE_SOURCE
      - \c NDI_LIBRARY_NAME
  
*/
int NDI2_get_string_val(int handle, int in_key, char *ret_val,
                        int str_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_string_val_h(state_p, in_key, ret_val, str_size));
}

/*!
  \brief Get the string value indicated by \a in_key and \a vec_num
  from \a handle

  \par Fortran example:
  \code
  ! get the first reaction identifier
  call ndi2_get_string_val_n(handle, NDI_LIST_REAC, 0, ret_val, ndi_err)
  \endcode

  \param handle: integer input handle
  \param in_key: integer input key indicating the data sought
  \param vec_num: integer input index indicating the data sought
  \param ret_val: character output array, allocated by the user,
  containing the values returned by NDI
  \param str_size: integer input indicating the allocated size of 
  \a ret_val (C only)
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 

  \par Valid input keys:
  ======================

  - Multigroup Transport \& Dosimetry \a handle:
      - \c NDI_ZAID
      - \c NDI_SEC_PART_TYPES
      - \c NDI_SEC_PART_ZAIDS
      - \c NDI_TARGET
      - \c NDI_RPRODS_ID
      - \c NDI_INC_PART_TYPES
  - Thermonuclear burn \a handle:
      - \c NDI_SEC_PART_TYPES
      - \c NDI_RPRODS_ID
      - \c NDI_REACTANTS_ID
  - Thermonuclear burn reaction \a handle:
      - \c NDI_LIST_REAC

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

