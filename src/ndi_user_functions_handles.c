#include <stdio.h>
#include <assert.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"
#include "ndi_var_type.h"

/*! \defgroup open_gendir Gendir Handle Open and Close
 *  These functions open and close gendir handles.  The first call made 
 *  to NDI should be \a NDI2_open_gendir; the last call made 
 *  to NDI should be \a NDI2_close_gendir.
 * @{
 */

/*!
  \brief Open handle to NDI gendir index file indicated by \a path.

  This function opens the indicated NDI gendir index file, initializes 
  NDI variables and default values, and returns a handle to the file.  
  It should be the first NDI function called; at least one gendir handle 
  is needed to access NDI index data and to make handles to NDI physical data.

  \par Fortran example
  \code
  ! Create handle to index file at default path
  call ndi2_open_gendir(handle, "", ndi_err)
  \endcode

  \param handle: integer output gendir handle
  \param path: string input containing the index file path; 
  "" for release default path
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only)

*/
int 
NDI2_open_gendir(int *handle, const char *path)
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
  \brief Close handle to NDI gendir index file.

  This function deallocates the memory associated with a gendir handle and its 
  children and closes the associated NDI gendir file pointer.  It should be 
  called once for each gendir handle opened after it and its children are no 
  longer needed.

  \par Fortran example
  \code
  ! Close gendir handle
  call ndi2_close_gendir(handle, ndi_err)
  \endcode

  \param handle: integer input gendir handle
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only)

*/
int 
NDI2_close_gendir(int handle)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);

    if (is_gendir(state_p)) {
        return (__ndi_close_handle(handle));
    } else {
	return -2;
    }
}

/*! @} end of open_gendir */

/*! \defgroup gendir_set Gendir Handle Set and Check
 *  These functions set and check the gendir handle state, 
 *  which affects subsequent 
 *  requests for gendir data including the creation state of data handles.
 *  They require a valid gendir handle from \a NDI2_open_gendir, and are 
 *  usually called immediately after the gendir handle is opened.
 *  @{
 */

/*!
  \brief Set the global string option indicated by \a in_key to 
  \a value for gendir \a handle.

  This function sets the state of the gendir handle that affects
  subsequent NDI calls.

  \par Fortran example
  \code
  ! Set default library type to "multigroup_neutron"
  call ndi2_set_option_gendir(handle, NDI_LIB_TYPE_DEFAULT, "multigroup_neutron", ndi_err)
  \endcode

  \param handle: integer input gendir handle
  \param in_key: integer input indicating the option set
  \param value: string input containing the option value
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only)

  \par Valid in_key values:
       ===================

       - \c NDI_LIB_TYPE_DEFAULT: Library type  
       - \c NDI_LIBRARY_DEFAULT: Library name 
       - \c NDI_MAT_ORDER_DEFAULT: Matrix transpose order 
       - \c NDI_COLLAPSE_DEFAULT: Energy group structure 
       - \c NDI_E_BOUNDS_CURR_PART_DEFAULT: Current particle energy group structure 
       - \c NDI_TRANS_CORR_DEFAULT: Use transport corrections 
       - \c NDI_NORM_DEFAULT: Normalize chi and e_dist 
       - \c NDI_REAC_ID_DEFAULT: Specify reaction identifiers as MT or C values 
       - \c NDI_E_AVG_DEFAULT: Specify average bin energy calculation 
       - \c NDI_KERMA_DEFAULT: Specify kerma/heating calculation 
       - \c NDI_CURR_PART_DEFAULT: Specify current particle 
       - \c NDI_TEMP_DEFAULT: Set default temperature  
       - \c NDI_FISS_PROD_DEFAULT: Set default fission product ZAID 
*/
int 
NDI2_set_option_gendir(int handle, int in_key, const char *value)
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
  \brief Retrieves the global string option indicated by \a in_key for 
  gendir \a handle.

  This function checks the state of the gendir handle 
  previously set by default or by \a NDI2_set_option_gendir calls.

  \par Fortran example
  \code
  ! Check default library
  call ndi2_check_option_gendir(handle, NDI_LIBRARY_DEFAULT, out_value, ndi_err)
  \endcode

  \param handle: integer input gendir handle
  \param in_key: integer input indicating the option checked
  \param out_value: string output containing the option value
  \param string_size: integer input indicating the allocated size of 
  out_value (C only)
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only)

  \par Valid in_key values:
       ===================

       - \c NDI_LIB_TYPE_DEFAULT: Library type  
       - \c NDI_LIBRARY_DEFAULT: Library name 
       - \c NDI_MAT_ORDER_DEFAULT: Matrix transpose order 
       - \c NDI_COLLAPSE_DEFAULT: Energy group structure 
       - \c NDI_E_BOUNDS_CURR_PART_DEFAULT: Current particle energy group structure 
       - \c NDI_TRANS_CORR_DEFAULT: Use transport corrections 
       - \c NDI_NORM_DEFAULT: Normalize chi and e_dist 
       - \c NDI_REAC_ID_DEFAULT: Specify reaction identifiers as MT or C values 
       - \c NDI_E_AVG_DEFAULT: Specify average bin energy calculation 
       - \c NDI_KERMA_DEFAULT: Specify kerma/heating calculation 
       - \c NDI_CURR_PART_DEFAULT: Specify current particle 
       - \c NDI_TEMP_DEFAULT: Set default temperature  
       - \c NDI_FISS_PROD_DEFAULT: Set default fission product ZAID 
