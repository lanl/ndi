#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

#define MAX_ARRAY_SIZE 1000


int check_vec_mgn_func();
int check_ndi_int_vec_func_mgn();
int check_ndi_float_vec_func_mgn();
int check_ndi_int_vec_n_func_mgn();
int check_ndi_float_vec_n_func_mgn();
void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );
void print_float32_vec( ndi_float32 *flt_ptr, int num_elements );
void print_int_vec( int *int_ptr, int num_elements );

int main ()

{
   int ierr;
   int i;
   int num_zaid = 5;
   int ext_num_zaid = 24;
   int gendir_h = -1;
   int handle = -1;
   char test_zaid[133] = "";

   ierr = NDI2_open_gendir(&gendir_h, "");
   ierr = NDI2_get_handle(gendir_h, &handle);
   ierr = NDI2_set_option( handle, NDI_LIB_TYPE, "multigroup_neutron" );


   printf("\n\n*********************************************************************\n");
   printf("           Library does not exist\n");
   printf("*********************************************************************\n");
   ierr = NDI2_set_isotope(handle, "94239.001");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "94239.001" );
   ierr = NDI2_get_string_val( handle, NDI_ZAID, test_zaid, 133 );
   printf( "ZAID=%s\n", test_zaid );
   ierr = check_vec_mgn_func();

   printf("\n\n*********************************************************************\n");
   printf("           Bad keyword in library and gendir\n");
   printf("*********************************************************************\n");
   ierr = NDI2_set_isotope(handle, "94239.006");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "94239.006" );
   ierr = NDI2_get_string_val( handle, NDI_ZAID, test_zaid, 133 );
   printf( "ZAID=%s\n", test_zaid );
   ierr = check_vec_mgn_func();


   printf("\n\n*********************************************************************\n");
   printf("       Offset value is given incorrectly (before real zaid)\n");
   printf("*********************************************************************\n");
   ierr = NDI2_set_isotope(handle, "92235.030");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "92235.030" );
   ierr = NDI2_get_string_val( handle, NDI_ZAID, test_zaid, 133 );
   printf( "ZAID=%s\n", test_zaid );
   ierr = check_vec_mgn_func();


   printf("\n\n*********************************************************************\n");
   printf("       Offset value is given incorrectly (after real zaid)\n");
   printf("*********************************************************************\n");
   ierr = NDI2_set_isotope(handle, "92235.031");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "92235.031" );
   ierr = NDI2_get_string_val( handle, NDI_ZAID, test_zaid, 133 );
   printf( "ZAID=%s\n", test_zaid );
   ierr = check_vec_mgn_func();


   printf("\n\n*********************************************************************\n");
   printf("           Library does not exist (again)\n");
   printf("*********************************************************************\n");
   ierr = NDI2_set_isotope("94239.002");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "94239.002" );
   ierr = NDI2_get_string_val( NDI_ZAID, test_zaid, 133 );
   printf( "ZAID=%s\n", test_zaid );
   ierr = check_vec_mgn_func();

   ierr = NDI2_close_gendir(gendir_h);
   return(ierr);
}

int check_vec_mgn_func()

{

  int tmp_err = 0;

   tmp_err = check_ndi_float_vec_func_mgn();

  return(tmp_err);
}

  
/****************************************************************
****************************************************************/

int check_ndi_int_vec_func_mgn()

{
   int n;
   int tmp_err = 0;
   int ret_vec[MAX_ARRAY_SIZE];
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

   memset( ret_vec, 0, MAX_ARRAY_SIZE * sizeof(int) );

   printf("\nINTEGER VECTORS:\n");

   /* First retrieve number of reaction products */
   for (n = 0; n < num_keys; n++) {
      printf( "%s:  ", int_vec_str[n]);
      num_elements = NDI2_get_size( inkey[n], &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	if ( num_elements > MAX_ARRAY_SIZE ) {
	  num_elements = MAX_ARRAY_SIZE;
	}
      }
      tmp_err = NDI2_get_int_vec( inkey[n], ret_vec, MAX_ARRAY_SIZE );
      if (tmp_err) {
	 printf("\n  **Error %d in %s -- returned value is:\n", tmp_err, 
		int_vec_str[n] );
	 print_int_vec( ret_vec,  num_elements );
      } else {
	 print_int_vec( ret_vec, num_elements );	 
      }
   }
   
   printf("\n");
   return(tmp_err);
   
}

    
/****************************************************************
****************************************************************/

