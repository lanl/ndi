#include <stdio.h>
#include <string.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static int set_mpmg_library(state_struct *mpmg_handle,
                            const char *value );

/**********************************************************************/

int ndimp_set_option(state_struct *mpmg_handle, 
                     int in_key, const char *value )
{

    int ierr = 0;

    /* Check each particle is multigroup */
    /* Check num_parts <= MAX_HANDLE_NUM */

    switch (in_key) {

    case NDI_LIBRARY:
	ierr = set_mpmg_library(mpmg_handle, value);
	break;

    default:
	ierr = -4;
	break;
    }

    return (ierr);

}

/**********************************************************************/
static int set_mpmg_library(state_struct *mpmg_handle,
                            const char *value )
{

#define MPMG_LIB_LABEL_LENGTH       24

    typedef char label_type[MPMG_LIB_LABEL_LENGTH];

    int i;
    int ierr = 0;
    int tmp_err = 0;
    int num_parts;
    int value_len;
    char *library;
    char *token;
    char seps[] = " ,\t\n";
    label_type *lib_types = NULL;
    label_type *lib_names = NULL;
    label_type *grp_structs = NULL;
    state_struct *curr_h;

    value_len = (int) strlen(value) + 1;
    library = (char *) ndi_malloc(value_len * sizeof(char));
    ndi_niceStrCp(library, value, value_len);

    token = strtok(library, seps);
    if (strncmp(token,"ngk6", 5) == 0) {
        /*num_parts == 2*/
        /* n_lib = mendf6; p_lib = mendf6g */
        /* n_grp = 21_lanl; p_grp = 12_phot */
        num_parts = 2;
        lib_types = (label_type *)ndi_malloc(num_parts *
                                                sizeof(label_type));
        lib_names = (label_type *)ndi_malloc(num_parts *
                                                sizeof(label_type));
        grp_structs = (label_type *)ndi_malloc(num_parts *
                                               sizeof(label_type));
        strncpy(lib_types[0], "multigroup_neutron", 19);
        strncpy(lib_types[1], "multigroup_photon", 18);
        strncpy(lib_names[0], "mendf6", 7);
        strncpy(lib_names[1], "mendf6g", 8);
        strncpy(grp_structs[0], "21_lanl", 8);
        strncpy(grp_structs[1], "12_phot", 8);

    } else if (strncmp(token,"m6_30_12", 9) == 0) {
        /*num_parts == 2*/
        /* n_lib = mendf6; p_lib = mendf6g */
        /* n_grp = 30_lanl; p_grp = 12_phot */
        num_parts = 2;
        lib_types = (label_type *)ndi_malloc(num_parts *
                                                sizeof(label_type));
        lib_names = (label_type *)ndi_malloc(num_parts *
                                                sizeof(label_type));
        grp_structs = (label_type *)ndi_malloc(num_parts *
                                               sizeof(label_type));
        strncpy(lib_types[0], "multigroup_neutron", 19);
        strncpy(lib_types[1], "multigroup_photon", 18);
        strncpy(lib_names[0], "mendf6", 7);
        strncpy(lib_names[1], "mendf6g", 8);
        strncpy(grp_structs[0], "30_lanl", 8);
        strncpy(grp_structs[1], "12_phot", 8);


    } else if (strncmp(token,"acti_all", 9) == 0) {
        /*num_parts == 2*/
        /* n_lib = acti; p_lib = actig */
        /* n_grp = 130_lanl; p_grp = 120_phot */
        num_parts = 2;
        lib_types = (label_type *)ndi_malloc(num_parts *
                                                sizeof(label_type));
        lib_names = (label_type *)ndi_malloc(num_parts *
                                                sizeof(label_type));
        grp_structs = (label_type *)ndi_malloc(num_parts *
                                               sizeof(label_type));
        strncpy(lib_types[0], "multigroup_neutron", 19);
        strncpy(lib_types[1], "multigroup_photon", 18);
        strncpy(lib_names[0], "acti", 5);
        strncpy(lib_names[1], "actig", 6);
        strncpy(grp_structs[0], "130_lanl", 9);
        strncpy(grp_structs[1], "120_phot", 9);

    } else {
        ierr = 54;
    }


    if (ierr == 0) {
        for (i = 0; i < MAX_HANDLE_NUM; i++) /* close children */
            if (mpmg_handle->handle_map[i] != NULL) 
                ierr = ndi_c_close_h(&(mpmg_handle->handle_map[i]));
        mpmg_handle->handle_free = 0;
        
        for (i = 0; i < num_parts; i++) {
            tmp_err = __ndi_add_child_handle(mpmg_handle);
            curr_h = mpmg_handle->handle_map[i];
            if ( (tmp_err != 0) &&
                 ((ierr >= 0) || (tmp_err < 0 )))
                ierr = tmp_err;
            
            tmp_err = ndi_c_set_option_h(curr_h, NDI_LIB_TYPE, lib_types[i]);
            if ( (tmp_err != 0) &&
                 ((ierr >= 0) || (tmp_err < 0 )))
                ierr = tmp_err;
            
            tmp_err = ndi_c_set_option_h(curr_h, NDI_LIBRARY, lib_names[i]);
            if ( (tmp_err != 0) &&
                 ((ierr >= 0) || (tmp_err < 0 )))
                ierr = tmp_err;
            
            tmp_err = ndi_c_set_option_h(curr_h, NDI_COLLAPSE, grp_structs[i]);
            if ( (tmp_err != 0) &&
                 ((ierr >= 0) || (tmp_err < 0 )))
                ierr = tmp_err;
        }
    }

    if (library) 
        ndi_free((void **) &library);

    if (lib_types) 
        ndi_free((void **) &lib_types);

    if (lib_names)
        ndi_free((void **) &lib_names);

    if (grp_structs)
        ndi_free((void **) &grp_structs);

    return (ierr);
}

/**********************************************************************/
