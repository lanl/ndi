
/*
 * File:        ndi_parse_zaid.c
 * Description:        Internal function--parse zaid into Z, A, and state
 * Time-stamp:        <2003-03-25 09:26:02 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "ndi.h"
#include "ndi_internal.h"

static int ndi_parse_symbolic_zaid(state_struct *state, char *in_zaid,
                                   int *Z, int *A, int *S);
static int ndi_find_symbolic_state(char *in_str);

/**********************************************************************/

int
ndi_parse_str_zaid(state_struct *state, char *zaid_str, int *Z,
                   int *A, int *S)
{

    /* Parse a ZAID represented as a string.  This ZAID can be symbolic
       (e.g. U235, h-1, am240m2, u235.600nm) or numeric (92235, 1001,
       2095240, 92235.600nm).
     */

    int ierr = 0;
    char *tmp_zaid = zaid_str;
    char seps[] = " ,\t\n";

    if (!state) {
        return -2;
    }

    tmp_zaid = strtok(zaid_str, seps);

    if (isalpha(*tmp_zaid)) {
        /* First character is alphabetic, implying a symbolic zaid */
        ierr = ndi_parse_symbolic_zaid(state, tmp_zaid, Z, A, S);
    } else {
        /* Numeric (integer) zaid */
        /* ZAID extension ('.' and characters following) is removed by strtol */
        int tmp_izaid = (int) strtol(tmp_zaid, (char **) NULL, 10);
        ierr = ndi_parse_int_zaid(tmp_izaid, Z, A, S);
    }

    return ierr;
}

/**********************************************************************/
/* Name or Serpent naming convention:

   symbolic_sza = E, ['-'], A, S;
              E = ? case insensitive element symbol ?;
              A = {digit}
              S = ('g'|'G') | (('m'|'M'), {digit}), eos; 
*/
int ndi_parse_symbolic_zaid(state_struct *state, char *in_zaid, 
                            int *Z, int *A, int *S)
{
    /* Parse a symbolic zaid, i.e. of the form U235 or Pu-239.  
       The zaid may have an ending extension, i.e. H1.600nm.
       This function should be case insensitive, e.g. H1 == h1.
       Leading blanks have been removed, the first character
       should be the start of the element symbol.
     */

    int ierr = 0;
    char *curr_char = in_zaid;
    char element_symbol[2] = "  ";

    if (!state) {
        return -2;
    }

    /* Find Z = alpha, [alpha]; */
    element_symbol[0] = tolower(*curr_char);
    curr_char++;

    if (isalpha(*curr_char)) {
        element_symbol[1] = tolower(*curr_char);
        curr_char++;
    }

    *Z = state->ndi_elements[element_index(element_symbol)];

    /* Skip optional dash */
    if (*curr_char == '-') {
        curr_char = curr_char + 1;
    }

    /* Find A = {digit} */
    if (isdigit(*curr_char)) {
        *A = (int) strtol(curr_char, &curr_char, 10);
    } else {
        *A = 0;
    }

    /* Find S */
    *S = ndi_find_symbolic_state(curr_char);
    return ierr;

}

/**********************************************************************/
/* SZA naming convention:

   sza = (S * 1000 + Z) * 1000 + A 

   Reqt. 20: Simplify math */
int ndi_parse_int_zaid(int sza, int *Z, int *A, int *S)
{
    *S = sza / 1000000;
    *Z = (sza / 1000) % 1000;
    *A = sza % 1000;
    return 0;
}

/**********************************************************************/
/* symbolic_state = [body] | eos;
             body = ('g'|'G') | (('m'|'M'), {digit});
              eos = '.' | ({ \t\f\n}), '\0');

   Reqt. 20: refactor
*/
static int ndi_find_symbolic_state(char *in_str)
{

    /*  Find the state in the input string.  The state will be
       of the form m2 (second excited state).  The integer
       following the character 'm' is the state number to be
       returned.  If there is no number following the 'm', the
       state is 1.  If there is no 'm' in the string (before
       the period '.'), then the state is 0.

       It is assumed that the in_str pointer is pointing to the
       string after the elemental symbol (since this function
       can't distinguish an 'm' which is part of the elemental
       symbol).

       It is assumed that the string is ended with a string-
       terminating character '\0'.  
     */

    int s = 0;
    char *c = in_str;

    /* body */
    switch (tolower(*c)) {
    case 'g':                   /* Explicit ground state */
        c++;
        break;
    case 'm':                   /* Explicit iso-state */
        c++;
        if (isdigit(*c))
            s = (int) strtol(c, &c, 10);
        else
            s = 1;
        break;
    }

    /* eos */
    if (*c == '.')
        return s;
    while (isspace(*c))
        c++;                    /* skip whitespace */
    if (*c == '\0')
        return s;
    else
        return -1;              /* Error! */
}

/**********************************************************************/
