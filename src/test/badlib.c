#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

#define MAX_ARRAY_SIZE 100

int main ()
{

  int ierr;
  int i;
  int num_zaid = 5;
  int ext_num_zaid = 24;
  char test_zaid[133] = "";
  
  ierr = ndi_init();
  ierr = ndi_set_option( NDI_LIB_TYPE, "multigroup_neutron" );
  
  ierr = ndi_set_isotope("1001.601nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "1001.601nm" );
  ierr = ndi_set_isotope("1001.602nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "1001.602nm" );
  ierr = ndi_set_isotope("1001.603nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "1001.603nm" );
  ierr = ndi_set_isotope("1001.604nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "1001.604nm" );
  ierr = ndi_set_isotope("1001.605nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "1001.605nm" );
  ierr = ndi_set_isotope("1001.606nm");
  ierr = ndi_get_int_val( NDI_NUM_GRPS, int_val );
  ierr = ndi_get_float64_vec( NDI_WGTS, float64_vec, 50 );

  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "1001.606nm" );
  ierr = ndi_set_isotope("1001.607nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "1001.607nm" );

  ierr = ndi_set_isotope("92235.031nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "92235.031nm" );
  ierr = ndi_set_isotope("92235.032nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "92235.032nm" );
  ierr = ndi_set_isotope("92235.033nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "92235.033nm" );
  ierr = ndi_set_isotope("92235.034nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "92235.034nm" );
  ierr = ndi_set_isotope("92235.035nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "92235.035nm" );
 
  ierr = ndi_set_isotope("92235.036nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "92235.036nm" );

  ierr = ndi_set_isotope("94239.041nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "94239.041nm" );
  ierr = ndi_set_isotope("94239.042nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "94239.042nm" );
  ierr = ndi_set_isotope("94239.043nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "94239.043nm" );
  ierr = ndi_set_isotope("94239.044nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "94239.044nm" );
  ierr = ndi_set_isotope("94239.045nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "94239.045nm" );
  ierr = ndi_set_isotope("94239.046nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "94239.046nm" );
  ierr = ndi_set_isotope("94239.047nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "94239.047nm" );
  ierr = ndi_set_isotope("94239.048nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "94239.048nm" );

  ierr = ndi_set_isotope("8016.051nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "8016.051nm" );
  ierr = ndi_set_isotope("8016.052nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "8016.052nm" );
  ierr = ndi_set_isotope("8016.053nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "8016.053nm" );
  ierr = ndi_set_isotope("8016.055nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "8016.055nm" );
  ierr = ndi_set_isotope("8016.056nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "8016.056nm" );
  ierr = ndi_set_isotope("8016.057nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "8016.057nm" );
  ierr = ndi_set_isotope("8016.058nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "8016.058nm" );

  ierr = ndi_set_isotope("9019.031nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "9019.031nm" );
  ierr = ndi_set_isotope("9019.032nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "9019.032nm" );
  ierr = ndi_set_isotope("9019.033nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "9019.033nm" );
  ierr = ndi_set_isotope("9019.034nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "9019.034nm" );
  ierr = ndi_set_isotope("9019.035nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "9019.035nm" );
  ierr = ndi_set_isotope("9019.036nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "9019.036nm" );


  ierr = ndi_set_isotope("25056.031nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "25056.031nm" );
  ierr = ndi_set_isotope("25056.032nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "25056.032nm" );
  ierr = ndi_set_isotope("25056.033nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "25056.033nm" );
  ierr = ndi_set_isotope("25056.034nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "25056.034nm" );
  ierr = ndi_set_isotope("25056.035nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "25056.035nm" );

  ierr = ndi_set_isotope("1002.031nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "1002.031nm" );
  ierr = ndi_set_isotope("1002.032nm");
  if (ierr) 
    printf("  **Error %d in setting isotope %s.\n", ierr, "1002.032nm" );


  ierr = ndi_close();
  return(ierr);
  
}
