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
void static test_export(void *data, char *buff, int leng, lmsrp_mess *rang) {
	PJ_LOG(1, ("export","allss"));
}
void test_msrp() {
	char *lms2 = "MSRP fdls SEND\n"
			"To-Path: msrp://test1@192.168.4.38;tcp\r\n"
			"From-Path: msrp://test1@192.168.4.38;tcp \r\n"
			"\r\n"
			"thien ha vo dich thach hac long"
			"-------fdls$\r\n";
	lmsrp_mess * ms = lmsrp_mess_create_from_buff(lpool, lms2, strlen(lms2));
	lmsrp_context ctx;
	lmsrp_context_init(&ctx, &cp, 4000, NULL, &test_export);
	int pkm = strlen(lms2);
	int step = 20;
	while (pkm > 0) {
		if (pkm < step)
			step = pkm;
		pkm = pkm - step;
		lmsrp_context_update(&ctx, lms2, step);
		lms2 = lms2 + step;
	}

	PJ_LOG(1, ("stop","all"));
}
void test_prase2() {
	pj_str_t log = { "hdgcwx aujh\0shdauuodghrs", 20 };
	pj_str_t find = { "dauuo", 5 };
	char *rm = pj_strstr(&log, &find);
	printf("%s", rm);
}
int main(void) {
	init();
//	char *lms2 = "MSRP fdls REPORT\n"
//			"To-Path: msrp://test1@192.168.4.38;tcp\r\n"
//			"From-Path: msrp://test1@192.168.4.38;tcp \r\n"
//			"-------fdls$\r\n";
//	char *test2 = "MSRP fdwq REPORT\n"
//			"To-Path: msrp://t3w31@192.168.4.58;tcp\r\n"
//			"From-Path: msrp://t3w31@192.168.4.58;tcp \r\n"
//			"-------fdls$\r\n";
//	test_prase2();
	test_msrp();

}
