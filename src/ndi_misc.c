
/*
 * $Id$
 * Description:        Internal function--small misc functions used throughout.
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>

#include "ndi.h"
#include "ndi_internal.h"


/**********************************************************************/
/**********************************************************************/
int ndi_comp_zaids(const void *z1, const void *z2)
{

    long int a1, a2;
    int s1, s2, zsize;

    a1 = atoi(z1);
    a2 = atoi(z2);

    if (a1 < a2) {
        return -1;
    } else if (a1 > a2) {
        return 1;
    } else {
        s1 = (int) strlen(z1) + 1;
        s2 = (int) strlen(z2) + 1;
        if (s1 > s2)
            zsize = s1;
        else
            zsize = s2;
        return strncmp((char *) z1, (char *) z2, zsize);
    }
}

/**********************************************************************/
/**********************************************************************/

int ndi_init_int_vec(int *vec, int num_elements)
{

    int i;
    int ierr = 0;
    int *curr_vec = vec;

    for (i = 0; i < num_elements; i++) {
        *curr_vec = 0;
        curr_vec++;
    }
    return ierr;
}

/*********************************************************************/
/**********************************************************************/

int ndi_free_ptr_array(int array_size, char **array)
{

    int i;
    int ierr = 0;

    for (i = 0; i < array_size; i++) {
        if (array[i])
            ndi_free((void **) &array[i]);
    }

    return ierr;
}

/**********************************************************************/
int ndi_get_rprod_int_n(state_struct *state, int vec_num, int *rprod_id)
{

    int ierr = 0;
    int curr_index;
    depl_target_struct *curr_target;

    if (state) {
        curr_target = state->depl_target;
    } else {
        return -2;
    }

    *rprod_id = 0;
    ierr = ndi_set_curr_target(state, &curr_target, &curr_index);
    if ((ierr >= 0) && (curr_target != NULL)) {
        /* check vec_num is less than num of rprods */
        if (vec_num < curr_target->num_rprods) {

            if (curr_target->rprod[vec_num].izaid == 99999) {
                /* Translate fission product identifier 99999 to default f.p. ZAID */
                *rprod_id = atoi(state->def_fiss_prod_id);
            } else {
                *rprod_id = curr_target->rprod[vec_num].izaid;
            }
        } else {
            /* Invalid vec_num */
            ierr = -5;
        }
    } else {
        rprod_id = 0;
    }

    return ierr;

}

/**********************************************************************/
int ndi_get_rprod_vec_mg(state_struct *state, int *rprod_vec, int vec_size)
{

    int ierr = 0;
    int curr_index;
    int i;
    int *rprod_p = rprod_vec;
    depl_target_struct *curr_target;

    if (state) {
        curr_target = state->depl_target;
    } else {
        return -2;
    }

    /* Initialize vector with zeroes */
    for (i = 0; i < vec_size; i++) {
        rprod_vec[i] = 0;
    }

    ierr = ndi_set_curr_target(state, &curr_target, &curr_index);
    if ((ierr >= 0) && (curr_target != NULL)) {
        for (i = 0; i < curr_target->num_rprods; i++) {
            if (i >= vec_size) {
                ierr = 7;
                break;
            } else {
                if (curr_target->rprod[i].izaid == 99999) {
                    /* Translate fission product identifier 99999 to default f.p. ZAID */
                    *rprod_p = atoi(state->def_fiss_prod_id);
                } else {
                    *rprod_p = curr_target->rprod[i].izaid;
                }
            }
            rprod_p++;
        }
    } else {
        ierr = -30;
    }

    return ierr;

}

