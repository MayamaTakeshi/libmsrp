/*
 * lmsrp_session.h
 *
 *  Created on: Oct 20, 2021
 *      Author: amneiht
 */

#ifndef LMSRP_FILE_LMSRP_SESSION_H_
#define LMSRP_FILE_LMSRP_SESSION_H_

#include "lmsrp_module.h"
typedef struct lmsrp_send_stream {
	pj_pool_t *pool;
	pj_thread_t *thread_send_sess;
	pj_thread_t *thread_send_handle;
	lmsrp_report_list *list;
	lmsrp_session_info *info;
	lmsrp_flag flag;
	lmsrp_send_module *module;
	int block_size;
	int codec_size;
} lmsrp_send_stream;

typedef struct lmsrp_recv_stream {
	pj_pool_t *pool;
	pj_caching_pool *cp;
	pj_thread_t *thread_recv_get;
	pj_thread_t *thread_recv_handle;
	pj_thread_t *thread_recv_stream;
	lmsrp_block_list *block;
	lmsrp_recv_data_block *rblock;
	lmsrp_flag flag;
	lmsrp_session_info *info;
	lmsrp_recv_module *module;
	int block_size;
	pj_int64_t sizeof_file;
} lmsrp_recv_stream;
typedef struct {
	/**
	 * comute decode leng of size input
	 * @param size
	 * @return leng of in
	 */
	pj_uint32_t (*decode_leng)(pj_uint32_t size);
	/**
	 * compute encode leng from size input
	 * @param input_size
	 * @return
	 */
	pj_uint32_t (*encode_leng)(pj_uint32_t size);

	/**
	 * encode data
	 * @param input
	 * @param input_length
	 * @param output
	 * @param output_length leng of buffer
	 * @return 0 if ok
	 */
	pj_status_t (*encode)(void *input, pj_uint32_t input_length, void *output,
			pj_uint32_t *output_length);
	/**
	 * decode data
	 * @param input
	 * @param input_length
	 * @param output
	 * @param output_length
	 * @return
	 */
	pj_status_t (*decode)(void *input, pj_uint32_t input_length, void *output,
			pj_uint32_t *output_length);
} lmsrp_codec;
void lmsrp_send_stream_init(lmsrp_send_stream *send,
		lmsrp_send_module *module);
void lmsrp_send_stream_start(pj_pool_t *pool, lmsrp_session_info *info,
		lmsrp_send_stream *send);
void lmsrp_send_stream_close(lmsrp_send_stream *close);

void lmsrp_recv_stream_init(lmsrp_recv_stream *send,
		lmsrp_recv_module *module);
void lmsrp_recv_stream_start(pj_pool_t *pool, lmsrp_session_info *info,
		lmsrp_recv_stream *stream, pj_int64_t filesize);
void lmsrp_recv_stream_close(lmsrp_recv_stream *stream);

// msrp encode funtion
lmsrp_codec* lmsrp_r128_create(pj_pool_t *pool);
lmsrp_codec* lmsrp_base64_create(pj_pool_t *pool);



#endif /* LMSRP_FILE_LMSRP_SESSION_H_ */
