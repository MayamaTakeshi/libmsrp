/*
 * msrp_fsend.c
 *
 *  Created on: Mar 11, 2021
 *      Author: amneiht
 */

#include <lmsrp_file.h>

static int lmsrp_report_handle(void *arg);

static int find_report(void *value, const pj_list_type *node) {
	pj_bool_t ss = *((pj_bool_t*) value);
	const lmsrp_report_list *bl = (const lmsrp_report_list*) node;
//	pj_bool_t st = ((lmsrp_report_list*) node)->active;
	pj_bool_t st = bl->active;
	if (st == ss)
		return 0;
	return 1;
}
static lmsrp_report_list* new_node(pj_pool_t *pool) {
	lmsrp_report_list *res = pj_pool_alloc(pool, sizeof(lmsrp_report_list));
	res->active = PJ_FALSE;
	return res;
}

static lmsrp_report_list* am_sip_send_get_report(lmsrp_report_list *list) {
	static int ls = PJ_TRUE;
	return pj_list_search(list, &ls, &find_report);
}
static lmsrp_report_list* init_report_list(pj_pool_t *pool, int size) {

	lmsrp_report_list *res = new_node(pool);
	pj_list_init(res);
	for (int i = 1; i < size; i++) {
		lmsrp_report_list *add = new_node(pool);
		pj_list_insert_after(res, add);
	}
	return res;
}
static lmsrp_report_list* am_sip_send_get_free_lot(pj_pool_t *pool,
		lmsrp_report_list *list) {
	static int ls = PJ_FALSE;
	lmsrp_report_list *res = pj_list_search(list, &ls, &find_report);
	if (res == NULL) {
		res = new_node(pool);
		pj_list_insert_after(list, res);
	}
	return res;
}
// TODO : add support for pre encode base 64
/**
 *
 * @param arg module data
 * @return
 * to ensure the message is not drop by msrp server such as Kamailio , etc
 * the contend of message is auto encode with base64 or base128 ( 7byte -> 8 byte)
 * pre encode is support on future
 */
