
/*
 * File:	ndi_interp_integ.c
 * Description:	Internal functions for integration and interpolation
 * Time-stamp:	<01/01/24 11:07:11 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"


static double erfn(double sign, double x);


/*********************************************************************/
/*********************************************************************/
int
ndi_integrate_bins(double *distr_x, double *distr_y,
		   interp_params *interp, double *bin_bound,
		   int num_bins, double *return_vec, int return_size)

     /*  The x, y points are assumed to be monotonically increasing in 
        x.  The bin boundaries are assumed to be monotonically decreasing.
        Integration will take place from low x values to high; so the
        distribution points will start at index 0 and be incremented, 
        while the bin boundary index will start at num_bins - 1 and be
        decremented.  

        The indices stored in the nbt array are one-based.  That's why
        everytime this is accessed, one is subtracted to convert to
        zero-based indexing.

        Note on bin and bin boundary indices.  The same index value
        can be used to index a bin and its high-energy boundary.
        For example, 
        return_vec[0] is the integrated value in the highest-energy bin,
        bin_bound[0] is the high-energy boundary of this bin,
        bin_bound[1] is the low-energy boundary of this bin.
      */
{

    int ierr = 0;

    /* Distribution variables */
    int curr_distr_index = 0;
    int num_pts = interp->npts;
    int num_interp_regions = interp->nint;
    int interp_region = 0;
    int interp_type = interp->int_type[interp_region];
    int interp_bound_index = interp->nbt[interp_region] - 1;	/* Convert to zero-based index */

    /* Bin variables */
    int curr_bin_index = num_bins - 1;

    /* Integration variables */
    double integral_val;
    double lo_integr_limit = distr_x[curr_distr_index];
    double hi_integr_limit = lo_integr_limit;


    /* Initialize return_vec */
    ierr = ndi_init_double_vec(return_vec, return_size);


    /*  FIND BEGINNING BIN */
    while ((distr_x[curr_distr_index] >= bin_bound[curr_bin_index]) &&
	   (curr_bin_index > 0)) {
	curr_bin_index--;
    }

    if (interp_type != NDI_INTERP_DISCRETE) {
	curr_distr_index++;
    }
    /* check interp already? */


    /* MAIN LOOP - Integrate distribution points into bins */
    while (curr_distr_index < num_pts) {

	if ((curr_bin_index == 0) ||
	    (distr_x[curr_distr_index] <= bin_bound[curr_bin_index])) {

	    /* Integrate up to current distribution point */
	    hi_integr_limit = distr_x[curr_distr_index];

	} else {

	    /* Integrate up to current bin boundary */
	    hi_integr_limit = bin_bound[curr_bin_index];
	}

/*     printf("lo=%e  hi=%e E=%e  P=%e bin=%e  E=%e",  
	   lo_integr_limit, hi_integr_limit, distr_x[curr_distr_index], distr_y[curr_distr_index], 
	   bin_bound[curr_bin_index], ( hi_integr_limit-lo_integr_limit)*distr_y[curr_distr_index-1]);
*/

	/*  For discrete, hi_integr_limit is always the current point */
	ierr = new_integrate(distr_x, distr_y, num_pts, interp_type,
			     curr_distr_index,
			     lo_integr_limit, hi_integr_limit,
			     &integral_val);

	if ((ierr >= 0) && (curr_bin_index < return_size) && (curr_bin_index >= 0)) {	/* Redundant check? */

	    /*  ADD SPECIAL CHECK FOR DISCRETE */

	    /*  COMMENT MORE HERE */
	    if ((interp_type == NDI_INTERP_DISCRETE) &&
		(((curr_bin_index > 0) &&
		  (double_compare
		   (hi_integr_limit, bin_bound[curr_bin_index]) == 0))
		 || ((curr_bin_index < num_bins - 1)
		     &&
		     (double_compare
		      (hi_integr_limit,
		       bin_bound[curr_bin_index + 1]) == 0)))) {

		return_vec[curr_bin_index] += integral_val / 2;
		if (double_compare
		    (hi_integr_limit, bin_bound[curr_bin_index]) == 0) {
		    return_vec[curr_bin_index - 1] += integral_val / 2;
		} else {
		    return_vec[curr_bin_index + 1] += integral_val / 2;
		}
	    } else {
		return_vec[curr_bin_index] += integral_val;
	    }

	}



	/* else ?? */
	/* CHECK TO SEE IF WE MUST MOVE TO THE NEXT BIN */
	if ((curr_bin_index > 0) &&
	    (double_compare(hi_integr_limit, bin_bound[curr_bin_index]) ==
	     0)) {
	    curr_bin_index--;
	}

	/* CHECK TO SEE IF WE MUST MOVE TO THE NEXT DISTRIBUTION POINT */
	if (double_compare(hi_integr_limit, distr_x[curr_distr_index]) ==
	    0) {
	    curr_distr_index++;
	}

	/* CHECK TO SEE IF WE'VE MOVED TO THE NEXT INTERPOLATION REGION */
	if (curr_distr_index < num_pts) {

	    /*  ERROR CHECK HERE IF NOT MONOTONICALLY INCREASING INTERP BOUNDARIES */
	    while (curr_distr_index > interp_bound_index) {
		interp_region++;
		if (interp_region < num_interp_regions) {
		    interp_type = interp->int_type[interp_region];
		    interp_bound_index = interp->nbt[interp_region];	/* Convert to zero-based index */
		} else {
		    /* Error? */
		    break;
		}
	    }			/* while */
	}

	/* UPDATE LOWER INTEGRATION LIMIT */
	lo_integr_limit = hi_integr_limit;

    }				/* while */



    return (ierr);
}

