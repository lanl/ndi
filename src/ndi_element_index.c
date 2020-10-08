
/*
 * File:        ndi_element_index.c
 * Description:        Internal function--convert elemental symbol to array index.
 * Time-stamp:        <1999-04-06 17:52:31 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ndi.h"
#include "ndi_internal.h"


/**********************************************************************/

int element_index(const char *el_symbol)

/*  Purpose:  Calculate an index to an array given a lower case elemental
    symbol.

    This is implemented in a very inelegant way--a 26x27 matrix representing
    the first character by the 2nd character. 

    Parameters:  el_symbol = two-character string representing elemental symbol

    Assumptions:
    It is assumed that the input is a two-character string of 
    lowercase letters (a-z); the last character can also be a blank
    space (for a 1-character symbol).
    
    The elements array has 26 rows (representing the first letter of the
    symbol) and 27 columns (representing the 2nd letter or blank space).

    Return values:  Returns the array index corresponding to the input
    elemental symbol--integer between 0 and 702.

***********************************************************************/
{
    int row;                        /* Array row--1st character of symbol a=0,b=1...      */
    int col;                        /* Array column--2nd character of symbol a=0,b=1...
                                   blank space or '-' = 26             */


/******  How should errors be handled????   *******/

    /* Convert 1st character to integer index */
    row = (int) (tolower(*el_symbol) - 'a');
    if ((row < 0) || (row >= 26)) {
/*       printf("Error in row; char = *%c* row = %d  %d  %d\n",  */
/*              *el_symbol, row, *el_symbol, 'a'); */
        return 0;
    }


    /* Convert 2nd character to integer index */

    ++el_symbol;
    col = (int) (tolower(*el_symbol) - 'a');
    if ((*el_symbol == ' ') || (*el_symbol == '-')) {
        col = 26;
    }
    if ((col < 0) || (col >= 27)) {
/*       printf("Error in col\n"); */
        return 0;
    }

    return (row * 27) + col;

}

/***********************************************************************/
