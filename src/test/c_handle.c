#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

int testc (void);
static int check_all_ndi_get_mgn_functions(int handle);
static int check_ndi_get_mgn_functions_mt(int handle, int reac_id, char *reac_type );
static int ndi_unknown_keyword_mgn(int handle);
static int check_ndi_string_val_func_mgn(int handle);
static int check_ndi_int_val_func_mgn(int handle);
static int check_ndi_float_val_func_mgn(int handle);
static int check_ndi_string_val_n_func_mgn(int handle);
static int check_ndi_int_val_n_func_mgn(int handle);
static int check_ndi_float_val_n_func_mgn(int handle);
static int check_ndi_int_vec_func_mgn(int handle);
static int check_ndi_float_vec_func_mgn(int handle);
static int check_ndi_int_vec_n_func_mgn(int handle);
static int check_ndi_float_vec_n_func_mgn(int handle);
static int check_ndi_float_vec_x_func_mgn(int handle);
static int check_ndi_sec_part_mgn(int handle);
static void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );
static void print_float32_vec( ndi_float32 *flt_ptr, int num_elements );
static void print_int_vec( int *int_ptr, int num_elements );

#ifdef __STANDALONE
int main (void)
{
  int ierr = 0;

  ierr = testc();
  return(ierr);
}
#endif  /* __STANDALONE */

int testc (void)
{
    int handle = 0;
   int ierr = 0;
   int i;
   int num_zaid = 5;
   int ext_num_zaid = 24;
   char test_zaid[133] = "";

   char *zaid_list[] = { "92235.", "1001.600nm", "o16", "pu239", "10020"  };
   char *extend_zaid_list[] = { "o16", "pu239", "U-233", "fe", "016", "Fl-19",
				"ne20", "i129.6", "pu239.600nm", "u235500nm",
				"24000", "1001.", "92235.5", "26054.60", "94241.990",
				"3006.500n", "92233.600nm", "92233.600nr", "10020.",
				"92233600nm", "1001nm", "1001.nm", "1001.6.nm",
				"h1.6.nm"};
 
   printf("  NDI TEST SUITE -- testc \n");
/****************************************************************
    Test 1 -- Not initialized.
****************************************************************/
   printf("  ************************* TEST 1 ************************* \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Try getting data when NDI is not initialized.\n " );
   printf(" ********************************************************** \n\n" );
   
   /*  Without setting isotope */

   ierr = check_all_ndi_get_mgn_functions(handle);

   /* Set isotope */
   printf("***********************ISOTOPE=%s***********************\n\n",
	  zaid_list[0] );
   ierr = ndi2_set_isotope(handle, zaid_list[0]);
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, zaid_list[0] );
   
   ierr = check_all_ndi_get_mgn_functions(handle);
   ierr = ndi2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );

/****************************************************************
    Test 2 -- Initialized, but lib_type and isotope are not set.
****************************************************************/

   printf("\n\n  ************************* TEST 2 ************************* \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Lib_type and isotope are not set.\n " );
   printf(" ********************************************************** \n\n" );
   
   ierr = ndi2_open(&handle, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = check_all_ndi_get_mgn_functions(handle);
   ierr = ndi2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );


