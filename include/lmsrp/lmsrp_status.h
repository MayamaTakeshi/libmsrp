/*
 * lmsrp_status.h
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#ifndef LMSRP_STATUS_H_
#define LMSRP_STATUS_H_
#include <pjlib.h>
typedef struct _lmsrp_status {
	pj_pool_t *pool;
	int code;
	int rfc_code;
	pj_str_t reason;
} lmsrp_status_h;

void lmsrp_status_h_prase(pj_pool_t *pool, lmsrp_status_h *header, char *data,
		int end);
lmsrp_status_h* lmsrp_status_h_create(pj_pool_t *pool, int code, int rfcode,
		pj_str_t *reason) ;
#endif /* LMSRP_STATUS_H_ */
