/*
 * File:	ndi_internal.h
 * Description:	Header file for libndi
 * Time-stamp:	<2000-02-03 11:12:37 jomc>
 *
 * This header file is included in internal library code only.
 */

/**********************************************************************/

#ifndef __NDI_INTERNAL_H
#define __NDI_INTERNAL_H

#include "ndi_var_type.h"
#include "ndi_version.h"

/**********************************************************************/


/* Defines
 */


#define MAX_HANDLE_NUM         10

#define M_NEUT_KG       1.67492716e-27
#define J_TO_MEV        1.602177e-13

#define NUM_FISSION_Q		6
#define NUM_NORM		4

#define MAX_TN_REAC            50
#define MAX_TN_REAC_LEN        24
#define MAX_DEPL_REAC         250
#define DEF_FISSION_PROD      "46119"

#define READ_MT                -1
#define NO_MT                   0
#define ID_PARAM                0
#define MULT_PARAM              1
#define BOUNDARY_PARAM          0
#define TYPE_PARAM              1

#define MAX_IN_STR_LEN        133
#define MAX_IN_PATH_LEN       250
#define MAX_LIB_VERS_LEN       26
#define MAX_NGROUP_STR_LEN     20
#define MAX_Z                 103

/**********************************************************************/

/*
 *    Macros
 */

/*define NDI_CURR_ERR(new_err, curr_err) (  if ( (new_err) < 0 ) {
      (curr_err) = (new_err);
  } elseif ( ( (curr_err) >= 0 ) && ( (b=new_err) != 0 ) {
      (curr_err) = (new_err);
  }
)
*/

/**********************************************************************/

/*
 * Data types
 */

typedef long int_zaid_type;

typedef char mg_zaid[64];
typedef char tn_zaid[64];

enum e_avg_type { VEL_CALC, E_MEAN, SGNDF };
enum kerma_calc_type { DATA, CALC, DATA_CALC };

typedef struct _dir_entry {
    char zaid[MAX_IN_STR_LEN];
    char date[MAX_IN_STR_LEN];
    char lib[MAX_IN_STR_LEN];
    char path[MAX_IN_STR_LEN];
    char lib_type[4];
    char egrp[MAX_IN_STR_LEN];
    long int line_num;
    long int offset;
    int num_grps;
    double temp;
    double sig_0;
    double at_wgt;
    double awr;
} dir_entry;

typedef struct _status_flag {
    int length;
    long int file_ptr;
} stat_flag;

typedef struct _multi_status {
    int num_objs;
    stat_flag *stat;
} multi_stat;

typedef struct _sec_part_mg {
    int part_type;
    mg_zaid zaid;
    int num_grps;
    int pn_order;
    short collapse_flag;
    int grp_num_new;
    char n_group[MAX_NGROUP_STR_LEN];
    int *new_ebounds;
    double *collapse_ebounds;
    double *e_bounds;
    stat_flag heating;
    stat_flag kerma;
    multi_stat pn_prod_full;
} sec_part_mg;

typedef struct _interp_params {
    int npts;			/* number of x,y pairs given */
    int nint;			/* number of interpolations regions */
    int *nbt;			/* using ENDF nomenclature--the value separating 2 adj interp regions */
    int *int_type;		/* interpolation scheme: 0=discrete,2=lin-lin */
} interp_params;

enum {
    NDI_INTERP_DISCRETE = 0,
    NDI_INTERP_HISTOGRAM = 1,
    NDI_INTERP_LIN_LIN = 2,
    NDI_INTERP_LIN_LOG = 3,
    NDI_INTERP_LOG_LIN = 4,
    NDI_INTERP_LOG_LOG = 5,
    NDI_INTERP_HALE = 6
};

typedef struct _edist_data {
    double temp;
    interp_params interp;
    long int file_ptr;
} edist_data;

typedef struct _sec_part_distr {
    int_zaid_type part_type;
    interp_params temp_interp;
    edist_data *edist;
} sec_part_distr;

typedef struct _depl_rprod_struct {
    int izaid;
    int num_reac;
    int *mt;
    int *mult;
} depl_rprod_struct;