/**********************************************************************/
int
ndi_get_rprod_id_n(state_struct *state, int vec_num, char *rprod_id,
                   int str_size)
{

    int ierr = 0;
    int curr_index;
    depl_target_struct *curr_target;

    if (state) {
        curr_target = state->depl_target;
    } else {
        return -2;
    }

    ierr = ndi_set_curr_target(state, &curr_target, &curr_index);
    if ((ierr >= 0) && (curr_target != NULL)) {
        /* check vec_num is less than num of rprods */
        if (vec_num < curr_target->num_rprods) {

            if (curr_target->rprod[vec_num].izaid == 99999) {
                /* Translate fission product identifier 99999 to default f.p. ZAID */
                strncpy(rprod_id, state->def_fiss_prod_id, str_size);
            } else {
                if (sprintf
                    (rprod_id, "%d",
                     curr_target->rprod[vec_num].izaid) > str_size) {
                    /* Error */
                }
            }

        } else {
            /* Invalid vec_num */
            ierr = -5;
        }
    } else {
        strcpy(rprod_id, "");
    }

    return ierr;

}

/**********************************************************************/

int ndi_check_array_err(int num)

   /* Determine if there was no data contained in the library or
      if the subscript was just out of range.  Works only for values
      (like .num_objs) that are initialized to a negative number and
      are overwritten when library data is read.
    */
{
    if (num > 0) {
        /*  Subscript out of range */
        return -6;
    } else {
        /* Error: data not available for this isotope */
        return 9;
    }

}

/**********************************************************************/
int
ndi_read_float_pair(FILE *stream, long int file_pos, int num_prs,
                    double *flt_vecx, double *flt_vecy, int return_size)

/*
  Purpose:  Read from the current stream a pair of floating-point
  numbers.
  
  Parameters: 
  stream -- in: file to read from (open library file).
  num_prs -- in: number of pairs to read in.
  flt_vecx-- out: floating point array containing x-values read from stream.
  flt_vecy-- out: floating point array containing y-values read from stream.
  return_sizes -- in: the size of arrays flt_vecx and flt_vecy. 

  Return values:  
  Returns negative value for error condition, positive value for 
  warning, and a '0' if everything's ok.   values read in.

***********************************************************************/
{

    int pair_count = 0;
    int ierr = 0;                /* Error flag: 0=no error,pos=warning, neg=error */
    char in_line[MAX_IN_STR_LEN];        /* Current input line */
    char *token;                /* Current data item */
    char seps[] = " ,\t\n";        /* Data delimiters in library file */
    double *cur_fltx_ptr = flt_vecx;
    double *cur_flty_ptr = flt_vecy;

    double ysum = 0.0;


    /* Go to start of data, read line and count floats */
    /*  Check for data not available */
    fseek(stream, file_pos, SEEK_SET);
    while (pair_count < num_prs) {
        if (fgets(in_line, sizeof(in_line), stream) != NULL) {
            token = strtok(in_line, seps);
            while (token != NULL) {
                if (pair_count < num_prs) {
                    *cur_fltx_ptr = (double) strtod(token, NULL);
                    cur_fltx_ptr++;
                    token = strtok(NULL, seps);
                    *cur_flty_ptr = (double) strtod(token, NULL);

                    ysum += *cur_flty_ptr;

                    cur_flty_ptr++;
                    token = strtok(NULL, seps);
                    pair_count++;
                } else {
                    /* More data than expected */
                    ierr = 6;
                    break;
                }
            }
        } else {
            /* Error reading file */
            ierr = -11;
            return ierr;
        }
    }

    return ierr;

}

/*********************************************************************/
int
ndi_translate_fiss_prod(state_struct *state, int *zaid_list, int num_zaids)
     /* Takes an array of zaids (integers), and will translate 99999
        (indicating a fission product) to the default fission product
        ZAID.
      */
{

    int *zaid_p = zaid_list;
    int i;
    int ierr = 0;

    if (!state) {
        return -2;
    }

    for (i = 0; i < num_zaids; i++) {
        if (*zaid_p == 99999) {
            *zaid_p = atoi(state->def_fiss_prod_id);
        }
        zaid_p++;
    }
    return ierr;
}

/*********************************************************************/

