#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;

int testtn4(void);

#ifdef __STANDALONE
int main (void)
{
  int ierr = 0;

  ierr = testtn4();
  return(ierr);
}
#endif  /* __STANDALONE */

int testtn4(void)
{
   typedef char tn_zaid[30];
   
   tn_zaid zaidlist[50];
   int ierr;
   int num_reac;
   int i,j;
   int num_sec_parts;
   int handle;
   char part_type[10];
   char zaid[24];
   float64 eout;

   strcpy(zaidlist[0], "d+t->n+a");
   strcpy(zaidlist[1], "d+he3->p+a");
   num_reac = 2;

   ierr = NDI2_init(&handle);
   if ( ierr != 0 ) {
      return(0);
   }


  printf("\n\n  NDI TEST SUITE -- testtn4\n\n");

 
   /*  TEST OF INTERPOLATION AND INTEGRATION -- TN DATA  */
   printf("\n*******************************\n     TN DATA\n*******************************\n\n");

   ierr = NDI2_set_option(handle, NDI_LIB_TYPE, "tn" );
   if ( ierr != 0 ) printf("  Error setting lib_type is %d.\n\n", ierr );
   ierr = NDI2_set_option(handle, NDI_LIBRARY, "testlib_tn" );
   if ( ierr != 0 ) printf("  Error setting library is %d.\n\n", ierr );

   /* Set energy bins--for energy distribution--to 30 groups */
   ierr = NDI2_set_option(handle, NDI_COLLAPSE, "30_group" );


   for ( i = 0; i < num_reac; i++ ) {
     printf("\n\n**************************************************************\n");
     printf("                  REACTION  %s\n",zaidlist[i] );
     printf("**************************************************************\n");
     ierr = NDI2_set_isotope(handle, zaidlist[i] );
     if ( ierr != 0 ) printf("  Error setting isotope %s is %d.\n",  "d+t->n+a", ierr );
     ierr = NDI2_get_string_val(handle, NDI_ZAID, zaid, 24);
     printf("Zaid %s (error = %d).\n", zaid, ierr );
     
     
     /* Secondary particle info */
     printf("\n***\n");
     
     /* Set temperature (in MeV) */
     ierr = NDI2_set_option(handle, NDI_TEMP, "1.0e-4" );
     if ( ierr != 0 ) printf("  Error setting temperature is %d.\n\n", ierr );
     
     ierr = NDI2_get_int_val(handle, NDI_NUM_SEC_PARTS, &num_sec_parts );
     printf("Num secondary particles = %d; err = %d\n", num_sec_parts, ierr );
     for ( j = 0; j < num_sec_parts; j++ ) {
       ierr = NDI2_get_string_val_n(handle, NDI_SEC_PART_TYPES, j, part_type, 10 );
       printf("Particle %d is a %s.  (err=%d)\n", j, part_type, ierr );
       NDI2_set_option(handle, NDI_CURR_PART, part_type );
       ierr = NDI2_get_float64_val(handle, NDI_EAVG_OUT, &eout );
       printf("   average energy for outgoing particle %s is:  %11.6f (err=%d).\n", 
	      part_type, eout,ierr);
       
       ierr = NDI2_get_float64_val(handle, NDI_REAC_Q, &eout );
       printf("   reaction Q is  %11.6f. (err=%d)\n", eout,ierr);
       /*  	 ierr = ndi_get_float64_vec( NDI_EDIST, n_edist, 200 );  */
       /* 	 printf( "   edist err = %d.\n", ierr ); */
       /* 	 for ( k = 0; k <= num_grps; k++ ) { */
       /* 	   printf ("%11.6f   ", n_edist[k]); */
       /* 	   if ( ( (k + 1) % 6 ) == 0 ) { */
       /* 	     printf("\n"); */
       /* 	   } */
       /* 	 } */
       
       printf("\n");
     }
     
     printf("\n*****************************\n\n\n");
   }
   
   ierr = NDI2_close(handle);
   if ( ierr != 0 ) printf("  Error closing NDI is %d.\n\n", ierr );

   return(0);
}