/****************************************************************
    Test 3 -- Initialized and lib_type set, but isotope is not set.
****************************************************************/

   printf("\n\n  ************************* TEST 3 ************************* \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Isotope is not set.\n " );
   printf(" ********************************************************** \n\n" );
   
   ierr = ndi2_open(&handle, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi2_set_option(handle, NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = check_all_ndi_get_mgn_functions(handle);
   ierr = ndi2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );

/****************************************************************
    Test 4 -- Get all multigroup data--no special options.
****************************************************************/

   printf("\n\n  ************************* TEST 4 ************************* \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--no collapsing\n " );
   printf(" ********************************************************** \n\n" );

   ierr = ndi2_open(&handle, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi2_set_option(handle, NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi2_set_option(handle, NDI_E_AVG, "from_e");

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
      /* Set isotope, no collapsing */
     printf("***********************ISOTOPE=%s***********************\n\n",
	    zaid_list[i] );
      ierr = ndi2_set_isotope(handle, zaid_list[i]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
  
      ierr = check_all_ndi_get_mgn_functions(handle);
   }  
   ierr = ndi2_close(handle);

/****************************************************************
    Test 5 -- Get all multigroup data--collasped to 16_LANL.
****************************************************************/

   printf("\n\n  ************************* TEST 5 *************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--collapse to 16_LANL\n " );
   printf(" ************************************************************ \n\n" );

   ierr = ndi2_open(&handle, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi2_set_option(handle, NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi2_set_option(handle, NDI_COLLAPSE, "16_group" );
   ierr = ndi2_set_option(handle, NDI_E_AVG, "from_e");

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
     /* Set isotope, no collapsing */
     printf("***********************ISOTOPE=%s***********************\n\n",
	    zaid_list[i] );
     ierr = ndi2_set_isotope(handle, zaid_list[i]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
     ierr = check_all_ndi_get_mgn_functions(handle);
   }  

   ierr = ndi2_set_option(handle, NDI_COLLAPSE, "default" );
   ierr = check_all_ndi_get_mgn_functions(handle);
   ierr = ndi2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );
    
/****************************************************************
    Test 6 -- Get all multigroup data--set default library.
****************************************************************/

   printf("\n\n  ************************* TEST 6 *************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--set library to mendf5\n " );
   printf(" ************************************************************ \n\n" );

   ierr = ndi2_open(&handle, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi2_set_option(handle, NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi2_set_option(handle, NDI_LIBRARY, "mendf5" );
   ierr = ndi2_set_option(handle, NDI_E_AVG, "from_e");

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
      /* Set isotope, no collapsing */
     printf("***********************ISOTOPE=%s***********************\n\n",
	    zaid_list[i] );
     ierr = ndi2_set_isotope(handle, zaid_list[i]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
     ierr = check_all_ndi_get_mgn_functions(handle);
   }  
   ierr = ndi2_set_option(handle, NDI_LIBRARY, "default" );
   ierr = check_all_ndi_get_mgn_functions(handle);
   ierr = ndi2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );


/****************************************************************
    Test 8 -- Get all multigroup data--set transport corrections.
****************************************************************/

   printf("\n\n  ************************* TEST 7 ********************************* \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--set transport corrections.\n " );
   printf(" ****************************************************************** \n\n" );

   ierr = ndi2_open(&handle, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi2_set_option(handle, NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi2_set_option(handle, NDI_E_AVG, "from_e");

   printf(" ***********Diagonal transport correction***********\n\n"); 
   ierr = ndi2_set_option(handle, NDI_TRANS_CORR, "diag_n" );
   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
      /* Set isotope, no collapsing */
     printf("***********************ISOTOPE=%s***********************\n\n",
	    zaid_list[i] );
     ierr = ndi2_set_isotope(handle, zaid_list[i]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
     ierr = check_all_ndi_get_mgn_functions(handle);
   }  

   printf("\n ***********Bell-Hansen-Sandmeier transport correction***********\n\n"); 
   ierr = ndi2_set_option(handle, NDI_TRANS_CORR, "bhs_n" );
   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
      /* Set isotope, no collapsing */
     printf("***********************ISOTOPE=%s***********************\n\n",
	    zaid_list[i] );
     ierr = ndi2_set_isotope(handle, zaid_list[i]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
     ierr = check_all_ndi_get_mgn_functions(handle);
   }  


   printf("\n ***********NO transport correction***********\n\n"); 
   ierr = ndi2_set_option(handle, NDI_TRANS_CORR, "none" );
   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
     
     /* Set isotope, no collapsing */
     printf("***********************ISOTOPE=%s***********************\n\n",
	    zaid_list[i] );
     ierr = ndi2_set_isotope(handle, zaid_list[i]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
     ierr = check_all_ndi_get_mgn_functions(handle);
   }  

   ierr = ndi2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );



/****************************************************************
    Test 9 -- Get all multigroup data--with depletion set.
****************************************************************/

   printf("\n\n  ************************* TEST 9 *************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--with depletion set\n " );
   printf(" ************************************************************ \n\n" );

   ierr = ndi2_open(&handle, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi2_set_option(handle, NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi2_set_option(handle, NDI_DEPLETION, "test_depl" );
   ierr = ndi2_set_option(handle, NDI_E_AVG, "from_e");

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
     /* Set isotope, no collapsing */
     printf("***********************ISOTOPE=%s***********************\n\n",
	    zaid_list[i] );
     ierr = ndi2_set_isotope(handle, zaid_list[i]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
     ierr = check_all_ndi_get_mgn_functions(handle);
   }  

   ierr = ndi2_set_option(handle, NDI_DEPLETION, "default" );
   ierr = check_all_ndi_get_mgn_functions(handle);
   ierr = ndi2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );
    


/****************************************************************
    Test 10 -- Get all multigroup data--with current particle set.
****************************************************************/

   printf("\n\n  ************************* TEST 10 ******************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--with current particle set\n " );
   printf(" ***************************************************************** \n\n" );

   ierr = ndi2_open(&handle, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi2_set_option(handle, NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi2_set_option(handle, NDI_CURR_PART, "n" );
   ierr = ndi2_set_option(handle, NDI_E_AVG, "from_e");

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
     /* Set isotope, no collapsing */
     printf("***********************ISOTOPE=%s***********************\n\n",
	    zaid_list[i] );
     ierr = ndi2_set_isotope(handle, zaid_list[i]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
     ierr = check_all_ndi_get_mgn_functions(handle);
   }  
   
   ierr = ndi2_close(handle);
    if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );
   

/****************************************************************
    Test 11 -- Get all multigroup data--with temperature set.
****************************************************************/

   printf("\n\n  ************************* TEST 11 ******************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--with temperature set\n " );
   printf(" ***************************************************************** \n\n" );

   ierr = ndi2_open(&handle, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi2_set_option(handle, NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi2_set_option(handle, NDI_TEMP, "300" );
   ierr = ndi2_set_option(handle, NDI_E_AVG, "from_e");

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
     /* Set isotope, no collapsing */
     printf("***********************ISOTOPE=%s***********************\n\n",
	    zaid_list[i] );
     ierr = ndi2_set_isotope(handle, zaid_list[i]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
     ierr = check_all_ndi_get_mgn_functions(handle);
   }  

   ierr = ndi2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );
    

/****************************************************************
    Test 12 -- Get multigroup data using MTs and C values.
****************************************************************/

   printf("\n\n  ************************* TEST 12 ******************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get data for requested isotopes using both MT and C values.\n " );
   printf(" ***************************************************************** \n\n" );

   ierr = ndi2_open(&handle, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi2_set_option(handle, NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi2_set_option(handle, NDI_E_AVG, "from_e");

   
   printf(" ***********C values***********\n\n"); 
   ierr = ndi2_set_option(handle, NDI_REAC_ID, "c" );
   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
     /* Set isotope, no collapsing */
     printf("***********************ISOTOPE=%s***********************\n\n",
	    zaid_list[i] );
     ierr = ndi2_set_isotope(handle, zaid_list[i]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
     ierr = check_ndi_get_mgn_functions_mt( handle, 10, "C" );
     ierr = check_ndi_get_mgn_functions_mt(  handle, 999, "C" );
   }  

   printf(" ***********MT values***********\n\n"); 
   ierr = ndi2_set_option(handle, NDI_REAC_ID, "mt" );
   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
     printf("***********************ISOTOPE=%s***********************\n\n",
	    zaid_list[i] );
     ierr = ndi2_set_isotope(handle, zaid_list[i]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
     ierr = check_ndi_get_mgn_functions_mt(  handle, 2, "MT" );
     ierr = check_ndi_get_mgn_functions_mt(  handle, 999, "MT" );
   }  
   ierr = ndi2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );
    
/****************************************************************
    Test 13 -- Test unknown keywords
****************************************************************/

   printf("\n\n  ************************* TEST 13 ******************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Test using unknown keywords.\n " );
   printf(" ***************************************************************** \n\n" );

   ierr = ndi2_open(&handle, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
   ierr = ndi2_set_option(handle, NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi2_set_option(handle, NDI_E_AVG, "from_e");

   printf("***********************ISOTOPE=%s***********************\n\n",
	  zaid_list[0] );
   ierr = ndi2_set_isotope(handle, zaid_list[0]);
      if (ierr) 
	printf("\n  **Error %d in setting isotope %s.\n", ierr, zaid_list[i] );
   ierr = ndi_unknown_keyword_mgn(handle);
   ierr = ndi2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );


/****************************************************************
    Test 14 -- Test ZAID translation
****************************************************************/

   printf("\n\n  ************************* TEST 14 ******************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Test ZAID translation.\n " );
   printf(" ***************************************************************** \n\n" );

   ierr = ndi2_open(&handle, "");
   if (ierr)
     printf("  **Error %d in NDI init.\n\n", ierr );
  ierr = ndi2_set_option(handle, NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi2_set_option(handle, NDI_LIBRARY, "default" );
   ierr = ndi2_set_option(handle, NDI_E_AVG, "from_e");

   printf("\nLibrary is defaulting:\n\n");

   for ( i = 0; i < ext_num_zaid; i++ ) {
     printf("***********************ISOTOPE=%s***********************\n",
	    extend_zaid_list[i] );
     ierr = ndi2_set_isotope(handle, extend_zaid_list[i]);
     if (ierr) 
       printf("  **Error %d in setting isotope %s.\n\n", ierr, extend_zaid_list[i] );
     ierr = ndi2_get_string_val(  handle, NDI_ZAID, test_zaid, 133 );
     if (ierr) {
       printf("  **Error %d in getting ZAID --  returned value is \'%s\'\n\n",
	      ierr, test_zaid );
     } else {
       printf("Returned ZAID = %s.\n\n", test_zaid );
     }
   }

   ierr = ndi2_set_option(handle, NDI_LIBRARY, "mendf5" );
   printf("\nDefault library set to mendf5:\n\n");
   for ( i = 0; i < ext_num_zaid; i++ ) {
     printf("***********************ISOTOPE=%s***********************\n",
	    extend_zaid_list[i] );
     ierr = ndi2_set_isotope(handle, extend_zaid_list[i]);
     if (ierr) 
       printf("  **Error %d in setting isotope %s.\n\n", ierr, extend_zaid_list[i] );
     ierr = ndi2_get_string_val(  handle, NDI_ZAID, test_zaid, 133 );
     if (ierr) {
       printf("  **Error %d in getting ZAID --  returned value is \'%s\'\n\n",
	      ierr, test_zaid );
     } else {
       printf("Returned ZAID = %s.\n\n", test_zaid );
     }
   }
   printf("\n");
   ierr = ndi2_set_option(handle, NDI_LIBRARY, "mendf6" );
   printf("\nDefault library set to mendf6:\n\n");
   for ( i = 0; i < ext_num_zaid; i++ ) {
     printf("***********************ISOTOPE=%s***********************\n",
	    extend_zaid_list[i] );
     ierr = ndi2_set_isotope(handle, extend_zaid_list[i]);
     if (ierr) 
       printf("  **Error %d in setting isotope %s.\n\n", ierr, extend_zaid_list[i] );
     ierr = ndi2_get_string_val(  handle, NDI_ZAID, test_zaid, 133 );
     if (ierr) {
       printf("  **Error %d in getting ZAID --  returned value is \'%s\'\n\n",
	      ierr, test_zaid );
     } else {
       printf("Returned ZAID = %s.\n\n", test_zaid );
     }
   }
   printf("\n");
				
   ierr = ndi2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );

   return(ierr);
}

/****************************************************************
****************************************************************/

static int check_all_ndi_get_mgn_functions(int handle)

{
   int tmp_err = 0;

   tmp_err = check_ndi_string_val_func_mgn(handle);
   tmp_err = check_ndi_int_val_func_mgn(handle);
   tmp_err = check_ndi_float_val_func_mgn(handle);
   tmp_err = check_ndi_string_val_n_func_mgn(handle);
   tmp_err = check_ndi_int_val_n_func_mgn(handle);
   tmp_err = check_ndi_float_val_n_func_mgn(handle);
   tmp_err = check_ndi_int_vec_func_mgn(handle);
   tmp_err = check_ndi_float_vec_func_mgn(handle);
   tmp_err = check_ndi_int_vec_n_func_mgn(handle);
   tmp_err = check_ndi_float_vec_n_func_mgn(handle);
   tmp_err = check_ndi_float_vec_x_func_mgn(handle);
   tmp_err = check_ndi_sec_part_mgn(handle);
  
   return(tmp_err);
}

/****************************************************************
****************************************************************/

static int check_ndi_get_mgn_functions_mt( int handle, int reac_id, char *reac_type )
{
   int tmp_err = 0;
   int n = 0;
   int num_elements;
   int int_val_num_keys = 3;
   int flt_val_num_keys = 1;
   int int_vec_num_keys = 6;
   int flt_vec_num_keys = 1;
   int int_val;
   ndi_float64 flt64_val = 0.0;
   ndi_float32 flt32_val= 0.0;
   int *int_vec;
   ndi_float64 *flt64_vec;
   ndi_float32 *flt32_vec;
   int int_val_inkey[] = {
      NDI_RPROD_ALL, 
      NDI_RPROD_RMO,
      NDI_RPROD_FEW, 
   };

   char *int_val_str[] = {
      "No. reaction prod--all", 
      "No. reaction prod--rmo", 
      "No. reaction prod--few", 
   };

   int flt_val_inkey[] = {
     NDI_REAC_Q };

   char *flt_val_str[] = {
      "Reaction Q" };

   int int_vec_inkey[] = {
      NDI_RPROD_ID_ALL, 
      NDI_RPROD_MLT_ALL, 
      NDI_RPROD_ID_RMO,
      NDI_RPROD_MLT_RMO, 
      NDI_RPROD_ID_FEW, 
      NDI_RPROD_MLT_FEW };
   
   char *int_vec_str[] = {
      "Reaction prod. ID--all", 
      "Reaction prod. multiplicity--all", 
      "Reaction prod. ID--rmo", 
      "Reaction prod. multiplicity--rmo", 
      "Reaction prod. ID--few", 
      "Reaction prod. multiplicity--few" };

   int flt_vec_inkey[] = {
      NDI_SIG_REAC
   };

   char *flt_vec_str[] = {
     "Reaction cross section"
   };
   
   /* Integer values */
   for ( n = 0; n < int_val_num_keys; n++ ) {
     printf( "%s (%s=%d):", int_val_str[n], reac_type, reac_id );
     tmp_err = ndi2_get_int_val_n(  handle, int_val_inkey[n], reac_id, &int_val );
     if (tmp_err) 
       printf("\n  **Error %d: in %s -- returned value is %d\n", tmp_err, 
	      int_val_str[n], int_val);
     else
       printf( "%d\n", int_val );
   }
   printf("\n");

   /* Float 64 values */
   for ( n = 0; n < flt_val_num_keys; n++ ) {
     printf( "%s (%s=%d):  ", flt_val_str[n], reac_type, reac_id );
     tmp_err = ndi2_get_float64_val_n(  handle, flt_val_inkey[n], reac_id, &flt64_val );
     if (tmp_err) 
       printf("\n  **Error %d: in %s -- returned value is %22.15e\n", tmp_err, 
	      flt_val_str[n], flt64_val);
     else
       printf( "%22.15e\n", flt64_val );
   }
   printf("\n");

   /* Float 32 values */
   for ( n = 0; n < flt_val_num_keys; n++ ) {
     printf( "%s (%s=%d):  ", flt_val_str[n], reac_type, reac_id );
     tmp_err = ndi2_get_float32_val_n(  handle, flt_val_inkey[n], reac_id, &flt32_val );
     if (tmp_err) 
       printf("\n  **Error %d: in %s -- returned value is %13.6e\n", tmp_err, 
	      flt_val_str[n], flt32_val);
     else
       printf( "%13.6e\n", flt32_val );
   }
   printf("\n");

   /* Integer vectors */
   for ( n = 0; n < int_vec_num_keys; n++ ) {
     printf( "%s (%s=%d):  \n", int_vec_str[n], reac_type, reac_id );
     num_elements = ndi2_get_size_n(  handle, int_vec_inkey[n], reac_id, &tmp_err );
     if ( num_elements <= 0 ) {
       num_elements = 30;
       printf("***ERROR GETTING SIZE FOR %s.\n", int_vec_str[n] );
     }
     int_vec = (int *)malloc(num_elements * sizeof(int) );
     tmp_err = ndi2_get_int_vec_n(  handle, int_vec_inkey[n], reac_id, int_vec, num_elements );
     if (tmp_err) {
       printf("  **Error %d: in %s -- returned value is\n", tmp_err, 
	      int_vec_str[n]);
       print_int_vec( int_vec, num_elements );
     }  else {
       print_int_vec( int_vec, num_elements );
     }
     if (int_vec)
       free(int_vec);
   }
   printf("\n");

   /* Float 64 vectors */
   for ( n = 0; n < flt_vec_num_keys; n++ ) {
     printf( "%s (%s=%d):  \n", flt_vec_str[n], reac_type, reac_id );
     num_elements = ndi2_get_size_n(  handle, flt_vec_inkey[n], reac_id, &tmp_err );
     if ( num_elements <= 0 ) {
       num_elements = 30;
       printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_str[n] );
     }
     flt64_vec = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
     tmp_err = ndi2_get_float64_vec_n(  handle, flt_vec_inkey[n], reac_id, flt64_vec, num_elements );
     if (tmp_err) {
       printf("  **Error %d: in %s -- returned value is\n", tmp_err, 
	      flt_val_str[n]);
       print_float64_vec( flt64_vec, num_elements );
     }  else {
       print_float64_vec( flt64_vec, num_elements );
     }
     if (flt64_vec)
       free(flt64_vec);
   }

   /* Float 32 vectors */
   for ( n = 0; n < flt_vec_num_keys; n++ ) {
     printf( "%s (%s=%d):  \n", flt_vec_str[n], reac_type, reac_id );
     num_elements = ndi2_get_size_n(  handle, flt_vec_inkey[n], reac_id, &tmp_err );
     if ( num_elements <= 0 ) {
       num_elements = 30;
       printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_str[n] );
     }
     flt32_vec = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
     tmp_err = ndi2_get_float32_vec_n(  handle, flt_vec_inkey[n], reac_id, flt32_vec, num_elements );
     if (tmp_err) {
       printf("  **Error %d: in %s -- returned value is\n", tmp_err, 
	      flt_val_str[n]);
       print_float32_vec( flt32_vec, num_elements );
     }  else {
       print_float32_vec( flt32_vec, num_elements );
     }
     if (flt32_vec)
       free(flt32_vec);
   }

   printf("\n");
   return(tmp_err);
}

/****************************************************************
****************************************************************/

static int ndi_unknown_keyword_mgn(int handle)
{
   int tmp_err = 0;
   char str_val[50];
   int int_val = 0;
   ndi_float64 flt64_val = 0.0;
   ndi_float32 flt32_val = 0.0;
   int *int_vec;
   ndi_float64 *flt64_vec;
   ndi_float32 *flt32_vec;
   int num_elements; 

   /* String value */
   printf( "String value (NDI_COLLAPSE):  ");
   tmp_err = ndi2_get_string_val(  handle, NDI_COLLAPSE, str_val, 50 );
   if (tmp_err) {
     printf("\n  **Error %d: in string_val (NDI_COLLAPSE) -- returned value is \'%s\'\n", 
	    tmp_err, str_val);
   }  else {
     printf( "\'%s\'\n", str_val);    
   }

   /* Integer value */
   printf( "Integer value (NDI_COLLAPSE):  ");
   tmp_err = ndi2_get_int_val( handle, NDI_COLLAPSE, &int_val );
   if (tmp_err) {
     printf("\n  **Error %d: in int_val (NDI_COLLAPSE) -- returned value is %d\n", 
	    tmp_err, int_val);
   }  else {
     printf( "%d\n", int_val);    
   }

   /* Float 64 value */
   printf( "Float64 value (NDI_COLLAPSE):  ");
   tmp_err = ndi2_get_float64_val( handle, NDI_COLLAPSE, &flt64_val );
   if (tmp_err) {
     printf("\n  **Error %d: in float_val (NDI_COLLAPSE) -- returned value is %22.15e\n", 
	    tmp_err, flt64_val);
   }  else {
     printf( "%22.15e\n", flt64_val);    
   }

   /* Float 32 value */
   printf( "Float32 value (NDI_COLLAPSE):  ");
   tmp_err = ndi2_get_float32_val( handle, NDI_COLLAPSE, &flt32_val );
   if (tmp_err) {
     printf("\n  **Error %d: in float32_val (NDI_COLLAPSE) -- returned value is %13.6e\n", 
	    tmp_err, flt32_val);
   }  else {
     printf( "%13.6e\n", flt32_val);    
   }

   /* String value (n) */
   printf( "String value--0 (NDI_COLLAPSE):  ");
   tmp_err = ndi2_get_string_val_n( handle, NDI_COLLAPSE, 0, str_val, 50 );
   if (tmp_err) {
     printf("\n  **Error %d: in string_val_n (NDI_COLLAPSE) -- returned value is \'%s\'\n", 
	    tmp_err, str_val);
   }  else {
     printf( "%s\n", str_val);    
   }

   /* Integer value (n) */
   printf( "Integer value--0 (NDI_COLLAPSE):  ");
   tmp_err = ndi2_get_int_val_n( handle, NDI_COLLAPSE, 0, &int_val );
   if (tmp_err) {
     printf("\n  **Error %d: in int_val_n (NDI_COLLAPSE) -- returned value is %d\n", 
	    tmp_err, int_val);
   }  else {
     printf( "%d\n", int_val);    
   }

   /* Float 64 value (n) */
   printf( "Float64 value--0 (NDI_COLLAPSE):  ");
   tmp_err = ndi2_get_float64_val_n( handle, NDI_COLLAPSE, 0, &flt64_val );
   if (tmp_err) {
     printf("\n  **Error %d: in float64_val_n (NDI_COLLAPSE) -- returned value is %22.15e\n", 
	    tmp_err, flt64_val);
   }  else {
     printf( "%22.15e\n", flt64_val);    
   }

   /* Float 32 value (n) */
   printf( "Float32 value--0 (NDI_COLLAPSE):  ");
   tmp_err = ndi2_get_float32_val_n( handle, NDI_COLLAPSE, 0, &flt32_val );
   if (tmp_err) {
     printf("\n  **Error %d: in float32_val_n (NDI_COLLAPSE) -- returned value is %13.6e\n", 
	    tmp_err, flt32_val);
   }  else {
     printf( "%13.6e\n", flt32_val);    
   }

   /* Integer vector */
   printf( "Integer vector (NDI_COLLAPSE):  ");
   num_elements = ndi2_get_size( handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (Int vec).\n" );
   }
   int_vec = (int *)malloc( num_elements * sizeof(int) );
   tmp_err = ndi2_get_int_vec( handle, NDI_COLLAPSE, int_vec, num_elements );
   if (tmp_err) {
     printf("\n  **Error %d: in int_vec (NDI_COLLAPSE) -- returned value is %d\n", 
	    tmp_err, num_elements );
     print_int_vec( int_vec, num_elements );
   }  else {
     print_int_vec( int_vec, num_elements );
   }
   if (int_vec)
     free(int_vec);

   /* Float64 vector */
   printf( "Float64 vector (NDI_COLLAPSE):  ");
   num_elements = ndi2_get_size( handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (float64 vec).\n" );     
   }
   flt64_vec = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
   tmp_err = ndi2_get_float64_vec( handle, NDI_COLLAPSE, flt64_vec, num_elements );
   if (tmp_err) {
     printf("\n  **Error %d: in float64_vec (NDI_COLLAPSE) -- returned value is %d\n", 
	    tmp_err, num_elements );
     print_float64_vec( flt64_vec, num_elements );
   }  else {
     print_float64_vec( flt64_vec, num_elements );
   }
   if (flt64_vec)
     free(flt64_vec);

   /* Float32 vector */
   printf( "Float32 vector (NDI_COLLAPSE):  ");
   num_elements = ndi2_get_size( handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (Float32 vec).\n" );     
   }
   flt32_vec = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
   tmp_err = ndi2_get_float32_vec( handle, NDI_COLLAPSE, flt32_vec, num_elements );
   if (tmp_err) {
     printf("\n  **Error %d: in float32_vec (NDI_COLLAPSE) -- returned value is %d\n", 
	    tmp_err, num_elements );
     print_float32_vec( flt32_vec, num_elements );
   }  else {
     print_float32_vec( flt32_vec, num_elements );
   }
   if (flt32_vec)
     free(flt32_vec);

   /* Integer vector (n) */
   printf( "Integer vector--0 (NDI_COLLAPSE):  ");
   num_elements = ndi2_get_size( handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (Int vec n).\n" );
   }
   int_vec = (int *)malloc(num_elements * sizeof(int) );
   tmp_err = ndi2_get_int_vec_n( handle, NDI_COLLAPSE, 0, int_vec, num_elements );
   if (tmp_err) {
     printf("\n  **Error %d: in int_vec_n (NDI_COLLAPSE) -- returned value is %d\n", 
	    tmp_err, num_elements );
     print_int_vec( int_vec, num_elements );
   }  else {
     print_int_vec( int_vec, num_elements );
   }
   if (int_vec)
     free(int_vec);

   /* Float64 vector (n) */
   printf( "Float64 vector--0 (NDI_COLLAPSE):  ");
   num_elements = ndi2_get_size( handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (Float64 vec n).\n" );
   }
   flt64_vec = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
   tmp_err = ndi2_get_float64_vec_n( handle, NDI_COLLAPSE, 0, flt64_vec, num_elements );
   if (tmp_err) {
     printf("\n  **Error %d: in float_vec_n (NDI_COLLAPSE) -- returned value is %d\n", 
	    tmp_err, num_elements );
     print_float64_vec( flt64_vec, num_elements );
   }  else {
     print_float64_vec( flt64_vec, num_elements );
   }
   if (flt64_vec)
     free(flt64_vec);

   /* Float32 vector (n) */
   printf( "Float32 vector--0 (NDI_COLLAPSE):  ");
   num_elements = ndi2_get_size( handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (Float32 vec n).\n" );     
   }
   flt32_vec = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
   tmp_err = ndi2_get_float32_vec_n( handle, NDI_COLLAPSE, 0, flt32_vec, num_elements );
   if (tmp_err) {
     printf("\n  **Error %d: in float_vec_n (NDI_COLLAPSE) -- returned value is %d\n", 
	    tmp_err, num_elements );
     print_float32_vec( flt32_vec, num_elements );
   }  else {
     print_float32_vec( flt32_vec, num_elements );
   }
   if (flt32_vec)
     free(flt32_vec);

   /* Float64 vector (x) */
   printf( "Float64 vector--0 (NDI_COLLAPSE):  ");
   num_elements = ndi2_get_size( handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (Float64 vec x).\n" );
   }
   flt64_vec = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
   tmp_err = ndi2_get_float64_vec_x( handle, NDI_COLLAPSE, "92235", flt64_vec, num_elements );
   if (tmp_err) {
     printf("\n  **Error %d: in float_vec_x -- NDI_COLLAPSE (92235) -- returned value is %d\n", 
	    tmp_err, num_elements );
     print_float64_vec( flt64_vec, num_elements );
   }  else {
     print_float64_vec( flt64_vec, num_elements );
   }
   if (flt64_vec)
     free(flt64_vec);

   /* Float32 vector (x) */
   printf( "Float32 vector--0 (NDI_COLLAPSE):  ");
   num_elements = ndi2_get_size( handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (Float32 vec x).\n" );     
   }
   flt32_vec = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
   tmp_err = ndi2_get_float32_vec_x( handle, NDI_COLLAPSE, "92235", flt32_vec, num_elements );
   if (tmp_err) {
     printf("\n  **Error %d: in float_vec_x -- NDI_COLLAPSE (92235)  -- returned value is %d\n", 
	    tmp_err, num_elements );
     print_float32_vec( flt32_vec, num_elements );
   }  else {
     print_float32_vec( flt32_vec, num_elements );
   }
   if (flt32_vec)
     free(flt32_vec);
  
   printf("\n");
   return(tmp_err);
}

/****************************************************************
****************************************************************/

static int check_ndi_string_val_func_mgn(int handle)

{
   int n = 0;
   int tmp_err = 0;
   char str_val[132 + 1];
   int num_keys = 4;
   int inkey[] = {
     NDI_ZAID,
     NDI_INFO,
     NDI_DEPL_ZAID,
     NDI_DEPL_INFO
   };

   char *str_str[] = {
     "ZAID",
     "Info",
     "Depletion ZAID",
     "Depletion info"
   };

   printf("STRING VALUES:\n");
   for ( n = 0; n < num_keys; n++ ) {
     printf( "%s:  ", str_str[n] );
     tmp_err = ndi2_get_string_val( handle, inkey[n], str_val, 133 );
     if (tmp_err) 
       printf("\n  **Error %d: in %s -- returned value is  \'%s\'\n", tmp_err, str_str[n], str_val);
     else
       printf( "%s\n", str_val );
   }
   printf("\n");
   return(tmp_err);
}

/****************************************************************
****************************************************************/

static int check_ndi_int_val_func_mgn(int handle)

{
   int n;
   int tmp_err = 0;
   int ret_int = 0;
   int num_keys = 10;
   int inkey[] = {
      NDI_NUM_GRPS, 
      NDI_UP_GRPS, 
      NDI_DOWN_GRPS,
      NDI_NUM_REAC, 
      NDI_NUM_GRPS_CURR_PART, 
      NDI_NUM_TARGET,
      NDI_NUM_RPRODS, 
      NDI_PN_ORDER, 
      NDI_NUM_SEC_PARTS,
      NDI_PN_ORDER_CURR_PART };

   char *int_str[] = {
      "No. of groups", 
      "No. of upscatter groups", 
      "No. of downscatter groups", 
      "No. of reactions",
      "No. of photon groups",
      "No. of targets",
      "No. of reaction prod",
      "Pn order -- scattering",
      "No. of secondary particles",
      "Pn order -- photon prod" };

   tmp_err = ndi2_set_option(handle, NDI_CURR_PART, "0" );
   if ( tmp_err ) {
     printf("  ****Error %d setting current particle photon.\n", tmp_err );
   }

   printf("\nINTEGER VALUES:\n");
   for ( n = 0; n < num_keys; n++ ) {
     printf( "%s:  ", int_str[n]);
     tmp_err = ndi2_get_int_val( handle, inkey[n], &ret_int);
     if (tmp_err) 
       printf("\n  **Error %d: in %s -- returned value is %d\n", tmp_err, int_str[n], ret_int);
     else
       printf("%d\n", ret_int);
   }
   printf("\n");
   return(tmp_err);
}

/****************************************************************
****************************************************************/

static int check_ndi_float_val_func_mgn(int handle)

{
   int n;
   int tmp_err = 0;
   ndi_float64 ret_flt64 = 0.0;
   ndi_float32 ret_flt32 = 0.0;
   int num_keys = 4;
   int inkey[] = {
      NDI_TEMP,
      NDI_SIG_0, 
      NDI_AT_WGT,
      NDI_AWR };

   char *flt_val_str[] = {
      "Temperature", 
      "Sigma zero", 
      "Atomic weight", 
      "AWR" };

   printf("\nFLOAT64 VALUES:\n");
   for ( n = 0; n < num_keys; n++ ) {
      printf("%s:  ", flt_val_str[n]);
      tmp_err = ndi2_get_float64_val( handle, inkey[n], &ret_flt64 );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %22.15e\n", tmp_err, flt_val_str[n], ret_flt64 );
      else
	 printf("%22.15e\n", ret_flt64 );
   }

   printf("\nFLOAT32 VALUES:\n");
   for ( n = 0; n < num_keys; n++ ) {
      printf("%s:  ", flt_val_str[n]);
      tmp_err = ndi2_get_float32_val( handle, inkey[n], &ret_flt32 );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %13.6e\n", tmp_err, flt_val_str[n], ret_flt32 );
      else
	 printf("%13.6e\n", ret_flt32 );
   }
   printf("\n");
   return(tmp_err);
}

/****************************************************************
****************************************************************/

static int check_ndi_string_val_n_func_mgn(int handle)

{
   int n;
   int tmp_err = 0;
   char ret_str[133] = "";
   int num_keys = 4;
   int inkey[] = {
      NDI_SEC_PART_TYPES,
      NDI_SEC_PART_ZAIDS,
      NDI_TARGET,
      NDI_RPRODS_ID };

   char *int_val_n_str[] = {
      "Sec. particle type",
      "Sec. particle zaid", 
      "Target", 
      "Reaction prod. ID" };

   printf("\nSTRING VALUES (N):\n");

   /* First retrieve number of reaction products */
   for (n = 0; n < num_keys; n++) {
      /* Index = 0 */
      printf( "%s (0):  ", int_val_n_str[n]);
      tmp_err = ndi2_get_string_val_n( handle, inkey[n], 0, ret_str, 133 );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is \'%s\'.\n", tmp_err, 
		int_val_n_str[n], ret_str );
      else
	 printf("%s\n", ret_str);
      /* Index = 999 */
      printf( "%s (999):  ", int_val_n_str[n]);
      tmp_err = ndi2_get_string_val_n( handle, inkey[n], 999, ret_str, 133 );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is \'%s\'.\n", tmp_err, 
		int_val_n_str[n], ret_str );
      else
	 printf("%s\n", ret_str);
   }
   
   printf("\n");
   return(tmp_err);
}
  

/****************************************************************
****************************************************************/

static int check_ndi_int_val_n_func_mgn(int handle)

{
   int n;
   int tmp_err = 0;
   int ret_int = 0;
   int num_keys = 4;
   int inkey[] = {
      NDI_RPROD_ALL, 
      NDI_RPROD_RMO,
      NDI_RPROD_FEW, 
      NDI_REAC_MT };

   char *int_val_n_str[] = {
      "No. reaction prod--all", 
      "No. reaction prod--rmo", 
      "No. reaction prod--few", 
      "Reaction MT" };

   printf("\nINTEGER VALUES (N):\n");

   /* First retrieve number of reaction products */
   for (n = 0; n < (num_keys - 1 ); n++) {
      /* All should contain MT=2 */
      printf( "%s (MT=2):  ", int_val_n_str[n]);
      tmp_err = ndi2_get_int_val_n( handle, inkey[n], 2, &ret_int );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %d.\n", tmp_err, 
		int_val_n_str[n], ret_int );
      else
	 printf("%d\n", ret_int);
      /* MT = 16 */
      printf( "%s (MT=16):  ", int_val_n_str[n]);
      tmp_err = ndi2_get_int_val_n( handle, inkey[n], 16, &ret_int );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %d.\n", tmp_err, 
		int_val_n_str[n], ret_int );
      else
	 printf("%d\n", ret_int);
      /* MT = 9999 */
      printf( "%s (MT=9999):  ", int_val_n_str[n]);
      tmp_err = ndi2_get_int_val_n( handle, inkey[n], 9999, &ret_int );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %d.\n", tmp_err, 
		int_val_n_str[n], ret_int );
      else
	 printf("%d\n", ret_int);
   }

   /* Get 1st reaction MT */
   printf( "%s (0):  ", int_val_n_str[num_keys-1]);
   tmp_err = ndi2_get_int_val_n( handle, inkey[num_keys-1], 0, &ret_int );
   if (tmp_err)
      printf("\n  **Error %d in %s -- returned value is %d.\n", tmp_err, 
	     int_val_n_str[num_keys-1], ret_int );
   else
      printf("%d\n", ret_int);
   /* Get 999th reaction MT */
   printf( "%s (999):  ", int_val_n_str[num_keys-1]);
   tmp_err = ndi2_get_int_val_n( handle, inkey[num_keys-1], 999, &ret_int );
   if (tmp_err)
      printf("\n  **Error %d in %s -- returned value is %d.\n", tmp_err, 
	     int_val_n_str[num_keys-1], ret_int );
   else
      printf("%d\n", ret_int);
   
   printf("\n");
   return(tmp_err);
}
  
  
/****************************************************************
****************************************************************/

static int check_ndi_float_val_n_func_mgn(int handle)

{
   int n;
   int tmp_err = 0;
   ndi_float64 ret_flt64 = 0.0;
   ndi_float32 ret_flt32 = 0.0;
   int num_keys = 1;
   int inkey[] = {
      NDI_REAC_Q };

   char *flt_val_str[] = {
      "Reaction Q" };

   printf("\nFLOAT64 VALUES (N):\n");

   for ( n = 0; n < num_keys; n++ ) {
      /* MT = 2; All with reaction data should have this */
      printf( "%s (MT=2):  ", flt_val_str[n] );
      tmp_err = ndi2_get_float64_val_n( handle, inkey[n], 2, &ret_flt64 );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %22.15e.\n", tmp_err, 
		flt_val_str[n], ret_flt64 );
      else
	 printf("%22.15e\n", ret_flt64);
      /* MT = 999 */
      printf( "%s (MT=999):  ", flt_val_str[n] );
      tmp_err = ndi2_get_float64_val_n( handle, inkey[n], 999, &ret_flt64 );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %22.15e.\n", tmp_err, 
		flt_val_str[n], ret_flt64 );
      else
	 printf("%22.15e\n", ret_flt64);
      printf("\n");
   }
   printf("\n");
   printf("FLOAT32 VALUES (N):\n");

   for ( n = 0; n < num_keys; n++ ) {
      /* MT = 2; All with reaction data should have this */
      printf( "%s (MT=2):  ", flt_val_str[n] );
      tmp_err = ndi2_get_float32_val_n( handle, inkey[n], 2, &ret_flt32 );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %13.6e.\n", tmp_err, 
		flt_val_str[n], ret_flt32 );
      else
	 printf( "%13.6e\n", ret_flt32 );
      /* MT = 999 */
      printf( "%s (MT=999):  ", flt_val_str[n] );
      tmp_err = ndi2_get_float32_val_n( handle, inkey[n], 999, &ret_flt32 );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %13.6e.\n", tmp_err, 
		flt_val_str[n], ret_flt32 );
      else
	 printf( "%13.6e\n", ret_flt32 );
      printf("\n");
   }
   printf("\n");

   return(tmp_err);
}


  
/****************************************************************
****************************************************************/