int ndi_parse_mg_zaid(char *zaid, int *z, int *a)
{

    /* Take input of multigroup ZAID, e.g. 92235.600nm, and return
       Z and A (atomic number and mass)

     */

    int ierr = 0;
    int izaid;
    int tmpz;
    /* int s = 0; *//* State */

    /* Find integer ZAID part */
    /* Convert string to integer */
    /* Extract Z and A */

    izaid = (int) strtol(zaid, (char **) NULL, 10);

    if (izaid > 1000) {
        *a = (int) ldiv(izaid, 1000).rem;
        tmpz = (int) ldiv(izaid, 1000).quot;
        if (tmpz > 999) {
            /* Excited state */
            *z = (int) ldiv(tmpz, 1000).rem;
        } else {
            *z = tmpz;
        }
    } else if (izaid == 1) {
        *a = (int) 1;
        *z = (int) 0;
    } else {
        /* ??? Error value */
        ierr = -9;
    }

    return ierr;
}


/**********************************************************************/
/**********************************************************************/

int double_compare(const double a, const double b)
     /*  Return =  0, if a ~ b
                =  1, if a > b
                = -1, if a < b
      */
{

    if (ndi_approx_equal(a, b, DBL_EPSILON, 0.0)) {
        return 0;
    } else if (a < b) {
        return -1;
    } else {
        return 1;
    }

}

/**********************************************************************/
/* Approximate equal

   See: Knuth V2 pg. 234
   |u-v| <= eps*|u| or |u-v| <= eps*|v| ==> u~v
*/
int ndi_approx_equal(double u, double v, double relerr, double abserr)
{
    double absu = fabs(u);
    double absv = fabs(v);

    return fabs(u-v) <= (relerr * ((absu > absv) ? absu : absv) + abserr);
}
        
/**********************************************************************/
int ndi_empty_string(char *in_str)

     /* Empty string returns true (-1) if the input character array
        contains only whitespace before the newline.  It will return 
        false (0) if a non-whitespace character exists before the newline.
      */
{

    char *curr_char = in_str;
    int str_size = strlen(in_str);

    while ((*curr_char != '\0') && (str_size > 0)) {
        if (!isspace(*curr_char)) {
            return 0;
        }
        curr_char++;
        str_size--;
    }

    return -1;
}

/**********************************************************************/
void check_fissionable_err(int *curr_err, int fission_flag)
{

    /* Purpose: This subroutine should be used when returning fission
       data.  If fission data has been requested for a non-fissioning
       isotope, then the user should not be returned an error indicating
       that no fission data was available.

       If fission_flag == 0, then the isotope is considered to be
       a non-fissionable isotope.  If the current error indicates that
       the data was not found on the library (9 or -9), then the error
       will be changed to 0.
     */

    if (fission_flag == 0) {
        if ((*curr_err == 9) || (*curr_err == -9)) {
            *curr_err = 0;
        }
    }

    return;
}

/**********************************************************************/
int
ndi_get_nth_inc_part_type(state_struct *state, int vec_num,
                          char *ret_str, const int str_size)
{

    int ierr = 0;
    char tmp_str[MAX_IN_STR_LEN];

    if (!state) {
        return -2;
    }

    if (str_size <= 0) {
        return 7;
    }

    if (!state->chain_stat.set_flag) {
        ierr = ndi_set_default_depl(state);
    }

    if (state->chain_stat.num_inc_parts <= 0) {
        strcpy(tmp_str, "1");        /* Default is neutron */
        ierr = 9;
    } else {
        if (state->chain_stat.num_inc_parts > vec_num) {
            sprintf(tmp_str, "%d",
                    state->chain_stat.inc_part_types[vec_num]);
        } else {
            ierr = ndi_check_array_err(state->chain_stat.num_inc_parts);
        }
    }

    if ((strlen(tmp_str) + 1) <= (unsigned long) str_size) {
        strcpy(ret_str, tmp_str);
    } else {
        strncpy(ret_str, tmp_str, str_size * sizeof(char));
        ret_str[str_size - 1] = '\0';
        if (ierr == 0) {
            ierr = 7;
        }
    }
    return ierr;
}

