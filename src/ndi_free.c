#include <stdlib.h>

void ndi_free(void **p) {

  free(*p);
  *p = NULL;

}
