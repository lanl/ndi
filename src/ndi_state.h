/*
 * File:	ndi_state.h
 * Description:	Header file for libndi
 * Time-stamp:	<2000-02-03 11:34:05 jomc>
 *
 * This header file is included in library code only.
 */

/**********************************************************************/

#ifndef __NDI_STATE_H
#define __NDI_STATE_H

#ifdef	__cplusplus
extern "C" {
#endif

/**********************************************************************/
/*
 *    Macros
 */

#define TN_DATASET(i)       ((char *) (  tn_list_reac + (i) *  MAX_TN_REAC_LEN ))

/**********************************************************************/

/*    GENERAL STATE VARIABLES -- not changed when lib_type changes */
extern short ndi_initialized;
extern FILE *ndi_dir;
extern FILE *lib_stream;
extern char *open_lib_file;
extern short *ndi_elements;
extern char *library_vers;
extern char particle_type;
extern char library_type;
extern char mat_order;
extern char reac_id;               /*  Reaction identifier: m=MT (default), c=c-value */
extern int curr_reac_index;        /*  Obsolete? */
extern char nuc_data_type[2];      /*  Obsolete? */

/*    GENERAL STATE VARIABLES -- initialized when lib_type changes */
extern int curr_part_index;        /* index of current particle */
extern char n_group[MAX_NGROUP_STR_LEN];        /*  Type of group structure to collapse to */
extern int grp_num_new;            /*  Number of groups to collapse to */


/* Multigroup data */
extern isotope_status iso_stat;
extern int *new_ebounds;           /*  Index of new ebound group structure */
extern short collapse_flag;        /*  Collapsing required?  0=no; -1=yes */
extern ndi_float64 *lib_weight;    /* Array of weights read in for current nuc */
extern ndi_float64 *lib_ebounds;   /* Array of ebounds read in for current nuc */
extern char trans_corr;            /* Transport correction: " "=no correction, 
			                 d=diagonal, b=Bell-Hansen-Sandmeier */
extern int trans_corr_order;       /* Transport correction order */
extern int no_norm;                /* Normalize chi's and edists: 0=normalize; -1=do not normalize */
extern char def_fiss_prod_id[20];  /* Value of default fiss. prod. ZAID  */

/* TN data */
extern tn_status tn_stat;
extern int tn_num_grps;
extern int sec_part_filled;         /*  Number of secondary particles which have edist data */
extern double *tn_ebounds;
extern double default_temp;

/* TN reaction data */
extern tnreac_status tnreac_stat;

/* Chain data */
extern chain_status chain_stat;

extern char tn_dataset[60];         /* Name of current set of TN reaction data */
extern char tn_reac_info [132 + 1]; /* Information about current TN reaction dataset */
extern int tn_reac_num;             /* Number of reactions in TN reaction dataset */
extern int sec_part_filled;         /* Number of secondary particles which have edist data */
extern char *tn_list_reac[ MAX_TN_REAC ];
extern stat_flag *tn_reactants;
extern stat_flag *tn_rprods;

/* Depletion data */
extern int ndi_depl_set;                /* Is depletion set? True = -1; False= 0 */
extern char depl_dataset[60];           /* Name of current set of depletion data */
extern char depl_info [132 + 1];        /* Information about current depletion dataset */
extern int num_target;                  /* Number of reactions in depletion dataset */
extern int chain_filled;                /* Number of depletion targets whose data has been read */

extern depl_target_struct *depl_target;

#ifdef	__cplusplus
}
#endif

#endif	/* __NDI_STATE_H */
