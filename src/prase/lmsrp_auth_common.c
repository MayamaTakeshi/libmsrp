/*
 * lmsrp_auth_common.c
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>

void lmsrp_auth_common_prase(pj_pool_t *pool, lmsrp_auth_common_header *dst,
		lmsrp_check *check, lmsrp_auth_set_pro *set_pro,int size , char *data, int end) {
	int status = 0;
	pj_str_t name, value;
	pj_bzero(dst, sizeof(size));
	dst->pool = pool;

	int dem = 0;
	while (1) {

		dem = lmsrp_get_str(&name, data, end, check, &status);
		if (dem < 1) {
			set_pro(dst, &name, NULL);
			break;
		}
		data = data + dem;
		end = end - dem;
		dem = lmsrp_header_check_equal(data, end, &status);
		if (dem == -1) {
			set_pro(dst, &name, NULL);
			break;
		}
		data = data + dem;
		end = end - dem;
		if (status == 1) {
			status = 0;
			dem = lmsrp_get_str(&value, data, end, check, &status);
			set_pro(dst, &name, &value);
			if (dem < 1)
				break;
			if (data[dem] == '"')
				dem++;
			data = data + dem;
			end = end - dem;

		} else {
			set_pro(dst, &name, NULL);
		}
	}
}