int check_ndi_float_vec_func_mgn()

{
   int n;
   int tmp_err = 0;
   ndi_float64 ret_vec64[MAX_ARRAY_SIZE];
   ndi_float32 ret_vec32[MAX_ARRAY_SIZE];
   int num_keys = 24;
   int num_elements = 0;
   int inkey[] = {
      NDI_E_BOUNDS, 
      NDI_E_AVG,
      NDI_ENERGY_N,
      NDI_ENERGY_AVAIL,
      NDI_WGTS,
      NDI_VEL,
      NDI_HEATING,
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
      NDI_E_BOUNDS_P };

   char *flt_vec_str[] = {
      "Energy boundaries", 
      "Average energy",
      "Energy n",
      "Energy available",
      "Weights (fluxes)", 
      "Velocities", 
      "Heating", 
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

   memset( ret_vec64, 0, MAX_ARRAY_SIZE * sizeof(ndi_float64) );
   memset( ret_vec32, 0, MAX_ARRAY_SIZE * sizeof(ndi_float32) );

   printf("\nFLOAT64 VECTORS:\n");

   for ( n = 0; n < num_keys; n++ ) {
      printf( "%s: \n", flt_vec_str[n] );
      num_elements = NDI2_get_size( inkey[n], &tmp_err );  
      if ( num_elements <= 0 ) {
	num_elements = 30;
	if ( num_elements > MAX_ARRAY_SIZE ) {
	  num_elements = MAX_ARRAY_SIZE;
	}
      }
      tmp_err = NDI2_get_float64_vec( inkey[n], ret_vec64, MAX_ARRAY_SIZE );
      if (tmp_err) {
	 printf("  **Error %d in %s -- returned value is:\n", tmp_err, 
		flt_vec_str[n] );
	 print_float64_vec( ret_vec64,  num_elements );
      } else {
	 print_float64_vec( ret_vec64, num_elements );	 
      }
   }
   printf("\n");

   return(tmp_err);
}



/****************************************************************
****************************************************************/

int check_ndi_int_vec_n_func_mgn()

{
   int n;
   int tmp_err = 0;
   int id[MAX_ARRAY_SIZE], mult[MAX_ARRAY_SIZE];
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
	 num_elements = NDI2_get_size_n( inkey[n], 2, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   if ( num_elements > MAX_ARRAY_SIZE ) {
	     num_elements = MAX_ARRAY_SIZE;
	   }
	 }
	 tmp_err = NDI2_get_int_vec_n( inkey[n], 2, id, MAX_ARRAY_SIZE );
	 if (tmp_err) {
	    printf("  **Error %d in %s--returned value is\n", tmp_err, int_vec_n_str[n]);
	    print_int_vec( id, num_elements ); 
	 } else {
	    print_int_vec( id, num_elements ); 
	 }
	 printf( "%s (MT=2): \n", int_vec_n_str[n+1] );
	 num_elements = NDI2_get_size_n( inkey[n+1], 2, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   if ( num_elements > MAX_ARRAY_SIZE ) {
	     num_elements = MAX_ARRAY_SIZE;
	   }
	 }
	 tmp_err = NDI2_get_int_vec_n( inkey[n+1], 2, mult, MAX_ARRAY_SIZE );
	 if (tmp_err) {
	    printf("  **Error %d in %s--returned value is\n", tmp_err, int_vec_n_str[n+1]);
	    print_int_vec( mult, num_elements ); 
	 } else {
	    print_int_vec( mult, num_elements ); 
	 }

	 /* MT = 999 */
	 printf( "%s (MT=999): \n", int_vec_n_str[n] );
	 num_elements = NDI2_get_size_n( inkey[n], 999, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   if ( num_elements > MAX_ARRAY_SIZE ) {
	     num_elements = MAX_ARRAY_SIZE;
	   }
	 }
	 tmp_err = NDI2_get_int_vec_n( inkey[n], 999, id, MAX_ARRAY_SIZE );
	 if (tmp_err) {
	    printf("  **Error %d in %s--returned value is\n", tmp_err, int_vec_n_str[n]);
	    print_int_vec( id, num_elements ); 
	 } else {
	    print_int_vec( id, num_elements ); 
	 }
	 printf( "%s (MT=999): \n", int_vec_n_str[n+1] );
	 num_elements = NDI2_get_size_n( inkey[n+1], 999, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   if ( num_elements > MAX_ARRAY_SIZE ) {
	     num_elements = MAX_ARRAY_SIZE;
	   }
	 }
	 tmp_err = NDI2_get_int_vec_n( inkey[n+1], 999, mult, MAX_ARRAY_SIZE );
	 if (tmp_err) {
	    printf("  **Error %d in %s--returned value is\n", tmp_err, int_vec_n_str[n+1]);
	    print_int_vec( mult, num_elements ); 
	 } else {
	    print_int_vec( mult, num_elements ); 
	 }
      } 
      printf("\n");  
      return(tmp_err);
}


