#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "ndi_internal.h"
#include "ndi.h"
#include "ndi_state.h"
#include "ndi_var_type.h"


typedef float float32;
typedef double float64;
typedef char group_string[50];

void print_float64_array( float64 *array, int num );
int find_average_E( double *x, double *y, interp_params *iparm, double *ret_val );
int integrate_bins( double *x, double *y, interp_params *iparm, double *ret_vec, int vec_size );
double discrete_interp( double *x, double *y, double xin, int num_pts );
int interpolate_reg( double *x, double *y, interp_params *iparm );
int ndi_init_double_vec( double *vec, int num_elements );
static double ndi_discrete_integrate( double xstart, double xend, double *x, 
				      double *y, int npts );
static double ndi_histogram_integrate( double xstart, double xend, 
				       double *x, double *y, int npts  );
static double ndi_lin_lin_integrate( double xstart, double xend, double *x, 
				     double *y, int npts );
static double ndi_log_log_integrate( double xstart, double xend, double *x, 
				     double *y, int npts );
static double ndi_lin_log_integrate( double xstart, double xend, double *x, 
				     double *y, int npts );
static double ndi_log_lin_integrate( double xstart, double xend, double *x, 
				     double *y, int npts );
static double ndi_hale_integrate( double xstart, double xend, double *x, 
				  double *y, int npts );
static double erfn( double sign, double x );


int main ()

