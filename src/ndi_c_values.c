
/*
 * File:	ndi_def_c_values.c
 * Description:	Internal function--fill array LLNL c-values.
 * Time-stamp:	<1998-12-10 11:12:55 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ndi.h"
#include "ndi_internal.h"

static int ndi_mt_to_label(int mt, char *label);
static int ndi_label_to_mt(const char *in_value);
static int ndi_niceStrCp(char *new_str, const char *old_str, int new_len);

/**********************************************************************/

int
ndi_c_c_to_mt(int c_val)
{

/*  Sources:  C-values: xeroxed copy of "Introduction" chapter of draft OMEGA 
              documentation from LLNL.
              MT-values: ENDF Format Manual, NNDC, Brookhaven, Appendix B

	      What to do when ENDF LR flags should be used?  as in c=20,21

    Parameters:  
    c_val -- in:integer corresponding to a LLNL C-value.

    Returned value:  The MT number corresponding to the input C value is 
                     returned.  0 is returned for unassigned C values and 
		     for C-values with no equivalent MT number. 
***********************************************************************/
    int ndi_c_value[100];

/*              C-value        MT-value                    */
    ndi_c_value[1] = 1;		/* (n, total) */
    ndi_c_value[2] = -1;	/* unassigned */
    ndi_c_value[3] = -1;	/* unassigned */
    ndi_c_value[4] = -1;	/* unassigned */
    ndi_c_value[5] = -1;	/* unassigned */
    ndi_c_value[6] = -1;	/* unassigned */
    ndi_c_value[7] = -1;	/* unassigned */
    ndi_c_value[8] = 0;		/* lg. angle coul. scat.--no MT equivalent */
    ndi_c_value[9] = 0;		/* elas. + interference--no MT equivalent */
    ndi_c_value[10] = 2;	/* elastic */
    ndi_c_value[11] = 4;	/* (n,n') - inelastic */
    ndi_c_value[12] = 16;	/* (n,2n) */
    ndi_c_value[13] = 17;	/* (n,3n) */
    ndi_c_value[14] = 37;	/* (n,4n) */
    ndi_c_value[15] = 18;	/* (n,fission) - total fission */
    ndi_c_value[16] = 0;	/* unassigned */
    ndi_c_value[17] = 0;	/* unassigned */
    ndi_c_value[18] = 0;	/* unassigned */
    ndi_c_value[19] = 0;	/* unassigned */
    ndi_c_value[20] = 28;	/* (n,n'p) */
    ndi_c_value[21] = 28;	/* ?(n,pn') */
    ndi_c_value[22] = 32;	/* (n,n'd) */
    ndi_c_value[23] = 35;	/* (n,n'd2a) */
    ndi_c_value[24] = 33;	/* (n,n't) */
    ndi_c_value[25] = 34;	/* (n,n'3^He) */
    ndi_c_value[26] = 22;	/* (n,n'a) */
    ndi_c_value[27] = 23;	/* (n,n'3a) */
    ndi_c_value[28] = 36;	/* (n,n't2a) */
    ndi_c_value[29] = 41;	/* (n,2np) */
    ndi_c_value[30] = -1;	/* ??(n,gna) */
    ndi_c_value[31] = 30;	/* (n,2n2a) */
    ndi_c_value[32] = -1;	/* (n,2nda) */
    ndi_c_value[33] = 0;	/* unassigned */
    ndi_c_value[34] = 0;	/* unassigned */
    ndi_c_value[35] = 0;	/* unassigned */
    ndi_c_value[36] = 0;	/* unassigned */
    ndi_c_value[37] = 109;	/* (n,3a) */
    ndi_c_value[38] = -1;	/* (n,3He2a) */
    ndi_c_value[39] = -1;	/* (n,pta) */
    ndi_c_value[40] = 103;	/* (n,p) */
    ndi_c_value[41] = 104;	/* (n,d) */
    ndi_c_value[42] = 105;	/* (n, t) */
    ndi_c_value[43] = 113;	/* (n,t2a) */
    ndi_c_value[44] = 106;	/* (n,3^He) */
    ndi_c_value[45] = 107;	/* (n,a) */
    ndi_c_value[46] = 102;	/* (n,g) */
    ndi_c_value[47] = 0;	/* unassigned */
    ndi_c_value[48] = 0;	/* unassigned */
    ndi_c_value[49] = 0;	/* unassigned */
    ndi_c_value[50] = 203;	/* (n,Xp) */
    ndi_c_value[51] = 204;	/* (n,Xd) */
    ndi_c_value[52] = -1;	/* ?Unspecified in my LLNL source (n,Xt)?? */
    ndi_c_value[53] = 206;	/* (n,X3^He) */
    ndi_c_value[54] = 207;	/* (n,Xa) */
    ndi_c_value[55] = 202;	/* (n,Xg) */
    ndi_c_value[56] = 201;	/* (n,Xn) */
    ndi_c_value[57] = -1;	/* (n,Xbeta-) */
    ndi_c_value[58] = 0;	/* unassigned */
    ndi_c_value[59] = 0;	/* unassigned */
    ndi_c_value[60] = 0;	/* unassigned */
    ndi_c_value[61] = 0;	/* unassigned */
    ndi_c_value[62] = 0;	/* unassigned */
    ndi_c_value[63] = 0;	/* unassigned */
    ndi_c_value[64] = 0;	/* unassigned */
    ndi_c_value[65] = -1;	/* Activation (undefined reaction) */
    ndi_c_value[66] = 0;	/* unassigned */
    ndi_c_value[67] = 0;	/* unassigned */
    ndi_c_value[68] = 0;	/* unassigned */
    ndi_c_value[69] = 0;	/* unassigned */
    ndi_c_value[70] = 501;	/* Total photon interaction */
    ndi_c_value[71] = 502;	/* Coherent scattering */
    ndi_c_value[72] = 504;	/* Incoherent scattering */
    ndi_c_value[73] = 522;	/* Photoelectric */
    ndi_c_value[74] = 516;	/* Pair production */


    if ((c_val < 1) || (c_val > 74)) {
	return (0);
    } else {
	return (ndi_c_value[c_val]);
    }
}

