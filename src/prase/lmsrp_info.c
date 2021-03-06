/*
 * lmsrp_info.c
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>
static int check(char d, void *info) {

	if (d == ' ' || d == ':')
		return 1;
	else
		return 0;
}
int lmsrp_info_prase(lmsrp_info *inf, char *data, int end) {
	pj_bzero(inf, sizeof(lmsrp_info));
	pj_str_t in;
	int dem = lmsrp_get_str(&in, data, end, &check, NULL);
	long lp = pj_strtol(&in);
	if (lp == 0) {
		pj_memcpy(&inf->request.method, &in, sizeof(pj_str_t));
		return lmsrp_mess_type_request;
	} else {
		inf->respone.code = lp;
	}
	data = data + dem;
	end = end - dem;
	in.ptr = data;
	in.slen = dem;
	pj_str_t *out = pj_strtrim(&in);
	pj_memcpy(&inf->respone.reason, out, sizeof(pj_str_t));
	return lmsrp_mess_type_respone;
}
