#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;

int test_depl_zaid(void);
static void print_int_array( int *array, int num );

#ifdef __STANDALONE 
int main ()
{
  int ierr = 0;

  ierr = test_depl_zaid();
  return(ierr);
}
#endif  /* __STANDALONE */

int test_depl_zaid(void)
{

    int ierr, i;
    int num_rprods;
    int str_size;
    int irprod;
    int *rprod_vec = NULL;
    char *rprod = NULL;
        
  printf(" ********************************************************** \n" );
   printf("    TEST OF PRODUCTION/DEPLETION REACTION PRODUCT ZAID.\n");
   printf(" ********************************************************** \n\n" );
   ierr = ndi_init();
   if ( ierr != 0 ) {
      return(0);
   }
   ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
   if ( ierr != 0 ) printf("    Error setting lib type is %d.\n", ierr );
   ierr = ndi_set_option( NDI_LIBRARY, "mendf6" );
   if ( ierr != 0 ) printf("    Error setting library is %d.\n", ierr );
   ierr = ndi_set_option( NDI_DEPLETION, "test2.002zpd" );
   if ( ierr != 0 ) printf("    Error setting depletion is %d.\n", ierr );
   ierr = ndi_set_option( NDI_COLLAPSE, "30_group" );
   if ( ierr != 0 ) printf("    Error setting collapse groups is %d.\n", ierr );
   printf("\n");

   ierr = ndi_set_isotope( "94239" );
   if ( ierr != 0 ) printf("    Error setting isotope U235 is %d.\n", ierr );
   ierr = ndi_get_int_val( NDI_NUM_RPRODS, &num_rprods );
   printf("   No. of rprods available for target:   %d\n", num_rprods );
   if ( ierr != 0 ) {
       printf("  Error getting number of rprods = %d.\n", ierr );
   }

   rprod_vec = (int *)malloc(num_rprods * sizeof(int));
   ierr = ndi_get_int_vec(NDI_RPRODS_ID, rprod_vec, num_rprods);
   print_int_array(rprod_vec, num_rprods);
   if (rprod_vec) {
       free(rprod_vec);
       rprod_vec = NULL;
   }

   for (i = 0; i < num_rprods; i++) {

       str_size = ndi_get_size_n( NDI_RPRODS_ID, i, &ierr );
       rprod = (char *)malloc(str_size * sizeof(char) );
       ierr = ndi_get_string_val_n( NDI_RPRODS_ID, i, rprod, str_size );
       if ( ierr != 0 ) printf("    Error getting rprod string is %d.\n", ierr );
       
       ierr = ndi_get_int_val_n(NDI_RPRODS_ID, i, &irprod );
       if ( ierr != 0 ) printf("    Error getting rprod izaid is %d.\n", ierr );

       printf( "  %d: %s (%d)\n", i, rprod, irprod);
       if (rprod) { 
           free(rprod);
           rprod = NULL;
       }
   }

   ierr = ndi_set_option( NDI_LIB_TYPE, "tn" );
   if ( ierr != 0 ) printf("    Error setting tn lib type is %d.\n", ierr );
   ierr= ndi_set_reaction("d+t->n+a");
   if ( ierr != 0 ) printf("    Error setting reaction is %d.\n", ierr );
   ierr = ndi_get_int_val(NDI_NUM_RPRODS, &num_rprods);
   if ( ierr != 0 ) printf("    Error getting number of rprods is %d.\n", ierr );
   
   rprod_vec = (int *)malloc(num_rprods * sizeof(int));
   ierr = ndi_get_int_vec(NDI_RPRODS_ID, rprod_vec, num_rprods);
   if ( ierr != 0 ) printf("    Error getting rprod vector is %d.\n", ierr );
   print_int_array(rprod_vec, num_rprods);
   if (rprod_vec) {
       free(rprod_vec);
       rprod_vec = NULL;
   }

   for (i = 0; i < num_rprods; i++) {

       str_size = ndi_get_size_n( NDI_RPRODS_ID, i, &ierr );
       rprod = (char *)malloc(str_size * sizeof(char) );
       ierr = ndi_get_string_val_n( NDI_RPRODS_ID, i, rprod, str_size );
       if ( ierr != 0 ) printf("    Error getting rprod string is %d.\n", ierr );
       
       ierr = ndi_get_int_val_n(NDI_RPRODS_ID, i, &irprod );
       if ( ierr != 0 ) printf("    Error getting rprod izaid is %d.\n", ierr );

       printf( "  %d: %s (%d)\n", i, rprod, irprod);
       if (rprod) { 
           free(rprod);
           rprod = NULL;
       }
   }


   ndi_close();
   return(ierr);
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
