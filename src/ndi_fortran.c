
/*
 * File:	ndi_fortran.c
 * Description:	Source for libndi.a
 * Time-stamp:	<2000-02-02 17:12:31 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fmangle.h"

#include "ndi.h"
#include "ndi_internal.h"
#include "ndi_var_type.h"


/**********************************************************************/

static void ndi_f2c_string(char *fstring, char **cstring, int flen,
			   int clen);
static void ndi_c2f_string(char *cstring, char *fstring, int flen);
void F_NDID_GET_SIZE(int *in_key, char *in_value, int *arr_size, int *ierr,
		     int flen);
void F_NDID_GET_STRING_VEC(int *in_key, char *in_value, char *str_arr,
			   int *arr_size, int *str_size, int *ierr,
			   int flen1, int flen2);
void F_NDID_ZAID_FLOAT64_VEC(int *in_key, char *in_value, char *str_arr,
			     int *arr_size, int *zaid_size,
			     ndi_float64 * float_vec, int *fl_arr_size,
			     int *ierr, int flen1, int flen2);
void F_NDID_ZAID_FLOAT32_VEC(int *in_key, char *in_value, char *str_arr,
			     int *arr_size, int *zaid_size,
			     ndi_float32 * float_vec, int *fl_arr_size,
			     int *ierr, int flen1, int flen2);
void F_NDI_INIT(int *ierr);
void F_NDI_CLOSE(int *ierr);
void F_NDI_SET_OPTION(int *in_key, char *in_value, int *ierr, int len);
void F_NDI_CHECK_OPTION(int *in_key, char *out_value, int *ierr, int len);
void F_NDI_SET_ISOTOPE(char *zaid, int *ierr, int zaid_len);
void F_NDI_SET_ZAID(char *zaid, int *ierr, int zaid_len);
void F_NDI_SET_REACTION(char *zaid, int *ierr, int zaid_len);
void F_NDI_GET_SIZE(int *in_key, int *return_val, int *ierr);
void F_NDI_GET_SIZE_N(int *in_key, int *vec_num, int *return_val,
		      int *ierr);
void F_NDI_GET_SIZE_X(int *in_key, char *in_value, int *return_val,
		      int *ierr, int len);
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
void F_NDI_GET_FLOAT32_VEC_X(int *in_key, char *in_str,
			     ndi_float32 * return_vec, int *vec_size,
			     int *ierr, int in_str_len);
void F_NDI_GET_FLOAT64_VEC_X(int *in_key, char *in_str,
			     ndi_float64 * return_vec, int *vec_size,
			     int *ierr, int in_str_len);
void F_NDI_GET_STRING_VAL(int *in_key, char *return_val, int *ierr,
			  int char_len);
void F_NDI_GET_STRING_VAL_N(int *in_key, int *vec_num, char *return_val,
			    int *ierr, int char_len);

/* NDI 2.0 user functions with handles */

void F_NDI2_OPEN(int *handle, char *path, int *ierr, int len);
void F_NDI2_SET_ZAID(int *handle, char *zaid, int *ierr, int len);
void F_NDI2_SET_ISOTOPE(int *handle, char *zaid, int *ierr, int len);
void F_NDI2_SET_REACTION(int *handle, char *zaid, int *ierr, int len);
void F_NDI2_GET_HANDLE(int *handle0, int *handle1, int *ierr);
void F_NDI2_GET_MPMG_HANDLE(int *handle1, int *handle2, int *ierr);
void F_NDI2_GET_SIZE(int *handle, int *in_key, int *return_val, int *ierr);
void F_NDI2_GET_SIZE_N(int *handle, int *in_key, int *vec_num,
                       int *return_val, int *ierr);
void F_NDI2_GET_SIZE_X(int *handle, int *in_key, char *in_str,
                       int *return_val, int *ierr, int len);
void F_NDI2_SET_OPTION(int *handle, int *in_key, char *value, int *ierr,
                       int len);
void F_NDI2_SET_FLOAT64_VEC_OPTION(int *handle, int *in_key,
                                   ndi_float64 *option_array,
                                   int *array_size, int *ierr);
void F_NDI2_CHECK_OPTION(int *handle, int *in_key, char *out_value,
                         int *ierr, int len);
