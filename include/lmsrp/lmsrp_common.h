/*
 * lmsrp_common.c
 *
 *  Created on: Mar 3, 2021
 *      Author: amneiht
 */

#ifndef LMSRP_COMMON_H_
#define LMSRP_COMMON_H_

typedef int (lmsrp_check)(char d, void *info);
typedef struct _lmsrp_param {
	pj_str_t name;
	pj_str_t value;
} lmsrp_param;

typedef struct _lmsrp_list_param {
	PJ_DECL_LIST_MEMBER(struct _lmsrp_list_param);
	lmsrp_param var;
} lmsrp_list_param;

typedef struct _lmsrp_uri {
	pj_pool_t *pool;
	pj_str_t scheme;
	struct {
		pj_str_t userinfo;
		pj_str_t host;
		pj_int32_t port;
	} authority;
	pj_str_t session_id;
	pj_str_t transport;
} lmsrp_uri;

typedef struct _lmsrp_list_uri {
	PJ_DECL_LIST_MEMBER(struct _lmsrp_list_uri);
	pj_pool_t *pool;
	lmsrp_uri *uri;
} lmsrp_list_uri;
typedef struct _header_property {
	pj_str_t name;
	int pro;
} header_property;

/**
 * gan cac gia tri vao uri
 * @param pool
 * @param dst
 * @param data
 * @param end
 */
void lmsrp_list_uri_prase(pj_pool_t *pool, lmsrp_list_uri *dst, char *data,
		int end);
int lmsrp_uri_prase(pj_pool_t *pool, lmsrp_uri *dst, char *data, int end);
int lmsrp_get_str(pj_str_t *str, char *data, int end, lmsrp_check *test,
		void *arg);
int lmsrp_list_uri_tostring(lmsrp_list_uri *urs, char *buff, int leng);

lmsrp_uri* lmsrp_uri_create(pj_pool_t *pool, pj_str_t *host, int port,
		pj_str_t *sessid, pj_str_t *username, int transport);

int lmsrp_uri_tostring(lmsrp_uri *uri, char *buff, int leng);
void lmsrp_list_uri_add(lmsrp_list_uri *list, lmsrp_uri *uri);
lmsrp_list_uri* lmsrp_list_uri_create(pj_pool_t *pool, lmsrp_uri *uri);
#endif /* LMSRP_COMMON_H_ */
