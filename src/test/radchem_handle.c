#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;


int test_radchem_chain(void);
static void print_float64_array( float64 *array, int num );


/*ifdef __STANDALONE*/
int main ()
{
  int ierr = 0;

  ierr = test_radchem_chain();
  return(ierr);
}
/*endif  * __STANDALONE */

int test_radchem_chain(void)
{
   typedef char radchem_zaid[30];
   
   int i, j;
   int ierr;
   int curr_izaid;
   int irprod;
   int gendir_h;
   int handle; 
   char zaid[24];
   char radchem_info[120];
   int num_target;
   char curr_zaid[24];
   char mg_zaid[24];
   int num_rprods;
   radchem_zaid rprod[80];
   float64 sig_tot[50];
   float64 abundance;
   float64 production_q;
   int num_grps;

   printf("  NDI TEST SUITE -- test_radchem \n");

   /* These tests will also check 'init'ing and
      closing the NDI multiple times
      */



   /* Test 1 -- Set everything
      Set radchem zaid, set isotope, get radchem data
      */
   
   printf(" ********************************************************** \n" );
   printf("    TEST OF RADCHEM FUNCTIONALITY.\n");
   printf(" ********************************************************** \n\n" );
   ierr = NDI2_open_gendir(&gendir_h, "");
   if ( ierr != 0 ) {
      return(0);
   }
    ierr = NDI2_make_handle(gendir_h, "dosimetry_neutron", &handle);
    if( ierr != 0) printf("  Error %d making particle handle.\n", ierr);

   /* Test abundances */
   ierr = NDI2_set_isotope(handle, "77191");
   ierr = NDI2_get_float64_val_n(handle, NDI_ABUNDANCE, 77191, &abundance );
   printf ("Abundance for 77191 is %f.\n", abundance);
   ierr = NDI2_set_isotope(handle, "77193");
   ierr = NDI2_get_float64_val_n(handle, NDI_ABUNDANCE, 77193, &abundance );
   printf ("Abundance for 77193 is %f.\n", abundance);
   ierr = NDI2_set_isotope(handle, "39089");
   ierr = NDI2_get_float64_val_n(handle, NDI_ABUNDANCE, 39089, &abundance );
   printf ("Abundance for 39089 is %f.\n\n\n\n", abundance);


   /*
   ierr = ndi_set_option( NDI_LIBRARY, "mendf5" );
   if ( ierr != 0 ) printf("    Error setting library is %d.\n", ierr );
   */
   ierr = NDI2_set_option(handle, NDI_CHAIN, "ir.001zrc" );
   if ( ierr != 0 ) printf("    Error setting radchem is %d.\n", ierr );
   ierr = NDI2_set_option(handle, NDI_COLLAPSE, "16_group" );
   if ( ierr != 0 ) printf("    Error setting collapse groups is %d.\n", ierr );
   printf("\n");


   /* Radchem ZAID */
   ierr = NDI2_get_string_val(handle, NDI_CHAIN_ZAID, zaid, 24 );
   printf("Radchem zaid:\n   %s\n", zaid);
   if ( ierr != 0 ) {
     printf("  Error getting chain zaid = %d.\n", ierr );
   }
   /* Radchem info */
   ierr = NDI2_get_string_val(handle, NDI_CHAIN_INFO, radchem_info, 120 );
   printf("Radchem info:\n   %s\n", radchem_info );
   if ( ierr != 0 ) {
     printf("  Error getting radchem info = %d.\n", ierr );
   }
   /* Number of targets */
   ierr = NDI2_get_int_val(handle, NDI_NUM_TARGET, &num_target );
   printf("No. of targets:\n   %d\n", num_target );
   if ( ierr != 0 ) {
     printf("  Error getting number of targets = %d.\n", ierr );
   }
   
   
   /* Target zaids */
   for ( j = 0; j < num_target; j++ ) {
     
     ierr = NDI2_get_string_val_n(handle, NDI_TARGET, j, curr_zaid, 24 );
     printf( "\n\n***********************\nCURRENT TARGET:  %s\n", curr_zaid );
     if ( ierr != 0 ) {
       printf("  Error getting current target = %d.\n", ierr );
     }     
     ierr = NDI2_set_isotope(handle, curr_zaid ); 
     if ( ierr != 0 ) printf("    Error setting isotope %s is %d.\n", curr_zaid, ierr );
     
     /* Abundance */
     ierr = NDI2_get_int_val_n(handle, NDI_TARGET, j, &curr_izaid );
     printf( "    target(int): %d\n", curr_izaid);
     ierr = NDI2_get_float64_val_n(handle, NDI_ABUNDANCE, curr_izaid, &abundance );
     if ( ierr != 0 ) {
       printf("    Error getting abundance for %d is %d.\n", curr_izaid, ierr );
     }
     printf("  Abundance = %f.\n", abundance);
     
     
     /* Number of rprods for current target */
     ierr = NDI2_get_string_val(handle, NDI_ZAID, mg_zaid, 24 );
     ierr = NDI2_get_int_val(handle, NDI_NUM_RPRODS, &num_rprods );
     printf("   No. of rprods available for target %s:   %d\n", mg_zaid, num_rprods );
     if ( ierr != 0 ) {
       printf("  Error getting number of rprods = %d.\n", ierr );
     }
     
     /* Rprod zaid */
     printf("   Rprods available in current radchem set for target %s:\n", mg_zaid );
     for ( i = 0; i < num_rprods; i++ ) {
       ierr = NDI2_get_string_val_n(handle, NDI_RPRODS_ID, i, rprod[i], 30 );
       ierr = NDI2_get_int_val_n(handle, NDI_RPRODS_ID, i, &irprod );
       printf( "     %s  %d(int)\n", rprod[i], irprod );
       if ( ierr != 0 ) {
	 printf("  Error getting current rprod = %d.\n", ierr );
       }
     }
     
     /* Total production cross section for each rprod */
     /*    ierr = ndi_get_int_val( NDI_NUM_GRPS, &num_grps ); */
     num_grps = 16;
     if ( num_grps > 50 ) { num_grps = 50; }
     
     /* Production Q not relevant for radchem */
     printf("   Total production cross sections for target %s:\n", mg_zaid );
     for ( i = 0; i < num_rprods; i++ ) {

     
       ierr = NDI2_get_int_val_n(handle, NDI_RPRODS_ID, i, &irprod );
       ierr = NDI2_get_float64_val_n(handle, NDI_TOT_PROD_Q, irprod, &production_q ); 
       printf("     Production Q-value for %d: %f\n", irprod, production_q );
       if ( ierr != 0 ) {
       printf("  Error getting production Q-value = %d.\n", ierr );
       }

     
       ierr = NDI2_get_float64_vec_x(handle, NDI_TOT_PROD_XS, rprod[i], sig_tot, 50 ); 
       printf("     %s:\n", rprod[i] );
       print_float64_array( sig_tot, num_grps );
       if ( ierr != 0 ) {
	 printf("  Error getting production cross section = %d.\n", ierr );
       }

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
   return(ierr);
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
