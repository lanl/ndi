#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"


void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );
void print_float32_vec( ndi_float32 *flt_ptr, int num_elements );

int main ()

{

  int ierr;
  int i, j;
  int num_sec_parts = 0;
  int num_zaid = 45;
  int vec_size;
  int str_size;
  char *curr_part;
  char info[133];
  ndi_float64 eavg_out;
  ndi_float64 *edist64;
  
  
  char *zaid_list[] = { "d+t->n+a.111ztn",
			"d+t->n+a.222ztn",
			"d+t->n+a.980ztn",
			"d+he3->p+a.980ztn",
			"d+d->n+he3.980ztn",
			"d+d->p+t.980ztn",
			"t+t->2n+a.980ztn",
			"t+li6->n+2a.980ztn",
			"d+t->n+a.981ztn",
			"d+t->n+a.982ztn",
			"d+t->n+a.983ztn",
			"d+t->n+a.984ztn",
			"d+t->n+a.985ztn",
			"d+t->n+a.986ztn",
			"d+he3->p+a.986ztn",
			"d+t->n+a.987ztn",
			"d+t->n+a.988ztn",
			"d+t->n+a.989ztn",
			"d+t->n+a.990ztn",
			"d+t->n+a.991ztn",
			"d+t->n+a.992ztn",
			"d+t->n+a.993ztn",
			"d+t->n+a.994ztn",
			"d+t->n+a.995ztn",
			"d+t->n+a.996ztn",
			"d+t->n+a.997ztn",
			"d+t->n+a.998ztn",
			"d+t->n+a.999ztn",
			"d+t->n+a.000ztn",
			"d+t->n+a.001ztn",
			"d+t->n+a.002ztn",
			"d+t->n+a.003ztn",
			"d+t->n+a.004ztn",
			"d+t->n+a.005ztn",
			"d+t->n+a.006ztn",
			"d+t->n+a.007ztn",
			"d+t->n+a.008ztn",
			"d+t->n+a.009ztn",
			"d+t->n+a.010ztn",
			"d+t->n+a.011ztn",
			"d+t->n+a.012ztn",
			"d+t->n+a.013ztn",
			"d+t->n+a.014ztn",
			"d+t->n+a.015ztn",
			"d+t->n+a.016ztn",
  };



   ierr = ndi_init();
   ierr = ndi_set_option( NDI_LIB_TYPE, "tn" );
   ierr = ndi_set_option( NDI_COLLAPSE, "16_group" );
   ierr = ndi_set_option( NDI_TEMP, "1.0e-4" );
   if (ierr) {
     printf("  **Error %d in setting temperature.\n", ierr );
   }

   /*  For all ZAIDs */
   for ( i = 0; i < num_zaid; i++ ) {
      
     /* Set isotope, no collapsing */
     printf("***********************REACTION=%s***********************\n\n",
	    zaid_list[i] );
     ierr = ndi_set_isotope(zaid_list[i]);
     if (ierr) {
       printf("  **Error %d in setting reaction ZAID.\n", ierr );
     }

     ierr = ndi_get_string_val( NDI_INFO, info, 133 );
     if (ierr) {
       printf("  **Error %d in getting info line.\n", ierr );
     }
     printf("***  %s*************************************************************************\n\n", 
	    info );
     
     ierr = ndi_get_int_val( NDI_NUM_SEC_PARTS, &num_sec_parts );
     if (ierr) {
       printf("  **Error %d in getting number of secondary particles.\n", ierr );
     }
     for( j = 0; j < num_sec_parts; j++ ) {
       str_size = ndi_get_size_n(  NDI_SEC_PART_TYPES, j, &ierr );
       if ( ( str_size < 1 ) || ( ierr != 0 ) ) {
	 printf("  **Error %d in getting size of sec. particle id (%d).\n", ierr, str_size );
       }
       curr_part = (char *)malloc( str_size * sizeof(char) );
       ierr = ndi_get_string_val_n( NDI_SEC_PART_TYPES, j, curr_part, str_size );
       if (ierr) {
	 printf("  **Error %d in getting secondary particle types.\n", ierr );
       }
       printf("*******Particle %s:\n", curr_part );
       ierr = ndi_set_option( NDI_CURR_PART, curr_part );
       if (ierr) {
	 printf("  **Error %d in setting current particle to %s.\n", ierr, curr_part );
       }


       /*********** SECONDARY PARTICLE ENERGY DISTRIBUTION ***********/

       vec_size = ndi_get_size( NDI_EDIST, &ierr );
       if ( ( vec_size < 1 ) || ( ierr != 0 ) ) {
	 printf("  **Error %d in getting size of edist (%d).\n", ierr, vec_size );
       }
       printf ("      Vector size is:  %d\n", vec_size );
       edist64 = (ndi_float64 *)malloc( vec_size * sizeof(ndi_float64) );
       ierr = ndi_get_float64_vec( NDI_EDIST, edist64, vec_size );
       if (ierr) {
	 printf("  **Error %d in energy distribution -- returned value for particle %s is:\n", 
		ierr, curr_part );
       }
       print_float64_vec( edist64, vec_size);	 
       if ( curr_part ) { free( curr_part ); }
       if ( edist64 ) { free( edist64 ); }

       /*********** SECONDARY PARTICLE AVERAGE OUTGOING ENERGY ***********/

       ierr = ndi_get_float64_val( NDI_EAVG_OUT, &eavg_out );
       printf("\nAverage particle energy is:  %e\n\n", eavg_out );

     }

     /* Try a alpha to check what happens for those reactions without secondary alphas */
       printf("*******Last particle alpha:\n"  );
       ierr = ndi_set_option( NDI_CURR_PART, "alpha" );
       if (ierr) {
	 printf("  **Error %d in setting current particle to alpha.\n", ierr );
       }

       /*********** SECONDARY PARTICLE ENERGY DISTRIBUTION ***********/

       vec_size = ndi_get_size( NDI_EDIST, &ierr );
       if ( ( vec_size <= 0 ) || (  ierr != 0 ) ) {
	 printf("  **Error %d in getting size of edist (%d).\n", ierr, vec_size );
       }
       printf ("      Vector size is:  %d\n", vec_size );
       edist64 = (ndi_float64 *)malloc( vec_size * sizeof(ndi_float64) );
       ierr = ndi_get_float64_vec( NDI_EDIST, edist64, vec_size );
       if (ierr) {
	 printf("  **Error %d in energy distribution -- returned value for particle alpha is:\n", 
		ierr );
       }
       print_float64_vec( edist64, vec_size );	 
       if ( edist64 ) { free( edist64 ); }
     
       /*********** SECONDARY PARTICLE AVERAGE OUTGOING ENERGY ***********/

       ierr = ndi_get_float64_val( NDI_EAVG_OUT, &eavg_out );
       printf("\nAverage particle energy is:  %e\n\n", eavg_out );


   }  

   ierr = ndi_close();
   
   return(0);
}

/**********************************************************************/

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 4 ) == 0) )
	 printf("\n");
      printf("%22.15e  ", flt64_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/
