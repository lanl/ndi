
/*
 * File:	ndi_fortran.c
 * Description:	Source for libndi.a
 * Time-stamp:	<1999-11-12 17:02:52 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fortran.h>

#include "fmangle.h"

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"


/**********************************************************************/

static void ndi_f2c_string(_fcd fstring, char **cstring, int cstr_size);
static void ndi_c2f_string(char *cstring, _fcd fstring);
void F_NDID_GET_SIZE(int *in_key, _fcd in_value, int *arr_size, int *ierr);
void F_NDID_GET_STRING_VEC(int *in_key, _fcd in_value, _fcd str_arr,
			   int *arr_size, int *str_size, int *ierr);
void F_NDID_ZAID_FLOAT64_VEC(int *in_key, _fcd in_value, _fcd str_arr,
			     int *arr_size, int *str_size,
			     ndi_float64 * float_arr, int *fl_arr_size,
			     int *ierr);
void F_NDID_ZAID_FLOAT32_VEC(int *in_key, _fcd in_value, _fcd str_arr,
			     int *arr_size, int *str_size,
			     ndi_float32 * float_arr, int *fl_arr_size,
			     int *ierr);
void F_NDI_INIT(int *ierr);
void F_NDI_CLOSE(int *ierr);
void F_NDI_SET_OPTION(int *in_key, _fcd in_value, int *ierr);
void F_NDI_CHECK_OPTION(int *in_key, _fcd out_value, int *ierr);
void F_NDI_SET_ZAID(_fcd zaid, int *ierr);
void F_NDI_SET_ISOTOPE(_fcd zaid, int *ierr);
void F_NDI_SET_REACTION(_fcd zaid, int *ierr);
void F_NDI_GET_SIZE(int *in_key, int *return_val, int *ierr);
void F_NDI_GET_SIZE_N(int *in_key, int *vec_num, int *return_val,
		      int *ierr);
void F_NDI_GET_INT_VAL(int *in_key, int *return_val, int *ierr);
void F_NDI_GET_INT_VAL_N(int *in_key, int *vec_num, int *return_val,
			 int *ierr);
void F_NDI_GET_INT_VEC(int *in_key, int *return_val, int *vec_size,
		       int *ierr);
void F_NDI_GET_INT_VEC_N(int *in_key, int *vec_num, int *return_val,
			 int *vec_size, int *ierr);
void F_NDI_GET_FLOAT32_VAL(int *in_key, ndi_float32 * return_val,
			   int *ierr);
void F_NDI_GET_FLOAT64_VAL(int *in_key, ndi_float64 * return_val,
			   int *ierr);
void F_NDI_GET_FLOAT32_VAL_N(int *in_key, int *vec_num,
			     ndi_float32 * return_val, int *ierr);
void F_NDI_GET_FLOAT64_VAL_N(int *in_key, int *vec_num,
			     ndi_float64 * return_val, int *ierr);
void F_NDI_GET_FLOAT32_VEC(int *in_key, ndi_float32 * return_vec,
			   int *vec_size, int *ierr);
void F_NDI_GET_FLOAT64_VEC(int *in_key, ndi_float64 * return_vec,
			   int *vec_size, int *ierr);
void F_NDI_GET_FLOAT32_VEC_N(int *in_key, int *vec_num,
			     ndi_float32 * return_vec, int *vec_size,
			     int *ierr);
void F_NDI_GET_FLOAT64_VEC_N(int *in_key, int *vec_num,
			     ndi_float64 * return_vec, int *vec_size,
			     int *ierr);
void F_NDI_GET_FLOAT32_VEC_X(int *in_key, _fcd in_str,
			     ndi_float32 * return_vec, int *vec_size,
			     int *ierr);
void F_NDI_GET_FLOAT64_VEC_X(int *in_key, _fcd in_str,
			     ndi_float64 * return_vec, int *vec_size,
			     int *ierr);