static int check_ndi_int_vec_func_mgn(int handle)

{
   int n;
   int tmp_err = 0;
   int *ret_vec;
   int num_keys = 4;
   int num_elements = 0;
   int inkey[] = {
      NDI_RPROD_ALL, 
      NDI_RPROD_RMO,
      NDI_RPROD_FEW, 
      NDI_REAC_MT };

   char *int_vec_str[] = {
      "No. reaction prod--all", 
      "No. reaction prod--rmo", 
      "No. reaction prod--few", 
      "Reaction MT" };


   printf("\nINTEGER VECTORS:\n");

   /* First retrieve number of reaction products */
   for (n = 0; n < num_keys; n++) {
      printf( "%s:  ", int_vec_str[n]);
      num_elements = ndi2_get_size( handle, inkey[n], &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s.\n", int_vec_str[n] );
      }
      ret_vec = (int *)malloc(num_elements * sizeof(int) );
      tmp_err = ndi2_get_int_vec( handle, inkey[n], ret_vec, num_elements );
      if (tmp_err) {
	 printf("\n  **Error %d in %s -- returned value is:\n", tmp_err, 
		int_vec_str[n] );
	 print_int_vec( ret_vec, num_elements );
      } else {
	 print_int_vec( ret_vec, num_elements );	 
      }
      if (ret_vec)
	free(ret_vec);
   }
   
   printf("\n");
   return(tmp_err);
   
}

    
/****************************************************************
****************************************************************/

