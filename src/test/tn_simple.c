#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;


#ifdef __STANDALONE
int main ()
{
  int ierr = 0;

  ierr = tn_simple();
  return(ierr);
}
#endif  /* __STANDALONE */

int tn_simple(void)
{
   int ierr;
   int j,k;
   float64 q_value;
   int num_reactants;
   int  num_sigvbar;
   int reac_id[10], reac_mlt[10];
   int num_sec_parts;
   char part_type[10];
   char zaid[24];
   float64 temp[200], sigvbar[200], n_edist[200], eavg[200];
   int num_grps;

  printf("\n\n  NDI TEST SUITE -- testtn_simple\n\n");

   ierr = ndi_init();
   if ( ierr != 0 ) {
      return(0);
   }
   ierr = ndi_set_option( NDI_LIB_TYPE, "tn" );
   if ( ierr != 0 ) printf("  Error setting lib_type is %d.\n\n", ierr );

     /****** I would suggest you not set the library right now and use the defaults.
     To do this, just skip the ndi_set_option( NDI_LIBRARY,... ) step.

     The current set of released tn data is not very comprehensive; by not setting the
     library you will get the up-to-date data when it's available, and get not-as-good data
     when that's all there is.  New libraries are supposed to be released very soon.
     ************************/
   ierr = ndi_set_option( NDI_LIBRARY, "testtn" );
   if ( ierr != 0 ) printf("  Error setting library is %d.\n\n", ierr );

   /* Set energy bins--for energy distribution--to 30 groups */
   ierr = ndi_set_option( NDI_COLLAPSE, "30_group" );
   num_grps = 30;

   /* Get info for reaction d+t->n+a */
   ierr = ndi_set_isotope( "d+t->n+a" );
   if ( ierr != 0 ) printf("  Error setting reaction d+t->n+a is %d.\n", ierr );
   
      /* Again the next pieces of data may not be very useful to user codes,
	 but is available and demonstrated here.  */
      ierr = ndi_get_string_val( NDI_ZAID, zaid, 24);
      printf("Zaid %s (error = %d).\n", zaid, ierr );
      ierr = ndi_get_float64_val( NDI_REAC_Q, &q_value );
      printf("  Q-value=%lf; err=%d\n", q_value, ierr );
      ierr = ndi_get_int_val( NDI_NUM_REACTANTS, &num_reactants );
      printf("  Num reactants=%d; err=%d\n", num_reactants, ierr );
      ierr = ndi_get_int_vec( NDI_REACTANTS_ID, reac_id, 10 );
      if ( ierr != 0 ) printf("  Error getting id's is %d.\n", ierr );
      ierr = ndi_get_int_vec( NDI_REACTANTS_MLT, reac_mlt, 10 );
      if ( ierr != 0 ) printf("  Error getting multiplicities is %d.\n", ierr );
      for ( j = 0; j < num_reactants; j++ ) {
	 printf("  Number of particle %d is %d.\n", reac_id[j], reac_mlt[j] );
      }
      
      /*  Warning: in the next release the keyword NDI_NUM_TEMPS will be
	  changed to NDI_NUM_TEMPS  */
      ierr = ndi_get_int_val( NDI_NUM_TEMPS, &num_sigvbar );
      if ( ierr != 0 ) printf( "    num sigvbar err = %d.\n", ierr );
      printf("  There are %d temp,sigvbar pairs.\n", num_sigvbar); 
      ierr = ndi_get_float64_vec( NDI_TEMPS, temp, 200 );
      if ( ierr != 0 ) printf( "    temperature err = %d.\n", ierr );
      ierr = ndi_get_float64_vec( NDI_SIGVBARS, sigvbar, 200 );
      if ( ierr != 0 ) printf( "    sigvbars err = %d.\n", ierr );
      ierr = ndi_get_float64_vec( NDI_E_AVG, eavg, 200 );
      if ( ierr != 0 ) printf( "    avg. incident energy err = %d.\n", ierr );
      for ( j = 0; j < num_sigvbar; j++ ) {
	/*	printf("  Sigvbar = %e and Eavg = %e at temperature %e.\n", sigvbar[j], eavg[j], temp[j] );
	 */
      }
      

      /* Secondary particle info */
      printf("\n***\n");

      /* Energy distribution data */

      /* Find number of secondary particles for which energy-distribution data is given */
      ierr = ndi_get_int_val( NDI_NUM_SEC_PARTS, &num_sec_parts );
      printf("Num secondary particles = %d; err = %d\n", num_sec_parts, ierr );

      /* Set temperature (in MeV).
	 Energy distribution data will be data for the closest temperature available */
      /*  Note weirdness of temperature being passed in as a string */
      ierr = ndi_set_option( NDI_TEMP, "2.0e-4" );
      if ( ierr != 0 ) printf("  Error setting temperature is %d.\n\n", ierr );

      for ( j = 0; j < num_sec_parts; j++ ) {
	 ierr = ndi_get_string_val_n( NDI_SEC_PART_TYPES, j, part_type, 10 );
	 printf("Particle %d is a %s.  (err=%d)\n", j, part_type, ierr );
	 ndi_set_option( NDI_CURR_PART, part_type );
	 ierr = ndi_get_float64_vec( NDI_EDIST, n_edist, 200 );
	 printf( "   edist err = %d.\n", ierr );
	 for ( k = 0; k <= num_grps; k++ ) {
	   printf ("%11.6f   ", n_edist[k]);
	   if ( ( (k + 1) % 6 ) == 0 ) {
	     printf("\n");
	   }
	 }
	 printf("\n");
      }
 
   ierr = ndi_close();
   if ( ierr != 0 ) printf("  Error closing NDI is %d.\n\n", ierr );

   return(ierr);
}
