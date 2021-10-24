/*
 * lmsrp_www_auth.h
 *
 *  Created on: Mar 3, 2021
 *      Author: amneiht
 */

#ifndef LMSRP_WWW_AUTH_H_
#define LMSRP_WWW_AUTH_H_

#include <pjlib.h>
typedef struct _lmsrp_www_h {
	pj_pool_t *pool;
	pj_str_t scheme;
	pj_str_t realm;
	pj_str_t domain;
	pj_str_t nonce;
	pj_str_t opaque;
	pj_str_t algorithm;
	pj_str_t qop;
	lmsrp_list_param *params;
} lmsrp_www_h;

void lmsrp_www_h_prase(pj_pool_t *pool, lmsrp_www_h *dst, char *data, int end);
int lmsrp_www_h_to_string(lmsrp_www_h *header, char *buff, int size);
#endif /* LMSRP_WWW_AUTH_H_ */
