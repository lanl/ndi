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
int is_gendir(int handle)
{
    return 0 <= handle && handle < MAX_HANDLE_NUM;
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
