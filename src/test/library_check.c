#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"


int test_library_check(void);

#ifdef __STANDALONE
int main ()
{
  int ierr = 0;

  ierr = test_library_check();
  return(ierr);
}
#endif  /* __STANDALONE */

int test_library_check(void)
{

  int ierr = 0;
  char mg_lib1[] = "mendf6";
  char mg_lib2[] = "mendf10";
  char mg_lib3[] = "MEndf6";
  char tn_lib1[] = "hale98";
  char tn_lib2[] = "testlib-tn";
  char tn_lib3[] = "TESTLIB_tn";
  char dos_lib1[] = "dostest";
  char dos_lib2[] = "dosimetrytest";
  char dos_lib3[] = "abuNTESt";

  ierr = ndi_init();
  if ( ierr != 0 ) {
    return(0);
  }

  ierr = ndi_set_option(NDI_LIB_TYPE, "multigroup_neutron" );
  printf ("  Setting multigroup library %s ", mg_lib1);
  ierr = ndi_set_option(NDI_LIBRARY, mg_lib1);
  printf ("error is %d.\n", ierr);
  printf ("  Setting multigroup library %s ", mg_lib2);
  ierr = ndi_set_option(NDI_LIBRARY, mg_lib2);
  printf ("error is %d.\n", ierr);
  printf ("  Setting multigroup library %s ", mg_lib3);
  ierr = ndi_set_option(NDI_LIBRARY, mg_lib3);
  printf ("error is %d.\n", ierr);

  ierr = ndi_set_option(NDI_LIB_TYPE, "tn" );
  printf ("  Setting tn library %s ", tn_lib1);
  ierr = ndi_set_option(NDI_LIBRARY, tn_lib1);
  printf ("error is %d.\n", ierr);
  printf ("  Setting tn library %s ", tn_lib2);
  ierr = ndi_set_option(NDI_LIBRARY, tn_lib2);
  printf ("error is %d.\n", ierr);
  printf ("  Setting tn library %s ", tn_lib3);
  ierr = ndi_set_option(NDI_LIBRARY, tn_lib3);
  printf ("error is %d.\n", ierr);

  ierr = ndi_set_option(NDI_LIB_TYPE, "dosimetry_neutron" );
  printf ("  Setting dosimetry library %s ", dos_lib1);
  ierr = ndi_set_option(NDI_LIBRARY, dos_lib1);
  printf ("error is %d.\n", ierr);
  printf ("  Setting dosimetry library %s ", dos_lib2);
  ierr = ndi_set_option(NDI_LIBRARY, dos_lib2);
  printf ("error is %d.\n", ierr);
  printf ("  Setting dosimetry library %s ", dos_lib3);
  ierr = ndi_set_option(NDI_LIBRARY, dos_lib3);
  printf ("error is %d.\n", ierr);

  ierr = ndi_close();
  return(0);
}
