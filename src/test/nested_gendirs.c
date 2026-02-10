#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ndi.h"

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );
void print_string_vec( char *vec, int num_elements );

int main ()

{
   int ierr;
   int i,j,k,g;
   int array_size, array_num;
   int str_size;
   int num_zaids = 6;
   int gendir_h = -1;
   int handle = -1;
   char zaid[25], library[25];
   char *info_str;
   char *str_ptr;
   char *zaid_list[] = { "3006.600nm", "3006", "3006.500nm", "3006", "3006.990nm", "92235.603nm" };
   char *library_list[] = { "mendf6", "mendf6", "mendf5", "mendf5", "rmoxsct", "ss_test" };
   char *str_arr;

   /*  Initialize */
   ierr = NDI2_open_gendir(&gendir_h, "./gendir.start");
   if (ierr != 0)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle);
   if (ierr != 0)
     printf("  Error %d making particle handle.\n", ierr);

   printf("\n");

   array_num = NDI2_get_size(gendir_h, NDI_TEMP, &ierr);
   if (ierr != 0)
     printf("  Error %d getting NDI_TEMP size with gendir handle.\n",ierr);

   array_size = NDI2_get_size(gendir_h, NDI_ZAID, &ierr);
   if (ierr != 0)
     printf("  Error %d getting NDI_ZAID size with gendir handle.\n",ierr);

   str_arr = (char *) malloc(array_size*sizeof(char));
   ierr = NDI2_get_string_vec(gendir_h, NDI_ZAID, str_arr, array_size);
   if (ierr != 0)
     printf("  Error %d getting NDI_ZAID vector of strings.\n",ierr);
   print_string_vec(str_arr, array_num);
   printf("\n");

   for ( j = 0; j < 1000; j++ ) { /* Repeat for better idea of timing */
   for ( i = 0; i < num_zaids; i++ ) {

     /* Set library */
     ierr = NDI2_set_option( handle, NDI_LIBRARY, library_list[i] );
     if (ierr != 0)
       printf("  **Error %d setting library %s.\n", ierr, library_list[i]);

     /* Set isotope */
     ierr = NDI2_set_isotope(handle, zaid_list[i]);
     if (ierr != 0) 
       printf("  **Error %d in setting isotope %s.\n", ierr, zaid_list[i]);

     /* Double check library */
     printf("Input library %s\n",library_list[i]);
     ierr = NDI2_get_string_val(handle, NDI_LIBRARY_NAME, library, 25);
     if (ierr != 0) {
       printf("  **Error %d in getting library %s.\n", ierr, library_list[i]);
     } else {
       printf("Returned library: %s\n", library);
     }

     /* Double check ZAID */
     printf("Input ZAID %s\n", zaid_list[i]);
     ierr = NDI2_get_string_val(handle, NDI_ZAID, zaid, 25);
     if (ierr != 0) {
       printf("  **Error %d in getting ZAID %s.\n", ierr, zaid_list[i]);
     } else {
       printf("Returned ZAID: %s\n\n", zaid);
     }

     printf("\n");
   } /* for i */
   } /* for j */

   printf("\nTEST NDI2_set_zaid_float64_temp_sig_0\n\n");

   ierr = NDI2_set_zaid_float64_temp_sig_0(handle, "92235", 5.17e-8, 1.0e1 );
   if (ierr)
     printf("  **Error %d in set_zaid_float64_temp_sig_0.\n\n", ierr );

   /* Double check ZAID */
   printf("Input ZAID %s\n", "92235");
   ierr = NDI2_get_string_val(handle, NDI_ZAID, zaid, 25);
   if (ierr != 0) {
     printf("  **Error %d in getting ZAID.\n", ierr);
   } else {
     printf("Returned ZAID: %s\n\n", zaid);
   }

   printf("\nTEST NDI2_set_zaid_float64_temp\n\n");

   ierr = NDI2_set_zaid_float64_temp(handle, "92235", 5.17e-8 );
   if (ierr)
     printf("  **Error %d in set_zaid_float64_temp.\n\n", ierr );

   /* Double check ZAID */
   printf("Input ZAID %s\n", "92235");
   ierr = NDI2_get_string_val(handle, NDI_ZAID, zaid, 25);
   if (ierr != 0) {
     printf("  **Error %d in getting ZAID.\n", ierr);
   } else {
     printf("Returned ZAID: %s\n\n", zaid);
   }

   ierr = NDI2_close_gendir(gendir_h);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );

   printf("\nTEST gendir.newdata\n\n");

   /*  Initialize */
   ierr = NDI2_open_gendir(&gendir_h, "./gendir.start2");
   if (ierr != 0)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle);
   if (ierr != 0)
     printf("  Error %d making particle handle.\n", ierr);

   ierr = NDI2_set_option( handle, NDI_LIBRARY, "mendf6" );
   if (ierr != 0) 
     printf("  **Error %d in setting library mendf6.\n", ierr);

   ierr = NDI2_set_isotope(handle, "3006");
   if (ierr != 0)
     printf("  **Error %d in setting isotope 3006.\n", ierr);

   printf("Set ZAID: 3006\n");
   ierr = NDI2_get_string_val(handle, NDI_ZAID, zaid, 25);
   if (ierr != 0) {
     printf("  **Error %d in getting ZAID.\n", ierr);
   } else {
     printf("Returned ZAID: %s\n\n", zaid);
   }

   printf("Input library mendf6\n");
   ierr = NDI2_get_string_val(handle, NDI_LIBRARY_NAME, library, 25);
   if (ierr != 0) {
     printf("  **Error %d in getting library mendf6.\n", ierr);
   } else {
     printf("Returned library: %s\n", library);
   }

   ierr = NDI2_set_isotope(handle, "3006.600nm");
   if (ierr != 0)
     printf("  **Error %d in setting isotope 3006.\n", ierr);

   printf("Set ZAID: 3006.600nm\n");
   ierr = NDI2_get_string_val(handle, NDI_ZAID, zaid, 25);
   if (ierr != 0) {
     printf("  **Error %d in getting ZAID.\n", ierr);
   } else {
     printf("Returned ZAID: %s\n\n", zaid);
   }

   printf("Input library %s\n","mendf6");
   ierr = NDI2_get_string_val(handle, NDI_LIBRARY_NAME, library, 25);
   if (ierr != 0) {
     printf("  **Error %d in getting library mendf6.\n", ierr);
   } else {
     printf("Returned library: %s\n", library);
   }

   ierr = NDI2_close_gendir(gendir_h);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );

   printf("\n");

   return(ierr);
}

/**********************************************************************/

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 6 ) == 0) )
          printf("\n");
      printf("%13.6e   ", flt64_ptr[n]);
   }
   printf("\n");
}

void print_string_vec( char *vec, int num_elements ) {

   char *str_p;
   int count = 0;

   str_p = vec;
   while(count < num_elements) {
     count++;
     printf("%d: %s\n", count, str_p);
     str_p += strlen(str_p) + 1;
   }

}

/**********************************************************************/