typedef struct _depl_target_struct {
    int izaid;
    int num_rprods;
    depl_rprod_struct *rprod;
} depl_target_struct;

typedef struct _multigroup_status {
    int set_flag;
    mg_zaid zaid;
    char info[MAX_IN_STR_LEN];
    char date_processed[MAX_IN_STR_LEN];
    char date_source[MAX_IN_STR_LEN];
    char library_name[MAX_IN_STR_LEN];
    int fissionable;
    int num_grps;
    int up_grps;
    int down_grps;
    int last_grp;
    int num_sec_parts;
    int num_reac;
    int pn_order;
    int *reac_mt;
    double *reac_q;
    double temp;
    double sig_0;
    double at_wgt;
    double awr;
    stat_flag e_bounds;
    stat_flag wgts;
    stat_flag vel;
    stat_flag heat;
    stat_flag kerma;
    stat_flag sig_tot;
    stat_flag nu_tot;
    stat_flag nu_pr;
    stat_flag nu_del;
    stat_flag nu_sig_f_tot;
    stat_flag nu_sig_f_pr;
    stat_flag nu_sig_f_del;
    stat_flag chi_vec_tot;
    stat_flag chi_vec_pr;
    stat_flag chi_vec_del;
    stat_flag chi_mat_tot;
    stat_flag chi_mat_pr;
    stat_flag chi_mat_del;
    stat_flag fiss_q;
    stat_flag norm;
    multi_stat pn_down_full;
    multi_stat pn_full;
    multi_stat sig_reac;
    multi_stat rprod_all;
    multi_stat rprod_rmo;
    multi_stat rprod_few;
    sec_part_mg *sec_part;
} isotope_status;

typedef struct _tn_status {
    int set_flag;
    int num_temps;
    int num_sec_parts;
    int num_grps;
    int num_edist;		/* All sec. parts. may not have E. distr. data */
    tn_zaid zaid;
    char info[MAX_IN_STR_LEN];
    char date_processed[MAX_IN_STR_LEN];
    char date_source[MAX_IN_STR_LEN];
    char library_name[MAX_IN_STR_LEN];
    int_zaid_type *sec_part_types;
    int q_set;
    double q_value;
    double *e_bounds;
    stat_flag int_sigvbars;
    stat_flag int_einbars;
    stat_flag temps;
    stat_flag sigvbars;
    stat_flag einbars;
    stat_flag rprods;
    stat_flag reactants;
    sec_part_distr *sec_part;
} tn_status;

typedef struct _tnreac_status {
    int set_flag;
    int reac_num;		/*  Number of reactions in TN reaction dataset */
    char dataset[60];		/*  Name of current set of TN reaction data */
    char info[MAX_IN_STR_LEN];	/*  Information about current TN reaction dataset */
    char date_processed[MAX_IN_STR_LEN];
    char date_source[MAX_IN_STR_LEN];
    char library_name[MAX_IN_STR_LEN];
    char *list_reac[MAX_TN_REAC];	/*  List of current set of TN reaction zaids */
    stat_flag *reactants;
    stat_flag *rprods;
} tnreac_status;

typedef struct _chain_status {
    int set_flag;		/*  Is chain set? True = -1; False= 0 */
    int num_inc_parts;		/* Number of incident particles */
    int num_reac_non_null;	/* Number of target/prod pairs where target != prod */
    int num_target;		/* Number of targets in radchem dataset */
    int num_target_unique;	/* Number of unique targets in radchem dataset */
    int *inc_part_types;	/* Incident particles types */
    int target_filled;		/* Number of targets for which data has been stored */
    char dataset[60];		/*  Name of current set of chain data */
    char info[MAX_IN_STR_LEN];	/*  Information about current chain dataset */
    char date_processed[MAX_IN_STR_LEN];
    char date_source[MAX_IN_STR_LEN];
    char library_name[MAX_IN_STR_LEN];
    depl_target_struct *target;
} chain_status;

