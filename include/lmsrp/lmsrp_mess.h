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
	pj_int64_t expries;
	pj_str_t contend;
	char flag;
} lmsrp_mess;
typedef struct lmsrp_mess_endline {
	pj_str_t tid;
	char flag;
	int vt;
} lmsrp_mess_endline;
pj_bool_t lmsrp_check_end(const char *buff, int leng, char *flag);

pj_bool_t lmsrp_check_end2(char *buff, int leng, lmsrp_mess_endline *end);

/**
 * 	to save memory , i only use ponter to point data , not copy it
 * 	this is important to remember that you data will lost if you don't use dynamic alloc
 *	THIS IS USEFULL TO READ ON ONE MSRP MESS , BUT IT NOT WORD FOR CHAIN OF MSRP MESS
 * @param pool pj_pool
 * @param data data buffer
 * @param end  poniter
 * @return
 */
lmsrp_mess* lmsrp_mess_create_from_buff(pj_pool_t *pool, char *data, int end);

int lmsrp_mess_tostring(lmsrp_mess *mess, char *data, int size);
lmsrp_mess* lmsrp_mess_create_request(pj_pool_t *pool, pj_str_t *sessid,
		pj_str_t *method);
#endif /* LMSRP_MESS_H_ */