void F_NDI_GET_STRING_VAL(int *in_key, _fcd return_val, int *ierr);
void F_NDI_GET_STRING_VAL_N(int *in_key, int *vec_num, _fcd return_val,
			    int *ierr);
static void print_list(char *list, int list_num);

/***************     Functions     ***************/

void F_NDID_GET_SIZE(int *in_key, _fcd in_value, int *arr_size, int *ierr)
{

    char *c_val;
    int temp_arr_size;
    int flen = _fcdlen(in_value);
    int cstr_size;

    cstr_size = flen + 1;
    c_val = (char *) ndi_malloc(cstr_size);
    (void) ndi_f2c_string(in_value, &c_val, cstr_size);
    *ierr = ndid_c_get_num(*in_key, c_val, &temp_arr_size);
    *arr_size = temp_arr_size;
    if (c_val)
	ndi_free((void **) &c_val);
}


void F_NDID_GET_STRING_VEC(int *in_key, _fcd in_value, _fcd str_arr,
			   int *arr_size, int *str_size, int *ierr)
{
    int i, j;
    int flen1;
    int flen2;
    int tmp_err = 0;
    char *curr_zaid;
    char *c_val;
    char *c_zaid;
    int c_arr_size;
    int c_arr_num;
    int count = 0;
    int copysize;
    int padsize;
    char *fstr_p;

    flen1 = _fcdlen(in_value);
    flen2 = _fcdlen(str_arr);

    c_arr_size = flen1 + 1;
    c_val = (char *) ndi_malloc(c_arr_size * sizeof(char));
    (void) ndi_f2c_string(in_value, &c_val, c_arr_size);

    tmp_err = ndid_c_get_size(*in_key, c_val, &c_arr_size);
    c_zaid = (char *) ndi_malloc(c_arr_size * sizeof(char));

    *ierr = ndid_c_get_string_vec(*in_key, c_val, c_zaid, c_arr_size);

    curr_zaid = c_zaid;

#if    defined(_ADDR64)		/* CRAY T3D, CRAY TS, Solaris */
    fstr_p = str_arr.c_point;
    /* End of _ADDR64 */
#elif     _ADDR32 && ! defined(_WORD32)	/* CRAY Y-MP and CRAY-2  */
    fstr_p = (char *) str_arr;
#endif				/* Y-MP */

    tmp_err = ndid_c_get_num(*in_key, c_val, &c_arr_num);
    while (count < c_arr_num) {
	/* ????? check we've not exceed the bounds on str_arr or c_zaid ????? */
	if (curr_zaid != NULL) {

#if      defined(_ADDR64)	/* CRAY T3D, CRAY TS, Solaris */
	    if (strlen(curr_zaid) <= flen2) {
		copysize = strlen(curr_zaid);
		padsize = flen2 - copysize + 1;
	    } else {
		copysize = flen2;
		padsize = 0;
	    }
	    strncpy(fstr_p, curr_zaid, copysize);
	    for (j = 0; j < padsize; j++) {
		*(fstr_p + copysize + j) = ' ';
	    }
	    /* End of _ADDR64 */

#elif _ADDR32 && ! defined(_WORD32)	/* CRAY Y-MP and CRAY-2  */
	    ndi_c2f_string(curr_zaid, (_fcd) fstr_p);
#endif				/* Y-MP */

	    fstr_p += *str_size;
	    curr_zaid += strlen(curr_zaid) + 1;
	}

	count++;
    }

    if (c_val)
	ndi_free((void **) &c_val);
    if (c_zaid)
	ndi_free((void **) &c_zaid);
}


