/*
 * lmsrp_auth_tostring.c
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */
#include <lmsrp.h>
int lmsrp_authorization_header_to_string(lmsrp_authorization_h *header,
		char *buff, int size) {
	int dem = 0;
	int tong = 0;
	char *point = buff;

	if (header->scheme.slen>0) {
		dem = sprintf(point, "%.*s ", (int) header->scheme.slen,
				header->scheme.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->realm.slen>0) {
		dem = sprintf(point, "realm=\"%.*s\"", (int) header->realm.slen,
				header->realm.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->algorithm.slen>0) {
		dem = sprintf(point, ",algorithm=\"%.*s\"",
				(int) header->algorithm.slen, header->algorithm.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->nc.slen>0) {
		dem = sprintf(point, ",nc=%.*s", (int) header->nc.slen,
				header->nc.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->nonce.slen>0) {
		dem = sprintf(point, ",nonce=\"%.*s\"", (int) header->nonce.slen,
				header->nonce.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->cnonce.slen>0) {
		dem = sprintf(point, ",cnonce=\"%.*s\"", (int) header->cnonce.slen,
				header->cnonce.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->opaque.slen>0) {
		dem = sprintf(point, ",opaque=\"%.*s\"", (int) header->opaque.slen,
				header->opaque.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->username.slen>0) {
		dem = sprintf(point, ",username=\"%.*s\"", (int) header->username.slen,
				header->username.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->response.slen>0) {
		dem = sprintf(point, ",response=\"%.*s\"", (int) header->response.slen,
				header->response.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->uri.slen>0) {
		dem = sprintf(point, ",uri=\"%.*s\"", (int) header->uri.slen,
				header->uri.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->qop.slen>0) {
		dem = sprintf(point, ",qop=%.*s", (int) header->qop.slen,
				header->qop.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	return tong;
}
