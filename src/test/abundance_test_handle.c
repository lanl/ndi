#include <stdio.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_var_type.h"


int main ()

{

#include "ndi_abundance.h"

    int ierr;
    int i;
    int last_Z = 1;
    int gendir_h = -1;
    int handle = -1;
    ndi_float64 return_val;
    ndi_float64 abund_sum = 0.0;

    printf("\n  ********************************************************* \n" );
    printf("    TEST--Abundances\n " );
    printf(" ********************************************************** \n\n" );
  
    ierr = ndi2_open_gendir(&gendir_h, "");
    if (ierr)
	printf("  **Error %d in NDI init.\n\n", ierr );
    ierr = ndi2_get_handle(gendir_h, &handle);
    if (ierr)
	printf("  **Error %d getting particle handle.\n\n", ierr );
    ierr = ndi2_set_option( handle, NDI_LIB_TYPE, "dosimetry_neutron" );
    if (ierr)
	printf("  **Error %d in setting library type.\n\n", ierr );
   
    /* Abundances should not require a library type or isotope to be set.
       However, that is currently not true.  "Dosimetry" must be the library
       type and an isotope must be set.  When this restriction is removed, 
       add new tests to check for this.
    */

     ierr = ndi2_set_isotope(handle, "39086");
     if (ierr)
	printf("  **Error %d in setting isotope.\n\n", ierr );

     for ( i = 0; i < num_of_abundance_values; i++ ) {
	 ierr = ndi2_get_float64_val_n( handle, NDI_ABUNDANCE, abundance_id[i], &return_val );
	 if ( last_Z != abundance_id[i]/1000 ) {
	     printf("        %11.8f\n\n", abund_sum );
	     abund_sum = 0.0;
	     last_Z = abundance_id[i]/1000;
	 }
	 abund_sum += return_val;
	 printf("%6d  %11.8f\n", abundance_id[i], return_val );
     }
     printf("        %11.8f\n\n", abund_sum );

     ierr = ndi2_close_gendir(gendir_h);
     return(ierr);
}
