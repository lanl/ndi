#include <stdio.h>
#include <stdlib.h>

#include "ndi.h"

void print_int_vec( int *int_ptr, int num_elements );

int main ()

{
   int ierr;
   int i, j;
   int num_reac, num_prod;
   int array_size = 10;
   int mt_array[10];
   int mlt_array[10];
   int id_array[10];


   /*  Initialize */
   ierr = ndi_init();
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi_set_isotope("92235.60");
   if (ierr) 
       printf("\n  **Error %d in setting isotope 92235.60.\n", ierr );
   

   num_reac = ndi_c_get_size(NDI_REAC_MT, &ierr);
   printf("Size of REAC_MT is %d\n", num_reac);
   ierr = ndi_c_get_int_vec(NDI_REAC_MT, mt_array, array_size);
   if (ierr)
       printf("   ERROR GETTING REAC_MT %d\n", ierr);
   for (i = 0; i < num_reac; i++) {
       num_prod = ndi_c_get_size_n(NDI_RPROD_ID_ALL, mt_array[i], &ierr);
       ierr = ndi_c_get_int_vec_n(NDI_RPROD_ID_ALL, mt_array[i], id_array, array_size);
       if (ierr)
           printf("   ERROR GETTING RPROD_ID_ALL (MT=%d) %d\n", ierr, mt_array[i]);
       ierr = ndi_c_get_int_vec_n(NDI_RPROD_MLT_ALL, mt_array[i], mlt_array, array_size);
       if (ierr)
           printf("   ERROR GETTING RPROD_MLT_ALL (MT=%d) %d\n", ierr, mt_array[i]);
       printf("REACTION %d:\n", mt_array[i]);
       for (j = 0; j < num_prod; j++) {
           printf("   Product %10d; multiplicity %d\n", id_array[j], mlt_array[j]);
       }
   }

   
   ierr = ndi_close();
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );

   return(ierr);
}

/**********************************************************************/

void print_int_vec( int *int_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 8 ) == 0) )
	 printf("\n");
      printf("%9d  ", int_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/
