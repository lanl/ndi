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
   int handle1,  handle2, handle3;
   double at_wgt, awr;

   char zaid[80];

   printf("\n  ********************************************************* \n" );
   printf("    TEST - Set blank ZAID (should return -22 error) \n" );
   printf("  ********************************************************** \n\n" );

   /* Test NDI_CHAIN with dosimetry_neutron handle */
   ierr = NDI2_open_gendir(&gendir_h, "");
   if (ierr != 0) printf("**Error %d in NDI init.\n\n", ierr );
   ierr = NDI2_make_handle(gendir_h, "dosimetry_neutron", &handle1);
   if (ierr != 0) printf("**Error %d making dosimetry_neutron handle.\n", ierr);

   /* Set NDI_CHAIN empty zaid */
   ierr = NDI2_set_option(handle1, NDI_CHAIN, "");
   if (ierr != 0) printf("**Error %d in setting empty chain ZAID\n",ierr);
   str_size = NDI2_get_size(handle1, NDI_CHAIN_ZAID, &ierr);
   if (ierr != 0) printf("**Error %d in getting chain ZAID size\n",ierr);
   ierr = NDI2_get_string_val(handle1, NDI_CHAIN_ZAID, zaid, str_size);
   if (ierr != 0) printf("**Error %d in getting chain ZAID\n",ierr);

   /* Set proper NDI_CHAIN_ZAID */
   ierr = NDI2_make_handle(gendir_h, "dosimetry_neutron", &handle1);
   if (ierr != 0) printf("**Error %d making dosimetry_neutron handle.\n", ierr);

   ierr = NDI2_set_option(handle1, NDI_CHAIN, "ir.001zrc"); //39087.713ny");
   if (ierr != 0) printf("**Error %d in setting chain ZAID\n",ierr);
   str_size = NDI2_get_size(handle1, NDI_CHAIN_ZAID, &ierr);
   if (ierr != 0) printf("**Error %d in getting chain ZAID size\n",ierr);
   ierr = NDI2_get_string_val(handle1, NDI_CHAIN_ZAID, zaid, str_size);
   if (ierr != 0) printf("**Error %d in getting chain ZAID\n",ierr);

   printf("Chain ZAID = %s\n", zaid);

   /* Set NDI_DEPLETION empty zaid */
   ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle2);
   if (ierr != 0) printf("**Error %d making multigroup_neutron handle.\n", ierr);
   ierr = NDI2_set_option(handle2, NDI_LIBRARY, "mendf6");
   if (ierr != 0) printf("**Error %d in setting library ZAID\n",ierr);
   ierr = NDI2_set_option(handle2, NDI_DEPLETION, "");
   if (ierr != 0) printf("**Error %d in setting empty depletion ZAID\n",ierr);

   str_size = NDI2_get_size(handle2, NDI_DEPL_ZAID, &ierr);
   if (ierr != 0) printf("**Error %d in getting chain ZAID size\n",ierr);
   ierr = NDI2_get_string_val(handle2, NDI_DEPL_ZAID, zaid, str_size);
   if (ierr != 0) printf("**Error %d in getting depletion ZAID\n",ierr);

   /* Set proper NDI_DEPL_ZAID */
   ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle3);
   if (ierr != 0) printf("**Error %d making multigroup_neutron handle.\n", ierr);
   ierr = NDI2_set_option(handle3, NDI_LIBRARY, "mendf6");
   if (ierr != 0) printf("**Error %d in setting library ZAID\n",ierr);
   ierr = NDI2_set_option(handle3, NDI_DEPLETION, "test2");//"test_depl" );
   if (ierr != 0) printf("**Error %d setting depletion ZAID.\n", ierr);

   str_size = NDI2_get_size(handle3, NDI_DEPL_ZAID, &ierr);
   if (ierr != 0) printf("**Error %d in getting depletion ZAID size\n",ierr);
   ierr = NDI2_get_string_val(handle3, NDI_DEPL_ZAID, zaid, str_size);
   if (ierr != 0) printf("**Error %d in getting depletion ZAID\n",ierr);

   printf("Depletion ZAID = %s\n", zaid);

   ierr = NDI2_close_gendir(gendir_h);

   return(ierr);
}