{
   int i;
   int ierr;
   int num_grps;
   interp_params iparm;

   /* Adjusted so that a point falls on egroup boundary */
   /*
   double x[] = { 1.38947E-10, 8.39066E-10, 2.39184E-09, 
		  6.39303E-09, 1.39422E-08, 5.39540E-08, 
		  2.39659E-07, 7.39778E-07, 3.39897E-06, 
		  8.40016E-06, 3.40135E-05, 9.40254E-05, 
		  4.40373E-04, 6.40492E-04, 2.40611E-03, 
		  6.40730E-03, 2.40849E-02, 7.40969E-02, 
		  3.41088E-01, 8.41208E-01, 2.86500E+00, 
		  7.41447E+00, 9.01566E+00, 1.21686E+01, 
		  1.41805E+01, 2.41925E+01, 3.02045E+01 };
   */


   double x[] = { 1.38947E-10, 8.39066E-10, 2.39184E-09, 
		  6.39303E-09, 1.39422E-08, 5.39540E-08, 
		  2.39659E-07, 7.39778E-07, 3.39897E-06, 
		  8.40016E-06, 3.40135E-05, 9.40254E-05, 
		  4.40373E-04, 6.40492E-04, 2.40611E-03, 
		  6.40730E-03, 2.40849E-02, 7.40969E-02, 
		  3.41088E-01, 8.41208E-01, 1.41327E+00,
		  7.41447E+00, 9.01566E+00, 1.21686E+01, 
		  1.41805E+01, 2.41925E+01, 3.02045E+01 };


   /* Real distribution - x values */
   /*
   double x[] = { 3.38820E+00, 3.39986E+00, 3.41155E+00, 
		  3.42325E+00, 3.43498E+00, 3.44672E+00, 
		  3.45848E+00, 3.47027E+00, 3.48207E+00, 
		  3.49390E+00, 3.50574E+00, 3.51761E+00, 
		  3.52949E+00, 3.54139E+00, 3.55332E+00, 
		  3.56526E+00, 3.57723E+00, 3.58921E+00, 
		  3.60122E+00, 3.61324E+00, 3.62529E+00, 
		  3.63735E+00, 3.64944E+00, 3.66154E+00, 
		  3.67367E+00, 3.68581E+00, 3.69798E+00 
   };
   */
   /* Real distribution - y values */
   /*
   double y[] = { 
     1.10652E-08, 2.52079E-07, 4.47242E-06,
     6.17982E-05, 6.65022E-04, 5.57344E-03,
     3.63779E-02, 1.84918E-01, 7.32062E-01,
     2.25707E+00, 5.41960E+00, 1.01349E+01,
     1.47603E+01, 1.67417E+01, 1.47888E+01,
     1.01740E+01, 5.45102E+00, 2.27453E+00,
     7.39149E-01, 1.87068E-01, 3.68719E-02,
     5.66002E-03, 6.76655E-04, 6.30005E-05,
     4.56823E-06, 2.57976E-07, 1.13459E-08
   };
   */

   /* Hale function a=1, b=1, c=1 */
   /*
   double y[] = { 	4.336510E-06, 1.065685E-05, 1.799343E-05,
			2.941903E-05, 4.344839E-05, 8.549074E-05, 
			1.802717E-04, 3.169589E-04, 6.807365E-04, 
			1.072416E-03, 2.170612E-03, 3.636719E-03,
			8.047334E-03, 9.787379E-03, 1.985749E-02,
			3.433896E-02, 7.601810E-02, 1.602736E-01,
			4.912311E-01, 9.109035E-01, 1.147177E+00,
			1.398971E-01, 5.442342E-02, 7.136973E-03,
			1.794255E-03, 1.054647E-06,   9.156696E-09 };
   */

   /* Hale function a=3, b=7, c=8 */
   /*
   double y[] = { 7.735726E-08, 1.901024E-07, 3.209747E-07,
		  5.247853E-07, 7.750379E-07, 1.524951E-06,
		  3.215411E-06, 5.652908E-06, 1.213786E-05,
		  1.911673E-05, 3.866552E-05, 6.472241E-05,
		  1.428575E-04, 1.735897E-04, 3.506510E-04,
		  6.038000E-04, 1.332237E-03, 2.849882E-03,
		  1.020596E-02, 2.697236E-02, 5.235670E-02,
		  8.300138E-01, 1.222260E+00, 2.240240E+00,
		  3.055550E+00, 8.585706E+00, 1.242616E+01 };
   */

   /* Hale function a=4, b=1, c=-2 */
   /*
    double y[] = { 7.773681E-05, 1.910261E-04, 3.225166E-04,
		  5.272613E-04, 7.786129E-04, 1.531504E-03,
		  3.226945E-03, 5.667409E-03, 1.213615E-02,
		  1.905874E-02, 3.823908E-02, 6.333431E-02,
		  1.355500E-01, 1.627841E-01, 3.083787E-01,
		  4.888467E-01, 8.869724E-01, 1.418989E+00,
		  2.547223E+00, 3.681301E+00, 4.840762E+00,
		  4.805248E+01, 8.921035E+01, 3.084845E+02,
		  6.900595E+02, 4.248807E+04, 5.385727E+05 };
		  */

   /* x=y */
   /*
   double y[] = { 1.38947E-10, 8.39066E-10, 2.39184E-09, 
		  6.39303E-09, 1.39422E-08, 5.39540E-08, 
		  2.39659E-07, 7.39778E-07, 3.39897E-06, 
		  8.40016E-06, 3.40135E-05, 9.40254E-05, 
		  4.40373E-04, 6.40492E-04, 2.40611E-03, 
		  6.40730E-03, 2.40849E-02, 7.40969E-02, 
		  3.41088E-01, 8.41208E-01, 1.41327E+00, 
		  7.41447E+00, 9.01566E+00, 1.21686E+01, 
		  1.41805E+01, 2.41925E+01, 3.02045E+01 };
   */
   double y[] = { 1.0, 1.0, 1.0,
		  1.0, 1.0, 1.0, 
		  1.0, 1.0, 1.0, 
		  1.0, 1.0, 1.0, 
		  1.0, 1.0, 1.0, 
		  1.0, 1.0, 1.0, 
		  1.0, 1.0, 1.0, 
		  1.0, 1.0, 1.0, 
		  1.0, 1.0, 1.0
   };

   /*
   double y[] = { 1.06852E-08, 2.44036E-07, 4.34077E-06,
		  6.01336E-05, 6.48795E-04, 5.45175E-03,
		  3.56782E-02, 1.81848E-01, 7.21863E-01,
		  2.23171E+00, 5.37355E+00, 1.00768E+01,
		  1.47172E+01, 1.67404E+01, 1.48301E+01,
		  1.02320E+01, 5.49819E+00, 2.30100E+00,
		  7.49985E-01, 1.90383E-01, 3.76394E-02,
		  5.79557E-03, 6.95008E-04, 6.49116E-05,
		  5.72166E-06, 2.67489E-07, 1.18021E-08 };
		  */

   double return_vec[200];
   double energyb[31];
   double avge;
   char line[100];
   char group_string[10] = "12_group";
   int interp_type;
   

   while ( 1 ) { 

   printf("Enter interpolation type [< 0 to quit]: \n");
   fgets(line, sizeof(line), stdin);
   sscanf(line, "%d", &interp_type);
   if ( interp_type < 0 ) {
     break;
   }


   printf("Enter number of groups: \n");
   fgets(line, sizeof(line), stdin);
   sscanf(line, "%d", &num_grps);
   sprintf(group_string, "%d_group", num_grps);
     
   iparm.npts = 27;
   iparm.nint = 1;
   iparm.nbt = (int *)ndi_malloc( sizeof(int) );
   iparm.int_type = (int *)ndi_malloc( sizeof(int) );
   iparm.nbt[0] = 27;
   iparm.int_type[0] = interp_type;

   /*
   for (i = 1; i < iparm.npts; i++ ) {
     printf(" %11.5e  %11.5e\n", x[i], y[i]);
   }
   */

   ierr = ndi_init();
   ierr = ndi_set_option( NDI_LIB_TYPE, "tn" );
   if ( ierr != 0 ) printf("  Error setting lib_type is %d.\n\n", ierr );
   ierr = ndi_set_option( NDI_LIBRARY, "lanl01" );
   if ( ierr != 0 ) printf("  Error setting library is %d.\n\n", ierr );

   /* Set energy bins--for energy distribution--to 30 groups */
   ierr = ndi_set_option( NDI_COLLAPSE, group_string );
   /*
   if ( ierr != 0 ) {
     printf( "Bad group string %s.\n", group_string );
     continue;
   }
   */
/*    num_grps = 30; */

   ierr = ndi_set_isotope( "d+t->n+a" );

   ierr = ndi_get_float64_vec( NDI_E_BOUNDS, energyb, 31 );

   ierr = interpolate_reg( x, y, &iparm );   

   /*  Collapse continuous distribution given by x,y into energy bin structure */
   ierr = integrate_bins( x, y, &iparm, return_vec, 200 );
   printf("\n***********\n");

   for (i = 0; i < num_grps; i++ ) {
     printf(" %11.5e  %11.5e\n", energyb[i], return_vec[i]);
   }

   /*
   print_float64_array( energyb, num_grps + 1 );
   print_float64_array( return_vec, num_grps  );
   */

   /* Find average energy */
   /*
   ierr = find_average_E( x, y, &iparm, &avge );
   printf("AVG ENERGY = %lf\n",avge ); 
   */

   printf("\n\n***********\n");

   }   /* while ( 1 ) */

   ierr = ndi_close();

   if (iparm.nbt) {
     ndi_free( (void **) &iparm.nbt );
   }
   if (iparm.int_type) {
     ndi_free( (void **) &iparm.int_type );
   }

   return(ierr);
}


