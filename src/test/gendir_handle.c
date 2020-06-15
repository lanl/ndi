#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ndi.h"

#define OPT_SIZE    24
#define GENDIR_PATH_SIZE  200
#define ZAID_SIZE   24

static void print_float64_vec(ndi_float64 *flt64_ptr, int num_elements);
static void print_string_vec(char *str_arr, int num_elements);

int main ()

{
   int ierr;
   int gendir0, gendir1;
   int handle0, handle1;
   char gendir_path[GENDIR_PATH_SIZE];
   char option[OPT_SIZE];
   char zaid[ZAID_SIZE];
 
   int size, zaid_size;
   char *str_arr;
   ndi_float64 *flt_arr;
   
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

   /* Requirement 45 */

   printf("  List of U zaids\n\n");
   size = NDI2_get_size_x(gendir0, NDI_ZAID, "92", &ierr);
   if (ierr != 0) {
       printf("    **Error getting zaid size is %d\n", ierr);
   } else {
       printf("  Size of list is %d\n", size);
       str_arr = (char *) malloc(size * sizeof(char));
       ierr = NDI2_get_string_vec_x(gendir0, NDI_ZAID, "92", str_arr, size);
       if (ierr != 0) 
	   printf("    **Error getting zaids is %d\n", ierr);
       print_string_vec(str_arr, size);
       size = NDI2_get_size_x(gendir0, NDI_TEMP, "92", &ierr);
       if (ierr != 0)
	   printf("    ** Error getting temp size is %d\n", ierr);
       free(str_arr);
   }
       
   printf("  List of atomic weights\n");
   size = NDI2_get_size_x(gendir0, NDI_AT_WGT, "92", &ierr);
   if (ierr != 0) {
       printf("    ** Error getting at_wgt size is %d\n", ierr);
   } else {
       printf("  Size of list is %d\n", size);
       flt_arr = (ndi_float64 *) malloc(size * sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec_x(gendir0, NDI_AT_WGT, "92", flt_arr, size);
       if (ierr != 0)
	   printf("    **Error getting at_wgts is %d\n", ierr);
       print_float64_vec(flt_arr, size);
       free(flt_arr);
   }

   /* Test different ZAID formats */

   printf("  List of atomic weights and ZAIDS, 92235\n");
   size = NDI2_get_size_x(gendir0, NDI_AT_WGT, "92235", &ierr);
   if (ierr != 0) printf("    ** Error getting at_wgt size is %d\n", ierr);
   zaid_size = NDI2_get_size_x(gendir0, NDI_ZAID, "92235", &ierr);
   if (ierr != 0) {
       printf("    ** Error getting zaid size is %d\n", ierr);
   } else {
       printf("  Size of list is %d\n", zaid_size);
       str_arr = (char *) malloc(zaid_size * sizeof(char));
       ierr = NDI2_get_string_vec_x(gendir0, NDI_ZAID, "92235", str_arr, zaid_size);
       if (ierr != 0)
           printf("    **Error getting zaids is %d\n", ierr);
       print_string_vec(str_arr, zaid_size);
       free(str_arr);
       printf("  Size of list is %d\n", size);
       flt_arr = (ndi_float64 *) malloc(size * sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec_x(gendir0, NDI_AT_WGT, "92235", flt_arr, size);
       if (ierr != 0)
           printf("    **Error getting at_wgts is %d\n", ierr);
       print_float64_vec(flt_arr, size);
       free(flt_arr);
   }

   printf("  List of atomic weights and ZAIDS, 92235.6\n");
   size = NDI2_get_size_x(gendir0, NDI_AT_WGT, "92235.6", &ierr);
   if (ierr != 0) printf("    ** Error getting at_wgt size is %d\n", ierr);
   zaid_size = NDI2_get_size_x(gendir0, NDI_ZAID, "92235.6", &ierr);
   if (ierr != 0) {
       printf("    ** Error getting zaid size is %d\n", ierr);
   } else {
       printf("  Size of list is %d\n", zaid_size);
       str_arr = (char *) malloc(zaid_size * sizeof(char));
       ierr = NDI2_get_string_vec_x(gendir0, NDI_ZAID, "92235.6", str_arr, zaid_size);
       if (ierr != 0)
           printf("    **Error getting zaids is %d\n", ierr);
       print_string_vec(str_arr, zaid_size);
       free(str_arr);
       printf("  Size of list is %d\n", size);
       flt_arr = (ndi_float64 *) malloc(size * sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec_x(gendir0, NDI_AT_WGT, "92235.6", flt_arr, size);
       if (ierr != 0)
           printf("    **Error getting at_wgts is %d\n", ierr);
       print_float64_vec(flt_arr, size);
       free(flt_arr);
   }

   printf("  List of atomic weights and ZAIDS, 92235.600\n");
   size = NDI2_get_size_x(gendir0, NDI_AT_WGT, "92235.600", &ierr);
   if (ierr != 0) printf("    ** Error getting at_wgt size is %d\n", ierr);
   zaid_size = NDI2_get_size_x(gendir0, NDI_ZAID, "92235.600", &ierr);
   if (ierr != 0) {
       printf("    ** Error getting zaid size is %d\n", ierr);
   } else {
       printf("  Size of list is %d\n", zaid_size);
       str_arr = (char *) malloc(zaid_size * sizeof(char));
       ierr = NDI2_get_string_vec_x(gendir0, NDI_ZAID, "92235.600", str_arr, zaid_size);
       if (ierr != 0)
           printf("    **Error getting zaids is %d\n", ierr);
       print_string_vec(str_arr, zaid_size);
       free(str_arr);
       printf("  Size of list is %d\n", size);
       flt_arr = (ndi_float64 *) malloc(size * sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec_x(gendir0, NDI_AT_WGT, "92235.600", flt_arr, size);
       if (ierr != 0)
           printf("    **Error getting at_wgts is %d\n", ierr);
       print_float64_vec(flt_arr, size);
       free(flt_arr);
   }

   printf("  List of atomic weights and ZAIDS, 92235.600nm\n");
   size = NDI2_get_size_x(gendir0, NDI_AT_WGT, "92235.600nm", &ierr);
   if (ierr != 0) printf("    ** Error getting at_wgt size is %d\n", ierr);
   zaid_size = NDI2_get_size_x(gendir0, NDI_ZAID, "92235.600nm", &ierr);
   if (ierr != 0) {
       printf("    ** Error getting zaid size is %d\n", ierr);
   } else {
       printf("  Size of list is %d\n", zaid_size);
       str_arr = (char *) malloc(zaid_size * sizeof(char));
       ierr = NDI2_get_string_vec_x(gendir0, NDI_ZAID, "92235.600nm", str_arr, zaid_size);
       if (ierr != 0)
           printf("    **Error getting zaids is %d\n", ierr);
       print_string_vec(str_arr, zaid_size);
       free(str_arr);
       printf("  Size of list is %d\n", size);
       flt_arr = (ndi_float64 *) malloc(size * sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec_x(gendir0, NDI_AT_WGT, "92235.600nm", flt_arr, size);
       if (ierr != 0)
           printf("    **Error getting at_wgts is %d\n", ierr);
       print_float64_vec(flt_arr, size);
       free(flt_arr);
   }

       
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
   ierr = NDI2_open_gendir(&gendir1, "lib/gendir.test2");
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

/**********************************************************************/

static void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements )
{
   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 4 ) == 0) )
	 printf("\n");
      printf("%22.15e  ", flt64_ptr[n]);
   }
   printf("\n\n");
}

/**********************************************************************/

/**********************************************************************/

static void print_string_vec(char *str_arr, int size)
{
    int i, count;
    char *t;
    t = str_arr;

    i = 0;
    count = 0;
    while ( i < size ) {
	count++;
	printf(" Isotope %d: zaid=%s\n",count,t);
        i+= strlen(t)+1;
	t+= strlen(t)+1;
    }
}

/**********************************************************************/

