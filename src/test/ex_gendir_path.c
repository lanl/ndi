#include <stdio.h>
#include <stdlib.h>
#include "ndi.h"

int main() {

    int ierr;
    int str_size;
    char *path = NULL;

    ierr = ndi_init();
    if ( ierr != 0 ) {
        return(0);
    }

    /* Note that only ndi_init needs to be called before asking for NDI_GENDIR_PATH */
    str_size = ndi_get_size(NDI_GENDIR_PATH, &ierr);
    if (ierr != 0) {
        printf("  Error getting size of path = %d\n", ierr);
    }
    path = (char *)malloc(str_size * sizeof(char));
    ierr = ndi_get_string_val(NDI_GENDIR_PATH, path, str_size);

    printf("  GENDIR PATH: %s (size=%d; err=%d)\n\n", path, str_size, ierr);

    ierr = ndi_close();
    
    return(0);
}