void F_NDID_ZAID_FLOAT64_VEC(int *in_key, _fcd in_value, _fcd str_arr,
			     int *arr_size, int *str_size,
			     ndi_float64 * float_arr, int *fl_arr_size,
			     int *ierr)
{
    int i, j;
    int flen1;
    int flen2;
    int tmp_err = 0;
    char *curr_zaid;
    char *c_val;
    char *c_zaid;
    int c_arr_size;
    int c_arr_num;
    int count = 0;
    int copysize;
    int padsize;
    char *fstr_p;

    flen1 = _fcdlen(in_value);
    flen2 = _fcdlen(str_arr);

    c_arr_size = flen1 + 1;
    c_val = (char *) ndi_malloc(c_arr_size * sizeof(char));
    (void) ndi_f2c_string(in_value, &c_val, c_arr_size);

    tmp_err = ndid_c_get_size(*in_key, c_val, &c_arr_size);
    c_zaid = (char *) ndi_malloc(c_arr_size * sizeof(char));

    *ierr =
	ndid_c_zaid_float64_vec(*in_key, c_val, c_zaid, c_arr_size,
				float_arr, *fl_arr_size);

    curr_zaid = c_zaid;

#if    defined(_ADDR64)		/* CRAY T3D, CRAY TS, Solaris */
    fstr_p = str_arr.c_point;
    /* End of _ADDR64 */
#elif     _ADDR32 && ! defined(_WORD32)	/* CRAY Y-MP and CRAY-2  */
    fstr_p = (char *) str_arr;
#endif				/* Y-MP */

    tmp_err = ndid_c_get_num(*in_key, c_val, &c_arr_num);
    while (count < c_arr_num) {
	if (curr_zaid != NULL) {
	    /* ???? Check we've not exceeded the bound on str_arr or c_zaid ???? */

#if      defined(_ADDR64)	/* CRAY T3D, CRAY TS, Solaris */
	    if (strlen(curr_zaid) <= flen2) {
		copysize = strlen(curr_zaid);
		padsize = flen2 - copysize + 1;
	    } else {
		copysize = flen2;
		padsize = 0;
	    }
	    strncpy(fstr_p, curr_zaid, copysize);
	    for (j = 0; j < padsize; j++) {
		*(fstr_p + copysize + j) = ' ';
	    }
	    /* End of _ADDR64 */

#elif _ADDR32 && ! defined(_WORD32)	/* CRAY Y-MP and CRAY-2  */
	    ndi_c2f_string(curr_zaid, (_fcd) fstr_p);
#endif				/* Y-MP */

	    fstr_p += *str_size;
	    curr_zaid += strlen(curr_zaid) + 1;
	}
	count++;
    }

    if (c_val)
	ndi_free((void **) &c_val);
    if (c_zaid)
	ndi_free((void **) &c_zaid);
}


void F_NDID_ZAID_FLOAT32_VEC(int *in_key, _fcd in_value, _fcd str_arr,
			     int *arr_size, int *str_size,
			     ndi_float32 * float_arr, int *fl_arr_size,
			     int *ierr)
{
    int i, j;
    int flen1;
    int flen2;
    int tmp_err = 0;
    char *curr_zaid;
    char *c_val;
    char *c_zaid;
    int c_arr_size;
    int c_arr_num;
    int copysize;
    int padsize;
    int count = 0;
    char *fstr_p;

    flen1 = _fcdlen(in_value);
    flen2 = _fcdlen(str_arr);

    c_arr_size = flen1 + 1;
    c_val = (char *) ndi_malloc(c_arr_size * sizeof(char));
    (void) ndi_f2c_string(in_value, &c_val, c_arr_size);

    tmp_err = ndid_c_get_size(*in_key, c_val, &c_arr_size);
    c_zaid = (char *) ndi_malloc(c_arr_size * sizeof(char));

    *ierr =
	ndid_c_zaid_float32_vec(*in_key, c_val, c_zaid, c_arr_size,
				float_arr, *fl_arr_size);

    curr_zaid = c_zaid;

#if    defined(_ADDR64)		/* CRAY T3D, CRAY TS, Solaris */
    fstr_p = str_arr.c_point;
    /* End of _ADDR64 */
#elif     _ADDR32 && ! defined(_WORD32)	/* CRAY Y-MP and CRAY-2  */
    fstr_p = (char *) str_arr;
#endif				/* Y-MP */

    tmp_err = ndid_c_get_num(*in_key, c_val, &c_arr_num);
    while (count < c_arr_num) {
	if (curr_zaid != NULL) {
	    /* ???? Check we've not exceeded the bound on str_arr or c_zaid ???? */

#if      defined(_ADDR64)	/* CRAY T3D, CRAY TS, Solaris */
	    if (strlen(curr_zaid) <= flen2) {
		copysize = strlen(curr_zaid);
		padsize = flen2 - copysize + 1;
	    } else {
		copysize = flen2;
		padsize = 0;
	    }
	    strncpy(fstr_p, curr_zaid, copysize);
	    for (j = 0; j < padsize; j++) {
		*(fstr_p + copysize + j) = ' ';
	    }
	    /* End of _ADDR64 */

#elif _ADDR32 && ! defined(_WORD32)	/* CRAY Y-MP and CRAY-2  */
	    ndi_c2f_string(curr_zaid, (_fcd) fstr_p);
#endif				/* Y-MP */

	    fstr_p += *str_size;
	    curr_zaid += strlen(curr_zaid) + 1;
	}
	count++;
    }

    if (c_val)
	ndi_free((void **) &c_val);
    if (c_zaid)
	ndi_free((void **) &c_zaid);
}


