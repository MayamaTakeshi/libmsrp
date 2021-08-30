/*
 * lmsrp_auth_commom.h
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#ifndef LMSRP_AUTH_COMMOM_H_
#define LMSRP_AUTH_COMMOM_H_

#include <lmsrp.h>
typedef void (lmsrp_auth_set_pro)(void *header, pj_str_t *name, pj_str_t *value);
typedef int (lmsrp_auth_header_property)(header_property *pro, int size,
		pj_str_t *in);

typedef struct _lmsrp_auth_common_header {
	pj_pool_t *pool;
} lmsrp_auth_common_header;
enum lmsrp_auth_header_pro {
	lmsrp_auth_header_realm = 0,
	lmsrp_auth_header_nonce,
	lmsrp_auth_header_cnonce,
	lmsrp_auth_header_qop,
	lmsrp_auth_header_opaque,
	lmsrp_auth_header_algorithm,
	lmsrp_auth_header_domain,
	lmsrp_auth_header_scheme,
	lmsrp_auth_header_nc,
	lmsrp_auth_header_response,
	lmsrp_auth_header_uri,
	lmsrp_auth_header_username,
	lmsrp_auth_header_unknow
};
int lmsrp_find_header_property(const header_property *pro, const int size, pj_str_t *in);

int lmsrp_header_check_equal(char *data, int end, int *result);
void lmsrp_auth_common_prase(pj_pool_t *pool, lmsrp_auth_common_header *dst,
		lmsrp_check *check, lmsrp_auth_set_pro *set_pro,int size , char *data, int end);

#endif /* LMSRP_AUTH_COMMOM_H_ */