/*********************************************************************/
/*******************************************************************/
int
find_average_E(double *x, double *y, interp_params *iparm, double *ret_val)
{


    int ierr = 0;
    int start_index;
    int pt_index;
    int interp_index = 0;
    int num_interp_regions = iparm->nint;
    int npts = iparm->npts;
    int int_type = iparm->int_type[0];
    double xstart;
    double xend;
    double tmp_val;
    double y_integr = 0.0;
    double xy_integr = 0.0;

    *ret_val = 0.0;

    if (int_type == NDI_INTERP_DISCRETE) {
	/* Discrete -- need npts number of passes; one for each point */
	start_index = 0;
    } else {
	/* Need npts - 1 number of passes; one for each interval */
	start_index = 1;
    }

    xstart = x[0];
    for (pt_index = start_index; pt_index < npts; pt_index++) {

	/* Check if we're in the right interpolation region */
	while ((pt_index > iparm->nbt[interp_index]) &&
	       (interp_index < (num_interp_regions - 1))) {
	    interp_index++;
	    int_type = iparm->int_type[interp_index];
	}

	xend = x[pt_index];
	ierr = fabs(new_integrate_xy(x, y, npts, int_type,
				     pt_index, xstart, xend, &tmp_val));
	/* Check for error else */
	xy_integr += tmp_val;
	ierr = fabs(new_integrate(x, y, npts, int_type,
				  pt_index, xstart, xend, &tmp_val));
	/* Check for error else */
	y_integr += tmp_val;

	/* Update xstart */
	xstart = xend;

    }

    if (y_integr != 0) {
	*ret_val = xy_integr / y_integr;
    }

    return (ierr);
}

/*******************************************************************/
/*********************************************************************/
int
new_integrate(double *x, double *y, int npts, int interp_type,
	      int curr_x_index,
	      double xstart, double xend, double *return_val)

     /* This function should only be used to integrate regions between
        two distribution points.  It should not be used to integrate
        across the whole distribution, for instance, since only two
        (three for hale) distribution points are used in the integration.
        The calling function should walk point-by-point across the
        distribution and call this function multiple times.
      */
     /* For discrete, x_end is always the current probability to add */
     /* curr_x_index points to the high-E distribution point bounding 
        integration region.
        Is it possible that curr_x_index <= 0 or curr_x_index >= npts? */
     /* interp_type is an integer indicating what type of interpolation is to
        be used for the data.
        0=discrete,
        1=histogram, 2=lin-lin, 3=lin-log(y is linear in ln(x)),
        4=log-lin( ln(y) is linear in x), 5=log-log
      */
{

    typedef double (*integration_function_type) (double, double, double *,
						 double *);

    int ierr = 0;

    static integration_function_type integration_function[7];
    static int initialized = 0;
    double xpts[3], ypts[3];	/* Containers to hold x & y distr pts for types 0 - 6 */

    if (!initialized) {
	initialized = 1;
	integration_function[NDI_INTERP_DISCRETE] = &discrete_integrate;
	integration_function[NDI_INTERP_HISTOGRAM] = &histogram_integrate;
	integration_function[NDI_INTERP_LIN_LIN] = &lin_lin_integrate;
	integration_function[NDI_INTERP_LIN_LOG] = &lin_log_integrate;
	integration_function[NDI_INTERP_LOG_LIN] = &log_lin_integrate;
	integration_function[NDI_INTERP_LOG_LOG] = &log_log_integrate;
	integration_function[NDI_INTERP_HALE] = &hale_integrate;
    }

    *return_val = 0.0;

    /* SPECIAL CASE:  x1 == x2 */
    /* !!!UNLESS DISCRETE */
    if ((interp_type != NDI_INTERP_DISCRETE) &&
	(double_compare(xstart, xend) == 0)) {
	return (ierr);
    }

    /* Need to check order of xstart and xend points? */


    /* Find distribution points needed for integration and integrate */
    if (interp_type == NDI_INTERP_DISCRETE) {
	/* discrete */
	xpts[0] = x[curr_x_index];
	ypts[0] = y[curr_x_index];
    } else if ((interp_type > 0) && (interp_type < 6)) {
	xpts[0] = x[curr_x_index - 1];
	xpts[1] = x[curr_x_index];
	ypts[0] = y[curr_x_index - 1];
	ypts[1] = y[curr_x_index];
    } else if (interp_type == 6) {
	if (curr_x_index == 0) {
	    xpts[0] = x[curr_x_index];
	    ypts[0] = y[curr_x_index];
	    xpts[1] = x[curr_x_index + 1];
	    ypts[1] = y[curr_x_index + 1];
	    xpts[2] = x[curr_x_index + 2];
	    ypts[2] = y[curr_x_index + 2];
	} else if ((curr_x_index == 1) || ((curr_x_index < (npts - 1)) &&	/* Not at the last point */
					   ((xstart -
					     x[curr_x_index - 1]) >
					    DBL_EPSILON)
					   /* Empirically, this seems to work better */
		   )) {
	    xpts[0] = x[curr_x_index - 1];
	    ypts[0] = y[curr_x_index - 1];
	    xpts[1] = x[curr_x_index];
	    ypts[1] = y[curr_x_index];
	    xpts[2] = x[curr_x_index + 1];
	    ypts[2] = y[curr_x_index + 1];
	} else {
	    xpts[0] = x[curr_x_index - 2];
	    ypts[0] = y[curr_x_index - 2];
	    xpts[1] = x[curr_x_index - 1];
	    ypts[1] = y[curr_x_index - 1];
	    xpts[2] = x[curr_x_index];
	    ypts[2] = y[curr_x_index];
	}
    } else {
	/* ???????? */
    }
    *return_val =
	integration_function[interp_type] (xstart, xend, xpts, ypts);
/*   printf("   I=%e\n", *return_val ); */
    return (ierr);

}



