#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"
#include "ndi_sf.h"

#define MAX_SF_PNU 10

int main(int argc, char *argv[])
{
    int i, z;
    char keyword[50];
    char* zas[] = {"92238", "94240", "94242", "94239"};

    int gendir_h;
    int handle;
    int ierr;
    ndi_float64 pnu, sfr;

    /* Test of spontaneous fission rate and p(nu) (multiplicity) */

    ierr = NDI2_open_gendir(&gendir_h, "./gendir");
    if (ierr) printf("  **Error %d in NDI init.\n\n", ierr);
    ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle);

    for (z = 0; z < 4; z++){
        ierr = NDI2_set_isotope(handle, zas[z]);
        if (ierr) printf("  **Error %d setting isotope.\n", ierr);

        ierr = NDI2_get_float64_val(handle, NDI_SF_RATE, &sfr);
        if (ierr) printf("  **Error %d getting spontaneous fission rate.\n", ierr);
	printf("\nSpontaneous fission rate for isotope %s = %e\n\n",zas[z], sfr);
	
        for ( i = 0; i < MAX_SF_PNU; i++ ) {
            ierr = NDI2_get_float64_val_n(handle, NDI_SF_PNU, i, &pnu);
            if (ierr) {
                printf("  **Error %d getting spontaneous fission p(nu) for %s.\nBreaking loop.\n\n", ierr, zas[z]);
                break;
            }
	    printf("Spontaneous fission multiplicity %d = %f\n", i, pnu);
	   
        }
    }

    ierr = NDI2_close_gendir(gendir_h);
    if (ierr) printf("  **Error %d in closing gendir\n", ierr);

    return 0;
}

