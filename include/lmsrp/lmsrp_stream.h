/*
 * lmsrp_stream.h
 *
 *  Created on: Mar 13, 2021
 *      Author: amneiht
 */

#ifndef LMSRP_LMSRP_STREAM_H_
#define LMSRP_LMSRP_STREAM_H_
#include <lmsrp.h>
typedef enum lmsrp_prase_state {
	lmsrp_prase_state_mess, // prase first msrp line
	lmsrp_prase_state_header, // header is parsing
	lmsrp_prase_state_content, // the program is parsing contend msrp
	lmsrp_prase_state_unknow, // we are pass sing un know data
	lmsrp_prase_state_done // previous data is complete
} lmsrp_prase_state;

typedef struct lmsrp_context {
	pj_int32_t max_byte;
	pj_pool_t *pool;
	pj_pool_t *spool;
	lmsrp_prase_state state;
	lmsrp_mess *mess;
	int data_read; /// number of data is prase in mess
	pj_size_t content_leng; /// number of data read in contend lend
	char tid[20];
	char *tmp; /// tmp buffer to keep previous data size is  2*(maxbyte + 500)
	int sum; /// sum of data in tmp buffer

	void *data;
	/**
	 * export message data for user callback
	 * @param data  user data
	 * @param buff point to data
	 * @param leng size of data to export
	 * @param rang byte rang header
	 */
	void (*export)(void *data, char *buff, int leng, lmsrp_mess *arg);
} lmsrp_context;

pj_bool_t lmsrp_stream_prase(lmsrp_context *ctx, char *data, int end);
void lmsrp_context_init(lmsrp_context *ctx, pj_caching_pool *cp, int max_size,
		void *data,
		void (*export)(void *data, char *buff, int leng, lmsrp_mess *arg));
void lmsrp_context_clear(lmsrp_context *ctx);
pj_bool_t lmsrp_context_update(lmsrp_context *ctx, char *buff, pj_int32_t size);

#endif /* LMSRP_LMSRP_STREAM_H_ */