/**********************************************************************/

/**********************************************************************/

int
ndi_c_mt_to_c(int mt)
{

/*  Sources:  C-values: xeroxed copy of "Introduction" chapter of draft OMEGA 
              documentation from LLNL.
              MT-values: ENDF Format Manual, NNDC, Brookhaven, Appendix B

	      What to do when ENDF LR flags should be used?  as in c=20,21

    Parameters:  
    mt -- in:integer corresponding to an ENDF MT number (reaction type)..

    Returned value:  The C value corresponding to the input MT number is 
                     returned.  0 is returned for unassigned MT numbers and 
		     for MT's with no equivalent C value. 
***********************************************************************/

    int c_val = 0;

    switch (mt) {

    case 1:			/* (n, total) */
	c_val = 1;
	break;
    case 2:			/* elastic */
	c_val = 10;
	break;
    case 4:			/* (n,n') - inelastic */
	c_val = 11;
	break;
    case 16:			/* (n,2n) */
	c_val = 12;
	break;
    case 17:			/* (n,3n) */
	c_val = 13;
	break;
    case 18:			/* (n,fission) - total fission */
	c_val = 15;
	break;
    case 22:			/* (n,n'a) */
	c_val = 26;
	break;
    case 23:			/* (n,n'3a) */
	c_val = 27;
	break;
    case 28:			/* (n,n'p) */
	c_val = 20;
	break;
    case 30:			/* (n,2n2a) */
	c_val = 31;
	break;
    case 32:			/* (n,n'd) */
	c_val = 22;
	break;
    case 33:			/* (n,n't) */
	c_val = 24;
	break;
    case 34:			/* (n,n'3^He) */
	c_val = 25;
	break;
    case 35:			/* (n,n'd2a) */
	c_val = 23;
	break;
    case 36:			/* (n,n't2a) */
	c_val = 28;
	break;
    case 37:			/* (n,4n) */
	c_val = 14;
	break;
    case 41:			/* (n,2np) */
	c_val = 29;
	break;
    case 102:			/* (n,g) */
	c_val = 46;
	break;
    case 103:			/* (n,p) */
	c_val = 40;
	break;
    case 104:			/* (n,d) */
	c_val = 41;
	break;
    case 105:			/* (n, t) */
	c_val = 42;
	break;
    case 106:			/* (n,3^He) */
	c_val = 44;
	break;
    case 107:			/* (n,a) */
	c_val = 45;
	break;
    case 109:			/* (n,3a) */
	c_val = 37;
	break;
    case 113:			/* (n,t2a) */
	c_val = 43;
	break;
    case 201:			/* (n,Xn) */
	c_val = 56;
	break;
    case 202:			/* (n,Xg) */
	c_val = 55;
	break;
    case 203:			/* (n,Xp) */
	c_val = 50;
	break;
    case 204:			/* (n,Xd) */
	c_val = 51;
	break;
    case 206:			/* (n,X3^He) */
	c_val = 53;
	break;
    case 207:			/* (n,Xa) */
	c_val = 54;
	break;
    case 501:			/* Total photon interaction */
	c_val = 70;
	break;
    case 502:			/* Coherent scattering */
	c_val = 71;
	break;
    case 504:			/* Incoherent scattering */
	c_val = 72;
	break;
    case 522:			/* Photoelectric */
	c_val = 73;
	break;
    case 516:			/* Pair production */
	c_val = 74;
	break;
    default:
	c_val = 0;
	break;
    }

    return (c_val);
}

