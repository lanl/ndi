#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

int main (void)
{
    int ierr = 0;
    int str_size;
    char *version;

    ierr = ndi_init();
    if (ierr) printf("**Error %d initializing NDI.\n", ierr);
    str_size = ndi_get_size( NDI_VERSION, &ierr );
    if (ierr) printf("**Error %d getting version size.\n", ierr);
    version = (char *)malloc(str_size * sizeof(char) );
    ierr = ndi_get_string_val(NDI_VERSION, version, str_size);
    if (ierr) printf("**Error %d getting version.\n", ierr);
    printf("NDI VERSION IS %s.\n", version );

    return(ierr);
}
