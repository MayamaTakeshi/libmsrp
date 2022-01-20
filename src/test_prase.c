/*
 * test_prase.c
 *
 *  Created on: Sep 3, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>
#include <lmsrp_file.h>
extern pj_caching_pool cp;
//extern pj_pool_t *pool;

static void export(void *data, char *buff, int leng, lmsrp_mess *mess);
FILE *output_data;
int count = 1;
void test_prase(pj_pool_t *pool) {
	(void) pool;
	FILE *input_data = fopen("/tftp/trecv2", "rb");
	FILE *input_leng = fopen("/tftp/tlog", "rb");
	output_data = fopen("/tftp/testrecv", "wb");
	int read_lend = 0;
	lmsrp_context ctx;
	lmsrp_context_init(&ctx, &cp, 2000, NULL, export);
	char buff[4000];
	int prelen = 0;
	(void) prelen;
	int lcount = 0;
	(void) lcount;
//	while (!feof(input_leng)) {
//		prelen = read_lend ;
//		read_lend = 0;
//		fscanf(input_leng, "%d", &read_lend);
//		count ++ ;
//		if(count == 390)
//		{	// diem neo
//			 prelen = count ;
//		}
//		if (read_lend > 0) {
//			read_lend = fread(buff, 1, read_lend, input_data);
//			lmsrp_context_update(&ctx, buff, read_lend);
//		}
//	}
	while (!feof(input_data)) {
		read_lend = 1;
		read_lend = fread(buff, 1, read_lend, input_data);
		if (read_lend > 0)
			lmsrp_context_update(&ctx, buff, read_lend);

	}
	lmsrp_context_clear(&ctx);
	fclose(output_data);
	fclose(input_data);
	fclose(input_leng);
}

static void export(void *data, char *buff, int leng, lmsrp_mess *mess) {
	(void) data;
	(void) mess;
	fprintf(output_data, "%d\n", count);
	if (count == 14)
		printf("packet is: %d\n", count);

	count++;
	fwrite(buff, 1, leng, output_data);
	fprintf(output_data, "\n");
}