void F_NDI_INIT(int *ierr)
{
    *ierr = ndi_c_init();
}

void F_NDI_CLOSE(int *ierr)
{
    *ierr = ndi_c_close();
}

void F_NDI_SET_OPTION(int *in_key, _fcd in_value, int *ierr)
{

    char *c_val;
    int len = _fcdlen(in_value);

    c_val = (char *) ndi_malloc((len + 1) * sizeof(char));
    (void) ndi_f2c_string(in_value, &c_val, len + 1);
    *ierr = ndi_c_set_option(*in_key, c_val);
    if (c_val)
	ndi_free((void **) &c_val);
}


void F_NDI_CHECK_OPTION(int *in_key, _fcd out_value, int *ierr)
{

    char c_str[MAX_IN_STR_LEN];

    *ierr = ndi_c_check_option(*in_key, c_str, MAX_IN_STR_LEN);
    (void) ndi_c2f_string(c_str, out_value);

}


void F_NDI_SET_ZAID(_fcd zaid, int *ierr)
{

    char *c_zaid;
    int zaid_len = _fcdlen(zaid);

    c_zaid = (char *) ndi_malloc((zaid_len + 1) * sizeof(char));
    (void) ndi_f2c_string(zaid, &c_zaid, zaid_len + 1);
    *ierr = ndi_c_set_zaid(c_zaid);
    if (c_zaid)
	ndi_free((void **) &c_zaid);
}

void F_NDI_SET_ISOTOPE(_fcd zaid, int *ierr)
{

    char *c_zaid;
    int zaid_len = _fcdlen(zaid);

    c_zaid = (char *) ndi_malloc((zaid_len + 1) * sizeof(char));
    (void) ndi_f2c_string(zaid, &c_zaid, zaid_len + 1);
    *ierr = ndi_c_set_isotope(c_zaid);
    if (c_zaid)
	ndi_free((void **) &c_zaid);
}

void F_NDI_SET_REACTION(_fcd zaid, int *ierr)
{

    char *c_zaid;
    int zaid_len = _fcdlen(zaid);

    c_zaid = (char *) ndi_malloc((zaid_len + 1) * sizeof(char));
    (void) ndi_f2c_string(zaid, &c_zaid, zaid_len + 1);
    *ierr = ndi_c_set_reaction(c_zaid);
    if (c_zaid)
	ndi_free((void **) &c_zaid);
}

void F_NDI_GET_SIZE(int *in_key, int *return_val, int *ierr)
{
    *return_val = ndi_c_get_size(*in_key, ierr);
}

