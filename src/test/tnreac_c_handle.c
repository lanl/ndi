#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"


int test_tnreac_c(void);
static int check_all_ndi_get_tn_functions(int handle);
static int check_ndi_get_tn_functions_mt( int handle, int reac_id, char *reac_type );
static int ndi_unknown_keyword_tn(int handle);
static int check_ndi_string_val_func_tn(int handle);
static int check_ndi_int_val_func_tn(int handle);
static int check_ndi_float_val_func_tn(int handle);
static int check_ndi_string_val_n_func_tn(int handle);
static int check_ndi_int_val_n_func_tn(int handle);
static int check_ndi_float_val_n_func_tn(int handle);
static int check_ndi_int_vec_func_tn(int handle);
static int check_ndi_float_vec_func_tn(int handle);
static int check_ndi_int_vec_n_func_tn(int handle);
static int check_ndi_float_vec_n_func_tn(int handle);
static void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );
static void print_float32_vec( ndi_float32 *flt_ptr, int num_elements );
static void print_int_vec( int *int_ptr, int num_elements );

#ifdef __STANDALONE
int main (void)
{
  int ierr = 0;

  ierr = test_tnreac_c();
  return(ierr);
}
#endif  /* __STANDALONE */

int test_tnreac_c(void)
{
   int ierr;
   int i;
   int num_zaid = 2;
   int handle;


   char *zaid_list[] = { 
     "testlib_tn_list",
     "li_list"
   };


  printf("\n\n  NDI TEST SUITE -- test_tnreac_c \n\n");

/****************************************************************
    Test 1 -- Not initialized.
****************************************************************/
   printf("  ************************* TEST 1 ************************* \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Try getting data when NDI is not initialized.\n " );
   printf(" ********************************************************** \n\n" );
   
   /*  Without setting isotope */

   ierr = check_all_ndi_get_tn_functions(handle);

   /* Set isotope */
   printf("***********************REACTION=%s***********************\n\n",
	  zaid_list[0] );
   ierr = NDI2_set_isotope(handle, zaid_list[0]);
   ierr = check_all_ndi_get_tn_functions(handle);
   ierr = NDI2_close(handle);

/****************************************************************
    Test 2 -- Initialized, but lib_type and isotope are not set.
****************************************************************/

   printf("\n\n  ************************* TEST 2 ************************* \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Lib_type and isotope are not set.\n " );
   printf(" ********************************************************** \n\n" );
   
   ierr = NDI2_init(&handle);
   ierr = check_all_ndi_get_tn_functions(handle);
   ierr = NDI2_close(handle);


/****************************************************************
    Test 3 -- Initialized and lib_type set, but isotope is not set.
****************************************************************/

   printf("\n\n  ************************* TEST 3 ************************* \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Isotope is not set.\n " );
   printf(" ********************************************************** \n\n" );
   
   ierr = NDI2_init(&handle);
   ierr = NDI2_set_option( handle, NDI_LIB_TYPE, "tnreactions" );
   ierr = check_all_ndi_get_tn_functions(handle);
   ierr = NDI2_close(handle);

/****************************************************************
    Test 4 -- Get all multigroup data--no special options.
****************************************************************/

   printf("\n\n  ************************* TEST 4 ************************* \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--no collapsing\n " );
   printf(" ********************************************************** \n\n" );

   ierr = NDI2_init(&handle);
   ierr = NDI2_set_option( handle, NDI_LIB_TYPE, "tnreactions" );

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
      /* Set isotope, no collapsing */
     printf("***********************REACTION=%s***********************\n\n",
	    zaid_list[i] );
      ierr = NDI2_set_isotope(handle, zaid_list[i]);
      if ( ierr != 0 ) {
	printf("  **Error %d setting isotope %s\n", ierr, zaid_list[i] );
      }
      ierr = check_all_ndi_get_tn_functions(handle);
   }  
   ierr = NDI2_close(handle);

