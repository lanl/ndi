#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ndi.h"


int main (int argc, char *argv[], char *envp[]) {

    int ierr;
    int str_size;
    char *gendir_path;
    char *curr;
    int i = 0;

    curr = envp[0];
    while (curr) {
        printf("%s\n", curr);
        i++;
        curr = envp[i];
    }

    printf("\n\n\n************>%s\n\n\n", getenv("NDI_GENDIR_PATH"));

    ierr = ndi_init();
/*     printf("\nNDI_GENDIR_PATH is default value\n"); */
    str_size = ndi_get_size(NDI_GENDIR_PATH, &ierr);
    gendir_path = (char *)malloc(str_size * sizeof(char));
    ierr = ndi_get_string_val(NDI_GENDIR_PATH, gendir_path, str_size);
/*     printf("  Gendir = %s\n", gendir_path); */
    if (gendir_path) free(gendir_path);
    gendir_path = NULL;
    ierr = ndi_close();

    /* Set path to gendir.test2 */
/*     printf("\nNDI_GENDIR_PATH is set to gendir.test2\n"); */
    
    ierr = ndi_set_option(NDI_GENDIR_PATH, "../../src/test/lib/gendir.test2");
    if (ierr != 0) {
/*         printf("ERROR SETTING GENDIR PATH IS %d\n", ierr); */
    }

    printf("\n\n\n\n***START NEW *******\n\n\n\n");
    curr = envp[0];
    i = 0;
    while (curr) {
        printf("%s\n", curr);
        i++;
        curr = envp[i];
    }

    printf("\n\n\n************>%s\n\n\n", getenv("NDI_GENDIR_PATH"));
    ierr = ndi_init();
    str_size = ndi_get_size(NDI_GENDIR_PATH, &ierr);
    gendir_path = (char *)malloc(str_size * sizeof(char));
    ierr = ndi_get_string_val(NDI_GENDIR_PATH, gendir_path, str_size);
/*     printf("  Gendir = %s\n", gendir_path); */
    if (gendir_path) free(gendir_path);
    gendir_path = NULL;


    /* Set path to non-existent gendir */
    ierr = ndi_set_option(NDI_GENDIR_PATH, "../../src/test/lib/gendir.test3");
    if (ierr != 0) {
/*         printf("ERROR SETTING GENDIR PATH IS %d\n", ierr); */
    }
/*     printf("\nNDI_GENDIR_PATH is set to gendir.test3\n"); */
    ierr = ndi_init();
    str_size = ndi_get_size(NDI_GENDIR_PATH, &ierr);
    gendir_path = (char *)malloc(str_size * sizeof(char));
    ierr = ndi_get_string_val(NDI_GENDIR_PATH, gendir_path, str_size);
/*     printf("  Gendir = %s\n", gendir_path); */
    if (gendir_path) free(gendir_path);
    gendir_path = NULL;


    ierr = ndi_close();

    return 0;
}
