#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ndi.h"

typedef float float32;
typedef double float64;

#include "ndi_internal.h"
#include "ndi_state.h"

static int setup_integ( void );

int testtn2(void)
{
   typedef char tn_zaid[30];
   
   tn_zaid zaidlist[50];
   int ierr;
   double x1,x2,x,y1,y2,ynew;


   strcpy(zaidlist[0], "d+t->n+a");
   strcpy(zaidlist[1], "d+he3->p+a");

   ierr = ndi_init();
   if ( ierr != 0 ) {
      return(0);
   }


  printf("\n\n  NDI TEST SUITE -- testtn2\n\n");

   /*  TEST OF INTERPOLATION AND INTEGRATION -- TN DATA  */

   /* Change now to Hale98 TN data */
   ierr = ndi_set_option( NDI_LIB_TYPE, "tn" );
   if ( ierr != 0 ) printf("  Error setting lib_type is %d.\n\n", ierr );
   ierr = ndi_set_option( NDI_LIBRARY, "testint" );
   if ( ierr != 0 ) printf("  Error setting library is %d.\n\n", ierr );

   /* Set energy bins--for energy distribution--to 30 groups */
   ierr = ndi_set_option( NDI_COLLAPSE, "30_group" );


   /*  LIN-LIN INTERPOLATION TESTS  */
   printf("LIN-LIN INTERPOLATION TESTS\n");
   /* Lower bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 1.0;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Upper bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.0;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Midpoint */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 1.5;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Within range */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 1.8;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Below lower bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 0.5;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );
   
   /* Above upper bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.5;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" x1 > x2\n");
   /* x1 > x2 -- Lower bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.0;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", x1
	  , y1, x2, y2, x, ynew );

   /* x1 > x2 -- Upper bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 3.0;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Midpoint */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.5;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Within range */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.8;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Below lower bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 0.5;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );
   
   /* x1 > x2 -- Above upper bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 3.5;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" decreasing function\n");
   /* Decreasing function -- midpoint */
   x1= 1.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 1.5;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Decreasing function -- in range */
   x1= 1.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 1.8;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Decreasing function; x1 > x2 -- Midpoint */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 2.5;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Decreasing function; x1 > x2 -- Within range */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 2.8;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" x1 = x2 \n");
   /*  x1= x2  */
   x1= 3.0;   y1= 10.0;   x2= 3.0;   y2= 5.0;   x= 3.0;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1 = x2; y1 = y2; x=x1=x2  */
   x1= 3.0;   y1= 5.0;   x2= 3.0;   y2= 5.0;   x= 3.0;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1 = x2; y1 != y2; x=x1=x2  */
   x1= 3.0;   y1= 10.0;   x2= 3.0;   y2= 5.0;   x= 3.0;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1= x2; y1 = y2; x!=x1  */
   x1= 3.0;   y1= 5.0;   x2= 3.0;   y2= 5.0;   x= 2.0;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1= x2; y1 != y2; x!=x1  */
   x1= 3.0;   y1= 10.0;   x2= 3.0;   y2= 5.0;   x= 2.0;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" y1 = y2\n");
   /* y1 = y2; x1 < x2 -- midpoint  */
   x1= 3.0;   y1= 10.0;   x2= 5.0;   y2= 10.0;   x= 4.0;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* y1 = y2; x1 < x2 -- outside range */
   x1= 3.0;   y1= 10.0;   x2= 5.0;   y2= 10.0;   x= 7.0;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* y1 = y2; x1 > x2 -- in range */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 10.0;   x= 2.8;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* y1 = y2; x1 > x2 -- outside range */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 10.0;   x= 7.0;
   ynew = (double)lin_lin_interp(x,x1,y1,x2,y2);
   printf(" LIN_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );


   /*  LOG-LOG INTERPOLATION TESTS  */
   printf("\nLOG-LOG INTERPOLATION TESTS\n");
   /* Lower bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 1.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Upper bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Midpoint */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 1.5;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Within range */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 1.8;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Below lower bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 0.5;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );
   
   /* Above upper bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.5;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" x1 > x2\n");
   /* x1 > x2 -- Lower bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Upper bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 3.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Midpoint */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.5;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Within range */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.8;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Below lower bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 0.5;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );
   
   /* x1 > x2 -- Above upper bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 3.5;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" decreasing function\n");
   /* Decreasing function -- midpoint */
   x1= 1.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 1.5;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Decreasing function -- in range */
   x1= 1.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 1.8;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Decreasing function; x1 > x2 -- Midpoint */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 2.5;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Decreasing function; x1 > x2 -- Within range */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 2.8;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );
   printf(" x1 = x2 \n");
   /*  x1= x2  */
   x1= 3.0;   y1= 10.0;   x2= 3.0;   y2= 5.0;   x= 3.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1 = x2; y1 = y2; x=x1=x2  */
   x1= 3.0;   y1= 5.0;   x2= 3.0;   y2= 5.0;   x= 3.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1 = x2; y1 != y2; x=x1=x2  */
   x1= 3.0;   y1= 10.0;   x2= 3.0;   y2= 5.0;   x= 3.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1= x2; y1 = y2; x!=x1  */
   x1= 3.0;   y1= 5.0;   x2= 3.0;   y2= 5.0;   x= 2.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1= x2; y1 != y2; x!=x1  */
   x1= 3.0;   y1= 10.0;   x2= 3.0;   y2= 5.0;   x= 2.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" y1 = y2\n");
   /* y1 = y2; x1 < x2 -- midpoint  */
   x1= 3.0;   y1= 10.0;   x2= 5.0;   y2= 10.0;   x= 4.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* y1 = y2; x1 < x2 -- outside range */
   x1= 3.0;   y1= 10.0;   x2= 5.0;   y2= 10.0;   x= 7.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* y1 = y2; x1 > x2 -- in range */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 10.0;   x= 2.8;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* y1 = y2; x1 > x2 -- outside range */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 10.0;   x= 7.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" y1 = 0\n");
   /* y1 = y2; x1 < x2 -- midpoint  */
   x1= 3.0;   y1= 0.0;   x2= 5.0;   y2= 10.0;   x= 4.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" x1 = 0\n");
   /* y1 = y2; x1 < x2 -- midpoint  */
   x1= 0.0;   y1= 5.0;   x2= 5.0;   y2= 10.0;   x= 4.0;
   ynew = (double)log_log_interp(x,x1,y1,x2,y2);
   printf(" LOG_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );


   /*  LOG-LIN INTERPOLATION TESTS  */
   printf("\nLOG-LIN INTERPOLATION TESTS\n");
   /* Lower bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 1.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Upper bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Midpoint */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 1.5;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Within range */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 1.8;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Below lower bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 0.5;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );
   
   /* Above upper bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.5;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" x1 > x2\n");
   /* x1 > x2 -- Lower bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Upper bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 3.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Midpoint */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.5;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Within range */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.8;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Below lower bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 0.5;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );
   
   /* x1 > x2 -- Above upper bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 3.5;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" decreasing function\n");
   /* Decreasing function -- midpoint */
   x1= 1.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 1.5;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Decreasing function -- in range */
   x1= 1.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 1.8;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Decreasing function; x1 > x2 -- Midpoint */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 2.5;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Decreasing function; x1 > x2 -- Within range */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 2.8;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );
   printf(" x1 = x2 \n");
   /*  x1= x2  */
   x1= 3.0;   y1= 10.0;   x2= 3.0;   y2= 5.0;   x= 3.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1 = x2; y1 = y2; x=x1=x2  */
   x1= 3.0;   y1= 5.0;   x2= 3.0;   y2= 5.0;   x= 3.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1 = x2; y1 != y2; x=x1=x2  */
   x1= 3.0;   y1= 10.0;   x2= 3.0;   y2= 5.0;   x= 3.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1= x2; y1 = y2; x!=x1  */
   x1= 3.0;   y1= 5.0;   x2= 3.0;   y2= 5.0;   x= 2.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1= x2; y1 != y2; x!=x1  */
   x1= 3.0;   y1= 10.0;   x2= 3.0;   y2= 5.0;   x= 2.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" y1 = y2\n");
   /* y1 = y2; x1 < x2 -- midpoint  */
   x1= 3.0;   y1= 10.0;   x2= 5.0;   y2= 10.0;   x= 4.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* y1 = y2; x1 < x2 -- outside range */
   x1= 3.0;   y1= 10.0;   x2= 5.0;   y2= 10.0;   x= 7.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* y1 = y2; x1 > x2 -- in range */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 10.0;   x= 2.8;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* y1 = y2; x1 > x2 -- outside range */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 10.0;   x= 7.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" y1 = 0\n");
   /* y1 = y2; x1 < x2 -- midpoint  */
   x1= 3.0;   y1= 0.0;   x2= 5.0;   y2= 10.0;   x= 4.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" x1 = 0\n");
   /* y1 = y2; x1 < x2 -- midpoint  */
   x1= 0.0;   y1= 5.0;   x2= 5.0;   y2= 10.0;   x= 4.0;
   ynew = (double)log_lin_interp(x,x1,y1,x2,y2);
   printf(" LOG_LIN:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );




   /*  LIN-LOG INTERPOLATION TESTS  */
   printf("\nLIN-LOG INTERPOLATION TESTS\n");
   /* Lower bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 1.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Upper bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Midpoint */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 1.5;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Within range */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 1.8;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Below lower bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 0.5;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );
   
   /* Above upper bound */
   x1= 1.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.5;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" x1 > x2\n");
   /* x1 > x2 -- Lower bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Upper bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 3.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Midpoint */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.5;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Within range */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 2.8;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* x1 > x2 -- Below lower bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 0.5;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );
   
   /* x1 > x2 -- Above upper bound */
   x1= 3.0;   y1= 5.0;   x2= 2.0;   y2= 10.0;   x= 3.5;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" decreasing function\n");
   /* Decreasing function -- midpoint */
   x1= 1.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 1.5;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Decreasing function -- in range */
   x1= 1.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 1.8;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Decreasing function; x1 > x2 -- Midpoint */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 2.5;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* Decreasing function; x1 > x2 -- Within range */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 5.0;   x= 2.8;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );
   printf(" x1 = x2 \n");
   /*  x1= x2  */
   x1= 3.0;   y1= 10.0;   x2= 3.0;   y2= 5.0;   x= 3.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1 = x2; y1 = y2; x=x1=x2  */
   x1= 3.0;   y1= 5.0;   x2= 3.0;   y2= 5.0;   x= 3.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1 = x2; y1 != y2; x=x1=x2  */
   x1= 3.0;   y1= 10.0;   x2= 3.0;   y2= 5.0;   x= 3.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1= x2; y1 = y2; x!=x1  */
   x1= 3.0;   y1= 5.0;   x2= 3.0;   y2= 5.0;   x= 2.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /*  x1= x2; y1 != y2; x!=x1  */
   x1= 3.0;   y1= 10.0;   x2= 3.0;   y2= 5.0;   x= 2.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" y1 = y2\n");
   /* y1 = y2; x1 < x2 -- midpoint  */
   x1= 3.0;   y1= 10.0;   x2= 5.0;   y2= 10.0;   x= 4.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* y1 = y2; x1 < x2 -- outside range */
   x1= 3.0;   y1= 10.0;   x2= 5.0;   y2= 10.0;   x= 7.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* y1 = y2; x1 > x2 -- in range */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 10.0;   x= 2.8;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /* y1 = y2; x1 > x2 -- outside range */
   x1= 3.0;   y1= 10.0;   x2= 2.0;   y2= 10.0;   x= 7.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" y1 = 0\n");
   /* y1 = y2; x1 < x2 -- midpoint  */
   x1= 3.0;   y1= 0.0;   x2= 5.0;   y2= 10.0;   x= 4.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   printf(" x1 = 0\n");
   /* y1 = y2; x1 < x2 -- midpoint  */
   x1= 0.0;   y1= 5.0;   x2= 5.0;   y2= 10.0;   x= 4.0;
   ynew = (double)lin_log_interp(x,x1,y1,x2,y2);
   printf(" LIN_LOG:  x1=%4.1lf y1=%4.1lf x2=%4.1lf y2=%4.1lf x=%4.1lf y=%lf\n", 
	  x1, y1, x2, y2, x, ynew );

   /****************************************************************************/
   /*  LIN-LIN INTEGRATION TESTS  */
   printf("\n\n************************************\nLIN-LIN INTEGRATION TESTS\n");
   ierr = ndi_set_isotope("d+he3->p+a");
   if ( ierr != 0 ) printf("  Error setting isotope %s is %d.\n",  "d+t->n+a", ierr );
   
   
   /* Set temperature (in MeV) */
   ierr = ndi_set_option( NDI_TEMP, "2.0e-4" );
   if ( ierr != 0 ) printf("  Error setting temperature is %d.\n\n", ierr );

   ndi_set_option( NDI_CURR_PART, "alpha" );

   /*   ierr = ndi_get_float64_val( NDI_EAVG_OUT, &eout );
	printf("   average energy for outgoing particle %s is:  %11.6f (err=%d).\n", 
	part_type, eout,ierr); */
   ierr = setup_integ();
   ierr = ndi_set_option( NDI_TEMP, "1.0e-4" );
   if ( ierr != 0 ) printf("  Error setting temperature is %d.\n\n", ierr );
   ierr = setup_integ();


   ierr = ndi_close();
   if ( ierr != 0 ) printf("  Error closing NDI is %d.\n\n", ierr );

   return(0);
}


