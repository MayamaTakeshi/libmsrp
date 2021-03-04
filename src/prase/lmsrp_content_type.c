/*
 * lmsrp_content_type.c
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */
#include <lmsrp.h>

static int lmsrp_content_type_check(char d, void *arg) {
	if (d == ' ' || d == '/' || d == ':')
		return 1;
	return 0;
}
void lmsrp_content_type_prase(pj_pool_t *pool, lmsrp_content_type_h *header,
		char *data, int end) {
	pj_bzero(header, sizeof( lmsrp_content_type_h));
	int dem = 0;
	pj_str_t name, value;
	header->pool = pool;
	while (1) {
		dem = lmsrp_get_str(&name, data, end, &lmsrp_content_type_check, NULL);
		data = data + dem;
		end = end - dem;
		if (dem < 1) {
			break;
		}
		if (data[0] != '/')
			break;
		dem = lmsrp_get_str(&value, data, end, &lmsrp_content_type_check, NULL);
		data = data + dem;
		end = end - dem;

		lmsrp_list_param *ls = pj_pool_alloc(pool, sizeof(lmsrp_list_param));
		pj_memcpy(&ls->var.name, &name, sizeof(pj_str_t));
		pj_memcpy(&ls->var.value, &value, sizeof(pj_str_t));

		if (header->type == NULL) {
			header->type = ls;
			pj_list_init(header->type);
		} else {
			pj_list_insert_after(header->type, ls);
		}
		if (dem < 1) {
			break;
		}
	}
}
