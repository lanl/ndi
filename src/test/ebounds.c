#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"
#include "ndi_test.h"


static int check_vec_mgn_func(int handle);
static int check_ndi_float_vec_func_mgn(int handle);
static int check_ndi_float_vec_n_func_mgn(int handle);
static void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );


#ifdef __STANDALONE

int main ()
{
  int ierr = 0;

  ierr = test_collapse();
  return(ierr);
}
#endif  /* __STANDALONE */


int test_collapse (void)
{
   int ierr;
   int gendir_h;
   int handle;
   ndi_float64 _4_lanl[] = { 17.0, 7.79, 2.232, 0.184, 1.67e-4 };
   ndi_float64 _1_lanl[] = { 17.0, 1.67e-4 };

   ierr = ndi2_open_gendir(&gendir_h, "");
   ierr = ndi2_get_handle(gendir_h, &handle);
   if( ierr != 0) printf("  Error %d getting particle handle.\n", ierr);

   ierr = ndi2_set_option( handle, NDI_LIB_TYPE, "multigroup_neutron" );
   ierr = ndi2_set_option( handle, NDI_LIBRARY, "mendf6" );
   ierr = ndi2_set_option( handle, NDI_E_AVG, "from_e" );

   printf("  NDI TEST SUITE -- test_collapse \n");

   /* 4 group */
   printf("\n********************************************************************\n");
   printf("           4_lanl group\n");
   printf("*********************************************************************\n");
   ierr = ndi2_set_option( handle, NDI_COLLAPSE, "4_lanl" );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "4_lanl" );
   ierr = ndi2_set_isotope( handle, "92235");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "92235" );
   ierr = check_vec_mgn_func(handle);

   printf("*********************************************************************\n");
   printf("            1-group energy bounds\n");
   printf("*********************************************************************\n");
   ierr = ndi2_set_float64_vec_option( handle, NDI_COLLAPSE, _1_lanl, 2 );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "1 energy bounds" );


   ierr = ndi2_set_isotope(handle, "92235");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "92235" );
   ierr = check_vec_mgn_func(handle);


   printf("*********************************************************************\n");
   printf("            4-group energy bounds\n");
   printf("*********************************************************************\n");
   ierr = ndi2_set_float64_vec_option( handle, NDI_COLLAPSE, _4_lanl, 5 );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "4 energy bounds" );
   ierr = ndi2_set_isotope(handle, "92235");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "92235" );
   ierr = check_vec_mgn_func(handle);


   printf("*********************************************************************\n");
   printf("            1_lanl\n");
   printf("*********************************************************************\n");
   ierr = ndi2_set_option( handle, NDI_COLLAPSE, "1_lanl" );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "1_lanl" );
   ierr = ndi2_set_isotope(handle, "92235");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "92235" );
   ierr = check_vec_mgn_func(handle);

   ierr = ndi2_close_gendir(gendir_h);
   return(ierr);
}

int check_vec_mgn_func(int handle)

{

  int tmp_err = 0;

/*    tmp_err = check_ndi_int_vec_func_mgn(handle); */
  tmp_err = check_ndi_float_vec_func_mgn(handle);
/*    tmp_err = check_ndi_int_vec_n_func_mgn(handle); */
  tmp_err = check_ndi_float_vec_n_func_mgn(handle); 
   
  return(tmp_err);
}

  
/****************************************************************
****************************************************************/

static int check_ndi_float_vec_func_mgn(int handle)

{
   int n;
   int tmp_err = 0;
   ndi_float64 *ret_vec64;
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
      NDI_E_BOUNDS_CURR_PART };

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


   printf("\nFLOAT64 VECTORS:\n");

   tmp_err = ndi2_set_option( handle, NDI_CURR_PART, "photon" );

   for ( n = 0; n < num_keys; n++ ) {
      printf( "%s: \n", flt_vec_str[n] );
      num_elements = ndi2_get_size( handle, inkey[n], &tmp_err );  
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE of %s.\n", flt_vec_str[n] );
      }
      ret_vec64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
      tmp_err = ndi2_get_float64_vec( handle, inkey[n], ret_vec64, num_elements );
      if (tmp_err) {
	 printf("  **Error %d in %s -- returned value is:\n", tmp_err, 
		flt_vec_str[n] );
	 print_float64_vec( ret_vec64,  num_elements );
      } else {
	 print_float64_vec( ret_vec64, num_elements );	 
      }
      if (ret_vec64) 
	free(ret_vec64);
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
   int num_keys = 7;
   int num_elements = 0;
   int inkey[] = {
      NDI_PN_DOWN_FULL, 
      NDI_PN_FULL, 
      NDI_TRANSFER_MAT_TOT,
      NDI_TRANSFER_MAT_PR,
      NDI_CORR_DIAG,
      NDI_CORR_BHS,
      NDI_SIG_REAC
   };

   char *flt_vec_n_str[] = {
      "Scatter matrix--lower diagonal", 
      "Scatter matrix--full", 
      "Transfer matrix--total",
      "Transfer matrix--prompt",
      "Transport correction--diagonal",
      "Transport correction--Bell-Hansen-Sandmeier",
      "Reaction cross section"
   };


   tmp_err = ndi2_set_option( handle, NDI_CURR_PART, "photon" );

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
	free (ret_flt64);
 
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
	free (ret_flt64);
     
   }
   
   /* Handle data which requires an MT value as input */
   /* MT = 2 */
   num_elements = ndi2_get_size_n( handle, inkey[num_keys-1], 0, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_n_str[n] );
      }
   printf("%s (MT=2): \n", flt_vec_n_str[num_keys-1] );
   ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
   tmp_err = ndi2_get_float64_vec_n( handle, inkey[num_keys-1], 2, ret_flt64, num_elements);
   if (tmp_err) {
      printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[num_keys-1] );
      print_float64_vec( ret_flt64, num_elements );
   } else {
      print_float64_vec( ret_flt64, num_elements );
   }
   if (ret_flt64)
     free (ret_flt64);
     

   /* MT = 999 */
   num_elements = ndi2_get_size_n( handle, inkey[num_keys-1], 999, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_n_str[n] );
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
   printf("\n");
   if (ret_flt64)
     free (ret_flt64);

   return(tmp_err);

}


/****************************************************************
****************************************************************/

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
