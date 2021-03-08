/*
 * lmsrp_authorization.c
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>

static const header_property auth_msrp[] = {
		{ { "realm", 5 }, lmsrp_auth_header_realm }, //
		{ { "nonce", 5 }, lmsrp_auth_header_nonce }, //
		{ { "opaque", 6 }, lmsrp_auth_header_opaque }, //
		{ { "algorithm", 9 }, lmsrp_auth_header_algorithm }, //
		{ { "uri", 3 }, lmsrp_auth_header_uri }, //
		{ { "Digest", 6 }, lmsrp_auth_header_scheme }, //
		{ { "qop", 3 }, lmsrp_auth_header_qop }, //
		{ { "username", 8 }, lmsrp_auth_header_username }, //
		{ { "nc", 2 }, lmsrp_auth_header_nc }, //
		{ { "cnonce", 6 }, lmsrp_auth_header_cnonce }, //
		{ { "response", 8 }, lmsrp_auth_header_response } //
};
void lmsrp_authorization_set_pro(void *header, pj_str_t *name, pj_str_t *value) {
	lmsrp_authorization_h *dst = header;
	static const int ls = sizeof(pj_str_t);
	if (name == NULL || name->slen == 0)
		return;
	static const int auth_leng = sizeof(auth_msrp) / sizeof(header_property);
	int st = lmsrp_find_header_property(auth_msrp, auth_leng, name);
	switch (st) {
	case lmsrp_auth_header_realm:
		pj_memcpy(&dst->realm, value, ls);
		break;
	case lmsrp_auth_header_nonce:
		pj_memcpy(&dst->nonce, value, ls);
		break;
	case lmsrp_auth_header_opaque:
		pj_memcpy(&dst->opaque, value, ls);
		break;
	case lmsrp_auth_header_algorithm:
		pj_memcpy(&dst->algorithm, value, ls);
		break;
	case lmsrp_auth_header_uri:
		pj_memcpy(&dst->uri, value, ls);
		break;
	case lmsrp_auth_header_scheme:
		pj_memcpy(&dst->scheme, name, ls);
		break;
	case lmsrp_auth_header_qop:
		pj_memcpy(&dst->qop, value, ls);
		break;
	case lmsrp_auth_header_username:
		pj_memcpy(&dst->username, value, ls);
		break;
	case lmsrp_auth_header_nc:
		pj_memcpy(&dst->nc, value, ls);
		break;
	case lmsrp_auth_header_cnonce:
		pj_memcpy(&dst->cnonce, value, ls);
		break;
	case lmsrp_auth_header_response:
		pj_memcpy(&dst->response, value, ls);
		break;
	default:
		;
		lmsrp_list_param *listp;
		listp = pj_pool_alloc(dst->pool, sizeof(lmsrp_list_param));
		if (dst->params == NULL) {
			pj_list_init(listp);
			dst->params = listp;
		} else {
			pj_list_insert_after(dst->params, listp);
		}
		if (value == NULL || value->slen == 0) {
			pj_bzero(&listp->var.value, ls);
		} else {
			pj_memcpy(&listp->var.value, value, ls);
		}
		pj_memcpy(&listp->var.name, name, ls);
		break;
	}
	return;
}
static char split[] = { ',', ' ', '=' };
static char sprc = '"';
static int lmsrp_auth_check(char d, void *arg) {
	int *sp = arg;
	if (d == sprc) {
		*sp = 1 - (*sp); // dao dau
		return 1;
	}
	if (*sp > 0)
		return 0;
	int dem = sizeof(split);
	for (int i = 0; i < dem; i++) {
		if (split[i] == d) {
			return 1;
		}
	}
	return 0;
}

void lmsrp_authorization_header_prase(pj_pool_t *pool,
		lmsrp_authorization_h *dst, char *data, int end) {
	lmsrp_auth_common_prase(pool, (lmsrp_auth_common_header*) dst,
			&lmsrp_auth_check, &lmsrp_authorization_set_pro,
			sizeof(lmsrp_authorization_h), data, end);
}