/****************************************************************
    Test 5 -- Get all multigroup data--collasped to 16_LANL.
****************************************************************/

   printf("\n\n  ************************* TEST 5 *************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--collapse to 16_LANL\n " );
   printf(" ************************************************************ \n\n" );

   ierr = NDI2_init(&handle);
   ierr = NDI2_set_option( handle,NDI_LIB_TYPE, "tnreactions" );
   ierr = NDI2_set_option(handle, NDI_COLLAPSE, "16_group" );

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
     /* Set isotope, no collapsing */
     printf("***********************REACTION=%s***********************\n\n",
	    zaid_list[i] );
     ierr = NDI2_set_isotope(handle, zaid_list[i]);
     ierr = check_all_ndi_get_tn_functions(handle);
   }  

   ierr = NDI2_set_option(handle, NDI_COLLAPSE, "default" );
   ierr = check_all_ndi_get_tn_functions(handle);
   ierr = NDI2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );
    
/****************************************************************
    Test 6 -- Get all multigroup data--set default library.
****************************************************************/

   printf("\n\n  ************************* TEST 6 *************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--set library to testtn\n " );
   printf(" ************************************************************ \n\n" );

   ierr = NDI2_init(&handle);
   ierr = NDI2_set_option(handle, NDI_LIB_TYPE, "tnreactions" );
/*    ierr = NDI2_set_option( NDI_LIBRARY, "testtn2" ); */
   ierr = NDI2_set_option(handle, NDI_LIBRARY, "testlib_tn_list" );

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
      /* Set isotope, no collapsing */
     printf("***********************REACTION=%s***********************\n\n",
	    zaid_list[i] );
     ierr = NDI2_set_isotope(handle, zaid_list[i]);
     ierr = check_all_ndi_get_tn_functions(handle);
   }  
   ierr = NDI2_set_option(handle, NDI_LIBRARY, "default" );
   ierr = check_all_ndi_get_tn_functions(handle);
   ierr = NDI2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );


/****************************************************************
    Test 7 -- Get all multigroup data--set transport corrections.
****************************************************************/

   printf("\n\n  ************************* TEST 7 ********************************* \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--set transport corrections.\n " );
   printf(" ****************************************************************** \n\n" );

   ierr = NDI2_init(&handle);
   ierr = NDI2_set_option(handle, NDI_LIB_TYPE, "tnreactions" );

   printf(" ***********Diagonal transport correction***********\n\n"); 
   ierr = NDI2_set_option(handle, NDI_TRANS_CORR, "diag_n" );
   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
      /* Set isotope, no collapsing */
     printf("***********************REACTION=%s***********************\n\n",
	    zaid_list[i] );
     ierr = NDI2_set_isotope(handle, zaid_list[i]);
     ierr = check_all_ndi_get_tn_functions(handle);
   }  

   printf("\n ***********Bell-Hansen-Sandmeier transport correction***********\n\n"); 
   ierr = NDI2_set_option(handle, NDI_TRANS_CORR, "bhs_n" );
   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
      /* Set isotope, no collapsing */
     printf("***********************REACTION=%s***********************\n\n",
	    zaid_list[i] );
     ierr = NDI2_set_isotope(handle, zaid_list[i]);
     ierr = check_all_ndi_get_tn_functions(handle);
   }  


   printf("\n ***********NO transport correction***********\n\n"); 
   ierr = NDI2_set_option(handle, NDI_TRANS_CORR, "none" );
   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
     
     /* Set isotope, no collapsing */
     printf("***********************REACTION=%s***********************\n\n",
	    zaid_list[i] );
     ierr = NDI2_set_isotope(handle, zaid_list[i]);
     ierr = check_all_ndi_get_tn_functions(handle);
   }  

   ierr = NDI2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );



/****************************************************************
    Test 9 -- Get all multigroup data--with depletion set.
****************************************************************/

   printf("\n\n  ************************* TEST 9 *************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--with depletion set\n " );
   printf(" ************************************************************ \n\n" );

   ierr = NDI2_init(&handle);
   ierr = NDI2_set_option(handle, NDI_LIB_TYPE, "tnreactions" );
   ierr = NDI2_set_option(handle, NDI_DEPLETION, "testlib_depl" );

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
     /* Set isotope, no collapsing */
     printf("***********************REACTION=%s***********************\n\n",
	    zaid_list[i] );
     ierr = NDI2_set_isotope(handle, zaid_list[i]);
     ierr = check_all_ndi_get_tn_functions(handle);
   }  

   ierr = NDI2_set_option(handle, NDI_DEPLETION, "default" );
   ierr = check_all_ndi_get_tn_functions(handle);
   ierr = NDI2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );
    


