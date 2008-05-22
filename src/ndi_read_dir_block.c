
/*
 * $Id$
 * Description: Internal function--read the gendir entry for one ZAID.
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
int read_dir_block(char *in_line, FILE *dir_stream, dir_entry *dir_rec)
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
    }                           /* while not done */

    return (ierr);
}


/**********************************************************************/
/**********************************************************************/

static int ndi_init_dir_rec(dir_entry *dir_rec)
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
/**********************************************************************/

/* ndi_fseek_section sets the file position for stream; a subsequent
 * read or write will access data begining at the new position.  The
 * position is set to the line following the first line that matches
 * str.  Since the GENDIR file format has section headers starting on
 * a new line, this routine provides a convenient way to find a
 * section in a GENDIR file.  Search from beginning of the file if
 * wherefrom = 0, the current position if wherefrom = 1.  Restores
 * file position and returns non-zero on error. */

int ndi_fseek_section(FILE *stream, const char *str, int wherefrom)
{
    char *l;
    char line[MAX_IN_STR_LEN];
    long fpos;

    fpos = ftell(stream);      /* remember file position */

    if (wherefrom == 0) rewind(stream); /* start from beginning */

    while ((l = fgets(line, sizeof(line), stream)) != NULL)
        if (strncmp(line, str, strlen(str)) == 0) break;

    if (l == NULL) {            /* while ended by EOF */
        fseek(stream, fpos, SEEK_SET); /* restore original file position */
        return -1;            /* not found or file error */
    } else {                    /* while ended by break */
        return 0;             /* success */
    }
}

/**********************************************************************/
/**********************************************************************/

/* ndi_fget_dir_block reads the next record in the GENDIR file and
 * attempts to parse it as a dir_block.  Returns rec if successful or
 * NULL if end of section, end of file, or error occurs. */

dir_entry *ndi_fget_dir_block(dir_entry *rec, FILE *stream)
{
    char *l = NULL;
    char s[] = " ,\t\n=";
    char *t;
    char line[MAX_IN_STR_LEN];
    int n = 0;

    if (ndi_init_dir_rec(rec) != 0) goto catch;

    while ((l=fgets(line, sizeof(line), stream)) != NULL) {
        for (t = strtok(line, s); t != NULL; t = strtok(NULL, s)) {
            if (strcmp(t, "z") == 0) { /* ZAID */
                t = strtok(NULL, s);
                strcpy(rec->zaid, t);
                n++;
            } else if (strcmp(t, "d") == 0) { /* Date */
                t = strtok(NULL, s);
                strcpy(rec->date, t);
                n++;
            } else if (strcmp(t, "l") == 0) { /* Library */
                t = strtok(NULL, s);
                strcpy(rec->lib, t);
                n++;
            } else if (strcmp(t, "f") == 0) { /* file path */
                t = strtok(NULL, s);
                strcpy(rec->path, t);
                n++;
            } else if (strcmp(t, "ft") == 0) { /* File type (asc,bin) */
                t = strtok(NULL, s);
                strcpy(rec->lib_type, t);
                n++;
            } else if (strcmp(t, "ln") == 0) { /* Start line number */
                t = strtok(NULL, s);
                rec->line_num = atol(t);
                n++;
            } else if (strcmp(t, "o") == 0) { /* File offset */
                t = strtok(NULL, s);
                rec->offset = atol(t);
                n++;
            } else if (strcmp(t, "ng") == 0) {/* Number of groups */
                t = strtok(NULL, s);
                rec->num_grps = atoi(t);
                n++;
            } else if (strcmp(t, "t") == 0) {/* Temperature */
                t = strtok(NULL, s);
                rec->temp = atof(t);
                n++;
            } else if (strcmp(t, "s0") == 0) {/* Self-shielding */
                t = strtok(NULL, s);
                rec->sig_0 = atof(t);
                n++;
            } else if (strcmp(t, "aw") == 0) {/* Atomic weight */
                t = strtok(NULL, s);
                rec->at_wgt = atof(t);
                n++;
            } else if (strcmp(t, "awr") == 0) {/* Atomic weight ratio */
                t = strtok(NULL, s);
                rec->awr = atof(t);
                n++;
            } else if (strcmp(t, "eg") == 0) { /* Energy group symbol */
                t = strtok(NULL, s);
                strcpy(rec->egrp, t);
                n++;
            } else if (strcmp(t, "end") == 0) {/* End of Record */
                goto catch;
            }
        }
    }

 catch:
    if (n == 0 || l == NULL) {  /* EOF or end of section */
        return NULL;
    } else {                    /* Success */
        return rec;
    }
}