/**********************************************************************/
int ndi_use_sgndf_values(state_struct *state, int num_grps)
{

    /* Test to see if the current energy group structure is one for
       which we have hard-wired average energy values.

       num_grps: either the number of groups the user expects

       Currently this is 16_llnl and 30_lanl.

       Return value:  0=this not an SGNDF energy structure,
       -1=this is an SGNDF energy structure
     */

    int i;
    int ret_val = -1;
    ndi_float64 egrp_bounds[31];
    ndi_float64 *bound_p = NULL;
    ndi_float64 _30_group[] =
        { 17.0, 15.0, 13.5, 12.0, 10.0, 7.79, 6.07, 3.68,
        2.865, 2.232, 1.738, 1.353, 0.823, 0.5, 0.303,
        0.184, 6.76e-2, 2.48e-2, 9.12e-3, 3.35e-3,
        1.235e-3, 4.54e-4, 1.67e-4, 6.14e-5, 2.26E-5,
        8.32E-6, 3.06E-6, 1.13E-6, 4.14E-7, 1.52E-7,
        1.39E-10
    };
    ndi_float64 _16_LLNL[] = { 17.0, 13.5, 12.0, 10.0, 7.79, 6.07, 3.68,
        2.865, 1.353, 0.5, 0.184, 6.76e-2, 2.48e-2,
        9.12e-3, 3.35e-3, 4.54e-4, 1.67e-4
    };


    if ((num_grps == 16) || (num_grps == 30)) {
        (void) ndi_c_get_float64_vec_h(state, NDI_E_BOUNDS, egrp_bounds,
                                       31);
        if (num_grps == 16) {
            bound_p = _16_LLNL;
        } else if (num_grps == 30) {
            bound_p = _30_group;
        }
        for (i = 0; i <= num_grps; i++) {
            if (fabs(*bound_p - egrp_bounds[i]) > DBL_EPSILON) {
                ret_val = 0;
                break;
            }
            bound_p++;
        }
    } else {
        ret_val = 0;
    }

    return ret_val;
}

/**********************************************************************/
int ndi_special_sza(int sza, char *sym_str, int str_size)
{

    char tmp_str[50];
    int ret_val = 0;                /* False: not special character */

    switch (sza) {

    case -999:
        strncpy(tmp_str, "Generic Destruction", 20);
        ret_val = 1;
        break;

    case 99999:
        strncpy(tmp_str, "Fission Product", 16);
        ret_val = 1;
        break;

    case 45117:
        strncpy(tmp_str, "Fission Product", 16);
        ret_val = 1;
        break;

    case 50998:
        strncpy(tmp_str, "Fission Product", 16);
        ret_val = 1;
        break;

    case 50999:
        strncpy(tmp_str, "Fission Product", 16);
        ret_val = 1;
        break;
    }

    if ((ret_val) && (str_size > 0) && (sym_str)) {
        strncpy(sym_str, tmp_str, str_size);
    }

    return ret_val;
}

/**********************************************************************/

int ndi_niceStrCp(char *new_str, const char *old_str, int new_len)
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
    return ierr;
}

/**********************************************************************/
/*
 * Description: Return error or warning string corresponding to error number 
 *
 */

