#ifndef NDI_STATE_HANDLE_H
#define NDI_STATE_HANDLE_H 1

typedef struct ndi_state_handle ndi_state_handle;

extern ndi_state_handle *ndi_state_handle_new(void);
extern ndi_state_handle *ndi_state_handle_copy(ndi_state_handle *);
extern void ndi_state_handle_free(ndi_state_handle *);
extern void ndi_state_handle_swap(ndi_state_handle *);

#endif