static int check_ndi_float_vec_func_mgn(int handle)

{
   int n;
   int tmp_err = 0;
   ndi_float64 *ret_vec64;
   ndi_float32 *ret_vec32;
   int num_keys = 25;
   int num_elements = 0;
   int inkey[] = {
      NDI_E_BOUNDS, 
      NDI_WGTS,
      NDI_VEL,
      NDI_E_AVG,
      NDI_ENERGY_N,
      NDI_ENERGY_AVAIL,
      NDI_HEATING,
      NDI_KERMA,
      NDI_SIG_TOT,
      NDI_NU_TOT,
      NDI_NU_PR,
      NDI_NU_DEL,
      NDI_NU_SIG_F_TOT,
      NDI_NU_SIG_F_PR,
      NDI_NU_SIG_F_DEL,
      NDI_CHI_VEC_TOT,
      NDI_CHI_VEC_PR, 
      NDI_CHI_VEC_DEL,
      NDI_CHI_MAT_TOT,
      NDI_CHI_MAT_PR,
      NDI_CHI_MAT_DEL,
      NDI_NORM,
      NDI_FISS_Q, 
      NDI_REAC_Q, 
      NDI_E_BOUNDS_CURR_PART };

   char *flt_vec_str[] = {
      "Energy boundaries", 
      "Weights (fluxes)", 
      "Velocities", 
      "Average energy",
      "Energy n",
      "Energy available",
      "Heating", 
      "Kerma", 
      "Total cross section", 
      "Nubar--total",
      "Nubar--prompt",
      "Nubar--delayed",
      "Nubar*SigmaF--total", 
      "Nubar*SigmaF--prompt", 
      "Nubar*SigmaF--delayed", 
      "Chi vector--total",
      "Chi vector--prompt",
      "Chi vector--delayed",
      "Chi matrix--total",
      "Chi matrix--prompt",
      "Chi matrix--delayed",
      "Normalization factors",
      "Fission Q values",
      "Reaction Q values",
      "Photon energy bounds" };


   tmp_err = ndi2_set_option(handle, NDI_CURR_PART, "0" );
   if ( tmp_err ) {
     printf("  ****Error %d setting current particle photon.\n", tmp_err );
   }

   printf("\nFLOAT64 VECTORS:\n");

   for ( n = 0; n < num_keys; n++ ) {
      printf( "%s: \n", flt_vec_str[n] );
      num_elements = ndi2_get_size( handle, inkey[n], &tmp_err );  
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_str[n] );
      }
      ret_vec64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
      tmp_err = ndi2_get_float64_vec( handle, inkey[n], ret_vec64, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s -- returned value is:\n", tmp_err, 
		flt_vec_str[n] );
	 print_float64_vec( ret_vec64, num_elements );
      } else {
	 print_float64_vec( ret_vec64, num_elements );	 
      }
      if (ret_vec64)
	free(ret_vec64);
   }
   printf("\n");

   printf("\nFLOAT32 VECTORS:\n");
   for ( n = 0; n < num_keys; n++ ) {
      printf( "%s: \n", flt_vec_str[n] );
      num_elements = ndi2_get_size( handle, inkey[n], &tmp_err );  
      if ( num_elements <= 0 ) {
	num_elements = 30;
       printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_str[n] );
      }
      ret_vec32 = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
      tmp_err = ndi2_get_float32_vec( handle, inkey[n], ret_vec32, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s -- returned value is:\n", tmp_err, 
		flt_vec_str[n] );
	 print_float32_vec( ret_vec32, num_elements );
      } else {
	 print_float32_vec( ret_vec32, num_elements );	 
      }
      if (ret_vec32)
	free(ret_vec32);
   }
   printf("\n");
   return(tmp_err);
}