/**********************************************************************/
/**********************************************************************/

/* ndi_fget_next_dir reads records in the GENDIR file and attempts to
 * parse it as a series of dir_blocks.  Returns rec if a record is
 * found that matches input lib and zaid or NULL if end of
 * section, end of file, or error occurs */  

dir_entry *ndi_fget_next_dir(dir_entry *rec, FILE *stream,
                             const char *lib, const char *zaid)
{
    dir_entry *d;

    while ((d = ndi_fget_dir_block(rec, stream)) != NULL) {
        if ((strncmp(d->zaid, zaid, strlen(zaid)) == 0) &&
            ((strcmp("", lib) == 0) ||
             (strcmp(d->lib, lib) == 0)))
            break;
    }
     
    return d;
}

/**********************************************************************/
/**********************************************************************/

/* ndi_count_dir returns the size of in_key in the specified section
 * of a GENDIR stream matching lib and zaid.  Returns the number of
 * matches for double quantities, or the total string length,
 * including EOS, for string quantities.  Restores original file
 * position and returns a negative error flag if unsuccessful. */

int ndi_count_dir_block(FILE *stream, int in_key, const char *section, 
                        const char *lib, const char *zaid)
{
    long fpos = -1;
    dir_entry rec;
    int count = 0;

    if (section == 0) NDI_THROW(count, -16); /* Library type not set */

    fpos = ftell(stream); /* remember file position */
        
    if (ndi_fseek_section(stream, section, 0) != 0)
        NDI_THROW(count, -11); /* Section not found */
    
    while (ndi_fget_next_dir(&rec, stream, lib, zaid))
        switch (in_key) {
        case NDI_ZAID:
            count += strlen(rec.zaid) + 1;
            break;
        case NDI_TEMP:
        case NDI_SIG_0:
        case NDI_AT_WGT:
        case NDI_AWR:
            count++;
            break;
        default:
            NDI_THROW(count, -4); /* Unrecognized input key */
            break;
        }

 catch:
    if (fpos >= 0) fseek(stream, fpos, SEEK_SET);
    return count;
}

/**********************************************************************/
/**********************************************************************/

/* ndi_float_dir_block fills the vec array of double in_key values
 * read from a GENDIR stream section which match the input lib and
 * zaid.  Restores original file position and returns zero on success,
 * negative on error, positive on warning */

int ndi_float_dir_block(FILE *stream, int in_key, const char *section, 
                        const char *lib, const char *zaid,
                        double *vec, int size)
{
    int ierr = 0;
    long fpos = -1;
    dir_entry rec;
    int i = 0;
    double *dptr;

    if (section == 0) NDI_THROW(ierr, -16); /* Library type not set */

    switch(in_key) {            /* Process keyword: select in_key result */
    case NDI_TEMP:
        dptr = &rec.temp;
        break;
    case NDI_SIG_0:
        dptr = &rec.sig_0;
        break;
    case NDI_AT_WGT:
        dptr = &rec.at_wgt;
        break;
    case NDI_AWR:
        dptr = &rec.awr;
        break;
    default:
        NDI_THROW(ierr, -4);   /* Unrecognized input key */
    }

    fpos = ftell(stream); /* remember file position */
        
    if (ndi_fseek_section(stream, section, 0) != 0)
        NDI_THROW(ierr, -11); /* Section not found */
    
    while (ndi_fget_next_dir(&rec, stream, lib, zaid)) {
        if (i < size) {
            vec[i++] = *dptr;
        } else {
            NDI_THROW(ierr, 7);
            break;
        }
    }

 catch:
    if (fpos >= 0) fseek(stream, fpos, SEEK_SET);
    return ierr;
}

/**********************************************************************/
