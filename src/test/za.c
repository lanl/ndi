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

   char *zaid_list[] = { "92235.600nm", "1001.", "1039087", "pu239", "10020"  };

   printf("\n\n  ********************************************************* \n" );
   printf("    TEST--RETRIEVE Z and A\n " );
   printf(" ********************************************************** \n\n" );

   ierr = ndi_init();
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
     printf("ISOTOPE=%s\n\n",
	    zaid_list[i] );
      ierr = ndi_set_isotope(zaid_list[i]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
  
      ierr = ndi_get_int_val( NDI_ATOMIC_NUM, &z );
      ierr = ndi_get_int_val( NDI_MASS_NUM, &a );

      printf (  "  Z=%d;  A=%d\n\n\n", z, a);
   }  
   ierr = ndi_close();

   return(ierr);
}
