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

#define TN_DATASET(i)       ((char *) ( (state->tn_list_reac) + (i) *  MAX_TN_REAC_LEN ))
#define MAX_HANDLE_NUM  10

/**********************************************************************/

     extern state_struct *default_h;

#ifdef	__cplusplus
}
#endif

#endif	/* __NDI_STATE_H */