/****************************************************************
****************************************************************/

static int check_ndi_int_vec_n_func_mgn(int handle)

{
   int n;
   int tmp_err = 0;
   int *id, *mult;
   int num_keys = 6;
   int num_elements = 0;
   int inkey[] = {
      NDI_RPROD_ID_ALL, 
      NDI_RPROD_MLT_ALL, 
      NDI_RPROD_ID_RMO,
      NDI_RPROD_MLT_RMO, 
      NDI_RPROD_ID_FEW, 
      NDI_RPROD_MLT_FEW };

   char *int_vec_n_str[] = {
      "Reaction prod. ID--all", 
      "Reaction prod. multiplicity--all", 
      "Reaction prod. ID--rmo", 
      "Reaction prod. multiplicity--rmo", 
      "Reaction prod. ID--few", 
      "Reaction prod. multiplicity--few" };


   printf("\nINTEGER VECTORS (N):\n");

      for ( n = 0; n < num_keys ; n += 2 ) {
	 /* MT = 2 */
	 printf( "%s (MT=2): \n", int_vec_n_str[n] );
	 num_elements = ndi2_get_size_n( handle, inkey[n], 2, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s id.\n", int_vec_n_str[n] );
	 }
	 id = (int *)malloc(num_elements * sizeof(int) );
	 tmp_err = ndi2_get_int_vec_n( handle, inkey[n], 2, id, num_elements );
	 if (tmp_err) {
	    printf("  **Error %d in %s--returned value is\n", tmp_err, int_vec_n_str[n]);
	    print_int_vec( id, num_elements ); 
	 } else {
	    print_int_vec( id, num_elements ); 
	 }
	 printf( "%s (MT=2): \n", int_vec_n_str[n+1] );
	 num_elements = ndi2_get_size_n( handle, inkey[n+1], 2, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s.\n", int_vec_n_str[n+1] );
	 }
	 mult = (int *)malloc(num_elements * sizeof(int) );
	 tmp_err = ndi2_get_int_vec_n( handle, inkey[n+1], 2, mult, num_elements );
	 if (tmp_err) {
	    printf("  **Error %d in %s--returned value is\n", tmp_err, int_vec_n_str[n+1]);
	    print_int_vec( mult, num_elements ); 
	 } else {
	    print_int_vec( mult, num_elements ); 
	 }
	 if (id)
	   free(id);

	 if (mult)
	   free(mult);

	 /* MT = 999 */
	 printf( "%s (MT=999): \n", int_vec_n_str[n] );
	 num_elements = ndi2_get_size_n( handle, inkey[n], 999, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s id.\n", int_vec_n_str[n] );
	 }
	 id = (int *)malloc(num_elements * sizeof(int) );
	 tmp_err = ndi2_get_int_vec_n( handle, inkey[n], 999, id, num_elements );
	 if (tmp_err) {
	    printf("  **Error %d in %s--returned value is\n", tmp_err, int_vec_n_str[n]);
	    print_int_vec( id, num_elements ); 
	 } else {
	    print_int_vec( id, num_elements ); 
	 }
	 printf( "%s (MT=999): \n", int_vec_n_str[n+1] );
	 num_elements = ndi2_get_size_n( handle, inkey[n+1], 999, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s.\n", int_vec_n_str[n + 1] );
	 }
	 mult = (int *)malloc(num_elements * sizeof(int) );
	 tmp_err = ndi2_get_int_vec_n( handle, inkey[n+1], 999, mult, num_elements );
	 if (tmp_err) {
	    printf("  **Error %d in %s--returned value is\n", tmp_err, int_vec_n_str[n+1]);
	    print_int_vec( mult, num_elements ); 
	 } else {
	    print_int_vec( mult, num_elements ); 
	 }
	 if (id)
	   free(id);
	 if (mult)
	   free(mult);
      } 
      printf("\n");  
      return(tmp_err);
}


