#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

int main ()

{
   int ierr;
   int i;
   int z, a, str_size;
   int gendir_h;
   int handle;
   double at_wgt, awr;

   char *zaid = "1.800nm";
   char libname[80];

   printf("\n  ********************************************************* \n" );
   printf("    TEST - Set neutron \"isotope\" \n" );
   printf("  ********************************************************** \n\n" );

   ierr = NDI2_open_gendir(&gendir_h, "lib/gendir.neut");
   if (ierr != 0) printf("**Error %d in NDI init.\n\n", ierr );
   ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle);
   if (ierr != 0) printf("**Error %d making particle handle.\n", ierr);

   /*  Set neutron "isotope" */
   printf("ISOTOPE=%s\n\n",zaid);
   ierr = NDI2_set_isotope(handle, zaid);
   if (ierr != 0) printf("\n**Error %d in setting isotope %s.\n", ierr, zaid);

   /* Get integer data */
  
   ierr = NDI2_get_int_val(handle, NDI_ATOMIC_NUM, &z);
   if (ierr != 0) printf("\n**Error %d in retrieving atomic number.\n", ierr);

   ierr = NDI2_get_int_val(handle, NDI_MASS_NUM, &a);
   if (ierr != 0) printf("\n**Error %d in retrieving mass number.\n", ierr);

   printf("  Z=%d;  A=%d\n\n", z, a);

   /* Get string data */

   str_size = NDI2_get_size(handle, NDI_LIBRARY_NAME, &ierr);
   if (ierr != 0) printf("\n**Error %d in getting library name size.\n", ierr);

   ierr = NDI2_get_string_val(handle, NDI_LIBRARY_NAME, libname, str_size);
   if (ierr != 0) printf("\n**Error %d in getting library name.\n", ierr);

   printf("  Library name=%s\n", libname);

   /* Get float data */

   ierr = NDI2_get_float64_val(handle, NDI_AT_WGT, &at_wgt);
   if (ierr != 0) printf("\n**Error %d in retrieving atomic weight.\n", ierr);

   ierr = NDI2_get_float64_val(handle, NDI_AWR, &awr);
   if (ierr != 0) printf("\n**Error %d in retrieving atomic weight ratio.\n", ierr);

   printf("  Atomic weight=%f; AWR=%f\n\n", at_wgt, awr);

   ierr = NDI2_close_gendir(gendir_h);

   return(ierr);
}