/*******************************************************************/
/*********************************************************************/
int
new_integrate_xy(double *x, double *y, int npts, int interp_type,
		 int curr_x_index,
		 double xstart, double xend, double *return_val)

     /* Integrates the function x * f(x)  -- where f(x) = y here */
     /* This function should only be used to integrate regions between
        two distribution points.  It should not be used to integrate
        across the whole distribution, for instance, since only two
        (three for hale) distribution points are used in the integration.
        The calling function should walk point-by-point across the
        distribution and call this function multiple times.
      */
     /* curr_x_index points to the high-E distribution point bounding 
        integration region.
        Is it possible that curr_x_index <= 0 or curr_x_index >= npts? */
     /* interp_type is an integer indicating what type of interpolation is to
        be used for the data.
        0=discrete,
        1=histogram, 2=lin-lin, 3=lin-log(y is linear in ln(x)),
        4=log-lin( ln(y) is linear in x), 5=log-log
      */
{

    typedef double (*integration_function_type) (double, double, double *,
						 double *);

    int ierr = 0;

    static integration_function_type integration_function[7];
    static int initialized = 0;
    double xpts[3], ypts[3];	/* Containers to hold x & y distr pts for types 0 - 6 */

    if (!initialized) {
	initialized = 1;
	integration_function[NDI_INTERP_DISCRETE] = &discrete_integrate_x;
	integration_function[NDI_INTERP_HISTOGRAM] =
	    &histogram_integrate_x;
	integration_function[NDI_INTERP_LIN_LIN] = &lin_lin_integrate_x;
	integration_function[NDI_INTERP_LIN_LOG] = &lin_log_integrate_x;
	integration_function[NDI_INTERP_LOG_LIN] = &log_lin_integrate_x;
	integration_function[NDI_INTERP_LOG_LOG] = &log_log_integrate_x;
	integration_function[NDI_INTERP_HALE] = &hale_integrate_x;
    }

    *return_val = 0.0;

    /* SPECIAL CASE:  x1 == x2 */
    /* !!!UNLESS DISCRETE */
    if ((interp_type != NDI_INTERP_DISCRETE) &&
	(double_compare(xstart, xend) == 0)) {
	return (ierr);
    }

    /* Need to check order of xstart and xend points? */


    /* Find distribution points needed for integration and integrate */
    if (interp_type == NDI_INTERP_DISCRETE) {
	/* discrete */
	xpts[0] = x[curr_x_index];
	ypts[0] = y[curr_x_index];
    } else if ((interp_type > 0) && (interp_type < 6)) {
	xpts[0] = x[curr_x_index - 1];
	xpts[1] = x[curr_x_index];
	ypts[0] = y[curr_x_index - 1];
	ypts[1] = y[curr_x_index];
    } else if (interp_type == 6) {
	if (curr_x_index == 0) {
	    xpts[0] = x[curr_x_index];
	    ypts[0] = y[curr_x_index];
	    xpts[1] = x[curr_x_index + 1];
	    ypts[1] = y[curr_x_index + 1];
	    xpts[2] = x[curr_x_index + 2];
	    ypts[2] = y[curr_x_index + 2];
	} else if ((curr_x_index == 1) || ((curr_x_index < (npts - 1)) &&	/* Not at the last point */
					   ((xstart -
					     x[curr_x_index - 1]) >
					    DBL_EPSILON)
					   /* Empirically, this seems to work better */
		   )) {
	    xpts[0] = x[curr_x_index - 1];
	    ypts[0] = y[curr_x_index - 1];
	    xpts[1] = x[curr_x_index];
	    ypts[1] = y[curr_x_index];
	    xpts[2] = x[curr_x_index + 1];
	    ypts[2] = y[curr_x_index + 1];
	} else {
	    xpts[0] = x[curr_x_index - 2];
	    ypts[0] = y[curr_x_index - 2];
	    xpts[1] = x[curr_x_index - 1];
	    ypts[1] = y[curr_x_index - 1];
	    xpts[2] = x[curr_x_index];
	    ypts[2] = y[curr_x_index];
	}
    } else {
	/* ???????? */
    }
    *return_val =
	integration_function[interp_type] (xstart, xend, xpts, ypts);
/*   printf("   I=%e\n", *return_val ); */
    return (ierr);

}



/*********************************************************************/
/********************************************************************************/

int ndi_init_double_vec(double *vec, int num_elements)
{

    int ierr = 0;

    while (num_elements--) {
	*vec++ = 0.0;
    }

    return (ierr);
}

/********************************************************************************/

/*********************************************************************/
/*                        INTERPOLATION                              */
/*********************************************************************/


double
histogram_interp(double xin, double x1, double y1, double x2, double y2)
{
    /* Interp scheme = 1 */

    double result = 0.0;

    /* Check that x1 <= xin <= x2 */
    result = y1;
    return (result);

}

/*********************************************************************/


