#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;

int update_inventory( int part_type, int num_parts, char *proddep_flag );
int particle_is_transportable ( int part_type );
int construct_source_for_transport( float64 *edist, float64 reac_rate, int mult, int num_grps );

int main ()

{
   int ierr;
   int j;
   int num_reactants, num_rprods;
   int reac_id[10], reac_mlt[10];
   int num_grps;
   int num_parts;
   int part_multiplicity;
   int multiplicity_vec[20];
   int rprod_id[20];
   int part_type;
   int gendir_h;
   int handle;
   char part_type_str[10];
   float64 reaction_rate = 1.0;
   float64 q_value;
   float64 edist[200];
   float64 avg_part_E;
   float64 local_E_dep = 0;

   ierr = NDI2_open_gendir(&gendir_h, "");
   if ( ierr != 0 ) {
      return(0);
   }
   ierr = NDI2_make_handle(gendir_h, "tn", &handle);
   if( ierr != 0) printf("  Error %d making particle handle.\n", ierr);

   ierr = NDI2_set_option(handle, NDI_LIBRARY, "hale98" );

   /* Set energy bins--for energy distribution--to 30 groups */
   ierr = NDI2_set_option(handle, NDI_COLLAPSE, "30_group" );
   num_grps = 30;


   /* Set temperature (in MeV).
      Energy distribution data will be data for the closest temperature available */
      /*  Note weirdness of temperature being passed in as a string */
      ierr = NDI2_set_option(handle, NDI_TEMP, "2.0e-4" );
      if ( ierr != 0 ) printf("  Error setting temperature is %d.\n\n", ierr );


   /* Get info for reaction d+t->n+a */
   ierr = NDI2_set_isotope(handle, "d+t->n+a" );
   if ( ierr != 0 ) printf("  Error setting reaction d+t->n+a is %d.\n", ierr );
   
 
   /* Again the next pieces of data may not be very useful to user codes,
      but is available and demonstrated here.  */
   ierr = NDI2_get_float64_val( handle, NDI_REAC_Q, &q_value );
   printf("  Q-value=%lf; err=%d\n", q_value, ierr );

   /* Reactant inventory update */
   ierr = NDI2_get_int_val(handle, NDI_NUM_REACTANTS, &num_reactants );
   printf("  Num reactants=%d; err=%d\n", num_reactants, ierr );
   ierr = NDI2_get_int_vec(handle, NDI_REACTANTS_ID, reac_id, 10 );
   if ( ierr != 0 ) printf("  Error getting id's is %d.\n", ierr );
   ierr = NDI2_get_int_vec(handle, NDI_REACTANTS_MLT, reac_mlt, 10 );
   if ( ierr != 0 ) printf("  Error getting multiplicities is %d.\n", ierr );
   for ( j = 0; j < num_reactants; j++ ) {
     num_parts = reaction_rate * reac_mlt[j];
     ierr = update_inventory( reac_id[j], num_parts, "deplete" );
     printf("  Number of particle %d is %d.\n", reac_id[j], reac_mlt[j] );
   }
 


   /* Energy distribution data */

   /* Find number of secondary particles for which energy-distribution data is given */
/*    ierr = ndi_get_int_val( NDI_NUM_SEC_PARTS, &num_sec_parts ); */
/*    printf("Num secondary particles = %d; err = %d\n", num_sec_parts, ierr ); */

   ierr = NDI2_get_int_val(handle, NDI_NUM_RPRODS, &num_rprods );
   ierr = NDI2_get_int_vec(handle, NDI_RPRODS_ID, rprod_id, 20 );
   ierr = NDI2_get_int_vec(handle, NDI_RPRODS_MLT, multiplicity_vec, 20 );
   if ( ierr != 0 ) {};

   for ( j = 0; j < num_rprods; j++ ) {
/*      ierr = ndi_get_string_val_n( NDI_SEC_PART_TYPES, j, part_type, 10 ); */
     part_type = rprod_id[j];
     sprintf(part_type_str, "%d", part_type);
     part_multiplicity = multiplicity_vec[j];     
     printf("Particle %d is a %d.  (err=%d)\n", j, part_type, ierr );
     NDI2_set_option( handle, NDI_CURR_PART, part_type_str );
     if ( particle_is_transportable(part_type) ) {
       /* Collect energy distribution for to send to transport package */
       ierr = NDI2_get_float64_vec(handle, NDI_EDIST, edist, 200 );
       if ( ierr != 0 ) {
	 printf( "   edist err for particle %s = %d.\n", part_type_str, ierr );
	 continue;
       }
       ierr = construct_source_for_transport( edist, reaction_rate, part_multiplicity, num_grps );
     } else {
       /* Deposit particle energy locally */
       ierr = NDI2_get_float64_val(handle, NDI_EOUTBAR, &avg_part_E );
       if ( ierr != 0 ) printf( "   ebarout err for particle %d = %d.\n", part_type, ierr );
       num_parts = reaction_rate * part_multiplicity;
       local_E_dep += num_parts * avg_part_E;
       printf("local_E_dep is %e.\n", local_E_dep);
       /* Update reaction product inventory */
       ierr = update_inventory( part_type, num_parts, "produce" );
     }
   }
 
   ierr = NDI2_close_gendir(gendir_h);
   if ( ierr != 0 ) printf("  Error closing NDI is %d.\n\n", ierr );
   return(ierr);
}

int update_inventory( int part_type, int num_parts, char *proddep_flag ) {

  int ierr = 0;

  if ( strncmp( proddep_flag, "produce", strlen(proddep_flag) + 1) == 0 ) {
    /* Add to particle inventory */
    printf("  Adding %d particles/s to particle %d.\n", num_parts, part_type );
  } else {
    /* Subtract from particle inventory */
    printf("  Subtracting %d particles/s from particle %d.\n", num_parts, part_type );
  }
  return(ierr);
}


int particle_is_transportable ( int part_type ) {

  int i;
  int ret_value = 0;                          /* Default is not transportable */
  int num_of_transportable_parts = 1;
  int particles_to_transport[] = {1};     /* Currently transport only neutrons */

  for ( i = 0; i < num_of_transportable_parts; i++ ) {
    if ( particles_to_transport[i] == part_type ) {
      ret_value = -1;
      break;
    }
  }

  return( ret_value );
}

int construct_source_for_transport( float64 *edist, float64 reac_rate, int mult, int num_grps ) {
  
  int ierr = 0;
  int i;
  float64 *p = edist;

  /* Whatever needs to be sent to transport package ???? */
  for ( i = 0; i < num_grps; i++ ) {
    *p *= reac_rate * mult;
    p++;
  }
  /* Send to transport package? */
  return( ierr );
}