void F_NDI_GET_SIZE_N(int *in_key, int *vec_num, int *return_val,
		      int *ierr)
{
    *return_val = ndi_c_get_size_n(*in_key, *vec_num, ierr);
}

void F_NDI_GET_SIZE_X(int *in_key, _fcd in_value, int *return_val,
		      int *ierr)
{

    char *c_val;
    int len = _fcdlen(in_value);

    c_val = (char *) ndi_malloc((len + 1) * sizeof(char));
    (void) ndi_f2c_string(in_value, &c_val, len + 1);
    *return_val = ndi_c_get_size_x(*in_key, c_val, ierr);
    if (c_val)
	ndi_free((void **) &c_val);
}

void F_NDI_GET_INT_VAL(int *in_key, int *return_val, int *ierr)
{
    *ierr = ndi_c_get_int_val(*in_key, return_val);
}

void F_NDI_GET_INT_VAL_N(int *in_key, int *vec_num, int *return_val,
			 int *ierr)
{
    *ierr = ndi_c_get_int_val_n(*in_key, *vec_num, return_val);
}

void F_NDI_GET_INT_VEC(int *in_key, int *return_val, int *vec_size,
		       int *ierr)
{
    *ierr = ndi_c_get_int_vec(*in_key, return_val, *vec_size);
}

void F_NDI_GET_INT_VEC_N(int *in_key, int *vec_num, int *return_val,
			 int *vec_size, int *ierr)
{
    *ierr = ndi_c_get_int_vec_n(*in_key, *vec_num, return_val, *vec_size);
}

void F_NDI_GET_FLOAT32_VAL(int *in_key, ndi_float32 * return_val,
			   int *ierr)
{
    *ierr = ndi_c_get_float32_val(*in_key, return_val);
}

void F_NDI_GET_FLOAT32_VAL_N(int *in_key, int *vec_num,
			     ndi_float32 * return_val, int *ierr)
{
    *ierr = ndi_c_get_float32_val_n(*in_key, *vec_num, return_val);
}

void F_NDI_GET_FLOAT32_VEC(int *in_key, ndi_float32 * return_val,
			   int *vec_size, int *ierr)
{
    *ierr = ndi_c_get_float32_vec(*in_key, return_val, *vec_size);
}

void F_NDI_GET_FLOAT32_VEC_N(int *in_key, int *vec_num,
			     ndi_float32 * return_vec, int *vec_size,
			     int *ierr)
{
    *ierr =
	ndi_c_get_float32_vec_n(*in_key, *vec_num, return_vec, *vec_size);
}


void F_NDI_GET_FLOAT32_VEC_X(int *in_key, _fcd in_str,
			     ndi_float32 * return_vec, int *vec_size,
			     int *ierr)
{

    char *c_str;
    int in_str_len = _fcdlen(in_str);

    c_str = (char *) ndi_malloc((in_str_len + 1) * sizeof(char));
    (void) ndi_f2c_string(in_str, &c_str, in_str_len + 1);
    *ierr = ndi_c_get_float32_vec_x(*in_key, c_str, return_vec, *vec_size);
    if (c_str)
	ndi_free((void **) &c_str);
}

void F_NDI_GET_FLOAT64_VAL(int *in_key, ndi_float64 * return_val,
			   int *ierr)
{
    *ierr = ndi_c_get_float64_val(*in_key, return_val);
}

void F_NDI_GET_FLOAT64_VAL_N(int *in_key, int *vec_num,
			     ndi_float64 * return_val, int *ierr)
{

    *ierr = ndi_c_get_float64_val_n(*in_key, *vec_num, return_val);
}

void F_NDI_GET_FLOAT64_VEC(int *in_key, ndi_float64 * return_val,
			   int *vec_size, int *ierr)
{
    *ierr = ndi_c_get_float64_vec(*in_key, return_val, *vec_size);
}

