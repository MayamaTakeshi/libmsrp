/*
 * test_prase.c
 *
 *  Created on: Sep 3, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>
char mess[] =
		"MSRP fewr1 401 Unauthorized\r\n"
				"To-Path: msrp://test1@192.168.5.195/fewr1;tcp\r\n"
				"From-Path: msrp://test1@192.168.5.195/fewr1;tcp\r\n"
				"WWW-Authenticate: Digest realm=\"192.168.5.195\", nonce=\"YTHn8mEx5sb3wLeegDFRUsu/k3j5UrmedUbnrIA=\", qop=\"auth\", opaque=\"c52854eebc40626ab22f751cf58bc722\"\r\n"
				"-------fewr1$\r\n";
char mess2[] =
		"MSRP fewr1 AUTH\r\n"
				"To-Path: msrp://test1@192.168.5.195/fewr1;tcp\r\n"
				"From-Path: msrp://test1@192.168.5.195/fewr1;tcp\r\n"
				"Authorization: Digest realm=\"192.168.5.195\",nc=00000001,nonce=\"YTH1yWEx9J1EaV8WbK5HHT/sFK87S25kdUbzuIA=\",cnonce=\"53fd7356\",opaque=\"c52854eebc40626ab22f751cf58bc722\",username=\"test1\",response=\"9622da1ea7b19bcfee6cfcf2422fb795\",uri=\"fewr1\",qop=auth\r\n"
				"-------fewr1$\r\n";
void test_prase(pj_pool_t *pool) {
	lmsrp_mess *ms = lmsrp_mess_create_from_buff(pool, mess2, strlen(mess2));
	char buff[1000];
	memset(buff, 0, 1000);
	lmsrp_mess_tostring(ms, buff, 1000);
	puts(buff);
}
void test_authenticaion(pj_pool_t *pool) {

	lmsrp_cred_info inf;
#if LMSRP_ONLY_PJLIB == 1
	inf.data_type = LMSRP_CRED_DATA_PLAIN_PASSWD;
#else
	inf.data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
#endif
	inf.realm = pj_str("192.168.5.195");
	inf.username = pj_str("test1");
	pj_str_t sess_id = pj_str("dsqwwde");
	inf.data = pj_str("123456");

	lmsrp_mess *ms = lmsrp_gen_auth_mess(pool, &inf, 1222, &sess_id,
			lmsrp_transport_type_tcp);
	pj_str_t mss = pj_str("message");
	pj_str_t cpim = pj_str("cpim");
	ms->content_type = lmsrp_content_type_create(pool, &mss, &cpim);
	char buff[1000];
	memset(buff, 0, 1000);
	lmsrp_mess_tostring(ms, buff, 1000);
	lmsrp_mess *au = lmsrp_mess_create_from_buff(pool, mess, strlen(mess));
//	pj_str_t *ps = lmsrp_authorizate_mess(ms, au, &inf);
//	puts(ps->ptr);
}
