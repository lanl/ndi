#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"
#include "ndi_test.h"



int main ()
{
  int ierr = 0;

  ierr = test_collapse();
  ierr = testc();
  ierr = test_depl();
  ierr = test_mgn_c();
  ierr = test_ndid();
  ierr = test_tn_c();
  ierr = test_tnreac_c();
/*   ierr = testtn2(); */
  ierr = testtn3();
  ierr = testtn4();
  ierr = testtn5();
  ierr = testtn_new(); 
  ierr = tn_chain();
  ierr = tn_simple();
  ierr = kerma_example();
  ierr = test5();

  return(ierr);
}
