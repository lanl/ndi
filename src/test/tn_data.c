#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ndi.h"

typedef double float64;

int tn_data(void);

#ifdef __STANDALONE
int main ()
{
  int ierr = 0;

  ierr = tn_data();
  return(ierr);
}
#endif  /* __STANDALONE */

int tn_data(void)
{
   int ierr;
   int array_size;
   int j;
   int zaid_num;
   int zaid_count = 0;
   int num_sec_parts;
   char part_type[10];
   char zaid[64];
   char *curr_zaid;
   char *zaid_list;
   float64 eavg;

  printf("\n\n********** NDI DATA TEST SUITE -- tn_data **********\n");

   ierr = ndi_init();
   if ( ierr != 0 ) {
      return(0);
   }

   ierr = ndid_c_get_size( NDI_ZAID, "tndata", &array_size );
   if ( array_size > 0 ) {
       zaid_list = (char *)malloc(array_size);
       if (zaid_list) {
	   ierr = ndid_c_get_num( NDI_ZAID, "tndata", &zaid_num);
	   if ( ierr != 0 ) printf("***Error %d getting number of zaids.\n", ierr );
	   ierr = ndid_c_get_string_vec( NDI_ZAID, "tndata", zaid_list, array_size );
	   if ( ierr != 0 ) printf("  Error %d getting zaid list.\n", ierr );

   
	   ierr = ndi_set_option( NDI_LIB_TYPE, "tn" );
	   if ( ierr != 0 ) {
	       printf("***Error %d setting lib_type.", ierr );
	       if (ierr < 0) {
		   printf("  Quitting with fatal error.\n");
		   return(ierr);
	       } else {
		   printf("\n");
	       }
	   }
	   ierr = ndi_set_option( NDI_TEMP, "2.0e-4" );
	   if (ierr != 0) printf("***Error setting temperature.\n");

	   curr_zaid = zaid_list;
	   while ( (zaid_num > 0) && ( curr_zaid != NULL ) ) {
	       ierr = ndi_set_reaction( curr_zaid );
	       if ( ierr != 0 ) printf("***Error %d setting reaction %s.\n", ierr, curr_zaid );
	       ierr = ndi_get_string_val( NDI_ZAID, zaid, 64);
	       if (ierr != 0) printf("   ***Error %d getting reactionzaid  %s.\n", ierr, curr_zaid );
	       printf("%s:\n", zaid);

	       ierr = ndi_get_int_val( NDI_NUM_SEC_PARTS, &num_sec_parts );
	       if ( (ierr != 0 ) && (fabs((double)ierr) != 9 ) )
		   printf("   ***Error %d getting number of secondary particles.\n", ierr);

	       for ( j = 0; j < num_sec_parts; j++ ) {
		   ierr = ndi_get_string_val_n( NDI_SEC_PART_TYPES, j, part_type, 10 );
		   if (ierr) printf("   ***Error %d getting particle type %d.\n", ierr, j );
		   ierr = ndi_set_option( NDI_CURR_PART, part_type );
		   if (ierr) printf("   ***Error %d setting current particle.\n", ierr );
		   ierr = ndi_get_float64_val( NDI_EAVG_OUT, &eavg );
		   if (ierr) printf("   ***Error %d getting Eavg for particle %s.\n", ierr, part_type );
		   printf( "   Particle %s:  %12.5e\n", part_type, eavg );
	       }
	       zaid_num--;
	       zaid_count++;
	       curr_zaid = strchr(curr_zaid, '\0') + 1;
	       printf("\n");
	   }
	   printf("ZAID COUNT = %d\n", zaid_count);
	   ierr = ndi_close();
	   if ( ierr != 0 ) printf("  Error closing NDI is %d.\n\n", ierr );

       } else {
	   printf("Bad zaid list.  Quitting.\n");
       }
   } else {
       printf("********NO TN DATA\n");
   }

   return(ierr);
}
