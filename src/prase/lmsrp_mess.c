/*
 * lmsrp_mess.c
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */
#include <lmsrp.h>
static int find_CRLF(char *data, int end) {
	for (int i = 0; i < end; i++) {
		if (data[i] == '\n')
			return i;
	}
	return end;
}
static int lmsrp_mess_check(char d, void *arg) {
	if (d == ' ' || d == ':')
		return 1;
	return 0;
}
typedef enum lmsrp_mess_header_type {
	lmsrp_mess_header_msrp,
	lmsrp_mess_header_to_path,
	lmsrp_mess_header_status,
	lmsrp_mess_header_byte_range,
	lmsrp_mess_header_content_type,
	lmsrp_mess_header_from_path,
	lmsrp_mess_header_use_path,
	lmsrp_mess_header_messid,
	lmsrp_mess_header_failure_report,
	lmsrp_mess_header_sucess_report,
	lmsrp_mess_header_max_express,
	lmsrp_mess_header_min_express,
	lmsrp_mess_header_express,
	lmsrp_mess_header_www_auth,
	lmsrp_mess_header_authorization
} lmsrp_mess_header_type;
// 15 phan tu
static int hmess_leng = 15;
static header_property hmess[] = { //
		{ { "MSRP", 4 }, lmsrp_mess_header_msrp }, // rfc 4975
				{ { "To-Path", 7 }, lmsrp_mess_header_to_path }, //  rfc 4975
				{ { "Status", 6 }, lmsrp_mess_header_status }, //  rfc 4975
				{ { "Byte-Range", 10 }, lmsrp_mess_header_byte_range }, //  rfc 4975
				{ { "Content-Type", 12 }, lmsrp_mess_header_content_type }, //  rfc 4975
				{ { "From-Path", 9 }, lmsrp_mess_header_from_path }, //  rfc 4975
				{ { "Message-ID", 10 }, lmsrp_mess_header_messid }, //  rfc 4975
				{ { "Use-Path", 8 }, lmsrp_mess_header_use_path }, //  rfc 4975
				{ { "Success-Report", 14 }, lmsrp_mess_header_sucess_report }, //  rfc 4975
				{ { "Failure-Report", 14 }, lmsrp_mess_header_failure_report }, //  rfc 4975
				{ { "Max-Expires", 11 }, lmsrp_mess_header_max_express }, //  rfc 4976
				{ { "Min-Expires", 11 }, lmsrp_mess_header_min_express }, //  rfc 4976
				{ { "Expires", 7 }, lmsrp_mess_header_express }, //  rfc 4976
				{ { "WWW-Authenticate", 16 }, lmsrp_mess_header_www_auth }, //  rfc 4976
				{ { "Authorization", 13 }, lmsrp_mess_header_authorization }, //  rfc 4976
		};
void lmsrp_mess_set_header(lmsrp_mess *mess, pj_str_t *name, char *data,
		int end) {
	int st = lmsrp_find_header_property(hmess, hmess_leng, name);
	static int ls = sizeof(pj_str_t);
	pj_pool_t *pool = mess->pool;
	static pj_str_t out;
	static pj_str_t *rs;
	switch (st) {
	case lmsrp_mess_header_msrp:
		;
		int dem = lmsrp_get_str(&out, data, end, &lmsrp_mess_check, NULL);
		data = data + dem;
		end = end - dem;
		rs = pj_strtrim(&out);
		pj_memcpy(&mess->tid, rs, ls);
		mess->type = lmsrp_info_prase(&mess->info, data, end);
		break;
	case lmsrp_mess_header_to_path:
		mess->to_path = pj_pool_alloc(mess->pool, sizeof(lmsrp_list_uri));
		lmsrp_list_uri_prase(mess->pool, mess->to_path, data, end);
		break;
	case lmsrp_mess_header_from_path:
		mess->from_path = pj_pool_alloc(mess->pool, sizeof(lmsrp_list_uri));
		lmsrp_list_uri_prase(mess->pool, mess->from_path, data, end);
		break;
	case lmsrp_mess_header_use_path:
		mess->use_path = pj_pool_alloc(mess->pool, sizeof(lmsrp_list_uri));
		lmsrp_list_uri_prase(mess->pool, mess->use_path, data, end);
		break;
	case lmsrp_mess_header_byte_range:
		mess->byte_range = pj_pool_alloc(mess->pool, sizeof(lmsrp_byte_range));
		lmsrp_byte_range_prase(pool, mess->byte_range, data, end);
		break;
	case lmsrp_mess_header_content_type:
		;
		mess->content_type = pj_pool_alloc(pool, sizeof(lmsrp_content_type_h));
		lmsrp_content_type_prase(pool, mess->content_type, data, end);
		break;
	case lmsrp_mess_header_messid:
		out.ptr = data;
		out.slen = end;
		rs = pj_strtrim(&out);
		pj_memcpy(&mess->messid, rs, ls);
		break;
	case lmsrp_mess_header_failure_report:
		;
		pj_str_t out;
		out.ptr = data;
		out.slen = end;
		pj_str_t *rs = pj_strtrim(&out);
		pj_memcpy(&mess->failure_report, rs, ls);
		break;
	case lmsrp_mess_header_sucess_report:
		;

		out.ptr = data;
		out.slen = end;
		rs = pj_strtrim(&out);
		pj_memcpy(&mess->success_report, rs, ls);
		break;
	case lmsrp_mess_header_status:
		mess->status = pj_pool_alloc(pool, sizeof(lmsrp_status_h));
		lmsrp_status_h_prase(pool, mess->status, data, end);
		break;
	}
}
pj_bool_t lmsrp_check_end(const char *buff, int leng, char *flag) {
	static int back = 20; // olyread 20 character

	int i = 1;
	for (; i <= back; i++) {
		if (buff[leng - i] > '"') {
			if (buff[leng - i] == '#' || buff[leng - i] == '$'
					|| buff[leng - i] == '+') {
				*flag = buff[leng - i];
				break;
			} else
				return PJ_FALSE;
		}
	}
	int dem = 0;

	while (i < back + 7) {
		if (buff[leng - i] == '-') {
			dem++;
			if (dem == 7)
				return PJ_TRUE;
		} else {
			dem = 0;
		}
		i++;
	}
	return PJ_FALSE;
}

lmsrp_mess* lmsrp_mess_create_from_buff(pj_pool_t *pool, char *data, int end) {
	char flag;
	pj_bool_t rs = lmsrp_check_end(data, end, &flag);
	if (!rs) {
		PJ_LOG(2, (__FILE__,"this is not msrp mess"));
		return NULL;
	}
	lmsrp_mess *mess = pj_pool_zalloc(pool, sizeof(lmsrp_mess));
	mess->pool = pool;
	pj_str_t name;
	int dem, lend;
	while (1) {
		dem = lmsrp_get_str(&name, data, end, &lmsrp_mess_check, NULL);
		if (dem < 1)
			break;
		data = data + dem;
		end = end - dem;
		lend = find_CRLF(data, end);
		lmsrp_mess_set_header(mess, &name, data, lend);
	}
	return mess;
}
