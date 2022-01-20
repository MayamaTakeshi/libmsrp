/*
 * lmsrp.h
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#ifndef LMSRP_H_
#define LMSRP_H_

#include <pjlib.h>
#include <lmsrp/lmsrp_config.h>
#if ! defined (LMSRP_ONLY_PJLIB) ||  LMSRP_ONLY_PJLIB != 1
	#include <pjlib-util.h>
	#include <pjsip.h>
#endif

#include <lmsrp/lmsrp_util.h>
#include <lmsrp/lmsrp_common.h>
#include <lmsrp/lmsrp_www_auth.h>
#include <lmsrp/lmsrp_auth_commom.h>
#include <lmsrp/lmsrp_authorization.h>
#include <lmsrp/lmsrp_info.h>
#include <lmsrp/lmsrp_status.h>
#include <lmsrp/lmsrp_byte_range.h>
#include <lmsrp/lmsrp_content_type.h>
#include <lmsrp/lmsrp_mess.h>
#include <lmsrp/lmsrp_stream.h>


typedef enum _lmsrp_transport_type {
	lmsrp_transport_type_udp, lmsrp_transport_type_tcp, lmsrp_transport_type_tls
} lmsrp_transport_type;


#if defined (LMSRP_ONLY_PJLIB) && LMSRP_ONLY_PJLIB == 1
lmsrp_mess* lmsrp_gen_auth_mess(pj_pool_t *pool, lmsrp_cred_info *cred,
		int port, pj_str_t *session_id, lmsrp_transport_type pro);
pj_status_t lmsrp_authorizate_mess(lmsrp_mess *mess, lmsrp_mess *respone,
		lmsrp_cred_info *cred , pj_str_t* out_put);
#else
lmsrp_mess* lmsrp_gen_auth_mess(pj_pool_t *pool, pjsip_cred_info *cred,
		int port, pj_str_t *session_id, lmsrp_transport_type pro);
pj_status_t lmsrp_authorizate_mess(lmsrp_mess *mess, lmsrp_mess *respone,
		pjsip_cred_info *cred , pj_str_t* out_put);
#endif

#endif /* LMSRP_H_ */


// todo : sua lai encrypt mote
// todo : quan ly phan gui lai tuong thich e]voi encrypt mode
// todo sua lai luong
// todo : gioi han khich thuoc nhan
