#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"
#include "ndi_test.h"


static int check_vec_mgn_func(void);
static int check_ndi_int_vec_func_mgn(void);
static int check_ndi_float_vec_func_mgn(void);
static int check_ndi_int_vec_n_func_mgn(void);
static int check_ndi_float_vec_n_func_mgn(void);
static void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );
static void print_float32_vec( ndi_float32 *flt_ptr, int num_elements );
static void print_int_vec( int *int_ptr, int num_elements );


#ifdef NDI_STANDALONE

int main ()
{
  int ierr = 0;

  ierr = test_collapse();
  return(ierr);
}
#endif  /* NDI_STANDALONE */


int test_collapse (void)
{
   int ierr;

   ierr = ndi_init();
   ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );

   printf("  NDI TEST SUITE -- test_collapse \n");

   /*  13 => 12 group */
   printf("\n\n*********************************************************************\n");
   printf("           13 group => 12 group\n");
   printf("*********************************************************************\n");
   ierr = ndi_set_option( NDI_COLLAPSE, "12_group" );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "12_group" );
   ierr = ndi_set_isotope("92235.130");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "92235.130" );
   ierr = check_vec_mgn_func();

   /*  3 => 1 group */
   printf("\n\n*********************************************************************\n");
   printf("           3 group => 1 group\n");
   printf("*********************************************************************\n");
   ierr = ndi_set_option( NDI_COLLAPSE, "1_group" );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "1_group" );
   ierr = ndi_set_isotope("92235.030");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "92235.030" );
   ierr = check_vec_mgn_func();

    /*  16 => 14 group */
   printf("\n\n*********************************************************************\n");
   printf("           16 group => 14 group\n");
   printf("*********************************************************************\n");
   ierr = ndi_set_isotope("92235.160");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "92235.160" );
   ierr = ndi_set_option( NDI_COLLAPSE, "14_group" );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "14_group" );
   ierr = check_vec_mgn_func();

     /*  16 lanl => 16 group awe */
   printf("\n\n*********************************************************************\n");
   printf("           16 group lanl => 16 group awe\n");
   printf("*********************************************************************\n");
/*    Isotope is already set to 16 groups above
      ierr = ndi_set_isotope("92235.160"); */
   ierr = ndi_set_option( NDI_COLLAPSE, "16_awe" );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "16_awe" );
   ierr = check_vec_mgn_func();

     /*  12 => 10 group */
   printf("\n\n*********************************************************************\n");
   printf("           12 group => 10 group\n");
   printf("*********************************************************************\n");
   ierr = ndi_set_option( NDI_COLLAPSE, "10_group" );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "10_group" );
   ierr = ndi_set_isotope("92235.120");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "92235.120" );
   ierr = check_vec_mgn_func();

   /*  4 => 2 group */
   printf("\n\n*********************************************************************\n");
   printf("           4 group => 2 group\n");
   printf("*********************************************************************\n");
   /*  This is incompatible with the 12-group structure of the current isotope */
   ierr = ndi_set_option( NDI_COLLAPSE, "2_group" );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "2_group" );
   ierr = ndi_set_isotope("92235.040");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "92235.040" );
   ierr = check_vec_mgn_func();

   /* Try again */
   printf("********************************\n");
   printf("Setting collapse structure again\n");
   printf("********************************\n");
   ierr = ndi_set_option( NDI_COLLAPSE, "2_group" );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "2_group" );
   ierr = check_vec_mgn_func();

   /*  4 => 3 group */
   printf("\n\n*********************************************************************\n");
   printf("           4 group => 3 group\n");
   printf("*********************************************************************\n");
   ierr = ndi_set_option( NDI_COLLAPSE, "3_group" );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "3_group" );
/*    Isotope with 4 groups already set just above
      ierr = ndi_set_isotope("92235.040"); */
   ierr = check_vec_mgn_func();

   /*  5 => 4 group */
   printf("\n\n*********************************************************************\n");
   printf("           5 group => 4 group\n");
   printf("*********************************************************************\n");
   ierr = ndi_set_option( NDI_COLLAPSE, "4_group" );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "4_group" );
   ierr = ndi_set_isotope("92235.050");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "92235.050" );
   ierr = check_vec_mgn_func();

   /*  3 => 30 group */
   printf("\n\n*********************************************************************\n");
   printf("           3 group => 30 group\n");
   printf("*********************************************************************\n");
   ierr = ndi_set_option( NDI_COLLAPSE, "30_group" );
   if (ierr) 
     printf("  **Error %d in setting collapse groups %s.\n\n", ierr,  "30_group" );
   ierr = ndi_set_isotope("92235.030");
   if (ierr) 
     printf("  **Error %d in setting isotope %s.\n\n", ierr, "92235.030" );
   ierr = check_vec_mgn_func();

   ierr = ndi_close();
   return(ierr);
}

int check_vec_mgn_func(void)

{

  int tmp_err = 0;

/*    tmp_err = check_ndi_int_vec_func_mgn(); */
   tmp_err = check_ndi_float_vec_func_mgn();
/*    tmp_err = check_ndi_int_vec_n_func_mgn(); */
   tmp_err = check_ndi_float_vec_n_func_mgn();

  return(tmp_err);
}

  
/****************************************************************
****************************************************************/