/*******************************************************************/
int find_average_E( double *x, double *y, interp_params *iparm, double *ret_val ) {


  typedef double (*two_pt_xyintegr_func)( double, double, double, double);
  typedef double (*two_pt_integr_func)( double, double, double, double);

  int ierr = 0;
  int npts = iparm->npts;
  int int_type = iparm->int_type[0];
  int interp_index = 0;
  int pt_index;
  int index1, index2, index3;
  double y_integ = 0.0;
  double xy_integ = 0.0;
  two_pt_xyintegr_func two_pt_xyintegrate[6];
  two_pt_integr_func two_pt_integrate[6];

  two_pt_xyintegrate[1] = &histogram_integrate_x;
  two_pt_xyintegrate[2] = &lin_lin_integrate_x;
  two_pt_xyintegrate[3] = &lin_log_integrate_x;
  two_pt_xyintegrate[4] = &log_lin_integrate_x;
  two_pt_xyintegrate[5] = &log_log_integrate_x;

  two_pt_integrate[1] = &histogram_integrate;
  two_pt_integrate[2] = &lin_lin_integrate;
  two_pt_integrate[3] = &lin_log_integrate;
  two_pt_integrate[4] = &log_lin_integrate;
  two_pt_integrate[5] = &log_log_integrate;

  /* Discrete points -- start at first point */
  if ( int_type == 0 ) {
    xy_integ += x[0] * fabs(y[0]);
    y_integ += fabs(y[0]);
  }

  for (pt_index = 1; pt_index < iparm->npts; pt_index++) {
    if ( pt_index > iparm->nbt[interp_index] ) {
      interp_index++;
      int_type = iparm->int_type[interp_index];
    }

    
    if ( ( int_type > 0 ) && ( int_type < 6 ) ) { 
      /* histogram, lin-lin, lin-log, log-lin, or log-log */
      xy_integ += fabs( two_pt_xyintegrate[int_type]( x[pt_index-1], y[pt_index - 1], 
						      x[pt_index], y[pt_index] ) ) ;
      y_integ += fabs( two_pt_integrate[int_type]( x[pt_index-1], y[pt_index - 1], 
						   x[pt_index], y[pt_index] ) ) ;
    } else if ( int_type == 0 ) {
      /* Discrete values */
      xy_integ += x[pt_index] * fabs(y[pt_index]);
      y_integ += fabs(y[pt_index]);

    } else if ( int_type == 6 ) {
      /* Gerry Hale's interpolation scheme */
      if ( ( pt_index >= ( iparm->npts - 1 ) ) ||     /* x[pt_index] is last point of distribution */
	   ( iparm->nbt[interp_index] == pt_index  )  /* x[pt_index] is last point in interp region */
	   ) {
	index1 = pt_index - 2;
	index2 = pt_index - 1;
	index3 = pt_index;
      } else {
	index1 = pt_index - 1;
	index2 = pt_index;
	index3 = pt_index + 1;
      }
      xy_integ += fabs(hale_integrate_x( x[pt_index-1], x[pt_index], x[index1], y[index1], 
					 x[index2], y[index2],x[index3], y[index3]) );
      y_integ += fabs(hale_integrate( x[pt_index-1], x[pt_index], x[index1], y[index1], 
				      x[index2], y[index2],x[index3], y[index3] ) );

    } else {
      /*?????*/
    }

  }
  if (y_integ != 0) {
    *ret_val = xy_integ/y_integ;
  }
  return(ierr);

}


/*******************************************************************/

