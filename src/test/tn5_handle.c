#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;

int testtn5(void);

#ifdef __STANDALONE
int main (void)
{
  int ierr = 0;

  ierr = testtn5();
  return(ierr);
}
#endif  /* __STANDALONE */


int testtn5(void)
{
   typedef char tn_zaid[30];
   
   tn_zaid zaidlist[50];
   int ierr;
   int num_reac;
   int isize;
   char tn_info[120];
   int i,j;
   int gendir_h;
   int handle;
   float64 q_value;
   int num_sec_parts;
   char part_type[10];
   char zaid[24];
   float64 n_edist[200];
   float64 q;

   ierr = ndi2_open_gendir(&gendir_h, "");
   if ( ierr != 0 ) {
      return(0);
   }

   ierr = ndi2_get_handle(gendir_h, &handle);
   if( ierr != 0) printf("  Error %d getting particle handle.\n", ierr);


  printf("\n\n  NDI TEST SUITE -- testtn5\n\n");


   /* TN REACTION DATA */

   /* Set NDI TN reaction set */
   ndi2_set_option(handle, NDI_LIB_TYPE, "tnreactions" );
   if ( ierr != 0 ) printf("    Error setting lib_type is %d.\n", ierr );
   ierr = ndi2_set_isotope(handle, "testlib_tn_list.001ztr" );
   if ( ierr != 0 ) printf("    Error setting zaid is %d.\n", ierr );

   /* Getting the zaid would only by of much interest if the ZAID were
      defaulted by the NDI in some way.  In this case, it's just here
      to demonstrate how it works */
   ierr = ndi2_get_string_val(handle, NDI_ZAID, zaid, 24);    
   if ( ierr != 0 ) printf("    Error = %d getting zaid %s.\n", ierr, zaid );

   /* Comment from library regarding this TN reaction set */
   ierr = ndi2_get_string_val(handle, NDI_INFO, tn_info, 120);
   if ( ierr != 0 ) printf("%s    Info err = %d.\n", tn_info, ierr );



   /*  Determine the number of reactions in this set then
       loop over each reaction */
   printf("    Error = %d getting zaid %s.\n", ierr, zaid );
   ierr = ndi2_get_int_val(handle, NDI_NUM_REAC, &num_reac );
   printf("Num reac = %d; err = %d\n", num_reac, ierr );
   for ( i = 0; i < num_reac; i++ ) {
     isize = ndi2_get_size_n(handle, NDI_LIST_REAC, i, &ierr );
     ierr = ndi2_get_string_val_n(handle, NDI_LIST_REAC, i, zaidlist[i], 30 );
     printf( "Reaction %d: >%s<  (size = %d)\n", i, zaidlist[i], isize );
     if ( ierr != 0 ) printf( "    list reaction err = %d.\n", ierr );
     
   }

 
   /*  NEW TN DATA  */
   printf("\n*******************************\n     TN DATA\n*******************************\n\n");

   /* Change now to Hale98 TN data */
   ierr = ndi2_set_option(handle, NDI_LIB_TYPE, "tn" );
   if ( ierr != 0 ) printf("  Error setting lib_type is %d.\n\n", ierr );
   ierr = ndi2_set_option(handle, NDI_LIBRARY, "testlib_tn" );
   if ( ierr != 0 ) printf("  Error setting library is %d.\n\n", ierr );

   /* Set energy bins--for energy distribution--to 30 groups */
   ierr = ndi2_set_option(handle, NDI_COLLAPSE, "30_group" );


   /* For each reaction in the "big5" set that was collected above,
      get all available TN data */
   for ( i = 0; i < num_reac; i++ ) {
      ierr = ndi2_set_isotope(handle, zaidlist[i] );
      if ( ierr != 0 ) printf("  Error setting isotope %s is %d.\n", zaidlist[i], ierr );
      ierr = ndi2_get_string_val(handle, NDI_ZAID, zaid, 24);
      printf("Zaid %s (error = %d).\n", zaid, ierr );
      ierr = ndi2_get_float64_val(handle, NDI_REAC_Q, &q_value );
      printf("  Q-value=%lf; err=%d\n", q_value, ierr );
     
     

      /* Secondary particle info */
      printf("\n***\n");

      /* Set temperature (in MeV) */
      ierr = ndi2_set_option(handle, NDI_TEMP, "1.0e-4" );
      if ( ierr != 0 ) printf("  Error setting temperature is %d.\n\n", ierr );

      ierr = ndi2_get_int_val(handle, NDI_NUM_SEC_PARTS, &num_sec_parts );
      printf("Num secondary particles = %d; err = %d\n", num_sec_parts, ierr );
      for ( j = 0; j < num_sec_parts; j++ ) {
	 ierr = ndi2_get_string_val_n(handle, NDI_SEC_PART_TYPES, j, part_type, 10 );
	 printf("Particle %d is a %s.  (err=%d)\n", j, part_type, ierr );
	 ndi2_set_option(handle, NDI_CURR_PART, part_type );
	 ierr = ndi2_get_float64_val(handle, NDI_REAC_Q, &q );

 	 ierr = ndi2_get_float64_vec(handle, NDI_EDIST, n_edist, 200 ); 
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

   ierr = ndi2_close_gendir(gendir_h);
   if ( ierr != 0 ) printf("  Error closing NDI is %d.\n\n", ierr );
   
   return(0);
}