*/
int 
NDI2_check_option_gendir(int handle, int in_key, char *out_value,
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

/*! @} end of gendir_set */

/*! \defgroup make_handle Data Handle Make
 *  This function makes data handles.  It requires a valid gendir handle
 *  from a call to \a NDI2_open_gendir.
 *  @{
 */

/*!
  \brief Make child \a handle1 of \a type from parent \a handle0.

  This function makes a handle of specificed type capable of accessing 
  the physical data indexed by its parent gendir.

  \par Fortran example
  \code
  ! Make a multigroup neutron data handle
  call ndi2_make_handle(handle0, "multigroup_neutron", handle1, ndi_err)
  \endcode

  \param handle0: integer input parent handle
  \param type: string input containing the library type
  \param handle1: integer output child handle
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only)

  \par Valid type values:
       =================

      - \c multigroup_neutron
      - \c multigroup_photon
      - \c multigroup_multi
      - \c tn
      - \c tnreactions
      - \c dosimetry_neutron

*/
int 
NDI2_make_handle(int handle0, const char *type, int *handle1)
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

/*! @} end of make_handle */

/*! \defgroup set_handle Data Handle Set and Check
 *  These functions set and check data handle state, which affects 
 *  subsequent requests for handle data.  These routines require a valid data
 *  handle from \a NDI2_make_handle, and are usually called immediately 
 *  after the data handle is made.
 *  @{
 */

/*!
  \brief Set isotope for \a handle.

  This function associates a handle with an entry in the NDI index file.  
  The file associated with the first matching isotope entry in the index file 
  is opened, and the physical data associated with that entry initializes the 
  handle data.  Subsequent call to the \a NDI2_get family of functions with 
  this handle will return the requested physical data associated with this 
  isotope.

  \par Fortran example
  \code
  ! Set isotope U-235
  call ndi2_set_isotope(handle, "92235", ndi_err)
  \endcode

  \param handle: integer input handle
  \param zaid: string input containing Z, A id
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only)

*/
int 
NDI2_set_isotope(int handle, const char *zaid)
{
    return NDI2_set_zaid(handle, zaid);
}

/*!
  \brief Set zaid for \a handle.

  This function associates a handle with an entry in the NDI index file.  
  The file associated with the first matching zaid entry in the index file 
  is opened, and the physical data associated with that entry initializes the 
  handle data.  Subsequent call to the \a NDI2_get family of functions with 
  this handle will return the requested physical data associated with this 
  zaid.

  \par Fortran example
  \code
  ! Set zaid U-235
  call ndi2_set_zaid(handle, "92235", ndi_err)
  \endcode

  \param handle: integer input handle
  \param zaid: string input containing zaid
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only)
*/
int 
NDI2_set_zaid(int handle, const char *zaid)
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
  \brief Set reaction for \a handle.

  This function associates a handle with an entry in the NDI index file.  
  The file associated with the first matching reaction entry in the index file 
  is opened, and the physical data associated with that entry initializes the 
  handle data.  Subsequent call to the \a NDI2_get family of functions with 
  this handle will return the requested physical data associated with this 
  reaction.

  \par Fortran example
  \code
  ! Set reaction "d+t->n+a"
  call ndi2_set_reaction(handle, "d+t->n+a", ndi_err)
  \endcode

  \param handle: integer input handle
  \param reaction: string input containing reaction
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only)

*/
int 
NDI2_set_reaction(int handle, const char *zaid)
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
  \brief Set the option indicated by \a in_key to \a value for \a handle.

  This function sets the state of the data handle that affects 
  subsequent NDI calls.

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

  \par Valid in_key values:
       ===================

  - With all \a handles:
       - \c NDI_LIB_TYPE:           Library type  
       - \c NDI_LIBRARY:            Library name 
       - \c NDI_MAT_ORDER:          Matrix transpose order 
       - \c NDI_COLLAPSE:           Energy group structure 
       - \c NDI_E_BOUNDS_CURR_PART:  Current particle energy group structure 
       - \c NDI_TRANS_CORR:     Use transport corrections 
       - \c NDI_NORM:   Normalize chi and e_dist 
       - \c NDI_REAC_ID:        Specify reaction identifiers as MT or C values 
       - \c NDI_E_AVG:  Specify average bin energy calculation 
       - \c NDI_KERMA:  Specify kerma/heating calculation 
       - \c NDI_DEPLETION:      Set depletion chain 
       - \c NDI_CHAIN:  Set radiochemistry chain 
       - \c NDI_CURR_PART:      Specify current particle 
       - \c NDI_TEMP:   Set default temperature  
       - \c NDI_FISS_PROD:      Set default fission product ZAID 
*/
int 
NDI2_set_option(int handle, int in_key, const char *value)
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
  \brief Set the option indicated by \a in_key to \a option_array for 
  \a handle.

  This function sets the state of the data handle that affects 
  subsequent NDI calls.

  \par Fortran example
  \code
  ! 
  call ndi2_set_float64_vec_option(handle, in_key, array, size, ndi_err)
  \endcode

  \param handle: integer input state handle
  \param in_key: integer input key indicating the option set
  \param option_array: float64 input array containing option values
  \param array_size: integer input size of option array
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only)

  \par Valid in_key values:
       ===================

  - All \a handles:
      - \c NDI_COLLAPSE
      - \c NDI_E_BOUNDS_CURR_PART
