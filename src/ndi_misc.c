
/*
 * File:	ndi_misc.c
 * Description:	Internal function--small misc functions used throughout.
 * Time-stamp:	<1999-05-05 11:02:20 jomc>
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
	return (-1);
    } else if (a1 > a2) {
	return (1);
    } else {
	s1 = (int) strlen(z1) + 1;
	s2 = (int) strlen(z2) + 1;
	if (s1 > s2)
	    zsize = s1;
	else
	    zsize = s2;
	return (strncmp((char *) z1, (char *) z2, zsize));
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
    return (ierr);
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

    return (ierr);
}

/**********************************************************************/
int ndi_get_rprod_int_n(state_struct * state, int vec_num, int *rprod_id)
{

    int ierr = 0;
    int curr_index;
    depl_target_struct *curr_target;

    if (state) {
	curr_target = state->depl_target;
    } else {
	return (-2);
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

    return (ierr);

}

/**********************************************************************/
int ndi_get_rprod_vec_mg(state_struct * state, int *rprod_vec,
			 int vec_size)
{

    int ierr = 0;
    int curr_index;
    int i;
    int *rprod_p = rprod_vec;
    depl_target_struct *curr_target;

    if (state) {
	curr_target = state->depl_target;
    } else {
	return (-2);
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

    return (ierr);

}

/**********************************************************************/
int ndi_get_rprod_id_n(state_struct * state, int vec_num, char *rprod_id,
		       int str_size)
{

    int ierr = 0;
    int curr_index;
    depl_target_struct *curr_target;

    if (state) {
	curr_target = state->depl_target;
    } else {
	return (-2);
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

    return (ierr);

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
	return (-6);
    } else {
	/* Error: data not available for this isotope */
	return (9);
    }

}

/**********************************************************************/
int ndi_read_float_pair(FILE * stream, long int file_pos, int num_prs,
			double *flt_vecx, double *flt_vecy,
			int return_size)

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
    int ierr = 0;		/* Error flag: 0=no error,pos=warning, neg=error */
    char in_line[MAX_IN_STR_LEN];	/* Current input line */
    char *token;		/* Current data item */
    char seps[] = " ,\t\n";	/* Data delimiters in library file */
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
	    return (ierr);
	}
    }

    return (ierr);

}

/*********************************************************************/
int ndi_translate_fiss_prod(state_struct * state, int *zaid_list,
			    int num_zaids)
     /* Takes an array of zaids (integers), and will translate 99999
        (indicating a fission product) to the default fission product
        ZAID.
      */
{

    int *zaid_p = zaid_list;
    int i;
    int ierr = 0;

    if (!state) {
	return (-2);
    }

    for (i = 0; i < num_zaids; i++) {
	if (*zaid_p == 99999) {
	    *zaid_p = atoi(state->def_fiss_prod_id);
	}
	zaid_p++;
    }
    return (ierr);
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
/*       s = (int) ldiv(tmpz, 1000 ).quot;  */
	} else {
	    *z = tmpz;
	}
    } else {
	/* ??? Error value */
	ierr = -9;
    }

    return (ierr);
}


/**********************************************************************/
/**********************************************************************/

int double_compare(const double a, const double b)
     /*  Return =  0, if a == b
        =  1, if a > b
        = -1, if a < b
      */
{

    double absa = fabs(a);
    double absb = fabs(b);
    double max;

    if (absa > absb) {
	max = absa;
    } else {
	max = absb;
    }

    if (fabs(a - b) <= DBL_EPSILON * max) {
	return (0);
    } else if (a < b) {
	return (-1);
    } else {
	return (1);
    }

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
	    return (0);
	}
	curr_char++;
	str_size--;
    }

    return (-1);
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
int ndi_get_nth_inc_part_type(state_struct * state, int vec_num,
			      char *ret_str, const int str_size)
{

    int ierr = 0;
    char tmp_str[MAX_IN_STR_LEN];

    if (!state) {
	return (-2);
    }

    if (str_size <= 0) {
	return (7);
    }

    if (!state->chain_stat.set_flag) {
	ierr = ndi_set_default_depl(state);
    }

    if (state->chain_stat.num_inc_parts <= 0) {
	strcpy(tmp_str, "1");	/* Default is neutron */
	ierr = 9;
    } else {
	if (state->chain_stat.num_inc_parts > vec_num) {
	    sprintf(tmp_str, "%d",
		    state->chain_stat.inc_part_types[vec_num]);
	} else {
	    ierr = ndi_check_array_err(state->chain_stat.num_inc_parts);
	}
    }

    if ((strlen(tmp_str) + 1) <= str_size) {
	strcpy(ret_str, tmp_str);
    } else {
	strncpy(ret_str, tmp_str, str_size * sizeof(char));
	ret_str[str_size - 1] = '\0';
	if (ierr == 0) {
	    ierr = 7;
	}
    }
    return (ierr);
}

/**********************************************************************/
int ndi_use_sgndf_values(state_struct * state, int num_grps)
{

    /* Test to see if the current energy group structure is one for
       which we have hard-wired average energy values.

       num_grps: either the number of groups the user expects

       Currently this is 16_llnl and 30_lanl.

       Return value:  0=this not an SGNDF energy structure,
       -1=this is an SGNDF energy structure
     */

    int i;
    int ierr;
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
	ierr =
	    ndi_c_get_float64_vec_h(state, NDI_E_BOUNDS, egrp_bounds, 31);
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
    }

    return (ret_val);
}

/**********************************************************************/
