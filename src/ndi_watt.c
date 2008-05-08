/*
 * $Id$
 * Utility Level: Watt spectrum
 *
 */

#include <math.h>

#include "ndi_watt.h"

#define PI 3.14159265359

/* Normalized Watt energy spectrum */
double ndi_watt(double E, double a, double b)
{
    double c;
    double K;

    c = sqrt(a*b)/2.0;
    K = 1.0/(a * sqrt(PI) * c * exp(c*c));
    return K * exp(-E/a) * sinh(sqrt(b*E));
}

/* Integrated Watt energy spectrum */
double ndi_int_watt(double E, double a, double b)
{
    double c;
    double K;

    c = sqrt(a*b)/2.0;
    K = 1.0/(a * sqrt(PI) * c * exp(c*c));

    return K * sqrt(PI)*a*c/2.0 * exp(c*c) * 
        (erf(sqrt(E/a) - c) + erf(sqrt(E/a) + c)) - 
        K * a * exp(-E/a) * sinh(sqrt(b*E));
}

#ifdef TEST_NDI_WATT

#include <stdio.h>

int main(int argc, char *argv[])
{

    int i;
    double a = 0.7747;          /* U-235 parameters from Sources-4A */
    double b = 4.852;
    double E, P, C;

    for (i = 0; i < 50; i++) {
        E = i/5.0;
        P = ndi_watt(E, a, b);
        C = ndi_int_watt(E, a, b);
        printf("%f\t%f\t%f\n", E, P, C);
    }
    return 0;
}

#endif
