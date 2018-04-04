#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ndi.h"

void print_float64_vec( ndi_float64 *flt64_ptr, int num_elements );
void print_float32_vec( ndi_float32 *flt_ptr, int num_elements );
void print_int_vec( int *int_ptr, int num_elements );


int main (){
   int ierr;

   /* Inintialize gendir */
   int gendir_handle;
   ierr = NDI2_open_gendir(&gendir_handle, "");
   if (ierr)
     printf("  **Error %d in NDI open gendir.\n\n", ierr );

   /* Get mpmg handle */
   int data_handle;
   ierr = NDI2_make_handle(gendir_handle, "multigroup_neutron", &data_handle);
   if (ierr)
     printf("  **Error %d in NDI make_handle.\n\n", ierr );

   ierr = NDI2_set_option( data_handle, NDI_LIBRARY, "mendf5" );
   ierr = NDI2_set_isotope( data_handle, "U235" );

   printf("\nUser-specified Weights Extension Tests\n"
          "================================================================================\n\n"
          "Library: mendf5 \n"
          "Library Type: multigroup_neutron"
          "Isotope: U235 \n\n"
          "LibraryInformation\n"
          "================== \n\n" );

   int weight_array_size;
   weight_array_size = NDI2_get_size(data_handle, NDI_WGTS, &ierr);
   printf(" Weight array size: %d.\n\n", weight_array_size );

   ndi_float64 lib_e_bound[31];
   ierr = NDI2_get_float64_vec(data_handle, NDI_E_BOUNDS, lib_e_bound, weight_array_size + 1);
   const int t = 4 * ( weight_array_size / 4 );
   printf(" Library Energy Boundaries:\n" );
   for (int i = 0; i < (weight_array_size + 1) / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %e  ",  lib_e_bound[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < (weight_array_size + 1) % 4; ++i){
       printf(" %e  ",  lib_e_bound[ t + i ] );
   }
   printf("\n\n");

   ndi_float64 lib_total_xs[30];
   ierr = NDI2_get_float64_vec(data_handle, NDI_SIG_TOT, lib_total_xs, weight_array_size);
   printf(" Library Total XS:\n" );
   for (int i = 0; i < weight_array_size / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %e  ", lib_total_xs[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < weight_array_size % 4; ++i){
       printf(" %e  ", lib_total_xs[ t + i ] );
   }
   printf("\n\n");

   ndi_float64 lib_weights[30];
   ierr = NDI2_get_float64_vec(data_handle, NDI_WGTS, lib_weights, weight_array_size);
   printf(" Library weights:\n" );
   for (int i = 0; i < weight_array_size / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %f  ",  lib_weights[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < weight_array_size % 4; ++i){
       printf(" %f  ",  lib_weights[ t + i ] );
   }
   printf("\n\n");

   printf("===================================================================== \n" 
          "Test 1: Argument weights must be greater than 0 \n"
          "===================================================================== \n\n" );

   printf(" Test 1.1: Zero weight \n"
          " ────────────────────────────────────────────────────────────────────\n\n" );

   ndi_float64 arg_weights[31] = {
     0.0,   1.073857,   1.019046,   1.014105, 
     1.023115,   1.043957,   1.223525,   1.234471,  
     1.342931,   1.448154,   1.540776,   2.175640,  
     2.030730,   1.734145,   1.580912,   2.156350,  
     2.157770,   2.154330,   2.156910,   2.152800,  
     2.154780,   2.153680,   2.154820,   2.153970,  
     2.154850,   2.155340,   2.148720,   2.158570,  
     2.149890,   13.707600,  99999999  
   };
   printf(" Argument weights:\n" );
   for (int i = 0; i < weight_array_size / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %f  ",  arg_weights[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < weight_array_size % 4; ++i){
       printf(" %f  ",  arg_weights[ t + i ] );
   }
   printf("\n\n");
   printf("Assigning (illegal) weights; Error expected\n");
   printf(" ierr = NDI2_set_float64_vec_option(data_handle, NDI_WGTS, arg_weights, 30);\n\n");
   ierr = NDI2_set_float64_vec_option(data_handle, NDI_WGTS, arg_weights, 30);
   if (ierr){
     printf("Weight assignment failed\n");
     printf("Error code: %i\n\n", ierr);
   } else{
     printf("Did not encounter expected error");
     return 1;
   }

   printf(" Test 1.2: Negative weight \n"
          " ────────────────────────────────────────────────────────────────────\n\n"  );

   arg_weights[0] = -1.0;

   printf(" Argument weights:\n" );
   for (int i = 0; i < weight_array_size / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %f  ",  arg_weights[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < weight_array_size % 4; ++i){
       printf(" %f  ",  arg_weights[ t + i ] );
   }
   printf("\n\n");
   printf("Assigning (illegal) weights; Error expected\n");
   printf(" ierr = NDI2_set_float64_vec_option(data_handle, NDI_WGTS, arg_weights, 30);\n\n");
   ierr = NDI2_set_float64_vec_option(data_handle, NDI_WGTS, arg_weights, 30);
   if (ierr){
     printf("Weight assignment failed\n");
     printf("Error code: %i\n\n", ierr);
   } else{
     printf("Did not encounter expected error");
     return 1;
   }

   printf("===================================================================== \n" 
          "Test 2: User-weight array must be same length as library-weight array \n"
          "===================================================================== \n\n" 
);
   printf(" Test 2.1: Too few elements \n"
          " ────────────────────────────────────────────────────────────────────\n\n" );
   
   arg_weights[0] = 1.003722;

   printf(" Argument weights:\n" );
   for (int i = 0; i < (weight_array_size - 1) / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %f  ",  arg_weights[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < (weight_array_size - 1) % 4; ++i){
       printf(" %f  ",  arg_weights[ t + i ] );
   }

   printf("\n\n");
   printf("Assigning (illegal) weights; Error expected\n");
   printf(" ierr = NDI2_set_float64_vec_option(data_handle, NDI_WGTS, arg_weights, 29);\n\n");
   ierr = NDI2_set_float64_vec_option(data_handle, NDI_WGTS, arg_weights, 29);
   if (ierr){
     printf("Weight assignment failed\n");
     printf("Error code: %i\n\n", ierr);
   } else{
     printf("Did not encounter expected error");
     return 1;
   }

   printf(" Test 2.2: Too many elements \n"
          " ────────────────────────────────────────────────────────────────────\n\n" );
   
   printf(" Argument weights:\n" );
   for (int i = 0; i < (weight_array_size + 1) / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %f  ",  arg_weights[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < (weight_array_size + 1) % 4; ++i){
       printf(" %f  ",  arg_weights[ t + i ] );
   }

   printf("\n\n");
   printf("Assigning (illegal) weights; Error expected\n");
   printf(" ierr = NDI2_set_float64_vec_option(data_handle, NDI_WGTS, arg_weights, 31);\n\n");
   ierr = NDI2_set_float64_vec_option(data_handle, NDI_WGTS, arg_weights, 29);
   if (ierr){
     printf("Weight assignment failed\n");
     printf("Error code: %i\n\n", ierr);
   } else{
     printf("Did not encounter expected error");
     return 1;
   }

   printf("Test 3: Correct Behavior \n"
          "===================================================================== \n\n" );
   printf(" Test 3.1: Retrieved values \n"
          " ────────────────────────────────────────────────────────────────────\n\n" );

   printf(" Argument weights:\n" );
   for (int i = 0; i < (weight_array_size) / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %f  ",  arg_weights[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < (weight_array_size) % 4; ++i){
       printf(" %f  ",  arg_weights[ t + i ] );
   }
   printf("\n\n");

   printf(
   "assigning and retrieving user specified weights, i.e.\n"
   "  ierr = NDI2_set_float64_vec_option(data_handle, NDI_WGTS, arg_weights, 30);\n" 
   "  ierr = NDI2_get_float64_vec(data_handle, NDI_WGTS, return_weights, 30);\n\n"
     );

   ierr = NDI2_set_float64_vec_option(data_handle, NDI_WGTS, arg_weights, 30);
   ndi_float64 return_weights[30];
   ierr = NDI2_get_float64_vec(data_handle, NDI_WGTS, return_weights, 30);
   if (ierr){
     printf("Weight retrieval failed");
     return 1;
   }
   printf(" returned weights:\n" );
   for (int i = 0; i < weight_array_size / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %f  ", return_weights[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < weight_array_size % 4; ++i){
       printf(" %f  ",  return_weights[ t + i ] );
   }
   printf("\n\n");

   printf(" Test 3.1: Same-to-Same collapse does not modify fields \n"
          " ────────────────────────────────────────────────────────────────────\n\n" );
   printf( "Collapsing to 30_lanl group structure, i.e.\n"
           "   ierr = NDI2_set_option(data_handle, NDI_COLLAPSE, \"30_lanl\");\n\n" );
   
   ierr = NDI2_set_option(data_handle, NDI_COLLAPSE, "30_lanl");

   ndi_float64 trial_e_bound[31];
   ierr = NDI2_get_float64_vec(data_handle, NDI_E_BOUNDS, trial_e_bound, weight_array_size + 1);
   printf("Energy Boundary Differences:\n" );
   for (int i = 0; i < (weight_array_size + 1) / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %e  ",  trial_e_bound[4*i + j] - lib_e_bound[ 4*i + j ] );
     }
     printf("\n");
   }
   for (int i = 0; i < (weight_array_size + 1) % 4; ++i){
       printf(" %e  ",  trial_e_bound[t + i ] - lib_e_bound[ t + i ] );
   }
   printf("\n\n");

   ndi_float64 trial_total_xs[30];
   ierr = NDI2_get_float64_vec(data_handle, NDI_SIG_TOT, trial_total_xs, weight_array_size);
   printf(" Total XS Differences:\n" );
   for (int i = 0; i < weight_array_size / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %f  ", 
              trial_total_xs[4*i + j] - lib_total_xs[4*i + j] > ( 1E-15 * lib_total_xs[4*i + j] ) ? 
              trial_total_xs[4*i + j] - lib_total_xs[4*i + j] : 0.0 );
     }
     printf("\n");
   }
   for (int i = 0; i < weight_array_size % 4; ++i){
       printf(" %f  ", 
              trial_total_xs[t + i] - lib_total_xs[t + i] > ( 1E-15 * lib_total_xs[t + i] ) ? 
              trial_total_xs[t + i] - lib_total_xs[t + i] : 0.0 );
   }
   printf("\n\n");

   ndi_float64 trial_weights[30];
   ierr = NDI2_get_float64_vec(data_handle, NDI_WGTS, trial_weights, weight_array_size);
   printf(" Weight Differences:\n" );
   for (int i = 0; i < weight_array_size / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %f  ", 
              trial_weights[4*i + j] - arg_weights[4*i + j] > ( 1E-15 * arg_weights[4*i + j] ) ? 
              trial_weights[4*i + j] - arg_weights[4*i + j] : 0.0 );
     }
     printf("\n");
   }
   for (int i = 0; i < weight_array_size % 4; ++i){
       printf(" %f  ", 
              trial_weights[t + i] - arg_weights[t + i] > ( 1E-15 * arg_weights[t + i] ) ? 
              trial_weights[t + i] - arg_weights[t + i] : 0.0 );
   }
   printf("\n\n");

   printf(" Test 3.2: Collapse of fields is accurate \n"
          " ────────────────────────────────────────────────────────────────────\n\n" );
   printf(
     "Collapsing to 4_lanl group structure, i.e.\n"
     "   ierr = NDI2_set_option(data_handle, NDI_COLLAPSE, \"4_lanl\");\n\n" );

   ierr = NDI2_set_option(data_handle, NDI_COLLAPSE, "4_lanl");

   ierr = NDI2_get_float64_vec(data_handle, NDI_E_BOUNDS, lib_e_bound, 5);
   printf(" collapsed e bounds:\n" );
   for (int i = 0; i < 5; ++i){
       printf(" %f  ",  lib_e_bound[ i ] );
   }
   printf("\n\n");

   printf(
     "Evaluating weights and total xs for handle with user-specified weights\n\n" );

   ierr = NDI2_get_float64_vec(data_handle, NDI_WGTS, return_weights, 4);
   printf(" collapsed user specified weights:\n" );
   for (int i = 0; i <  4; ++i){
       printf(" %f  ",  return_weights[ i ] );
   }
   printf("\n\n");

   ierr = NDI2_get_float64_vec(data_handle, NDI_SIG_TOT, lib_total_xs, 4);
   printf(" collapsed total XS from user specified weights:\n" );
   for (int i = 0; i <  4; ++i){
       printf(" %f  ",  lib_total_xs[ i ] );
   }
   printf("\n\n");

   printf(" reference solution for collapsed total XS from user specified weights \n"
          " (see collapse_check.py):\n" );
   ndi_float64 reference[4] = {  5.88145925,   7.50612957,   7.80092745,  20.5393206 };
   for (int i = 0; i <  4; ++i){
       printf(" %f  ",  reference[ i ] );
   }
   printf("\n\n");

   printf(
     "Evaluating weights and total xs for handle with library weights\n\n" );

   int data_handle_clone;
   ierr = NDI2_make_handle(gendir_handle, "multigroup_neutron", &data_handle_clone);
   if (ierr)
     printf("  **Error %d in NDI make_handle.\n\n", ierr );

   ierr = NDI2_set_option( data_handle_clone, NDI_LIBRARY, "mendf5" );
   ierr = NDI2_set_isotope( data_handle_clone, "U235" );
   ierr = NDI2_set_option(data_handle_clone, NDI_COLLAPSE, "4_lanl");
   ierr = NDI2_get_float64_vec(data_handle_clone, NDI_WGTS, return_weights, 4);
   printf(" collapsed library weights:\n" );
   for (int i = 0; i < 4; ++i){
       printf(" %f  ",  return_weights[ i ] );
   }
   printf("\n\n");

   ierr = NDI2_get_float64_vec(data_handle_clone, NDI_SIG_TOT, lib_total_xs, 4);
   printf(" collapsed total XS from library weights:\n" );
   for (int i = 0; i <  4; ++i){
       printf(" %f  ",  lib_total_xs[ i ] );
   }
   printf("\n\n");
   printf(" Test 3.3: Behavior mimics set_float64_vec_option for NDI_COLLAPSE \n"
          " ────────────────────────────────────────────────────────────────────\n\n" );
   
   printf(" ────────────────────────────────────────────────────────────────────\n"
          " Collapse Energy Bounds Examination\n"
          " ────────────────────────────────────────────────────────────────────\n\n");

   int reference_handle;
   ierr = NDI2_make_handle(gendir_handle, "multigroup_neutron", &reference_handle);
   ierr = NDI2_set_option( reference_handle, NDI_LIBRARY, "mendf5" );
   ierr = NDI2_set_isotope( reference_handle, "U235" );

   printf(" Examining energy bounds behavior... \n"
          "   ierr = NDI2_make_handle(gendir_handle, \"multigroup_neutron\", &reference_handle);\n"
          "   ierr = NDI2_set_option( reference_handle, NDI_LIBRARY, \"mendf5\" );\n"
          "   ierr = NDI2_set_isotope( reference_handle, \"U235\" );\n"
          "   ierr = NDI2_get_float64_vec(reference_handle, NDI_E_BOUNDS, lib_e_bound, 31);\n\n");

   ierr = NDI2_get_float64_vec(reference_handle, NDI_E_BOUNDS, lib_e_bound, weight_array_size + 1);
   printf(" Library Energy Boundaries:\n" );
   for (int i = 0; i < (weight_array_size + 1) / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %e  ",  lib_e_bound[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < (weight_array_size + 1) % 4; ++i){
       printf(" %e  ",  lib_e_bound[ t + i ] );
   }
   printf("\n\n");
   
   int size = 0;
   ndi_float64 return_buffer[1000];
   ndi_float64 custom_e_bound[5] = 
     { 1.700000e+01, 5.000000e-01, 2.480000e-02, 4.540000e-04, 1.520000e-07 };

   printf(" Setting custom energy bounds... \n"
          "   ierr = NDI2_set_float64_vec_option(reference_handle, NDI_COLLAPSE, custom_e_bound, 5);\n");

   ierr = NDI2_set_float64_vec_option(reference_handle, NDI_COLLAPSE, custom_e_bound, 5);
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   size = NDI2_get_size(reference_handle, NDI_E_BOUNDS, &ierr);
   printf("   size = NDI2_get_size(reference_handle, NDI_E_BOUNDS, &ierr);\n");
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   ierr = NDI2_get_float64_vec(reference_handle, NDI_E_BOUNDS, return_buffer, size);
   printf("   ierr = NDI2_get_float64_vec(reference_handle, NDI_E_BOUNDS, return_buffer, size);\n\n" );
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   printf(" Size: %i \n\n", size);

   printf(" Returned energy bounds:\n");
   for (int i = 0; i < 5; ++i){
       printf(" %e  ",  return_buffer[ i ] );
   }
   printf("\n\n");
   
   printf("Energy Bounds remain unchanged as you change isotopes...\n"
          "   ierr = NDI2_set_isotope( reference_handle, \"U238\" );\n");
   ierr = NDI2_set_isotope( reference_handle, "U238" );
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   printf("   ierr = NDI2_get_float64_vec(reference_handle, NDI_E_BOUNDS, return_buffer, size);\n\n" );
   ierr = NDI2_get_float64_vec(reference_handle, NDI_E_BOUNDS, return_buffer, size);
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   printf(" Returned energy bounds:\n");
   for (int i = 0; i < 5; ++i){
       printf(" %e  ",  return_buffer[ i ] );
   }
   printf("\n\n");

   printf("Energy Bounds ARE CHANGED as you change particle/library type...\n"
          "   ierr = NDI2_set_option( reference_handle, NDI_LIBRARY, \"multigroup_photon\" );\n");

   ierr = NDI2_set_option( reference_handle, NDI_LIB_TYPE, "multigroup_photon" );
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }

   ierr = NDI2_set_isotope( reference_handle, "U238" );
   printf("   (isotope information is not retained after altering NDI_LIB_TYPE in C-interface)\n"
          "   ierr = NDI2_set_isotope( reference_handle, \"U238\" );\n");
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   size = NDI2_get_size(reference_handle, NDI_E_BOUNDS, &ierr);
   printf("   ierr = NDI2_get_size(reference_handle, NDI_E_BOUNDS, &size);\n");
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   ierr = NDI2_get_float64_vec(reference_handle, NDI_E_BOUNDS, return_buffer, size);
   printf("   ierr = NDI2_get_float64_vec(reference_handle, NDI_E_BOUNDS, return_buffer, size);\n\n" );
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   printf(" Size: %i \n\n", size);

   printf(" Returned energy bounds:\n");
   for (int i = 0; i < (size / 4); ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %e  ",  return_buffer[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < (size % 4); ++i){
       printf(" %e  ",  return_buffer[ t + i ] );
   }
   printf("\n\n");

   printf(" ────────────────────────────────────────────────────────────────────\n"
          " User-Specified Weights Examination\n"
          " ────────────────────────────────────────────────────────────────────\n\n");

   int user_weight_handle;
   ierr = NDI2_make_handle(gendir_handle, "multigroup_neutron", &user_weight_handle);
   ierr = NDI2_set_option( user_weight_handle, NDI_LIBRARY, "mendf5" );
   ierr = NDI2_set_isotope( user_weight_handle, "U235" );

   printf(" Examining user-specified weight behavior... \n"
          "   ierr = NDI2_make_handle(gendir_handle, \"multigroup_neutron\", &user_weight_handle);\n"
          "   ierr = NDI2_set_option( user_weight_handle, NDI_LIBRARY, \"mendf5\" );\n"
          "   ierr = NDI2_set_isotope( user_weight_handle, \"U235\" );\n"
          "   ierr = NDI2_get_float64_vec(user_weight_handle, NDI_WGTS, lib_weights, 30);\n\n");

   ierr = NDI2_get_float64_vec(user_weight_handle, NDI_WGTS, lib_weights, weight_array_size );
   printf(" Library Weights:\n" );
   for (int i = 0; i < (weight_array_size) / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %e  ",  lib_weights[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < (weight_array_size) % 4; ++i){
       printf(" %e  ",  lib_weights[ t + i ] );
   }
   printf("\n\n");
   
   size = 0;
   
   printf(" Setting custom group weights... \n"
          "   ierr = NDI2_set_float64_vec_option(user_weight_handle, NDI_WGTS, arg_weights, 30);\n");

   ierr = NDI2_set_float64_vec_option(user_weight_handle, NDI_WGTS, arg_weights, 30);
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   size = NDI2_get_size(user_weight_handle, NDI_WGTS, &ierr);
   printf("   size = NDI2_get_size(user_weight_handle, NDI_WGTS, &ierr);\n");
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   ierr = NDI2_get_float64_vec(user_weight_handle, NDI_WGTS, return_buffer, size);
   printf("   ierr = NDI2_get_float64_vec(user_weight_handle, NDI_WGTS, return_buffer, size);\n\n" );
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   printf(" Size: %i \n\n", size);

   printf(" Returned group weights:\n");
   for (int i = 0; i < (size) / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %e  ",  return_buffer[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < (size) % 4; ++i){
       printf(" %e  ",  return_buffer[ t + i ] );
   }
   printf("\n\n");
   
   printf("Group weights remain unchanged as you change isotopes...\n"
          "   ierr = NDI2_set_isotope( user_weight_handle, \"U238\" );\n");
   ierr = NDI2_set_isotope( user_weight_handle, "U238" );
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   printf("   ierr = NDI2_get_float64_vec(user_weight_handle, NDI_WGTS, return_buffer, size);\n\n" );
   ierr = NDI2_get_float64_vec(user_weight_handle, NDI_WGTS, return_buffer, size);
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   printf(" Returned weights:\n");
   for (int i = 0; i < (size) / 4; ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %e  ",  return_buffer[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < (size) % 4; ++i){
       printf(" %e  ",  return_buffer[ t + i ] );
   }
   printf("\n\n");

   printf("Weights ARE CHANGED as you change particle/library type...\n"
          "   ierr = NDI2_set_option( user_weight_handle, NDI_LIBRARY, \"multigroup_photon\" );\n");

   ierr = NDI2_set_option( user_weight_handle, NDI_LIB_TYPE, "multigroup_photon" );
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }

   ierr = NDI2_set_isotope( user_weight_handle, "U238" );
   printf("   (isotope information is not retained after altering NDI_LIB_TYPE in C-interface)\n"
          "   ierr = NDI2_set_isotope( user_weight_handle, \"U238\" );\n");
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   size = NDI2_get_size(user_weight_handle, NDI_WGTS, &ierr);
   printf("   ierr = NDI2_get_size(user_weight_handle, NDI_WGTS, &size);\n");
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   ierr = NDI2_get_float64_vec(user_weight_handle, NDI_WGTS, return_buffer, size);
   printf("   ierr = NDI2_get_float64_vec(user_weight_handle, NDI_WGTS, return_buffer, size);\n\n" );
   if (ierr) {
     printf("Error: %i  ", ierr );
     return 1;
   }
   printf(" Size: %i \n\n", size);

   printf(" Returned weights:\n");
   for (int i = 0; i < (size / 4); ++i){
     for (int j = 0; j < 4; ++j){
       printf(" %e  ",  return_buffer[4*i + j] );
     }
     printf("\n");
   }
   for (int i = 0; i < (size % 4); ++i){
       printf(" %e  ",  return_buffer[ t + i ] );
   }
   printf("\n\n");
}

