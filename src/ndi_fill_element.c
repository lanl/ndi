
/*
 * File:	ndi_fill_element.c
 * Description:	Internal function--fill lookup array of elements.
 * Time-stamp:	<1998-09-14 19:42:17 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"


/**********************************************************************/


int ndi_fill_elements ( state_struct *state ) 

/*  Purpose:  Fill the external short array ndi_elements with 
    the z-number.  The array can be referenced using the element_index
    function and the chemical symbol.

    It is assumed that only the first MAX_Z elements will ever be needed.
    This is implemented in a very inelegant way--it is assumed that the
    ndi_elements array has 26 rows (representing the first letter of the
    symbol) and 27 columns (representing the 2nd letter or blank space).
    The input to element_index is assumed to be lower-case.

    Return values:  Returns negative value for error condition,
    positive value for warning, and a '0' if everything's ok.  The
    array ndi_elements will be filled.

***********************************************************************/
{
   int ret_code = 0;
   int rows = 26;
   int cols = 27;
   int i;

   if (! state) {
       return(-2);
   }

   /*  Allocate space and zero array  */
   if ( state->ndi_elements ) ndi_free( (void **) &(state->ndi_elements) );
   state->ndi_elements = (short *)ndi_malloc((rows * cols + 1) * sizeof(short));
   memset( state->ndi_elements, 0, sizeof(state->ndi_elements) );
   for ( i = 0; i < 702; i++ ) {
     state->ndi_elements[i] = 0;
   }

   state->ndi_elements[element_index("h ")] = 1; 
   state->ndi_elements[element_index("he")] = 2;
   state->ndi_elements[element_index("li")] = 3;
   state->ndi_elements[element_index("be")] = 4;
   state->ndi_elements[element_index("b ")] = 5;
   state->ndi_elements[element_index("c ")] = 6;
   state->ndi_elements[element_index("n ")] = 7;
   state->ndi_elements[element_index("o ")] = 8;
   state->ndi_elements[element_index("f ")] = 9;
   state->ndi_elements[element_index("ne")] = 10;
   state->ndi_elements[element_index("na")] = 11;
   state->ndi_elements[element_index("mg")] = 12;
   state->ndi_elements[element_index("al")] = 13;
   state->ndi_elements[element_index("si")] = 14;
   state->ndi_elements[element_index("p ")] = 15;
   state->ndi_elements[element_index("s ")] = 16;
   state->ndi_elements[element_index("cl")] = 17;
   state->ndi_elements[element_index("ar")] = 18;
   state->ndi_elements[element_index("k ")] = 19;
   state->ndi_elements[element_index("ca")] = 20;
   state->ndi_elements[element_index("sc")] = 21;
   state->ndi_elements[element_index("ti")] = 22;
   state->ndi_elements[element_index("v ")] = 23;
   state->ndi_elements[element_index("cr")] = 24;
   state->ndi_elements[element_index("mn")] = 25;
   state->ndi_elements[element_index("fe")] = 26;
   state->ndi_elements[element_index("co")] = 27;
   state->ndi_elements[element_index("ni")] = 28;
   state->ndi_elements[element_index("cu")] = 29;
   state->ndi_elements[element_index("zn")] = 30;
   state->ndi_elements[element_index("ga")] = 31;
   state->ndi_elements[element_index("ge")] = 32;
   state->ndi_elements[element_index("as")] = 33;
   state->ndi_elements[element_index("se")] = 34;
   state->ndi_elements[element_index("br")] = 35;
   state->ndi_elements[element_index("kr")] = 36;
   state->ndi_elements[element_index("rb")] = 37;
   state->ndi_elements[element_index("sr")] = 38;
   state->ndi_elements[element_index("y ")] = 39;
   state->ndi_elements[element_index("zr")] = 40;
   state->ndi_elements[element_index("nb")] = 41;
   state->ndi_elements[element_index("mo")] = 42;
   state->ndi_elements[element_index("tc")] = 43;
   state->ndi_elements[element_index("ru")] = 44;
   state->ndi_elements[element_index("rh")] = 45;
   state->ndi_elements[element_index("pd")] = 46;
   state->ndi_elements[element_index("ag")] = 47;
   state->ndi_elements[element_index("cd")] = 48;
   state->ndi_elements[element_index("in")] = 49;
   state->ndi_elements[element_index("sn")] = 50;
   state->ndi_elements[element_index("sb")] = 51;
   state->ndi_elements[element_index("te")] = 52;
   state->ndi_elements[element_index("i ")] = 53;
   state->ndi_elements[element_index("xe")] = 54;
   state->ndi_elements[element_index("cs")] = 55;
   state->ndi_elements[element_index("ba")] = 56;
   state->ndi_elements[element_index("la")] = 57;
   state->ndi_elements[element_index("ce")] = 58;
   state->ndi_elements[element_index("pr")] = 59;
   state->ndi_elements[element_index("nd")] = 60;
   state->ndi_elements[element_index("pm")] = 61;
   state->ndi_elements[element_index("sm")] = 62;
   state->ndi_elements[element_index("eu")] = 63;
   state->ndi_elements[element_index("gd")] = 64;
   state->ndi_elements[element_index("tb")] = 65;
   state->ndi_elements[element_index("dy")] = 66;
   state->ndi_elements[element_index("ho")] = 67;
   state->ndi_elements[element_index("er")] = 68;
   state->ndi_elements[element_index("tm")] = 69;
   state->ndi_elements[element_index("yb")] = 70;
   state->ndi_elements[element_index("lu")] = 71;
   state->ndi_elements[element_index("hf")] = 72;
   state->ndi_elements[element_index("ta")] = 73;
   state->ndi_elements[element_index("w ")] = 74;
   state->ndi_elements[element_index("re")] = 75;
   state->ndi_elements[element_index("os")] = 76;
   state->ndi_elements[element_index("ir")] = 77;
   state->ndi_elements[element_index("pt")] = 78;
   state->ndi_elements[element_index("au")] = 79;
   state->ndi_elements[element_index("hg")] = 80;
   state->ndi_elements[element_index("tl")] = 81;
   state->ndi_elements[element_index("pb")] = 82;
   state->ndi_elements[element_index("bi")] = 83;
   state->ndi_elements[element_index("po")] = 84;
   state->ndi_elements[element_index("at")] = 85;
   state->ndi_elements[element_index("rn")] = 86;
   state->ndi_elements[element_index("fr")] = 87;
   state->ndi_elements[element_index("ra")] = 88;
   state->ndi_elements[element_index("ac")] = 89;
   state->ndi_elements[element_index("th")] = 90;
   state->ndi_elements[element_index("pa")] = 91;
   state->ndi_elements[element_index("u ")] = 92;
   state->ndi_elements[element_index("np")] = 93;
   state->ndi_elements[element_index("pu")] = 94;
   state->ndi_elements[element_index("am")] = 95;
   state->ndi_elements[element_index("cm")] = 96;
   state->ndi_elements[element_index("bk")] = 97;
   state->ndi_elements[element_index("cf")] = 98;
   state->ndi_elements[element_index("es")] = 99;
   state->ndi_elements[element_index("fm")] = 100;
   state->ndi_elements[element_index("md")] = 101;
   state->ndi_elements[element_index("no")] = 102;
   state->ndi_elements[element_index("lr")] = 103;  /* Value of MAX_Z */

   return(ret_code);
  
}