double
lin_lin_interp(double xin, double x1, double y1, double x2, double y2)
{
    /* Interp scheme = 2 */
    /* Check for x2 = x1 */

    double result = 0.0;

    if (fabs(x2 - x1) < DBL_EPSILON) {
	result = y1;
    } else {
	result = y1 + ((y2 - y1) * (xin - x1) / (x2 - x1));
    }
    return (result);

}

/*********************************************************************/

double
log_log_interp(double xin, double x1, double y1, double x2, double y2)
{
    /* Interp scheme = 5 */

    double result = 0.0;

    if (fabs(x1) < DBL_EPSILON ||
	fabs(x2) < DBL_EPSILON ||
	fabs(xin) < DBL_EPSILON ||
	fabs(y1) < DBL_EPSILON || fabs(y2) < DBL_EPSILON) {
	result = 0.0;
    } else if (fabs((log(x2) - log(x1)) * (log(y2) - log(y1))) <
	       DBL_EPSILON) {
	result = y1;
    } else {
	result =
	    exp(log(y1) +
		(log(xin) - log(x1)) / (log(x2) - log(x1)) * (log(y2) -
							      log(y1)));
    }
    return (result);

}

/*********************************************************************/
double
lin_log_interp(double xin, double x1, double y1, double x2, double y2)
{
    /* Interp scheme = 3 */

    double result = 0.0;

    if (fabs(x1) < DBL_EPSILON ||
	fabs(x2) < DBL_EPSILON || fabs(xin) < DBL_EPSILON) {
	result = 0.0;
    } else if (fabs((log(x2) - log(x1)) * (y2 - y1)) < DBL_EPSILON) {
	result = y1;
    } else {
	result =
	    y1 + (log(xin) - log(x1)) / (log(x2) - log(x1)) * (y2 - y1);
    }
    return (result);

}

/*********************************************************************/
double
log_lin_interp(double xin, double x1, double y1, double x2, double y2)
{
    /* Interp scheme = 4 */

    double result = 0.0;

    if (fabs(y1) < DBL_EPSILON || fabs(y2) < DBL_EPSILON) {
	result = 0.0;
    } else if (fabs((x2 - x1) * (log(y2) - log(y1))) < DBL_EPSILON) {
	result = y1;
    } else {
	result =
	    exp(log(y1) + (xin - x1) / (x2 - x1) * (log(y2) - log(y1)));
    }
    return (result);

}

/*********************************************************************/
double
hale_interp(double xin, double x1, double y1, double x2,
	    double y2, double x3, double y3)
{

    /* Interp scheme = 6
       Gerry Hale's function for secondary energy distributions:
       y=a*sqrt(E)*exp(-1*(sqrt(E) - b)^2/c)  where E is energy
     */

    double result = 0.0;
    double a, b, c;
    double del;
    double dif1, dif2;
    double f1, f2, f3;
/*    double coef1, coef2; */

    if (xin <= 0 ||
	x1 <= 0 || x2 <= 0 || x3 <= 0 || y1 == 0 || y2 == 0 || y3 == 0) {
	return (0.0);
    }

    /* Check for negative values in sqrt and zeroes in denom */
    f1 = log(y1 / sqrt(x1));
    f2 = log(y2 / sqrt(x2));
    f3 = log(y3 / sqrt(x3));
    dif1 = f2 - (f1 + f3) / 2;
    dif2 = (f1 - f3) / 2;
    del = sqrt(x2) - sqrt(x1);

    /*
       coef1 = f2 - f1;
       coef2 = coef1 / (f3 - f1);
     */

    if (fabs(2 * del) < DBL_EPSILON || fabs(4 * dif1) < DBL_EPSILON) {
	return (0.0);
    }


    c = (del * del) / dif1;
    b = sqrt(x2) - c * dif2 / (2 * del);
    a = exp(f2 + dif2 * dif2 / (4 * dif1));


    /*
       b = ( coef2 * (sqrt(x1) + sqrt(x3)) * (sqrt(x1) - sqrt(x3)) - 
       (sqrt(x1) + sqrt(x2)) * ( sqrt(x1) - sqrt(x2) ) ) /
       ( 2 * coef2 * ( sqrt(x1) - sqrt(x3) ) - 2 * ( sqrt(x1) - sqrt(x2) ) );
       c = ( (sqrt(x1) + sqrt(x2) - 2 * b) * ( sqrt(x1) - sqrt(x2) ) ) / coef1;
       a = y2 * exp( pow((sqrt(x2) - b), 2)/ c ) / sqrt(x2);
     */

/*    printf( "     A=%e, B= %e;        a=%e, b=%e, c=%e\n", A, B, a, b, c);  */
    result = a * sqrt(xin) * exp(-1 * pow((sqrt(xin) - b), 2) / c);

    return (result);

}

/*********************************************************************/
/*                        INTEGRATION                                */
/*                       Integrate y(E)                              */
/*********************************************************************/
double
histogram_integrate(double xstart, double xend, double *x, double *y)
{
    /* Interp scheme = 1  (y1 = y2) */

    /* Assume that (x[0] <= xstart < xend <= x[1]) */
    /* What if y < 0 ?????? */

    double ystart;

    ystart = histogram_interp(xstart, x[0], y[0], x[1], y[1]);
    return (fabs((xend - xstart) * ystart));

}

