
/*
 * File:	ndi_state.c
 * Description:	Variable declarations-internal use.
 * Time-stamp:	<2000-02-03 11:32:51 jomc>
 *
 */

/**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ndi.h"
#include "ndi_internal.h"

/**********************************************************************/

/*    GENERAL STATE VARIABLES -- not changed when lib_type changes */
short ndi_initialized = 0;  /*  ndi_init called: 0=false, 1=true*/
FILE *ndi_dir;              /*  Gendir (directory) file */
FILE *lib_stream;           /*  Current library file */
char *open_lib_file;        /*  Path for current (open) library file */
short *ndi_elements;        /*  Lookup table for element symbols */
char *library_vers;         /*  Default library type (mendf5,mendf6,rmoxsct,"") */
char particle_type = ' ';   /*  Incident particle type: n-neutron, p-photon,0-tn */
char library_type = ' ';    /*  m-multigroup */
char mat_order;             /*  Transpose return matrices? 
			              d=no(default), t=yes(transpose) */
char reac_id;               /*  Reaction identifier: m=MT (default), c=c-value */
int curr_reac_index;        /*  Obsolete? */
char nuc_data_type[2];      /*  Obsolete? */

/*    GENERAL STATE VARIABLES -- initialized when lib_type changes */
int curr_part_index;        /*  index of current particle */
char n_group[MAX_NGROUP_STR_LEN];  /*  Type of group structure to collapse to */
int grp_num_new;            /*  Number of groups to collapse to */


/* Multigroup data */
isotope_status iso_stat;    /*  Struct containing isotope state */
int *new_ebounds;           /*  Index of new ebound group structure, 
			              allocated in ndi_set_e_grp  */
short collapse_flag;        /*  Collapsing required? 0=no; -1=yes  */
ndi_float64 *lib_weight;    /*  Array of weights read in for current nuc */
ndi_float64 *lib_ebounds;   /*  Array of ebounds read in for current nuc */
char trans_corr;            /*  Transport correction: " "=no correction, 
			              d=diagonal, b=Bell-Hansen-Sandmeier */
int trans_corr_order;       /*  Transport correction order */
int no_norm;                /*  Normalize chi's and edists: 0=normalize; -1=do not normalize */
char def_fiss_prod_id[20];  /* Value of default fiss. prod. ZAID  */


/* TN data */
tn_status tn_stat;
int tn_num_grps;
int sec_part_filled;             /*  Number of secondary particles which have edist data */
double *tn_ebounds;
double default_temp;

/* TN reaction data */
tnreac_status tnreac_stat;

/* Chain data */
chain_status chain_stat;

/* TN reaction data */
char tn_dataset[60];             /* Name of current set of TN reaction data */
char tn_reac_info [MAX_IN_STR_LEN]; /* Information about current TN reaction dataset */
int tn_reac_num;                 /* Number of reactions in TN reaction dataset */
int sec_part_filled;             /* Number of secondary particles which have edist data */
char *tn_list_reac[ MAX_TN_REAC ];  /* List of current set of TN reaction zaids */
stat_flag *tn_reactants;
stat_flag *tn_rprods;

/* Depletion data */
int ndi_depl_set;                /* Is depletion set? True = -1; False= 0 */
char depl_dataset[60];           /* Name of current set of depletion data */
char depl_info [MAX_IN_STR_LEN];        /* Information about current depletion dataset */
int num_target;                  /* Number of targets in depletion dataset */
int chain_filled;                /* Number of depletion targets whose data has been read */

depl_target_struct *depl_target = NULL; 

