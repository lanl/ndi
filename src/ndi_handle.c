#include <stdio.h>

#include "ndi_internal.h"
#include "ndi_state.h"

static state_struct *handle_map[MAX_HANDLE_NUM];

state_struct *handle_to_state_p(int handle)
{

    state_struct *state_p = NULL;
    int i, j, k;

    if (0 <= handle && handle < MAX_HANDLE_NUM) {
				/* gendir level */
	 state_p = handle_map[handle];
    } else if (MAX_HANDLE_NUM <= handle &&
	       handle < (MAX_HANDLE_NUM*(MAX_HANDLE_NUM+1))) {
				/* mg level */
	 handle -= MAX_HANDLE_NUM;
	 i = handle / MAX_HANDLE_NUM;
	 j = handle % MAX_HANDLE_NUM;
	 state_p = handle_map[i] -> handle_map[j];
    } else if (MAX_HANDLE_NUM * (MAX_HANDLE_NUM+1) <= handle &&
	       handle < MAX_HANDLE_NUM*(1+MAX_HANDLE_NUM*
					(1+MAX_HANDLE_NUM))) {
				/* mpmg level */
	 handle -= MAX_HANDLE * (MAX_HANDLE_NUM+1);
	 i = handle / (MAX_HANDLE_NUM * MAX_HANDLE_NUM);
	 handle -= i * (MAX_HANDLE_NUM * MAX_HANDLE_NUM);
	 j = handle / MAX_HANDLE_NUM;
	 k = handle % MAX_HANDLE_NUM;
	 state_p = handle_map[i] -> handle_map[j] -> handle_map[k];
    }
    return (state_p);
}

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

int __ndi_close_handle(int handle)
{

    int ret_val;
    state_struct *state_p;

    if ((state_p = handle_to_state_p(handle))) {
	 ret_val = ndi_c_close_h(&state_p);
	 if (0 <= handle && handle < MAX_HANDLE_NUM) {
				/* gendir level */
	      handle_map[handle] = NULL;
	 } else if (MAX_HANDLE_NUM <= handle &&
		    handle < (MAX_HANDLE_NUM*(MAX_HANDLE_NUM+1))) {
				/* mg level */
	      handle -= MAX_HANDLE_NUM;
	      i = handle / MAX_HANDLE_NUM;
	      j = handle % MAX_HANDLE_NUM;
	      handle_map[i] -> handle_map[j] = NULL;
	 } else if (MAX_HANDLE_NUM * (MAX_HANDLE_NUM+1) <= handle &&
		    handle < MAX_HANDLE_NUM*(1+MAX_HANDLE_NUM*
					     (1+MAX_HANDLE_NUM))) {
				/* mpmg level */
	      handle -= MAX_HANDLE * (MAX_HANDLE_NUM+1);
	      i = handle / (MAX_HANDLE_NUM * MAX_HANDLE_NUM);
	      handle -= i * (MAX_HANDLE_NUM * MAX_HANDLE_NUM);
	      j = handle / MAX_HANDLE_NUM;
	      k = handle % MAX_HANDLE_NUM;
	      handle_map[i] -> handle_map[j] -> handle_map[k] = NULL;
	 }
	 state_p = NULL;
	 return (ret_val);
    } else {
	 return (-2);
    }
}
