
/*
 * File:	ndi_read_dir_block.c
 * Description:	Internal function--read the gendir entry for one ZAID.
 * Time-stamp:	<1998-12-18 18:09:41 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"


static int ndi_init_dir_rec(dir_entry *dir_rec);

/**********************************************************************/

int
read_dir_block(char *in_line, FILE *dir_stream, dir_entry *dir_rec)


/**********************************************************************/
/*  Purpose: Read a record ('line') from the gendir file for one ZAID.
    If in_line does not contain all of the gendir info for the ZAID, then
    dir_stream will be read from to obtain all the info.
    
    Parameters: 
    in_line -- in:Current line from mg section of the gendir file.
    dir_stream -- in:File ptr to current place in the gendir file.
    zaid -- out:ZAID whose dir info is currently being read.
    dir_rec -- pointer to the dir_entry structure to store gendir data.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok. 

***********************************************************************/
/* Handle when there is no end-of-string character. */
{

    int ierr = 0;

    char seps[] = " ,\t\n=";
    char *token;
    char in_str[MAX_IN_STR_LEN];
    int done = 0;

    ierr = ndi_init_dir_rec(dir_rec);
    strcpy(in_str, in_line);
    token = strtok(in_str, seps);

    /* Make check for missing '!' -- see if z= is read twice */

    while (!done) {
	if (token != NULL) {
	    if (strcmp(token, "z") == 0) {
		/* ZAID */
		token = strtok(NULL, seps);
		strcpy(dir_rec->zaid, token);
	    } else if (strcmp(token, "d") == 0) {
		/* Date */
		token = strtok(NULL, seps);
		strcpy(dir_rec->date, token);
	    } else if (strcmp(token, "l") == 0) {
		/* Library */
		token = strtok(NULL, seps);
		strcpy(dir_rec->lib, token);
	    } else if (strcmp(token, "f") == 0) {
		/* file path */
		token = strtok(NULL, seps);
		strcpy(dir_rec->path, token);
	    } else if (strcmp(token, "ft") == 0) {
		/* File type (ascii,bin) */
		token = strtok(NULL, seps);
		strcpy(dir_rec->lib_type, token);
	    } else if (strcmp(token, "ln") == 0) {
		/* Starting line number */
		token = strtok(NULL, seps);
		dir_rec->line_num = strtol(token, (char **) NULL, 10);
	    } else if (strcmp(token, "o") == 0) {
		/* Offset from beginning of library file */
		token = strtok(NULL, seps);
		dir_rec->offset = strtol(token, (char **) NULL, 10);
	    } else if (strcmp(token, "ng") == 0) {
		/* Number of groups */
		token = strtok(NULL, seps);
		dir_rec->num_grps =
		    (int) strtol(token, (char **) NULL, 10);
	    } else if (strcmp(token, "t") == 0) {
		/* Temperature */
		token = strtok(NULL, seps);
		dir_rec->temp = strtod(token, (char **) NULL);
	    } else if (strcmp(token, "s0") == 0) {
		/* Self-shielding -- sigma zero */
		token = strtok(NULL, seps);
		dir_rec->sig_0 = strtod(token, (char **) NULL);
	    } else if (strcmp(token, "aw") == 0) {
		/* Atomic weight */
		token = strtok(NULL, seps);
		dir_rec->at_wgt = strtod(token, (char **) NULL);
	    } else if (strcmp(token, "awr") == 0) {
		/* Atomic weight ratio */
		token = strtok(NULL, seps);
		dir_rec->awr = strtod(token, (char **) NULL);
	    } else if (strcmp(token, "eg") == 0) {
		/* Energy group symbol */
		token = strtok(NULL, seps);
		strcpy(dir_rec->egrp, token);
	    } else if (strcmp(token, "end") == 0) {
		/* Done with this ZAID */
		done = -1;
	    }
	    if (!done) {
		token = strtok(NULL, seps);
	    }
	} else {
	    if (fgets(in_str, sizeof(in_str), dir_stream) != NULL) {
		token = strtok(in_str, seps);
	    } else {
		/* Error reading file */
		ierr = -11;
		done = -1;
	    }
	}
    }				/* while not done */

    return (ierr);
}


/**********************************************************************/

static int
ndi_init_dir_rec(dir_entry *dir_rec)
{

    int ierr = 0;

    *(dir_rec->zaid) = '\0';
    *(dir_rec->date) = '\0';
    *(dir_rec->lib) = '\0';
    *(dir_rec->path) = '\0';
    *(dir_rec->lib_type) = '\0';
    *(dir_rec->egrp) = '\0';

    dir_rec->line_num = -1;
    dir_rec->offset = -1;
    dir_rec->num_grps = 0;
    dir_rec->temp = 0.0;
    dir_rec->sig_0 = 0.0;
    dir_rec->at_wgt = 0.0;
    dir_rec->awr = 0.0;

    return (ierr);
}

/**********************************************************************/