/****************************************************************
****************************************************************/

static int check_ndi_float_vec_n_func_mgn(int handle)

{
   int n;
   int tmp_err = 0;
   ndi_float64 *ret_flt64;
   ndi_float32 *ret_flt32;
   int num_keys = 8;
   int num_elements = 0;
   int inkey[] = {
      NDI_PN_DOWN_FULL, 
      NDI_PN_FULL, 
      NDI_PN_PROD_FULL_CURR_PART,
      NDI_TRANSFER_MAT_TOT,
      NDI_TRANSFER_MAT_PR,
      NDI_CORR_DIAG,
      NDI_CORR_BHS,
      NDI_SIG_REAC
   };

   char *flt_vec_n_str[] = {
      "Scatter matrix--lower diagonal", 
      "Scatter matrix--full", 
      "Photon prod matrix--full",
      "Transfer matrix--total",
      "Transfer matrix--prompt",
      "Transport correction--diagonal",
      "Transport correction--Bell-Hansen-Sandmeier",
      "Reaction cross section"
   };


   tmp_err = ndi2_set_option(handle, NDI_CURR_PART, "0" );
   if ( tmp_err ) {
     printf("  ****Error %d setting current particle photon.\n", tmp_err );
   }
   printf("\nFLOAT64 VECTORS (N):\n");

   /* Handle data which require a Pn order as input */
   for ( n = 0; n < (num_keys - 1); n++) {
      /* Pn order = 0 */
      printf("%s (0): \n", flt_vec_n_str[n] );
      num_elements = ndi2_get_size_n( handle, inkey[n], 0, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_n_str[n] );
      }
      ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
      tmp_err = ndi2_get_float64_vec_n( handle, inkey[n], 0, ret_flt64, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[n]);
	 print_float64_vec( ret_flt64, num_elements );
      } else {
	 print_float64_vec( ret_flt64, num_elements );
      }
      if (ret_flt64)
	free(ret_flt64);
 
      /* Pn order = 99 */
      printf("%s (99): \n", flt_vec_n_str[n] );
      num_elements = ndi2_get_size_n( handle, inkey[n], 99, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_n_str[n] );
      }
      ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
      tmp_err = ndi2_get_float64_vec_n( handle, inkey[n], 99, ret_flt64, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[n] );
	 print_float64_vec( ret_flt64, num_elements );
      } else {
	 print_float64_vec( ret_flt64, num_elements );
      }
      if (ret_flt64)
	free(ret_flt64);
   }
   
   /* Handle data which requires an MT value as input */
   /* MT = 2 */
   num_elements = ndi2_get_size_n( handle, inkey[num_keys-1], 2, &tmp_err );

   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR %s id.\n", flt_vec_n_str[num_keys-1] );
   }
   ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
   printf("%s (MT=2): \n", flt_vec_n_str[num_keys-1] );
   tmp_err = ndi2_get_float64_vec_n( handle, inkey[num_keys-1], 2, ret_flt64, num_elements );
   if (tmp_err) {
      printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[num_keys-1] );
      print_float64_vec( ret_flt64, num_elements );
   } else {
      print_float64_vec( ret_flt64, num_elements );
   }
   if (ret_flt64)
     free(ret_flt64);

   /* MT = 999 */
   num_elements = ndi2_get_size_n( handle, inkey[num_keys-1], 999, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR %s id.\n", flt_vec_n_str[num_keys-1] );
   }
   ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );   
   printf("%s (MT=999): \n", flt_vec_n_str[num_keys-1] );
   tmp_err = ndi2_get_float64_vec_n( handle, inkey[num_keys-1], 999, ret_flt64, num_elements );
   if (tmp_err) {
      printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[num_keys-1] );
      print_float64_vec( ret_flt64, num_elements );
   } else {
      print_float64_vec( ret_flt64, num_elements );
   }
   if (ret_flt64)
     free(ret_flt64);
   printf("\n");

   printf("\nFLOAT32 VECTORS (N):\n");
   /* Handle data which require a Pn order as input */
   for ( n = 0; n < (num_keys - 1); n++) {
      /* Pn order = 0 */
      printf("%s (0): \n", flt_vec_n_str[n] );
      num_elements = ndi2_get_size_n( handle, inkey[n], 0, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s id.\n", flt_vec_n_str[n] );
      }
      ret_flt32 = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
      tmp_err = ndi2_get_float32_vec_n( handle, inkey[n], 0, ret_flt32, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[n]);
	 print_float32_vec( ret_flt32, num_elements );
      } else {
	 print_float32_vec( ret_flt32, num_elements );
      }
      if (ret_flt32)
	free(ret_flt32);

      /* Pn order = 99 */
      printf("%s (99): \n", flt_vec_n_str[n] );
      num_elements = ndi2_get_size_n( handle, inkey[n], 99, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s id.\n", flt_vec_n_str[n] );
      }
      ret_flt32 = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
      tmp_err = ndi2_get_float32_vec_n( handle, inkey[n], 99, ret_flt32, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[n] );
	 print_float32_vec( ret_flt32, num_elements );
      } else {
	 print_float32_vec( ret_flt32, num_elements );
      }
      if (ret_flt32)
	free(ret_flt32);
   }
   
   /* Handle data which requires an MT value as input */
   /* MT = 2 */
   num_elements = ndi2_get_size_n( handle, inkey[num_keys-1], 0, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR %s id.\n", flt_vec_n_str[num_keys-1] );
   }
   ret_flt32 = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
   printf("%s (MT=2): \n", flt_vec_n_str[num_keys-1] );
   tmp_err = ndi2_get_float32_vec_n( handle, inkey[num_keys-1], 2, ret_flt32, num_elements );
   if (tmp_err) {
      printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[num_keys-1] );
      print_float32_vec( ret_flt32, num_elements );
   } else {
      print_float32_vec( ret_flt32, num_elements );
   }
   if (ret_flt32)
     free(ret_flt32);

   /* MT = 999 */
   num_elements = ndi2_get_size_n( handle, inkey[num_keys-1], 999, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR %s id.\n", flt_vec_n_str[num_keys-1] );
   }
   ret_flt32 = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
   printf("%s (MT=999): \n", flt_vec_n_str[num_keys-1] );
   tmp_err = ndi2_get_float32_vec_n( handle, inkey[num_keys-1], 999, ret_flt32, num_elements );
   if (tmp_err) {
      printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[num_keys-1] );
      print_float32_vec( ret_flt32, num_elements );
   } else {
      print_float32_vec( ret_flt32, num_elements );
   }
   if (ret_flt32)
     free(ret_flt32);
   printf("\n");
   return(tmp_err);

}


