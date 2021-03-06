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
static char msrp_mess[] =
				"MSRP fhdw REPORT\r\n"
				"To-Path: msrp://test1@192.168.4.38;tcp\r\n"
				"From-Path: msrp://test1@192.168.4.38:354/s.64.8.1017189353;tcp msrp://dccan@192.168.4.38/s.63.8.1126255474;tcp\r\n"
				"Message-Id :fbedekjd\r\n"
				"Byte-Range: 1-16/16\r\n"
				"Failure-Report :no\r\n"
				"\r\n"
				"abbdefghiklmcwqvawrcf\n--------aerqgvWFDFYREVBAEWRH6BVQ32nopq\r\n"
				"-------fhdw$\r\n\r\n";
void init() {
	pj_init();
//	pjlib_util_init();
	pj_caching_pool_init(&cp, &pj_pool_factory_default_policy, 0);
	lpool = pj_pool_create(&(cp.factory), "Sip pool", 4000, 4000, NULL);
	pj_log_set_level(5);
}
int main(void) {
	init();

	int end = strlen(msrp_mess);
	lmsrp_mess *inf;
	inf = lmsrp_mess_create_from_buff(lpool, msrp_mess, end);
	char buff[1000];
	pj_bzero(buff, 1000);
	int i = lmsrp_mess_tostring(inf, buff, 1000);
	fprintf(stderr,"%.*s",i,buff);
	fprintf(stderr,"data leng %d",i);
	end = strlen(msrp_mess);
}
