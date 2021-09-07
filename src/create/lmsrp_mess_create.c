/*
 * lmsrp_mess_create.c
 *
 *  Created on: Mar 6, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>

const static pj_str_t lmsrp_tp_tcp = { "tcp", 3 };
const static pj_str_t lmsrp_tp_tls = { "tls", 3 };
const static pj_str_t lmsrp_tp_udp = { "udp", 3 };
const static pj_str_t lmsrp_ch_tls = { "msrps", 5 };
const static pj_str_t lmsrp_ch_tcp = { "msrp", 4 };

lmsrp_mess* lmsrp_mess_create_request(pj_pool_t *pool, pj_str_t *sessid,
		pj_str_t *method) {
	lmsrp_mess *res = pj_pool_zalloc(pool, sizeof(lmsrp_mess));
	res->pool = pool;
	if (sessid != NULL)
		pj_memcpy(&res->tid, sessid, sizeof(pj_str_t));
	pj_memcpy(&res->info.request.method, method, sizeof(pj_str_t));
	res->flag = '$';
	return res;
}
lmsrp_status_h* lmsrp_status_h_create(pj_pool_t *pool, int code, int rfcode,
		pj_str_t *reason) {
	lmsrp_status_h *res = PJ_POOL_ZALLOC_T(pool, lmsrp_status_h);
	res->pool = pool;
	res->code = code;
	res->rfc_code = rfcode;
	if (reason != NULL)
		pj_memcpy(&res->reason, reason, sizeof(pj_str_t));
	return res;
}
lmsrp_uri* lmsrp_uri_create(pj_pool_t *pool, pj_str_t *host, int port,
		pj_str_t *sessid, pj_str_t *username, int transport) {
	lmsrp_uri *uri = PJ_POOL_ZALLOC_T(pool, lmsrp_uri);
	uri->pool = pool;
//	pj_memcpy(&uri->session_id, sessid, sizeof(pj_str_t));
	uri->session_id = *sessid;
	if (port > 0)
		uri->authority.port = port;
	switch (transport) {
	case lmsrp_transport_tcp:
		uri->transport = lmsrp_tp_tcp;
		uri->scheme = lmsrp_ch_tcp;
		break;
	case lmsrp_transport_udp:
		uri->transport = lmsrp_tp_udp;
		uri->scheme = lmsrp_ch_tcp;
		break;
	case lmsrp_transport_tls:
		uri->transport = lmsrp_tp_tls;
		uri->scheme = lmsrp_ch_tls;
		break;
	default:
		;
		uri->scheme = lmsrp_ch_tcp;
		break;
	}
	if (host != NULL) {
//		pj_memcpy(&uri->authority.host, host, sizeof(pj_str_t));
		uri->authority.host = *host;
	}
	if (sessid != NULL) {
		uri->session_id = *sessid;
//		pj_memcpy(&uri->session_id, sessid, sizeof(pj_str_t));
	}
	if (username != NULL) {
		uri->authority.userinfo = *username;
//		pj_memcpy(&uri->authority.userinfo, username, sizeof(pj_str_t));
	}

	return uri;
}
lmsrp_byte_range* lmsrp_byte_range_create(pj_pool_t *pool, pj_uint64_t start,
		pj_uint64_t end, pj_uint64_t total) {
	lmsrp_byte_range *res = PJ_POOL_ALLOC_T(pool, lmsrp_byte_range);
	res->pool = pool;
	res->start = start;
	res->end = end;
	res->total = total;
	return res;
}
void lmsrp_list_uri_add(lmsrp_list_uri *list, lmsrp_uri *uri) {
	if (list->uri == NULL) {
		list->uri = uri;
	} else {
		lmsrp_list_uri *res = PJ_POOL_ZALLOC_T(list->pool, lmsrp_list_uri);
		res->pool = list->pool;
		res->uri = uri;
		uri->pool = list->pool;
		pj_list_insert_before(list, res);
	}
}
lmsrp_list_uri* lmsrp_list_uri_create(pj_pool_t *pool, lmsrp_uri *uri) {
	lmsrp_list_uri *res = PJ_POOL_ZALLOC_T(pool, lmsrp_list_uri);
	pj_list_init(res);
	res->pool = pool;
	res->uri = uri;
	return res;
}

lmsrp_content_type_h* lmsrp_content_type_create(pj_pool_t *pool, pj_str_t *name,
		pj_str_t *value) {
	if (!name || !pool || !value)
		return NULL;
	lmsrp_content_type_h *cont = pj_pool_alloc(pool,
			sizeof(lmsrp_content_type_h));
	cont->pool = pool;
	cont->type = pj_pool_alloc(pool, sizeof(lmsrp_list_param));
	pj_list_init(cont->type);
	cont->type->var.name = *name;
	cont->type->var.value = *value;
	return cont;
}

