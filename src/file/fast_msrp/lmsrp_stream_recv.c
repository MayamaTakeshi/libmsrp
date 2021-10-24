/*
 * stream_recv.c
 *
 *  Created on: Aug 3, 2021
 *      Author: amneiht
 */

#include <lmsrp_file.h>
#define t_name "msrp_get.c"
static pj_str_t report = { "REPORT", 6 };
#define t_name "msrp_get.c"

extern lmsrp_block_list* lmsrp_get_free_block(pj_pool_t *pool,
		lmsrp_block_list *block);
extern lmsrp_recv_data_block* lmsrp_get_recv_data_block(pj_pool_t *pool,
		lmsrp_recv_data_block *block, int count);
extern lmsrp_recv_data_block* lmsrp_get_recv_free_block(pj_pool_t *pool,
		lmsrp_recv_data_block *block, int block_size);
extern void lmsrp_init_recv_block(pj_pool_t *pool,
		lmsrp_recv_data_block **block, int size, int block_size);
static int lmsrp_get_handle(void *arg);
extern pj_status_t lmsrp_init_data_block(pj_pool_t *pool,
		lmsrp_block_list **block, pj_int64_t size, int block_size);

static int session_stream_prase(void *arg);

static int lmsrp_get_socket(void *arg) {
	PJ_LOG(2, (t_name,"stat recv"));
	lmsrp_recv_stream *sess = arg;
	lmsrp_recv_data_block *block;
	const int recv_size = sess->block_size * 3;
	pj_pool_t *pool = sess->pool;
	lmsrp_recv_module *module = sess->module;
	module->init(module->data, sess->info->filename);
	block = sess->rblock;
//	block->size = recv_size;
	pj_status_t st;
	pj_thread_create(pool, "recvweet", &lmsrp_get_handle, arg,
	PJ_THREAD_DEFAULT_STACK_SIZE, 0, &sess->thread_recv_handle);
	pj_int32_t count = 1;
	pj_thread_create(pool, "recv get", &session_stream_prase, arg,
	PJ_THREAD_DEFAULT_STACK_SIZE, 0, &sess->thread_recv_stream);
//	PJ_LOG(1, (t_name ,"start connect"));
	while (sess->flag != lmsrp_flag_close && sess->flag != lmsrp_flag_suspend) {
		block->size = recv_size;
		st = module->get_mess(pool, module->data, block);
//		PJ_LOG(1, (t_name ,"st connect is %d", st));
		if (st == PJ_SUCCESS) {
			if (block->size > 0) {
//				PJ_LOG(1, (t_name ,"put data %d", count));
				block->package_count = count;
				count++;
				block->state = lmsrp_block_state_recv;
				block = lmsrp_get_recv_free_block(pool, block,
						sess->block_size);
			} else {
				PJ_LOG(1, (t_name ,"socket is broken %d\n", block->size ));
				sess->flag = lmsrp_flag_suspend;
			}
		}
	}
	PJ_LOG(1, (t_name ,"end connect"));
	if (sess->flag == lmsrp_flag_close)
		module->close(module->data, 0);
	else
		module->close(module->data, 1);
	return PJ_SUCCESS;
}
static void export(void *data, char *buff, int leng, lmsrp_mess *mess) {
	if (leng == 0) {
		PJ_LOG(3, (t_name,"no data to export"));
		return;
	}
	lmsrp_recv_stream *sess = data;
	lmsrp_block_list *blg = lmsrp_get_free_block(sess->pool, sess->block);
	blg->state = lmsrp_block_state_raw;
	pj_memcpy(&blg->range, mess->byte_range, sizeof(lmsrp_byte_range));
	pj_memcpy(blg->data, buff, leng);
	blg->size = leng;
	blg->flag = mess->flag;
	blg->state = lmsrp_block_state_ready;
}
static int session_stream_prase(void *arg) {
//	PJ_LOG(3, (t_name,"start stream parse"));
	lmsrp_recv_stream *sess = arg;
	lmsrp_recv_data_block *rblock;
	lmsrp_recv_data_block *rblock2;
	rblock = sess->rblock->prev;
	lmsrp_context ctx;
	pj_bool_t rp;
	pj_int32_t count = 1;
	lmsrp_context_init(&ctx, sess->module->get_cp(sess->module),
			sess->block_size + 500, arg, &export);

	pj_pool_t *pool = sess->pool;
	while (sess->flag != lmsrp_flag_close && sess->flag != lmsrp_flag_suspend) {
//		puts("ssaaa wws sss  tun");
		rblock2 = lmsrp_get_recv_data_block(pool, rblock, count);
		if (rblock2 != NULL) {
//			puts("write data");
			int ls = rblock2->size;
			rp = lmsrp_context_update(&ctx, rblock2->data, rblock2->size);
			if (rp == PJ_FALSE) {
				fprintf(stderr, "ls %d\n", ls);
			}
			rblock2->package_count = 0;
			rblock2->size = 0;
			count++;
			rblock2->state = lmsrp_block_state_free;
			rblock = rblock2;

		}
	}
	lmsrp_context_clear(&ctx);
	return PJ_SUCCESS;
}
static int lmsrp_get_handle(void *arg) {
	PJ_LOG(2, (t_name,"star handle"));
	int start = 1;
	lmsrp_recv_stream *sess = arg;
	lmsrp_recv_module *module = sess->module;
	int end = sess->sizeof_file + 1;
	pj_str_t ran = pj_str("dwijda");
	pj_pool_t *pool = sess->pool;
	lmsrp_codec *codec = lmsrp_r128_create(pool);
	// tinh toan gia tri sau khi decode data
	const int realleng = codec->decode_leng(sess->block_size) + 1;
	char decode_buff[realleng];
	lmsrp_mess *mess = lmsrp_mess_create_request(pool, &ran, &report);
	mess->to_path = lmsrp_list_uri_create(pool, sess->info->caller);
	lmsrp_list_uri_add(mess->to_path, sess->info->caller);
	mess->from_path = lmsrp_list_uri_create(pool, sess->info->callee);
	mess->status = lmsrp_status_h_create(pool, 0, 413, NULL);
	mess->status->rfc_code = 413;
	mess->byte_range = lmsrp_byte_range_create(pool, 0, 0, 0);
	pj_int32_t min_start; // min start we can find
	lmsrp_block_list *neo, *next;
	neo = sess->block;
	next = neo;
	const int period = 2;
	int time = 7; // 10 s
	pj_time_val past, smp;
	pj_gettimeofday(&past);
	min_start = sess->sizeof_file;
	char buff[1000];
	pj_uint32_t dem;
	dem = module->create_header(buff, &sess->info->caller->authority.userinfo,
			sess->sizeof_file, 0);
	module->write_header(module->data, buff, dem);
	while (start < end && sess->flag != lmsrp_flag_close
			&& sess->flag != lmsrp_flag_suspend) {
		next = neo;
		do {
			if (next->state == lmsrp_block_state_ready) {
				if (next->flag == '#') {
					sess->flag = lmsrp_flag_suspend;
					return 1;
				}
				if (next->range.start <= start) {
					pj_gettimeofday(&past);
					min_start = sess->sizeof_file;
					if (start < next->range.end + 1)
						start = next->range.end + 1;
					dem = realleng;
					codec->decode(next->data, next->size, decode_buff, &dem);
					module->write(module->data, decode_buff, next->range.start,
							next->range.end);
					time = 4;
					next->state = lmsrp_block_state_free;
				} else {
					if (next->range.start < min_start) {
						min_start = next->range.start;
					}
				}
			}
			next = next->next;
		} while (neo != next);
		pj_gettimeofday(&smp);
		if (smp.sec - past.sec > period) {
			mess->byte_range->start = start;
			mess->byte_range->end = min_start;
			dem = lmsrp_mess_tostring(mess, buff, 1000);
			PJ_LOG(1, (t_name,"report"));
			module->send_report(module->data, buff, dem);
			pj_gettimeofday(&past);
			time--;
			if (time < 0) {
				sess->flag = lmsrp_flag_suspend;
			}
		}
	}
	if (start == end) {
		mess->status->rfc_code = 200;
		dem = lmsrp_mess_tostring(mess, buff, 1000);
		module->send_report(module->data, buff, dem);
		sess->flag = lmsrp_flag_close;
	} else {
		sess->flag = lmsrp_flag_suspend;
	}
	return PJ_SUCCESS;
}

