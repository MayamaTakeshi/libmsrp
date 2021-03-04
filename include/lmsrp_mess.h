/*
 * lmsrp_mess.h
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#ifndef LMSRP_MESS_H_
#define LMSRP_MESS_H_

#include <lmsrp.h>
typedef struct _lmsrp_mess {
	pj_pool_t *pool;
	pj_str_t tid;
	lmsrp_info info;
	lmsrp_mess_type type;

	lmsrp_status_h *status;
	lmsrp_byte_range *byte_range;
	lmsrp_content_type_h *content_type;

	lmsrp_list_uri *to_path;
	lmsrp_list_uri *from_path;
	lmsrp_list_uri *use_path;

	lmsrp_www_h *www;
	lmsrp_authorization_h *auth;

	pj_str_t messid;
	pj_str_t failure_report;
	pj_str_t success_report;
	pj_int64_t max_expries;
	pj_int64_t min_expries;
	pj_int64_t express;
	pj_str_t contend;
	char *flag;
} lmsrp_mess;

pj_bool_t lmsrp_check_end(const char *buff, int leng, char *flag);

lmsrp_mess* lmsrp_mess_create_from_buff(pj_pool_t *pool,  char *data, int end);

#endif /* LMSRP_MESS_H_ */