typedef struct _state_struct {
    short ndi_initialized;	/*  ndi_init called: 0=false, 1=true */
    int handle;
    FILE *ndi_dir;		/*  Gendir (directory) file */
    char *open_gendir_file;	/*  Current gendir file name */
    FILE *lib_stream;		/*  Current library file */
    char ndi_gendir_path[MAX_IN_PATH_LEN]; /* Path for open gendir file */
    char *open_lib_file; /* Path for open library file */
    short *ndi_elements;	/*  Lookup table for element symbols */
    char *library_vers;		/*  Default library type (mendf5,mendf6,rmoxsct,"") */
    int particle_type;		/*  Incident particle type: 1-neutron, 0-photon, -1-tn */
    char library_type;		/*  m-multigroup */
    char mat_order;		/*  Transpose return matrices? 
				   d=no(default), t=yes(transpose) */
    char reac_id;		/*  Reaction identifier: m=MT (default), c=c-value */
    enum e_avg_type e_avg_calc;	/* VEL_CALC=> e = 1/2 m vel^2; E_MEAN=> mean of boundaries */
    enum kerma_calc_type kerma_calc;	/* CALC=>calculate kerma; DATA=>get data from library
					   DATA_CALC=> get data from lib; use calc if not avail */
    int curr_reac_index;	/*  Obsolete? */
    char nuc_data_type[2];	/*  Obsolete? */

    /*    GENERAL STATE VARIABLES -- initialized when lib_type changes */
    int curr_part_index;	/*  index of current particle */
    char n_group[MAX_NGROUP_STR_LEN];	/*  Type of group structure to collapse to */
    int grp_num_new;		/*  Number of groups to collapse to */
    double *collapse_ebounds;	/* User-set boundaries--used with TN or user-defined for MG */

    /* Multigroup data */
    isotope_status iso_stat;	/*  Struct containing isotope state */
    int *new_ebounds;		/*  Index of new ebound group structure, 
				   allocated in ndi_set_e_grp  */
    short collapse_flag;	/*  Collapsing required? 0=no; -1=yes  */
    ndi_float64 *lib_weight;	/*  Array of weights read in for current nuc */
    ndi_float64 *lib_ebounds;	/*  Array of ebounds read in for current nuc */
    char trans_corr;		/*  Transport correction: " "=no correction, 
				   d=diagonal, b=Bell-Hansen-Sandmeier */
    int trans_corr_order;	/*  Transport correction order */
    int no_norm;		/*  Normalize chi's and edists: 0=normalize; -1=do not normalize */
    char def_fiss_prod_id[20];	/* Value of default fiss. prod. ZAID  */

    /* TN data */
    tn_status tn_stat;
    int tn_num_grps;
    int sec_part_filled;	/*  Number of secondary particles with edist */
    double default_temp;

    /* TN reaction data */
    tnreac_status tnreac_stat;

    /* Chain data */
    chain_status chain_stat;

    /* TN reaction data */
    char tn_dataset[60];	/* Name of current set of TN reaction data */
    char tn_reac_info[MAX_IN_STR_LEN];	/* Current TN reaction dataset info */
    int tn_reac_num;		/* Number of TN reactions in dataset */
    char *tn_list_reac[MAX_TN_REAC];	/* List of current TN reaction zaids */
    stat_flag *tn_reactants;
    stat_flag *tn_rprods;

    /* Depletion data */
    int ndi_depl_set;		/* Is depletion set? True = -1; False= 0 */
    char depl_dataset[60];	/* Name of current set of depletion data */
    char depl_info[MAX_IN_STR_LEN];	/* current depletion dataset info */
    int num_target;		/* Number of targets in depletion dataset */
    int chain_filled;		/* Number of read depletion targets */

    depl_target_struct *depl_target;

    struct _state_struct *handle_map[MAX_HANDLE_NUM];
} state_struct;

/**********************************************************************/

/*
 * Function prototypes
 */

