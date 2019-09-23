/*
 * $Id$
 * Parser Level: Spontaneous Fission Data
 *
 */

/**********************************************************************/
   
#ifndef NDI_SF_H
#define NDI_SF_H

#include "ndi_internal.h"
#include "ndi_var_type.h"

typedef struct spont_fiss {
    long za;
    double lambda;
    double ratio;
    double nubar;
    double a;
    double b;
} spont_fiss;

extern spont_fiss *ndi_get_sf_data(const char *zaid);

/**********************************************************************/

char *sf_mult_id[NUM_SF_PNU_ISOTOPES];

ndi_float64 sf_mult_prob[NUM_SF_PNU_ISOTOPES][MAX_SF_PNU];

extern int GET_SF_PNU(state_struct *state, FLOAT_TYPE *return_val);

#endif
