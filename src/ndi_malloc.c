#include <stdlib.h>

void *ndi_malloc(size_t size) {

   if ( size <= 0 )
      return (NULL);
   else
      return(malloc(size)); 
}

void *ndi_calloc(size_t nmemb, size_t size) {

   if ( ( nmemb <= 0 ) || ( size <= 0 ) )
      return (NULL);
   else
      return(calloc(nmemb,size)); 
}