#ifdef	__cplusplus
extern "C" {
#endif

    /* Handle function prototypes */
    int ndi_c_init_h(state_struct **curr_state, const char *path);
    int ndi_c_clone_h(state_struct *parent, state_struct **child);
    int ndi_c_close_h(state_struct **curr_state);
    int ndi_c_set_zaid_h(state_struct *state, const char *in_zaid);
    int ndi_c_set_isotope_h(state_struct *state, const char *in_zaid);
    int ndi_c_set_reaction_h(state_struct *state, const char *in_zaid);
    int ndi_c_get_size_h(state_struct *state, int in_key, int *ierr);
    int ndi_c_get_size_n_h(state_struct *state, int in_key, int vec_num,
			   int *ierr);
    int ndi_c_get_size_x_h(state_struct *state, int in_key,
			   const char *in_str, int *ierr);
    int ndi_c_set_option_h(state_struct *state, int in_key,
			   const char *in_value);
    int ndi_c_set_float64_vec_option_h(state_struct *state,
				       const int in_key,
				       const ndi_float64 *option_array,
				       const int array_size);
    int ndi_c_check_option_h(state_struct *state, int in_key,
			     char *out_value, int string_size);
    int ndi_c_get_int_val_h(state_struct *state, int in_key,
			    int *ret_val);
    int ndi_c_get_int_val_n_h(state_struct *state, int in_key,
			      int vec_num, int *ret_val);
    int ndi_c_get_int_vec_h(state_struct *state, int in_key,
			    int *return_vec, int vec_size);
    int ndi_c_get_int_vec_n_h(state_struct *state, int in_key,
			      int vec_num, int *return_vec, int vec_size);
    int ndi_c_get_float32_val_h(state_struct *state, int in_key,
				ndi_float32 *ret_val);
    int ndi_c_get_float32_val_n_h(state_struct *state, int in_key,
				  int vec_num, ndi_float32 *ret_val);
    int ndi_c_get_float32_vec_h(state_struct *state, int in_key,
				ndi_float32 *return_vec, int return_size);
    int ndi_c_get_float32_vec_n_h(state_struct *state, int in_key,
				  int vec_num, ndi_float32 *return_vec,
				  int return_size);
    int ndi_c_get_float32_vec_x_h(state_struct *state, int in_key,
				  const char *in_str,
				  ndi_float32 *return_vec, int vec_size);
    int ndi_c_get_float64_val_h(state_struct *state, int in_key,
				ndi_float64 *ret_val);
    int ndi_c_get_float64_val_n_h(state_struct *state, int in_key,
				  int vec_num, ndi_float64 *ret_val);
    int ndi_c_get_float64_vec_h(state_struct *state, int in_key,
				ndi_float64 *return_vec, int return_size);
    int ndi_c_get_float64_vec_n_h(state_struct *state, int in_key,
				  int vec_num, ndi_float64 *return_vec,
				  int return_size);
    int ndi_c_get_float64_vec_x_h(state_struct *state, int in_key,
				  const char *in_str,
				  ndi_float64 *return_vec, int vec_size);
    int ndi_c_get_string_val_h();
    int ndi_c_get_string_val_n_h();

    int ndi_set0(state_struct *state, char l_type);
    /*    int ndi_set0_tn_sec_part ( sec_part_distr *part ); */
    int ndi_set0_mg_sec_part(sec_part_mg *part);
    int ndi_set0_tn_sec_part(sec_part_distr *part, int reset_flag);
    int ndi_zaid_dir(state_struct *state, int pt_type, char l_type,
		     const char *in_zaid, char *real_zaid, char *library,
		     char *lib_path, char *lib_file_type,
		     long int *iso_start, double *at_wgt,
		     int reaction_flag);
    int ndi_close_lib_file(state_struct *state);
    FILE *ndi_open_lib_file(state_struct *state, const char *path);
    int ndi_open_gendir(FILE **ndi_dir, char *path);
    int ndi_init_iso_state(state_struct *state, FILE *stream,
			   char lib_type, long int iso_start,
			   const char *zaid);
    int ndi_std_zaid(state_struct *state, char *in_zaid, int zaid_size);
    int ndi_fill_elements(state_struct *state);
    int element_index(const char *el_symbol);
    int ndi_set_e_grps(const int old_grp, double *old_grp_bounds,
		       int *new_grp, double **new_grp_bounds,
		       const char *grp_struc, char library_type,
		       int **new_ebound_index, short *c_flag);
/*
   int ndi_set_e_grps( state_struct *state, int **new_ebound_p, const char *grp_struc, int *new_grp,
		       int old_grp, short *c_flag);
*/
    int ndi_get_old_kerma64(state_struct *state, ndi_float64 *return_vec,
			    int return_size);
    int ndi_get_old_kerma32(state_struct *state, ndi_float32 *return_vec,
			    int return_size);
    int ndi_collapse_float32(state_struct *state,
			     const ndi_float32 *old_vec, int old_grp_num,
			     int recip_flag, ndi_float32 *return_vec,
			     int return_size);
    int ndi_collapse_float64(state_struct *state,
			     const ndi_float64 *old_vec, int old_grp_num,
			     int recip_flag, ndi_float64 *return_vec,
			     int return_size);
    int ndi_collapse_mat32(const FLOAT_TYPE *old_mat, int old_grp_num_in,
			   int old_grp_num_out, int new_grp_num_in,
			   int new_grp_num_out, const int *new_ebounds_in,
			   const int *new_ebounds_out,
			   const ndi_float64 *local_weight,
			   FLOAT_TYPE *new_mat, int new_size);

    int ndi_collapse_mat64(const FLOAT_TYPE *old_mat, int old_grp_num_in,
			   int old_grp_num_out, int new_grp_num_in,
			   int new_grp_num_out, const int *new_ebounds_in,
			   const int *new_ebounds_out,
			   const ndi_float64 *local_weight,
			   FLOAT_TYPE *new_mat, int new_size);
    int skip_float_block(FILE *stream, int *bsize, const int num_floats);
    int ndi_read_float32_block(FILE *stream, long file_pos,
			       const int num_floats,
			       ndi_float32 *return_vec,
			       const int return_size);
    int ndi_read_float64_block(FILE *stream, long file_pos,
			       const int num_floats,
			       ndi_float64 *return_vec,
			       const int return_size);
    int ndi_skip_rx_prod(FILE *stream, stat_flag *rx_prod,
			 short mt_flag);
    int ndi_read_rx_prod(FILE *stream, long file_pos, int **int_vec,
			 const int data_type, int *num_prod,
			 short mt_flag);
    int ndi_read_string_block(FILE *stream, const int num_strings,
			      char **p);
    int read_dir_block(char *in_line, FILE *dir_stream,
		       dir_entry *dir_rec);
    int ndi_read_int_block(FILE *stream, const int num_ints,
			   long *return_vec, const int return_size);
    void *ndi_malloc(size_t size);
    void *ndi_calloc(size_t nmemb, size_t size);
    void ndi_free(void **p);
    int ndi_set_tn_dataset(const char *dataset);
    int ndi_get_prod_reac32(state_struct *state, const char *rprod,
			    ndi_float32 *return_vec, int vec_size);
    int ndi_get_prod_reac64(state_struct *state, const char *rprod,
			    ndi_float64 *return_vec, int vec_size);


   /***  Subroutines in file ndi_init_state.c ***/
    int ndi_init_state_common(state_struct *state);
    int ndi_init_state_iso_state(state_struct *state);
    int ndi_init_state_tn(state_struct *state);
    int ndi_init_state_tnreac(state_struct *state);
    int ndi_init_state_chain(state_struct *state);

   /***  Subroutines in file ndi_misc.c ***/
    int ndi_init_int_vec(int *vec, int num_elements);
    int ndi_comp_zaids(const void *z1, const void *z2);
    int ndi_free_ptr_array(int array_size, char **array);
    int ndi_get_rprod_int_n(state_struct *state, int vec_num,
			    int *rprod_id);
    int ndi_get_rprod_vec_mg(state_struct *state, int *rprod_vec,
			     int vec_size);
    int ndi_get_rprod_id_n(state_struct *state, int vec_num,
			   char *rprod_id, int str_size);
    int ndi_check_array_err(int num);
    int ndi_read_float_pair(FILE *stream, long int file_pos, int num_prs,
			    double *flt_vecx, double *flt_vecy,
			    int return_size);
    int ndi_translate_fiss_prod(state_struct *state, int *zaid_list,
				int num_zaids);
    int ndi_parse_mg_zaid(char *zaid, int *z, int *a);
    int double_compare(const double a, const double b);
    int ndi_empty_string(char *in_str);
    void check_fissionable_err(int *curr_err, int fission_flag);
    int ndi_get_nth_inc_part_type(state_struct *state, int vec_num,
				  char *in_str, const int str_size);
    int ndi_use_sgndf_values(state_struct *state, int num_grps);


  /***  Subroutines in file ndi_misc_depl.c ***/
    int ndi_depletion(state_struct *state, const char *in_zaid);
    int ndi_radchem(state_struct *state, const char *in_zaid);
    int ndi_set_default_depl(state_struct *state);
    int ndi_set_curr_target(state_struct *state,
			    depl_target_struct **targ_p,
			    int *target_index);
    int ndi_set_curr_rprod(state_struct *state,
			   depl_rprod_struct **rprod_p, const char *rprod,
			   int *prod_index);

    /***  Subroutines in file ndi_misc_float.c ***/
    int ndi_init_float32_vec(ndi_float32 *vec, int num_elements);
    int ndi_init_float64_vec(ndi_float64 *vec, int num_elements);
    int ndi_get_simple_float32_vec(state_struct *state, FILE *stream,
				   long int f_ptr, int collapse_type,
				   int library_grp_num,
				   ndi_float32 *return_vec,
				   int return_size);
    int ndi_get_simple_float64_vec(state_struct *state, FILE *stream,
				   long int f_ptr, int collapse_type,
				   int library_grp_num,
				   ndi_float64 *return_vec,
				   int return_size);
    int ndi_transpose_float32(ndi_float32 *in_mat, int row_num,
			      int col_num);
    int ndi_transpose_float64(ndi_float64 *in_mat, int row_num,
			      int col_num);
    int ndi_vec2mat32(const ndi_float32 *vec, int num_vec,
		      ndi_float32 *return_mat, int num_mat, int mat_size);
    int ndi_vec2mat64(const ndi_float64 *vec, int num_vec,
		      ndi_float64 *return_mat, int num_mat, int mat_size);
    int ndi_get_sig_reac32(state_struct *state, isotope_status *mg,
			   int mt, ndi_float32 *return_vec,
			   int *return_size);
    int ndi_get_sig_reac64(state_struct *state, isotope_status *mg,
			   int mt, ndi_float64 *return_vec,
			   int *return_size);
    int ndi_copy_matrix32(ndi_float32 *dest, FLOAT_TYPE *source,
			  int num);
    int ndi_copy_matrix64(ndi_float64 *dest, FLOAT_TYPE *source,
			  int num);
    int ndi_mult_float32_vec(state_struct *state,
			     const ndi_float64 *vec1,
			     const ndi_float64 *vec2, int num_elements,
			     int collapse_flag, ndi_float32 *return_vec,
			     int vec_size);
    int ndi_mult_float64_vec(state_struct *state,
			     const ndi_float64 *vec1,
			     const ndi_float64 *vec2, int num_elements,
			     int collapse_flag, ndi_float64 *return_vec,
			     int vec_size);
    int ndi_divide_float32_vec(state_struct *state,
			       const ndi_float64 *vec1,
			       const ndi_float64 *vec2, int num_elements,
			       int collapse, ndi_float32 *return_vec,
			       int return_size);
    int ndi_divide_float64_vec(state_struct *state,
			       const ndi_float64 *vec1,
			       const ndi_float64 *vec2, int num_elements,
			       int collapse, ndi_float64 *return_vec,
			       int return_size);
    int ndi_norm_float32_vec(ndi_float32 *vec_p, int num_elements,
			     const ndi_float32 norm_val);
    int ndi_norm_float64_vec(ndi_float64 *vec_p, int num_elements,
			     const ndi_float64 norm_val);
    int ndi_get_abundance32(int izaid, ndi_float32 *return_val);
    int ndi_get_abundance64(int izaid, ndi_float64 *return_val);
    int ndi_chi_mat_to_vec32(state_struct *state, FILE *stream,
			     long int f_ptr, ndi_float32 *return_vec,
			     int return_size, int collapse_type,
			     int num_grp);
    int ndi_chi_mat_to_vec64(state_struct *state, FILE *stream,
			     long int f_ptr, ndi_float64 *return_vec,
			     int return_size, int collapse_type,
			     int num_grp);

   /*** Subroutines in file ndi_find_index.c ***/
    int ndi_find_rprod_index(FILE *stream, multi_stat *rprod,
			     int *ret_index, int mt);
    int ndi_find_reac_index(int *mt_array, int *ret_index, int mt,
			    int num);
    int ndi_find_tn_part_index(const long ptype,
			       const sec_part_distr *part, int num_parts);
    int ndi_find_mg_part_index(const int ptype, const sec_part_mg *part,
			       int num_parts);
    int ndi_find_iso_index(const char *ptype, const char **string_array,
			   int num_parts);
    int ndi_find_temp_bound_index(double default_temp, edist_data *edist,
				  int num_temps, int *hi, int *lo,
				  int *closest);


  /*** Subroutines in file ndi_avail.c ***/
    int ndi_nusigf_avail(state_struct *state, char nu_type);
    int ndi_nu_avail(state_struct *state, char nu_type);
    int ndi_sigf_avail(state_struct *state);

    /*** Subroutines in file ndi_parse_zaid.c ***/
    int ndi_parse_str_zaid(state_struct *state, char *zaid_str, int *Z,
			   int *A, int *S);
    int ndi_parse_int_zaid(int izaid, int *Z, int *A, int *S);

    /***Subroutines in file ndi_sza_symbol_translations.c ***/
    int ndi_translate_sza_to_symbol(int sza, char *sym_str, int str_size);

     /*** Subroutines in file ndi_handle.c ***/
    state_struct *handle_to_state_p(int handle);
    int set_handle(state_struct *state);
    int __ndi_close_handle(int handle);

   /*** Subroutines in file ndi_interp_integ.c ***/
    int ndi_integrate_bins(double *distr_x, double *distr_y,
			   interp_params *interp, double *bin_bound,
			   int num_bins, double *return_vec,
			   int return_size);
    int find_average_E(double *x, double *y, interp_params *iparm,
		       double *ret_val);
    int new_integrate(double *x, double *y, int npts, int interp_type,
		      int curr_x_index, double xstart, double xend,
		      double *return_val);
    int new_integrate_xy(double *x, double *y, int npts, int interp_type,
			 int curr_x_index, double xstart, double xend,
			 double *return_val);
    int ndi_init_double_vec(double *vec, int num_elements);
    int ndi_find_energy_bin_index(double in_energy, double *energy_vec,
				  int num_grps, int *eindex);
    int integrate2(double xstart, double xend, double *x, double *y,
		   interp_params *interp, double *ret_val, int int_flag);
    double integrate(int int_type, double x1, double y1, double x2,
		     double y2);
    double histogram_interp(double xin, double x1, double y1, double x2,
			    double y2);
    double lin_lin_interp(double xin, double x1, double y1, double x2,
			  double y2);
    double log_log_interp(double xin, double x1, double y1, double x2,
			  double y2);
    double lin_log_interp(double xin, double x1, double y1, double x2,
			  double y2);
    double log_lin_interp(double xin, double x1, double y1, double x2,
			  double y2);
    double hale_interp(double xin, double x1, double y1, double x2,
		       double y2, double x3, double y3);
    double histogram_integrate(double xstart, double xend, double *x,
			       double *y);
    double lin_lin_integrate(double xstart, double xend, double *x,
			     double *y);
    double log_log_integrate(double xstart, double xend, double *x,
			     double *y);
    double lin_log_integrate(double xstart, double xend, double *x,
			     double *y);
    double log_lin_integrate(double xstart, double xend, double *x,
			     double *y);
    double hale_integrate(double xstart, double xend, double *x,
			  double *y);
    double discrete_integrate(double xstart, double xend, double *x,
			      double *y);
    double histogram_integrate_x(double xstart, double xend, double *x,
				 double *y);
    double lin_lin_integrate_x(double xstart, double xend, double *x,
			       double *y);
    double log_log_integrate_x(double xstart, double xend, double *x,
			       double *y);
    double lin_log_integrate_x(double xstart, double xend, double *x,
			       double *y);
    double log_lin_integrate_x(double xstart, double xend, double *x,
			       double *y);
    double hale_integrate_x(double xstart, double xend, double *x,
			    double *y);
    double discrete_integrate_x(double xstart, double xend, double *x,
				double *y);


  /**********************************************************************/


#endif				/* __NDI_INTERNAL_H */