/**********************************************************************/

/**********************************************************************/

static int
ndi_mt_to_label(int mt, char *label)
{

/*  Sources:  MT-values: ENDF Format Manual, NNDC, Brookhaven, Appendix B
	      What to do when ENDF LR flags should be used?  as in c=20,21

    Parameters:  
    mt -- in:integer corresponding to an ENDF MT number (reaction type)..

    Returned value:  A pointer to a string containing a reaction label.

***********************************************************************/

    int ierr = 0;

    switch (mt) {

    case 1:			/* (n, total) */
	strncpy(label, "(n,total)", 10);
	break;
    case 2:			/* elastic */
	strncpy(label, "elastic", 8);
	break;
    case 4:			/* (n,n') - inelastic */
	strncpy(label, "inelastic", 10);
	break;
    case 16:			/* (n,2n) */
	strncpy(label, "(n,2n)", 7);
	break;
    case 17:			/* (n,3n) */
	strncpy(label, "(n,3n)", 7);
	break;
    case 18:			/* (n,fission) - total fission */
	strncpy(label, "(n,Xf)", 7);
	break;
    case 22:			/* (n,n'a) */
	strncpy(label, "(n,n'alpha)", 12);
	break;
    case 23:			/* (n,n'3a) */
	strncpy(label, "(n,n'3alpha)", 13);
	break;
    case 28:			/* (n,n'p) */
	strncpy(label, "(n,n'p)", 8);
	break;
    case 30:			/* (n,2n2a) */
	strncpy(label, "(n,2n2alpha)", 13);
	break;
    case 32:			/* (n,n'd) */
	strncpy(label, "(n,n'd)", 8);
	break;
    case 33:			/* (n,n't) */
	strncpy(label, "(n,n't)", 8);
	break;
    case 34:			/* (n,n'3^He) */
	strncpy(label, "(n,n'He3)", 10);
	break;
    case 35:			/* (n,n'd2a) */
	strncpy(label, "(n,n'd2a)", 10);
	break;
    case 36:			/* (n,n't2a) */
	strncpy(label, "(n,n't2a)", 10);
	break;
    case 37:			/* (n,4n) */
	strncpy(label, "(n,4n)", 7);
	break;
    case 41:			/* (n,2np) */
	strncpy(label, "(n,2np)", 8);
	break;
    case 102:			/* (n,g) */
	strncpy(label, "(n,gamma)", 10);
	break;
    case 103:			/* (n,p) */
	strncpy(label, "(n,p)", 6);
	break;
    case 104:			/* (n,d) */
	strncpy(label, "(n,d)", 6);
	break;
    case 105:			/* (n,t) */
	strncpy(label, "(n,t)", 6);
	break;
    case 106:			/* (n,3^He) */
	strncpy(label, "(n,He3)", 7);
	break;
    case 107:			/* (n,a) */
	strncpy(label, "(n,alpha)", 10);
	break;
    case 109:			/* (n,3a) */
	strncpy(label, "(n,3alpha)", 11);
	break;
    case 113:			/* (n,t2a) */
	strncpy(label, "(n,t2alpha)", 12);
	break;
    case 201:			/* (n,Xn) */
	strncpy(label, "(n,Xn)", 7);
	break;
    case 202:			/* (n,Xg) */
	strncpy(label, "(n,Xgamma)", 11);
	break;
    case 203:			/* (n,Xp) */
	strncpy(label, "(n,Xp)", 7);
	break;
    case 204:			/* (n,Xd) */
	strncpy(label, "(n,Xd)", 7);
	break;
    case 206:			/* (n,X3^He) */
	strncpy(label, "(n,XHe3)", 9);
	break;
    case 207:			/* (n,Xa) */
	strncpy(label, "(n,Xalpha)", 11);
	break;
    case 501:			/* Total photon interaction */
	strncpy(label, "Total photon", 13);
	break;
    case 502:			/* Coherent scattering */
	strncpy(label, "Coherent", 9);
	break;
    case 504:			/* Incoherent scattering */
	strncpy(label, "Incoherent", 11);
	break;
    case 522:			/* Photoelectric */
	strncpy(label, "Photoelectric", 14);
	break;
    case 516:			/* Pair production */
	strncpy(label, "Pair production", 16);
	break;
    default:
	*label = '\0';
	break;
    }

    return (ierr);
}

/**********************************************************************/
/**********************************************************************/

