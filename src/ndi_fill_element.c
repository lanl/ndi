
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
#include "ndi_state.h"


/**********************************************************************/


int ndi_fill_elements ( void ) 

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

   /*  Allocate space and zero array  */
   if ( ndi_elements ) ndi_free( (void **) &ndi_elements );
   ndi_elements = (short *)ndi_malloc((rows * cols + 1) * sizeof(short));
   memset( ndi_elements, 0, sizeof(ndi_elements) );
   for ( i = 0; i < 702; i++ ) {
     ndi_elements[i] = 0;
   }

   ndi_elements[element_index("h ")] = 1; 
   ndi_elements[element_index("he")] = 2;
   ndi_elements[element_index("li")] = 3;
   ndi_elements[element_index("be")] = 4;
   ndi_elements[element_index("b ")] = 5;
   ndi_elements[element_index("c ")] = 6;
   ndi_elements[element_index("n ")] = 7;
   ndi_elements[element_index("o ")] = 8;
   ndi_elements[element_index("f ")] = 9;
   ndi_elements[element_index("ne")] = 10;
   ndi_elements[element_index("na")] = 11;
   ndi_elements[element_index("mg")] = 12;
   ndi_elements[element_index("al")] = 13;
   ndi_elements[element_index("si")] = 14;
   ndi_elements[element_index("p ")] = 15;
   ndi_elements[element_index("s ")] = 16;
   ndi_elements[element_index("cl")] = 17;
   ndi_elements[element_index("ar")] = 18;
   ndi_elements[element_index("k ")] = 19;
   ndi_elements[element_index("ca")] = 20;
   ndi_elements[element_index("sc")] = 21;
   ndi_elements[element_index("ti")] = 22;
   ndi_elements[element_index("v ")] = 23;
   ndi_elements[element_index("cr")] = 24;
   ndi_elements[element_index("mn")] = 25;
   ndi_elements[element_index("fe")] = 26;
   ndi_elements[element_index("co")] = 27;
   ndi_elements[element_index("ni")] = 28;
   ndi_elements[element_index("cu")] = 29;
   ndi_elements[element_index("zn")] = 30;
   ndi_elements[element_index("ga")] = 31;
   ndi_elements[element_index("ge")] = 32;
   ndi_elements[element_index("as")] = 33;
   ndi_elements[element_index("se")] = 34;
   ndi_elements[element_index("br")] = 35;
   ndi_elements[element_index("kr")] = 36;
   ndi_elements[element_index("rb")] = 37;
   ndi_elements[element_index("sr")] = 38;
   ndi_elements[element_index("y ")] = 39;
   ndi_elements[element_index("zr")] = 40;
   ndi_elements[element_index("nb")] = 41;
   ndi_elements[element_index("mo")] = 42;
   ndi_elements[element_index("tc")] = 43;
   ndi_elements[element_index("ru")] = 44;
   ndi_elements[element_index("rh")] = 45;
   ndi_elements[element_index("pd")] = 46;
   ndi_elements[element_index("ag")] = 47;
   ndi_elements[element_index("cd")] = 48;
   ndi_elements[element_index("in")] = 49;
   ndi_elements[element_index("sn")] = 50;
   ndi_elements[element_index("sb")] = 51;
   ndi_elements[element_index("te")] = 52;
   ndi_elements[element_index("i ")] = 53;
   ndi_elements[element_index("xe")] = 54;
   ndi_elements[element_index("cs")] = 55;
   ndi_elements[element_index("ba")] = 56;
   ndi_elements[element_index("la")] = 57;
   ndi_elements[element_index("ce")] = 58;
   ndi_elements[element_index("pr")] = 59;
   ndi_elements[element_index("nd")] = 60;
   ndi_elements[element_index("pm")] = 61;
   ndi_elements[element_index("sm")] = 62;
   ndi_elements[element_index("eu")] = 63;
   ndi_elements[element_index("gd")] = 64;
   ndi_elements[element_index("tb")] = 65;
   ndi_elements[element_index("dy")] = 66;
   ndi_elements[element_index("ho")] = 67;
   ndi_elements[element_index("er")] = 68;
   ndi_elements[element_index("tm")] = 69;
   ndi_elements[element_index("yb")] = 70;
   ndi_elements[element_index("lu")] = 71;
   ndi_elements[element_index("hf")] = 72;
   ndi_elements[element_index("ta")] = 73;
   ndi_elements[element_index("w ")] = 74;
   ndi_elements[element_index("re")] = 75;
   ndi_elements[element_index("os")] = 76;
   ndi_elements[element_index("ir")] = 77;
   ndi_elements[element_index("pt")] = 78;
   ndi_elements[element_index("au")] = 79;
   ndi_elements[element_index("hg")] = 80;
   ndi_elements[element_index("tl")] = 81;
   ndi_elements[element_index("pb")] = 82;
   ndi_elements[element_index("bi")] = 83;
   ndi_elements[element_index("po")] = 84;
   ndi_elements[element_index("at")] = 85;
   ndi_elements[element_index("rn")] = 86;
   ndi_elements[element_index("fr")] = 87;
   ndi_elements[element_index("ra")] = 88;
   ndi_elements[element_index("ac")] = 89;
   ndi_elements[element_index("th")] = 90;
   ndi_elements[element_index("pa")] = 91;
   ndi_elements[element_index("u ")] = 92;
   ndi_elements[element_index("np")] = 93;
   ndi_elements[element_index("pu")] = 94;
   ndi_elements[element_index("am")] = 95;
   ndi_elements[element_index("cm")] = 96;
   ndi_elements[element_index("bk")] = 97;
   ndi_elements[element_index("cf")] = 98;
   ndi_elements[element_index("es")] = 99;
   ndi_elements[element_index("fm")] = 100;
   ndi_elements[element_index("md")] = 101;
   ndi_elements[element_index("no")] = 102;
   ndi_elements[element_index("lr")] = 103;  /* Value of MAX_Z */

   return(ret_code);
  
}

