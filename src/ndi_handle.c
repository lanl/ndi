#include <stdio.h>

#include "ndi_internal.h"
#include "ndi_state.h"

static state_struct *handle_map[MAX_HANDLE_NUM] = { NULL }; /* gendir array */
/*!
  \fn is_gendir
  \brief determine if handle is gendir
  \param handle
  \return 1 if gendir
*/
int is_gendir(state_struct *state)
{
    return state && state->ndi_initialized && 
	0 <= state->handle && state->handle < MAX_HANDLE_NUM;
}

int is_mpmg(state_struct *state)
{
    return state && state->ndi_initialized && state->library_type == 'p';
}

/*!
  \fn handle_to_state_p
  \brief get state_struct represented by handle
  \param handle: integer handle
  \return state_struct if successful; NULL if error
*/
state_struct *handle_to_state_p(int handle)
{
    state_struct *state_p = NULL;
    int m1, m2, m3;
    int i, j, k;

    m1 = MAX_HANDLE_NUM;
    m2 = m1 * m1;
    m3 = m1 * m2;

    if (0 <= handle && handle < m1) { 
				/* handle = k */
	k = handle;
	state_p = handle_map[k];

    } else if (m1 <= handle && handle < m1 + m2) { 
                                /* handle = (1+j)m1 + k */
	 handle -= m1;		/* handle =    j*m1 + k */
	 j = handle / m1;
	 handle -= j * m1;	/* handle = k */
	 k = handle;
	 state_p = handle_map[j] -> handle_map[k];

    } else if (m1 + m2 <= handle && handle < m1 + m2 + m3) {
				/* handle = (1+i)m2 + (1+j)m1 + k */
	 handle -= (m1 + m2);	/* handle =    i*m2 +    j*m1 + k */
	 i = handle / m2;
	 handle -= i * m2;	/* handle =              j*m1 + k */
	 j = handle / m1;
	 handle -= j * m1;	/* handle =                     k */
	 k = handle;
	state_p = handle_map[i] -> handle_map[j] -> handle_map[k];
    }	
    return (state_p);
}

/*!
  \fn set_handle
  \brief set first free level 0 handle to state
  \param state: state_struct
  \return handle if successful, negative if error
*/
int set_handle(state_struct *state)
{

    int i;
    int handle = -1;

    if (state) {
        for (i = 0; i < MAX_HANDLE_NUM; i++) {
            if (!handle_map[i]) {
                handle = i;
                handle_map[handle] = state;
                if (state) {
                    state->handle = handle;
                }
                break;
            }
        }
    }

    return (handle);
}

/*!
  \fn __ndi_close_handle
  \brief close the specified level 0 handle
  \param handle
  \return 0 if sucessful; negative if error
*/
int __ndi_close_handle(int handle)
{
    int ret_val;
    state_struct *state_p;

    if (0 <= handle && handle < MAX_HANDLE_NUM &&
	(state_p = handle_to_state_p(handle))) {
	 ret_val = ndi_c_close_h(&state_p);
	 handle_map[handle] = NULL;
	 return (ret_val);
    } else {
	 return (-2);
    }
}

int __ndi_add_child_handle(state_struct *parent)
{
    int h;
    state_struct *child = NULL;

    if (! parent) 
        return(-2);

    h = parent->handle_free;	/* get next child handle */
    if (h < MAX_HANDLE_NUM && !(parent->handle_map[h])) {
	ndi_c_clone_h(parent, &child);
	parent->handle_map[h] = child;
	parent->handle_free++;
	if (child) {
	    child->handle = h + (parent->handle + 1) * MAX_HANDLE_NUM;
	}
    } else {
	return -1;		/* handle array exhausted */
    }
    
    return 0;
}

char *ndi_get_section(state_struct *state)
{
    static char *in_value[] = {"multigroup_neutron", 
			       "neutron_depletion", 
			       "multigroup_photon", 
			       "tndata", 
			       "tn_reactions", 
			       "dosimetry_neutron",
			       "radchem_chain",
			       "" };
    int sec;

    /* Find library section */
    if (state->library_type == 'm' && state->particle_type == 1) {
	if (state->ndi_depl_set != 1)
	    sec = 0;
	else
	    sec = 1;
    } else if (state->library_type == 'm' && state->particle_type == 0) {
	sec = 2;
    } else if (state->library_type == 's') {
	sec = 3;
    } else if (state->library_type == 'r') {
	sec = 4;
    } else if (state->library_type == 'd') {
	if (state->ndi_depl_set != 1)
	    sec = 5;
	else 
	    sec = 6;
    } else {
	sec = 7;
    }

    return (in_value[sec]);
}

int ndi_is_library_type(state_struct *state, const char *type)
{

    if (!state)
	return (0);
    if (strcmp(type, "multigroup_neutron") == 0)
	return (state->library_type == 'm' && state->particle_type == 1);
    else if (strcmp(type, "multigroup_photon") == 0)
	return (state->library_type == 'm' && state->particle_type == 0);
    else if (strcmp(type, "tn") == 0)
	return (state->library_type == 's');
    else if (strcmp(type, "tnreactions") == 0)
	return (state->library_type == 'r');
    else if (strcmp(type, "dosimetry_neutron") == 0)
	return (state->library_type == 'd' && state->particle_type == 1);
    else if (strcmp(type, "multigroup_multi") == 0)
	return (state->library_type == 'p');
    else 
	return (0);
}
