
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
static int get_nested_string_vec_x_gendir(state_struct *state, int in_key,
                                          const char *in_str, FILE *stream,
                                          char **str_arr, int *arr_size);

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
    char *section;

    if (!state && !state->ndi_dir) /* check state sanity */
        NDI_THROW(ierr, -2);

    section = ndi_get_section(state);
    if (section == 0)
        NDI_THROW(ierr, -16);      /* Library type not set */

    fpos = ftell(state->ndi_dir);  /* remember file position */
        
    if (ndi_fseek_section(state->ndi_dir, section, 0) != 0)
        NDI_THROW(ierr, -11);      /* Section not found */

    ierr = get_nested_string_vec_x_gendir(state, in_key, in_str, state->ndi_dir, &str_arr, &arr_size);

 catch:
    if (fpos >= 0) fseek(state->ndi_dir, fpos, SEEK_SET);
    return ierr;
}

/**********************************************************************/

static int get_nested_string_vec_x_gendir(state_struct *state, int in_key,
                                          const char *in_str, FILE *stream,
                                          char **str_arr, int *arr_size)
{
    long int size = 0;
    int ierr = 0;
    dir_entry rec;
    FILE *temp_stream;
    dir_entry *d = NULL;
    char *path = NULL;
    long int len_diff = 0;
    char *token;
    char seps[] = "ngz";
    char seps_dot[] = ".";
    char zaid[MAX_IN_STR_LEN];


    while ((d = ndi_fget_dir_block(&rec,stream)) != NULL) {
        strcpy(zaid,d->zaid);

        if (strncmp(d->zaid,"gendir",strlen("gendir")) == 0 &&
            strncmp(d->lib,state->library_vers,strlen(state->library_vers)) == 0 &&
            strlen(d->lib) == strlen(state->library_vers)) {

            ierr = ndi_open_gendir(&temp_stream, d->path, &path);
            ierr = get_nested_string_vec_x_gendir(state, in_key, in_str, temp_stream,
                                                  str_arr, arr_size);
            if (fclose(temp_stream) != 0) NDI_THROW(ierr, -3);

        } else {

            token =  strtok(zaid,seps_dot);
            if (token == NULL) continue;
            len_diff = strlen(in_str) - strlen(token);
            if (len_diff > 0) {
                strcpy(zaid,d->zaid);
                token = strtok(zaid,seps);
            }
            if ((len_diff > 1 && strncmp(token, in_str, strlen(in_str)) == 0) || /* Most of the zaid matches */
                (strncmp(d->zaid, in_str, strlen(d->zaid)) == 0) || /* All of the zaid matches */
                ((strncmp(d->zaid, in_str, strlen(in_str)) == 0) && /* The zaid matches to the ".", and... */
                 ((strcmp("", state->library_vers) == 0) || /* there is no default library */
                  (strcmp(d->lib, state->library_vers) == 0)))) { /* or the default library matches */

                switch(in_key) {
                    case NDI_ZAID:
                        size = strlen(rec.zaid) + 1; /* include EOS */
                        if (size > *arr_size) {
                            size = *arr_size;
                            NDI_THROW(ierr, 7); /* Data truncated */
                        }
                        strcpy(*str_arr, rec.zaid);
                        *arr_size -= size;
                        *str_arr += size;
                        if (arr_size == 0) goto catch;
                        break;
                    default:
                        NDI_THROW(ierr, -4);
                }
            }
        }
    }
    catch:
        return ierr;
}
