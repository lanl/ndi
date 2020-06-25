#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;

static void print_float64_array( float64 *array, int num );
int test5(void);

#ifdef NDI_STANDALONE
int main ()
{
  int ierr = 0;

  ierr = test5();
  return(ierr);
}
#endif  /* NDI_STANDALONE */


int test5(void)
{
  int i;
  int j;
  int ierr;
  int ierr2;
  int num_parts;
  int pn_order;
  int arr_size;
  int arr_size2;
  int gendir_h;
  int handle;
  char zaid[24];
  char ptype[50];
  char pzaid[50];
  float64 ebounds[50];
  float64 heat[50];
  float64 kerma[50];
  float64 prod_mat[2500];

  ierr = NDI2_open_gendir(&gendir_h, "");
  if ( ierr != 0 ) {
    return(0);
  }

  ierr = NDI2_make_handle(gendir_h, "multigroup_neutron", &handle);
  if( ierr != 0) printf("  Error %d making particle handle.\n", ierr);

  printf("\n\n  NDI TEST SUITE -- test5\n\n");
  printf("\n\n****************************************\n  Test of MG secondary particle data\n");

  ierr = NDI2_set_isotope( handle, "92235" );
  if ( ierr != 0 ) printf("  Error setting isotope is %d.\n", ierr );

  arr_size = NDI2_get_size( handle, NDI_ZAID, &ierr2 );  
  ierr = NDI2_get_string_val( handle, NDI_ZAID, zaid, 24);
  printf("****************************************\n\nZAID %s     (size = %d; error = %d,%d).\n", 
	 zaid, arr_size, ierr2, ierr );

  arr_size = NDI2_get_size( handle, NDI_NUM_SEC_PARTS, &ierr2 );  
  ierr = NDI2_get_int_val( handle, NDI_NUM_SEC_PARTS, &num_parts );
  printf("  No. sec parts =  %d     (size = %d; error = %d,%d).\n", 
	 num_parts, arr_size, ierr2, ierr );

  for ( i = 0; i < num_parts; i++ ) {
    arr_size2 = NDI2_get_size_n( handle, NDI_SEC_PART_TYPES, i, &ierr2 );  
    ierr2 = NDI2_get_string_val_n(handle, NDI_SEC_PART_TYPES, i, ptype, 50 );
    arr_size = NDI2_get_size_n( handle, NDI_SEC_PART_ZAIDS, i, &ierr );  
    ierr = NDI2_get_string_val_n(handle, NDI_SEC_PART_ZAIDS, i, pzaid, 50 );
    printf("  Particle %d: Type is %s (size = %d; error = %d);  Zaid is %s (size = %d; error = %d).\n", 
	   i, ptype, arr_size2, ierr2, pzaid, arr_size, ierr );
   
    ierr = NDI2_set_option( handle, NDI_CURR_PART, ptype );
    if ( ierr != 0 ) printf("ERROR SETTING CURRENT PARTICLE TO %s is %d.\n", ptype, ierr );
    arr_size2 = NDI2_get_size( handle, NDI_SEC_PART_TYPES, &ierr2 );  

    arr_size2 = NDI2_get_size( handle, NDI_SEC_PART_TYPES, &ierr2 );  
    arr_size = NDI2_get_size( handle, NDI_SEC_PART_ZAIDS, &ierr );  
    printf("  Particle type is size %d;  Particle zaid is size %d.\n", arr_size2, arr_size);

    arr_size = NDI2_get_size( handle, NDI_E_BOUNDS_CURR_PART, &ierr2 );
    ierr = NDI2_get_float64_vec( handle, NDI_E_BOUNDS_CURR_PART, ebounds, 50 );
    printf("  Ebounds for particle %s is of size %d     (errors = %d,%d):\n",  
	   ptype, arr_size, ierr2, ierr );
    print_float64_array( ebounds, arr_size );

    arr_size = NDI2_get_size( handle, NDI_HEATING_CURR_PART, &ierr2 );
    ierr = NDI2_get_float64_vec( handle, NDI_HEATING_CURR_PART, heat, 50 );
    printf("  Heating for particle %s is of size %d     (errors = %d,%d):\n",  
	   ptype, arr_size, ierr2, ierr );
    print_float64_array( heat, arr_size );

    arr_size = NDI2_get_size( handle, NDI_KERMA_CURR_PART, &ierr2 );
    ierr = NDI2_get_float64_vec( handle, NDI_KERMA_CURR_PART, kerma, 50 );
    printf("  Kerma for particle %s is of size %d     (errors = %d,%d):\n",  
	   ptype, arr_size, ierr2, ierr );
    print_float64_array( kerma, arr_size );
   
    arr_size = NDI2_get_size( handle, NDI_PN_ORDER_CURR_PART, &ierr2 );
    ierr = NDI2_get_int_val( handle, NDI_PN_ORDER_CURR_PART, &pn_order );
    printf("  No. of production matrices is %d.    (size = %d; error = %d)\n",
	   pn_order, arr_size, ierr );

    for ( j = 0; j < pn_order; j++ ) {
      arr_size = NDI2_get_size_n( handle, NDI_PN_PROD_FULL_CURR_PART, j, &ierr2);
      ierr = NDI2_get_float64_vec_n( handle, NDI_PN_PROD_FULL_CURR_PART, j, prod_mat, 2500 );
      printf("    Production matrix of order %d    (size = %d; error = %d,%d):\n",
	     j, arr_size, ierr2, ierr );
      print_float64_array( prod_mat, arr_size );  
    }
  }
  (void)NDI2_close_gendir(gendir_h);

  return(ierr);
}

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