int integrate_bins( double *x, double *y, interp_params *iparm, double *ret_vec,
		    int vec_size ) {

  /* x is given in ASCENDING order; y is ordered to correspond to x-values.
     However, the energy bins are assumed to be in DESCENDING order.
     */

  /* The discrete calculation is not done very elegantly.  It was thrown in after the
     others, and it was assumed that the interpolation type may change between discrete
     and non-discrete; otherwise, several obvious, more elegant solutions presented
     themselves.  But with this assumption nothing was obvious to me, so it's being left 
     to some later, more inspired, time to come up with something better.
  */

  typedef double (*integration_function_type)( double, double, double *, double *, int );
     
  int ierr = 0;
  int distr_pts;          /* Number of distribution points being passed to integration function */
  int increment_xy_index = 0;      /* flag indicating whether xy_index should be incremented on next step*/
  int decrement_ebin_index = 0;    /* flag indicating whether ebin_index should be incremented on next step*/
  int xy_index = 0;       /* index of the first distribution pt. > xstart (discrete: current distr. pt.)*/
  int ebin_index;         /* index of the lower-energy boundary of the current bin */
  int interp_index = 0;
  int discrete_flag = 0;  /* non-zero if int_type ever != 0 -- special kludginess required for discrete */
  int num_bins = tn_num_grps;
  int npts = iparm->npts;
  int int_type = iparm->int_type[0];
  double *ebin = tn_e_bounds;
  double sum = 0.0;
  double xstart, xend;
  double ystart, yend;
  double *xptr = NULL;    /* Ptr to array of distribution x points passed to integration function */
  double *yptr = NULL;    /* Ptr to array of distribution y points passed to integration function */
  double xpts[3], ypts[3];   /* Containers to hold x & y distr pts for types 1 - 6 */
  int num_pts;               /* Number of valid points in the xpts and ypts arrays */
  integration_function_type integration_function[7];

  char line[100];

  integration_function[0] = &ndi_discrete_integrate;
  integration_function[1] = &ndi_histogram_integrate;
  integration_function[2] = &ndi_lin_lin_integrate;
  integration_function[3] = &ndi_lin_log_integrate;
  integration_function[4] = &ndi_log_lin_integrate;
  integration_function[5] = &ndi_log_log_integrate;
  integration_function[6] = &ndi_hale_integrate;


  /*  It is assumed that the energy group structure in ebin is decreasing by energy
      and that the distribution given by x and y are increasing in energy.  
      The integration takes place from low energy to high energy, so xy_index is
      incremented, while the ebin_index is decremented
  */

  /* Initialization */
  ndi_init_double_vec( ret_vec, vec_size );
  ebin_index = num_bins;
  xy_index = 0;
  interp_index = 0;
  int_type = iparm->int_type[interp_index];
  if ( int_type == 0 ) { discrete_flag = -1; }


  /* Choose xstart and set ebin_index and xy_index to appropriate values */
  if ( ebin[ebin_index] >= x[xy_index] ) {

    /* Distribution overlaps low-energy-end of group structure */

    xstart = ebin[ebin_index];

    /* Find the first distribution point greater than ebin[ebin_index] */
    while ( (xy_index < npts ) && 
	    ( x[xy_index] < ebin[ebin_index] ) ) {
      /* Set xy_index so that ebin[ebin_index] < x[xy_index] < ebin[ebin_index - 1] */
      xy_index++;
    }

    /* Distribution is completely outside (lower than) the group structure */
    if ( xy_index >= npts ) {
      /*       ????????; */
    }

    /* Adjust the interpolation index if necessary */
    while ( xy_index > iparm->nbt[interp_index] ) {
      if ( interp_index < iparm->nint ) {
	interp_index++;
	int_type = iparm->int_type[interp_index];
	if ( int_type == 0 ) { discrete_flag = -1; }
      } else {
	break;
	/*  ???????? */
      }
    }


  } else {

    /* Distribution starts inside group structure */

    xstart = x[xy_index];
    xy_index++;
    while ( ( ebin_index >= 1 ) &&
	    ( ebin[ebin_index - 1] < x[xy_index] ) ) {
      ebin_index--;
    }
    if ( ( ebin_index < 0 ) || ( ebin[0] == x[0] ) ) {
      /* Distribution lies completely above energy group structure */
      /*  ????????? */
    }
  }


  /* MAIN LOOP --
     Integrate distribution into energy bins in ascending order of energy */
  while ( ( ebin_index > 0 ) && ( xy_index < npts ) ) {
    /* xstart is already set to appropriate value;
       xy_index is already set to the closest distribution point > xstart;
       ebin_index is already set to the current bin, i.e. 
              ebin[ebin_index] <= xstart < ebin[ebin_idnex - 1];
       interp_index is already set appropriately for current xy_index.
    */
    
    /* Determine xend --
       xend is either x[xy_index] or ebin[ebin_index - 1] 
    */
    /* bounds check here too */
    if ( x[xy_index] < ebin[ebin_index - 1] ) {
      /* Integrate up to distribution point x[xy_index] */
      xend = x[xy_index];
      increment_xy_index = -1;  /* increment xy_index after this integration */
    } else {
      /* Integrate up to bin boundary ebin[ebin_index - 1] */
      /* It is important that the "==" case come here to decrement_ebin_index--
	 in case of discrete with distr pt == ebin[0] 
      */
      xend = ebin[ebin_index - 1];
      decrement_ebin_index = -1;  /* decrement ebin_index after this integration step */
      if ( x[xy_index] == ebin[ebin_index - 1] ) {
	/* Ending bin boundary is the same as current distr. pt., so increment xy_index too */
	 increment_xy_index = -1; 
      }
    }


    /* Find distribution points needed for integration and integrate */
    if ( int_type == 0 ) {
      /* discrete */
      xptr = x;
      yptr = y;
      num_pts = npts;
    } else if ( ( int_type > 0 ) && ( int_type < 6 ) ) {
      xpts[0] = x[xy_index - 1];
      xpts[1] = x[xy_index];
      xptr = xpts;
      ypts[0] = y[xy_index - 1];
      ypts[1] = y[xy_index];
      yptr = ypts;
      num_pts = 2;
    } else if ( int_type == 6 ) {
      if ( xy_index == 0 ) {
	xpts[0] = x[xy_index];
	ypts[0] = y[xy_index];
	xpts[1] = x[xy_index + 1];
	ypts[1] = y[xy_index + 1];
	xpts[2] = x[xy_index + 2];
	ypts[2] = y[xy_index + 2];
      } else if ( ( xy_index == 1 ) ||
		  ( ( xy_index < (npts - 1) ) &&                                        /* Not at the last point */
		    ( ( xstart - x[xy_index - 1] ) > DBL_EPSILON ) &&   /* Empirically, this seems to work better*/
		    ( iparm->nbt[interp_index] < xy_index ) ) ){        /* Not last pt. of current interp region */
	xpts[0] = x[xy_index - 1];
	ypts[0] = y[xy_index - 1];
	xpts[1] = x[xy_index];
	ypts[1] = y[xy_index];
	xpts[2] = x[xy_index + 1];
	ypts[2] = y[xy_index + 1];
      } else {
	xpts[0] = x[xy_index - 2];
	ypts[0] = y[xy_index - 2];
	xpts[1] = x[xy_index - 1];
	ypts[1] = y[xy_index - 1];
	xpts[2] = x[xy_index];
	ypts[2] = y[xy_index];
      }
      xptr = xpts;
      yptr = ypts;
      num_pts = 3;
    } else {
      /* ???????? */
    }
    sum +=  integration_function[int_type]( xstart, xend, xptr, yptr, num_pts );


    /* Setup for next step */

    if ( increment_xy_index ) {
      xy_index++;
      if ( xy_index >= npts )  {
	/* We're done so write out the result */
	ret_vec[ebin_index - 1] = sum;
      }

      /* Check to see if interp_index must also be updated */
      while ( xy_index > iparm->nbt[interp_index] ) {
	if ( interp_index < iparm->nint ) {
	  interp_index++;
	  int_type = iparm->int_type[interp_index];
	  if ( int_type == 0 ) { discrete_flag = -1; }
	} else {
	  break;
	  /*  ???????? */
	}
      }
    }

    if ( decrement_ebin_index ) {
      ebin_index--;
      /* bounds check on ret_vec */
      ret_vec[ebin_index] = sum;
      sum = 0.0;
    }

    xstart = xend;
    increment_xy_index = 0;
    decrement_ebin_index = 0;

  } /* while ebin_index > 0 */

  /* Special fix for discrete -- if a discrete point falls on an energy bin
     boundary, then half goes in the lower bin and half goes in the upper.
     However, if this happens on the lowest or highest bin boundary, half of
     the discrete value is discarded.  It has been decided (7/31/01) that
     the whole value should be counted and added to the lowest or highest
     bin. */
  if ( discrete_flag != 0 ) {
    ierr = fix_discrete_bounds( ebin[0], ebin[num_bins], x, y, iparm, &(ret_vec[0]),
				&(ret_vec[num_bins]) );
  }

  return(ierr);

}


