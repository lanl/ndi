#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ndi.h"

typedef double float64;

int mgn_data(void);

#ifdef __STANDALONE
int main ()
{
  int ierr = 0;

  ierr = mgn_data();
  return(ierr);
}
#endif  /* __STANDALONE */

int mgn_data(void)
{
   int ierr;
   int array_size;
   int zaid_num;
   int zaid_count = 0;
   char zaid[64];
   char *curr_zaid;
   char *zaid_list;

  printf("\n\n********** NDI DATA TEST SUITE -- mgn_data **********\n");

   ierr = ndi_init();
   if ( ierr != 0 ) {
      return(0);
   }

   ierr = ndid_c_get_size( NDI_ZAID, "multigroup_neutron", &array_size );
   if ( array_size > 0 ) {
       zaid_list = (char *)malloc(array_size);
       if (zaid_list) {
	   ierr = ndid_c_get_num( NDI_ZAID, "multigroup_neutron", &zaid_num);
	   if ( ierr != 0 ) printf("***Error %d getting number of zaids.\n", ierr );
	   ierr = ndid_c_get_string_vec( NDI_ZAID, "multigroup_neutron", zaid_list, array_size );
	   if ( ierr != 0 ) printf("  Error %d getting zaid list.\n", ierr );
	   
   
	   ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
	   if ( ierr != 0 ) {
	       printf("***Error %d setting lib_type.", ierr );
	       if (ierr < 0) {
		   printf("  Quitting with fatal error.\n");
		   return(ierr);
	       } else {
		   printf("\n");
	       }
	   }

	   curr_zaid = zaid_list;
	   while ( (zaid_num > 0) && ( curr_zaid != NULL ) ) {
	       ierr = ndi_set_isotope( curr_zaid );
	       if ( ierr != 0 ) printf("***Error %d setting isotope %s.\n", ierr, curr_zaid );
	       ierr = ndi_get_string_val( NDI_ZAID, zaid, 64);
	       if (ierr != 0) printf("   ***Error %d getting isotope zaid %s.\n", ierr, curr_zaid );
	       printf("   %s\n", zaid);

	       zaid_num--;
	       zaid_count++;
	       curr_zaid = strchr(curr_zaid, '\0') + 1;
	   }
	   printf("\nZAID COUNT = %d\n", zaid_count);
	   ierr = ndi_close();
	   if ( ierr != 0 ) printf("  Error closing NDI is %d.\n\n", ierr );

       } else {
	   printf("Bad zaid list.  Quitting.\n");
       }
   } else {
       printf("\n          NO MG_NEUTRON DATA\n");
   }

   return(ierr);
}
