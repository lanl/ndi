#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"


int test_gendir_path(void);

#ifdef __STANDALONE
int main ()
{
  int ierr = 0;

  ierr = test_gendir_path();
  return(ierr);
}
#endif  /* __STANDALONE */

int test_gendir_path(void)
{

  int ierr = 0;
  int str_size;
  char *path = NULL;

  ierr = ndi_init();
  if ( ierr != 0 ) {
    return(0);
  }

  str_size = ndi_get_size(NDI_GENDIR_PATH, &ierr);
  if (ierr != 0) {
      printf("  Error getting size of path = %d\n", ierr);
  }
  path = (char *)malloc(str_size * sizeof(char));
  ierr = ndi_get_string_val(NDI_GENDIR_PATH, path, str_size);

  printf("\n  PATH: %s (size=%d; err=%d)\n\n", path, str_size, ierr);

  ierr = ndi_close();

  if (path) free(path);

  return(0);
}