/********************************************************************************/
int integrate_bins_bak( double *x, double *y, interp_params *iparm, double *ret_vec ) {

  /* x is given in ASCENDING order; y is ordered to correspond to x-values.
     However, the energy bins are assumed to be in DESCENDING order.
     */

  typedef double (*two_pt_interp_func)( double, double, double, double, double);
  typedef double (*two_pt_integr_func)( double, double, double, double);
     
  int ierr = 0;
  int done = 0;
  int xy_index = 0;       /* index of the upper distribution pt. (discrete: current distr. pt.)*/
  int ebin_index;         /* index of the lower boundary of the current bin */
  int interp_index = 0;
  int next_bin_flag = 0;
  int num_boundaries = tn_num_grps;
  int npts = iparm->npts;
  int int_type = iparm->int_type[0];
  double *ebin = tn_e_bounds;
  double sum = 0.0;
  double xstart, xend;
  double ystart, yend;
  double x1, x2, x3;
  double y1, y2, y3;
  two_pt_interp_func two_pt_interpolate[6];
  two_pt_integr_func two_pt_integrate[6];

  char line[100];


  two_pt_interpolate[1] = &histogram_interp;
  two_pt_interpolate[2] = &lin_lin_interp;
  two_pt_interpolate[3] = &lin_log_interp;
  two_pt_interpolate[4] = &log_lin_interp;
  two_pt_interpolate[5] = &log_log_interp;

  two_pt_integrate[1] = &histogram_integrate;
  two_pt_integrate[2] = &lin_lin_integrate;
  two_pt_integrate[3] = &lin_log_integrate;
  two_pt_integrate[4] = &log_lin_integrate;
  two_pt_integrate[5] = &log_log_integrate;


  /* Start at upper index of energy bins (lowest energy) */
  ebin_index = num_boundaries;

/*   for ( ebin_index = num_boundaries - 1; ebin_index >= 0; ebin_index-- ) { */
  while ( ( ebin_index > 0 ) && !(done) ) {
    xstart = ebin[ebin_index];
/*     printf ("XSTART=%e  INDEX=%d\n", xstart, ebin_index ); */

    while ( xy_index < npts ) {
      while ( x[xy_index] <= xstart ) {  
	xy_index++;
	if ( xy_index > npts ) { 
	  ret_vec[ebin_index] = sum;        
	  done = -1;
	  break; 
	}
      }


      /* find xend */
      if ( x[xy_index] < ebin[ebin_index - 1] ) {
	/*  Integrating up to distribution point */
	xend = x[xy_index];
	next_bin_flag = 0;
      } else {
	if ( xy_index == 0 ) {
	  /* We haven't reached the first bin with non-zero entries yet */
	  ebin_index--;                     /* Move to next bin */
	  ret_vec[ebin_index] = sum;        
	  sum = 0.0;
	  break; 

	} else {
	  /*  Integrating up to bin boundary */
	  ebin_index--;
	  xend =  ebin[ebin_index];
	  next_bin_flag = -1;
	}
      }

      /* For 3-point integration scheme, choose 3rd point */
      if ( int_type == 6 ) {
	if ( ( xy_index < (npts - 1) ) &&                                        /* Not at the last point */
	     /* ( xstart >= ( x[xy_index - 1] ) ) && */ /* Starting pt. is between x[xy_index-1] & x[xy_index] */
	     ( xstart > ( DBL_EPSILON + ( x[xy_index - 1] ) ) ) &&  
	     ( iparm->nbt[interp_index] != xy_index ) ) {   /* Not last pt. of current interp region */
	  /* Use the point following x[xy_index]; otherwise the point before x[xy_index-1] is used */
	  xy_index++;
	}
	if ( (xy_index - 2) >= 0 ) {
	  x1 = x[xy_index-2];
	  y1 = y[xy_index-2];
	  x2 = x[xy_index-1];
	  y2 = y[xy_index-1];
	  x3 = x[xy_index];
	  y3 = y[xy_index];
	} else {
	  /* ???? */
	}
      }

      /* Check if we are in another interpolation region */
      while  ( (xy_index - 1) >= iparm->nbt[interp_index] ) {
	if ( interp_index >= iparm->nint ) { 
	  /*  ierr = ?;   */
	  break; 
	}
	interp_index++;
	int_type = iparm->int_type[interp_index];
      }


      /* Check that  x[0] does not fall ABOVE the first bin */

      if ( ( int_type > 0 ) && ( int_type < 6 ) ) { 
	/* histogram, lin-lin, lin-log, log-lin, or log-log */
	ystart = two_pt_interpolate[int_type](xstart, x[xy_index - 1], y[xy_index - 1],
					      x[xy_index], y[xy_index] );
      } 
  
      if ( ( int_type > 0 ) && ( int_type < 6 ) ) { 
	/* histogram, lin-lin, lin-log, log-lin, or log-log */
	yend = two_pt_interpolate[int_type](xend, x[xy_index - 1], y[xy_index - 1],
					    x[xy_index], y[xy_index] );
	sum +=  two_pt_integrate[int_type]( xstart, ystart, xend, yend );
      } else if ( int_type == 0 ) {
	/* discrete points */
	yend = discrete_interp( x, y, xend, npts );
	sum +=  yend;
      } else if ( int_type == 6 ) {
	double junk;
	/* Gerry Hale's interpolation scheme */
	/*
	printf("start=%e, end=%e\n1: %e  %e\n2: %e  %e\n3: %e  %e\n",
	       xstart, xend, x1, y1, x2, y2, x3, y3 );
	       */
        junk =  hale_integrate( xstart, xend, x1, y1, x2, y2, x3, y3 );
	sum += junk;
	/*
	printf ("JUNK=%e\n", junk );
	printf ("  sum=%e\n", sum );
	*/
      } else {
	/* ????? */
      }

      xstart = xend;

      if ( next_bin_flag ) { 
	ret_vec[ebin_index] = sum;
	sum = 0.0;
	break; 
      }
    }  /* while xy_index < npts */

  } /* while ebin_index > 0 */

  return(ierr);

}


