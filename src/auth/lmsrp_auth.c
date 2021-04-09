/*
 * lmsrp_auth.c
 *
 *  Created on: Apr 9, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>
static pj_str_t msrp_auth_method = { "AUTH", 4 };
static pj_str_t msrp_nc = { "00000001", 8 };
static void amsip_msrp_make_authorization_header(lmsrp_mess *resp,
		pjsip_cred_info *cred, lmsrp_authorization_h **auth_h) {

	lmsrp_www_h *www = resp->www;
	pj_pool_t *pool = www->pool;
// MD5 default
	pj_str_t *res = pj_pool_alloc(pool, sizeof(pj_str_t));
	res->ptr = pj_pool_alloc(pool, 32 * sizeof(char));
	res->slen = 32;
	pj_str_t *cnonce = pj_pool_alloc(pool, sizeof(pj_str_t));
	cnonce->ptr = pj_pool_alloc(pool, 8 * sizeof(char));
	cnonce->slen = 8;
//	amsip_randomstring2(cnonce->ptr, cnonce->slen);
	pj_create_random_string(cnonce->ptr, cnonce->slen);
	pjsip_auth_create_digest(res, &www->nonce, &msrp_nc, cnonce, &www->qop,
			&resp->tid, &www->realm, cred, &msrp_auth_method);
	lmsrp_authorization_h *test;
	test = lmsrp_authorization_h_create(pool, &www->scheme, &cred->username,
			&www->realm, &www->nonce, &resp->tid, res,
			NULL, cnonce, &www->opaque, &www->qop, &msrp_nc);
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
		int port, pj_str_t *session_id, lmsrp_transport pro) {
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
pj_str_t* lmsrp_authorizate_mess(lmsrp_mess *mess, lmsrp_mess *respone,
		pjsip_cred_info *cred) {
	lmsrp_authorization_h *auth_h;
	amsip_msrp_make_authorization_header(respone, cred, &auth_h);
	mess->auth = auth_h;
	pj_str_t *res = pj_pool_alloc(mess->pool, sizeof(pj_str_t));
	res->ptr = pj_pool_alloc(mess->pool, 1000 * sizeof(char));
	res->slen = 1000;
	res->slen = lmsrp_mess_tostring(mess, res->ptr, res->slen);
	return res;
}
