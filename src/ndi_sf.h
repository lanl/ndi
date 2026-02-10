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

typedef struct spont_fiss_pnu {
    long za;
    double pnu[11];
} spont_fiss_pnu;

extern spont_fiss *ndi_get_sf_data(const char *zaid);
extern spont_fiss_pnu *ndi_get_sf_pnu_data(const char *zaid);

#endif
