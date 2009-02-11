
#include <stdlib.h>

void ndi_free(void **p)
{

    if (*p)
	free(*p);
    *p = NULL;

}
