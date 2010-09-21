#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ndi.h"

/* Macro to print error or warning with string */
#define HANDLE_ERR(i, s) if(i) printf("  **Error %d in NDI2_%s\n\n", i, s)

/* Formulas from
   D. G. Madland, "Total prompt energy release in the neutron-induced
   fission of U235, U238, and Pu239",
   Nuc Phys A 722 (2006) 113-117
*/

ndi_float64 q_35(ndi_float64 e)
{
    return 180.57 + 0.1121*e - e;
}

ndi_float64 q_39(ndi_float64 e)
{
    return 188.42 + 0.0027*e - 0.0017*e*e - e;
}

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 6 ) == 0) )
	 printf("\n");
      printf("%13.6g   ", flt64_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/

int main()
{

    int ierr;
    int i;
    int num_grps;
    int gh = -1;
    int dh = -1;
    char *zaids[] = {"92235", "94239"};

    int j;
    typedef ndi_float64 (*P2F)(ndi_float64);
    P2F Q[] = {&q_35, &q_39};
 
    ndi_float64 *e = NULL;
    ndi_float64 *s = NULL;
    ndi_float64 *qsf = NULL;
    ndi_float64 *kd = NULL;
    ndi_float64 *kc = NULL;


    printf("  NDI TEST SUITE -- testqsf \n");

   printf("  ************************* TEST 1 ************************* \n" );
   printf("                       Q_SIG_F_PR TEST\n " );
   printf(" (NDI_Q_SIG_F_PR - (<E_d> - <E_n>))/NDI_Q_SIG_F_PR * 100%%.\n " );
   printf(" ********************************************************** \n\n" );

    ierr = NDI2_open_gendir(&gh, "");
    HANDLE_ERR(ierr, "open_gendir");
    ierr = NDI2_make_handle(gh, "multigroup_neutron", &dh);
    HANDLE_ERR(ierr, "make_handle");
    ierr = NDI2_set_option(dh, NDI_E_AVG, "from_td");
    HANDLE_ERR(ierr, "set_option");
    for (i = 0; i < 2; i++) {
        ierr = NDI2_set_zaid(dh, zaids[i]);
        HANDLE_ERR(ierr, "set_zaid");
        num_grps = NDI2_get_size(dh, NDI_E_AVG, &ierr);
        HANDLE_ERR(ierr, "get_size");
        e = (ndi_float64 *)malloc(num_grps*sizeof(ndi_float64));
        ierr = NDI2_get_float64_vec(dh, NDI_E_AVG, e, num_grps);
        HANDLE_ERR(ierr, "get_float64_vec");
        s = (ndi_float64 *)malloc(num_grps*sizeof(ndi_float64));
        ierr = NDI2_get_float64_vec_n(dh, NDI_SIG_REAC, 18, s, num_grps);
        HANDLE_ERR(ierr, "get_float64_vec");
        qsf = (ndi_float64 *)malloc(num_grps*sizeof(ndi_float64));
        ierr = NDI2_get_float64_vec(dh, NDI_Q_SIG_F_PR, qsf, num_grps);
        HANDLE_ERR(ierr, "get_float64_vec");

        /* Reqt 72 */
     printf("***********************ISOTOPE=%s***********************\n\n",
            zaids[i]);
        for(j = 0; j < num_grps; j++)
            qsf[j] = (qsf[j] - s[j] * (*Q[i])(e[j]))/qsf[j]*100.0;
        print_float64_vec(qsf, num_grps);
        printf(" ********************************************************** \n\n" );
        free((void *)qsf);
        free((void *)s);
        free((void *)e);
    }

   printf("  ************************* TEST 2 ************************* \n" );
   printf("                          KERMA CALC\n " );
   printf("                   (Data - Calc)/Calc * 100%%.\n " );
   printf(" ********************************************************** \n\n" );


    ierr = NDI2_set_option(dh, NDI_E_AVG, "from_td");
    HANDLE_ERR(ierr, "set_option NDI_E_AVG");
    ierr = NDI2_set_option(dh, NDI_Q_SIG_F_PR, "use_bad_calc");
    HANDLE_ERR(ierr, "set_option NDI_Q_SIG_F_PR");
    for (i = 0; i < 2; i++) {
        ierr = NDI2_set_zaid(dh, zaids[i]);
        HANDLE_ERR(ierr, "set_zaid");
        num_grps = NDI2_get_size(dh, NDI_KERMA, &ierr);
        HANDLE_ERR(ierr, "get_size");
        ierr = NDI2_set_option(dh, NDI_KERMA, "use_data_calc");
        HANDLE_ERR(ierr, "set_option NDI_KERMA");
        kd = (ndi_float64 *)malloc(num_grps*sizeof(ndi_float64));
        ierr = NDI2_get_float64_vec(dh, NDI_KERMA, kd, num_grps);
        HANDLE_ERR(ierr, "get_float64_vec");
        kc = (ndi_float64 *)malloc(num_grps*sizeof(ndi_float64));
        ierr = NDI2_set_option(dh, NDI_KERMA, "use_calc");
        HANDLE_ERR(ierr, "set_option NDI_KERMA");
        ierr = NDI2_get_float64_vec(dh, NDI_KERMA, kc, num_grps);
        HANDLE_ERR(ierr, "get_float64_vec");


        /* Reqt 72 */
     printf("***********************ISOTOPE=%s***********************\n\n",
            zaids[i]);
        for(j = 0; j < num_grps; j++)
            kc[j] = (kd[j] - kc[j])/kc[j]*100.0;
        print_float64_vec(kc, num_grps);
        printf(" ********************************************************** \n\n" );
        free((void *)kd);
        free((void *)kc);
    }
       
    ierr = NDI2_close_gendir(gh);
    HANDLE_ERR(ierr, "close_gendir");


    return ierr;
}