/****************************************************************
    Test 10 -- Get all multigroup data--with current particle set.
****************************************************************/

   printf("\n\n  ************************* TEST 10 ******************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--with current particle set\n " );
   printf(" ***************************************************************** \n\n" );

   ierr = NDI2_init(&handle);
   ierr = NDI2_set_option(handle, NDI_LIB_TYPE, "tnreactions" );
   ierr = NDI2_set_option(handle, NDI_CURR_PART, "n" );

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
     /* Set isotope, no collapsing */
     printf("***********************REACTION=%s***********************\n\n",
	    zaid_list[i] );
     ierr = NDI2_set_isotope(handle, zaid_list[i]);
     ierr = check_all_ndi_get_tn_functions(handle);
   }  
   
   ierr = NDI2_close(handle);
    if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );
   

/****************************************************************
    Test 11 -- Get all multigroup data--with temperature set.
****************************************************************/

   printf("\n\n  ************************* TEST 11 ******************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get all data for requested isotopes--with temperature set\n " );
   printf(" ***************************************************************** \n\n" );

   ierr = NDI2_init(&handle);
   ierr = NDI2_set_option(handle, NDI_LIB_TYPE, "tnreactions" );
   ierr = NDI2_set_option(handle, NDI_TEMP, "300" );

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
     /* Set isotope, no collapsing */
     printf("***********************REACTION=%s***********************\n\n",
	    zaid_list[i] );
     ierr = NDI2_set_isotope(handle, zaid_list[i]);
     ierr = check_all_ndi_get_tn_functions(handle);
   }  

   ierr = NDI2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );
    

/****************************************************************
    Test 12 -- Get multigroup data using MTs and C values.
****************************************************************/

   printf("\n\n  ************************* TEST 12 ******************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Get data for requested isotopes using both MT and C values.\n " );
   printf(" ***************************************************************** \n\n" );

   ierr = NDI2_init(&handle);
   ierr = NDI2_set_option(handle, NDI_LIB_TYPE, "tnreactions" );

   
   printf(" ***********C values***********\n\n"); 
   ierr = NDI2_set_option(handle, NDI_REAC_ID, "c" );
   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
     /* Set isotope, no collapsing */
     printf("***********************REACTION=%s***********************\n\n",
	    zaid_list[i] );
     ierr = NDI2_set_isotope(handle, zaid_list[i]);
     ierr = check_ndi_get_tn_functions_mt( handle, 10, "C" );
     ierr = check_ndi_get_tn_functions_mt( handle, 999, "C" );
   }  

   printf(" ***********MT values***********\n\n"); 
   ierr = NDI2_set_option(handle, NDI_REAC_ID, "mt" );
   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
     printf("***********************REACTION=%s***********************\n\n",
	    zaid_list[i] );
     ierr = NDI2_set_isotope(handle, zaid_list[i]);
     ierr = check_ndi_get_tn_functions_mt( handle, 2, "MT" );
     ierr = check_ndi_get_tn_functions_mt( handle, 999, "MT" );
   }  
   ierr = NDI2_close(handle);
   if (ierr)
     printf("  **Error %d in closing NDI.\n\n", ierr );
    
/****************************************************************
    Test 13 -- Test unknown keywords
****************************************************************/

   printf("\n\n  ************************* TEST 13 ******************************** \n" );
   printf("    MULTIGROUP NEUTRON DATA TEST\n " );
   printf("    Test using unknown keywords.\n " );
   printf(" ***************************************************************** \n\n" );

   ierr = NDI2_init(&handle);
   ierr = NDI2_set_option(handle, NDI_LIB_TYPE, "tnreactions" );
   printf("***********************REACTION=%s***********************\n\n",
	  zaid_list[0] );
   ierr = NDI2_set_isotope(handle, zaid_list[0]);
   ierr = ndi_unknown_keyword_tn(handle);
   ierr = NDI2_close(handle);

   return(ierr);
}

/****************************************************************
****************************************************************/

static int check_all_ndi_get_tn_functions(int handle)

{
   int tmp_err = 0;

   tmp_err = check_ndi_string_val_func_tn(handle);
   tmp_err = check_ndi_int_val_func_tn(handle);
   tmp_err = check_ndi_float_val_func_tn(handle);
   tmp_err = check_ndi_string_val_n_func_tn(handle);
   tmp_err = check_ndi_int_val_n_func_tn(handle);
   tmp_err = check_ndi_float_val_n_func_tn(handle);
   tmp_err = check_ndi_int_vec_func_tn(handle);
   tmp_err = check_ndi_float_vec_func_tn(handle);
   tmp_err = check_ndi_int_vec_n_func_tn(handle);
   tmp_err = check_ndi_float_vec_n_func_tn(handle);
   
   return(tmp_err);
}