*/
int 
NDI2_set_float64_vec_option(int handle, int in_key,
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
  \brief Retrieve the option indicated by \a in_key in \a out_value for 
  \a handle.

  This function checks the state of the data handle 
  previously set by default or by \a NDI2_set_option_gendir or 
  \a NDI2_set_float64_vec_option calls.

  \par Fortran example
  \code
  ! Get size of sigma total array
  call ndi2_get_size(handle, NDI_SIG_TOT, size, ierr)
  \endcode

  \param handle: integer input state handle
  \param in_key: integer input key indicating the option requested
  \param out_value: string output containing the option value
  \param string_size: integer input maximum size of option string
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only)

  \par Valid in_key values:
       ===================

  - With all \a handles:
       - \c NDI_LIB_TYPE:           Library type  
       - \c NDI_LIBRARY:            Library name 
       - \c NDI_MAT_ORDER:          Matrix transpose order 
       - \c NDI_COLLAPSE:           Energy group structure 
       - \c NDI_E_BOUNDS_CURR_PART:  Current particle energy group structure 
       - \c NDI_TRANS_CORR:     Use transport corrections 
       - \c NDI_NORM:   Normalize chi and e_dist 
       - \c NDI_REAC_ID:        Specify reaction identifiers as MT or C values 
       - \c NDI_E_AVG:  Specify average bin energy calculation 
       - \c NDI_KERMA:  Specify kerma/heating calculation 
       - \c NDI_DEPLETION:      Set depletion chain 
       - \c NDI_CHAIN:  Set radiochemistry chain 
       - \c NDI_CURR_PART:      Specify current particle 
       - \c NDI_TEMP:   Set default temperature  
       - \c NDI_FISS_PROD:      Set default fission product ZAID 
*/
int 
NDI2_check_option(int handle, int in_key, char *out_value, int string_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_check_option_h(state_p, in_key, out_value, string_size));
}

/*! @} end of handle_set */

/*! \defgroup handle_get Gendir and Data Handle Get
 *  These functions get gendir and data handle data.  They require a valid 
 *  data handle \a NDI2_make_handle.
 *  @{
 */