char *ndi_error_string(int n)
{
    static char *warning[] = { // All warnings are positive numbers
        "Call successfully executed.", // 0
        "NDI already initialized.", // 1
        "NDI not initialized before NDI2_close_gendir.", // 2
        "Cannot close NDI gendir file.", // 3
        "Attempt to close NULL stream.", // 4
        "Unexpected zero quantity.", // 5
        "Unexpected extra data.", // 6
        "Bounds test failed: array terminated.", // 7
        "Invalid warning number.", // 8
        "Data not available in current library for current zaid.", // 9
        "Data is constructed", // 10
        "Cannot open gendir specified by user path.", // 11
        "Cannot open gendir specified by NDI_GENDIR_PATH.", // 12
        "Invalid warning number.", // 13
        "Invalid warning number.", // 14
        "Unknown keyword in library.", // 15
        "Invalid interpolation type.", // 16
        "Quantity outside maximum and minimum library values.", // 17 (was +30)
        "New group structure is default but old group structure is not." // 18 (was 77)
        // Warning 54 was changed to -34
    };

    static char *error[] = { // All errors are negative numbers
        "Invalid warning/error number.", // 0
        "Cannot open file.", // 1
        "NDI not initialized before NDI library call.", // 2
        "Cannot close file.", // 3
        "Invalid input key to NDI user function.", // 4
        "Invalid key argument to NDI user function.", // 5
        "Index parameter out of range.", // 6
        "Isotope not set.", // 7
        "Isotope not found in gendir.", // 8
        "Data not available in current library for current zaid.", // 9
        "32-bit function is not implemented on current platform.", // 10
        "Cannot read file.", // 11
        "Unexpected library data.", // 12
        "Invalid input parameter.", // 13
        "Cannot allocate memory.", // 14
        "Invalid data in file.", // 15
        "Library type not set.", // 16
        "Isotope not found in library.", // 17
        "Library error: incorrect data order.", // 18
        "Cannot set file pointer.", // 19
        "NDI_LIB_TYPE not set.", // 20
        "Collapse requested without set energy structure.", // 21
        "Bad zaid input.", // 22
        "Incompatible collapse group structure.", // 23
        "Bad reaction MT or C identifier.", // 24
        "Null vector input by user.", // 25
        "Invalid error number.", // 26
        "Bad handle data.", // 27
        "Invalid error number.", // 28
        "Invalid error number.", // 29
        "NDI_DEPLETION not set.", // 30
        "Current zaid not in target set.", // 31
        "Reaction product not in depletion set.", // 32
        "Last index greater than the number of particles.", // 33
        "Library not found in gendir.", // 34 (was +54)
        "Invalid particle handle.", // 35 (was -99)
        "Bad particle." // 36 (was -999)
    };

    int num_warnings = sizeof(warning) / sizeof(warning[0]);
    int num_errors = sizeof(error) / sizeof(error[0]);

    if (-num_errors < n && n < 0) {        /* error */
        return error[-n];
    } else if (0 <= n && n < num_warnings) {
        return warning[n];
    } else {
        return error[0];
    }
}

/**********************************************************************/
/* Decode (possibly extended) mt into mt and state, return mt.
   See ENDF-102, June 2009, Appendix B
*/ 
static int ndi_decode_mt(int mt, int *state)
{
    int a, b;
    int m, s;

    /* Assume s=0 for MT <= 10 and s<10 for MT > 10 
       to decode old and new extended MT formats */ 

    a = mt/1000;
    b = mt%1000;
    if (a == 0) {               /* decode as MMM */
        /* Decode partial states and overwrite any state from above */
        if (b == 2) {
            s = 0;                  /* elastic implies ground */
            m = b;
        } else if (50 <= b && b < 91) {
            s = b-50;               /* inelastic to discrete */
            m = 4;
        } else if (b == 91) {
            s = 98;                 /* inelastic to continuum */
            m = 4;
        } else if (600 <= b && b < 649) {
            s = b-600;              /* proton production to discrete */
            m = 103;
        } else if (b == 649) {
            s = 98;                 /* proton production to continuum */
            m = 103;
        } else if (650 <= b && b < 699) {
            s = b - 650;            /* deuteron production to discrete */
            m = 104;
        } else if (b == 699) {
            s = 98;                 /* deuteron production to continuum */
            m = 104;
        } else if (700 <= b && b < 749) {
            s = b - 700;            /* triton production to discrete */
            m = 105;
        } else if (b == 749) {
            s = 98;                 /* triton production to continuum */
            m = 105;
        } else if (750 <= b && b < 799) {
            s = b - 750;            /* He3 production to discrete */
            m = 106;
        } else if (b == 799) {
            s = 98;                 /* He3 production to continuum */
            m = 106;
        } else if (800 <= b && b < 849) {
            s = b - 800;            /* alpha production to discrete */
            m = 107;
        } else if (b == 849) {
            s = 98;                 /* alpha production to continuum */
            m = 107;
        } else if (875 <= b && b < 891) {
            s = b - 875;             /* neutron production to discrete */
            m = 16;
        } else if (b == 891) {
            s = 98;                 /* neutron production to continuum */
            m = 16;
        } else {
            s = 99;                 /* summation */
            m = b;
        }
    } else if (a > b) {         /* decode as MMMSSS (new) */
        s = b;
        m = a;
    } else {                    /* decode as SMMM (old) */
        s = a;
        m = b;
    }

    *state = s;
    return m;
}
    
