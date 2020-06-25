#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;

int test_ndid(void);
static void print_list( char *list, int list_num );
static void print_f64list( float64 *list, int list_num );

#ifdef NDI_STANDALONE
int main ()
{
  int ierr = 0;

  ierr = test_ndid();
  return(ierr);
}
#endif  /* NDI_STANDALONE */

int test_ndid(void)
{

  int ierr = 0;
  int arr_size;
  int arr_num;
  char *str_arr;
  float64 *awr;

  ierr = ndi_init();
  if ( ierr != 0 ) {
    return(0);
  }

  printf("\n  NDI TEST SUITE -- test_ndid \n\n");

  ndid_get_num( NDI_ZAID,  "multigroup_neutron", &arr_num );
  printf(" String array num is %d\n", arr_num );

  ndid_get_size( NDI_ZAID, "multigroup_neutron", &arr_size );
  printf(" String array size is %d\n", arr_size );
  str_arr = (char *)malloc( arr_size * sizeof(char) );

  /*
  ierr = ndid_get_string_vec( NDI_ZAID, "multigroup_neutron", str_arr, arr_size * sizeof(char) );
  printf(" String vec error is %d\n", ierr );
  print_list( str_arr, arr_num );
  */

  awr = (float64 *)malloc( arr_size * sizeof(float64) );
  ierr = ndid_zaid_float64_vec( NDI_AWR, "multigroup_neutron", str_arr, (int)(arr_size * sizeof(char)),
				awr, (int)(arr_size * sizeof(float64)) );
  printf(" Float vec error is %d\n", ierr );
  print_list( str_arr, arr_num );
  print_f64list( awr, arr_num );

  if ( str_arr ) {free(str_arr);}
  if ( awr ) { free(awr);}

  ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
   if (ierr)
     printf("  **Error %d in setting lib type.\n\n", ierr );
   ierr = ndi_set_isotope("94239.99");
   if (ierr)
     printf("  **Error %d in setting 94239.50.\n\n", ierr );
   else
     printf("  **Isotope set successfully.\n" );
 
   ierr = ndi_set_isotope("92235.60");
   if (ierr)
     printf("  **Error %d in setting 92235.60.\n\n", ierr );
   else
     printf("  **Isotope set successfully.\n");
 
   return(0);
}


static void print_list( char *list, int list_num ) {

  char *str_p;
  int list_count = 0;

  str_p = list;
  while ( list_count < list_num ) {
    list_count++;
    printf("%d:  %s\n", list_count, str_p);
    str_p += strlen(str_p) + 1;
  }
}

static void print_f64list( float64 *list, int list_num ) {

  float64 *flt_p;
  int list_count = 0;

  flt_p = list;
  while ( list_count < list_num ) {
    list_count++;
    printf("%d:  %lf\n", list_count, *flt_p);
    flt_p++;
  }
}