/*!
  \brief Get the size indicated by \a in_key from \a handle.

  This function returns the number of array elements for an array specified 
  by \a in_key, or the number of characters for a string specified by 
  \a in_key.  Note that in C the number of characters in a string includes the 
  terminating '\0' character.  The size of a string returned by the C function
  \a NDI2_get_size is one greater than returned by the Fortran subroutine 
  \a ndi2_get_size.

  \par Fortran example
  \code
  ! Get size of sigma total array
  call ndi2_get_size(handle, NDI_SIG_TOT, size, ierr)
  \endcode

  \param handle: integer input state handle
  \param in_key: integer input key containing the size requested
  \param size: integer output containing the size (Fortran only)
  \param ierr: integer output error code
  \return integer output containing the size (C only)


  \par Valid in_key values:
       ===================

  - With all \a handles:
      - \c NDI_VERSION: String size
      - \c NDI_GENDIR_PATH: String size
  - With gendir \a handle:
      - \c NDI_ZAID: String size
      - \c NDI_TEMP: Array length
      - \c NDI_SIG_0: Array length
      - \c NDI_AT_WGT: Array length
      - \c NDI_AWR: Array length
  - With multigroup_neutron, multigroup_photon, or dosimetry_neutron \a handle:
      - \c NDI_ZAID: String size
      - \c NDI_INFO: String size
      - \c NDI_DATE_PROCESSED: String size
      - \c NDI_DATE_SOURCE: String size
      - \c NDI_LIBRARY_NAME: String size
      - \c NDI_DEPL_ZAID: String size
      - \c NDI_DEPL_INFO: String size
      - \c NDI_SEC_PART_TYPES: String size
      - \c NDI_SEC_PART_ZAIDS: String size
      - \c NDI_NUM_GRPS: Array length
      - \c NDI_TEMP: Array length
      - \c NDI_SIG_0: Array length
      - \c NDI_UP_GRPS: Array length
      - \c NDI_DOWN_GRPS: Array length
      - \c NDI_AT_WGT: Array length
      - \c NDI_AWR: Array length
      - \c NDI_NUM_REAC: Array length
      - \c NDI_NUM_RPRODS: Array length
      - \c NDI_PN_ORDER: Array length
      - \c NDI_NUM_SEC_PARTS: Array length
      - \c NDI_NUM_GRPS_CURR_PART: Array length
      - \c NDI_PN_ORDER_CURR_PART: Array length
      - \c NDI_NUM_TARGET: Array length
      - \c NDI_LAST_GRP: Array length
      - \c NDI_FISSIONABLE: Array length
      - \c NDI_ATOMIC_NUM: Array length
      - \c NDI_MASS_NUM: Array length
      - \c NDI_NUM_PNU_PR: Array length
      - \c NDI_E_AVG: Array length
      - \c NDI_ENERGY_N: Array length
      - \c NDI_ENERGY_AVAIL: Array length
      - \c NDI_WGTS: Array length
      - \c NDI_VEL: Array length
      - \c NDI_HEATING: Array length
      - \c NDI_KERMA: Array length
      - \c NDI_SIG_TOT: Array length
      - \c NDI_NU_TOT: Array length
      - \c NDI_NU_PR: Array length
      - \c NDI_NU_DEL: Array length
      - \c NDI_NU_SIG_F_TOT: Array length
      - \c NDI_NU_SIG_F_PR: Array length
      - \c NDI_NU_SIG_F_DEL: Array length
      - \c NDI_CHI_VEC_TOT: Array length
      - \c NDI_CHI_VEC_PR: Array length
      - \c NDI_CHI_VEC_DEL: Array length
      - \c NDI_HEATING_CURR_PART: Array length
      - \c NDI_KERMA_CURR_PART: Array length
      - \c NDI_TOT_PROD_XS: Array length
      - \c NDI_TOT_DEPL_XS: Array length
      - \c NDI_E_BOUNDS: Array length
      - \c NDI_NORM: Array length
      - \c NDI_FISS_Q: Array length
      - \c NDI_REAC_MT: Array length
      - \c NDI_REAC_Q: Array length
      - \c NDI_E_BOUNDS_CURR_PART: Array length
      - \c NDI_RPROD_ALL: Array length
      - \c NDI_RPROD_RMO: Array length
      - \c NDI_RPROD_FEW: Array length
      - \c NDI_CHI_MAT_TOT: Array length
      - \c NDI_CHI_MAT_PR: Array length
      - \c NDI_CHI_MAT_DEL: Array length
  - With mutigroup_multi \a handle:
      - \c NDI_E_BOUNDS: Array length
      - \c NDI_WGTS: Array length
      - \c NDI_VEL: Array length
      - \c NDI_E_AVG: Array length
      - \c NDI_HEATING: Array length
      - \c NDI_KERMA: Array length
      - \c NDI_SIG_TOT: Array length
      - \c NDI_NU_TOT: Array length
      - \c NDI_NU_PR: Array length
      - \c NDI_NU_DEL: Array length
      - \c NDI_NU_SIG_F_TOT: Array length
      - \c NDI_NU_SIG_F_PR: Array length
      - \c NDI_NU_SIG_F_DEL: Array length
      - \c NDI_CHI_VEC_TOT: Array length
      - \c NDI_CHI_VEC_PR: Array length
      - \c NDI_CHI_VEC_DEL: Array length
      - \c NDI_REAC_MT: Array length
      - \c NDI_ZAID: String size
      - \c NDI_NUM_GRPS: Array length
      - \c NDI_PN_ORDER: Array length
      - \c NDI_NUM_PNU_PR: Array length
      - \c NDI_NUM_REAC: Array length
      - \c NDI_FISSIONABLE: Array length
  - With tn \a handle:
      - \c NDI_ZAID: String size
      - \c NDI_INFO: String size
      - \c NDI_DATE_PROCESSED: String size
      - \c NDI_DATE_SOURCE: String size
      - \c NDI_LIBRARY_NAME: String size
      - \c NDI_NUM_TEMPS: Array length
      - \c NDI_NUM_SIGVBAR_INTERP: Array length
      - \c NDI_NUM_EINBAR_INTERP: Array length
      - \c NDI_NUM_SEC_PARTS: Array length
      - \c NDI_NUM_REAC: Array length
      - \c NDI_NUM_RPRODS: Array length
      - \c NDI_NUM_REACTANTS: Array length
      - \c NDI_REAC_Q: Array length
      - \c NDI_NUM_GRPS: Array length
      - \c NDI_EAVG_OUT: Array length
      - \c NDI_EOUTBAR: Array length
      - \c NDI_TEMPS: Array length
      - \c NDI_SIGVBARS: Array length
      - \c NDI_RPRODS_ID: Array length
      - \c NDI_RPRODS_MLT: Array length
      - \c NDI_REACTANTS_ID: Array length
      - \c NDI_REACTANTS_MLT: Array length
      - \c NDI_SIGVBAR_INTERP_REG: Array length
      - \c NDI_SIGVBAR_INTERP_TYPE: Array length
      - \c NDI_EINBAR_INTERP_REG: Array length
      - \c NDI_EINBAR_INTERP_TYPE: Array length
      - \c NDI_SEC_PART_TYPES: Array length
      - \c NDI_EDIST: Array length
      - \c NDI_EDIST_INTERP_REG: Array length
      - \c NDI_EDIST_INTERP_TYPE: Array length
      - \c NDI_EDIST_INTERP_REG: Array length
      - \c NDI_EDIST_TEMPS: Array length
      - \c NDI_EINBAR: Array length
      - \c NDI_E_BOUNDS: Array length
      - \c NDI_E_AVG: Array length
  - With tnreactions \a handle:
      - \c NDI_ZAID: String size
      - \c NDI_INFO: String size
      - \c NDI_DATE_PROCESSED: String size
      - \c NDI_DATE_SOURCE: String size
      - \c NDI_LIBRARY_NAME: String size
      - \c NDI_NUM_REAC: Array length
*/
int 
NDI2_get_size(int handle, int in_key, int *ierr)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_size_h(state_p, in_key, ierr));
}

