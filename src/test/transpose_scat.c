#include <stdio.h>
#include <stdlib.h>

#include "ndi.h"


void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );
void check_err(const int ierr, const char *msg);


int main()
{
    int ierr;
    int gendir, gendir2;
    int single_part_handle, single_part_handle2;
    int mpmg_handle, mpmg_handle2;
    int vec_size;
    int neutron_h, photon_h;
    int pgrps = 2;
    ndi_float64 *scat_trans;
    ndi_float64 *scat_no_trans;
    ndi_float64 pbounds[] = { 20.0, 4.0, 0.1 };

    printf("**  TRANSPOSE SET IN DATA TABLE  **\n\n");
    ierr = NDI2_open_gendir(&gendir, "");
    check_err(ierr, "opening gendir"); 

    /* Single particle test */
    printf("\n**************** Single Particle Test ****************\n\n");
    ierr = NDI2_make_handle(gendir, "multigroup_neutron", &single_part_handle);
    check_err(ierr, "getting single_particle_handle");

    ierr = NDI2_set_option(single_part_handle, NDI_LIBRARY, "mendf6");
    check_err(ierr, "setting mendf6 library");
    ierr = NDI2_set_option(single_part_handle, NDI_COLLAPSE, "6_lanl");
    check_err(ierr, "setting group structure");
    ierr = NDI2_set_zaid(single_part_handle, "92235");
    check_err(ierr, "setting isotope 92235");

    vec_size = NDI2_get_size_n(single_part_handle, NDI_PN_FULL, 0, &ierr);
    check_err(ierr, "getting pn_full size");
    scat_no_trans = malloc(vec_size * sizeof(ndi_float64));
    scat_trans = malloc(vec_size * sizeof(ndi_float64));
    
    ierr = NDI2_get_float64_vec_n(single_part_handle, NDI_PN_FULL, 0, 
                                  scat_no_trans, vec_size);
    check_err(ierr, "getting pn full");
    printf("No transpose:\n");
    print_float64_vec(scat_no_trans, vec_size);

    ierr = NDI2_set_option(single_part_handle, NDI_MAT_ORDER, "transpose");
    check_err(ierr, "setting mat order");
    ierr = NDI2_get_float64_vec_n(single_part_handle, NDI_PN_FULL, 0, 
                                  scat_trans, vec_size);
    check_err(ierr, "getting transposed pn full");
    printf("\n\nTranspose:\n");
    print_float64_vec(scat_trans, vec_size);

    free(scat_no_trans);
    free(scat_trans);


    /* Multiparticle test */
    printf("\n\n\n**************** Multiparticle Test ****************\n\n");
    ierr = NDI2_make_handle(gendir, "multigroup_multi", &mpmg_handle);
    check_err(ierr, "getting mpmg handle");

    ierr = NDI2_set_option(mpmg_handle, NDI_LIBRARY, "ngk6");
    check_err(ierr, "setting ngk6 library");
    ierr = NDI2_set_zaid(mpmg_handle, "92235");
    check_err(ierr, "setting zaid 92235");
    ierr = NDI2_set_option(mpmg_handle, NDI_MAT_ORDER, "default");
    check_err(ierr, "setting mat order default");

    /* Set group structures */
    ierr = NDI2_get_int_val_n(mpmg_handle, NDI_PARTICLE_N, 0,
                              &neutron_h);
    check_err(ierr, "getting neutron handle");
    ierr = NDI2_get_int_val_n(mpmg_handle, NDI_PARTICLE_N, 1,
                              &photon_h);
    check_err(ierr, "getting photon handle");
    ierr = NDI2_set_option(neutron_h, NDI_COLLAPSE, "4_lanl");
    check_err(ierr, "setting neutron group structure");
    ierr = NDI2_set_float64_vec_option(photon_h, NDI_COLLAPSE,
                                       pbounds, pgrps + 1);
    check_err(ierr, "getting photon group structure");

    /* Get data */
    vec_size = NDI2_get_size_n(mpmg_handle, NDI_PN_FULL, 0, &ierr);
    scat_no_trans = malloc(vec_size * sizeof(ndi_float64));
    scat_trans = malloc(vec_size * sizeof(ndi_float64));
    
    ierr = NDI2_get_float64_vec_n(mpmg_handle, NDI_PN_FULL, 0, 
                                  scat_no_trans, vec_size);
    check_err(ierr, "getting pn full");
    printf("No transpose:\n");
    print_float64_vec(scat_no_trans, vec_size);

    ierr = NDI2_set_option(mpmg_handle, NDI_MAT_ORDER, "transpose");
    check_err(ierr, "setting mat order transpose");
    ierr = NDI2_get_float64_vec_n(mpmg_handle, NDI_PN_FULL, 0, 
                                  scat_trans, vec_size);
    check_err(ierr, "getting transposed pn full");
    printf("\n\nTranspose:\n");
    print_float64_vec(scat_trans, vec_size);

    free(scat_no_trans);
    free(scat_trans);
    
    ierr = NDI2_close_gendir(gendir);
    check_err(ierr, "closing gendir");


    /************  SET TRANSPOSE IN GENDIR  ***********/
    printf("\n**  TRANSPOSE SET IN GENDIR  **\n\n");
    ierr = NDI2_open_gendir(&gendir2, "");
    check_err(ierr, "opening gendir2");

    /* Single particle test */
    printf("\n**************** Single Particle Test ****************\n\n");
    ierr = NDI2_make_handle(gendir2, "multigroup_neutron", 
			    &single_part_handle);
    check_err(ierr, "getting single_particle_handle");

    ierr = NDI2_set_option(single_part_handle, NDI_LIBRARY, "mendf6");
    check_err(ierr, "setting mendf6 library");
    ierr = NDI2_set_option(single_part_handle, NDI_COLLAPSE, "6_lanl");
    check_err(ierr, "setting group structure");
    ierr = NDI2_set_zaid(single_part_handle, "92235");
    check_err(ierr, "setting isotope 92235");

    vec_size = NDI2_get_size_n(single_part_handle, NDI_PN_FULL, 0, &ierr);
    check_err(ierr, "getting pn_full size");
    scat_no_trans = malloc(vec_size * sizeof(ndi_float64));
    scat_trans = malloc(vec_size * sizeof(ndi_float64));
    
    ierr = NDI2_get_float64_vec_n(single_part_handle, NDI_PN_FULL, 0, 
                                  scat_no_trans, vec_size);
    check_err(ierr, "getting pn full");
    printf("No transpose:\n");
    print_float64_vec(scat_no_trans, vec_size);

    /* Get another data handle after setting transpose */
    ierr = NDI2_set_option_gendir(gendir2, NDI_MAT_ORDER_DEFAULT,
                                 "transpose");
    check_err(ierr, "setting default matrix order");
    ierr = NDI2_make_handle(gendir2, "multigroup_neutron", 
			    &single_part_handle2);
    check_err(ierr, "making single_particle_handle2");

    ierr = NDI2_set_option(single_part_handle2, NDI_LIBRARY, "mendf6");
    check_err(ierr, "setting mendf6 library");
    ierr = NDI2_set_option(single_part_handle2, NDI_COLLAPSE, "6_lanl");
    check_err(ierr, "setting group structure");
    ierr = NDI2_set_zaid(single_part_handle2, "92235");
    check_err(ierr, "setting isotope 92235");
    ierr = NDI2_get_float64_vec_n(single_part_handle2, NDI_PN_FULL, 0, 
                                  scat_trans, vec_size);
    check_err(ierr, "getting transposed pn full");
    printf("\n\nTranspose:\n");
    print_float64_vec(scat_trans, vec_size);

    free(scat_no_trans);
    free(scat_trans);


    /* Multiparticle test */
    printf("\n\n\n**************** Multiparticle Test ****************\n\n");
    ierr = NDI2_make_handle(gendir2, "multigroup_multi", &mpmg_handle);
    check_err(ierr, "getting mpmg handle");

    ierr = NDI2_set_option(mpmg_handle, NDI_LIBRARY, "ngk6");
    check_err(ierr, "setting ngk6 library");
    ierr = NDI2_set_zaid(mpmg_handle, "92235");
    check_err(ierr, "setting zaid 92235");
    ierr = NDI2_set_option(mpmg_handle, NDI_MAT_ORDER, "default");
    check_err(ierr, "setting mat order default");

    /* Set group structures */
    ierr = NDI2_get_int_val_n(mpmg_handle, NDI_PARTICLE_N, 0,
                              &neutron_h);
    check_err(ierr, "getting neutron handle");
    ierr = NDI2_get_int_val_n(mpmg_handle, NDI_PARTICLE_N, 1,
                              &photon_h);
    check_err(ierr, "getting photon handle");
    ierr = NDI2_set_option(neutron_h, NDI_COLLAPSE, "4_lanl");
    check_err(ierr, "setting neutron group structure");
    ierr = NDI2_set_float64_vec_option(photon_h, NDI_COLLAPSE,
                                       pbounds, pgrps + 1);
    check_err(ierr, "getting photon group structure");
    vec_size = NDI2_get_size_n(mpmg_handle, NDI_PN_FULL, 0, &ierr);
    scat_no_trans = malloc(vec_size * sizeof(ndi_float64));
    scat_trans = malloc(vec_size * sizeof(ndi_float64));
    

    /* Get data */
    ierr = NDI2_get_float64_vec_n(mpmg_handle, NDI_PN_FULL, 0, 
                                  scat_no_trans, vec_size);
    check_err(ierr, "getting pn full");
    printf("No transpose:\n");
    print_float64_vec(scat_no_trans, vec_size);

    /* Get another data handle after setting tranpose */
    ierr = NDI2_set_option_gendir(gendir2, NDI_MAT_ORDER_DEFAULT,
                                  "transpose");
    check_err(ierr, "setting default mat order transpose");
    ierr = NDI2_make_handle(gendir2, "multigroup_multi", &mpmg_handle2);
    check_err(ierr, "getting mpmg2 handle");    

    ierr = NDI2_set_option(mpmg_handle2, NDI_LIBRARY, "ngk6");
    check_err(ierr, "setting ngk6 library");
    ierr = NDI2_set_zaid(mpmg_handle2, "92235");
    check_err(ierr, "setting zaid 92235");


    /* Set group structures */
    ierr = NDI2_get_int_val_n(mpmg_handle2, NDI_PARTICLE_N, 0,
                              &neutron_h);
    check_err(ierr, "getting neutron handle");
    ierr = NDI2_get_int_val_n(mpmg_handle2, NDI_PARTICLE_N, 1,
                              &photon_h);
    check_err(ierr, "getting photon handle");
    ierr = NDI2_set_option(neutron_h, NDI_COLLAPSE, "4_lanl");
    check_err(ierr, "setting neutron group structure");
    ierr = NDI2_set_float64_vec_option(photon_h, NDI_COLLAPSE,
                                       pbounds, pgrps + 1);
    check_err(ierr, "getting photon group structure");
    vec_size = NDI2_get_size_n(mpmg_handle2, NDI_PN_FULL, 0, &ierr);
    scat_no_trans = malloc(vec_size * sizeof(ndi_float64));
    scat_trans = malloc(vec_size * sizeof(ndi_float64));
    

    /* Get data */
    ierr = NDI2_get_float64_vec_n(mpmg_handle2, NDI_PN_FULL, 0, 
                                  scat_trans, vec_size);
    check_err(ierr, "getting transposed pn full");
    printf("\n\nTranspose:\n");
    print_float64_vec(scat_trans, vec_size);

    free(scat_no_trans);
    free(scat_trans);
    
    ierr = NDI2_close_gendir(gendir2);
    check_err(ierr, "closing gendir2");


    return(ierr);
}

/**********************************************************************/

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements ) {

   int n;

   for (n = 0; n < num_elements; n++) {
      if ( ( n != 0 ) && ( ( n % 6 ) == 0) )
	 printf("\n");
      printf("%13.6e   ", flt64_ptr[n]);
   }
   printf("\n");
}

/**********************************************************************/
void check_err(const int ierr, const char *msg) {

    if (ierr < 0) {
        printf("NDI ERROR %d: %s\n", ierr, msg);
    } else if (ierr > 0) {
        printf("NDI WARNING %d: %s\n", ierr, msg);
    }

    return;
}
