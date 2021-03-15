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
pj_pool_t *lpool;

void init() {
	pj_init();
//	pjlib_util_init();
	pj_caching_pool_init(&cp, &pj_pool_factory_default_policy, 0);
	lpool = pj_pool_create(&(cp.factory), "Sip pool", 4000, 4000, NULL);
	pj_log_set_level(5);
}
void static test_export(void *data, char *buff, int leng,
		lmsrp_byte_range *rang) {
}
void test_msrp() {
	char *lms = "----fdls$\r\n\r\n" ;
	char *lms2 = "MSRP fdls REPORT\n"
			"To-Path: msrp://test1@192.168.4.38;tcp\r\n"
			"From-Path: msrp://test1@192.168.4.38;tcp \r\n"
			"\r\n"
			"wvxhqjwxujqhxwgxghqsuxhlsjdhwjshhsssusu\n-------fdls$\r\n\r";
	lmsrp_context ctx;
	lmsrp_context_init(&ctx, lpool, 1000, NULL, &test_export);
	lmsrp_context_update(&ctx, lms2, strlen(lms2));
//	lmsrp_context_update(&ctx, lms, strlen(lms));
	PJ_LOG(1, ("stop","all"));
}
int main(void) {
	init();
	test_msrp();
}