/*!
  \brief Get the size indicated by \a in_key and \a vec_num from \a handle.

  This function returns the number of array elements for an array specified 
  by \a in_key and \a vec_num, or the number of characters for a string 
  specified by \a in_key and \a vec_num.  Note that in C the number of 
  characters in a string includes the terminating '\0' character.  The size 
  of a string returned by the C function \a NDI2_get_size_n is one greater 
  than returned by the Fortran subroutine \a ndi2_get_size_n.

  \par Fortran example
  \code
  ! Get size of reation label 18
  call ndi2_get_size_n(handle, NDI_REAC_LABEL, 18, size, ierr)
  \endcode

  \param handle: integer input state handle
  \param in_key: integer input key containing the size requested
  \param vec_num: integer input index
  \param size: integer output containing the size (Fortran only)
  \param ierr: integer output error code
  \return integer output containing the size (C only)

*/
int 
NDI2_get_size_n(int handle, int in_key, int vec_num, int *ierr)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_size_n_h(state_p, in_key, vec_num, ierr));
}


/*!
  \brief Get the size indicated by \a in_key and \a in_str from \a handle.

  This function returns the number of array elements for an array specified 
  by \a in_key and \a in_str, or the number of characters for a string 
  specified by \a in_key and \a in_str.  Note that in C the number of 
  characters in a string includes the terminating '\0' character.  The size 
  of a string returned by the C function \a NDI2_get_size_x is one greater 
  than returned by the Fortran subroutine \a ndi2_get_size_x.

  \par Fortran example
  \code
  ! Get size of sigma array
  call ndi2_get_size(handle, NDI_SIG_TOT, size, ierr)
  \endcode

  \param handle: integer input state handle
  \param in_key: integer input key containing the size requested
  \param in_str: string input index
  \param size: integer output containing the size (Fortran only)
  \param ierr: integer output error code
  \return integer output containing the size (C only)


  \par Valid in_key values:
       ===================

  - All \a handles:
      - \c NDI_ZAID
  - Multigroup tranport or dosimetry \a handles:
      - \c NDI_RPROD_ALL
      - \c NDI_RPROD_RMO
      - \c NDI_RPROD_FEW
      - \c NDI_REAC_MT
      - \c NDI_REAC_Q
      - \c NDI_ABUNDANCE
      - \c NDI_TOT_PROD_Q
      - \c NDI_SEC_PART_TYPES
      - \c NDI_SEC_PART_ZAIDS
      - \c NDI_TARGET
      - \c NDI_RPRODS_ID
      - \c NDI_INC_PART_TYPES
      - \c NDI_REAC_LABEL
      - \c NDI_TOT_PROD_XS
      - \c NDI_SIG_REAC
      - \c NDI_CORR_DIAG
      - \c NDI_CORR_BHS
      - \c NDI_PNU_PR
      - \c NDI_SIG_PNU_PR
      - \c NDI_RPROD_ID_ALL
      - \c NDI_RPROD_MLT_ALL
      - \c NDI_RPROD_ID_RMO
      - \c NDI_RPROD_MLT_RMO
      - \c NDI_RPROD_ID_FEW
      - \c NDI_RPROD_MLT_FEW
      - \c NDI_PN_DOWN_FULL
      - \c NDI_PN_FULL
      - \c NDI_TRANSFER_MAT_TOT
      - \c NDI_TRANSFER_MAT_PR
      - \c NDI_PN_PROD_FULL_CURR_PART
  - Multiparticle multigroup transport \a handle:
      - \c NDI_PN_FULL
      - \c NDI_SIG_REAC
      - \c NDI_PNU_PR
      - \c NDI_SIG_PNU_PR
      - \c NDI_CORR_DIAG
      - \c NDI_CORR_BHS
  - Thermonuclear burn \a handle:
      - \c NDI_RPRODS_MLT
      - \c NDI_REACTANTS_MLT
      - \c NDI_RPRODS_ID
      - \c NDI_REACTANTS_ID
      - \c NDI_SEC_PART_TYPES
  - Thermonuclear reaction \a handle:
      - \c NDI_NUM_RPRODS
      - \c NDI_NUM_REACTANTS
      - \c NDI_RPRODS_ID
      - \c NDI_RPRODS_MLT
      - \c NDI_REACTANTS_ID
      - \c NDI_REACTANTS_MLT
      - \c NDI_LIST_REAC
*/
int 
NDI2_get_size_x(int handle, int in_key, const char *in_str,
                    int *ierr)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_size_x_h(state_p, in_key, in_str, ierr));
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

  \par Valid in_key values:
       ===================

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
int 
NDI2_get_int_val(int handle, int in_key, int *ret_val)
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

  \par Valid in_key values:
       ===================

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
int 
NDI2_get_int_val_n(int handle, int in_key, int vec_num, int *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_val_n_h(state_p, in_key, vec_num, ret_val));
}

/*!
  \brief Get the integer array \a return_vec indicated by \a in_key from 
  \a handle.

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

  \par Valid in_key values:
       ===================

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
int 
NDI2_get_int_vec(int handle, int in_key, int *return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_vec_h(state_p, in_key, return_vec, vec_size));
}

/*!
  \brief Get integer array of \a return_vec values indicated by \a in_key 
  and \a vec_num from \a handle.
  
  This function returns the integer array data specified by \a in_key and 
  \a vec_num.

  \par Fortran example
  \code
  ! Get list of integer reaction product identifiers for first TN reaction
  call ndi2_get_int_vec_n(handle, NDI_RPROD_ID, 0, ret_vec, ierr, size)
  \endcode

  \param handle: integer input state handle
  \param in_key: integer input key containing the option sought
  \param vec_num: integer input index of vector
  \param return_vec: integer output vector of values
  \param ndi_err: integer output error code (Fortran only)
  \param vec_size: integer input maximum size of integer vector
  \return integer output error code (C only) 
*/
int 
NDI2_get_int_vec_n(int handle, int in_key, int vec_num,
		   int *return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_vec_n_h(state_p, in_key, vec_num, 
                                      return_vec, vec_size));
}