/********************************************************************************/
double discrete_interp( double *x, double *y, double xin, int num_pts ) {

  int i;
  double result = 0.0;

  for ( i = 0; i < num_pts; i++ ) {
    if ( fabs( xin - x[i] ) < DBL_EPSILON ) {
      result = y[i];
      break;
    }
  }
  return(result);

}

/********************************************************************************/

int interpolate_reg( double *x, double *y, interp_params *iparm ) {

  /* x is given in ASCENDING order; y is ordered to correspond to x-values.
     However, the energy distribution is assumed to be in DESCENDING order.
     */

  typedef double (*two_pt_interp_func)( double, double, double, double, double);
  typedef double (*two_pt_integr_func)( double, double, double, double);
     
  int ierr = 0;
  int i;
  int fnx_index;
  int no_interp_pts = 10;
  int npts = iparm->npts;
  int int_type = iparm->int_type[0];
  int interp_index = 0;

  double delta;
  double currx, curry;
  two_pt_interp_func two_pt_interpolate[6];
  two_pt_integr_func two_pt_integrate[6];

  two_pt_interpolate[1] = &histogram_interp;
  two_pt_interpolate[2] = &lin_lin_interp;
  two_pt_interpolate[3] = &lin_log_interp;
  two_pt_interpolate[4] = &log_lin_interp;
  two_pt_interpolate[5] = &log_log_interp;

  two_pt_integrate[1] = &histogram_integrate;
  two_pt_integrate[2] = &lin_lin_integrate;
  two_pt_integrate[3] = &lin_log_integrate;
  two_pt_integrate[4] = &log_lin_integrate;
  two_pt_integrate[5] = &log_log_integrate;


  fnx_index = 1;

  for ( fnx_index = 1; fnx_index < npts; fnx_index++ ) {
    delta = (x[fnx_index] - x[fnx_index - 1]) / (no_interp_pts + 1);
    currx =  x[fnx_index - 1];
    for ( i = 0; i <= no_interp_pts; i++ ) {

      if ( ( int_type > 0 ) && ( int_type < 6 ) ) { 
	/* histogram, lin-lin, lin-log, log-lin, or log-log */
	curry = two_pt_interpolate[int_type](currx, x[fnx_index - 1], y[fnx_index - 1],
					     x[fnx_index], y[fnx_index] );    
      } else if ( int_type == 0 ) {
	curry = 0.0;
      } else if ( int_type == 6) {
	double x1,x2,x3;
	double y1,y2,y3;
	if ( (fnx_index + 1) < npts ) {
	  x1 = x[fnx_index - 1];
	  y1 = y[fnx_index - 1];
	  x2 = x[fnx_index];
	  y2 = y[fnx_index];
	  x3 = x[fnx_index + 1];
	  y3 = y[fnx_index + 1];
	  
	  curry = hale_interp( currx, x1, y1, x2, y2, x3, y3);
	} else if ( fnx_index < npts) {
	  /* Last section -- use last three points */
	  x1 = x[fnx_index - 2];
	  y1 = y[fnx_index - 2];
	  x2 = x[fnx_index - 1];
	  y2 = y[fnx_index - 1];
	  x3 = x[fnx_index];
	  y3 = y[fnx_index];
	  
	  curry = hale_interp( currx, x1, y1, x2, y2, x3, y3);
	} else {
	  curry = 0.0;
	}
      } else {
      }

      printf("%11.5e  %11.5e\n",currx, curry); 
      currx += delta;
    }

    /* Check if we are in another interpolation region */
    while  ( fnx_index >= iparm->nbt[interp_index] ) {
      if ( interp_index >= iparm->nint ) { 
	/*  ierr = ?;   */
	break; 
      }
      interp_index++;
      int_type = iparm->int_type[interp_index];
    }
  }


  return(ierr);
}


