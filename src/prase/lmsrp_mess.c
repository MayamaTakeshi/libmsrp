/*
 * lmsrp_mess.c
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */
#include <lmsrp.h>
//static const char lmsrp_end_line[8] = "\n-------";
#define this "lmsrp_mess.c"
static pj_str_t lmsrp_eol = { "\r\n\r\n", 2 };
static int find_CRLF(char *data, int end) {
	for (int i = 0; i < end; i++) {
		if (data[i] == '\n')
			return i + 1;
	}
	return end + 1;
}
static int rm_colon(char *data, int end) {
	for (int i = 0; i < end; i++) {

		if (data[i] == ':')
			return i + 1;
		if (data[i] > ' ')
			return i;
	}
	return end;
}
static int lmsrp_mess_check(char d, void *arg) {
	if (d == ' ' || d == ':' || d == '\r' || d == '\n')
		return 1;
	return 0;
}

typedef enum lmsrp_mess_header_type {
	lmsrp_mess_header_msrp = 1,
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
	lmsrp_mess_header_authorization,
	lmsrp_mess_header_unknow
} lmsrp_mess_header_type;
// 15 phan tu

const static header_property hmess[] = { //
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
pj_int32_t lmsrp_mess_set_header(lmsrp_mess *mess, pj_str_t *name, char *data,
		int end) {
	static const int hmess_leng = sizeof(hmess) / sizeof(header_property);
	int st = lmsrp_find_header_property(hmess, hmess_leng, name);
	static int ls = sizeof(pj_str_t);
	pj_pool_t *pool = mess->pool;
	static pj_str_t out;
	static pj_str_t *rs;
	int dem = rm_colon(data, end);
	data = data + dem;
	end = end - dem;
	switch (st) {
	case lmsrp_mess_header_msrp:
		;
		if (mess->tid.slen > 0)
			return -1;
		int dem = lmsrp_get_str(&out, data, end, &lmsrp_mess_check, NULL);
		data = data + dem;
		end = end - dem;
		rs = pj_strtrim(&out);
		pj_memcpy(&mess->tid, rs, ls);
		mess->type = lmsrp_info_prase(&mess->info, data, end);
		break;
	case lmsrp_mess_header_to_path:
		if (mess->to_path != NULL) {
			return -1;
		}
		mess->to_path = pj_pool_alloc(mess->pool, sizeof(lmsrp_list_uri));

		lmsrp_list_uri_prase(mess->pool, mess->to_path, data, end);
		break;
	case lmsrp_mess_header_from_path:
		;
		if (mess->from_path != NULL) {
			return -1;
		}
		mess->from_path = pj_pool_alloc(mess->pool, sizeof(lmsrp_list_uri));
		lmsrp_list_uri_prase(mess->pool, mess->from_path, data, end);
		break;
	case lmsrp_mess_header_use_path:
		if (mess->use_path != NULL) {
			return -1;
		}
		mess->use_path = pj_pool_alloc(mess->pool, sizeof(lmsrp_list_uri));
		lmsrp_list_uri_prase(mess->pool, mess->use_path, data, end);
		break;
	case lmsrp_mess_header_byte_range:
		if (mess->byte_range != NULL) {
			return -1;
		}
		mess->byte_range = pj_pool_alloc(mess->pool, sizeof(lmsrp_byte_range));
		lmsrp_byte_range_prase(pool, mess->byte_range, data, end);
		break;
	case lmsrp_mess_header_content_type:
		;
		if (mess->content_type != NULL) {
			return -1;
		}
		mess->content_type = pj_pool_alloc(pool, sizeof(lmsrp_content_type_h));
		lmsrp_content_type_prase(pool, mess->content_type, data, end);
		break;
	case lmsrp_mess_header_messid:
		if (mess->messid.slen > 0)
			return -1;
		lmsrp_get_str(&out, data, end, lmsrp_mess_check, NULL);
		pj_memcpy(&mess->messid, &out, ls);
		break;
	case lmsrp_mess_header_failure_report:
		if (mess->failure_report.slen > 0)
			return -1;
		lmsrp_get_str(&out, data, end, lmsrp_mess_check, NULL);
		pj_memcpy(&mess->failure_report, &out, ls);
		break;
	case lmsrp_mess_header_sucess_report:
		if (mess->success_report.slen > 0)
			return -1;
		lmsrp_get_str(&out, data, end, lmsrp_mess_check, NULL);
		pj_memcpy(&mess->success_report, &out, ls);
		break;
	case lmsrp_mess_header_status:
		if (mess->status != NULL)
			return -1;
		mess->status = pj_pool_alloc(pool, sizeof(lmsrp_status_h));
		lmsrp_status_h_prase(pool, mess->status, data, end);
		break;
	case lmsrp_mess_header_max_express:
		if (mess->max_expries != 0)
			return -1;
		lmsrp_get_str(&out, data, end, lmsrp_mess_check, NULL);
		mess->max_expries = pj_strtol(&out);
		break;
	case lmsrp_mess_header_min_express:
		if (mess->min_expries != 0)
			return -1;
		lmsrp_get_str(&out, data, end, lmsrp_mess_check, NULL);
		mess->min_expries = pj_strtol(&out);
		break;
	case lmsrp_mess_header_express:
		if (mess->expries != 0)
			return -1;
		lmsrp_get_str(&out, data, end, lmsrp_mess_check, NULL);
		mess->expries = pj_strtol(&out);
		break;
	case lmsrp_mess_header_www_auth:
		if (mess->www != NULL)
			return -1;
		mess->www = pj_pool_alloc(pool, sizeof(lmsrp_www_h));
		lmsrp_www_h_prase(pool, mess->www, data, end);

		break;
	case lmsrp_mess_header_authorization:
		if (mess->www != NULL)
			return -1;
		mess->auth = pj_pool_alloc(pool, sizeof(lmsrp_authorization_h));
		lmsrp_authorization_header_prase(pool, mess->auth, data, end);
		break;
	default:
		return lmsrp_mess_header_unknow;
	}
	return st;
}

pj_bool_t lmsrp_check_end(const char *buff, int leng, char *flag) {
	static int back = 20; // olyread 20 character

	int i = 1;
	for (; i <= back && i < leng; i++) {
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

	while (i < back + 7 && i < leng) {
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
pj_bool_t lmsrp_check_end2(char *buff, int leng, lmsrp_mess_endline *end) {
	static int back = 40; // olyread 20 character
	end->vt = -1;
	int i = 1;
	for (; i <= leng; i++) {
		if (buff[leng - i] > '"') {
			if (buff[leng - i] == '#' || buff[leng - i] == '$'
					|| buff[leng - i] == '+') {
				end->flag = buff[leng - i];
				break;
			} else
				return PJ_FALSE;
		}
	}
	int check = leng - i;
	int dem = 0;

	while (i < back + 7) {
		if (buff[leng - i] == '-') {
			dem++;
			if (dem == 7) {
				end->tid.ptr = buff + (leng - i + 7);
				end->tid.slen = check - (leng - i + 7);
				end->vt = leng - i;
				return PJ_TRUE;
			}
		} else {
			dem = 0;
		}
		i++;
	}
	return PJ_FALSE;
}
lmsrp_mess* lmsrp_mess_create_from_buff(pj_pool_t *pool, char *data, int end) {
	lmsrp_mess_endline line;
	// luu gia bien dem
	int keep = 0;
	lmsrp_check_end2(data, end, &line);
	if (line.vt < 0) {
		PJ_LOG(2, (__FILE__,"this is not msrp mess"));
		return NULL;
	}
	lmsrp_mess *mess = pj_pool_zalloc(pool, sizeof(lmsrp_mess));
	mess->pool = pool;
	mess->flag = line.flag;
	pj_str_t name;
	int dem, lend;
	int kt = 0;
	int check = 0;
	while (1) {
		dem = lmsrp_get_str(&name, data, end, &lmsrp_mess_check, NULL);
		if (dem < 1)
			break;
		keep = keep + dem;
		data = data + dem;
		end = end - dem;
		lend = find_CRLF(data, end);
		keep = keep + lend;
		if (data[lend - 2] == '\r')
			kt = 2;
		else
			kt = 1;
		lmsrp_mess_set_header(mess, &name, data, lend - kt);
		data = data + lend;
		end = end - lend;
		if (data[0] == '\n')
			check = 1;
		else if (data[1] == '\n')
			check = 2;
		if (check) {
			// this is contend data
			name.ptr = data + check;
			name.slen = line.vt - keep - check - 2;
			pj_str_t *lp = (&name);
			pj_memcpy(&mess->contend, lp, sizeof(pj_str_t));
			break;
		}
	}
	return mess;
}

pj_bool_t lmsrp_stream_prase(lmsrp_context *ctx, char *data, int end) {
	lmsrp_mess *mess = ctx->mess;
	int dem, keep, lend, kt, check;
	keep = 0;
	check = 0;
	pj_str_t name;
	pj_int32_t st;
	if (ctx->state == lmsrp_prase_state_done)
		ctx->state = lmsrp_prase_state_unknow;
	else if (ctx->state == lmsrp_prase_state_content)
		goto CONTENT;
	{
		while (1) {
			dem = lmsrp_get_str(&name, data, end, &lmsrp_mess_check, NULL);
			if (dem < 1)
				break;

			data = data + dem;
			end = end - dem;
			lend = find_CRLF(data, end);
			if (lend > end) {
				ctx->data_read = ctx->data_read + keep;
				return PJ_TRUE;
			}
			keep = keep + dem;
			if (data[lend - 2] == '\r')
				kt = 2;
			else
				kt = 1;
			st = lmsrp_mess_set_header(mess, &name, data, lend - kt);
			if (st < 0) {
				ctx->data_read = ctx->data_read + keep;
				PJ_LOG(1, (this,"dulicate header"));
				return PJ_FALSE;
			}
			data = data + lend;
			end = end - lend;
			keep = keep + lend;
			if (st == lmsrp_mess_header_msrp)
				ctx->state = lmsrp_prase_state_mess;
			else
				ctx->state = lmsrp_prase_state_header;

			if (end < 2) {
				ctx->data_read = ctx->data_read + keep;
				return PJ_TRUE;
			}
			if (data[0] == '\n')
				check = 1;
			else if (data[1] == '\n')
				check = 2;
			if (check) {
				data = data + check;
				end = end - check;
				keep = keep + check;
				ctx->state = lmsrp_prase_state_content;
				mess->contend.ptr = data;
				goto CONTENT;
			}
//			if (st == lmsrp_mess_header_unknow) {
//				dem = sprintf(ctx->tid, "-------%.*s", (int) mess->tid.slen,
//						mess->tid.ptr);
//				pj_str_t tid = pj_str(ctx->tid);
//				name.slen = dem;
//				if (pj_strcmp(&tid, &name) == 0) {
//					ctx->data_read  = keep ;
//					ctx->state = lmsrp_prase_state_done;
//					return PJ_TRUE;
//				}
//			}
		}

	}
	CONTENT: {
		// gioi han data
		int all = 0;
		if (ctx->content_leng + end > ctx->max_byte + 20)
			all = 1;
		int start = ctx->content_leng;
		dem = sprintf(ctx->tid, "\n-------%.*s", (int) mess->tid.slen,
				mess->tid.ptr);
		pj_str_t tid = pj_str(ctx->tid);
		pj_str_t find = { (data + start), end - start };
		while (1) {
			char *el = pj_strstr(&find, &tid);
			if (el == NULL) {
				if (all == 1) {
					ctx->data_read = ctx->data_read + keep + (el - data);
					PJ_LOG(1, (this,"data is over when cant found endline"));
					return PJ_FALSE;
				} else {
					start = start + end - (tid.slen - 1);
					if (start < 0)
						start = 0;
					ctx->data_read = ctx->data_read + keep;
					ctx->content_leng = start;
					return PJ_TRUE;
				}
			} else {
				int slen = (el - data);
				end = end - slen;
				if (end - tid.slen < 1 + lmsrp_eol.slen) {
					if (all == 1) {
						PJ_LOG(1, (this,"data is over end line"));
						return PJ_FALSE;
					}
					start = start + end - (tid.slen - 1);
					if (start < 0)
						start = 0;
					ctx->data_read = ctx->data_read + keep;
					ctx->content_leng = start;
					return PJ_TRUE;
				}
				char flag = el[tid.slen];
				if (flag != '+' && flag != '#' && flag != '$') {
					find.ptr = data + slen + 1 + tid.slen;
					find.slen = find.slen - slen - 1 - tid.slen;
					continue;
				}
				pj_str_t eol = { data + slen + 1 + tid.slen, lmsrp_eol.slen };
				if (pj_strcmp(&eol, &lmsrp_eol) != 0) {
					find.ptr = data + slen + 1 + lmsrp_eol.slen + tid.slen;
					find.slen = find.slen - slen - 1 - lmsrp_eol.slen
							- tid.slen;
					continue;
				}
				mess->flag = el[tid.slen];
				mess->contend.slen = slen;
				if (mess->contend.ptr[slen-1] == '\r') {
					mess->contend.slen = slen - 1;
				}
				ctx->state = lmsrp_prase_state_done;
				ctx->data_read = ctx->data_read + keep + mess->contend.slen
						+ tid.slen + 1 + eol.slen;
				return PJ_TRUE;
			}
		}
	};
	return PJ_TRUE;
}