/*!
  \brief Get integer array of \a return_vec values indicated by \a in_key 
  and \a in_str from \a handle.
  
  This function returns the integer array data specified by \a in_key and 
  \a in_str.

  \par Fortran example
  \code
  ! Get list of integer reaction MTs for PD reactions
  ! from handle zaid target to "92235" product
  call ndi2_get_int_vec_x(handle, NDI_REAC_MT, "92235", ret_vec, ierr, size)
  \endcode

  \param handle: integer input state handle
  \param in_key: integer input key containing the option sought
  \param in_str: string input name of element sought
  \param return_vec: integer output vector of values
  \param ndi_err: integer output error code (Fortran only)
  \param vec_size: integer input maximum size of integer vector
  \return integer output error code (C only) 
*/
int 
NDI2_get_int_vec_x(int handle, int in_key, const char *in_str,
		   int *return_vec, int vec_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_int_vec_x_h(state_p, in_key, in_str, 
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

  \par Valid in_key values:
       ===================

  - multigroup_neutron \a handle:
      - \c NDI_TEMP: Temperature
      - \c NDI_SIG_0: Sigma zero
      - \c NDI_AT_WGT: Atomic weight
      - \c NDI_AWR: Atomic weight ratio
  - tn \a handle:
      - \c NDI_REAC_Q: Reaction Q value
      - \c NDI_EAVG_OUT: Average outgoing energy
      - \c NDI_EOUTBAR: Average outgoing energy

*/
int 
NDI2_get_float32_val(int handle, int in_key, ndi_float32 *ret_val)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_float32_val_h(state_p, in_key, ret_val));
}

