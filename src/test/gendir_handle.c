#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ndi.h"

#define OPT_SIZE    24
#define GENDIR_PATH_SIZE  200
#define ZAID_SIZE   24

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );
void print_float32_vec( ndi_float32 *flt_ptr, int num_elements );
void print_int_vec( int *int_ptr, int num_elements );


int main ()

{
   int ierr;
   int gendir0, gendir1;
   int handle0, handle1;
   char gendir_path[GENDIR_PATH_SIZE];
   char option[OPT_SIZE];
   char zaid[ZAID_SIZE];
 
   /* GENDIR 0 */
   ierr = NDI2_open_gendir(&gendir0, "");
   if (ierr)
     printf("  **Error %d in NDI open gendir.\n\n", ierr );

   ierr = NDI2_set_option_gendir(gendir0, NDI_LIB_TYPE_DEFAULT, 
				 "multigroup_neutron"); 
   if (ierr)
       printf("  **Error %d in setting lib_type.\n\n", ierr );
   ierr = NDI2_set_option_gendir(gendir0, NDI_LIBRARY_DEFAULT, "mendf5");
   if (ierr)
       printf("  **Error %d in setting library.\n\n", ierr );
   ierr = NDI2_set_option_gendir(gendir0, NDI_COLLAPSE_DEFAULT, "16_lanl");
   if (ierr)
       printf("  **Error %d in setting group structure.\n\n", ierr );
   ierr = NDI2_set_option_gendir(gendir0, NDI_FISS_PROD_DEFAULT, "50999");
   if (ierr)
       printf("  **Error %d in setting default fission product.\n\n", ierr );

   ierr = NDI2_get_string_val(gendir0, NDI_GENDIR_PATH, gendir_path, GENDIR_PATH_SIZE);
   printf("GENDIR0 (path=%s)\n", gendir_path);
   ierr = NDI2_check_option_gendir(gendir0, NDI_LIB_TYPE_DEFAULT, option, OPT_SIZE);
   printf("  Library type:  %s\n", option);
   ierr = NDI2_check_option_gendir(gendir0, NDI_LIBRARY_DEFAULT, option, OPT_SIZE);
   printf("  Library:  %s\n", option);
   ierr = NDI2_check_option_gendir(gendir0, NDI_MAT_ORDER_DEFAULT, option, OPT_SIZE);
   printf("  Matrix order:  %s\n", option);
   ierr = NDI2_check_option_gendir(gendir0, NDI_COLLAPSE_DEFAULT, option, OPT_SIZE);
   printf("  Collapse:  %s\n", option);
   ierr = NDI2_check_option_gendir(gendir0, NDI_TRANS_CORR_DEFAULT, option, OPT_SIZE);
   printf("  Transport correction:  %s\n", option);
   ierr = NDI2_check_option_gendir(gendir0, NDI_NORM_DEFAULT, option, OPT_SIZE);
   printf("  Normalize:  %s\n", option);
   ierr = NDI2_check_option_gendir(gendir0, NDI_REAC_ID_DEFAULT, option, OPT_SIZE);
   printf("  Reaction identifier:  %s\n", option);
   ierr = NDI2_check_option_gendir(gendir0, NDI_E_AVG_DEFAULT, option, OPT_SIZE);
   printf("  Average group energy:  %s\n", option);
   ierr = NDI2_check_option_gendir(gendir0, NDI_KERMA_DEFAULT, option, OPT_SIZE);
   printf("  Kerma:  %s\n", option);
   ierr = NDI2_check_option_gendir(gendir0, NDI_FISS_PROD_DEFAULT, option, OPT_SIZE);
   if (ierr != 0)
       printf("    **Error getting default fission product is %d\n", ierr);
   printf("  Default fission product:  %s\n", option);

   /* GET HANDLE 0 */
   ierr = NDI2_make_handle(gendir0, "multigroup_neutron", &handle0);
   if (ierr)
       printf("  **Error %d in NDI make_handle.\n\n", ierr );
   printf("  Handle0\n");
   ierr = NDI2_get_string_val(handle0, NDI_GENDIR_PATH, gendir_path, GENDIR_PATH_SIZE);
   printf("    Gendir path:  %s\n", gendir_path);
   ierr = NDI2_check_option(handle0, NDI_LIB_TYPE, option, OPT_SIZE);
   printf("    Library type:  %s\n", option);
   ierr = NDI2_check_option(handle0, NDI_LIBRARY, option, OPT_SIZE);
   printf("    Library:  %s\n", option);
   ierr = NDI2_check_option(handle0, NDI_MAT_ORDER, option, OPT_SIZE);
   printf("    Matrix order:  %s\n", option);
   ierr = NDI2_check_option(handle0, NDI_COLLAPSE, option, OPT_SIZE);
   printf("    Collapse:  %s\n", option);
   ierr = NDI2_check_option(handle0, NDI_TRANS_CORR, option, OPT_SIZE);
   printf("    Transport correction:  %s\n", option);
   ierr = NDI2_check_option(handle0, NDI_NORM, option, OPT_SIZE);
   printf("    Normalize:  %s\n", option);
   ierr = NDI2_check_option(handle0, NDI_REAC_ID, option, OPT_SIZE);
   printf("    Reaction identifier:  %s\n", option);
   ierr = NDI2_check_option(handle0, NDI_E_AVG, option, OPT_SIZE);
   printf("    Average group energy:  %s\n", option);
   ierr = NDI2_check_option(handle0, NDI_KERMA, option, OPT_SIZE);
   printf("    Kerma:  %s\n", option);
   ierr = NDI2_check_option(handle0, NDI_FISS_PROD, option, OPT_SIZE);
   printf("    Default fission product:  %s\n", option);

   /* SET GENDIR0 KERMA DEFAULT VALUE */
   ierr = NDI2_set_option_gendir(gendir0, NDI_KERMA_DEFAULT, "use_data_calc");
   if (ierr)
       printf("  **Error %d in setting gendir kerma default option.\n", ierr );

  /* GET HANDLE 1 */
   ierr = NDI2_make_handle(gendir0, "multigroup_neutron", &handle1);
   if (ierr)
       printf("  **Error %d in NDI make_handle.\n\n", ierr );
   ierr = NDI2_set_option(handle1, NDI_TRANS_CORR, "bhs_3");
   if (ierr)
       printf("  **Error %d in setting handle1 transport correction.\n\n", ierr );
   printf("  Handle1\n");
   ierr = NDI2_get_string_val(handle1, NDI_GENDIR_PATH, gendir_path, GENDIR_PATH_SIZE);
   printf("    Gendir path:  %s\n", gendir_path);
   ierr = NDI2_check_option(handle1, NDI_LIB_TYPE, option, OPT_SIZE);
   printf("    Library type:  %s\n", option);
   ierr = NDI2_check_option(handle1, NDI_LIBRARY, option, OPT_SIZE);
   printf("    Library:  %s\n", option);
   ierr = NDI2_check_option(handle1, NDI_MAT_ORDER, option, OPT_SIZE);
   printf("    Matrix order:  %s\n", option);
   ierr = NDI2_check_option(handle1, NDI_COLLAPSE, option, OPT_SIZE);
   printf("    Collapse:  %s\n", option);
   ierr = NDI2_check_option(handle1, NDI_TRANS_CORR, option, OPT_SIZE);
   printf("    Transport correction:  %s\n", option);
   ierr = NDI2_check_option(handle1, NDI_NORM, option, OPT_SIZE);
   printf("    Normalize:  %s\n", option);
   ierr = NDI2_check_option(handle1, NDI_REAC_ID, option, OPT_SIZE);
   printf("    Reaction identifier:  %s\n", option);
   ierr = NDI2_check_option(handle1, NDI_E_AVG, option, OPT_SIZE);
   printf("    Average group energy:  %s\n", option);
   ierr = NDI2_check_option(handle1, NDI_KERMA, option, OPT_SIZE);
   printf("    Kerma:  %s\n", option);
   ierr = NDI2_check_option(handle1, NDI_FISS_PROD, option, OPT_SIZE);
   printf("    Default fission product:  %s\n", option);


   /* GENDIR 1 SETUP */
   ierr = NDI2_open_gendir(&gendir1, "../../src/test/lib/gendir.test2");
   if (ierr)
       printf("  **Error %d in NDI open gendir.\n\n", ierr );
   ierr = NDI2_set_option_gendir(gendir1, NDI_LIB_TYPE_DEFAULT, 
				 "multigroup_neutron"); 
   if (ierr)
       printf("  **Error %d in setting lib_type.\n\n", ierr );

   /* ACCESS GENDIR DATA */
   ierr = NDI2_get_string_val(gendir1, NDI_GENDIR_PATH, gendir_path, GENDIR_PATH_SIZE);
   printf("\nGENDIR1 (path=%s)\n", gendir_path);

   /* GET LIBRARY HANDLE */
   ierr = NDI2_make_handle(gendir1, "multigroup_neutron", &handle0);
   if (ierr)
       printf("  **Error %d in NDI make_handle.\n\n", ierr );
   ierr = NDI2_set_isotope(handle0, "4009");
   if (ierr)
       printf("  **Error %d in setting zaid.\n\n", ierr );
   ierr = NDI2_get_string_val(handle0, NDI_ZAID, zaid, ZAID_SIZE);
    if (ierr)
       printf("  **Error %d in getting zaid.\n\n", ierr );
  printf("  Handle0\n    Zaid=%s\n", zaid);

   /* CLOSE GENDIRS*/
   ierr = NDI2_close_gendir(gendir0);
   if (ierr)
     printf("  **Error %d in closing NDI 0.\n\n", ierr );
   ierr = NDI2_close_gendir(gendir1);
   if (ierr)
     printf("  **Error %d in closing NDI 1.\n\n", ierr );

   return(ierr);
}