#if LM_STABLE == 1
static int lmsrp_send_session(void *arg) {
	PJ_CHECK_STACK();
	lmsrp_send_stream *sends = arg;
	lmsrp_send_module *module = sends->module;
	pj_pool_t *pool = module->pool;
	pj_int64_t total = module->init(module->data, sends->info->filepath);
	pj_ssize_t start, end;
	start = 1;
	end = 1;

#if LMSRP_DEBUG == 1
	FILE *tmpsend = fopen("/tmp/tsend.log", "wb");
	int count = 1;
# endif
//	lmsrp_codec *codec = lmsrp_r128_create(pool);
	lmsrp_codec *codec = lmsrp_base64_create(pool);
	int pre_read = codec->decode_leng(sends->block_size);
	if (module->decrypt_leng != NULL) {
		pre_read = module->decrypt_leng(module->data, pre_read);
	}
	const int byte_read = pre_read;
	sends->codec_size = pre_read;

	const int leng = sends->block_size + 500;
	char mess_send[leng]; //max_data  in messs
	char contend[sends->block_size];
	char readbuff[sends->block_size];
	const int messid_lg = 8;
	char rand[messid_lg];
	pj_create_random_string(rand, messid_lg);
	pj_str_t sessid = { rand, messid_lg };
	pj_str_t msend = { "SEND", 4 };
	lmsrp_mess *mess = lmsrp_mess_create_request(pool, &sessid, &msend);
	lmsrp_uri *to_uri = sends->info->callee;
	mess->to_path = lmsrp_list_uri_create(pool, to_uri);
	lmsrp_list_uri_add(mess->to_path, to_uri);
	mess->from_path = lmsrp_list_uri_create(pool, sends->info->caller);

	if (total == 0) {
		PJ_LOG(3, (__LNAME__,"data is failse"));
		module->close(module->data);
		return PJ_SUCCESS;
	}
	lmsrp_byte_range *byte = lmsrp_byte_range_create(pool, 1, 0, total);
	mess->byte_range = byte;
	mess->flag = '+';
	mess->contend.ptr = contend;

	// create handle thread
	pj_thread_create(module->pool, "send handle", lmsrp_report_handle, sends,
	PJ_THREAD_DEFAULT_STACK_SIZE, 0, &sends->thread_send_handle);
	lmsrp_report_list *res;
	pj_uint32_t dem;
	int stime;
	if (LMSRP_LOW_MEMORY) {
		stime = 10;
	} else {
		stime = 2;
	}
#if LMSRP_DEBUG == 1
	stime = 10;
#endif
	while (sends->flag != lmsrp_flag_close && sends->flag != lmsrp_flag_suspend) {
		pj_thread_sleep(stime);
		res = am_sip_send_get_report(sends->list);
		if (res != NULL) {

			mess->byte_range->start = res->rang.start;
			mess->byte_range->end = res->rang.end;
			pj_create_random_string(rand, messid_lg);
			dem = module->read(module->data, readbuff, res->rang.start,
					res->rang.end);
			mess->contend.slen = sends->block_size;
			dem = codec->encode(readbuff, dem, mess->contend.ptr,
					(pj_uint32_t*) &mess->contend.slen);

			dem = lmsrp_mess_tostring(mess, mess_send, leng);
			module->send(module->data, mess_send, dem);
			res->active = PJ_FALSE;
		} else {
			if (end < total) {

				end = start + byte_read - 1;
				if (end > total) {
					mess->flag = '$';
					end = total;
					sends->flag = lmsrp_flag_end;
				}
				mess->byte_range->start = start;
				mess->byte_range->end = end;
				pj_create_random_string(rand, messid_lg);

				dem = module->read(module->data, readbuff, start, end);
				mess->contend.slen = sends->block_size;
//				printf("status %d encode size :%ld\n", dem, mess->contend.slen);
				dem = codec->encode(readbuff, dem, mess->contend.ptr,
						(pj_uint32_t*) &mess->contend.slen);
//				printf("status %d encode size :%d\n", dem, mess->contend.slen);
#if LMSRP_DEBUG == 1
				fprintf(tmpsend, "%d\n", count);
				count++;
				fwrite(mess->contend.ptr, sizeof(char),
						(int) mess->contend.slen, tmpsend);
				fprintf(tmpsend, "\n");
//				printf("send:\n%.*s\n", (int) mess->contend.slen,
//						mess->contend.ptr) ;
# endif
				dem = lmsrp_mess_tostring(mess, mess_send, leng);
//				printf("mess leng :%d\n", dem);
				module->send(module->data, mess_send, dem);

				start = start + byte_read;
			}
		}

	}
#if LMSRP_DEBUG == 1
	fclose(tmpsend);
# endif
	if (sends->flag == lmsrp_flag_suspend) {
		PJ_LOG(2, (__LNAME__,"suppend send sesson"));
		module->suppend(module->data);
	} else
		module->close(module->data);
	return PJ_SUCCESS;
}
#else
static int lmsrp_send_session(void *arg) {
	PJ_CHECK_STACK();
	lmsrp_send_stream *sends = arg;
	lmsrp_send_module *module = sends->module;
	pj_pool_t *pool = module->pool;
	pj_int64_t total = module->init(module->data, sends->info->filepath);
	pj_uint64_t start, end;
	start = 1;
	end = 1;

#if LMSRP_DEBUG == 1
	FILE *tmpsend = fopen("/tmp/tsend.log", "wb");
	int count = 1;
# endif
//	lmsrp_codec *codec = lmsrp_r128_create(pool);
	lmsrp_codec *codec = lmsrp_base64_create(pool);

	const size_t byte_read = codec->decode_leng(sends->block_size);
	sends->codec_size = byte_read;

	const int leng = sends->block_size + 500;
	char mess_send[leng]; //max_data  in messs
	char contend[sends->block_size];
	char readbuff[sends->block_size];
	const int messid_lg = 8;
	char rand[messid_lg];
	pj_create_random_string(rand, messid_lg);
	pj_str_t sessid = { rand, messid_lg };
	pj_str_t msend = { "SEND", 4 };
	lmsrp_mess *mess = lmsrp_mess_create_request(pool, &sessid, &msend);
	lmsrp_uri *to_uri = sends->info->callee;
	mess->to_path = lmsrp_list_uri_create(pool, to_uri);
	lmsrp_list_uri_add(mess->to_path, to_uri);
	mess->from_path = lmsrp_list_uri_create(pool, sends->info->caller);

	if (total == 0) {
		PJ_LOG(3, (__LNAME__,"data is failse"));
		module->close(module->data);
		return PJ_SUCCESS;
	}
	lmsrp_byte_range *byte = lmsrp_byte_range_create(pool, 1, 0, total);
	mess->byte_range = byte;
	mess->flag = '+';
	mess->contend.ptr = contend;

	// create handle thread
	pj_thread_create(module->pool, "send handle", lmsrp_report_handle, sends,
	PJ_THREAD_DEFAULT_STACK_SIZE, 0, &sends->thread_send_handle);
	lmsrp_report_list *res;
	pj_uint32_t dem;
	int stime;
	if (LMSRP_LOW_MEMORY) {
		stime = 10;
	} else {
		stime = 2;
	}
#if LMSRP_DEBUG == 1
	stime = 10;
#endif
	while (sends->flag != lmsrp_flag_close && sends->flag != lmsrp_flag_suspend) {
		pj_thread_sleep(stime);
		res = am_sip_send_get_report(sends->list);
		if (res != NULL) {
// TODO : fix control resend mess bug
			pj_create_random_string(rand, messid_lg);
			dem = module->read(module->data, readbuff, byte_read,
					res->rang.start, &res->rang.end);
			// set byte range
			mess->byte_range->start = res->rang.start;
			mess->byte_range->end = res->rang.end;

			mess->contend.slen = sends->block_size;
			dem = codec->encode(readbuff, dem, mess->contend.ptr,
					(pj_uint32_t*) &mess->contend.slen);

			dem = lmsrp_mess_tostring(mess, mess_send, leng);
			module->send(module->data, mess_send, dem);
			res->active = PJ_FALSE;
		} else {
			if (end < total) {
				// create random id for one mess
				pj_create_random_string(rand, messid_lg);
				end = start + byte_read - 1;
				dem = module->read(module->data, readbuff, byte_read, start,
						&end);
				//set byte range
				if (end > total) {
					mess->flag = '$';
					end = total;
					sends->flag = lmsrp_flag_end;
				}
				mess->byte_range->start = start;
				mess->byte_range->end = end;

				mess->contend.slen = sends->block_size;
				//printf("status %d encode size :%ld\n", dem, mess->contend.slen);
				dem = codec->encode(readbuff, dem, mess->contend.ptr,
						(pj_uint32_t*) &mess->contend.slen);
				PJ_LOG(3,
						(__LNAME__,"status %d encode size :%d with end %ld", dem, mess->contend.slen , mess->byte_range->end));
#if LMSRP_DEBUG == 1
				fprintf(tmpsend, "%d\n", count);
				count++;
				fwrite(mess->contend.ptr, sizeof(char),
						(int) mess->contend.slen, tmpsend);
				fprintf(tmpsend, "\n");
				//printf("send:\n%.*s\n", (int) mess->contend.slen,
				//mess->contend.ptr) ;
# endif
				dem = lmsrp_mess_tostring(mess, mess_send, leng);
				//printf("mess leng :%d\n", dem);
				module->send(module->data, mess_send, dem);
				start = end + 1;
			}
		}

	}
#if LMSRP_DEBUG == 1
	fclose(tmpsend);
# endif
	if (sends->flag == lmsrp_flag_suspend) {
		PJ_LOG(2, (__LNAME__,"suppend send sesson"));
		module->suppend(module->data);
	} else
		module->close(module->data);
	return PJ_SUCCESS;
}
#endif
static int lmsrp_report_handle(void *arg) {
	lmsrp_send_stream *send = arg;
	pj_status_t st;
	char buff[1000];
	pj_ssize_t leng;
	lmsrp_send_module *module = send->module;
	pj_pool_t *pool = send->pool;
	lmsrp_report_list *res = am_sip_send_get_free_lot(pool, send->list);
	lmsrp_mess *mess;
	pj_time_val past, smp;

//	int
	int period = 10;
	while (send->flag != lmsrp_flag_close && send->flag != lmsrp_flag_suspend) {
		leng = 1000;
		st = module->get_report(module->data, buff, &leng);
		if (send->flag == lmsrp_flag_end) {
			pj_gettimeofday(&past);
			send->flag = lmsrp_flag_wait;
		}
		if (send->flag == lmsrp_flag_wait) {
			pj_gettimeofday(&smp);
			if (smp.sec - past.sec > period) {
				send->flag = lmsrp_flag_suspend;
			}
		}
		if (st == 1)
			continue;
		if (leng == 0) {
			if (st == 0) {
				PJ_LOG(3, (__LNAME__, "socket is broken"));
				send->flag = lmsrp_flag_suspend;
				break;
//				continue;
			} else
				continue;
		}
		mess = lmsrp_mess_prase_from_buff(pool, buff, leng);
		if (mess != NULL) {
			if (mess->status != NULL) {
				if (mess->status->rfc_code == lmsrp_REPORT_RESEND) {
					if (mess->byte_range != NULL) {
						int start = mess->byte_range->start;
						int end = mess->byte_range->end;
						while (start + send->codec_size < end) {
							mess->byte_range->end = start + send->codec_size
									- 1;
							pj_memcpy(&res->rang, mess->byte_range,
									sizeof(lmsrp_byte_range));
							res->active = PJ_TRUE;
							res = am_sip_send_get_free_lot(pool, send->list);
							start = start + send->codec_size;
							mess->byte_range->start = start;
							mess->byte_range->end = end;
						}
						pj_gettimeofday(&past); // khoi tao lai bo dem
						pj_memcpy(&res->rang, mess->byte_range,
								sizeof(lmsrp_byte_range));
						res->active = PJ_TRUE;
						res = am_sip_send_get_free_lot(pool, send->list);
					}
				} else {
					if (mess->status->rfc_code == lmsrp_REPORT_OK) {
						send->flag = lmsrp_flag_close;
					}
				}
			}
		}

	}

	return 0;

}
void lmsrp_send_stream_start(pj_pool_t *pool, lmsrp_session_info *info,
		lmsrp_send_stream *send) {
	send->info = info;
	send->pool = pool;
	send->module->pool = pool;
	send->list = init_report_list(send->pool, 10);
	send->flag = lmsrp_flag_continue;
	pj_thread_create(pool, "send stream", lmsrp_send_session, send,
	PJ_THREAD_DEFAULT_STACK_SIZE, 0, &send->thread_send_sess);
}
void lmsrp_send_stream_close(lmsrp_send_stream *send) {
	send->flag = lmsrp_flag_suspend;
	if (send->thread_send_handle) {
		pj_thread_join(send->thread_send_handle);
		pj_thread_destroy(send->thread_send_handle);
		send->thread_send_handle = NULL;
	}
	if (send->thread_send_sess) {
		pj_thread_join(send->thread_send_sess);
		pj_thread_destroy(send->thread_send_sess);
		send->thread_send_handle = NULL;
	}
	send->info = NULL;
	send->list = NULL;
	send->pool = NULL;
}
void lmsrp_send_stream_init(lmsrp_send_stream *send, lmsrp_send_module *module) {
	pj_bzero(send, sizeof(send));
	send->module = module;
}

