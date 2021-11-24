/*
 * demo.c
 *
 *  Created on: Nov 23, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>
#include <lmsrp_file.h>

static pj_caching_pool cp;
static pj_pool_t *pool;
static void init_lib() {
	pj_init();
#if LMSRP_ONLY_PJLIB == 0
	pjlib_util_init();
#endif
	pj_caching_pool_init(&cp, &pj_pool_factory_default_policy, 0);
	pool = pj_pool_create(&(cp.factory), "Sip pool", 4000, 4000, NULL);
	// set log level of pjlib
	pj_log_set_level(2);
}
void close_lib() {
	pj_pool_release(pool);
	pj_caching_pool_destroy(&cp);
	pj_shutdown();
}
void demo_msrp() {
	init_lib();

	close_lib();
}