/*!
  \brief Get the 32-bit floating-point \a ret_val indicated by \a in_key 
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

  \par Valid in_key values:
       ===================

  - All \a handles:
      - \c NDI_ABUNDANCE: Natural abundance for isotope \a vec_num
  - multigroup_neutron or dosimetry_neutron \a handle:
      - \c NDI_REAC_Q: Reaction Q for reaction \a vec_num
      - \c NDI_TOT_PROD_Q: Total production Q

*/
int 
NDI2_get_float32_val_n(int handle, int in_key, int vec_num,
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

  \par Valid in_key values:
       ===================

  - gendir \a handle:
      - \c NDI_TEMP: Temperatures
      - \c NDI_SIG_0: Sigma zero
      - \c NDI_AT_WGT: Atomic weight
      - \c NDI_AWR: Atomic weight ratio
  - multigroup_neutron \& dosimetry_neutron \a handle:
      - \c NDI_E_BOUNDS: Energy group boundaries
      - \c NDI_E_AVG: Average group energies
      - \c NDI_ENERGY_N: Energy given to neutrons
      - \c NDI_ENERGY_AVAIL: Energy available
      - \c NDI_WGTS: Group weights
      - \c NDI_VEL: Group velocities
      - \c NDI_HEATING: Group heating
      - \c NDI_KERMA: Group kerma
      - \c NDI_HEATING_CURR_PART: Partial heating for current particle
      - \c NDI_KERMA_CURR_PART: Partial kerma for current particle
      - \c NDI_SIG_TOT: Sigma total
      - \c NDI_NU_TOT: Nubar total
      - \c NDI_NU_PR: Nubar prompt
      - \c NDI_NU_DEL: Nubar delayed
      - \c NDI_NU_SIG_F_TOT: Nubar sigma fission total
      - \c NDI_NU_SIG_F_PR: Nubar sigma fission prompt
      - \c NDI_NU_SIG_F_DEL: Nubar sigma fission delayed
      - \c NDI_CHI_VEC_TOT: Chi vector total
      - \c NDI_CHI_VEC_PR: Chi vector prompt
      - \c NDI_CHI_VEC_DEL: Chi vector delayed
      - \c NDI_CHI_MAT_TOT: Chi matrix total
      - \c NDI_CHI_MAT_DEL: Chi matrix delayed
      - \c NDI_CHI_MAT_PR: Chi matrix prompt
      - \c NDI_NORM: Chi normalization
      - \c NDI_FISS_Q: Fission Q
      - \c NDI_REAC_Q: Reaction Q
      - \c NDI_E_BOUNDS_CURR_PART: Secondary particle energy boundaries
      - \c NDI_TOT_DEPL_XS: Total depletion cross section
  - tn \a handle:
      - \c NDI_TEMPS: Temperatures
      - \c NDI_SIGVBARS: Sigma Vbars
      - \c NDI_EINBAR: Thermal-averaged total incident energy
      - \c NDI_EDIST: Current particle's energy distribution
      - \c NDI_EDIST_TEMPS: Temperature interpolation type 
      - \c NDI_E_BOUNDS: Energy boundaries for secondary particle
      - \c NDI_E_AVG: Unweighted average group energy

*/
int 
NDI2_get_float32_vec(int handle, int in_key,
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

  \par Valid in_key values:
       ===================

  - multigroup_neutron \& dosimetry_neutron \a handle:
      - \c NDI_PN_DOWN_FULL: Pn scattering matrix - downscatter diagonal
      - \c NDI_PN_FULL: Full Pn scattering matrix
      - \c NDI_SIG_REAC: Sigma edit cross sections
      - \c NDI_PNU_PR: Prompt fission neutron multiplicity probabilities
      - \c NDI_SIG_PNU_PR:
      - \c NDI_PN_PROD_FULL_CURR_PART: Full production matrix current particle
      - \c NDI_TRANSFER_MAT_TOT: Transfer matrix total
      - \c NDI_TRANSFER_MAT_PR: Transfer matrix prompt
      - \c NDI_CORR_DIAG: Diagonal transport correction
      - \c NDI_CORR_BHS: Bell-Hansen-Sandmeier transport correction
      - \c NDI_TOT_PROD_XS: Total production cross section for product \a vec_num

*/
int 
NDI2_get_float32_vec_n(int handle, int in_key, int vec_num,
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

  \par Valid in_key values:
       ===================

  - multigroup_neutron \& dosimetry_neutron \a handle:
      - \c NDI_TOT_PROD_XS: Total production cross section for product \a in_str
      - \c NDI_RPROD_MLT: Reaction multiplicity for product \a in_str
*/
int 
NDI2_get_float32_vec_x(int handle, int in_key,
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

  \par Valid in_key values:
       ===================

  - multigroup_neutron \a handle:
      - \c NDI_TEMP: Temperature
      - \c NDI_SIG_0: Sigma zero
      - \c NDI_AT_WGT: Atomic weight
      - \c NDI_AWR: Atomic weight ratio
  - tn \a handle:
      - \c NDI_REAC_Q: Reaction Q value
      - \c NDI_EAVG_OUT: Average outgoing energy
      - \c NDI_EOUTBAR: Average outgoing energy

*/
int 
NDI2_get_float64_val(int handle, int in_key, ndi_float64 *ret_val)
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

  \par Valid in_key values:
       ===================

  - All \a handles:
      - \c NDI_ABUNDANCE: Natural abundance for isotope \a vec_num
  - multigroup_neutron or dosimetry_neutron \a handle:
      - \c NDI_REAC_Q: Reaction Q for reaction \a vec_num
      - \c NDI_TOT_PROD_Q: Total production Q

*/
int 
NDI2_get_float64_val_n(int handle, int in_key, int vec_num,
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

  \par Valid in_key values:
       ===================

  - gendir \a handle:
      - \c NDI_TEMP: Temperatures
      - \c NDI_SIG_0: Sigma zero
      - \c NDI_AT_WGT: Atomic weight
      - \c NDI_AWR: Atomic weight ratio
  - multigroup_neutron \& dosimetry_neutron \a handle:
      - \c NDI_E_BOUNDS: Energy group boundaries
      - \c NDI_E_AVG: Average group energies
      - \c NDI_ENERGY_N: Energy given to neutrons
      - \c NDI_ENERGY_AVAIL: Energy available
      - \c NDI_WGTS: Group weights
      - \c NDI_VEL: Group velocities
      - \c NDI_HEATING: Group heating
      - \c NDI_KERMA: Group kerma
      - \c NDI_HEATING_CURR_PART: Partial heating for current particle
      - \c NDI_KERMA_CURR_PART: Partial kerma for current particle
      - \c NDI_SIG_TOT: Sigma total
      - \c NDI_NU_TOT: Nubar total
      - \c NDI_NU_PR: Nubar prompt
      - \c NDI_NU_DEL: Nubar delayed
      - \c NDI_NU_SIG_F_TOT: Nubar sigma fission total
      - \c NDI_NU_SIG_F_PR: Nubar sigma fission prompt
      - \c NDI_NU_SIG_F_DEL: Nubar sigma fission delayed
      - \c NDI_CHI_VEC_TOT: Chi vector total
      - \c NDI_CHI_VEC_PR: Chi vector prompt
      - \c NDI_CHI_VEC_DEL: Chi vector delayed
      - \c NDI_CHI_MAT_TOT: Chi matrix total
      - \c NDI_CHI_MAT_DEL: Chi matrix delayed
      - \c NDI_CHI_MAT_PR: Chi matrix prompt
      - \c NDI_NORM: Chi normalization
      - \c NDI_FISS_Q: Fission Q
      - \c NDI_REAC_Q: Reaction Q
      - \c NDI_E_BOUNDS_CURR_PART: Secondary particle energy boundaries
      - \c NDI_TOT_DEPL_XS: Total depletion cross section
  - tn \a handle:
      - \c NDI_TEMPS: Temperatures
      - \c NDI_SIGVBARS: Sigma Vbars
      - \c NDI_EINBAR: Thermal-averaged total incident energy
      - \c NDI_EDIST: Current particle's energy distribution
      - \c NDI_EDIST_TEMPS: Temperature interpolation type 
      - \c NDI_E_BOUNDS: Energy boundaries for secondary particle
      - \c NDI_E_AVG: Unweighted average group energy

*/
int 
NDI2_get_float64_vec(int handle, int in_key,
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

  \par Valid in_key values:
       ===================

  - multigroup_neutron \& dosimetry_neutron \a handle:
      - \c NDI_PN_DOWN_FULL: Pn scattering matrix - downscatter diagonal
      - \c NDI_PN_FULL: Full Pn scattering matrix
      - \c NDI_SIG_REAC: Sigma edit cross sections
      - \c NDI_PNU_PR: Prompt fission neutron multiplicity probabilities
      - \c NDI_SIG_PNU_PR:
      - \c NDI_PN_PROD_FULL_CURR_PART: Full production matrix current particle
      - \c NDI_TRANSFER_MAT_TOT: Transfer matrix total
      - \c NDI_TRANSFER_MAT_PR: Transfer matrix prompt
      - \c NDI_CORR_DIAG: Diagonal transport correction
      - \c NDI_CORR_BHS: Bell-Hansen-Sandmeier transport correction
      - \c NDI_TOT_PROD_XS: Total production cross section for product \a vec_num

*/
int 
NDI2_get_float64_vec_n(int handle, int in_key, int vec_num,
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
  \brief Get the 64-bit floating-point \a return_vec indicated by \a in_key
  and \a in_str from \a handle.

  \param handle: integer input handle
  \param in_key: integer input key indicating data sought
  \param in_str: string input name of element sought
  \param return_vec: float64 array output containing the values returned by NDI
  \param vec_size: maximum size of float64 array
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only) 

  \par Valid in_key values:
       ===================

  - multigroup_neutron \& dosimetry_neutron \a handle:
      - \c NDI_TOT_PROD_XS: Total production cross section for product \a in_str
      - \c NDI_RPROD_MLT: Reaction multiplicity for product \a in_str
*/
int 
NDI2_get_float64_vec_x(int handle, int in_key,
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

  \par Valid in_key values:
       ===================

  - Any \a handle:
      - \c NDI_VERSION: NDI version
      - \c NDI_GENDIR_PATH: Gendir file path
  - multigroup_neutron \& dosimetry_neutron \a handle:
      - \c NDI_ZAID: Zaid
      - \c NDI_INFO: Information line
      - \c NDI_DATE_PROCESSED: Date zaid was processed
      - \c NDI_DATE_SOURCE: Date of zaid source info
      - \c NDI_LIBRARY_NAME: Name of current library
      - \c NDI_DEPL_ZAID: Depletion zaid
      - \c NDI_DEPL_INFO: Depletion information
      - \c NDI_SEC_PART_TYPES: Type of current particle
      - \c NDI_SEC_PART_ZAIDS: Zaid of current particle
  - tn \a handle:
      - \c NDI_ZAID: Zaid
      - \c NDI_INFO: Information line
      - \c NDI_DATE_PROCESSED: Date zaid was processed
      - \c NDI_DATE_SOURCE: Date of zaid source info
      - \c NDI_LIBRARY_NAME: Name of current library
  - tnreactions \a handle:
      - \c NDI_ZAID: Zaid
      - \c NDI_INFO: Information line
      - \c NDI_DATE_PROCESSED: Date zaid was processed 
      - \c NDI_DATE_SOURCE: Date of zaid source info
      - \c NDI_LIBRARY_NAME: Name of current library
  
*/
int 
NDI2_get_string_val(int handle, int in_key, char *ret_val,
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

  \par Valid in_key values:
       ===================

  - multigroup_neutron \& dosimetry_neutron \a handle:
      - \c NDI_ZAID: Zaid
      - \c NDI_SEC_PART_TYPES: Secondary particle types
      - \c NDI_SEC_PART_ZAIDS: Secondary particle zaids
      - \c NDI_TARGET: Zaid of targect specified by \a vec_num
      - \c NDI_RPRODS_ID: Reaction product id for product \a vec_num
      - \c NDI_INC_PART_TYPES: Incident particle types
      - \c NDI_REAC_LABEL: Reaction label by MT
  - multigroup_multi \a handle:
      - \c NDI_REAC_LABEL: Reaction label by MT
  - tn \a handle:
      - \c NDI_SEC_PART_TYPES: Secondary particle types
      - \c NDI_RPRODS_ID: Reaction product id for product \a vec_num
      - \c NDI_REACTANTS_ID: Reactant id specified by \a vec_num
  - tnreactions \a handle:
      - \c NDI_LIST_REAC: List of reactions

*/
int 
NDI2_get_string_val_n(int handle, int in_key, int vec_num,
                          char *ret_val, int str_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_string_val_n_h(state_p, in_key, vec_num, 
				     ret_val, str_size));
}

/*!
  \brief Get the string array \a ret_val indicated by \a in_key from \a handle.

  \param handle: integer input handle
  \param in_key: integer input key indicating the data sought
  \param ret_val: string output array, allocated by the user, 
  containing the values returned by NDI.
  \param str_size: integer input indicating the allocated size of ret_val.
  \param ndi_err: integer output error code (Fortran only)
  \return integer output error code (C only)

  \par Valid in_key values:
       ===================

  - gendir \a handle:
      - \c NDI_ZAID: Zaid
 
*/
int 
NDI2_get_string_vec(int handle, int in_key, char *ret_val,
                        int str_size)
{
    state_struct *state_p;

    state_p = handle_to_state_p(handle);
    return (ndi_c_get_string_vec_h(state_p, in_key, ret_val, str_size));
}

/*! @} end of handle_get */