/****************************************************************
****************************************************************/

static int check_ndi_sec_part_mgn(int handle)

{
   char part_type[133] = "";
   char part_zaid[133] = "";
   int i, j;
   int index_start = 0;
   int tmp_err = 0;
   int ret_int = 0;
   ndi_float64 *ret_flt64;
   ndi_float32 *ret_flt32;
   int num_elements = 0;
   int num_sec_parts = 0;

   int num_int_val = 2;
   int num_float_vec = 3;
   int num_float_vec_n = 1;
   int inkey[] = {
     NDI_NUM_GRPS_CURR_PART,
     NDI_PN_ORDER_CURR_PART,
     NDI_E_BOUNDS_CURR_PART,
     NDI_HEATING_CURR_PART,
     NDI_KERMA_CURR_PART,
     NDI_PN_PROD_FULL_CURR_PART
   };

   char *sec_part_str[] = {
      "Number of energy groups", 
      "Maximum PN order",
      "Energy group boundaries",
      "Partial Heating",
      "Partial KERMA",
      "Full production matrix"
   };


   printf("SECONDARY PARTICLE DATA:\n");

   /* FOR EACH PARTICLE: */
   tmp_err = ndi2_get_int_val( handle, NDI_NUM_SEC_PARTS, &num_sec_parts );
   if ( num_sec_parts <= 0 ) {
     printf("   NO SECONDARY PARTICLE DATA.\n");
   }
   for ( i = 0; i < num_sec_parts; i++ ) {

     /* GET PARTICLE TYPE AND ZAID */
     tmp_err = ndi2_get_string_val_n( handle, NDI_SEC_PART_TYPES, i, part_type, 133 );
     tmp_err = ndi2_get_string_val_n( handle, NDI_SEC_PART_ZAIDS, i, part_zaid, 133 );
     tmp_err = ndi2_set_option(handle, NDI_CURR_PART, part_type );
     if (tmp_err) 
       printf("\n  **Error %d: in setting particle %s.\n", tmp_err, part_type);
     else
       printf("%d\n", ret_int);
     printf( "  PARTICLE %s (%s):\n", part_type, part_zaid );

     /* FIND INT_VAL INFORMATION */
     for ( j = index_start; j < index_start + num_int_val; j++ ) {
       printf( "  %s:  ", sec_part_str[j]);
       tmp_err = ndi2_get_int_val( handle, inkey[j], &ret_int);
       if (tmp_err) 
	 printf("\n  **Error %d: in %s -- returned value is %d\n", tmp_err, sec_part_str[j], ret_int);
       else
	 printf("  %d\n", ret_int);
     }
     index_start += num_int_val;
     printf("\n");

     /* FIND FLOAT_VEC INFORMATION */
     for ( j = index_start; j < index_start + num_float_vec; j++ ) {
      printf( "  %s (64-bit): \n", sec_part_str[j] );
      num_elements = ndi2_get_size( handle, inkey[j], &tmp_err );  
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s id.\n", sec_part_str[j] );
      }
      ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
      tmp_err = ndi2_get_float64_vec( handle, inkey[j], ret_flt64, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s -- returned value is:\n", tmp_err, 
		sec_part_str[j] );
	 print_float64_vec( ret_flt64, num_elements );
      } else {
	 print_float64_vec( ret_flt64, num_elements );	 
      }
      if (ret_flt64)
	free(ret_flt64);
     }
     for ( j = index_start; j < index_start + num_float_vec; j++ ) {
      printf( "  %s (32-bit): \n", sec_part_str[j] );
      num_elements = ndi2_get_size( handle, inkey[j], &tmp_err );  
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s id.\n", sec_part_str[j] );
      }
      ret_flt32 = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
      tmp_err = ndi2_get_float32_vec( handle, inkey[j], ret_flt32, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s -- returned value is:\n", tmp_err, 
		sec_part_str[j] );
	 print_float32_vec( ret_flt32, num_elements );
      } else {
	 print_float32_vec( ret_flt32, num_elements );	 
      }
      if (ret_flt32)
	free(ret_flt32);
     }
     index_start += num_float_vec;
     printf("\n");


     /* FIND FLOAT_VEC_N INFORMATION */
     for ( j = index_start; j < index_start + num_float_vec_n; j++ ) {

       /* Handle data which require a Pn order as input */
       for ( j = index_start; j < index_start + num_float_vec_n; j++ ) {
	 /* Pn order = 0 */
	 printf("  %s Pn=0 (64-bit): \n", sec_part_str[j] );
	 num_elements = ndi2_get_size_n( handle, inkey[j], 0, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s id.\n", sec_part_str[j] );
	 }
	 ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) ); 
	 tmp_err = ndi2_get_float64_vec_n( handle, inkey[j], 0, ret_flt64, num_elements );
	 if (tmp_err) {
	   printf("  **Error %d in %s--returned value is:\n", tmp_err, sec_part_str[j]);
	   print_float64_vec( ret_flt64, num_elements );
	 } else {
	   print_float64_vec( ret_flt64, num_elements );
	 }
	 if (ret_flt64)
	   free(ret_flt64);
 
	 /* Pn order = 99 */
	 printf("  %s Pn=99 (64-bit): \n", sec_part_str[j] );
	 num_elements = ndi2_get_size_n( handle, inkey[j], 99, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s id.\n", sec_part_str[j] );
	 }
	 ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
	 tmp_err = ndi2_get_float64_vec_n( handle, inkey[j], 99, ret_flt64, num_elements );
	 if (tmp_err) {
	   printf("  **Error %d in %s--returned value is:\n", tmp_err, sec_part_str[j] );
	   print_float64_vec( ret_flt64, num_elements );
	 } else {
	   print_float64_vec( ret_flt64, num_elements );
	 }
	 if (ret_flt64)
	   free(ret_flt64);
       }
       printf("\n");

       /* Handle data which require a Pn order as input-- 32-bit  */
       for ( j = index_start; j < index_start + num_float_vec_n; j++ ) {
	 /* Pn order = 0 */
	 printf("  %s Pn=0 (32-bit): \n", sec_part_str[j] );
	 num_elements = ndi2_get_size_n( handle, inkey[j], 0, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s id.\n", sec_part_str[j] );
	 }
	 ret_flt32 = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
	 tmp_err = ndi2_get_float32_vec_n( handle, inkey[j], 0, ret_flt32, num_elements );
	 if (tmp_err) {
	   printf("  **Error %d in %s--returned value is:\n", tmp_err, sec_part_str[j]);
	   print_float32_vec( ret_flt32, num_elements );
	 } else {
	   print_float32_vec( ret_flt32, num_elements );
	 }
	 if (ret_flt32)
	   free(ret_flt32);

	 /* Pn order = 99 */
	 printf("  %s Pn=99 (32-bit): \n", sec_part_str[j] );
	 num_elements = ndi2_get_size_n( handle, inkey[j], 99, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s id.\n", sec_part_str[j]);
	 }
	 ret_flt32 = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
	 tmp_err = ndi2_get_float32_vec_n( handle, inkey[j], 99, ret_flt32, num_elements );
	 if (tmp_err) {
	   printf("  **Error %d in %s--returned value is:\n", tmp_err, sec_part_str[j] );
	   print_float32_vec( ret_flt32, num_elements );
	 } else {
	   print_float32_vec( ret_flt32, num_elements );
	 }
	 if (ret_flt32)
	   free(ret_flt32);
       }
     }
   }
   
   printf("\n");
   return(tmp_err);

}