/*********************************************************************/
double lin_lin_integrate(double xstart, double xend, double *x, double *y)
{
    /* Interp scheme = 2 */

    /* Assume that (x[0] <= xstart < xend <= x[1]) */
    /* What if y < 0 ?????? */

    double width = (xend - xstart);
    double ystart = 0.0;
    double yend = 0.0;

    ystart = lin_lin_interp(xstart, x[0], y[0], x[1], y[1]);
    yend = lin_lin_interp(xend, x[0], y[0], x[1], y[1]);

    return (fabs((ystart + yend) * width / 2.0));

}

/*********************************************************************/
double log_log_integrate(double xstart, double xend, double *x, double *y)
{
    /* Interp scheme = 5 */


    /* Assume that (x[0] <= xstart < xend <= x[1]) */
    /* What if y < 0 ?????? */

    double ystart = 0.0;
    double yend = 0.0;
    double alpha;
    double beta;
    double return_val;

    /* Assume that (x[0] <= xstart < xend <= x[1]) */
    /* What if y < 0 ?????? */
    /* Check that xend != xstart */

    ystart = log_log_interp(xstart, x[0], y[0], x[1], y[1]);
    yend = log_log_interp(xend, x[0], y[0], x[1], y[1]);

    /* check that xstart,xend,ystart,yend not equal 0 */
    if ((double_compare(xstart, xend) == 0) ||
	fabs(xstart) < DBL_EPSILON ||
	fabs(xend) < DBL_EPSILON ||
	fabs(ystart) < DBL_EPSILON || fabs(yend) < DBL_EPSILON) {
	return (0.0);
    } else {
	beta = (log(yend / ystart)) / (log(xend / xstart));
	alpha = log(ystart) - beta * log(xstart);
    }

    if (beta != -1) {
	return_val = (exp(alpha + (beta + 1) * log(xstart)) / (beta + 1)) *
	    (pow((xend / xstart), (beta + 1)) - 1.0);
    } else {
	return_val = exp(alpha) * log(xend / xstart);
    }

    return (return_val);

}

/*********************************************************************/
double lin_log_integrate(double xstart, double xend, double *x, double *y)
{
    /* Interp scheme = 3 (y is linear in the ln of x) */

    double ystart = 0.0;
    double yend = 0.0;
    double alpha;
    double beta;
    double return_val;


    ystart = lin_log_interp(xstart, x[0], y[0], x[1], y[1]);
    yend = lin_log_interp(xend, x[0], y[0], x[1], y[1]);

    /* check that xstart, xend not equal 0 and xstart !=  xend */
    /* Check that xstart, xend, and xend/xstart >= 0 !!!!!!!! */
    if ((double_compare(xstart, xend) == 0) ||
	fabs(xstart) < DBL_EPSILON ||
	fabs(xend) < DBL_EPSILON ||
	fabs(log(xend / xstart)) < DBL_EPSILON) {
	return (0.0);
    } else {
	beta = (yend - ystart) / (log(xend / xstart));
	alpha = ystart - beta * log(xstart);
    }

    return_val =
	alpha * (xend - xstart) + beta * ((xend * log(xend) - xend)
					  - (xstart * log(xstart) -
					     xstart));
    return (return_val);

}

/*********************************************************************/
double log_lin_integrate(double xstart, double xend, double *x, double *y)
{
    /* Interp scheme = 4 */

    double ystart = 0.0;
    double yend = 0.0;
    double alpha;
    double beta;
    double return_val;


    ystart = log_lin_interp(xstart, x[0], y[0], x[1], y[1]);
    yend = log_lin_interp(xend, x[0], y[0], x[1], y[1]);

    /* check that xstart,xend,ystart,yend not equal 0 */
    if ((double_compare(xstart, xend) == 0) ||
	fabs(ystart) < DBL_EPSILON || fabs(yend) < DBL_EPSILON) {
	return (0.0);
    } else {
	beta = (log(yend / ystart)) / (xend - xstart);
	alpha = log(ystart) - beta * xstart;
    }

    if (fabs(beta) < DBL_EPSILON) {
	return_val = (ystart * (xend - xstart));	/* What to do here????  */
    } else {
	return_val =
	    (exp(alpha + beta * xstart) / beta) *
	    (exp(beta * (xend - xstart)) - 1.0);
    }

    return (return_val);

}

