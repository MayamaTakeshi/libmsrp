/*
 * lmsrp_common.c
 *
 *  Created on: Mar 3, 2021
 *      Author: amneiht
 */
#include <lmsrp.h>

static char sperate[] = { ' ', ':', '/', '@', ';' };
static int check(char d, void *arg) {
	int dem = sizeof(sperate);
	for (int i = 0; i < dem; i++) {
		if (sperate[i] == d) {
			return 1;
		}
	}
	return 0;
}
void lmsrp_list_uri_prase(pj_pool_t *pool, lmsrp_list_uri *dst, char *data,
		int end) {
	//  lam sach data
	pj_bzero(dst, sizeof(lmsrp_list_uri));
	dst->pool = pool;
//	while
	pj_list_init(dst);

	lmsrp_uri lst;
	int dem = 0;
	lmsrp_list_uri *point = dst;
	while (1) {
		dem = lmsrp_uri_prase(pool, &lst, data, end);
		if (dem < 1)
			break;
		else {
			data = data + dem;
			end = end - dem;
			point->uri = pj_pool_alloc(pool, sizeof(lmsrp_uri));
			pj_memcpy(point->uri, &lst, sizeof(lmsrp_uri));
			lmsrp_list_uri *ls = pj_pool_alloc(pool, sizeof(lmsrp_list_uri));
			ls->pool = pool;
			pj_list_insert_before(dst, ls);
			point = ls;
		}
	}

}
int lmsrp_get_str(pj_str_t *str, char *data, int end, lmsrp_check *test,
		void *arg) {
	int i = 0;
	str->slen = 0;
	while (test(data[i], arg)) {
		i++;
		if (i >= end)
			return -1;
	}
	int dem = i;
	str->ptr = data + i;
	while (!test(data[i], arg)) {
		i++;
		if (i > end) {
			i--;
			break;
		}
	}
	str->slen = i - dem;
	return i;
}
//static int getint(pj_str_t *str) {
//	char buff[(int) (str->slen) + 1];
//	pj_memcpy(buff, str->ptr, str->slen);
//	buff[(int) (str->slen)] = '0';
//	return atoi(buff);
//}
int lmsrp_uri_prase(pj_pool_t *pool, lmsrp_uri *dst, char *data, int end) {
	pj_bzero(dst, sizeof(lmsrp_uri));
	dst->pool = pool;
	int prase = 0;
	int dem = lmsrp_get_str(&(dst->scheme), data, end, &check, NULL);
	pj_str_t out;
	prase += dem;

	if (dem < 0)
		return -1;
	data = data + dem;
	end = end - dem;

	dem = lmsrp_get_str(&(out), data, end, &check, NULL);
	if (dem < 0)
		return -1;
	prase += dem;
	data = data + dem;
	end = end - dem;
	if (data[0] == '@') {
		// ton tai user
		dst->authority.userinfo.ptr = out.ptr;
		dst->authority.userinfo.slen = out.slen;

		//prase host
		dem = lmsrp_get_str(&(out), data, end, &check, NULL);
		if (dem < 0)
			return -1;
		prase += dem;
		data = data + dem;
		end = end - dem;
		dst->authority.host.ptr = out.ptr;
		dst->authority.host.slen = out.slen;
	} else {
		//prase host
		dst->authority.host.ptr = out.ptr;
		dst->authority.host.slen = out.slen;
	}
	//
	if (data[0] == ':') {
		// prase port
		PJ_LOG(4, (__FILE__,"log port"));
		dem = lmsrp_get_str(&(out), data, end, &check, NULL);
		if (dem < 0)
			return -1;
		prase += dem;
		data = data + dem;
		end = end - dem;
//		dst->authority.port = getint(&out);
		dst->authority.port = pj_strtol(&out);
	}

	if (data[0] == '/') {
		// prase session id
		dem = lmsrp_get_str(&(out), data, end, &check, NULL);
		if (dem < 0)
			return -1;
		prase += dem;
		data = data + dem;
		end = end - dem;
		dst->session_id.ptr = out.ptr;
		dst->session_id.slen = out.slen;
	}

	if (data[0] == ';') {
		// prase tranport
		dem = lmsrp_get_str(&(out), data, end, &check, NULL);
		if (dem < 0)
			return -1;
		prase += dem;
		data = data + dem;
		end = end - dem;
		dst->transport.ptr = out.ptr;
		dst->transport.slen = out.slen;
	}
	return prase;
}