void F_NDI2_GET_INT_VAL(int *handle, int *in_key, int *return_val, int *ierr);
void F_NDI2_GET_INT_VAL_N(int *handle, int *in_key, int *vec_num,
                          int *return_val, int *ierr);
void F_NDI2_GET_INT_VEC(int *handle, int *in_key, int *return_vec,
                        int *vec_size, int *ierr);
void F_NDI2_GET_INT_VEC_N(int *handle, int *in_key, int *vec_num,
                          int *return_vec, int *vec_size, int *ierr);
void F_NDI2_GET_FLOAT32_VAL(int *handle, int *in_key,
                            ndi_float32 *return_val, int *ierr);
void F_NDI2_GET_FLOAT32_VAL_N(int *handle, int *in_key, int *vec_num,
                              ndi_float32 *return_val, int *ierr);
void F_NDI2_GET_FLOAT32_VEC(int *handle, int *in_key,
                            ndi_float32 *return_vec, int *vec_size, int *ierr);
void F_NDI2_GET_FLOAT32_VEC_N(int *handle, int *in_key, int *vec_num,
                              ndi_float32 *return_vec, int *vec_size, int *ierr);
void F_NDI2_GET_FLOAT32_VEC_X(int *handle, int *in_key,
                              char *in_str,
                              ndi_float32 *return_vec, int *vec_size, int *ierr,
                              int len);
void F_NDI2_GET_FLOAT64_VAL(int *handle, int *in_key,
                            ndi_float64 *return_val, int *ierr);
void F_NDI2_GET_FLOAT64_VAL_N(int *handle, int *in_key, int *vec_num,
                              ndi_float64 *return_val, int *ierr);
void F_NDI2_GET_FLOAT64_VEC(int *handle, int *in_key,
                            ndi_float64 *return_vec, int *vec_size, int *ierr);
void F_NDI2_GET_FLOAT64_VEC_N(int *handle, int *in_key, int *vec_num,
                              ndi_float64 *return_vec, int *vec_size, int *ierr);
void F_NDI2_GET_FLOAT64_VEC_X(int *handle, int *in_key,
                              char *in_str,
                              ndi_float64 * return_vec, int *vec_size, int *ierr,
                              int len);
void F_NDI2_GET_STRING_VAL(int *handle, int *in_key, char *return_val,
                           int *ierr, int len);
void F_NDI2_GET_STRING_VAL_N(int *handle, int *in_key, int *vec_num,
                             char *return_val, int *ierr, int len);
void F_NDI2_CLOSE(int *handle, int *ierr);

/***************     Functions     ***************/

void F_NDID_GET_SIZE(int *in_key, char *in_value, int *arr_size, int *ierr,
                     int flen)
{
    char *c_val;
    int temp_arr_size;
    int cstr_size;

    cstr_size = flen + 1;
    c_val = (char *) ndi_malloc(cstr_size * sizeof(char));
    (void) ndi_f2c_string(in_value, &c_val, flen, cstr_size);
    *ierr = ndid_c_get_num(*in_key, c_val, &temp_arr_size);
    *arr_size = temp_arr_size;	/*  / (sizeof(char) * 13); */
    if (c_val)
	ndi_free((void **) &c_val);
}

void F_NDID_GET_STRING_VEC(int *in_key, char *in_value, char *str_arr,
                           int *arr_size, int *str_size, int *ierr,
                           int flen1, int flen2)
{
    /* Is str_size needed?  */

    char *curr_zaid;
    char *c_val;
    char *c_zaid;
    int c_arr_num;
    int c_arr_size;
    int count = 0;
    char *fstr_p;

    c_arr_size = flen1 + 1;
    c_val = (char *) ndi_malloc(c_arr_size * sizeof(char));
    (void) ndi_f2c_string(in_value, &c_val, flen1, c_arr_size);

    (void) ndid_c_get_size(*in_key, c_val, &c_arr_size);
    c_zaid = (char *) ndi_malloc(c_arr_size * sizeof(char));

    *ierr = ndid_c_get_string_vec(*in_key, c_val, c_zaid, c_arr_size);

    fstr_p = str_arr;
    curr_zaid = c_zaid;
    (void) ndid_c_get_num(*in_key, c_val, &c_arr_num);
    while (count < c_arr_num) {
	if (curr_zaid != NULL) {
	    /* ????? check we've not exceed the bounds on str_arr or c_zaid ????? */
	    ndi_c2f_string(curr_zaid, fstr_p, *str_size);
	    curr_zaid += strlen(curr_zaid) + 1;
	    fstr_p += *str_size;
	}
	count++;
    }

    if (c_val)
	ndi_free((void **) &c_val);
    if (c_zaid)
	ndi_free((void **) &c_zaid);
}

