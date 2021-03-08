/*
 * lmsrp_mess_tostring.c
 *
 *  Created on: Mar 5, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>
static pj_str_t endline = { "\r\n", 2 };
// fast witer to chek out of data
#define _LMSRP_MESSEND      \
	if (dem == -1) return -1; \
	size = size - dem; \
	tong = tong + dem; \
	point = point + dem; \
	if ( size < 0) {  \
		return -1;  \
	}
#define _LMSRP_MESS_END(dem,size,tong,point)     \
		if (dem == -1) return -1; \
		size = size - dem; \
		tong = tong + dem; \
		point = point + dem; \
		if (size < 0) {  \
			return -1;  \
		}
static int path_to_string(char *buff, int size, char *path_name,
		lmsrp_list_uri *urs) {
	int dem = 0;
	int tong = 0;
	char *point = buff;
	dem = sprintf(point, "%s: ", path_name);
	_LMSRP_MESSEND;
	dem = lmsrp_list_uri_tostring(urs, point, size);
	_LMSRP_MESSEND;
	pj_memcpy(point, endline.ptr, endline.slen);
	return tong + 2;
}
int lmsrp_mess_tostring(lmsrp_mess *mess, char *data, int size) {
	int dem = 0;
	int tong = 0;
	char *point = data;
	if (mess->type == lmsrp_mess_type_request) {
		dem = sprintf(point, "MSRP %.*s %.*s\r\n", (int) mess->tid.slen,
				mess->tid.ptr, (int) mess->info.request.method.slen,
				mess->info.request.method.ptr);
//		tong =tong
	} else {
		dem = sprintf(point, "MSRP %.*s %d %.*s\r\n", (int) mess->tid.slen,
				mess->tid.ptr, mess->info.respone.code,
				(int) mess->info.respone.reason.slen,
				mess->info.respone.reason.ptr);
	}
	_LMSRP_MESSEND;
	if (mess->to_path != NULL) {
		dem = path_to_string(point, size, "To-Path", mess->to_path);
		_LMSRP_MESSEND;
	}
	if (mess->from_path != NULL) {
		dem = path_to_string(point, size, "From-Path", mess->from_path);
		_LMSRP_MESSEND;
	}
	if (mess->use_path != NULL) {
		dem = path_to_string(point, size, "Use-Path", mess->use_path);
		_LMSRP_MESSEND;
	}
	if (mess->status != NULL) {
		dem = sprintf(point, "Status : %d %d %.*s\r\n", mess->status->code,
				mess->status->rfc_code, (int) mess->status->reason.slen,
				mess->status->reason.ptr);
		_LMSRP_MESSEND;
	}
	if (mess->messid.slen > 0) {
		dem = sprintf(point, "Message-id: %.*s\r\n", (int) mess->messid.slen,
				mess->messid.ptr);
		_LMSRP_MESSEND;
	}
	if (mess->failure_report.slen) {
		dem = sprintf(point, "Failure-Report: %.*s\r\n",
				(int) mess->failure_report.slen, mess->failure_report.ptr);
		_LMSRP_MESSEND;
	}
	if (mess->success_report.slen) {
		dem = sprintf(point, "Success-Report: %.*s\r\n",
				(int) mess->success_report.slen, mess->success_report.ptr);
		_LMSRP_MESSEND;
	}
	if (mess->expries > 0) {
		dem = sprintf(point, "Expries: %lld\r\n", mess->expries);
		_LMSRP_MESSEND;
	}
	if (mess->max_expries > 0) {
		dem = sprintf(point, "Max-Expries: %lld\r\n", mess->max_expries);
		_LMSRP_MESSEND;
	}
	if (mess->min_expries > 0) {
		dem = sprintf(point, "Min-Expries: %lld\r\n", mess->min_expries);
		_LMSRP_MESSEND;
	}
	if (mess->byte_range != NULL) {
		if (mess->byte_range->start == 0) {
			dem = sprintf(point, "Byte-Range: *-");

		} else {
			dem = sprintf(point, "Byte-Range: %lld-", mess->byte_range->start);
		}
		_LMSRP_MESSEND;
		if (mess->byte_range->end == 0) {
			dem = sprintf(point, "*/");
		} else {
			dem = sprintf(point, "%lld/", mess->byte_range->end);
		}
		_LMSRP_MESSEND;
		if (mess->byte_range->total == 0) {
			dem = sprintf(point, "*\r\n");
		} else {
			dem = sprintf(point, "%lld\r\n", mess->byte_range->total);
		}
		_LMSRP_MESSEND;
	}
	if (mess->www != NULL) {
		dem = sprintf(point, "WWW-Authenticate: ");
		_LMSRP_MESSEND;
		dem = lmsrp_www_h_to_string(mess->www, point, size);
		_LMSRP_MESSEND;
		dem = sprintf(point, "\r\n");
		_LMSRP_MESSEND;
	}
	if (mess->auth != NULL) {
		dem = sprintf(point, "Authorization: ");
		_LMSRP_MESSEND;
		dem = lmsrp_authorization_header_to_string(mess->auth, point, size);
		_LMSRP_MESSEND;
		dem = sprintf(point, "\r\n");
		_LMSRP_MESSEND;
	}
// TODO add content type
	if (mess->contend.slen) {
		dem = sprintf(point, "\r\n");
		_LMSRP_MESSEND;
		dem = mess->contend.slen;
		pj_memcpy(point, mess->contend.ptr, mess->contend.slen);
		_LMSRP_MESSEND;
		dem = sprintf(point, "\r\n");
		_LMSRP_MESSEND;
	}
	dem = sprintf(point, "-------%.*s%c\r\n\r\n", (int) mess->tid.slen,
			mess->tid.ptr, mess->flag);
	_LMSRP_MESSEND;
	return tong;
}