/*********************************************************************/
double hale_integrate(double xstart, double xend, double *x, double *y)
{

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
/*    double coef1, coef2; */
    double fac1, fac2;
    double exp2, erf2;
    double u1, u2;
    double snc;
    double pi = 3.1415926536;	/* pi given exactly as in Gerry Hale's program */


    /* Check for negative values in sqrt and zeroes in denom */
    if ((double_compare(xstart, xend) == 0) || x[0] <= 0 ||
	x[1] <= 0 || x[2] <= 0 || y[0] == 0 || y[1] == 0 || y[2] == 0) {
	return (return_val);
    }

    f1 = log(y[0] / sqrt(x[0]));
    f2 = log(y[1] / sqrt(x[1]));
    f3 = log(y[2] / sqrt(x[2]));
    dif1 = f2 - (f1 + f3) / 2;
    dif2 = (f1 - f3) / 2;
    del = sqrt(x[1]) - sqrt(x[0]);

    /*
       coef1 = f2 - f1;
       coef2 = coef1 / (f3 - f1);
     */

    if (fabs(2 * del) < DBL_EPSILON || fabs(4 * dif1) < DBL_EPSILON) {
	return (0.0);
    }

    c = (del * del) / dif1;
    b = sqrt(x[1]) - c * dif2 / (2 * del);
    a = exp(f2 + dif2 * dif2 / (4 * dif1));

    /*
       b = ( coef2 * (sqrt(x[0]) + sqrt(x[2])) * (sqrt(x[0]) - sqrt(x[2])) - 
       (sqrt(x[0]) + sqrt(x[1])) * ( sqrt(x[0]) - sqrt(x[1]) ) ) /
       ( 2 * coef2 * ( sqrt(x[0]) - sqrt(x[2]) ) - 2 * ( sqrt(x[0]) - sqrt(x[1]) ) );
       c = ( (sqrt(x[0]) + sqrt(x[1]) - 2 * b) * ( sqrt(x[0]) - sqrt(x[1]) ) ) / coef1;
       a = y[1] * exp( pow((sqrt(x[1]) - b), 2)/ c ) / sqrt(x[1]);
     */


    fac1 = a * c;
    fac2 = (b * b + c / 2) * sqrt(pi / fabs(c));
    snc = c / fabs(c);
    u1 = snc * (sqrt(xstart) - b) / sqrt(fabs(c));
    u2 = snc * (sqrt(xend) - b) / sqrt(fabs(c));
    exp2 = (sqrt(xend) + b) * exp(-snc * u2 * u2);
    erf2 = erfn(snc, u2);

    return_val =
	fac1 * ((sqrt(x[0]) + b) * exp(-snc * u1 * u1) - exp2 -
		fac2 * (erfn(snc, u1) - erf2));

    return (return_val);

}

/*********************************************************************/
double discrete_integrate(double xstart, double xend, double *x, double *y)
{
    /* Interpolation scheme = 0 */
    /*  xend is always the current probability to add */

    /* This is assumed to be merely summation
       --so return the y value if xend == x[0] */

    /* Assume that xstart < xend;
       x is given in ascending order */

    double result = 0.0;

    if (double_compare(xend, x[0]) == 0) {
	result = y[0];
    }


    return (result);

}

/*********************************************************************/
/*                        INTEGRATION                                */
/*                    Integrate E * y(E)                             */
/*********************************************************************/

double
histogram_integrate_x(double xstart, double xend, double *x, double *y)
{
    /* Interp scheme = 1  (ystart = yend) */

    /* Assume that (x[0] <= xstart < xend <= x[1]) */
    /* What if y < 0 ?????? */

    double ystart;

    ystart = histogram_interp(xstart, x[0], y[0], x[1], y[1]);
    return (fabs(ystart * (xend + xstart) * (xend - xstart) / 2));
}

/*********************************************************************/
double
lin_lin_integrate_x(double xstart, double xend, double *x, double *y)
{
    /* Interp scheme = 1  (ystart = yend) */
    /* Interp scheme = 2 */

    /* Where y(x) = alpha + beta * E */

    double ystart = 0.0;
    double yend = 0.0;
    double alpha;
    double beta;
    double return_val;

    /* Assume that (x[0] <= xstart < xend <= x[1]) */
    /* What if y < 0 ?????? */
    /* Check that xend != xstart */

    ystart = lin_lin_interp(xstart, x[0], y[0], x[1], y[1]);
    yend = lin_lin_interp(xend, x[0], y[0], x[1], y[1]);

    if (double_compare(xstart, xend) == 0) {
	return (0.0);
    } else {
	alpha = (ystart * xend - yend * xstart) / (xend - xstart);
	beta = (yend - ystart) / (xend - xstart);
    }

    return_val = alpha * (xend * xend - xstart * xstart) / 2.0
	+ beta * (xend * xend * xend - xstart * xstart * xstart) / 3.0;

/*    printf("    INTEGRATE e*y(e)  (lin-lin):  a=%e b=%e int=%e\n", alpha,beta,return_val);    */
    return (return_val);
}

/*********************************************************************/
double
log_log_integrate_x(double xstart, double xend, double *x, double *y)
{
    /* Interp scheme = 5 */

    /* Assume that (x[0] <= xstart < xend <= x[1]) */
    /* What if y < 0 ?????? */
    /* Where ln(y(x)) = alpha + beta * ln(x)  OR
       y = exp(alpha) * x ^ (beta)   */


    double ystart = 0.0;
    double yend = 0.0;
    double alpha;
    double beta;
    double return_val;

    ystart = log_log_interp(xstart, x[0], y[0], x[1], y[1]);
    yend = log_log_interp(xend, x[0], y[0], x[1], y[1]);

    /* check that xstart,xend,ystart,yend not equal 0 and xstart != xend */
    if ((double_compare(xstart, xend) == 0) ||
	fabs(xstart) < DBL_EPSILON ||
	fabs(xend) < DBL_EPSILON ||
	fabs(ystart) < DBL_EPSILON ||
	fabs(yend) < DBL_EPSILON ||
	fabs(log(xend / xstart)) < DBL_EPSILON) {
	return (0.0);
    } else {
	beta = (log(yend / ystart)) / (log(xend / xstart));
	alpha = log(ystart) - beta * log(xstart);
    }

    if (beta != -2) {
	return_val = (exp(alpha + (beta + 2) * log(xstart)) / (beta + 2)) *
	    (pow((xend / xstart), (beta + 2)) - 1.0);
    } else {
	return_val = exp(alpha) * log(xend / xstart);
    }

    return (return_val);

}