/****************************************************************
****************************************************************/

static int check_ndi_get_tn_functions_mt( int handle, int reac_id, char *reac_type )
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
     tmp_err = NDI2_get_int_val_n(handle, int_val_inkey[n], reac_id, &int_val );
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
     tmp_err = NDI2_get_float64_val_n(handle, flt_val_inkey[n], reac_id, &flt64_val );
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
     tmp_err = NDI2_get_float32_val_n(handle, flt_val_inkey[n], reac_id, &flt32_val );
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
     num_elements = NDI2_get_size_n(handle, int_vec_inkey[n], reac_id, &tmp_err );
     if ( num_elements <= 0 ) {
       num_elements = 30;
       printf("***ERROR GETTING SIZE FOR %s.\n", int_vec_str[n] );
     }
     int_vec = (int *)malloc(num_elements * sizeof(int) );
     tmp_err = NDI2_get_int_vec_n( handle, int_vec_inkey[n], reac_id, int_vec, num_elements );
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
     num_elements = NDI2_get_size_n(handle, flt_vec_inkey[n], reac_id, &tmp_err );
     if ( num_elements <= 0 ) {
       num_elements = 30;
       printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_str[n] );
     }
     flt64_vec = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
     tmp_err = NDI2_get_float64_vec_n(handle, flt_vec_inkey[n], reac_id, flt64_vec, num_elements );
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
     num_elements = NDI2_get_size_n(handle, flt_vec_inkey[n], reac_id, &tmp_err );
     if ( num_elements <= 0 ) {
       num_elements = 30;
       printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_str[n] );
     }
     flt32_vec = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
     tmp_err = NDI2_get_float32_vec_n(handle, flt_vec_inkey[n], reac_id, flt32_vec, num_elements );
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