static int
ndi_label_to_mt(const char *in_value)
{

/*  Sources:  MT-values: ENDF Format Manual, NNDC, Brookhaven, Appendix B
	      What to do when ENDF LR flags should be used?  as in c=20,21

    Parameters:  
    in_value -- in:pointer to string representing a reaction.

    Returned value:  Aninteger corresponding to an ENDF MT number (reaction type).

***********************************************************************/

    int label_len = 0;
    int mt = 0;
    char *label;

    label_len = (int) strlen(in_value) + 1;
    label = (char *) ndi_malloc(label_len * sizeof(char));
    (void) ndi_niceStrCp(label, in_value, label_len);

    if (strncmp(label, "(n,total)", 10) == 0) {
	mt = 1;
    } else if (strncmp(label, "elastic", 8) == 0) {
	mt = 2;
    } else if (strncmp(label, "inelastic", 10) == 0) {
	mt = 4;
    } else if (strncmp(label, "(n,2n)", 7) == 0) {
	mt = 16;
    } else if (strncmp(label, "(n,3n)", 7) == 0) {
	mt = 17;
    } else if (strncmp(label, "(n,Xf)", 7) == 0) {
	mt = 18;
    } else if (strncmp(label, "(n,n'alpha)", 12) == 0) {
	mt = 22;
    } else if (strncpy(label, "(n,n'3alpha)", 13) == 0) {
	mt = 23;
    } else if (strncpy(label, "(n,n'p)", 8) == 0) {
	mt = 28;
    } else if (strncpy(label, "(n,2n2alpha)", 13) == 0) {
	mt = 30;
    } else if (strncpy(label, "(n,n'd)", 8) == 0) {
	mt = 32;
    } else if (strncpy(label, "(n,n't)", 8) == 0) {
	mt = 33;
    } else if (strncpy(label, "(n,n'He3)", 10) == 0) {
	mt = 34;
    } else if (strncpy(label, "(n,n'd2a)", 10) == 0) {
	mt = 35;
    } else if (strncpy(label, "(n,n't2a)", 10) == 0) {
	mt = 36;
    } else if (strncpy(label, "(n,4n)", 7) == 0) {
	mt = 37;
    } else if (strncpy(label, "(n,2np)", 8) == 0) {
	mt = 41;
    } else if (strncpy(label, "(n,gamma)", 10) == 0) {
	mt = 102;		/* (n,g) */
    } else if (strncpy(label, "(n,p)", 6) == 0) {
	mt = 103;
    } else if (strncpy(label, "(n,d)", 6) == 0) {
	mt = 104;
    } else if (strncpy(label, "(n,t)", 6) == 0) {
	mt = 105;
    } else if (strncpy(label, "(n,He3)", 7) == 0) {
	mt = 106;
    } else if (strncpy(label, "(n,alpha)", 10) == 0) {
	mt = 107;
    } else if (strncpy(label, "(n,3alpha)", 11) == 0) {
	mt = 109;
    } else if (strncpy(label, "(n,t2alpha)", 12) == 0) {
	mt = 113;
    } else if (strncpy(label, "(n,Xn)", 7) == 0) {
	mt = 201;
    } else if (strncpy(label, "(n,Xgamma)", 11) == 0) {
	mt = 202;
    } else if (strncpy(label, "(n,Xp)", 7) == 0) {
	mt = 203;
    } else if (strncpy(label, "(n,Xd)", 7) == 0) {
	mt = 204;
    } else if (strncpy(label, "(n,XHe3)", 9) == 0) {
	mt = 206;
    } else if (strncpy(label, "(n,Xalpha)", 11) == 0) {
	mt = 207;
    } else if (strncpy(label, "Total photon", 13) == 0) {
	mt = 501;
    } else if (strncpy(label, "Coherent", 9) == 0) {
	mt = 502;
    } else if (strncpy(label, "Incoherent", 11) == 0) {
	mt = 504;
    } else if (strncpy(label, "Photoelectric", 14) == 0) {
	mt = 522;
    } else if (strncpy(label, "Pair production", 16) == 0) {
	mt = 516;
    }

    if (label)
	ndi_free((void **) &label);
    return (mt);
}

/**********************************************************************/

static int
ndi_niceStrCp(char *new_str, const char *old_str, int new_len)
   /* Copies old_str to new_str making all characters lowercase 
      and stripping starting and ending whitespace.  The string
      cannot contain any "internal" whitespace.  The first whitespace
      found after characters will cause an end of string to be inserted.

    */
{
    int old_len = (int) strlen(old_str);
    int oldindx;
    int newindx = 0;
    int ierr = 0;

    for (oldindx = 0; oldindx <= old_len; oldindx++) {
	if (isspace(old_str[oldindx])) {
	    if (newindx > 0) {
		new_str[newindx] = '\0';
		break;
	    }
	} else {
	    new_str[newindx] = tolower(old_str[oldindx]);
	    newindx++;
	    if (newindx >= new_len) {
		new_str[newindx - 1] = '\0';
		ierr = 7;
		break;
	    }
	}
    }

    /* Handle 'empty' strings */
    if (newindx == 0)
	new_str[0] = '\0';
    return (ierr);
}


/**********************************************************************/
/**********************************************************************/
