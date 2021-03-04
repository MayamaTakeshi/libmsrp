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
	pjlib_util_init();
	pj_caching_pool_init(&cp, &pj_pool_factory_default_policy, 0);
	lpool = pj_pool_create(&(cp.factory), "Sip pool", 4000, 4000, NULL);
	pj_log_set_level(5);
}
int main(void) {
	init();
	char uri[] = " kieoc/hdweqhx wged/cpim dwgh/sgsg";
	int end = strlen(uri);
	lmsrp_content_type_h inf ;
	lmsrp_content_type_prase(lpool, &inf, uri, end);
	end = strlen(uri);
}