/*********************************************************************/
double
lin_log_integrate_x(double xstart, double xend, double *x, double *y)
{
    /* Interp scheme = 3 (y is linear in the ln of x) */

    /* Where y(x) = alpha + beta * ln(x)  */

    double ystart = 0.0;
    double yend = 0.0;
    double alpha;
    double beta;
    double return_val;


    ystart = lin_log_interp(xstart, x[0], y[0], x[1], y[1]);
    yend = lin_log_interp(xend, x[0], y[0], x[1], y[1]);

    /* check that xstart,xend not equal 0 and xstart !=  xend */
    if ((double_compare(xstart, xend) == 0) ||
	fabs(xstart) < DBL_EPSILON ||
	fabs(xend) < DBL_EPSILON ||
	fabs(log(xend / xstart)) < DBL_EPSILON) {
	return (0.0);
    } else {
	beta = (yend - ystart) / (log(xend / xstart));
	alpha = ystart - beta * log(xstart);
    }

    return_val = alpha * (xend * xend - xstart * xstart) / 2.0
	+ beta * (xend * xend * (log(xend) - 0.5)
		  - xstart * xstart * (log(xstart) - 0.5)) / 2.0;
/*    printf("    INTEGRATE e*y(e)  (lin-log):  a=%e b=%e int=%e\n", alpha,beta,return_val);    */

    return (return_val);

}

/*********************************************************************/
double
log_lin_integrate_x(double xstart, double xend, double *x, double *y)
{
    /* Interp scheme = 4 */

    double ystart = 0.0;
    double yend = 0.0;
    double alpha;
    double beta;
    double return_val;


    ystart = lin_log_interp(xstart, x[0], y[0], x[1], y[1]);
    yend = lin_log_interp(xend, x[0], y[0], x[1], y[1]);

    /* check that xstart,xend,ystart,yend not equal 0 */
    if ((double_compare(xstart, xend) == 0) ||
	fabs(ystart) < DBL_EPSILON || fabs(yend) < DBL_EPSILON) {
	return (0.0);
    } else {
	beta = (log(yend / ystart)) / (xend - xstart);
	alpha = log(ystart) - beta * xstart;
    }

    if (fabs(beta * beta) < DBL_EPSILON) {
	return_val = fabs(ystart * (xend + xstart) * (xend - xstart) / 2);	/* What to do here????  */
    } else {
	return_val = (exp(alpha + beta * xstart) / (beta * beta))
	    * (exp(beta * (xend - xstart)) * (beta * xend - 1.0) -
	       (beta * xstart - 1.0));
    }
/*    printf("    INTEGRATE e*y(e)  (log-lin):  a=%e b=%e int=%e\n", alpha,beta,return_val);    */
    return (return_val);

}

/*********************************************************************/
double hale_integrate_x(double xstart, double xend, double *x, double *y)
{

    /* Interp scheme = 6

       Assume that (x[0] <= xstart < xend <= x[2]) and (x[0] < x[1] < x[2]).

       Gerry Hale's function for secondary energy distributions:
       y=a*sqrt(E)*exp(-1*(sqrt(E) - b)^2/c)  where E is energy

       This function integrates between xstart and xend?
       The variables have been kept the same as in G. Hale's code ?.  However,
       a changes has been made: 
       1) in this code x[0],x[1],x[2] are energy values; in Gerry's codes x[0]=sqrt(E1), 
       x[1]=sqrt(E2), x[2]=sqrt(E3) ,where E1,E2,E3 are the energy values and 
       2) u1 and u2 in this code correspond to y[0] and y[1] in Gerry's code.

     */

    double return_val = 0.0;
/*    double a; */
    double b, c;
    double del;
    double dif1;
    /*    double dif2; */
    double f1, f2, f3;
    double coef1, coef2;
    /*    double fac1, fac2; */
    /*    double exp2, erf2; */
    double u1, u2;
    double snc;
    double A1, A2, A3, A4, B;
    double pi = 3.1415926536;	/* pi given exactly as in Gerry Hale's program */

    /* Check for negative values in sqrt and zeroes in denom */
    if ((double_compare(xstart, xend) == 0) || x[0] <= 0 ||
	x[0] <= 0 ||
	x[1] <= 0 || x[2] <= 0 || y[0] == 0 || y[1] == 0 || y[2] == 0) {
	return (0.0);
    }
    f1 = log(y[0] / sqrt(x[0]));
    f2 = log(y[1] / sqrt(x[1]));
    f3 = log(y[2] / sqrt(x[2]));
    dif1 = f2 - (f1 + f3) / 2;
/*    dif2 = ( f1 - f3 ) / 2; */
    del = sqrt(x[1]) - sqrt(x[0]);

    coef1 = f2 - f1;
    coef2 = coef1 / (f3 - f1);


    if (fabs(2 * del) < DBL_EPSILON || fabs(4 * dif1) < DBL_EPSILON) {
	return (0.0);
    }

    /*

       if ( fabs(2 * del) < DBL_EPSILON ||
       fabs(4 * dif1) < DBL_EPSILON ) {
       return ( 0.0 );
       }
       c = (del *del)/dif1;
       b = sqrt(x[1]) - c * dif2/ ( 2 * del );
       a = exp(f2 + dif2*dif2 / ( 4 * dif1 ));
     */

    b = (coef2 * (sqrt(x[0]) + sqrt(x[2])) * (sqrt(x[0]) - sqrt(x[2])) -
	 (sqrt(x[0]) + sqrt(x[1])) * (sqrt(x[0]) - sqrt(x[1]))) /
	(2 * coef2 * (sqrt(x[0]) - sqrt(x[2])) -
	 2 * (sqrt(x[0]) - sqrt(x[1])));
    c = ((sqrt(x[0]) + sqrt(x[1]) - 2 * b) * (sqrt(x[0]) -
					      sqrt(x[1]))) / coef1;
/*    a = y[1] * exp( pow((sqrt(x[1]) - b), 2)/ c ) / sqrt(x[1]); */


    /* A IS NEVER USED!!!!!!!  Must be a problem */
    snc = c / fabs(c);
    u1 = snc * (sqrt(xstart) - b) / sqrt(fabs(c));
    u2 = snc * (sqrt(xend) - b) / sqrt(fabs(c));

    /* coefficients */
    A1 = -2 * b * sqrt(c) * (b * b + c);
    A2 = -3 * c * (4 * b * b + c) / 4;
    A3 = -2 * b * sqrt(pow(c, 3));
    A4 = c * c / -2;
    B = sqrt(pi) * (3 * c * c + 12 * b * b * c + 4 * pow(b, 4)) / 8;

    return_val =
	(A1 + A2 * u2 + A3 * u2 * u2 + A4 * pow(u2, 3) / 2) * exp(-u2 * u2)
	+ B * erfn(snc, u2)
	- (A1 + A2 * u1 + A3 * u1 * u1 +
	   A4 * pow(u1, 3) / 2) * exp(-u1 * u1)
	- B * erfn(snc, u1);

    return (return_val);

}

