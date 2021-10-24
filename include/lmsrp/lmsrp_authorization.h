/*
 * lmsrp_authorization.h
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#ifndef LMSRP_AUTHORIZATION_H_
#define LMSRP_AUTHORIZATION_H_
#include <lmsrp.h>

typedef struct _lmsrp_header_authorization {
	pj_pool_t *pool;
	pj_str_t scheme;
	pj_str_t username;
	pj_str_t realm;
	pj_str_t nonce;
	pj_str_t uri;
	pj_str_t response;
	pj_str_t algorithm;
	pj_str_t cnonce;
	pj_str_t opaque;
	pj_str_t qop;
	pj_str_t nc;
	lmsrp_list_param *params;
} lmsrp_authorization_h;

void lmsrp_authorization_header_prase(pj_pool_t *pool,
		lmsrp_authorization_h *dst, char *data, int end);
int lmsrp_authorization_header_to_string(lmsrp_authorization_h *header,
		char *buff, int size);

lmsrp_authorization_h* lmsrp_authorization_h_create(pj_pool_t *pool,
		pj_str_t *scheme, pj_str_t *username, pj_str_t *realm, pj_str_t *nonce,
		pj_str_t *uri, pj_str_t *response, pj_str_t *algorithm,
		pj_str_t *cnonce, pj_str_t *opaque, pj_str_t *qop, pj_str_t *nc);

// define for extend data
#if defined (LMSRP_ONLY_PJLIB) && LMSRP_ONLY_PJLIB == 1

typedef enum lmsrp_cred_data_type
{
    LMSRP_CRED_DATA_PLAIN_PASSWD=0, /**< Plain text password.		*/
    LMSRP_CRED_DATA_DIGEST	=1, /**< Hashed digest.			*/

    LMSRP_CRED_DATA_EXT_AKA	=16 /**< Extended AKA info is available */

} lmsrp_cred_data_type;

typedef struct lmsrp_cred_info {
	pj_str_t realm; /**< Realm. Use "*" to make a credential that
	 can be used to authenticate against any
	 challenges.			    */
	pj_str_t scheme; /**< Scheme (e.g. "digest").		    */
	pj_str_t username; /**< User name.				    */
	int data_type; /**< Type of data (0 for plaintext passwd). */
	pj_str_t data; /**< The data, which can be a plaintext
	 password or a hashed digest.	    */

} lmsrp_cred_info;

void lmsrp_auth_create_digest(pj_str_t *result, const pj_str_t *nonce,
		const pj_str_t *nc, const pj_str_t *cnonce, const pj_str_t *qop,
		const pj_str_t *uri, const pj_str_t *realm,
		const lmsrp_cred_info *cred_info, const pj_str_t *method);

#else
//	#define lmsrp_cred_info pjsip_cred_info
typedef struct pjsip_cred_info lmsrp_cred_info;
#endif
#endif /* LMSRP_AUTHORIZATION_H_ */