void lmsrp_recv_stream_start(pj_pool_t *pool, lmsrp_session_info *info,
		lmsrp_recv_stream *stream, pj_int64_t file_size) {

	stream->info = info;
	stream->pool = pool;
	stream->module->pool = pool;
	pj_int64_t d = (file_size / stream->block_size) + 1;
	if (d > 5)
		d = 5;
	lmsrp_init_data_block(pool, &stream->block, d, stream->block_size + 500);
	lmsrp_init_recv_block(pool, &stream->rblock, d, stream->block_size + 500);
	stream->flag = lmsrp_flag_continue;
	stream->sizeof_file = file_size;
	pj_thread_create(pool, "recv get", &lmsrp_get_socket, stream,
	PJ_THREAD_DEFAULT_STACK_SIZE, 0, &stream->thread_recv_get);
}
void lmsrp_recv_stream_close(lmsrp_recv_stream *stream) {
	stream->flag = lmsrp_flag_suspend;
	if (stream->thread_recv_get) {
		pj_thread_join(stream->thread_recv_get);
		pj_thread_destroy(stream->thread_recv_get);
		stream->thread_recv_get = NULL;
	}
	if (stream->thread_recv_handle) {
		pj_thread_join(stream->thread_recv_handle);
		pj_thread_destroy(stream->thread_recv_handle);
		stream->thread_recv_handle = NULL;
	}
	if (stream->thread_recv_stream) {
		pj_thread_join(stream->thread_recv_stream);
		pj_thread_destroy(stream->thread_recv_stream);
		stream->thread_recv_stream = NULL;
	}
	stream->pool = NULL;
}
void lmsrp_recv_stream_init(lmsrp_recv_stream *recv, lmsrp_recv_module *module) {
	pj_bzero(recv, sizeof(lmsrp_recv_stream));
	recv->module = module;
}
