/*
 * File:	ndi_set_e_grps.c
 * Description:	Internal function--set the new energy group boundaries.
 * Time-stamp:	<99/11/03 17:11:51 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_state.h"

static int within_epsilon( ndi_float64 a, ndi_float64 b);
static int greater_than_epsilon(ndi_float64 a, ndi_float64 b);


/**********************************************************************/

int ndi_set_e_grps( int **new_ebound_p, const char *grp_struc, int *new_grp,
		    int old_grp, short *c_flag) {

/* Purpose:  Set an array to indicate which of the current library
   energy bounds will be used in the new (collapsed) group structure.
   
   Parameters:
   new_ebound_p -- out: array of indices indicating which of the library
                   energy boundaries are to be used in the new (collapsed)
		   energy group structure. (New boundaries subset of old ones.)
   grp_struc -- in: string indicating new group structure; valid entries are:
                 ( 30_group, 21_group, 16_group, 14_group, 13_group, 12_group, 
		 10_group, 9_group, 8_group, 7_group, 6_group, 5_group, 
		 4_group, 3_group, 2_group, 1_group, default )
   new_grp -- out: number of groups in the new (collapsed) group structure.
   old_grp -- in: number of groups in the old (library) group structure.
   c_flag -- out: 0 = no collapsing required, -1 = collapsing required

   Assumptions:
   All energy boundaries are listed in descending order.
   There is no default group structure for TN data--an error is returned.
   The global variable n_group is not set here--it is assumed to be set by 
    calling function.

   This function may be called before an old (library) group structure
   is read in (before an isotope is set).  If this is true is is assumed
   that old_grp input equals zero.  In this case only 'new_grp' is set to
   new number of groups and c_flag is 0.

   Also, if new_grp == old_grp the "collapsed" group structure may be the
   same as the old.  So the c_flag is set to zero, new_grp is set to the
   number of groups, but new_ebound_p is left NULL.

   Global Variables:
   lib_ebounds -- must be previously set to boundaries in library,
                  unless old_grp is zero
   
   Return values:  Returns negative value for error condition,
   positive value for warning, and a '0' if everything's ok.  On error,
   c_flag = 0, new_e_bound_p = NULL, new_grp = 0.

***********************************************************************/
  
   /* ndi_float64 hansenroach[] = { 15, 3, 1.4, 0.9, 0.4, 0.1, 1.7E-2, 
	                            3E-3, 5.5E-4, 1.0E-4, 3.0E-5, 1.0E-5, 
                                    3.0E-6, 1.0E-6, 4.0E-7, 1.0E-7,1.4E-10 }; 
   */
   
  
  ndi_float64 _130_lanl[] = 
    { 2.0000000E+01, 1.8900000E+01, 1.7900000E+01, 1.7000000E+01, 1.5000000E+01,
      1.3500000E+01, 1.2000000E+01, 1.0000000E+01, 7.7900000E+00, 6.0700000E+00,
      4.7240000E+00, 3.6800000E+00, 2.8650000E+00, 2.2320000E+00, 1.7380000E+00,
      1.3530000E+00, 1.1940000E+00, 1.0540000E+00, 9.3010000E-01, 8.2300000E-01,
      7.2440000E-01, 6.3930000E-01, 5.6420000E-01, 5.0000000E-01, 4.3940000E-01,
      3.8770000E-01, 3.0300000E-01, 2.3520000E-01, 1.8400000E-01, 1.4260000E-01,
      1.1110000E-01, 8.6520000E-02, 6.7600000E-02, 5.2480000E-02, 4.0870000E-02,
      3.1830000E-02, 2.8090000E-02, 2.6060000E-02, 2.4800000E-02, 2.1880000E-02,
      1.9310000E-02, 1.7040000E-02, 1.5030000E-02, 1.3270000E-02, 1.1710000E-02,
      1.0330000E-02, 9.1200000E-03, 8.0470000E-03, 7.1020000E-03, 6.2670000E-03,
      5.5310000E-03, 4.8810000E-03, 4.3070000E-03, 3.8010000E-03, 3.3500000E-03,
      2.9600000E-03, 2.6130000E-03, 2.3060000E-03, 2.0350000E-03, 1.7960000E-03,
      1.5850000E-03, 1.3980000E-03, 1.2350000E-03, 1.0890000E-03, 9.6110000E-04,
      7.4850000E-04, 5.8300000E-04, 4.5400000E-04, 3.5360000E-04, 2.7540000E-04,
      1.6700000E-04, 1.0130000E-04, 6.1400000E-05, 3.7270000E-05, 2.2600000E-05,
      1.3710000E-05, 8.3200000E-06, 5.0440000E-06, 3.0600000E-06, 2.7000000E-06,
      2.3820000E-06, 2.1020000E-06, 1.8550000E-06, 1.7260000E-06, 1.5950000E-06,
      1.4570000E-06, 1.3080000E-06, 1.1660000E-06, 1.1300000E-06, 1.0990000E-06,
      1.0720000E-06, 1.0620000E-06, 1.0530000E-06, 1.0430000E-06, 1.0140000E-06,
      9.9200000E-07, 9.7100000E-07, 9.5070000E-07, 9.1000000E-07, 8.7640000E-07,
      8.3370000E-07, 7.8210000E-07, 6.2510000E-07, 5.0320000E-07, 4.1400000E-07,
      3.5770000E-07, 3.2060000E-07, 3.0110000E-07, 2.9070000E-07, 2.7050000E-07,
      2.5100000E-07, 2.2770000E-07, 1.8440000E-07, 1.5200000E-07, 1.4570000E-07,
      1.1160000E-07, 8.1970000E-08, 6.7000000E-08, 5.6920000E-08, 5.0000000E-08,
      4.2760000E-08, 3.5500000E-08, 3.0610000E-08, 2.5500000E-08, 2.0490000E-08,
      1.2400000E-08, 6.3250000E-09, 2.2770000E-09, 7.6020000E-10, 1.3900000E-10,
      1.0000000E-11 };

  ndi_float64 _120_lanl[] =
    { 5.0000000E+01, 3.0000000E+01, 2.0000000E+01, 1.7000000E+01, 1.4000000E+01,
      1.2000000E+01, 1.0829105E+01, 1.0829095E+01, 1.0000000E+01, 9.0000000E+00,
      8.9984050E+00, 8.9983950E+00, 8.8884050E+00, 8.8883950E+00, 8.0000000E+00,
      7.9163050E+00, 7.9162950E+00, 7.7240050E+00, 7.7239950E+00, 7.6456050E+00,
      7.6455950E+00, 7.6380050E+00, 7.6379950E+00, 7.6312050E+00, 7.6311950E+00,
      7.5000000E+00, 7.0000000E+00, 6.8099050E+00, 6.8098950E+00, 6.5000000E+00,
      6.1907050E+00, 6.1906950E+00, 6.0000000E+00, 5.5000000E+00, 5.3213050E+00,
      5.3212950E+00, 5.2692050E+00, 5.2691950E+00, 5.2623050E+00, 5.2622950E+00,
      5.0000000E+00, 4.9465050E+00, 4.9464950E+00, 4.5000000E+00, 4.4390050E+00,
      4.4389950E+00, 4.0000000E+00, 3.6840050E+00, 3.6839950E+00, 3.5000000E+00,
      3.0000000E+00, 2.6660000E+00, 2.5000000E+00, 2.3330000E+00, 2.2246050E+00,
      2.2245950E+00, 2.0000000E+00, 1.8848050E+00, 1.8847950E+00, 1.8750000E+00,
      1.7791050E+00, 1.7790950E+00, 1.6600000E+00, 1.5000000E+00, 1.4540050E+00,
      1.4539950E+00, 1.4340050E+00, 1.4339950E+00, 1.4080050E+00, 1.4079950E+00,
      1.3300000E+00, 1.2380050E+00, 1.2379950E+00, 1.2000000E+00, 1.1250000E+00,
      1.0144050E+00, 1.0143950E+00, 1.0136050E+00, 1.0135950E+00, 1.0050050E+00,
      1.0049950E+00, 1.0000000E+00, 9.6200500E-01, 9.6199500E-01, 9.3600500E-01,
      9.3599500E-01, 9.0000000E-01, 8.5360500E-01, 8.5359500E-01, 8.4700500E-01,
      8.4699500E-01, 8.4380500E-01, 8.4379500E-01, 8.3487500E-01, 8.3486500E-01,
      8.0000000E-01, 7.0000000E-01, 6.6900500E-01, 6.6899500E-01, 6.0000000E-01,
      5.2500000E-01, 5.0000000E-01, 4.7760500E-01, 4.7759500E-01, 4.6500500E-01,
      4.6499500E-01, 4.5000000E-01, 4.0000000E-01, 3.5236500E-01, 3.5235500E-01,
      3.0000000E-01, 2.0000000E-01, 1.5000000E-01, 1.0000000E-01, 8.0000000E-02,
      6.0000000E-02, 4.5000000E-02, 3.0000000E-02, 2.0000000E-02, 1.0000000E-02,
      1.0000000E-03 };

  ndi_float64 _105_awe[] = 
  { 2.000E+01,1.950E+01,1.900E+01,1.850E+01,1.800E+01,1.750E+01,
    1.700E+01,1.650E+01,1.600E+01,1.575E+01,1.550E+01,1.525E+01,
    1.500E+01,1.480E+01,1.460E+01,1.440E+01,1.420E+01,1.405E+01,
    1.390E+01,1.370E+01,1.350E+01,1.325E+01,1.300E+01,1.275E+01,
    1.250E+01,1.200E+01,1.150E+01,1.100E+01,1.050E+01,1.000E+01,
    9.500E+00,9.000E+00,8.500E+00,8.000E+00,7.500E+00,7.000E+00,
    6.500E+00,6.000E+00,5.500E+00,5.000E+00,4.500E+00,4.000E+00,
    3.700E+00,3.400E+00,3.100E+00,2.860E+00,2.630E+00,2.400E+00,
    2.160E+00,1.960E+00,1.780E+00,1.600E+00,1.450E+00,1.330E+00,
    1.210E+00,1.100E+00,1.010E+00,9.300E-01,8.500E-01,7.800E-01,
    7.150E-01,6.550E-01,6.000E-01,5.500E-01,5.000E-01,4.500E-01,
    4.100E-01,3.700E-01,3.300E-01,2.900E-01,2.600E-01,2.200E-01,
    1.800E-01,1.530E-01,1.300E-01,1.020E-01,8.000E-02,5.900E-02,
    4.300E-02,3.076E-02,2.200E-02,1.483E-02,1.000E-02,6.320E-03,
    4.000E-03,2.530E-03,1.600E-03,1.020E-03,6.500E-04,4.110E-04,
    2.600E-04,1.690E-04,1.100E-04,6.800E-05,4.200E-05,2.500E-05,
    1.500E-05,9.100E-06,5.500E-06,3.300E-06,2.000E-06,1.170E-06,
    6.840E-07,4.000E-07,2.300E-07,1.000E-09 };

  ndi_float64 _32_awe[] = 
  { 1.460E+01,1.350E+01,1.250E+01,1.100E+01,9.500E+00,
    8.000E+00,6.500E+00,5.000E+00,3.400E+00,2.400E+00,
    1.600E+00,1.100E+00,7.800E-01,5.500E-01,3.700E-01,
    2.600E-01,1.800E-01,1.300E-01,8.000E-02,4.300E-02,
    2.200E-02,1.000E-02,4.000E-03,1.600E-03,6.500E-04,
    2.600E-04,1.100E-04,4.200E-05,1.500E-05,5.500E-06,
    2.000E-06,4.000E-07,1.000E-09 };

   ndi_float64 _30_group[] = { 17.0, 15.0, 13.5, 12.0, 10.0, 7.79, 6.07, 3.68,
			       2.865, 2.232, 1.738, 1.353, 0.823, 0.5, 0.303, 
			       0.184, 6.76e-2, 2.48e-2, 9.12e-3, 3.35e-3, 
			       1.235e-3, 4.54e-4, 1.67e-4, 6.14e-5, 2.26E-5, 
			       8.32E-6, 3.06E-6, 1.13E-6, 4.14E-7, 1.52E-7, 
			       1.39E-10 };

   ndi_float64 _21_group[] = { 17.0, 15.0, 13.5, 12.0, 10.0, 7.79, 6.07, 3.68,
			       2.865, 2.232, 1.738, 1.353, 0.823, 0.5, 0.303, 
			       0.184, 6.76e-2, 2.48e-2, 9.12e-3, 3.35e-3, 
			       1.235e-3, 1.67e-4 };

   ndi_float64 _16_group[] = { 17.0, 13.5, 12.0, 10.0, 7.79, 6.07, 3.68, 
			       2.865, 1.353, 0.5, 0.303, 0.184, 6.76e-2, 
			       2.48e-2, 9.12e-3, 3.35e-3, 1.67e-4 };

   ndi_float64 _16_LLNL[] = { 17.0, 13.5, 12.0, 10.0, 7.79, 6.07, 3.68,
			      2.865, 1.353, 0.5, 0.184, 6.76e-2, 2.48e-2,
			      9.12e-3, 3.35e-3, 4.54e-4, 1.67e-4 };

   ndi_float64 _16_awe[] = { 1.460E+01,1.350E+01,1.250E+01,1.150E+01,9.500E+00,
			      8.000E+00,6.500E+00,4.500E+00,2.400E+00,1.100E+00,
			      6.550E-01,2.900E-01,1.800E-01,1.020E-01,2.200E-02,
			      1.000E-02,1.000E-09 };
  
   ndi_float64 _14_group[] = { 17.0, 13.5, 10.0, 6.07, 2.865, 1.353, 0.5, 
			       0.184, 6.76e-2, 2.48e-2, 9.12e-3, 3.35e-3, 
			       4.54e-4, 1.67e-4, 1.39e-10 };

   ndi_float64 _13_group[] = { 17.0, 13.5, 10.0, 6.07, 2.865, 1.353, 0.5, 
			       0.184, 6.76e-2, 2.48e-2, 9.12e-3, 3.35e-3, 
			       4.54e-4, 1.67e-4 };

   ndi_float64 _12_group[] = { 17.0, 13.5, 10.0, 6.07, 2.865, 1.353, 0.5, 
			       0.184, 6.76e-2, 2.48e-2, 9.12e-3, 3.35e-3, 
			       4.54e-4 };

   ndi_float64 _10_group[] = { 17.0, 13.5, 10.0, 7.79, 3.68, 2.232, 0.5, 
			       0.184, 2.48e-2, 3.35e-3, 1.67e-4 };

   ndi_float64 _9_group[] = { 17.0, 13.5, 10.0, 3.68, 2.232, 0.5, 0.184, 
			      2.48e-2, 3.35e-3, 1.67e-4 };

   ndi_float64 _8_group[] = { 17.0, 13.5, 10.0, 2.232, 0.5, 0.184, 2.48e-2, 
			      3.35e-3, 1.67e-4 };

   ndi_float64 _7_group[] = { 17.0, 13.5, 10.0, 3.68, 2.232, 0.5, 2.48e-2, 
			      1.67e-4 };
   
   ndi_float64 _6_group[] = { 17.0, 10.0, 3.68, 2.232, 0.5, 2.48e-2, 1.67e-4 };
   
   ndi_float64 _5_group[] = { 17.0, 10.0, 3.68, 0.5, 2.48e-2, 1.67e-4 };
   
   ndi_float64 _4_group[] = { 17.0, 7.79, 2.232, 0.184, 1.67e-4 };
   
   ndi_float64 _3_group[] = { 17.0, 7.79, 0.184, 1.67e-4 };
   
   ndi_float64 _2_group[] = { 17.0, 2.232, 1.67e-4 }; 
/*    ndi_float64 _2_group[] = { 17.0, 3.68, 1.235e-3 }; */
   
   ndi_float64 _1_group[] = { 17.0, 1.67e-4 };
   
   int ierr = 0;
   int lib_count = 0;
   int i = 0;
   ndi_float64 *new_grp_ptr;
   int *ebounds;


 
   /*  Free previous energy boundary array */
   if (*new_ebound_p) ndi_free((void**)new_ebound_p);

   /* Set default value */
   *new_ebound_p = NULL;
   *c_flag = 0;
   
   /**********************/

   if ( ( strncmp( grp_struc, "30_group", sizeof(char) * 9 ) == 0) ||
	( strncmp( grp_struc, "30_lanl", sizeof(char) * 8 ) == 0) ) {
     new_grp_ptr = _30_group;
     *new_grp = 30;
      
   } else if ( ( strncmp( grp_struc, "130_lanl", sizeof(char) * 9 ) == 0) ||
	       ( strncmp( grp_struc, "130_group", sizeof(char) * 10 ) == 0 ) ) { 
     new_grp_ptr = _130_lanl;
     *new_grp = 130;
      
   } else if ( ( strncmp( grp_struc, "120_lanl", sizeof(char) * 9 ) == 0) ||
	       ( strncmp( grp_struc, "120_group", sizeof(char) * 10 ) == 0 ) ) {
     new_grp_ptr = _120_lanl;
     *new_grp = 120;
      
   } else if( strncmp( grp_struc, "105_awe", sizeof(char) * 8 ) == 0) {
     new_grp_ptr = _105_awe;
     *new_grp = 105;

   } else if( strncmp( grp_struc, "32_awe", sizeof(char) * 6 ) == 0) {
     new_grp_ptr = _32_awe;
     *new_grp = 32;

   } else if ( ( strncmp( grp_struc, "21_group", sizeof(char) * 9 ) == 0 ) ||
	       ( strncmp( grp_struc, "21_lanl", sizeof(char) * 8 ) == 0 ) ) {
     new_grp_ptr = _21_group;
     *new_grp = 21;
     
   } else if( ( strncmp( grp_struc, "16_group", sizeof(char) * 9 ) == 0) ||
	      ( strncmp( grp_struc, "16_lanl", sizeof(char) * 8 ) == 0) ) {
     new_grp_ptr = _16_group;
     *new_grp = 16;
      
   } else if( strncmp( grp_struc, "16_awe", sizeof(char) * 6 ) == 0) {
     new_grp_ptr = _16_awe;
     *new_grp = 16;
     
   } else if( strncmp( grp_struc, "16_llnl", sizeof(char) * 7 ) == 0) {
     new_grp_ptr = _16_LLNL;
     *new_grp = 16;
     
   } else if( ( strncmp( grp_struc, "14_group", sizeof(char) * 9 ) == 0 ) ||
	      ( strncmp( grp_struc, "14_lanl", sizeof(char) * 8 ) == 0 ) ){
     new_grp_ptr = _14_group;
     *new_grp = 14;
     
   } else if( ( strncmp( grp_struc, "13_group", sizeof(char) * 9 ) == 0 ) ||
	      ( strncmp( grp_struc, "13_lanl", sizeof(char) * 8 ) == 0 ) ) {
     new_grp_ptr = _13_group;
     *new_grp = 13;
     
   } else if( ( strncmp( grp_struc, "12_group", sizeof(char) * 9 ) == 0 ) ||
	      ( strncmp( grp_struc, "12_lanl", sizeof(char) * 8 ) == 0 ) ) {
     new_grp_ptr = _12_group;
     *new_grp = 12;
     
   } else if( ( strncmp( grp_struc, "10_group", sizeof(char) * 9 ) == 0) ||
	      ( strncmp( grp_struc, "10_lanl", sizeof(char) * 8 ) == 0) ) {
     new_grp_ptr = _10_group;
     *new_grp = 10;
      
   } else if( ( strncmp( grp_struc, "9_group", sizeof(char) * 8 ) == 0) ||
	      ( strncmp( grp_struc, "9_lanl", sizeof(char) * 7 ) == 0) ) {
     new_grp_ptr = _9_group;
     *new_grp = 9;
      
   } else if( ( strncmp( grp_struc, "8_group", sizeof(char) * 8 ) == 0) ||
	      ( strncmp( grp_struc, "8_lanl", sizeof(char) * 7 ) == 0) ) {
     new_grp_ptr = _8_group;
     *new_grp = 8;
      
   } else if( ( strncmp( grp_struc, "7_group", sizeof(char) * 8 ) == 0) ||
	      ( strncmp( grp_struc, "7_lanl", sizeof(char) * 7 ) == 0) ) {
     new_grp_ptr = _7_group;
     *new_grp = 7;
     
   } else if( ( strncmp( grp_struc, "6_group", sizeof(char) * 8 ) == 0) ||
	      ( strncmp( grp_struc, "6_lanl", sizeof(char) * 7 ) == 0) ) {
     new_grp_ptr = _6_group;
     *new_grp = 6;
     
   } else if( ( strncmp( grp_struc, "5_group", sizeof(char) * 8 ) == 0) ||
	      ( strncmp( grp_struc, "5_lanl", sizeof(char) * 7 ) == 0) ) {
     new_grp_ptr = _5_group;
     *new_grp = 5;
      
   } else if( ( strncmp( grp_struc, "4_group", sizeof(char) * 8 ) == 0) ||
	      ( strncmp( grp_struc, "4_lanl", sizeof(char) * 7 ) == 0) ) {
      new_grp_ptr = _4_group;
      *new_grp = 4;
      
   } else if( ( strncmp( grp_struc, "3_group", sizeof(char) * 8 ) == 0) || 
	      ( strncmp( grp_struc, "3_lanl", sizeof(char) * 7 ) == 0) ){
      new_grp_ptr = _3_group;
      *new_grp = 3;
      
   } else if( ( strncmp( grp_struc, "2_group", sizeof(char) * 8 ) == 0) ||
	      ( strncmp( grp_struc, "2_lanl", sizeof(char) * 7 ) == 0 ) ) {
     new_grp_ptr = _2_group;
     *new_grp = 2;
     
   } else if( ( strncmp( grp_struc, "1_group", sizeof(char) * 8 ) == 0) ||
	      ( strncmp( grp_struc, "1_lanl", sizeof(char) * 7 ) == 0 ) ){
     new_grp_ptr = _1_group;
     *new_grp = 1;
     
   } else if ( strncmp( grp_struc, "default", sizeof(char) * 8 ) == 0 ) {     
     /* Use library's group structure */
     *new_grp = 0;
     return (ierr);
     
   } else {
     /* Unknown group structure */
     *new_grp = 0;
     return(-5);
   }
   

   if ( ( library_type == 'm' ) ||
	( library_type == 'd' ) ) {
      /* Multigroup or dosimetry data*/

      *c_flag = -1;
      
      /* Test special cases for minimal processing */
      
      /* TN data or isotope not set for multigroup */
      if ( old_grp <= 0 ) {
	 /* Isotope not set, just return */
	 return(ierr);
      }
      
      /* old_grp must be at least as large as new_grp */
      if ( old_grp < *new_grp ) {
	 /* Incompatible group structure */
	 *new_grp = 0;
	 *c_flag = 0;
	 strcpy( n_group, "default" );
	 return(-23);
      } 
      
      /* Test if collapsed group structure is the
	 same as the library group structure      */
      else if ( old_grp == *new_grp ) {
	 for ( i = 0; i <= old_grp; i++ ) {
	    if ( lib_ebounds[i] == *new_grp_ptr ) {
	       new_grp_ptr++;
	    } else {
	       *new_grp = 0;
	       *c_flag = 0;
	       strcpy( n_group, "default" );
	       return(-23);
	    }
	 }
	 *c_flag = 0;
	 return(ierr);
      }
      
      ebounds = (int *)ndi_malloc( (*new_grp + 1) * sizeof(int) );
      
      for ( i = 0; i <= *new_grp; i++ ) {
	 while ( greater_than_epsilon(lib_ebounds[lib_count], *new_grp_ptr) ) {
	    lib_count++;
	    if ( lib_count > old_grp ) {
	       /* New boundaries extend farther than old (library) ones */
	       if (ebounds) ndi_free((void**)&ebounds);
	       *new_grp = 0;
	    }
	 }
	 if ( within_epsilon(lib_ebounds[lib_count], *new_grp_ptr ) != 0 ) {
	    ebounds[i] = lib_count;
	 } else {
	    /* Incompatible group structure */
	    if (ebounds) ndi_free((void**)&ebounds);
	    *new_grp = 0;
	    *c_flag = 0;
	    strcpy( n_group, "default" );
	    return(-23);
	 }
	 new_grp_ptr++;
      }
      
      *new_ebound_p = ebounds;
   } else if ( library_type == 's' ) {
      /* TN data */
      /* Collapse_flag set to '0' above and grp_num_new set above */
      double *curr_e_ptr;

      if (new_ebounds) ndi_free((void**)&new_ebounds);
      if (tn_ebounds) ndi_free( (void **)&(tn_ebounds));
      if ( *new_grp > 0 ) {
	 tn_num_grps = *new_grp;
	 tn_ebounds = (double *)ndi_malloc( ( *new_grp + 1 ) * sizeof(double));
	 curr_e_ptr = tn_ebounds;
	 for ( i = 0; i <= *new_grp; i++ ) {
	    *curr_e_ptr = *new_grp_ptr;
	    curr_e_ptr++;
	    new_grp_ptr++;
	 }
      } else {
	 /* No default group structure */
	*new_grp = 0;
	*c_flag = 0;
	strcpy( n_group, "default" );
	ierr = - 5;
      }
   } else {
      /* Incompatible library type */
     *new_grp = 0;
     *c_flag = 0;
     strcpy( n_group, "default" );
     ierr = -16;
   }

   return(ierr);

} 

/**********************************************************************/

static int within_epsilon(ndi_float64 a, ndi_float64 b) 
{
   if ( a == b ) 
      return(-1);
   else
      return(0);
    
}

/**********************************************************************/

static int greater_than_epsilon(ndi_float64 a, ndi_float64 b) 
{
   if ( a > b ) 
      return(-1);
   else
      return(0);
    
}

/**********************************************************************/