/********************************************************************************/
void print_float64_array( float64 *array, int num ) {

  int i;

  for (i = 0; i < num; i++ ) {
    printf("    %11.5e", array[i] );
    if ( ( (i+1) % 7 ) == 0 ) {
      printf("\n");
    }
  }
  printf("\n");

  return;

}
/********************************************************************************/

int ndi_init_double_vec( double *vec, int num_elements ) {

   int i;
   int ierr = 0;

   for( i = 0; i < num_elements; i++ ) {
      *vec = 0.0;
      vec++;
   }
   return(ierr);
}
/********************************************************************************/
/*********************************************************************/
/*                        INTEGRATION                                */
/*                       Integrate y(E)                              */
/*********************************************************************/
static double ndi_histogram_integrate( double xstart, double xend, 
				       double *x, double *y, int npts  ) {
  /* Interp scheme = 1  (y1 = y2) */

  /* Assume that (x[0] <= xstart < xend <= x[1]) */
  /* What if y < 0 ?????? */


  return( fabs( (xend - xstart) * y[0] ) );

}
/*********************************************************************/
static double ndi_lin_lin_integrate( double xstart, double xend, double *x, 
				     double *y, int npts ) {
  /* Interp scheme = 2 */

  /* Assume that (x[0] <= xstart < xend <= x[1]) */
  /* What if y < 0 ?????? */

   double width = ( xend - xstart );
   double ystart, yend;

   ystart = lin_lin_interp( xstart, x[0], y[0], x[1], y[1] );
   yend = lin_lin_interp( xend, x[0], y[0], x[1], y[1] );
   return(  fabs ( ( ystart + yend ) * width / 2.0 ) );

}

/*********************************************************************/
static double ndi_log_log_integrate( double xstart, double xend, double *x, 
				     double *y, int npts ) {
  /* Interp scheme = 5 */


  /* Assume that (x[0] <= xstart < xend <= x[1]) */
  /* What if y < 0 ?????? */

   double ystart, yend;
   double alpha;
   double beta;
   double return_val;
   
   ystart = log_log_interp( xstart, x[0], y[0], x[1], y[1] );
   yend = log_log_interp( xend, x[0], y[0], x[1], y[1] );

   /* check that xstart,xend,ystart,yend not equal 0 */ 
   if ( fabs(xstart) < DBL_EPSILON  ||
	fabs(xend) < DBL_EPSILON  ||
	fabs(ystart) < DBL_EPSILON  ||
	fabs(yend) < DBL_EPSILON  ) {
     return( 0.0 );
   } else {
     beta = ( log (yend/ystart) ) / ( log( xend / xstart ) );
     alpha = log(ystart) - beta * log(xstart);
   }

   if ( beta != -1 ) {
     return_val = (exp(alpha + (beta + 1) * log(xstart)) / (beta + 1)) *
       ( pow(( xend / xstart ), (beta + 1)) - 1.0 ); 
   } else {
     return_val = exp(alpha) * log( xend/xstart );
   }

   return( return_val );

}

