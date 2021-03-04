/*
 * lmsrp_status.h
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>
static int lmsrp_status_check(char d, void *arg) {
	if (d == ' ' || d == ':')
		return 1;
	return 0;
}
void lmsrp_status_h_prase(pj_pool_t *pool, lmsrp_status_h *header, char *data,
		int end) {

	pj_str_t out;
	pj_bzero(header, sizeof(lmsrp_status_h));
	header->pool = pool;
	int dem = lmsrp_get_str(&out, data, end, &lmsrp_status_check, NULL);

	data = data + dem;
	end = end - dem;
	header->code = pj_strtol(&out);
	if (dem < 1)
		return;
	dem = lmsrp_get_str(&out, data, end, &lmsrp_status_check, NULL);
	data = data + dem;
	end = end - dem;
	header->rfc_code = pj_strtol(&out);
	if (dem < 1)
		return;
	lmsrp_get_str(&out, data, end, &lmsrp_status_check, NULL);
	pj_str_t *ds = pj_strtrim(&out);
	if (ds->slen > 0) {
		pj_memcpy(&header->reason, ds, sizeof(pj_str_t));
	}
}
