/*
 * lmsrp_list_uri_tostring.c
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>
int lmsrp_uri_tostring(lmsrp_uri *uri, char *buff, int leng) {
	int dem = 0;
	int tong = 0;
	char *point = buff;
	dem = sprintf(point, " %.*s://", (int) uri->scheme.slen, uri->scheme.ptr);
	tong = tong + dem;
	if (tong > leng)
		return -1;
	point = point + dem;
	if (uri->authority.userinfo.slen > 0) {
		dem = sprintf(point, "%.*s@", (int) uri->authority.userinfo.slen,
				uri->authority.userinfo.ptr);
		tong = tong + dem;
		if (tong > leng)
			return -1;
		point = point + dem;
	}
	dem = sprintf(point, "%.*s", (int) uri->authority.host.slen,
			uri->authority.host.ptr);
	tong = tong + dem;
	if (tong > leng)
		return -1;
	point = point + dem;
	if (uri->authority.port > 0) {
		dem = sprintf(point, ":%.d", uri->authority.port);
		tong = tong + dem;
		if (tong > leng)
			return -1;
		point = point + dem;
	}
	if (uri->session_id.slen > 0) {
		dem = sprintf(point, "/%.*s", (int) uri->session_id.slen,
				uri->session_id.ptr);
		tong = tong + dem;
		if (tong > leng)
			return -1;
		point = point + dem;
	}
	if (uri->transport.slen > 0) {
		dem = sprintf(point, ";%.*s", (int) uri->transport.slen,
				uri->transport.ptr);
		tong = tong + dem;
		if (tong > leng)
			return -1;
		point = point + dem;
	}
	return tong;
}
int lmsrp_list_uri_tostring(lmsrp_list_uri *urs, char *buff, int leng) {
	int uri_num = pj_list_size(urs);
	int dem = 0;
	int tong = 0;
	char *point = buff;
	lmsrp_uri *uri;
	while (uri_num > 0) {
		uri = urs->uri;
		uri_num--;
		dem = lmsrp_uri_tostring(uri, point, leng);
		tong = tong + dem;
		if (tong > leng)
			return -1;
		point = point + dem;
		urs = urs->next;
	}
	return tong;
}