static int check_ndi_int_vec_func_mgn(void)

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
      num_elements = ndi_get_size( inkey[n], &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s.\n", int_vec_str[n] );
      }
      ret_vec = (int *)malloc(num_elements * sizeof(int) );
      tmp_err = ndi_get_int_vec( inkey[n], ret_vec, num_elements );
      if (tmp_err) {
	 printf("\n  **Error %d in %s -- returned value is:\n", tmp_err, 
		int_vec_str[n] );
	 print_int_vec( ret_vec,  num_elements );
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

static int check_ndi_float_vec_func_mgn(void)

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

   tmp_err = ndi_set_option( NDI_CURR_PART, "photon" );

   for ( n = 0; n < num_keys; n++ ) {
      printf( "%s: \n", flt_vec_str[n] );
      num_elements = ndi_get_size( inkey[n], &tmp_err );  
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE of %s.\n", flt_vec_str[n] );
      }
      ret_vec64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
      tmp_err = ndi_get_float64_vec( inkey[n], ret_vec64, num_elements );
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

static int check_ndi_int_vec_n_func_mgn(void)

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
	 num_elements = ndi_get_size_n( inkey[n], 2, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s.\n", int_vec_n_str[n] );
	 }
	 id = (int *)malloc(num_elements * sizeof(int) );
	 tmp_err = ndi_get_int_vec_n( inkey[n], 2, id, num_elements );
	 if (tmp_err) {
	    printf("  **Error %d in %s--returned value is\n", tmp_err, int_vec_n_str[n]);
	    print_int_vec( id, num_elements ); 
	 } else {
	    print_int_vec( id, num_elements ); 
	 }
	 printf( "%s (MT=2): \n", int_vec_n_str[n+1] );
	 num_elements = ndi_get_size_n( inkey[n+1], 2, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s.\n", int_vec_n_str[n] );
	 }
	 mult = (int *)malloc(num_elements * sizeof(int) );
	 tmp_err = ndi_get_int_vec_n( inkey[n+1], 2, mult, num_elements );
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
	 num_elements = ndi_get_size_n( inkey[n], 999, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s.\n", int_vec_n_str[n] );	   
	 }
	 id = (int *)malloc(num_elements * sizeof(int) );
	 tmp_err = ndi_get_int_vec_n( inkey[n], 999, id, num_elements );
	 if (tmp_err) {
	    printf("  **Error %d in %s--returned value is\n", tmp_err, int_vec_n_str[n]);
	    print_int_vec( id, num_elements ); 
	 } else {
	    print_int_vec( id, num_elements ); 
	 }
	 printf( "%s (MT=999): \n", int_vec_n_str[n+1] );
	 num_elements = ndi_get_size_n( inkey[n+1], 999, &tmp_err );
	 if ( num_elements <= 0 ) {
	   num_elements = 30;
	   printf("***ERROR GETTING SIZE FOR %s.\n", int_vec_n_str[n] );
	 }
	 mult = (int *)malloc(num_elements * sizeof(int) );
	 tmp_err = ndi_get_int_vec_n( inkey[n+1], 999, mult, num_elements );
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

static int check_ndi_float_vec_n_func_mgn(void)

{
   int n;
   int tmp_err = 0;
   ndi_float64 *ret_flt64;
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


   tmp_err = ndi_set_option( NDI_CURR_PART, "photon" );

   printf("\nFLOAT64 VECTORS (N):\n");

   /* Handle data which require a Pn order as input */
   for ( n = 0; n < (num_keys - 1); n++) {
      /* Pn order = 0 */
      printf("%s (0): \n", flt_vec_n_str[n] );
      num_elements = ndi_get_size_n( inkey[n], 0, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_n_str[n] );
      }
      ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
      tmp_err = ndi_get_float64_vec_n( inkey[n], 0, ret_flt64, num_elements );
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
      num_elements = ndi_get_size_n( inkey[n], 99, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_n_str[n] );
      }
      ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
      tmp_err = ndi_get_float64_vec_n( inkey[n], 99, ret_flt64, num_elements );
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
   num_elements = ndi_get_size_n( inkey[num_keys-1], 0, &tmp_err );
      if ( num_elements <= 0 ) {
	num_elements = 30;
	printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_n_str[n] );
      }
   printf("%s (MT=2): \n", flt_vec_n_str[num_keys-1] );
   ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );
   tmp_err = ndi_get_float64_vec_n( inkey[num_keys-1], 2, ret_flt64, num_elements);
   if (tmp_err) {
      printf("  **Error %d in %s--returned value is:\n", tmp_err, flt_vec_n_str[num_keys-1] );
      print_float64_vec( ret_flt64, num_elements );
   } else {
      print_float64_vec( ret_flt64, num_elements );
   }
   if (ret_flt64)
     free (ret_flt64);
     

   /* MT = 999 */
   num_elements = ndi_get_size_n( inkey[num_keys-1], 999, &tmp_err );
   if ( num_elements <= 0 ) {
     num_elements = 30;
     printf("***ERROR GETTING SIZE FOR %s.\n", flt_vec_n_str[n] );
   }
   ret_flt64 = (ndi_float64 *)malloc(num_elements * sizeof(ndi_float64) );

   printf("%s (MT=999): \n", flt_vec_n_str[num_keys-1] );
   tmp_err = ndi_get_float64_vec_n( inkey[num_keys-1], 999, ret_flt64, num_elements );
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
