#include <stdio.h>
#include <stdlib.h>

#include "ndi_internal.h"
#include "ndi.h"

#define MAX_HANDLES   15
#define STRING_LEN    100

int main ()
{
    int ierr;
    int i;
    int ill_gotten_handle;
    int gendir[MAX_HANDLES];
    int handle[MAX_HANDLES];
    char str_val[STRING_LEN];


    /* Get too many gendir handles */
    printf("*********** TEST 1 -- Too many gendir handles ***********\n\n");
    for (i = 0; i < MAX_HANDLES; i++) {
        ierr = NDI2_open_gendir(&gendir[i], "");
        if (ierr)
            printf("    **Error %d in NDI open gendir%d.\n", ierr, i );
        ierr = NDI2_set_option_gendir(gendir[i], NDI_LIB_TYPE_DEFAULT, 
                                     "multigroup_neutron");
        if (ierr)
            printf("    **Error %d in setting option for gendir%d.\n", ierr, i );
        ierr = NDI2_check_option_gendir(gendir[i], NDI_COLLAPSE_DEFAULT, str_val,
                                        STRING_LEN);
        if (ierr)
            printf("    **Error %d in checking option for gendir%d.\n", ierr, i);
        else
            printf("  gendir%d collapse = %s\n", i, str_val);
   }

    /* Pass valid handle ID that wasn't appropriately allocated */
    printf("\n\n*********** TEST 2 -- Ill-gotten, but valid handle ***********\n\n");
    ill_gotten_handle = (2 * MAX_HANDLE_NUM) + 1;
    ierr = NDI2_set_lib_type(ill_gotten_handle, "tn");
    if (ierr)
        printf("    **Error %d in setting library type for ill-gotten handle%d.\n", ierr,
               ill_gotten_handle);
    ierr = NDI2_check_option(ill_gotten_handle, NDI_COLLAPSE, str_val, STRING_LEN);
    if (ierr)
        printf("    **Error %d in checking option for ill-gotten handle%d.\n", ierr, 
               ill_gotten_handle);
    else
        printf("  handle%d collapse = %s\n", ill_gotten_handle, str_val);
    

    /* Get too many non-gendir handles */
    printf("\n\n*********** TEST 3 -- Too many non-gendir handles ***********\n\n");
    for (i = 0; i < MAX_HANDLES; i++) {
        ierr = NDI2_get_handle_gendir(gendir[0], &handle[i]);
        if (ierr)
            printf("    **Error %d in getting handle%d.\n", ierr, i );
        ierr = NDI2_set_option(handle[i], NDI_COLLAPSE, "21_lanl");
        if (ierr)
            printf("    **Error %d in setting option for handle%d.\n", ierr, i );
        ierr = NDI2_check_option(handle[i], NDI_COLLAPSE, str_val, STRING_LEN);
        if (ierr)
            printf("    **Error %d in checking option for handle%d.\n", ierr, i);
        else
            printf("  handle%d collapse = %s\n", i, str_val);

    }


    /* Get too many layers of handles */
    printf("\n\n*********** TEST 4 -- Too many layers of handles ***********\n\n");
    ierr = NDI2_set_lib_type(handle[0], "multigroup_multi");
    for (i = 1; i < MAX_HANDLES; i++) {
        ierr = NDI2_get_handle(handle[i-1], &handle[i]);
        if (ierr)
            printf("    **Error %d in getting handle%d.\n", ierr, i);
        ierr = NDI2_set_lib_type(handle[i], "multigroup_multi");
        if (ierr)
            printf("    **Error %d in setting library type for handle%d.\n", ierr, i );
        ierr = NDI2_check_option(handle[i], NDI_COLLAPSE, str_val, STRING_LEN);
        if (ierr)
            printf("    **Error %d in checking option for handle%d.\n", ierr, i);
        else
            printf("  handle%d collapse = %s\n", i, str_val);
        
    }

    /* Close all gendirs */
    printf("\n\n*********** TEST ? -- Close all gendirs ***********\n\n");
    for (i = 1; i < MAX_HANDLES; i++) {
        NDI2_close_gendir(gendir[i]);
        if (ierr)
            printf("    **Error %d closing gendir%d.\n", ierr, i);
        else
            printf("  Closed gendir%d.\n", i);
    }

    return 0;
}