void F_NDID_ZAID_FLOAT64_VEC(int *in_key, char *in_value, char *str_arr,
                             int *arr_size, int *str_size,
                             ndi_float64 * float_vec, int *fl_arr_size,
                             int *ierr, int flen1, int flen2)
{
    char *curr_zaid;
    char *c_val;
    char *c_zaid;
    char *fstr_p;
    int c_arr_num;
    int c_arr_size;
    int count = 0;

    c_arr_size = flen1 + 1;
    c_val = (char *) ndi_malloc(c_arr_size * sizeof(char));
    (void) ndi_f2c_string(in_value, &c_val, flen1, c_arr_size);

/*    c_arr_size = *arr_size * (flen2 + 1) * sizeof(char); */
    /*  ??? NEED TO GET THE BYTE SIZE HERE NOT THE NUMBER ???? */
    (void) ndid_c_get_size(*in_key, c_val, &c_arr_size);
    c_zaid = (char *) ndi_malloc(c_arr_size * sizeof(char));

    *ierr =
	ndid_c_zaid_float64_vec(*in_key, c_val, c_zaid, c_arr_size,
				float_vec, *fl_arr_size);

    fstr_p = str_arr;
    curr_zaid = c_zaid;
    (void) ndid_c_get_num(*in_key, c_val, &c_arr_num);
    while (count < c_arr_num) {
	if (curr_zaid != NULL) {
	    /* ????? check we've not exceed the bounds on str_arr or c_zaid ????? */
	    ndi_c2f_string(curr_zaid, fstr_p, *str_size);
	    curr_zaid += strlen(curr_zaid) + 1;
	    fstr_p += *str_size;
	}
	count++;
    }

    if (c_val)
	ndi_free((void **) &c_val);
    if (c_zaid)
	ndi_free((void **) &c_zaid);
}


