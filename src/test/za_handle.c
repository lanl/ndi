#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

int main ()

{
   int ierr;
   int i;
   int z,a;
   int num_zaid = 5;
   int gendir_h;
   int handle;
   double mass;

   char *zaid_list[] = { "92235.600nm", "1001.", "1039087", "pu239", "10020"  };

   printf("\n\n  ********************************************************* \n" );
   printf("    TEST--RETRIEVE Z and A\n " );
   printf(" ********************************************************** \n\n" );

   ierr = NDI2_open_gendir(&gendir_h, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle);
   if( ierr != 0) printf("  Error %d making particle handle.\n", ierr);


   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
     printf("ISOTOPE=%s\n\n",
	    zaid_list[i] );
      ierr = NDI2_set_isotope(handle, zaid_list[i]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
  
      ierr = NDI2_get_int_val( handle, NDI_ATOMIC_NUM, &z );
      ierr = NDI2_get_int_val( handle, NDI_MASS_NUM, &a );

      printf (  "  Z=%d;  A=%d\n\n\n", z, a);
   }  

   /* For neutron mass (direct from NDI) */
   ierr = NDI2_set_isotope(handle, zaid_list[0]);
   ierr = NDI2_get_float64_val(handle, NDI_NEUT_MASS, &mass);
   printf(" Neutron mass = %f and error = %d\n\n", mass, ierr);

   ierr = NDI2_close_gendir(gendir_h);

   return(ierr);
}