void F_NDI_GET_FLOAT64_VEC_N(int *in_key, int *vec_num,
			     ndi_float64 * return_vec, int *vec_size,
			     int *ierr)
{
    *ierr =
	ndi_c_get_float64_vec_n(*in_key, *vec_num, return_vec, *vec_size);
}

void F_NDI_GET_FLOAT64_VEC_X(int *in_key, _fcd in_str,
			     ndi_float64 * return_vec, int *vec_size,
			     int *ierr)
{

    char *c_str;
    int in_str_len = _fcdlen(in_str);

    c_str = (char *) ndi_malloc((in_str_len + 1) * sizeof(char));
    (void) ndi_f2c_string(in_str, &c_str, in_str_len + 1);
    *ierr = ndi_c_get_float64_vec_x(*in_key, c_str, return_vec, *vec_size);
    if (c_str)
	ndi_free((void **) &c_str);
}

void F_NDI_GET_STRING_VAL(int *in_key, _fcd return_val, int *ierr)
{

    char *c_str;
    int tmp_err;
    int temsize;

    temsize = ndi_c_get_size(*in_key, &tmp_err);
    if (tmp_err >= 0) {
	c_str = (char *) ndi_malloc(temsize * sizeof(char));
	*ierr = ndi_c_get_string_val(*in_key, c_str, temsize);
      /*** Check temsize v. size of return val ***/
	(void) ndi_c2f_string(c_str, return_val);
	(void) ndi_free((void **) &c_str);
    } else {
	*ierr = tmp_err;
    }
    return;
}

void F_NDI_GET_STRING_VAL_N(int *in_key, int *vec_num, _fcd return_val,
			    int *ierr)
{

    char *c_str;
    int tmp_err;
    int temsize;

    temsize = ndi_c_get_size_n(*in_key, *vec_num, &tmp_err);
    if (tmp_err >= 0) {
	c_str = (char *) ndi_malloc(temsize * sizeof(char));
	*ierr = ndi_c_get_string_val_n(*in_key, *vec_num, c_str, temsize);
      /*** Check temsize v. size of return val ***/
      /*** check error condition ***/
	(void) ndi_c2f_string(c_str, return_val);
	(void) ndi_free((void **) &c_str);
    } else {
	*ierr = tmp_err;
    }
    return;
}

static void ndi_c2f_string(char *cstring, _fcd fstring)
{
    int clen = strlen(cstring);
    int flen = _fcdlen(fstring);
    int i;
    char *full_cstr = (char *) ndi_malloc((flen + 1) * sizeof(char));

    if (clen > flen) {
	clen = flen;
    }
    strncpy(full_cstr, cstring, clen);
    for (i = clen; i < flen; i++) {
	/* pad string with blanks */
	full_cstr[i] = ' ';
    }
    full_cstr[flen + 1] = '\0';
    strncpy(_fcdtocp(fstring), full_cstr, flen);
    return;
}


static void ndi_f2c_string(_fcd fstring, char **cstring, int cstr_size)
{
    char *temp_str_p;
    char sep[] = " ";
    int str_len = _fcdlen(fstring);
    int junk;
    char *tmp_ptr = *cstring;
    int i;
    int cstr_count = 0;


    temp_str_p = _fcdtocp(fstring);
    junk = str_len * sizeof(char);
    for (i = 0; i < str_len; i++) {
	if (*temp_str_p != ' ') {
	    *tmp_ptr = *temp_str_p;
	    cstr_count++;
	    if (cstr_count >= cstr_size) {
		break;
	    }
	    tmp_ptr++;
	}
	temp_str_p++;
    }
    *tmp_ptr = '\0';
}


static void print_list(char *list, int list_num)
{

    char *str_p;
    int list_count = 0;

    str_p = list;
    while (list_count < list_num) {
	list_count++;
/*     printf("%d:  %s\n", list_count, str_p); */
	str_p += strlen(str_p) + 1;
    }
}