static int setup_integ( void  )
{

  /* Assume that if x1 == x2, use Xmax and Xmin */
   int ierr;
   int num_prs;
   sec_part_distr *part;
   int temp_index_hi, temp_index_lo, temp_index_close;
   long int curr_file_p;
   double *curr_energy = NULL;
   double *curr_prob = NULL;
   double probsum = 0.0;
   interp_params curr_interp;
 
   /*
   if ( x1 != x2 ) {
     if (x1 > x2 ) {
       xmax = x1;
       xmin = x2;
     } else {
       xmax = x2;
       xmin = x1;
   } else {
     xmin = curr_energy[0];
     xmin = curr_energy[num_prs-1];
   }
   */

   part = &(tn_stat.sec_part[curr_part_index]);
/*    if ( !part ) return( -30 ); */
   ierr = ndi_find_temp_bound_index( part->edist, part->temp_interp.npts,
				     &temp_index_hi, &temp_index_lo, &temp_index_close );
   printf(" Thi = %d Tlo = %d Tclose = %d\n", temp_index_hi, temp_index_lo, temp_index_close);
   if ( ierr >= 0 ) {
     curr_interp = part->edist[temp_index_close].interp;
     curr_file_p = part->edist[temp_index_close].file_ptr;
     num_prs = curr_interp.npts;
     curr_energy = (double *)ndi_malloc( num_prs * sizeof(double) );
     curr_prob = (double *)ndi_malloc( num_prs * sizeof(double) );
     ierr = ndi_read_float_pair( lib_stream, curr_file_p, num_prs, curr_energy, curr_prob, num_prs );
     /*     ierr = integrate2( curr_energy[0], curr_energy[num_prs-1], curr_energy,
			 curr_prob, &curr_interp, &probsum, 0 );
     */
     printf("  Numprs = %d E1 = %lf  E2 = %lf  sum = %lf.\n", num_prs,
	    curr_energy[0], curr_energy[num_prs-1], probsum);

   }

   return(ierr);
}

#ifdef __STANDALONE
int main (void)
{
  int ierr = 0;

  ierr = testtn2();
  return(ierr);
}
#endif  /* __STANDALONE */

