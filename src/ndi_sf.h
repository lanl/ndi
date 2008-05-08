/*
 * $Id$
 * Parser Level: Spontaneous Fission Data
 *
 */

/**********************************************************************/
   
#ifndef NDI_SF_H
#define NDI_SF_H

typedef struct spont_fiss {
    long za;
    double lambda;
    double ratio;
    double nubar;
    double a;
    double b;
} spont_fiss;

extern spont_fiss *ndi_get_sf_data(const char *zaid);

#endif
