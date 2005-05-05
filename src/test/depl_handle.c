#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;

int test_depl(void);
static void print_float64_array( float64 *array, int num );
static void print_int_array( int *array, int num );


#ifdef __STANDALONE 
int main ()
{
  int ierr = 0;

  ierr = test_depl();
  return(ierr);
}
#endif  /* __STANDALONE */

int test_depl(void)
{
   
   int i, j, k;
   int ierr;
   int str_size;
   int arr_size;
   int irprod;
   int gendir_h = -1;
   int handle = -1;
   char *zaid;
   char *depl_info;
   int num_target;
   char *curr_zaid;
   char *mg_zaid;
   int num_rprods;
   char *rprod;
   float64 *sig_tot;
   int *mt_arr;
   char *mt_label;
   float64 prod_q;

   printf("  NDI TEST SUITE -- test_depl \n");

   /* These tests will also check 'init'ing and
      closing the NDI multiple times
      */

   /* Test 1 -- Set everything
      Set depl zaid, set isotope, get depletion data
      */
   
   printf(" ********************************************************** \n" );
   printf("    TEST OF PRODUCTION/DEPLETION FUNCTIONALITY.\n");
   printf(" ********************************************************** \n\n" );
   ierr = NDI2_open_gendir(&gendir_h, "");
   if ( ierr != 0 ) {
      return(0);
   }
   ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle);
   if( ierr != 0) printf("  Error %d making particle handle.\n", ierr);
   ierr = NDI2_set_option(handle, NDI_LIBRARY, "mendf6" );
   if ( ierr != 0 ) printf("    Error setting library is %d.\n", ierr );
   ierr = NDI2_set_option(handle, NDI_DEPLETION, "test2" );
   if ( ierr != 0 ) printf("    Error setting depletion is %d.\n", ierr );
   ierr = NDI2_set_option(handle, NDI_COLLAPSE, "30_group" );
   if ( ierr != 0 ) printf("    Error setting collapse groups is %d.\n", ierr );
   printf("\n");

   /* Depletion ZAID */
   str_size = NDI2_get_size(handle, NDI_DEPL_ZAID, &ierr );
   zaid = (char *)malloc(str_size * sizeof(char) );
   ierr = NDI2_get_string_val(handle, NDI_DEPL_ZAID, zaid, str_size );
   printf("Depl zaid:\n   %s\n", zaid);
   if ( ierr != 0 ) {
     printf("  Error getting depl zaid = %d.\n", ierr );
   }
   if (zaid) 
     free(zaid);

   /* Depletion info */
   str_size = NDI2_get_size(handle, NDI_DEPL_INFO, &ierr );
   depl_info = (char *)malloc(str_size * sizeof(char) );
   ierr = NDI2_get_string_val(handle, NDI_DEPL_INFO, depl_info, str_size );
   printf("Depl info:\n   %s\n", depl_info );
   if ( ierr != 0 ) {
     printf("  Error getting depl info = %d.\n", ierr );
   }
   if (depl_info)
     free(depl_info);

   /* Number of targets */
   ierr = NDI2_get_int_val(handle, NDI_NUM_TARGET, &num_target );
   printf("No. of targets:\n   %d\n", num_target );
   if ( ierr != 0 ) {
     printf("  Error getting number of targets = %d.\n", ierr );
   }


   /* Target zaids */
   for ( j = 0; j < num_target; j++ ) {
     str_size = NDI2_get_size_n(handle, NDI_TARGET, j, &ierr );
     curr_zaid = (char *)malloc( str_size * sizeof(char) );
     ierr = NDI2_get_string_val_n(handle, NDI_TARGET, j, curr_zaid, str_size );
     printf( "\n\n***********************\nCURRENT TARGET:  %s\n", curr_zaid );
     if ( ierr != 0 ) {
       printf("  Error getting current target = %d.\n", ierr );
     }     
     ierr = NDI2_set_isotope(handle, curr_zaid );
     if ( ierr != 0 ) printf("    Error setting isotope %s is %d.\n", curr_zaid, ierr );
     if (curr_zaid )
       free(curr_zaid);

     /* Total depletion cross section for current target */
     arr_size = NDI2_get_size(handle, NDI_TOT_DEPL_XS, &ierr );
     sig_tot = (ndi_float64 *)malloc(arr_size * sizeof(ndi_float64) );
     ierr = NDI2_get_float64_vec(handle, NDI_TOT_DEPL_XS, sig_tot, arr_size ); 
     printf("Total depl cross section:\n");
     print_float64_array( sig_tot, arr_size );
     if ( ierr != 0 ) {
       printf("  Error getting depletion cross section = %d.\n", ierr );
     }
     if (sig_tot)
       free(sig_tot);
 

     /* Number of rprods for current target */
     str_size = NDI2_get_size(handle, NDI_ZAID, &ierr );
     mg_zaid = (char *)malloc(str_size * sizeof(char) );
     ierr = NDI2_get_string_val(handle, NDI_ZAID, mg_zaid, str_size );
     ierr = NDI2_get_int_val(handle, NDI_NUM_RPRODS, &num_rprods );
     printf("   No. of rprods available for target %s:   %d\n", mg_zaid, num_rprods );
     if ( ierr != 0 ) {
       printf("  Error getting number of rprods = %d.\n", ierr );
     }

     /* Rprod zaid */
     printf("   Rprods available in current depletion set for target %s:\n", mg_zaid );
     for ( i = 0; i < num_rprods; i++ ) {
       str_size = NDI2_get_size_n(handle, NDI_RPRODS_ID, i, &ierr );
       rprod = (char *)malloc(str_size * sizeof(char) );
       ierr = NDI2_get_string_val_n(handle, NDI_RPRODS_ID, i, rprod, str_size );
       printf( "     %s\n", rprod );
       if (rprod)
	 free(rprod);
       if ( ierr != 0 ) {
	 printf("  Error getting current rprod = %d.\n", ierr );
       }
     }

     /* Total production cross section for each rprod */
     printf("   Total production cross sections for target %s:\n", mg_zaid );
     if (mg_zaid)
       free(mg_zaid);
     for ( i = 0; i < num_rprods; i++ ) {

       /* Production Q value */
       ierr = NDI2_get_int_val_n(handle, NDI_RPRODS_ID, i, &irprod );
       printf("     %d:  ", irprod );

       ierr = NDI2_get_float64_val_n(handle, NDI_TOT_PROD_Q, irprod, &prod_q );
       printf("(Q = %e)\n", prod_q );
       if ( ierr != 0 ) {
	 printf("  Error getting production Q value = %d.\n", ierr );
       }

       /* Production cross section (1) */
       str_size = NDI2_get_size_n(handle, NDI_RPRODS_ID, i, &ierr );
       rprod = (char *)malloc(str_size * sizeof(char) );
       ierr = NDI2_get_string_val_n(handle, NDI_RPRODS_ID, i, rprod, str_size );

       arr_size = NDI2_get_size_x(handle, NDI_TOT_PROD_XS, rprod, &ierr );
       sig_tot = (ndi_float64 *)malloc(arr_size * sizeof(ndi_float64) );
       ierr = NDI2_get_float64_vec_x(handle, NDI_TOT_PROD_XS, rprod, sig_tot, 
                                     arr_size ); 
       print_float64_array( sig_tot, arr_size );
       if ( ierr != 0 ) {
	 printf("  Error getting production cross section = %d.\n", ierr );
       }
       if (sig_tot) free(sig_tot);

       /* Test for Requirement 16: Reaction MTs */
       arr_size = NDI2_get_size_x(handle, NDI_REAC_MT, rprod, &ierr);
       mt_arr = (int *)malloc(arr_size * sizeof(int));
       ierr = NDI2_get_int_vec_x(handle, NDI_REAC_MT, rprod, mt_arr, arr_size);
       printf("Reaction MTs:\n");
       print_int_array(mt_arr, arr_size);
       if ( ierr != 0 ) {
	 printf("  Error getting reaction MTs = %d.\n", ierr );
       }
       /* Test for Requirement 19: Reaction labels */
       for (k=0; k < arr_size; k++) {
            str_size = NDI2_get_size_n(handle, NDI_REAC_LABEL, mt_arr[k], 
                                       &ierr);
            mt_label = (char *)malloc(str_size * sizeof(char));
            ierr = NDI2_get_string_val_n(handle, NDI_REAC_LABEL, mt_arr[k],
                                         mt_label, str_size);
            printf("MT:%d = %s\n", mt_arr[k], mt_label);
            if (mt_label) free(mt_label);
       }
       if (mt_arr) free(mt_arr);

       /* Test for Requirement 16: Reaction Product Multiplicities */
       arr_size = NDI2_get_size_x(handle, NDI_RPROD_MLT, rprod, &ierr);
       sig_tot = (ndi_float64 *)malloc(arr_size * sizeof(ndi_float64));
       ierr = NDI2_get_float64_vec_x(handle, NDI_RPROD_MLT, rprod, sig_tot, 
                                     arr_size);
       printf("Reaction product multiplicities:\n");
       print_float64_array(sig_tot, arr_size);
       if ( ierr != 0 ) {
	 printf("  Error getting product multiplicities = %d.\n", ierr );
       }
       if (sig_tot) free(sig_tot);

       if (rprod) free(rprod);

 
       /* Production cross section (2) */
       arr_size = NDI2_get_size_n(handle, NDI_TOT_PROD_XS, irprod, &ierr );
       sig_tot = (ndi_float64 *)malloc(arr_size * sizeof(ndi_float64) );
       ierr = NDI2_get_float64_vec_n(handle, NDI_TOT_PROD_XS, irprod, sig_tot, arr_size ); 
       printf("\n");
       print_float64_array( sig_tot, arr_size );
       if ( ierr != 0 ) {
	 printf("  Error getting production cross section = %d.\n", ierr );
       }
       if (sig_tot)
	 free(sig_tot);

       /*
	 switch ( i ) {
	 case 0:
	 ierr = ndi_get_float64_vec_n( NDI_SIG_REAC, 16, sig_tot, 50 );
	 break;
	 case 1:
	 ierr = ndi_get_float64_vec_n( NDI_SIG_REAC, 102, sig_tot, 50 );
	 break;
	 case 2:
	 ierr = ndi_get_float64_vec_n( NDI_SIG_REAC, 18, sig_tot, 50 );
	 break;       
	 }
	 print_float64_array( sig_tot, num_grps );
	 if ( ierr != 0 ) {
	 printf("  Error getting reaction cross section = %d.\n", ierr );
	 }
	 */

     }
   }
   ierr = NDI2_close_gendir(gendir_h);
   return(0);
}


/********************************************************************************/
static void print_float64_array( float64 *array, int num ) {

  int i;

  for (i = 0; i < num; i++ ) {
    printf("    %11.5e", array[i] );
    if ( ( (i+1) % 7 ) == 0 ) {
      printf("\n");
    }
  }
  printf("\n");

  return;

}
/********************************************************************************/
static void print_int_array( int *array, int num ) {

  int i;

  for (i = 0; i < num; i++ ) {
    printf("    %10d", array[i] );
    if ( ( (i+1) % 5 ) == 0 ) {
      printf("\n");
    }
  }
  printf("\n");

  return;

}
/********************************************************************************/
