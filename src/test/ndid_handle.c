#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;

int test_ndid(void);
static void print_list( char *list, int list_num );
static void print_f64list( float64 *list, int list_num );

#ifdef __STANDALONE
int main ()
{
  int ierr = 0;

  ierr = test_ndid();
  return(ierr);
}
#endif  /* __STANDALONE */

int test_ndid(void)
{

  int ierr = 0;
  int arr_size;
  int arr_num;
  char *str_arr;
  float64 *awr;

  int gendir;

  ierr = NDI2_open_gendir(&gendir, "");
  if ( ierr != 0 ) {
    return(ierr);
  }

  ierr = NDI2_set_option_gendir(gendir, NDI_LIB_TYPE_DEFAULT,
				"multigroup_neutron");
  if (ierr != 0) {
      return(ierr);
  }

  printf("\n  NDI TEST SUITE -- test_ndid \n\n");

  arr_num = NDI2_get_size(gendir, NDI_TEMP, &ierr);
  printf(" String array num is %d\n", arr_num );

  arr_size = NDI2_get_size(gendir, NDI_ZAID, &ierr);
  printf(" String array size is %d\n", arr_size );
  str_arr = (char *)malloc( arr_size * sizeof(char) );

  awr = (float64 *)malloc( arr_num * sizeof(float64) );
  ierr = NDI2_get_string_vec(gendir, NDI_ZAID, str_arr, arr_size);
  if (ierr != 0) {
      return(ierr);
  }

  print_list( str_arr, arr_num );

  ierr = NDI2_get_float64_vec(gendir, NDI_AWR, awr, arr_num);
  printf(" Float vec error is %d\n", ierr );
  print_f64list( awr, arr_num );

  if ( str_arr ) {free(str_arr);}
  if ( awr ) { free(awr);}

  ierr = NDI2_close_gendir(gendir);
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
