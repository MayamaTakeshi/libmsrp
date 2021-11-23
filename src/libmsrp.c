/*
 ============================================================================
 Name        : libmsrp.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <lmsrp.h>

pj_caching_pool cp;
pj_pool_t *pool;
void test_msrp();
#define null NULL
static void init() {
	pj_init();
//	pjlib_util_init();
	pj_caching_pool_init(&cp, &pj_pool_factory_default_policy, 0);
	pool = pj_pool_create(&(cp.factory), "Sip pool", 4000, 4000, NULL);
	pj_log_set_level(5);
}
void test_msrp() {
	pj_str_t method = pj_str("SEND");
	pj_str_t sess_id = pj_str("sdwdeax");
	pj_str_t host = pj_str("1.1.1.1");
	pj_str_t user_1 = pj_str("pikakaka");
	pj_str_t user_2 = pj_str("d4p4pd");
	lmsrp_mess *mess = lmsrp_mess_create_request(pool, &sess_id, &method);
	mess->byte_range = lmsrp_byte_range_create(pool, 10088, 100000, 202020202);
	lmsrp_uri *urs = lmsrp_uri_create(pool, &host, 3343, &sess_id, &user_1,
			lmsrp_transport_type_tcp);
	lmsrp_uri *urd = lmsrp_uri_create(pool, &host, 3343, &sess_id, &user_2,
			lmsrp_transport_type_tcp);

	mess->to_path = lmsrp_list_uri_create(pool, urs);
	mess->from_path = lmsrp_list_uri_create(pool, urd);
	lmsrp_list_uri_add(mess->to_path, urs);
	char buff[1000];
	lmsrp_mess_tostring(mess, buff, 1000);
	puts(buff);

}

extern void test_prase(pj_pool_t *pool) ;
int main(int argc, char **argv) {

	init();
	test_prase(pool);
}