static int ndi_unknown_keyword_tn(int handle)
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
   tmp_err = NDI2_get_string_val(handle, NDI_COLLAPSE, str_val, 50 );
   if (tmp_err) {
     printf("\n  **Error %d: in string_val (NDI_COLLAPSE) -- returned value is \'%s\'\n", 
	    tmp_err, str_val);
   }  else {
     printf( "\'%s\'\n", str_val);    
   }

   /* Integer value */
   printf( "Integer value (NDI_COLLAPSE):  ");
   tmp_err = NDI2_get_int_val(handle, NDI_COLLAPSE, &int_val );
   if (tmp_err) {
     printf("\n  **Error %d: in int_val (NDI_COLLAPSE) -- returned value is %d\n", 
	    tmp_err, int_val);
   }  else {
     printf( "%d\n", int_val);    
   }

   /* Float 64 value */
   printf( "Float64 value (NDI_COLLAPSE):  ");
   tmp_err = NDI2_get_float64_val(handle, NDI_COLLAPSE, &flt64_val );
   if (tmp_err) {
     printf("\n  **Error %d: in float_val (NDI_COLLAPSE) -- returned value is %22.15e\n", 
	    tmp_err, flt64_val);
   }  else {
     printf( "%22.15e\n", flt64_val);    
   }

   /* Float 32 value */
   printf( "Float32 value (NDI_COLLAPSE):  ");
   tmp_err = NDI2_get_float32_val(handle, NDI_COLLAPSE, &flt32_val );
   if (tmp_err) {
     printf("\n  **Error %d: in float32_val (NDI_COLLAPSE) -- returned value is %13.6e\n", 
	    tmp_err, flt32_val);
   }  else {
     printf( "%13.6e\n", flt32_val);    
   }

   /* String value (n) */
   printf( "String value--0 (NDI_COLLAPSE):  ");
   tmp_err = NDI2_get_string_val_n(handle, NDI_COLLAPSE, 0, str_val, 50 );
   if (tmp_err) {
     printf("\n  **Error %d: in string_val_n (NDI_COLLAPSE) -- returned value is \'%s\'\n", 
	    tmp_err, str_val);
   }  else {
     printf( "%s\n", str_val);    
   }

   /* Integer value (n) */
   printf( "Integer value--0 (NDI_COLLAPSE):  ");
   tmp_err = NDI2_get_int_val_n(handle, NDI_COLLAPSE, 0, &int_val );
   if (tmp_err) {
     printf("\n  **Error %d: in int_val_n (NDI_COLLAPSE) -- returned value is %d\n", 
	    tmp_err, int_val);
   }  else {
     printf( "%d\n", int_val);    
   }

   /* Float 64 value (n) */
   printf( "Float64 value--0 (NDI_COLLAPSE):  ");
   tmp_err = NDI2_get_float64_val_n(handle, NDI_COLLAPSE, 0, &flt64_val );
   if (tmp_err) {
     printf("\n  **Error %d: in float64_val_n (NDI_COLLAPSE) -- returned value is %22.15e\n", 
	    tmp_err, flt64_val);
   }  else {
     printf( "%22.15e\n", flt64_val);    
   }

   /* Float 32 value (n) */
   printf( "Float32 value--0 (NDI_COLLAPSE):  ");
   tmp_err = NDI2_get_float32_val_n(handle, NDI_COLLAPSE, 0, &flt32_val );
   if (tmp_err) {
     printf("\n  **Error %d: in float32_val_n (NDI_COLLAPSE) -- returned value is %13.6e\n", 
	    tmp_err, flt32_val);
   }  else {
     printf( "%13.6e\n", flt32_val);    
   }

   /* Integer vector */
   printf( "Integer vector (NDI_COLLAPSE):  ");
   num_elements = NDI2_get_size(handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (Int vec).\n" );
   }
   int_vec = (int *)malloc( num_elements * sizeof(int) );
   tmp_err = NDI2_get_int_vec(handle, NDI_COLLAPSE, int_vec, num_elements );
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
   num_elements = NDI2_get_size(handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (float64 vec).\n" );     
   }
   flt64_vec = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
   tmp_err = NDI2_get_float64_vec(handle, NDI_COLLAPSE, flt64_vec, num_elements );
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
   num_elements = NDI2_get_size(handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (Float32 vec).\n" );     
   }
   flt32_vec = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
   tmp_err = NDI2_get_float32_vec(handle, NDI_COLLAPSE, flt32_vec, num_elements );
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
   num_elements = NDI2_get_size(handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (Int vec n).\n" );
   }
   int_vec = (int *)malloc(num_elements * sizeof(int) );
   tmp_err = NDI2_get_int_vec_n(handle, NDI_COLLAPSE, 0, int_vec, num_elements );
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
   num_elements = NDI2_get_size(handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (Float64 vec n).\n" );
   }
   flt64_vec = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
   tmp_err = NDI2_get_float64_vec_n(handle, NDI_COLLAPSE, 0, flt64_vec, num_elements );
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
   num_elements = NDI2_get_size(handle, NDI_COLLAPSE, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR Collapse (Float32 vec n).\n" );     
   }
   flt32_vec = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
   tmp_err = NDI2_get_float32_vec_n(handle, NDI_COLLAPSE, 0, flt32_vec, num_elements );
   if (tmp_err) {
     printf("\n  **Error %d: in float_vec_n (NDI_COLLAPSE) -- returned value is %d\n", 
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

static int check_ndi_string_val_func_tn(int handle)

{
   int n = 0;
   int tmp_err = 0;
   char str_val[132 + 1];
   int num_keys =2;
   int inkey[] = {
     NDI_ZAID,
     NDI_INFO
   };

   char *str_str[] = {
     "ZAID",
     "Info"
   };

   printf("STRING VALUES:\n");
   for ( n = 0; n < num_keys; n++ ) {
     printf( "%s:  ", str_str[n] );
     tmp_err = NDI2_get_string_val(handle, inkey[n], str_val, 133 );
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

static int check_ndi_int_val_func_tn(int handle)

{
   int n;
   int tmp_err = 0;
   int ret_int = 0;
   int num_keys = 1;
   int inkey[] = {
      NDI_NUM_REAC
   };

   char *int_str[] = {
      "No. of reactions"
   };

   printf("\nINTEGER VALUES:\n");
   for ( n = 0; n < num_keys; n++ ) {
     printf( "%s:  ", int_str[n]);
     tmp_err = NDI2_get_int_val(handle, inkey[n], &ret_int);
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

static int check_ndi_float_val_func_tn(int handle)

{
   int n;
   int tmp_err = 0;
   ndi_float64 ret_flt64 = 0.0;
   ndi_float32 ret_flt32 = 0.0;
   int num_keys = 0;
   int inkey[] = {
     0
   };

   char *flt_val_str[] = {
     " " 
   };

   printf("\nFLOAT64 VALUES:\n");
   for ( n = 0; n < num_keys; n++ ) {
      printf("%s:  ", flt_val_str[n]);
      tmp_err = NDI2_get_float64_val(handle, inkey[n], &ret_flt64 );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %22.15e\n", tmp_err, flt_val_str[n], ret_flt64 );
      else
	 printf("%22.15e\n", ret_flt64 );
   }

   printf("\nFLOAT32 VALUES:\n");
   for ( n = 0; n < num_keys; n++ ) {
      printf("%s:  ", flt_val_str[n]);
      tmp_err = NDI2_get_float32_val(handle, inkey[n], &ret_flt32 );
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

static int check_ndi_string_val_n_func_tn(int handle)

{
   int n;
   int tmp_err = 0;
   char ret_str[133];
   int num_keys = 1;
   int inkey[] = {
     NDI_LIST_REAC
   };

   char *int_val_n_str[] = {
      "Reaction"
   };

   printf("\nSTRING VALUES (N):\n");

   /* First retrieve number of reaction products */
   for (n = 0; n < num_keys; n++) {
      /* Index = 0 */
      printf( "%s (0):  ", int_val_n_str[n]);
      tmp_err = NDI2_get_string_val_n(handle, inkey[n], 0, ret_str, 133 );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is \'%s\'.\n", tmp_err, 
		int_val_n_str[n], ret_str );
      else
	 printf("%s\n", ret_str);
      /* Index = 999 */
      printf( "%s (999):  ", int_val_n_str[n]);
      tmp_err = NDI2_get_string_val_n(handle, inkey[n], 999, ret_str, 133 );
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

static int check_ndi_int_val_n_func_tn(int handle)

{
   int n;
   int tmp_err = 0;
   int ret_int = 0;
   int num_keys = 2;
   int inkey[] = {
      NDI_NUM_RPRODS, 
      NDI_NUM_REACTANTS, 
   };

   char *int_val_n_str[] = {
      "No. of reaction products", 
      "No. of reactants" 
   };

   printf("\nINTEGER VALUES (N):\n");

   /* First retrieve number of reaction products */
   for (n = 0; n < num_keys; n++) {
      /* All should contain i=2 */
      printf( "%s (i=2):  ", int_val_n_str[n]);
      tmp_err = NDI2_get_int_val_n(handle, inkey[n], 2, &ret_int );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %d.\n", tmp_err, 
		int_val_n_str[n], ret_int );
      else
	 printf("%d\n", ret_int);
      /* i = 16 */
      printf( "%s (i=16):  ", int_val_n_str[n]);
      tmp_err = NDI2_get_int_val_n(handle, inkey[n], 16, &ret_int );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %d.\n", tmp_err, 
		int_val_n_str[n], ret_int );
      else
	 printf("%d\n", ret_int);
      /* i = 9999 */
      printf( "%s (i=9999):  ", int_val_n_str[n]);
      tmp_err = NDI2_get_int_val_n(handle, inkey[n], 9999, &ret_int );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %d.\n", tmp_err, 
		int_val_n_str[n], ret_int );
      else
	 printf("%d\n", ret_int);
   }

   /* Get 1st reaction */
   printf( "%s (0):  ", int_val_n_str[num_keys-1]);
   tmp_err = NDI2_get_int_val_n(handle, inkey[num_keys-1], 0, &ret_int );
   if (tmp_err)
      printf("\n  **Error %d in %s -- returned value is %d.\n", tmp_err, 
	     int_val_n_str[num_keys-1], ret_int );
   else
      printf("%d\n", ret_int);
   /* Get 999th reaction */
   printf( "%s (999):  ", int_val_n_str[num_keys-1]);
   tmp_err = NDI2_get_int_val_n(handle, inkey[num_keys-1], 999, &ret_int );
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

static int check_ndi_float_val_n_func_tn(int handle)

{
   int n;
   int tmp_err = 0;
   ndi_float64 ret_flt64 = 0.0;
   ndi_float32 ret_flt32 = 0.0;
   int num_keys = 0;
   int inkey[] = {
     0
   };

   char *flt_val_str[] = {
     " "
   };

   printf("\nFLOAT64 VALUES (N):\n");

   for ( n = 0; n < num_keys; n++ ) {
      /* i = 2; All with reaction data should have this */
      printf( "%s (i=2):  ", flt_val_str[n] );
      tmp_err = NDI2_get_float64_val_n(handle, inkey[n], 2, &ret_flt64 );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %22.15e.\n", tmp_err, 
		flt_val_str[n], ret_flt64 );
      else
	 printf("%22.15e\n", ret_flt64);
      /* i = 999 */
      printf( "%s (i=999):  ", flt_val_str[n] );
      tmp_err = NDI2_get_float64_val_n(handle, inkey[n], 999, &ret_flt64 );
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
      /* i = 2; All with reaction data should have this */
      printf( "%s (i=2):  ", flt_val_str[n] );
      tmp_err = NDI2_get_float32_val_n(handle, inkey[n], 2, &ret_flt32 );
      if (tmp_err)
	 printf("\n  **Error %d in %s -- returned value is %13.6e.\n", tmp_err, 
		flt_val_str[n], ret_flt32 );
      else
	 printf( "%13.6e\n", ret_flt32 );
      /* i = 999 */
      printf( "%s (i=999):  ", flt_val_str[n] );
      tmp_err = NDI2_get_float32_val_n(handle, inkey[n], 999, &ret_flt32 );
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

static int check_ndi_int_vec_func_tn(int handle)

{
   int n;
   int tmp_err = 0;
   int *ret_vec;
   int num_keys = 0;
   int num_elements = 0;
   int inkey[] = {
     0
   };

   char *int_vec_str[] = {
      " ", 
   };

   printf("\nINTEGER VECTORS:\n");

   /* First retrieve number of reaction products */
   for (n = 0; n < num_keys; n++) {
      printf( "%s:  ", int_vec_str[n]);
      num_elements = NDI2_get_size(handle, inkey[n], &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s.\n", int_vec_str[n] );
      }
      ret_vec = (int *)malloc(num_elements * sizeof(int) );
      tmp_err = NDI2_get_int_vec(handle, inkey[n], ret_vec, num_elements  );
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

static int check_ndi_float_vec_func_tn(int handle)

{
   int n;
   int tmp_err = 0;
   ndi_float64 *ret_vec64;
   ndi_float32 *ret_vec32;
   int num_keys = 0;
   int num_elements = 0;
   int inkey[] = {
     0
   };

   char *flt_vec_str[] = {
     " ", 
   };

   printf("\nFLOAT64 VECTORS:\n");

   for ( n = 0; n < num_keys; n++ ) {
      printf( "%s: \n", flt_vec_str[n] );
      num_elements = NDI2_get_size(handle, inkey[n], &tmp_err );  
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_str[n] );
      }
      ret_vec64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
      tmp_err = NDI2_get_float64_vec(handle, inkey[n], ret_vec64, num_elements );
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
      num_elements = NDI2_get_size(handle, inkey[n], &tmp_err );  
      if ( num_elements <= 0 ) {
	num_elements = 30;
       printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_str[n] );
      }
      ret_vec32 = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
      tmp_err = NDI2_get_float32_vec(handle, inkey[n], ret_vec32, num_elements );
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

static int check_ndi_int_vec_n_func_tn(int handle)

{
   int n;
   int tmp_err = 0;
   int *id, *mult;
   int num_keys = 4;
   int num_elements = 0;
   int inkey[] = {
     NDI_RPRODS_ID,
     NDI_RPRODS_MLT,
     NDI_REACTANTS_ID,
     NDI_REACTANTS_MLT
   };

   char *int_vec_n_str[] = {
     "Reaction product ID",
     "Reaction product multiplicity",
     "Reactant ID",
     "Reactant multiplicity"
   };


   printf("\nINTEGER VECTORS (N):\n");

      for ( n = 0; n < num_keys ; n += 2 ) {
	 /* i = 2 */
	 printf( "%s (i=2): \n", int_vec_n_str[n] );
	 num_elements = NDI2_get_size_n(handle, inkey[n], 2, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s id.\n", int_vec_n_str[n] );
	 }
	 id = (int *)malloc(num_elements * sizeof(int) );
	 tmp_err = NDI2_get_int_vec_n(handle, inkey[n], 2, id, num_elements );
	 if (tmp_err) {
	    printf("  **Error %d in %s--returned value is\n", tmp_err, int_vec_n_str[n]);
	    print_int_vec( id, num_elements ); 
	 } else {
	    print_int_vec( id, num_elements ); 
	 }
	 printf( "%s (i=2): \n", int_vec_n_str[n+1] );
	 num_elements = NDI2_get_size_n(handle, inkey[n+1], 2, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s.\n", int_vec_n_str[n+1] );
	 }
	 mult = (int *)malloc(num_elements * sizeof(int) );
	 tmp_err = NDI2_get_int_vec_n(handle, inkey[n+1], 2, mult, num_elements );
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

	 /* i = 999 */
	 printf( "%s (i=999): \n", int_vec_n_str[n] );
	 num_elements = NDI2_get_size_n(handle, inkey[n], 999, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s id.\n", int_vec_n_str[n] );
	 }
	 id = (int *)malloc(num_elements * sizeof(int) );
	 tmp_err = NDI2_get_int_vec_n( handle, inkey[n], 999, id, num_elements );
	 if (tmp_err) {
	    printf("  **Error %d in %s--returned value is\n", tmp_err, int_vec_n_str[n]);
	    print_int_vec( id, num_elements ); 
	 } else {
	    print_int_vec( id, num_elements ); 
	 }
	 printf( "%s (i=999): \n", int_vec_n_str[n+1] );
	 num_elements = NDI2_get_size_n(handle, inkey[n+1], 999, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s.\n", int_vec_n_str[n + 1] );
	 }
	 mult = (int *)malloc(num_elements * sizeof(int) );
	 tmp_err = NDI2_get_int_vec_n(handle, inkey[n+1], 999, mult, num_elements );
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

static int check_ndi_float_vec_n_func_tn(int handle)

{
   int n;
   int tmp_err = 0;
   ndi_float64 *ret_flt64;
   ndi_float32 *ret_flt32;
   int num_keys = 0;
   int num_elements = 0;
   int inkey[] = {
     0
   };

   char *flt_vec_n_str[] = {
     " "
   };

   printf("\nFLOAT64 VECTORS (N):\n");

   /* Handle data which require a Pn order as input */
   for ( n = 0; n < num_keys; n++) {
      /* Pn order = 0 */
      printf("%s (0): \n", flt_vec_n_str[n] );
      num_elements = NDI2_get_size_n(handle, inkey[n], 0, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_n_str[n] );
      }
      ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
      tmp_err = NDI2_get_float64_vec_n(handle, inkey[n], 0, ret_flt64, num_elements );
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
      num_elements = NDI2_get_size_n(handle, inkey[n], 99, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_n_str[n] );
      }
      ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
      tmp_err = NDI2_get_float64_vec_n( handle, inkey[n], 99, ret_flt64, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[n] );
	 print_float64_vec( ret_flt64, num_elements );
      } else {
	 print_float64_vec( ret_flt64, num_elements );
      }
      if (ret_flt64)
	free(ret_flt64);
   }
   printf("\n");

   printf("\nFLOAT32 VECTORS (N):\n");
   /* Handle data which require a Pn order as input */
   for ( n = 0; n < num_keys; n++) {
      /* Pn order = 0 */
      printf("%s (0): \n", flt_vec_n_str[n] );
      num_elements = NDI2_get_size_n( handle, inkey[n], 0, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s id.\n", flt_vec_n_str[n] );
      }
      ret_flt32 = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
      tmp_err = NDI2_get_float32_vec_n(handle, inkey[n], 0, ret_flt32, num_elements );
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
      num_elements = NDI2_get_size_n(handle, inkey[n], 99, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s id.\n", flt_vec_n_str[n] );
      }
      ret_flt32 = (ndi_float32 *)malloc(num_elements * sizeof(ndi_float32) );
      tmp_err = NDI2_get_float32_vec_n(handle, inkey[n], 99, ret_flt32, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[n] );
	 print_float32_vec( ret_flt32, num_elements );
      } else {
	 print_float32_vec( ret_flt32, num_elements );
      }
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
      if (( ( (n+1) % 11) == 0) && (n != 0))
	 printf("\n");
      printf("%d   ", int_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/
