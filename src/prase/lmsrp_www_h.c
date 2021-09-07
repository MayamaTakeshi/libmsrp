/*
 * lmsrp_www_h.c
 *
 *  Created on: Mar 3, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>
static char split[] = { ',', ' ', '=' };
static char sprc = '"';

static const header_property pro[] = { //
		{ { "realm", 5 }, lmsrp_auth_header_realm }, //
				{ { "nonce", 5 }, lmsrp_auth_header_nonce }, //
				{ { "opaque", 6 }, lmsrp_auth_header_opaque }, //
				{ { "algorithm", 9 }, lmsrp_auth_header_algorithm }, //
				{ { "domain", 6 }, lmsrp_auth_header_domain }, //
				{ { "Digest", 6 }, lmsrp_auth_header_scheme }, //
				{ { "qop", 3 }, lmsrp_auth_header_qop } //

		};
int lmsrp_find_header_property(const header_property *list, const int size,
		pj_str_t *in) {
	for (int i = 0; i < size; i++) {
		if (pj_stricmp(&(list[i].name), in) == 0) {
			return list[i].pro;
		}
	}
	return -1;
}

static int lmsrp_www_check(char d, void *arg) {
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
/**
 * kiem tra neu co ki tu '=' trong chuoi
 * @param data
 * @param end
 * @param result 1 hoac khong
 * @return so byte da doc
 */
int lmsrp_header_check_equal(char *data, int end, int *result) {
	int i = 0;
	*result = 0;
	while (1) {
		if (i >= end)
			return -1;
		if (data[i] != ' ' && data[i] != ',') {
			if (data[i] == '=') {
				*result = 1;
			} else {
				return i;
			}
		}
		i++;
	}
}
static void _lmsrp_set_pro(void *data, pj_str_t *name, pj_str_t *value) {
	lmsrp_www_h *dst = data;
	static int ls = sizeof(pj_str_t);
	if (name == NULL || name->slen == 0)
		return;
	static const int wwwl = sizeof(pro) / sizeof(header_property);
	int st = lmsrp_find_header_property(pro, wwwl, name);
	switch (st) {
	case lmsrp_auth_header_realm:
		pj_memcpy(&dst->realm, value, ls);
		break;
	case lmsrp_auth_header_nonce:
		pj_memcpy(&dst->nonce, value, ls);
		break;
	case lmsrp_auth_header_qop:
		pj_memcpy(&dst->qop, value, ls);
		break;
	case lmsrp_auth_header_scheme:
		pj_memcpy(&dst->scheme, name, ls);
		break;
	case lmsrp_auth_header_algorithm:
		pj_memcpy(&dst->algorithm, value, ls);
		break;
	case lmsrp_auth_header_domain:
		pj_memcpy(&dst->domain, value, ls);
		break;
	case lmsrp_auth_header_opaque:
		pj_memcpy(&dst->opaque, value, ls);
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
void lmsrp_www_h_prase(pj_pool_t *pool, lmsrp_www_h *dst, char *data, int end) {
	lmsrp_auth_common_prase(pool, (lmsrp_auth_common_header*) dst,
			lmsrp_www_check, _lmsrp_set_pro, sizeof(lmsrp_www_h), data, end);
}