void F_NDID_ZAID_FLOAT32_VEC(int *in_key, char *in_value, char *str_arr,
                             int *arr_size, int *str_size,
                             ndi_float32 * float_vec, int *fl_arr_size,
                             int *ierr, int flen1, int flen2)
{
    char *curr_zaid;
    char *c_val;
    char *c_zaid;
    char *fstr_p;
    int c_arr_num;
    int c_arr_size;
    int count = 0;



    c_arr_size = flen1 + 1;
    c_val = (char *) ndi_malloc(c_arr_size * sizeof(char));
    (void) ndi_f2c_string(in_value, &c_val, flen1, c_arr_size);

/*    c_arr_size = *arr_size * (flen2 + 1) * sizeof(char); */
    /*  ??? NEED TO GET THE BYTE SIZE HERE NOT THE NUMBER ???? */
    (void) ndid_c_get_size(*in_key, c_val, &c_arr_size);
    c_zaid = (char *) ndi_malloc(c_arr_size * sizeof(char));

    *ierr =
	ndid_c_zaid_float32_vec(*in_key, c_val, c_zaid, c_arr_size,
				float_vec, *fl_arr_size);

    fstr_p = str_arr;
    curr_zaid = c_zaid;
    (void) ndid_c_get_num(*in_key, c_val, &c_arr_num);
    while (count < c_arr_num) {
	if (curr_zaid != NULL) {
	    /* ????? check we've not exceed the bounds on str_arr or c_zaid ????? */
	    ndi_c2f_string(curr_zaid, fstr_p, *str_size);
	    curr_zaid += strlen(curr_zaid) + 1;
	    fstr_p += *str_size;
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

void F_NDI_SET_OPTION(int *in_key, char *in_value, int *ierr, int len)
{
    char *c_val;

    c_val = (char *) ndi_malloc((len + 1) * sizeof(char));
    (void) ndi_f2c_string(in_value, &c_val, len, len + 1);
    *ierr = ndi_c_set_option(*in_key, c_val);
    if (c_val)
	ndi_free((void **) &c_val);
}

void F_NDI_CHECK_OPTION(int *in_key, char *out_value, int *ierr, int len)
{
    char c_str[MAX_IN_STR_LEN];

    *ierr = ndi_c_check_option(*in_key, c_str, MAX_IN_STR_LEN);
    (void) ndi_c2f_string(c_str, out_value, len);
}

void F_NDI_SET_ISOTOPE(char *zaid, int *ierr, int zaid_len)
{
    char *c_zaid;

    c_zaid = (char *) ndi_malloc((zaid_len + 1) * sizeof(char));
    (void) ndi_f2c_string(zaid, &c_zaid, zaid_len, zaid_len + 1);
    *ierr = ndi_c_set_isotope(c_zaid);
    if (c_zaid)
	ndi_free((void **) &c_zaid);
}

void F_NDI_SET_ZAID(char *zaid, int *ierr, int zaid_len)
{
    char *c_zaid;

    c_zaid = (char *) ndi_malloc((zaid_len + 1) * sizeof(char));
    (void) ndi_f2c_string(zaid, &c_zaid, zaid_len, zaid_len + 1);
    *ierr = ndi_c_set_zaid(c_zaid);
    if (c_zaid)
	ndi_free((void **) &c_zaid);
}

void F_NDI_SET_REACTION(char *zaid, int *ierr, int zaid_len)
{
    char *c_zaid;

    c_zaid = (char *) ndi_malloc((zaid_len + 1) * sizeof(char));
    (void) ndi_f2c_string(zaid, &c_zaid, zaid_len, zaid_len + 1);
    *ierr = ndi_c_set_reaction(c_zaid);
    if (c_zaid)
	ndi_free((void **) &c_zaid);
}

void F_NDI_GET_SIZE(int *in_key, int *return_val, int *ierr)
{
    *return_val = ndi_c_get_size(*in_key, ierr);
}

void F_NDI_GET_SIZE_N(int *in_key, int *vec_num, int *return_val, int *ierr)
{
    *return_val = ndi_c_get_size_n(*in_key, *vec_num, ierr);
}

void F_NDI_GET_SIZE_X(int *in_key, char *in_value, int *return_val,
                      int *ierr, int len)
{
    char *c_val;

    c_val = (char *) ndi_malloc((len + 1) * sizeof(char));
    (void) ndi_f2c_string(in_value, &c_val, len, len + 1);
    *return_val = ndi_c_get_size_x(*in_key, c_val, ierr);
    if (c_val)
	ndi_free((void **) &c_val);
}

void F_NDI_GET_INT_VAL(int *in_key, int *return_val, int *ierr)
{
    *ierr = ndi_c_get_int_val(*in_key, return_val);
}

void F_NDI_GET_INT_VAL_N(int *in_key, int *vec_num, int *return_val, int *ierr)
{
    *ierr = ndi_c_get_int_val_n(*in_key, *vec_num, return_val);
}

void F_NDI_GET_INT_VEC(int *in_key, int *return_val, int *vec_size, int *ierr)
{
    *ierr = ndi_c_get_int_vec(*in_key, return_val, *vec_size);
}

void F_NDI_GET_INT_VEC_N(int *in_key, int *vec_num, int *return_val,
		    int *vec_size, int *ierr)
{
    *ierr = ndi_c_get_int_vec_n(*in_key, *vec_num, return_val, *vec_size);
}


void F_NDI_GET_FLOAT64_VAL(int *in_key, ndi_float64 * return_val, int *ierr)
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
                             ndi_float64 * return_vec, int *vec_size, int *ierr)
{
    *ierr = ndi_c_get_float64_vec_n(*in_key, *vec_num, return_vec, *vec_size);
}

void F_NDI_GET_FLOAT64_VEC_X(int *in_key, char *in_str,
                             ndi_float64 * return_vec, int *vec_size,
                             int *ierr, int in_str_len)
{
    char *c_str;

    c_str = (char *) ndi_malloc((in_str_len + 1) * sizeof(char));
    (void) ndi_f2c_string(in_str, &c_str, in_str_len, in_str_len + 1);
    *ierr = ndi_c_get_float64_vec_x(*in_key, c_str, return_vec, *vec_size);
    if (c_str)
	ndi_free((void **) &c_str);
}

void F_NDI_GET_FLOAT32_VAL(int *in_key, ndi_float32 * return_val, int *ierr)
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
                             ndi_float32 * return_vec, int *vec_size, int *ierr)
{

    *ierr =
	ndi_c_get_float32_vec_n(*in_key, *vec_num, return_vec, *vec_size);
}

void F_NDI_GET_FLOAT32_VEC_X(int *in_key, char *in_str,
                             ndi_float32 * return_vec, int *vec_size,
                             int *ierr, int in_str_len)
{
    char *c_str;

    c_str = (char *) ndi_malloc((in_str_len + 1) * sizeof(char));
    (void) ndi_f2c_string(in_str, &c_str, in_str_len, in_str_len + 1);
    *ierr = ndi_c_get_float32_vec_x(*in_key, c_str, return_vec, *vec_size);
    if (c_str)
	ndi_free((void **) &c_str);

}

void F_NDI_GET_STRING_VAL(int *in_key, char *return_val,
                          int *ierr, int char_len)
{
    char *c_str;
    int tmp_err;
    int temsize;

    temsize = ndi_c_get_size(*in_key, &tmp_err);
    if (temsize < 0) {
	*ierr = tmp_err;
	(void) ndi_c2f_string("", return_val, char_len);
	return;
    }

    c_str = (char *) ndi_malloc(temsize * sizeof(char));
    *ierr = ndi_c_get_string_val(*in_key, c_str, temsize);
    (void) ndi_c2f_string(c_str, return_val, char_len);
    (void) ndi_free((void **) &c_str);
}

void F_NDI_GET_STRING_VAL_N(int *in_key, int *vec_num, char *return_val,
                            int *ierr, int char_len)
{
    char *c_str;
    int tmp_err;
    int temsize;

    temsize = ndi_c_get_size_n(*in_key, *vec_num, &tmp_err);
    if (temsize < 0) {
	*ierr = tmp_err;
	(void) ndi_c2f_string("", return_val, char_len);
	return;
    }

    c_str = (char *) ndi_malloc(temsize * sizeof(char));
    *ierr = ndi_c_get_string_val_n(*in_key, *vec_num, c_str, temsize);
    (void) ndi_c2f_string(c_str, return_val, char_len);
    (void) ndi_free((void **) &c_str);
}

/***************     NDI 2.0 functions with handles     ***************/

void F_NDI2_OPEN(int *handle, char *path, int *ierr, int len)
{
    char *c_val;

    c_val = (char *) ndi_malloc((len + 1) * sizeof(char));
    (void)ndi_f2c_string(path, &c_val, len, len+1);
    *ierr = ndi2_open(handle, c_val);
    if (c_val)
        ndi_free((void **) &c_val);
}

void F_NDI2_SET_ZAID(int *handle, char *zaid, int *ierr, int len)
{
    char *c_val;

    c_val = (char *) ndi_malloc((len + 1) * sizeof(char));
    (void) ndi_f2c_string(zaid, &c_val, len, len + 1);
    *ierr = ndi2_set_zaid(*handle, c_val);
    if (c_val)
        ndi_free((void **) &c_val);
}

void F_NDI2_SET_ISOTOPE(int *handle, char *zaid, int *ierr, int len)
{
    char *c_val;

    c_val = (char *) ndi_malloc((len + 1) * sizeof(char));
    (void) ndi_f2c_string(zaid, &c_val, len, len + 1);
    *ierr = ndi2_set_isotope(*handle, c_val);
    if (c_val)
        ndi_free((void **) &c_val);
}

void F_NDI2_SET_REACTION(int *handle, char *zaid, int *ierr, int len)
{
    char *c_val;

    c_val = (char *) ndi_malloc((len + 1) * sizeof(char));
    (void) ndi_f2c_string(zaid, &c_val, len, len + 1);
    *ierr = ndi2_set_reaction(*handle, c_val);
    if (c_val)
        ndi_free((void **) &c_val);
}

void F_NDI2_GET_HANDLE(int *handle0, int *handle1, int *ierr)
{
    *ierr = ndi2_get_handle(*handle0, handle1);
}

void F_NDI2_GET_MPMG_HANDLE(int *handle1, int *handle2, int *ierr)
{
/*     *ierr = ndi2_get_mpmg_handle(*handle1, handle2); */
}

void F_NDI2_GET_SIZE(int *handle, int *in_key, int *return_val, int *ierr)
{
    *return_val = ndi2_get_size(*handle, *in_key, ierr);
}

void F_NDI2_GET_SIZE_N(int *handle, int *in_key, int *vec_num,
                      int *return_val, int *ierr)
{
    *return_val = ndi2_get_size_n(*handle, *in_key, *vec_num, ierr);
}

void F_NDI2_GET_SIZE_X(int *handle, int *in_key, char *in_str,
                      int *return_val, int *ierr, int len)
{
    char *c_val;

    c_val = (char *) ndi_malloc((len + 1) * sizeof(char));
    (void) ndi_f2c_string(in_str, &c_val, len, len + 1);
    *return_val = ndi2_get_size_x(*handle, *in_key, c_val, ierr);
    if (c_val)
	ndi_free((void **) &c_val);
}

void F_NDI2_SET_OPTION(int *handle, int *in_key, char *value, 
                      int *ierr, int len)
{
    char *c_val;

    c_val = (char *) ndi_malloc((len + 1) * sizeof(char));
    (void) ndi_f2c_string(value, &c_val, len, len + 1);
    *ierr = ndi_c_set_option(*in_key, c_val);
    if (c_val)
	ndi_free((void **) &c_val);
}

void F_NDI2_SET_FLOAT64_VEC_OPTION(int *handle, int *in_key,
                                  ndi_float64 *option_array,
                                  int *array_size, int *ierr)
{
    *ierr = ndi2_set_float64_vec_option(*handle, *in_key, 
                                        option_array, *array_size);
}

void F_NDI2_CHECK_OPTION(int *handle, int *in_key, char *out_value,
                        int *ierr, int len)
{
    char c_str[MAX_IN_STR_LEN];

    *ierr = ndi2_check_option(*handle, *in_key, c_str, MAX_IN_STR_LEN);
    (void) ndi_c2f_string(c_str, out_value, len);
}

void F_NDI2_GET_INT_VAL(int *handle, int *in_key, int *return_val, int *ierr)
{
    *ierr = ndi2_get_int_val(*handle, *in_key, return_val);
}

void F_NDI2_GET_INT_VAL_N(int *handle, int *in_key, int *vec_num,
                         int *return_val, int *ierr)
{
    *ierr = ndi2_get_int_val_n(*handle, *in_key, *vec_num, return_val);
}

void F_NDI2_GET_INT_VEC(int *handle, int *in_key, int *return_vec,
                       int *vec_size, int *ierr)
{
    *ierr = ndi2_get_int_vec(*handle, *in_key, return_vec, *vec_size);
}

void F_NDI2_GET_INT_VEC_N(int *handle, int *in_key, int *vec_num,
                         int *return_vec, int *vec_size, int *ierr)
{
    *ierr = ndi2_get_int_vec_n(*handle, *in_key, *vec_num, return_vec,
                               *vec_size);
}

void F_NDI2_GET_FLOAT32_VAL(int *handle, int *in_key,
                           ndi_float32 * return_val, int *ierr)
{
    *ierr = ndi2_get_float32_val(*handle, *in_key, return_val);
}

void F_NDI2_GET_FLOAT32_VAL_N(int *handle, int *in_key, int *vec_num,
                             ndi_float32 * return_val, int *ierr)
{
    *ierr = ndi2_get_float32_val_n(*handle, *in_key, *vec_num, return_val);
}

void F_NDI2_GET_FLOAT32_VEC(int *handle, int *in_key,
                           ndi_float32 *return_vec, int *vec_size,
                           int *ierr)
{
    *ierr = ndi2_get_float32_vec(*handle, *in_key, return_vec, *vec_size);
}

void F_NDI2_GET_FLOAT32_VEC_N(int *handle, int *in_key, int *vec_num,
                             ndi_float32 *return_vec, int *vec_size,
                             int *ierr)
{
    *ierr = ndi2_get_float32_vec_n(*handle, *in_key, *vec_num,
                                   return_vec, *vec_size);
}

void F_NDI2_GET_FLOAT32_VEC_X(int *handle, int *in_key,
                             char *in_str,
                             ndi_float32 *return_vec, int *vec_size,
                             int *ierr, int len)
{
    char *c_str;

    c_str = (char *) ndi_malloc((len + 1) * sizeof(char));
    (void) ndi_f2c_string(in_str, &c_str, len, len + 1);
    *ierr = ndi2_get_float32_vec_x(*handle, *in_key, c_str, return_vec,
                                   *vec_size);
    if (c_str)
        ndi_free((void **) &c_str);
}

void F_NDI2_GET_FLOAT64_VAL(int *handle, int *in_key,
                           ndi_float64 *return_val, int *ierr)
{
    *ierr = ndi2_get_float64_val(*handle, *in_key, return_val);
}

void F_NDI2_GET_FLOAT64_VAL_N(int *handle, int *in_key, int *vec_num,
                             ndi_float64 *return_val, int *ierr)
{
    *ierr = ndi2_get_float64_val_n(*handle, *in_key, *vec_num,
                                   return_val);
}

void F_NDI2_GET_FLOAT64_VEC(int *handle, int *in_key,
                           ndi_float64 *return_vec, int *vec_size,
                           int *ierr)
{
    *ierr = ndi2_get_float64_vec(*handle, *in_key, return_vec, *vec_size);
}

void F_NDI2_GET_FLOAT64_VEC_N(int *handle, int *in_key, int *vec_num,
                             ndi_float64 *return_vec, int *vec_size,
                             int *ierr)
{
    *ierr = ndi2_get_float64_vec_n(*handle, *in_key, *vec_num,
                                   return_vec, *vec_size);
}

void F_NDI2_GET_FLOAT64_VEC_X(int *handle, int *in_key,
                             char *in_str,
                             ndi_float64 * return_vec, int *vec_size,
                             int *ierr, int len)
{
    char *c_str;

    c_str = (char *) ndi_malloc((len + 1) * sizeof(char));
    (void) ndi_f2c_string(in_str, &c_str, len, len + 1);
    *ierr = ndi2_get_float64_vec_x(*handle, *in_key, c_str, return_vec, *vec_size);
    if (c_str)
	ndi_free((void **) &c_str);
}

void F_NDI2_GET_STRING_VAL(int *handle, int *in_key, char *return_val,
                          int *ierr, int len)
{
    char *c_str;
    int tmp_err;
    int temsize;

    temsize = ndi2_get_size(*handle, *in_key, &tmp_err);
    if (temsize < 0) {
	*ierr = tmp_err;
	(void) ndi_c2f_string("", return_val, len);
	return;
    }

    c_str = (char *) ndi_malloc(temsize * sizeof(char));
    *ierr = ndi2_get_string_val(*handle, *in_key, c_str, temsize);
    (void) ndi_c2f_string(c_str, return_val, len);
    (void) ndi_free((void **) &c_str);
}

void F_NDI2_GET_STRING_VAL_N(int *handle, int *in_key, int *vec_num,
                            char *return_val, int *ierr, int len)
{
    char *c_str;
    int tmp_err;
    int temsize;

    temsize = ndi2_get_size_n(*handle, *in_key, *vec_num, &tmp_err);
    if (temsize < 0) {
	*ierr = tmp_err;
	(void) ndi_c2f_string("", return_val, len);
	return;
    }

    c_str = (char *) ndi_malloc(temsize * sizeof(char));
    *ierr = ndi2_get_string_val_n(*handle, *in_key, *vec_num, c_str, temsize);
    (void) ndi_c2f_string(c_str, return_val, len);
    (void) ndi_free((void **) &c_str);
}

void F_NDI2_CLOSE(int *handle, int *ierr)
{
    *ierr = ndi2_close(*handle);
}

/***************     Internal functions     ***************/

static void ndi_c2f_string(char *cstring, char *fstring, int flen)
{

    char *tmp_ptr = fstring;
    int cdone = 0;
    int n;
    char *curr_cptr = cstring;

    if (!cstring) {
	cdone = -1;
    }

    for (n = 0; n < flen; n++) {
	if (cdone == 0) {
	    if (*curr_cptr != '\0') {
		*tmp_ptr = *curr_cptr;
		curr_cptr++;
	    } else {
		cdone = -1;
		*tmp_ptr = ' ';
	    }
	} else {
	    *tmp_ptr = ' ';
	}

	tmp_ptr++;
    }
}


static void ndi_f2c_string(char *fstring, char **cstring, int flen, int clen)
{

    int i;


    for (i = flen - 1; i > 0; i--) {
	if (fstring[i] != ' ')
	    break;
    }

    if (i >= 0) {
	strncpy(*cstring, fstring, (i + 1));
	(*cstring)[i + 1] = '\0';
    }
}
