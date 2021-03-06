/*
 * lmsrp_authorization.h
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#ifndef LMSRP_AUTHORIZATION_H_
#define LMSRP_AUTHORIZATION_H_
#include <lmsrp.h>

typedef struct _lmsrp_header_authorization {
	pj_pool_t *pool;
	pj_str_t scheme;
	pj_str_t username;
	pj_str_t realm;
	pj_str_t nonce;
	pj_str_t uri;
	pj_str_t response;
	pj_str_t algorithm;
	pj_str_t cnonce;
	pj_str_t opaque;
	pj_str_t qop;
	pj_str_t nc;
	lmsrp_list_param *params;
} lmsrp_authorization_h;

void lmsrp_authorization_header_prase(pj_pool_t *pool,
		lmsrp_authorization_h *dst, char *data, int end);
int lmsrp_authorization_header_to_string(lmsrp_authorization_h *header,
		char *buff, int size);
#endif /* LMSRP_AUTHORIZATION_H_ */
