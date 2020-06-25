#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;

int testtn_new(void);

#ifdef NDI_STANDALONE
int main (void)
{
  int ierr = 0;

  ierr = testtn_new();
  return(ierr);
}
#endif  /* NDI_STANDALONE */

int testtn_new(void)
{
   typedef char tn_zaid[30];
   
   tn_zaid zaidlist[50];
   int ierr;
   int num_reac;
   char tn_info[120];
   int i,j,k;
   int gendir_h;
   int handle;
   float64 q_value;
   int num_reactants, num_rprods;
   int  num_sigvbar;
   int id[50], mlt[50];
   int reac_id[10], reac_mlt[10];
   int num_sec_parts;
   char part_type[10];
   char zaid[24];
   float64 temp[200], sigvbar[200], n_edist[200], eavg[200];
   int num_grps;
   int num_it;

   ierr = NDI2_open_gendir(&gendir_h, "");
   if ( ierr != 0 ) {
      return(0);
   }

    ierr = NDI2_make_handle(gendir_h, "tnreactions", &handle);
    if( ierr != 0) printf("  Error %d making particle handle.\n", ierr);

  printf("\n\n  NDI TEST SUITE -- testtn_new\n\n");


   /* TN REACTION DATA */

   /* Set NDI for "big5" TN reaction set */
   ierr = NDI2_set_isotope(handle, "testlib_tn_list" );
   if ( ierr != 0 ) printf("    Error setting zaid is %d.\n", ierr );

   /* Getting the zaid would only by of much interest if the ZAID were
      defaulted by the NDI in some way.  In this case, it's just here
      to demonstrate how it works */
   ierr = NDI2_get_string_val(handle, NDI_ZAID, zaid, 24);    
   if ( ierr != 0 ) printf("    Error = %d getting zaid %s.\n", ierr, zaid );

   /* Comment from library regarding this TN reaction set */
   ierr = NDI2_get_string_val(handle, NDI_INFO, tn_info, 120);
   if ( ierr != 0 ) printf("%s    Info err = %d.\n", tn_info, ierr );



   /*  Determine the number of reactions in this set then
       loop over each reaction */
   printf("    Error = %d getting zaid %s.\n", ierr, zaid );
   ierr = NDI2_get_int_val(handle, NDI_NUM_REAC, &num_reac );
   printf("Num reac = %d; err = %d\n", num_reac, ierr );
   for ( i = 0; i < num_reac; i++ ) {

     /* Get the ith reaction zaid */
      ierr = NDI2_get_string_val_n( handle, NDI_LIST_REAC, i, zaidlist[i], 30 );
      printf( "Reaction %d: %s\n", i, zaidlist[i] );
      if ( ierr != 0 ) printf( "    list reaction err = %d.\n", ierr );


      /* This information will probably not be used by a host code; but
	 info about the reactants and reaction products in each reaction
	 is available, and accessing this info is demonstrated here. */
      ierr = NDI2_get_int_val_n(handle, NDI_NUM_REACTANTS, i, &num_reactants );
      if ( ierr != 0 ) printf( "    num reactants err = %d.\n", ierr );
      ierr = NDI2_get_int_val_n(handle, NDI_NUM_RPRODS, i, &num_rprods );
      if ( ierr != 0 ) printf( "    num rprods err = %d.\n", ierr );
      printf( "  Number of reactants = %d; number of rprods = %d\n",  num_reactants, num_rprods );
      ierr = NDI2_get_int_vec_n(handle, NDI_REACTANTS_ID, i, id, 50 );
      if ( ierr != 0 ) printf( "    id err = %d; ", ierr); 
      ierr = NDI2_get_int_vec_n(handle, NDI_REACTANTS_MLT, i, mlt, 50 );
      if ( ierr != 0 ) printf( "mlt err = %d\n", ierr); 
      for ( j = 0; j < num_reactants; j++ ) {
	 printf("  Reactant %d multiplicity is %d.\n", id[j], mlt[j] );
      }
      ierr = NDI2_get_int_vec_n(handle, NDI_RPRODS_ID, i, id, 50 );
      if ( ierr != 0 ) printf( "    id err = %d; ", ierr); 
      ierr = NDI2_get_int_vec_n(handle, NDI_RPRODS_MLT, i, mlt, 50 );
      if ( ierr != 0 ) printf( "mlt err = %d\n", ierr); 
      for ( j = 0; j < num_rprods; j++ ) {
	 printf("  Rprod %d multiplicity is %d.\n", id[j], mlt[j] );
      }
   }


   /* Print the TN reaction zaids obtained above. */
   for ( i = 0; i < num_reac; i++ ) {
      printf("zaid %d:  %s\n", i, zaidlist[i] );
   }
   

   /*  TN DATA for the reaction zaids obtained above */
   printf("\n*******************************\n     TN DATA\n*******************************\n\n");

   /* Change now to Hale98 TN data */
   ierr = NDI2_set_option(handle, NDI_LIB_TYPE, "tn" );
   if ( ierr != 0 ) printf("  Error setting lib_type is %d.\n\n", ierr );
/*    ierr = ndi_set_option( NDI_LIBRARY, "testlib_tn" ); */
   if ( ierr != 0 ) printf("  Error setting library is %d.\n\n", ierr );

   /* Set energy bins--for energy distribution--to 30 groups */
   ierr = NDI2_set_option(handle, NDI_COLLAPSE, "30_group" );
   num_grps = 30;


   /* For each reaction in the "big5" set that was collected above,
      get all available TN data */
   for ( i = 0; i < num_reac; i++ ) {
     /* Set the reaction */
      ierr = NDI2_set_isotope(handle, zaidlist[i] );
      if ( ierr != 0 ) printf("  Error setting isotope %s is %d.\n", zaidlist[i], ierr );

      /* Again the next pieces of data may not be very useful to user codes,
	 but is available and demonstrated here.  */
      ierr = NDI2_get_string_val( handle,NDI_ZAID, zaid, 24);
      printf("Zaid %s (error = %d).\n", zaid, ierr );
      ierr = NDI2_get_float64_val(handle, NDI_REAC_Q, &q_value );
      printf("  Q-value=%lf; err=%d\n", q_value, ierr );
      ierr = NDI2_get_int_val(handle, NDI_NUM_REACTANTS, &num_reactants );
      printf("  Num reactants=%d; err=%d\n", num_reactants, ierr );
      ierr = NDI2_get_int_vec(handle, NDI_REACTANTS_ID, reac_id, 10 );
      if ( ierr != 0 ) printf("  Error getting id's is %d.\n", ierr );
      ierr = NDI2_get_int_vec(handle, NDI_REACTANTS_MLT, reac_mlt, 10 );
      if ( ierr != 0 ) printf("  Error getting multiplicities is %d.\n", ierr );
      for ( j = 0; j < num_reactants; j++ ) {
	 printf("  Number of particle %d is %d.\n", reac_id[j], reac_mlt[j] );
      }
      
      /*  Warning: in the next release the keyword NDI_NUM_TEMPS will be
	  changed to NDI_NUM_TEMPS  */
      ierr = NDI2_get_int_val(handle, NDI_NUM_TEMPS, &num_sigvbar );
      if ( ierr != 0 ) printf( "    num sigvbar err = %d.\n", ierr );
      printf("  There are %d temp,sigvbar pairs.\n", num_sigvbar); 
      ierr = NDI2_get_float64_vec(handle, NDI_TEMPS, temp, 200 );
      if ( ierr != 0 ) printf( "    temperature err = %d.\n", ierr );
      ierr = NDI2_get_float64_vec(handle, NDI_SIGVBARS, sigvbar, 200 );
      if ( ierr != 0 ) printf( "    sigvbars err = %d.\n", ierr );
      ierr = NDI2_get_float64_vec(handle, NDI_E_AVG, eavg, 200 );
      if ( ierr != 0 ) printf( "    avg. incident energy err = %d.\n", ierr );
      for ( j = 0; j < num_sigvbar; j++ ) {
	/*	printf("  Sigvbar = %e and Eavg = %e at temperature %e.\n", sigvbar[j], eavg[j], temp[j] );
	 */
      }
      
     

      /* Secondary particle info */
      printf("\n***\n");

      /* Energy distribution data */

      /* Find number of secondary particles for which energy-distribution data is given */
      ierr = NDI2_get_int_val(handle, NDI_NUM_SEC_PARTS, &num_sec_parts );
      printf("Num secondary particles = %d; err = %d\n", num_sec_parts, ierr );

      /* Set temperature (in MeV).
	 Energy distribution data will be data for the closest temperature available */
      /*  Note weirdness of temperature being passed in as a string */
      ierr = NDI2_set_option(handle, NDI_TEMP, "2.0e-4" );
      if ( ierr != 0 ) printf("  Error setting temperature is %d.\n\n", ierr );

      for ( j = 0; j < num_sec_parts; j++ ) {
	 ierr = NDI2_get_string_val_n(handle, NDI_SEC_PART_TYPES, j, part_type, 10 );
	 printf("Particle %d is a %s.  (err=%d)\n", j, part_type, ierr );
	 NDI2_set_option(handle, NDI_CURR_PART, part_type );
	 ierr = NDI2_get_float64_vec(handle, NDI_EDIST, n_edist, 200 );
	 printf( "   edist err = %d.\n", ierr );
	 for ( k = 0; k <= num_grps; k++ ) {
	   printf ("%11.6f   ", n_edist[k]);
	   if ( ( (k + 1) % 6 ) == 0 ) {
	     printf("\n");
	   }
	 }
	 printf("\n");
         /* Test for Requirement 12 */
         num_it = NDI2_get_size(handle, NDI_EDIST_TEMPS, &ierr);
         if (ierr != 0) {
              printf("  Error getting size of temp is %d.\n\n", ierr);
              return (ierr);
         }
         printf("There are %d temp, edist pairs.\n", num_it);
         ierr = NDI2_get_float64_vec(handle, NDI_EDIST_TEMPS, temp, 200);
         if (ierr != 0) {
              printf("  Error getting array of temps is %d.\n\n", ierr);
              return (ierr);
         }
         printf("edist_temps err = %d\n", ierr);
         for (k = 0; k < num_it; k++) {
              printf("%11.6f   ", temp[k]);
              if (((k+1)%6) == 0) printf("\n");
         }
         printf("\n");
         num_it = NDI2_get_size(handle, NDI_EDIST_INTERP_TYPE, &ierr);
         if (ierr != 0) {
              printf("  Error getting size of interp_type is %d.\n\n", ierr);
              return (ierr);
         }
         printf("There are %d edist interpolation types.\n", num_it);
         ierr = NDI2_get_int_vec(handle, NDI_EDIST_INTERP_TYPE, id, 50);
         if (ierr != 0) {
              printf("  Error getting array of interp_type is %d.\n\n", ierr);
              return (ierr);
         }
         printf("edist_interp_type err = %d.\n", ierr);
         for (k = 0; k < num_it; k++) printf("%6d   ", id[k]);
         printf("\n");
         num_it = NDI2_get_size(handle, NDI_EDIST_INTERP_REG, &ierr);
         if (ierr != 0) {
              printf("  Error getting size of interp_reg is %d.\n\n", ierr);
              return (ierr);
         }
         printf("There are %d edist interpolation regions.\n", num_it);
         ierr = NDI2_get_int_vec(handle, NDI_EDIST_INTERP_REG, id, 50);
         if (ierr != 0) {
              printf("  Error getting array of interp_reg is %d.\n\n", ierr);
              return (ierr);
         }
         printf("edist_interp_reg err = %d.\n", ierr);
         for (k = 0; k < num_it; k++) printf("%6d   ", id[k]);
         printf("\n");

      }
      
      printf("\n*****************************\n\n\n");
   
   }

   ierr = NDI2_close_gendir(gendir_h);
   if ( ierr != 0 ) printf("  Error closing NDI is %d.\n\n", ierr );

   return(ierr);
}
