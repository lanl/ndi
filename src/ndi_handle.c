#include <stdio.h>

#include "ndi_internal.h"
#include "ndi_state.h"

static state_struct *handle_map[MAX_HANDLE_NUM];

state_struct *handle_to_state_p(int handle) {

     state_struct *state_p = NULL;

     if (0 <= handle  && handle < MAX_HANDLE_NUM) {
          state_p = handle_map[handle];
     } else if (MAX_HANDLE_NUM <= handle && 
		handle < (MAX_HANDLE_NUM * MAX_HANDLE_NUM)) {
	 state_p = handle_map[handle / MAX_HANDLE_NUM - 1]->
	     handle_map[handle % MAX_HANDLE_NUM];
     }

     return(state_p);
}

int set_handle(state_struct *state) {

     int i;
     int handle = -1;

     for (i = 0; i < MAX_HANDLE_NUM; i++) {
          if (! handle_map[i]) {
               handle = i;
               handle_map[handle] = state;
               if (state) {
                   state->handle = handle;
               }
               break;
          }
     }

     return(handle);
}

int __ndi_close_handle(int handle) {

    int ret_val;
    state_struct *state_p;

    if ((state_p = handle_to_state_p(handle))) {
         ret_val = ndi_c_close_h(&state_p);
         handle_map[handle] = NULL;
         return (ret_val);
    } else {
          return(-2);
     }

}