/**********************************************************************/
/* Return label corresponding to 0<= mt <=999
   See ENDF-102, June 2009, Appendix B
*/
char *ndi_mt_label(int mt)
{
    /* pieces of label arrays */
    static char *MT0[] = {
        "",                     /* 0 */
        "(n,total)",            /* 1 */
        "(z,z)",                /* 2 */
        "(z,nonelastic)",       /* 3 */
        "(z,n)",                /* 4 */
        "(z,anything)",         /* 5 */
        "",                     /* 6 */
        "",                     /* 7 */
        "",                     /* 8 */
        "",                     /* 9 */
        "(z,continuum)",        /* 10 */
        "(z,2nd)",              /* 11 */
        "",                     /* 12 */
        "",                     /* 13 */
        "",                     /* 14 */
        "",                     /* 15 */
        "(z,2n)",               /* 16 */
        "(z,3n)",               /* 17 */
        "(z,fission)",          /* 18 */
        "(z,f)",                /* 19 */
        "(z,nf)",               /* 20 */
        "(z,2nf)",              /* 21 */
        "(z,na)",               /* 22 */
        "(z,n3a)",              /* 23 */
        "(z,2na)",              /* 24 */
        "(z,3na)",              /* 25 */
        "",                     /* 26 */
        "(z,abs)",              /* 27 */
        "(z,np)",               /* 28 */
        "(z,n2a)",              /* 29 */
        "(z,2n2a)",             /* 30 */
        "",                     /* 31 */
        "(z,nd)",               /* 32 */
        "(z,nt)",               /* 33 */
        "(z,nHe3)",             /* 34 used He3 instead of $^3$He */
        "(z,nd2a)",             /* 35 */
        "(z,nt2a)",             /* 36 */
        "(z,4n)",               /* 37 */
        "(z,3nf)",              /* 38 */
        "",                     /* 39 */
        "",                     /* 40 */
        "(z,2np)",              /* 41 */
        "(z,3np)",              /* 42 */
        "",                     /* 43 */
        "(z,n2p)",              /* 44 */
        "(z,npa)"               /* 45 */
    };

    static char *MT1[] = {
        "(z,disap)",            /* 101 */
        "(z,g)",                /* 102 used g instead of $\gamma$ */
        "(z,p)",                /* 103 */
        "(z,d)",                /* 104 */
        "(z,t)",                /* 105 */
        "(z,He3)",              /* 106 used He3 instead of $^3$He */
        "(z,a)",                /* 107 */
        "(z,2a)",               /* 108 */
        "(z,3a)",               /* 109 */
        "",                     /* 110 */
        "(z,2p)",               /* 111 */
        "(z,pa)",               /* 112 */
        "(z,t2a)",              /* 113 */
        "(z,d2a)",              /* 114 */
        "(z,pd)",               /* 115 */
        "(z,pt)",               /* 116 */
        "(z,da)"                /* 117 */
    };

    static char label[MAX_IN_STR_LEN];
    char **MT[] = {MT0, MT1};     /* piecewise mt array */
    int mt_start[] = {0, 101};  /* start indicies in array */
    int mt_end[] = {sizeof(MT0)/sizeof(MT0[0]), 
                    sizeof(MT1)/sizeof(MT1[0])}; /* length of pieces */

    int m, s;
    int i;


    m = ndi_decode_mt(mt, &s);            /* Decode mt and state */
    *label = '\0';
    for (i = 0; (unsigned long) i < sizeof(mt_start)/sizeof(mt_start[0]); i++)
        if (mt_start[i] <= m && m < mt_start[i]+mt_end[i])
            strcpy(label, MT[i][m-mt_start[i]]); /* mt found in piece */

    if (*label != '\0' && s != 99) /* add state information */
        sprintf(label+strlen(label), "m%d", s);

    return label;                          /* mt not found */
}