/****************************************************************
****************************************************************/

int check_ndi_float_vec_n_func_mgn()

{
   int n;
   int tmp_err = 0;
   ndi_float64 ret_flt64[MAX_ARRAY_SIZE];
   ndi_float32 ret_flt32[MAX_ARRAY_SIZE];
   int num_keys = 8;
   int num_elements = 0;
   int inkey[] = {
      NDI_PN_DOWN_FULL, 
      NDI_PN_FULL, 
      NDI_PN_PROD_FULL_P,
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

   memset( ret_flt64, 0, MAX_ARRAY_SIZE * sizeof(ndi_float64) );
   memset( ret_flt32, 0, MAX_ARRAY_SIZE * sizeof(ndi_float32) );

   printf("\nFLOAT64 VECTORS (N):\n");

   /* Handle data which require a Pn order as input */
   for ( n = 0; n < (num_keys - 1); n++) {
      /* Pn order = 0 */
      printf("%s (0): \n", flt_vec_n_str[n] );
      num_elements = NDI2_get_size_n( inkey[n], 0, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	if ( num_elements > MAX_ARRAY_SIZE ) {
	  num_elements = MAX_ARRAY_SIZE;
	}
      }
      tmp_err = NDI2_get_float64_vec_n( inkey[n], 0, ret_flt64, MAX_ARRAY_SIZE );
      if (tmp_err) {
	 printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[n]);
	 print_float64_vec( ret_flt64, num_elements );
      } else {
	 print_float64_vec( ret_flt64, num_elements );
      }
 
      /* Pn order = 99 */
      printf("%s (99): \n", flt_vec_n_str[n] );
      num_elements = NDI2_get_size_n( inkey[n], 99, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	if ( num_elements > MAX_ARRAY_SIZE ) {
	  num_elements = MAX_ARRAY_SIZE;
	}
      }
      tmp_err = NDI2_get_float64_vec_n( inkey[n], 99, ret_flt64, MAX_ARRAY_SIZE );
      if (tmp_err) {
	 printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[n] );
	 print_float64_vec( ret_flt64, num_elements );
      } else {
	 print_float64_vec( ret_flt64, num_elements );
      }
   }
   
   /* Handle data which requires an MT value as input */
   /* MT = 2 */
   num_elements = NDI2_get_size_n( inkey[num_keys-1], 0, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	if ( num_elements > MAX_ARRAY_SIZE ) {
	  num_elements = MAX_ARRAY_SIZE;
	}
      }
   printf("%s (MT=2): \n", flt_vec_n_str[num_keys-1] );
   tmp_err = NDI2_get_float64_vec_n( inkey[num_keys-1], 2, ret_flt64, MAX_ARRAY_SIZE );
   if (tmp_err) {
      printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[num_keys-1] );
      print_float64_vec( ret_flt64, num_elements );
   } else {
      print_float64_vec( ret_flt64, num_elements );
   }

   /* MT = 999 */
   num_elements = NDI2_get_size_n( inkey[num_keys-1], 999, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	if ( num_elements > MAX_ARRAY_SIZE ) {
	  num_elements = MAX_ARRAY_SIZE;
	}
      }
   printf("%s (MT=999): \n", flt_vec_n_str[num_keys-1] );
   tmp_err = NDI2_get_float64_vec_n( inkey[num_keys-1], 999, ret_flt64, MAX_ARRAY_SIZE );
   if (tmp_err) {
      printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[num_keys-1] );
      print_float64_vec( ret_flt64, num_elements );
   } else {
      print_float64_vec( ret_flt64, num_elements );
   }
   printf("\n");

   return(tmp_err);

}


/****************************************************************
****************************************************************/

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements ) {

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

void print_float32_vec( ndi_float32 *flt_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 5 ) == 0) )
	 printf("\n");
      printf("%13.6e   ", flt_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/

void print_int_vec( int *int_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( (n % 10) == 0) && (n != 0) )
	 printf("\n");
      printf("%d   ", int_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/
