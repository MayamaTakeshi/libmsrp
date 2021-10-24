/*
 * lmsrp_auth.c
 *
 *  Created on: Apr 9, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>

static pj_str_t msrp_auth_method = { "AUTH", 4 };
static pj_str_t msrp_nc = { "00000001", 8 };
#if defined (LMSRP_ONLY_PJLIB) && LMSRP_ONLY_PJLIB == 1

#include "lmsrp_digest.c"

static void msrp_make_authorization_header(lmsrp_mess *resp,
		lmsrp_cred_info *cred, lmsrp_authorization_h **auth_h, char *buff)

{

	lmsrp_www_h *www = resp->www;
	pj_pool_t *pool = www->pool;
	pj_str_t res = { buff, 32 };

	pj_str_t cnonce = { buff + 32, 8 };
	pj_create_random_string(cnonce.ptr, cnonce.slen);
	lmsrp_auth_create_digest(&res, &www->nonce, &msrp_nc, &cnonce, &www->qop,
			&resp->tid, &www->realm, cred, &msrp_auth_method);
	lmsrp_authorization_h *test;
	test = lmsrp_authorization_h_create(pool, &www->scheme, &cred->username,
			&www->realm, &www->nonce, &resp->tid, &res,
			NULL, &cnonce, &www->opaque, &www->qop, &msrp_nc);
	*auth_h = test;
}
/**
 * generate auth msrp mesage
 * @param pool
 * @param cred
 * @param port port of socket want to use
 * @param id id in msrp_path , example: msrp://alice.example.com:7394/<id>;tcp
 */
lmsrp_mess* lmsrp_gen_auth_mess(pj_pool_t *pool, lmsrp_cred_info *cred,
		int port, pj_str_t *session_id, lmsrp_transport_type pro) {
	lmsrp_mess *res = lmsrp_mess_create_request(pool, session_id,
			&msrp_auth_method);

	lmsrp_uri *to = lmsrp_uri_create(pool, &cred->realm, port, session_id,
			&cred->username, pro);
	res->to_path = lmsrp_list_uri_create(pool, to);
	res->from_path = res->to_path;
	return res;
}

/**
 * create authorizate message for 401 respone
 * @param mess MSRP auth Message
 * @param respone   MSRP 401  Message
 * @param credential info
 * @return string contain  MSRP auth Message with authorization header
 */
pj_status_t lmsrp_authorizate_mess(lmsrp_mess *mess, lmsrp_mess *respone,
		lmsrp_cred_info *cred, pj_str_t *out_put) {
	lmsrp_authorization_h *auth_h;
	char buff[40];
	msrp_make_authorization_header(respone, cred, &auth_h, buff);
	mess->auth = auth_h;
	out_put->slen = lmsrp_mess_tostring(mess, out_put->ptr, out_put->slen);
	return 1;
}
#else
static void msrp_make_authorization_header(lmsrp_mess *resp,
		pjsip_cred_info *cred, lmsrp_authorization_h **auth_h, char *buff)

{

	lmsrp_www_h *www = resp->www;
	pj_pool_t *pool = www->pool;
	pj_str_t res = { buff, 32 };

	pj_str_t cnonce = { buff + 32, 8 };
	pj_create_random_string(cnonce.ptr, cnonce.slen);
	pjsip_auth_create_digest(&res, &www->nonce, &msrp_nc, &cnonce, &www->qop,
			&resp->tid, &www->realm, cred, &msrp_auth_method);
	lmsrp_authorization_h *test;
	test = lmsrp_authorization_h_create(pool, &www->scheme, &cred->username,
			&www->realm, &www->nonce, &resp->tid, &res,
			NULL, &cnonce, &www->opaque, &www->qop, &msrp_nc);
	*auth_h = test;
}
/**
 * generate auth msrp mesage
 * @param pool
 * @param cred
 * @param port port of socket want to use
 * @param id id in msrp_path , example: msrp://alice.example.com:7394/<id>;tcp
 */
lmsrp_mess* lmsrp_gen_auth_mess(pj_pool_t *pool, pjsip_cred_info *cred,
		int port, pj_str_t *session_id, lmsrp_transport_type pro) {
	lmsrp_mess *res = lmsrp_mess_create_request(pool, session_id,
			&msrp_auth_method);

	lmsrp_uri *to = lmsrp_uri_create(pool, &cred->realm, port, session_id,
			&cred->username, pro);
	res->to_path = lmsrp_list_uri_create(pool, to);
	res->from_path = res->to_path;
	return res;
}
/**
 * create authorizate message for 401 respone
 * @param mess MSRP auth Message
 * @param respone   MSRP 401  Message
 * @param credential info
 * @return string contain  MSRP auth Message with authorization header
 */
pj_status_t lmsrp_authorizate_mess(lmsrp_mess *mess, lmsrp_mess *respone,
		pjsip_cred_info *cred, pj_str_t *out_put) {
	lmsrp_authorization_h *auth_h;
	char buff[40];
	msrp_make_authorization_header(respone, cred, &auth_h, buff);
	mess->auth = auth_h;
	out_put->slen = lmsrp_mess_tostring(mess, out_put->ptr, out_put->slen);
	return 1;
}
#endif