/**********************************************************************/

/* Does isotope table contain reaction mt? Reqt. 19*/
int ndi_has_mt(state_struct *state, int mt)
{
    int *reac;

    if (state && (state->iso_stat.num_reac > 0) && (state->iso_stat.reac_mt)) {
        reac = state->iso_stat.reac_mt;
        for (int i = 0; i < state->iso_stat.num_reac; i++) {
            if (mt == reac[i]) {
                return 1;
            }
        }
    }
    return 0;
}

/**********************************************************************/
/* Reqt. 19: return incident particle name from zaid 
 *
 * See "General NDI Library Conventions", X-5:MCW-02-82, Table 2 
*/

char ndi_get_incident_name(int sza)
{
    char c;

    /* get appropriate incident particle name */
    switch (sza) {
    case 0:                     /* photon */
        c = 'g';
        break;
    case 1:                     /* neutron */
        c = 'n';
        break;
    case 1001:                  /* proton */
        c = 'p';
        break;
    case 1002:                  /* deuteron */
        c = 'd';
        break;
    case 1003:                  /* triton */
        c = 't';
        break;
    case 2004:                  /* alpha */
        c = 'a';
        break;
    default:                    /* others */
        c = 'z';
        break;
    }
    return c;
}

/**********************************************************************/

/* 10 point Gauss-Legendre Quadrature
   See: M. Abramowitz and I. A. Stegun,
   Handbook of Mathematical Functions
   Dover Publications (1972), p.916 */
double ndi_gl10(double (*f)(double), double a, double b)
{
    static double z[] = {
        0.148874338981631, 
        0.433395394129247, 
        0.679409568299024,
        0.865063366688985,
        0.973906528517172
    };
    static double w[] = {
        0.295524224714753,
        0.269266719309996,
        0.219086362515982,
        0.149451349150581,
        0.066671344108688
    };

    int i;
    double xmid, xrange, dx, sum;

    xmid = (b+a)/2.0;
    xrange = (b-a)/2.0;
    sum = 0.0;

    for (i = 0; (unsigned long) i < sizeof(z)/sizeof(z[0]); i++) {
        dx = xrange * z[i];
        sum += ((*f)(xmid+dx) + (*f)(xmid-dx)) * w[i];
    }
    return sum *= xrange;
}

/* The TD weight function at room temperature.

   See: M. C. White
   The TD Weight Function Revisited 
   X-5:MCW-05-24(u)
   March 2005; 
   NJOY-99: groupr.f: genwtf: data w9 (iwt = 9)
   make-input.pl: print_gendf: @td_ergs; @td_wgts*/
