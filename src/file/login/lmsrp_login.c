/*
 * msrp_auth.c
 *
 *  Created on: Feb 25, 2021
 *      Author: amneiht
 *     this use for msrp AUTH method
 */

#include <lmsrp_file.h>


pj_str_t* lmsrp_login(pj_pool_t *pool, void *sock, lmsrp_transport_module *car,
		lmsrp_cred_info *cred, pj_sockaddr_in *server, int local_port,
		pj_str_t *sessid) {
#if defined (LMSRP_ONLY_PJLIB) && LMSRP_ONLY_PJLIB == 1
	lmsrp_mess *mess = lmsrp_gen_auth_mess(pool, (lmsrp_cred_info *)cred, local_port, sessid,
			lmsrp_transport_type_tcp);
#else
	lmsrp_mess *mess = lmsrp_gen_auth_mess(pool, cred, local_port, sessid,
				lmsrp_transport_type_tcp);
#endif

	// create message
	char buff[2000];
	pj_str_t out;
	out.ptr = buff;
	out.slen = lmsrp_mess_tostring(mess, out.ptr, 1000);
	// sending mess age
	pj_status_t st = car->connect(sock, server);
	if (st != PJ_SUCCESS) {
		printf("loi xmnr \n");
		return NULL;
	}
	pj_str_t m_recv = { buff + 1000, 1000 };
	int dem = 10;
	while (1) {
		dem--;
		if (dem < 0)
			return NULL;
		car->send(sock, out.ptr, out.slen);
		car->recv(sock, m_recv.ptr, &m_recv.slen);
		lmsrp_mess *au = lmsrp_mess_create_from_buff(pool, m_recv.ptr,
				m_recv.slen);
		if (au->type == lmsrp_mess_type_respone) {
			if (au->info.respone.code == 200) {
				PJ_LOG(1, (__FILE__,"msrp login succes full"));
				pj_str_t *rs = pj_pool_alloc(pool, sizeof(pj_str_t));
				pj_strdup(pool, rs, &au->use_path->uri->session_id);
				return rs;
			} else if (au->info.respone.code == 401
					|| au->info.respone.code == 407) {
				out.slen = 1000;
#if defined (LMSRP_ONLY_PJLIB) && LMSRP_ONLY_PJLIB == 1
				lmsrp_authorizate_mess(mess, au, (lmsrp_cred_info *)cred, &out);
#else
				lmsrp_authorizate_mess(mess, au, cred, &out);
#endif
			}
		} else {
			PJ_LOG(1, (__FILE__,"msrp login error"));
			return NULL;
		}
	}
}

