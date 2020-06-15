
/*
 * $Id$
 * Description: User function--return a string value for string key.
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

static int get_string_vec_x_gendir(state_struct *state, int in_key, 
                                   const char *in_str, 
                                   char *str_arr, int arr_size);

/**********************************************************************/

/*  Purpose: Return a string array associated with in_str.
    
Parameters: 
in_key -- in: key to specify type of data required
in_str -- in: string to specify key
str_vec -- out: string value returned
arr_size -- in: tells how large the string is (how many characters)

Return values:  Returns negative value for error condition,
positive value for warning, and a '0' if everything's ok. 

***********************************************************************/
int ndi_c_get_string_vec_x_h(state_struct *state, int in_key, 
                             const char *in_str, 
                             char *str_arr, int arr_size)

{
    int ierr = 0;

    if ((str_arr) && (arr_size > 0)) {
        *str_arr = '\0';
    } else {
        NDI_THROW(ierr, 7);     /* Array too small */
    }

    if ((!state) || (!state->ndi_initialized)) {
        NDI_THROW(ierr, -2);    /* State not initialized */
    }

    if (is_gendir(state)) {
        ierr = get_string_vec_x_gendir(state, in_key, in_str, 
                                       str_arr, arr_size);
    } else if ((state->library_type == 'm') || 
               (state->library_type == 'd')) {
        NDI_THROW(ierr, -2);
    } else if (state->library_type == 's') {
        NDI_THROW(ierr, -2);
    } else if (state->library_type == 'p') {
        NDI_THROW(ierr, -2);
    } else if (state->library_type == ' ') {
        NDI_THROW(ierr, -16);   /* Library type was not set */
    } else {
        NDI_THROW(ierr, -4);    /* Unrecognized library type */
    }

 catch:
    return ierr;

}

/**********************************************************************/
/**********************************************************************/

static int get_string_vec_x_gendir(state_struct *state, int in_key,
                                   const char *in_str,
                                   char *str_arr, int arr_size)
{
    int ierr = 0;
    long fpos = -1;
    dir_entry rec;
    char *section;
    int size;

    if (!state && !state->ndi_dir) /* check state sanity */
        NDI_THROW(ierr, -2);

    section = ndi_get_section(state);
    if (section == 0)
        NDI_THROW(ierr, -16);   /* Library type not set */

    fpos = ftell(state->ndi_dir); /* remember file position */
        
    if (ndi_fseek_section(state->ndi_dir, section, 0) != 0)
        NDI_THROW(ierr, -11);  /* Section not found */

    while (ndi_fget_next_dir(&rec, state->ndi_dir,
                             state->library_vers,  in_str)) {
        switch (in_key) {

        case NDI_ZAID:
            size = strlen(rec.zaid) + 1; /* include EOS */
            if (size > arr_size) {
                size = arr_size;
                NDI_THROW(ierr, 7); /* Data truncated */
            }
            strcpy(str_arr, rec.zaid); /* add string */
            arr_size -= size;   /* reduce available space */
            str_arr += size;    /* point past added string */
            if (arr_size == 0) goto catch; /* Out of space */
            break;
        default:
            NDI_THROW(ierr, -4);
        }
    }

 catch:
    if (fpos >= 0) fseek(state->ndi_dir, fpos, SEEK_SET);
    return ierr;
}

/**********************************************************************/
