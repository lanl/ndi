#ifndef NDI_MPMG_H
#define NDI_MPMG_H 1

typedef struct ndi_mpmg ndi_mpmg;

extern ndi_mpmg *ndi_mpmg_new(char *zaid);
extern void ndi_mpmg_free(ndi_mpmg *);

#endif
