#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;

int test_depl(void);
static void print_float64_array( float64 *array, int num );


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
   
   int i, j;
   int ierr;
   int str_size;
   int arr_size;
   int irprod;
   char *zaid;
   char *depl_info;
   int num_target;
   char *curr_zaid;
   char *mg_zaid;
   int num_rprods;
   char *rprod;
   float64 *sig_tot;
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
   ierr = ndi_init();
   if ( ierr != 0 ) {
      return(0);
   }
   ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
   if ( ierr != 0 ) printf("    Error setting lib type is %d.\n", ierr );
   ierr = ndi_set_option( NDI_LIBRARY, "mendf6" );
   if ( ierr != 0 ) printf("    Error setting library is %d.\n", ierr );
   ierr = ndi_set_option( NDI_DEPLETION, "test2" );
   if ( ierr != 0 ) printf("    Error setting depletion is %d.\n", ierr );
   ierr = ndi_set_option( NDI_COLLAPSE, "30_group" );
   if ( ierr != 0 ) printf("    Error setting collapse groups is %d.\n", ierr );
   printf("\n");

   /* Depletion ZAID */
   str_size = ndi_get_size( NDI_DEPL_ZAID, &ierr );
   zaid = (char *)malloc(str_size * sizeof(char) );
   ierr = ndi_get_string_val( NDI_DEPL_ZAID, zaid, str_size );
   printf("Depl zaid:\n   %s\n", zaid);
   if ( ierr != 0 ) {
     printf("  Error getting depl zaid = %d.\n", ierr );
   }
   if (zaid) 
     free(zaid);

   /* Depletion info */
   str_size = ndi_get_size( NDI_DEPL_INFO, &ierr );
   depl_info = (char *)malloc(str_size * sizeof(char) );
   ierr = ndi_get_string_val( NDI_DEPL_INFO, depl_info, str_size );
   printf("Depl info:\n   %s\n", depl_info );
   if ( ierr != 0 ) {
     printf("  Error getting depl info = %d.\n", ierr );
   }
   if (depl_info)
     free(depl_info);

   /* Number of targets */
   ierr = ndi_get_int_val( NDI_NUM_TARGET, &num_target );
   printf("No. of targets:\n   %d\n", num_target );
   if ( ierr != 0 ) {
     printf("  Error getting number of targets = %d.\n", ierr );
   }


   /* Target zaids */
   for ( j = 0; j < num_target; j++ ) {
     str_size = ndi_get_size_n( NDI_TARGET, j, &ierr );
     curr_zaid = (char *)malloc( str_size * sizeof(char) );
     ierr = ndi_get_string_val_n( NDI_TARGET, j, curr_zaid, str_size );
     printf( "\n\n***********************\nCURRENT TARGET:  %s\n", curr_zaid );
     if ( ierr != 0 ) {
       printf("  Error getting current target = %d.\n", ierr );
     }     
     ierr = ndi_set_isotope( curr_zaid );
     if ( ierr != 0 ) printf("    Error setting isotope %s is %d.\n", curr_zaid, ierr );
     if (curr_zaid )
       free(curr_zaid);

     /* Total depletion cross section for current target */
     arr_size = ndi_get_size( NDI_TOT_DEPL_XS, &ierr );
     sig_tot = (ndi_float64 *)malloc(arr_size * sizeof(ndi_float64) );
     ierr = ndi_get_float64_vec( NDI_TOT_DEPL_XS, sig_tot, arr_size ); 
     printf("Total depl cross section:\n");
     print_float64_array( sig_tot, arr_size );
     if ( ierr != 0 ) {
       printf("  Error getting depletion cross section = %d.\n", ierr );
     }
     if (sig_tot)
       free(sig_tot);
 

     /* Number of rprods for current target */
     str_size = ndi_get_size( NDI_ZAID, &ierr );
     mg_zaid = (char *)malloc(str_size * sizeof(char) );
     ierr = ndi_get_string_val( NDI_ZAID, mg_zaid, str_size );
     ierr = ndi_get_int_val( NDI_NUM_RPRODS, &num_rprods );
     printf("   No. of rprods available for target %s:   %d\n", mg_zaid, num_rprods );
     if ( ierr != 0 ) {
       printf("  Error getting number of rprods = %d.\n", ierr );
     }

     /* Rprod zaid */
     printf("   Rprods available in current depletion set for target %s:\n", mg_zaid );
     for ( i = 0; i < num_rprods; i++ ) {
       str_size = ndi_get_size_n( NDI_RPRODS_ID, i, &ierr );
       rprod = (char *)malloc(str_size * sizeof(char) );
       ierr = ndi_get_string_val_n( NDI_RPRODS_ID, i, rprod, str_size );
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
       ierr = ndi_get_int_val_n( NDI_RPRODS_ID, i, &irprod );
       printf("     %d:  ", irprod );

       ierr = ndi_get_float64_val_n( NDI_TOT_PROD_Q, irprod, &prod_q );
       printf("(Q = %e)\n", prod_q );
       if ( ierr != 0 ) {
	 printf("  Error getting production Q value = %d.\n", ierr );
       }

       /* Production cross section (1) */
       str_size = ndi_get_size_n( NDI_RPRODS_ID, i, &ierr );
       rprod = (char *)malloc(str_size * sizeof(char) );
       ierr = ndi_get_string_val_n( NDI_RPRODS_ID, i, rprod, str_size );

       arr_size = ndi_get_size_x( NDI_TOT_PROD_XS, rprod, &ierr );
       sig_tot = (ndi_float64 *)malloc(arr_size * sizeof(ndi_float64) );
       ierr = ndi_get_float64_vec_x( NDI_TOT_PROD_XS, rprod, sig_tot, arr_size ); 
       if (rprod)
	 free(rprod);
       print_float64_array( sig_tot, arr_size );
       if ( ierr != 0 ) {
	 printf("  Error getting production cross section = %d.\n", ierr );
       }
       if (sig_tot)
	 free(sig_tot);

 
       /* Production cross section (2) */
       arr_size = ndi_get_size_n( NDI_TOT_PROD_XS, irprod, &ierr );
       sig_tot = (ndi_float64 *)malloc(arr_size * sizeof(ndi_float64) );
       ierr = ndi_get_float64_vec_n( NDI_TOT_PROD_XS, irprod, sig_tot, arr_size ); 
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
   ierr = ndi_close();
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
