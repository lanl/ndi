#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;


int test_err22(void);
static void print_float64_array( float64 *array, int num );


/*ifdef __STANDALONE*/
int main ()
{
  int ierr = 0;

  ierr = test_err22();
  return(ierr);
}
/*endif  * __STANDALONE */

int test_err22(void)
{
   typedef char radchem_zaid[30];
   
   int i, j;
   int ierr;
   int curr_izaid;
   int irprod;
   int num_target;
   int num_grps;
   int num_rprods;
   int sym_size = 150;
   char sym[150];
   char zaid[24];
   char radchem_info[120];
   char curr_zaid[24];
   char mg_zaid[24];
   radchem_zaid rprod[80];
   float64 sig_tot[50];
   float64 production_q;

    ierr = ndi_init();
   if ( ierr != 0 ) {
      return(0);
   }


   ierr = ndi_set_option( NDI_LIB_TYPE, "dosimetry_neutron" );
   if ( ierr != 0 ) printf("    Error setting lib type is %d.\n", ierr );

   ierr = ndi_set_option( NDI_CHAIN, "ir.001zrc" );
   if ( ierr != 0 ) printf("    Error setting radchem is %d.\n", ierr );
   ierr = ndi_set_option( NDI_COLLAPSE, "16_group" );
   if ( ierr != 0 ) printf("    Error setting collapse groups is %d.\n", ierr );
   printf("\n");


   /* Radchem ZAID */
   ierr = ndi_get_string_val( NDI_CHAIN_ZAID, zaid, 24 );
   printf("Radchem zaid:\n   %s\n", zaid);
   if ( ierr != 0 ) {
     printf("  Error getting chain zaid = %d.\n", ierr );
   }
   /* Radchem info */
   ierr = ndi_get_string_val( NDI_CHAIN_INFO, radchem_info, 120 );
   printf("Radchem info:\n   %s\n", radchem_info );
   if ( ierr != 0 ) {
     printf("  Error getting radchem info = %d.\n", ierr );
   }
   /* Number of targets */
   ierr = ndi_get_int_val( NDI_NUM_TARGET, &num_target );
   printf("No. of targets:\n   %d\n", num_target );
   if ( ierr != 0 ) {
     printf("  Error getting number of targets = %d.\n", ierr );
   }
   
   
   /* Target zaids */
   for ( j = 0; j < num_target; j++ ) {
     
     ierr = ndi_get_string_val_n( NDI_TARGET, j, curr_zaid, 24 );
     printf( "\n\n***********************\nCURRENT TARGET:  %s\n", curr_zaid );
     if ( ierr != 0 ) {
       printf("  Error getting current target = %d.\n", ierr );
     }     
     ierr = ndi_set_isotope( curr_zaid ); 
     if ( ierr != 0 ) printf("    Error setting isotope %s is %d.\n", curr_zaid, ierr );
     
     ierr = ndi_get_int_val_n( NDI_TARGET, j, &curr_izaid );
     if ( ierr != 0 ) printf("    Error setting target is %d.\n", ierr );
    printf( "    target(int): %d\n", curr_izaid);
 
     
     
     /* Number of rprods for current target */
     ierr = ndi_get_string_val( NDI_ZAID, mg_zaid, 24 );
     ierr = ndi_get_int_val( NDI_NUM_RPRODS, &num_rprods );
     printf("   No. of rprods available for target %s:   %d\n", mg_zaid, num_rprods );
     if ( ierr != 0 ) {
       printf("  Error getting number of rprods = %d.\n", ierr );
     }
     
     /* Rprod zaid */
     printf("   Rprods available in current radchem set for target %s:\n", mg_zaid );
     for ( i = 0; i < num_rprods; i++ ) {
       ierr = ndi_get_string_val_n( NDI_RPRODS_ID, i, rprod[i], 30 );
       ierr = ndi_get_int_val_n( NDI_RPRODS_ID, i, &irprod );
       ierr = ndi_get_string_val_n( NDI_ZAID, irprod, sym, sym_size);
       printf( "     %s  %d(int)  %s(sym)\n", rprod[i], irprod, sym );
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

     
       ierr = ndi_get_int_val_n( NDI_RPRODS_ID, i, &irprod );
       ierr = ndi_get_float64_val_n( NDI_TOT_PROD_Q, irprod, &production_q ); 
       printf("     Production Q-value for %d: %f\n", irprod, production_q );
       if ( ierr != 0 ) {
       printf("  Error getting production Q-value = %d.\n", ierr );
       }

     
       ierr = ndi_get_float64_vec_x( NDI_TOT_PROD_XS, rprod[i], sig_tot, 50 ); 
       printf("     %s:\n", rprod[i] );
       print_float64_array( sig_tot, num_grps );
       if ( ierr != 0 ) {
	 printf("  Error getting production cross section = %d.\n", ierr );
       }

     }

   }
   ierr = ndi_close();
   return(ierr);
}


/********************************************************************************/
static void print_float64_array( float64 *array, int num ) {

  int i;

  for (i = 0; i < num; i++ ) {
    printf("    %11.5e", array[i] );
    if ( ( (i+1) % 4 ) == 0 ) {
      printf("\n");
    }
  }
  printf("\n");

  return;

}
/********************************************************************************/
