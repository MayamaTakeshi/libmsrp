/*
 * lmsrp_content_type.h
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#ifndef LMSRP_CONTENT_TYPE_H_
#define LMSRP_CONTENT_TYPE_H_

#include <lmsrp.h>

typedef struct _lmsrp_content_type {
	pj_pool_t *pool;
	lmsrp_list_param *type;
} lmsrp_content_type_h;

void lmsrp_content_type_prase(pj_pool_t *pool, lmsrp_content_type_h *header,
		char *data, int end);

lmsrp_content_type_h* lmsrp_content_type_create(pj_pool_t *pool, pj_str_t *name,
		pj_str_t *value);
int lmsrp_content_type_to_string(lmsrp_content_type_h *con, char *data,
		int size);
#endif /* LMSRP_CONTENT_TYPE_H_ */
