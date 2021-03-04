/*
 * lmsrp_byte_range.c
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#include<lmsrp.h>
static int lmsrp_byte_range_check(char d, void *arg) {
	if (d == ' ' || d == '/' || d == '-' || d == ':')
		return 1;
	return 0;
}
void lmsrp_byte_range_prase(pj_pool_t *pool, lmsrp_byte_range *header,
		char *data, int end) {
	//TODO kiem tra cac dieu kien '-' '/' sau nay
	int dem;
	pj_bzero(header, sizeof(lmsrp_byte_range));
	header->pool = pool;
	pj_str_t out;
	dem = lmsrp_get_str(&out, data, end, &lmsrp_byte_range_check, NULL);
	data = data + dem;
	end = end - dem;
	header->start = pj_strtol(&out);
	if (dem < 1)
		return;
	dem = lmsrp_get_str(&out, data, end, &lmsrp_byte_range_check, NULL);
	data = data + dem;
	end = end - dem;
	header->end = pj_strtol(&out);
	if (dem < 1)
		return;
	dem = lmsrp_get_str(&out, data, end, &lmsrp_byte_range_check, NULL);
	data = data + dem;
	end = end - dem;
	header->total = pj_strtol(&out);
	if (dem < 1)
		return;
}