/****************************************************************
****************************************************************/

static int check_ndi_float_vec_x_func_mgn(int handle)

{
   int n;
   int tmp_err = 0;
   ndi_float64 *ret_flt64;
   ndi_float32 *ret_flt32;
   int num_keys = 1;
   int num_elements = 0;
   int inkey[] = {
       NDI_TOT_PROD_XS
   };
   char tmp_zaid[133] = "";

   char *flt_vec_x_str[] = {
      "Total-production cross section"
   };


   printf("\nFLOAT64 VECTORS (X):\n");

   for ( n = 0; n < num_keys; n++) {
      /* First rprod */
     ndi2_get_string_val( handle, NDI_ZAID, tmp_zaid, 133 );
      printf("%s (%s==>%s): \n", flt_vec_x_str[n], tmp_zaid, "92235" );
      num_elements = ndi2_get_size( handle, inkey[n], &tmp_err ); 
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s id.\n", flt_vec_x_str[n] );
      }
      ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
      tmp_err = ndi2_get_float64_vec_x( handle, inkey[n], "92235", ret_flt64, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_x_str[n]);
	 print_float64_vec( ret_flt64, num_elements );
      } else {
	 print_float64_vec( ret_flt64, num_elements );
      }
      if (ret_flt64)
	free(ret_flt64);

      /* Pn order = 99 */
      printf("%s (99): \n", flt_vec_x_str[n] );
      num_elements = ndi2_get_size( handle, inkey[n], &tmp_err ); 
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s id.\n", flt_vec_x_str[n] );
      }
      ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
      tmp_err = ndi2_get_float64_vec_x( handle, inkey[n], "92235", ret_flt64, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_x_str[n] );
	 print_float64_vec( ret_flt64, num_elements );
      } else {
	 print_float64_vec( ret_flt64, num_elements );
      }
      if (ret_flt64)
	free(ret_flt64);
   }
   printf("\n");

   printf("\nFLOAT32 VECTORS (X):\n");
   /* Handle data which require a Pn order as input */
   for ( n = 0; n < num_keys; n++) {
      /* Pn order = 0 */
      printf("%s (0): \n", flt_vec_x_str[n] );
      num_elements = ndi2_get_size(handle, inkey[n], &tmp_err ); 
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s id.\n", flt_vec_x_str[n] );
      }
      ret_flt32 = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
      tmp_err = ndi2_get_float32_vec_x(handle, inkey[n], "92335", ret_flt32, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_x_str[n]);
	 print_float32_vec( ret_flt32, num_elements );
      } else {
	 print_float32_vec( ret_flt32, num_elements );
      }
      if (ret_flt32)
	free(ret_flt32);

      /* Pn order = 99 */
      printf("%s (99): \n", flt_vec_x_str[n] );
      num_elements = ndi2_get_size_n( handle, inkey[n], 99, &tmp_err ); 
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s id.\n", flt_vec_x_str[n] );
      }
      ret_flt32 = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
      tmp_err = ndi2_get_float32_vec_x( handle, inkey[n], "92235", ret_flt32, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_x_str[n] );
	 print_float32_vec( ret_flt32, num_elements );
      } else {
	 print_float32_vec( ret_flt32, num_elements );
      }
      if (ret_flt32)
	free(ret_flt32);
   }
   printf("\n");
   return(tmp_err);

}

/**********************************************************************/

static void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 4 ) == 0) )
	 printf("\n");
      printf("%22.15e  ", flt64_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/
/**********************************************************************/

static void print_float32_vec( ndi_float32 *flt_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 5 ) == 0) )
	 printf("\n");
      printf("%13.6e   ", flt_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/

static void print_int_vec( int *int_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( (n % 10) == 0) && (n != 0) )
	 printf("\n");
      printf("%d   ", int_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/