double ndi_td(double x)
{

    static double E[] = {              /* Energy grid (eV) */
        1.000000e-05, 1.390000e-04, 5.000000e-04, 1.000000e-03, 5.000000e-03,
        1.000000e-02, 2.500000e-02, 4.000000e-02, 5.000000e-02, 1.000000e-01,
        1.400000e-01, 1.500000e-01, 4.140000e-01, 1.130000e+00, 3.060000e+00,
        8.320000e+00, 2.260000e+01, 6.140000e+01, 1.670000e+02, 4.540000e+02,
        1.235000e+03, 3.350000e+03, 9.120000e+03, 2.480000e+04, 6.760000e+04,
        1.840000e+05, 3.030000e+05, 5.000000e+05, 8.230000e+05, 1.353000e+06,
        1.738000e+06, 2.232000e+06, 2.865000e+06, 3.680000e+06, 6.070000e+06,
        7.790000e+06, 1.000000e+07, 1.200000e+07, 1.300000e+07, 1.350000e+07,
        1.400000e+07, 1.410000e+07, 1.420000e+07, 1.430000e+07, 1.460000e+07,
        1.500000e+07, 1.600000e+07, 1.700000e+07, 2.000000e+07};

    static double w[] = {              /* weight @ E[i] */
        2.239100e+05, 3.019000e+06, 1.070000e+07, 2.098000e+07, 8.939000e+07,
        1.463800e+08, 2.008000e+08, 1.763500e+08, 1.478000e+08, 4.000000e+07,
        1.130000e+07, 7.600000e+06, 2.790000e+06, 1.020000e+06, 3.770000e+05,
        1.390000e+05, 5.110000e+04, 1.880000e+04, 6.910000e+03, 2.540000e+03,
        9.350000e+02, 3.450000e+02, 1.266000e+02, 4.650000e+01, 1.710000e+01,
        6.270000e+00, 3.880000e+00, 3.600000e+00, 2.870000e+00, 1.750000e+00,
        1.130000e+00, 7.300000e-01, 4.000000e-01, 2.050000e-01, 3.900000e-02,
        1.630000e-02, 6.500000e-03, 7.600000e-03, 1.230000e-02, 2.640000e-02,
        1.140000e-01, 1.140000e-01, 1.010000e-01, 6.500000e-02, 1.490000e-02,
        4.000000e-03, 1.540000e-03, 8.500000e-04, 1.727900e-04};

    static double lnw[] = {            /* log (w[i+1]/w[i])/log(E[i+1]/E[i]) */
         9.884296e-01, 9.884262e-01, 9.714039e-01, 9.005872e-01, 7.115331e-01,
         3.449816e-01,-2.762505e-01,-7.914665e-01,-1.885574e+00,-3.756853e+00,
        -5.749210e+00,-9.870729e-01,-1.002123e+00,-9.991121e-01,-9.975245e-01,
        -1.001403e+00,-1.000468e+00,-1.000303e+00,-1.000702e+00,-9.986448e-01,
        -9.991109e-01,-1.001001e+00,-1.001206e+00,-9.976161e-01,-1.001972e+00,
        -9.621974e-01,-1.495408e-01,-4.547460e-01,-9.951175e-01,-1.746723e+00,
        -1.746575e+00,-2.409498e+00,-2.670140e+00,-3.315940e+00,-3.496828e+00,
        -3.681218e+00, 8.575293e-01, 6.014927e+00, 2.023736e+01, 4.022351e+01,
         0.000000e+00,-1.713246e+01,-6.280423e+01,-7.094821e+01,-4.865451e+01,
        -1.478980e+01,-9.802970e+00,-9.802916e+00,-9.802916e+00};

    int l, u, m;

    x = x * 1.0e6;              /* convert MeV to eV */

    /* find x in E grid using binary search */

    l = 0;
    u = sizeof(E)/sizeof(E[0]) - 1;

    if (x < E[l]) {             /* off lower end */
        u = l;
    } else if (E[u] < x) {      /* off upper end */
        l = u;
    }
    while ((u-l) > 1) {         /* find x in table */
        m = (l+u)/2;
        if (E[m] < x) {
            l = m;
        } else if (E[m] == x) {
            l = m;
            u = m+1;
        } else if (E[m] > x) {
            u = m;
        }
    }

    /* return log-log interpolation */
    return w[l] * pow(x/E[l], lnw[l]);
}
/*********************************************************************/
/* The TD Weight Function at specified temperature.

   See: NJOY 99:groupr.f:getwtf: data wt10a, wt10b, wt10c (iwt = 10)
        make-input.pl:print_gendf
*/
double ndi_ttd(double E, double T)
{
    const double wt10a = 0.15;
    const double wt10b = 300.0;
    const double wt10c = 1.15e6;
    const double bk = 8.617385e-5;    /* Boltzmann's Constant (eV/K) */

    double x;
    double ea;
    double eb;

    x = E * 1.0e6;              /* Convert MeV to eV */
    ea = T * 1.0e6;             /* Convert MeV to eV */
    eb = wt10a * (ea/bk)/wt10b; /* Energy @ thermal upper bound */
    if (x < eb)
        return wt10c * (x/(eb*eb)) * exp(-(x-eb)/ea);
    else
        return ndi_td(E);
}
/*********************************************************************/