/*********************************************************************/
static double ndi_lin_log_integrate( double xstart, double xend, double *x, 
				     double *y, int npts ) {
  /* Interp scheme = 3 (y is linear in the ln of x) */

   double ystart, yend;
   double alpha;
   double beta;
   double return_val;
   

   ystart = lin_log_interp( xstart, x[0], y[0], x[1], y[1] );
   yend = lin_log_interp( xend, x[0], y[0], x[1], y[1] );
   /* check that xstart, xend not equal 0 and xstart !=  xend */ 
   if ( fabs(xstart) < DBL_EPSILON ||
	fabs(xend) < DBL_EPSILON ||
	fabs(log( xend / xstart )) < DBL_EPSILON ) {
     return(0.0);
   } else {
     beta = (yend - ystart) / ( log( xend / xstart ) );
     alpha = ystart - beta * log(xstart);
   }

   return_val = alpha * (xend - xstart) + beta * ( (xend * log(xend) - xend) 
					     - (xstart * log(xstart) - xstart ) );
   return( return_val );

}
/*********************************************************************/
static double ndi_log_lin_integrate( double xstart, double xend, double *x, 
				     double *y, int npts ) {
  /* Interp scheme = 4 */

   double ystart, yend;
   double alpha;
   double beta;
   double return_val;
   

   ystart = lin_log_interp( xstart, x[0], y[0], x[1], y[1] );
   yend = lin_log_interp( xend, x[0], y[0], x[1], y[1] );

   /* check that xstart,xend,ystart,yend not equal 0 */ 
   if ( fabs(ystart) < DBL_EPSILON ||
	fabs(yend) < DBL_EPSILON ||
	fabs( xend - xstart ) < DBL_EPSILON ) {
     return(0.0);
   } else {
     beta = ( log (yend/ystart) ) / ( xend - xstart );
     alpha = log(ystart) - beta * xstart;
   }

   if ( fabs(beta) < DBL_EPSILON ) {
     return_val = ( ystart * (xend - xstart) );  /* What to do here????  */
   } else {
     return_val = ( exp(alpha + beta * xstart)/beta ) * ( exp(beta * (xend - xstart)) - 1.0 );
   }

   return( return_val );

}
/*********************************************************************/
static double ndi_hale_integrate( double xstart, double xend, double *x, 
				  double *y, int npts ) {

  /* Interp scheme = 6

     Assume that (x[0] <= xstart < xend <= x[2]) and (x[0] < x[1] < x[2]).

     Gerry Hale's function for secondary energy distributions:
          y=a*sqrt(E)*exp(-1*(sqrt(E) - b)^2/c)  where E is energy

     This function integrates between xstart and xend.
     The variables have been kept the same as in G. Hale's code ?.  However,
     two changes have been made: 
     1) in this code x[0],x[1],x[2] are energy values; in Gerry's codes x[0]=sqrt(E1), 
        x[1]=sqrt(E2), x[2]=sqrt(E3) ,where E1,E2,E3 are the energy values and 
     2) u1 and u2 in this code correspond to y[0] and y[1] in Gerry's code.
   */

   double return_val = 0.0;
   double a, b, c;
   double del;
   double dif1, dif2;
   double f1, f2, f3;
   double coef1, coef2;
   double fac1, fac2;
   double exp2, erf2;
   double u1, u2;
   double snc;
   double pi = 3.1415926536;    /* pi given exactly as in Gerry Hale's program */


   /* Check for negative values in sqrt and zeroes in denom */
   if ( x[0] <= 0   ||
	x[1] <= 0   ||
	x[2] <= 0   ||
	y[0] == 0   ||
	y[1] == 0   ||
	y[2] == 0  )      {
     return ( return_val );
   }
   
   f1 = log(y[0]/sqrt(x[0]));
   f2 = log(y[1]/sqrt(x[1]));
   f3 = log(y[2]/sqrt(x[2]));
   dif1 = f2 - ( f1 + f3 ) / 2;
   dif2 = ( f1 - f3 ) / 2;
   del = sqrt(x[1]) - sqrt(x[0]);

   coef1 = f2 - f1;
   coef2 = coef1 / (f3 - f1);

   if ( fabs(2 * del) < DBL_EPSILON ||
	fabs(4 * dif1) < DBL_EPSILON ) {
     return ( 0.0 );
   }

   c = (del *del)/dif1;
   b = sqrt(x[1]) - c * dif2/ ( 2 * del );
   a = exp(f2 + dif2*dif2 / ( 4 * dif1 ));

   /*
   b = ( coef2 * (sqrt(x[0]) + sqrt(x[2])) * (sqrt(x[0]) - sqrt(x[2])) - 
	 (sqrt(x[0]) + sqrt(x[1])) * ( sqrt(x[0]) - sqrt(x[1]) ) ) /
     ( 2 * coef2 * ( sqrt(x[0]) - sqrt(x[2]) ) - 2 * ( sqrt(x[0]) - sqrt(x[1]) ) );
   c = ( (sqrt(x[0]) + sqrt(x[1]) - 2 * b) * ( sqrt(x[0]) - sqrt(x[1]) ) ) / coef1;
   a = y[1] * exp( pow((sqrt(x[1]) - b), 2)/ c ) / sqrt(x[1]);
   */

   printf("a=%e, b=%e, c=%e\n", a,b,c); 

   fac1 = a * c;
   fac2 = (b*b + c/2) * sqrt(pi/fabs(c));
   snc = c / fabs(c);
   u1 = snc * (sqrt(xstart) - b)/sqrt(fabs(c));
   u2 = snc * (sqrt(xend) - b)/sqrt(fabs(c));
   exp2 = (sqrt(xend)+b)*exp(-snc*u2*u2);
   erf2 = erfn(snc, u2);

   return_val = fac1 * ( (sqrt(x[0]) + b) * exp(-snc*u1*u1)-exp2-fac2*(erfn(snc,u1)-erf2));

   return( return_val );

}
/*********************************************************************/
static double ndi_discrete_integrate( double xstart, double xend, double *x, 
				      double *y, int npts ) {
  /* Interpolation scheme = 0 */

  /* Assume that xstart < xend;
     x is given in ascending order */

  int i = 0;
  double result = 0.0;

  while ( ( i < npts ) && ( x[i] <= xend ) ) {
    if ( x[i] == xstart ) {
      result += y[i] / 2;
    }
    if ( x[i] == xend ) {
      result += y[i]/2;
    }
    if ( ( xstart < x[i] ) && ( x[i] < xend ) ) {
      result += y[i];
    }
    i++;
  }

  return(result);

}
/********************************************************************************/
static int fix_discrete_bounds( double ebin_0, double ebin_last, double *x, double *y, 
				interp_params *iparm, double *ret_vec0, 
				double * ret_vec_last ) {
  
  int interp_index = 0;
  int ierr = 0;
  int i;

  for ( i = 0; i < iparm->npts; i++ ) {
    while ( ( interp_index < iparm->nint ) &&
	    ( i > iparm->nbt[interp_index] ) ) {
      interp_index++;
    }

    if ( x[i] == ebin_0 ) {
      *ret_vec0 = y[i] / 2;
    }

    if ( x[i] == ebin_last ) {
      *ret_vec_last = y[i] / 2;
    }
  }

  return(ierr);
}

/********************************************************************************/
static double erfn( double sign, double x )
{

  int i;
  int nmx;
  double eps = 1e-12;
  double pi = 3.1415926536;
  double sum = 0.0;
  double term;
  double return_value = 0.0;

  if ( sign > 0 ) {
    /* Normal error function */
    /*?*/
    return_value = erf(x);
  } else {
    nmx = fabs(x) + 0.5;
    nmx *= 20;
    sum = x / 2;
    
    for ( i = 0; i < nmx; i++ ) {
      term = exp( -i * i/4 ) * sinh(i * x)/i;
      sum += term;
      if ( fabs(term/sum) <= eps ) {
	return_value = 2/pi * sum;
	break;
      }
    }
  }

  return(return_value);
}

/*********************************************************************/