/*********************************************************************/
double
hale_integrate_x_bak(double x1, double y1, double x2, double y2,
		     double x3, double y3)
{

    /* Interp scheme = 6
       Gerry Hale's function for secondary energy distributions:
       y=a*sqrt(E)*exp(-1*(sqrt(E) - b)^2/c)  where E is energy

       This function integrates between x1 and x2?
       The variables have been kept the same as in G. Hale's code ?.  However,
       a changes has been made: 
       1) in this code x1,x2,x3 are energy values; in Gerry's codes x1=sqrt(E1), 
       x2=sqrt(E2), x3=sqrt(E3) ,where E1,E2,E3 are the energy values and 
       2) u1 and u2 in this code correspond to y1 and y2 in Gerry's code.

     */

    double return_val = 0.0;
    /*    double a; */
    double b, c;
    double del;
    double dif1, dif2;
    double f1, f2, f3;
    /*    double fac1, fac2; */
    /*    double exp2, erf2; */
    double u1, u2;
    double snc;
    double A1, A2, A3, A4, B;
    double pi = 3.1415926536;	/* pi given exactly as in Gerry Hale's program */

    /* Check for negative values in sqrt and zeroes in denom */
    if (x1 <= 0 || x2 <= 0 || x3 <= 0 || y1 == 0 || y2 == 0 || y3 == 0) {
	return (0.0);
    }
    f1 = log(y1 / sqrt(x1));
    f2 = log(y2 / sqrt(x2));
    f3 = log(y3 / sqrt(x3));
    dif1 = f2 - (f1 + f3) / 2;
    dif2 = (f1 - f3) / 2;
    del = sqrt(x2) - sqrt(x1);

    if (fabs(2 * del) < DBL_EPSILON || fabs(4 * dif1) < DBL_EPSILON) {
	return (0.0);
    }

    c = (del * del) / dif1;
    b = sqrt(x2) - c * dif2 / (2 * del);
    /*    a = exp(f2 + dif2*dif2 / ( 4 * dif1 )); */

    snc = c / fabs(c);
    u1 = snc * (sqrt(x1) - b) / sqrt(fabs(c));
    u2 = snc * (sqrt(x2) - b) / sqrt(fabs(c));

    /* coefficients */
    A1 = -2 * b * sqrt(c) * (b * b + c);
    A2 = -3 * c * (4 * b * b + c) / 4;
    A3 = -2 * b * sqrt(pow(c, 3));
    A4 = c * c / -2;
    B = sqrt(pi) * (3 * c * c + 12 * b * b * c + 4 * pow(b, 4)) / 8;

    return_val =
	(A1 + A2 * u2 + A3 * u2 * u2 + A4 * pow(u2, 3) / 2) * exp(-u2 * u2)
	+ B * erfn(snc, u2)
	- (A1 + A2 * u1 + A3 * u1 * u1 +
	   A4 * pow(u1, 3) / 2) * exp(-u1 * u1)
	- B * erfn(snc, u1);

    return (return_val);

}

/*********************************************************************/
double
discrete_integrate_x(double xstart, double xend, double *x, double *y)
{
    /* Interpolation scheme = 0 */
    /*  xend is always the current probability to add */

    /* This is assumed to be merely summation
       --so return the y value if xend == x[0] */

    /* Assume that xstart < xend;
       x is given in ascending order */

    double result = 0.0;

    if (double_compare(xend, x[0]) == 0) {
	result = x[0] * y[0];
    }

    return (result);

}

/*********************************************************************/
static double erfn(double sign, double x)
{

    int i;
    int nmx;
    double eps = 1e-12;
    double pi = 3.1415926536;
    double sum = 0.0;
    double term;
    double return_value = 0.0;

    if (sign > 0) {
	/* Normal error function */
	/*? */
	return_value = erf(x);
    } else {
	nmx = fabs(x) + 0.5;
	nmx *= 20;
	sum = x / 2;

	for (i = 0; i < nmx; i++) {
	    term = exp(-i * i / 4) * sinh(i * x) / i;
	    sum += term;
	    if (fabs(term / sum) <= eps) {
		return_value = 2 / pi * sum;
		break;
	    }
	}
    }

    return (return_value);
}
