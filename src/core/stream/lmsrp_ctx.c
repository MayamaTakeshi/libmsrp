/*
 * lmsrp_ctx.c
 *
 *  Created on: Mar 13, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>
#include <lmsrp/lmsrp_stream.h>
# define fconvert(pid) \
if(pid.slen>0) { \
	convert_str(&pid, str_old, str); \
}
//extern int lmsrp_mess_check(char d, void *arg) ;
static void convert_str(pj_str_t *sl, char *str_old, char *str) {
	int dem = sl->ptr - str_old;
	sl->ptr = str + dem;
}
static void lmsrp_list_uri_change(lmsrp_list_uri *list, char *str_old,
		char *str) {
	lmsrp_list_uri *start = list;
	lmsrp_uri *uri;
	do {
		uri = start->uri;
		fconvert(uri->authority.host);
		fconvert(uri->authority.userinfo);
		fconvert(uri->scheme);
		fconvert(uri->session_id);
		start = start->next;
	} while (list != start);
}
//static
static void change_point(lmsrp_mess *mess, char *str_old, char *str , lmsrp_prase_state prase_state) {

	if (mess->tid.slen > 0) {
		convert_str(&mess->tid, str_old, str);
	}
	if (prase_state == lmsrp_prase_state_content) {
			convert_str(&mess->contend, str_old, str);
		}
	if (mess->auth != NULL) {
		fconvert(mess->auth->scheme);
		fconvert(mess->auth->username);
		fconvert(mess->auth->realm);
		fconvert(mess->auth->nonce);
		fconvert(mess->auth->uri);
		fconvert(mess->auth->response);
		fconvert(mess->auth->algorithm);
		fconvert(mess->auth->cnonce);
		fconvert(mess->auth->opaque);
		fconvert(mess->auth->qop);
		fconvert(mess->auth->nc);
	}
	if (mess->www != NULL) {
		fconvert(mess->www->scheme);
		fconvert(mess->www->realm);
		fconvert(mess->www->domain);
		fconvert(mess->www->nonce);
		fconvert(mess->www->opaque);
		fconvert(mess->www->algorithm);
		fconvert(mess->www->qop);
	}
	// TODO : tam thoi bo qua to , from , user path
	if (mess->to_path) {
		lmsrp_list_uri_change(mess->to_path, str_old, str);
	}
	if (mess->from_path) {
		lmsrp_list_uri_change(mess->from_path, str_old, str);
	}
	if (mess->use_path) {
		lmsrp_list_uri_change(mess->use_path, str_old, str);
	}

	if (mess->content_type) {
		lmsrp_list_param *type = mess->content_type->type;
		do {
			fconvert(type->var.name);
			fconvert(type->var.value);
			type = mess->content_type->type->next;
		} while (type != mess->content_type->type);
	}
}
static int _lmsrp_context_update2(lmsrp_context *ctx, char *buff,
		pj_int32_t size, pj_bool_t *report) {

	switch (ctx->state) {
	case lmsrp_prase_state_done:
		;
		pj_pool_reset(ctx->pool);
		ctx->mess = pj_pool_zalloc(ctx->pool, sizeof(lmsrp_mess));
		ctx->mess->pool = ctx->pool;
		ctx->mess->contend.slen=0 ;
		ctx->data_read = 0;
		ctx->sum = 0;
		*report = lmsrp_stream_prase(ctx, buff, size);
		if (*report == PJ_FALSE) {
			ctx->state = lmsrp_prase_state_done;
			*report = PJ_FALSE;
			return ctx->data_read;
		}
		if (ctx->state == lmsrp_prase_state_done) {
			ctx->export(ctx->data, ctx->mess->contend.ptr,
					ctx->mess->contend.slen, ctx->mess);
			*report = PJ_TRUE;
			return ctx->data_read;
		} else {
			// perpare for new update
			if (size > ctx->max_byte) {
				PJ_LOG(3, ("msrp ctx", "over data to prase"));
				*report = PJ_FALSE;
				return ctx->data_read;
			}
			pj_memcpy(ctx->tmp, buff, size);
			ctx->sum = size;
			change_point(ctx->mess, buff, ctx->tmp , ctx->state );
			return size;
		}
		break;
	default: {
		//all header leng + maxbyte
		int cpy = ctx->max_byte - ctx->sum;
		int nlen = 0; //no copy len
		if (cpy > size) {
			cpy = size;
		} else {
			if (cpy <= 0) {
				PJ_LOG(3, ("msrp ctx", "over data to prase"));
				ctx->state = lmsrp_prase_state_done;
				*report = PJ_FALSE;
				return ctx->data_read;
			}
			nlen = size - cpy;
		}
		pj_memcpy((ctx->tmp + ctx->sum), buff, cpy);
		// so data can doc
		ctx->sum = ctx->sum + cpy;
		pj_size_t size2 = ctx->sum - ctx->data_read;
		*report = lmsrp_stream_prase(ctx, (ctx->tmp + ctx->data_read), size2);
		if (*report == PJ_FALSE) {
			ctx->state = lmsrp_prase_state_done;
			return ctx->data_read;
		}
		if (ctx->state == lmsrp_prase_state_done) {
			ctx->export(ctx->data, ctx->mess->contend.ptr,
					ctx->mess->contend.slen, ctx->mess);
			/*
			 * after update ctx , ctx->data_read=total byte are readed
			 */
			return size - (ctx->sum - ctx->data_read) -nlen;
		} else {
			// khang dinh sum =size
//			old_leng = ctx->data_read - old_leng;
//			pj_memcpy(ctx->tmp + ctx->sum, buff, size2);

			return cpy;
		}
	}
		break;
	}
	return size;
}
pj_bool_t lmsrp_context_update(lmsrp_context *ctx, char *buff, pj_int32_t size) {
	pj_int32_t count = 0;
	pj_bool_t rp;
	while (size > 0) {
		count = _lmsrp_context_update2(ctx, buff, size, &rp);
		size = size - count;
		buff = buff + count;
	}
	return PJ_TRUE;
}
//static  __attribute__((unused)) pj_bool_t test_lmsrp_context_update(lmsrp_context *ctx, char *buff,
//		pj_int32_t size) {
//
//	pj_bool_t report;
//	switch (ctx->state) {
//	case lmsrp_prase_state_done:
//		;
//		pj_pool_reset(ctx->pool);
//		ctx->mess = pj_pool_zalloc(ctx->pool, sizeof(lmsrp_mess));
//		ctx->mess->pool = ctx->pool;
//		ctx->mess->contend.slen=0 ;
//		ctx->data_read = 0;
//		ctx->sum = 0;
//		report = lmsrp_stream_prase(ctx, buff, size);
//		if (report == PJ_FALSE) {
//			ctx->state = lmsrp_prase_state_done;
//			return report;
//		}
//		if (ctx->state == lmsrp_prase_state_done) {
//			ctx->export(ctx->data, ctx->mess->contend.ptr,
//					ctx->mess->contend.slen, ctx->mess);
//			if (ctx->data_read < size) {
//				buff = buff + ctx->data_read;
//				size = size - ctx->data_read;
//				return lmsrp_context_update(ctx, buff, size);
//			}
//		} else {
//			// perpare for new update
//			if (size > ctx->max_byte) {
//				PJ_LOG(3, ("msrp ctx", "over data to prase"));
//				ctx->state = lmsrp_prase_state_done;
//				return -1;
//			}
//			pj_memcpy(ctx->tmp, buff, size);
//			ctx->sum = size;
//			change_point(ctx->mess, buff, ctx->tmp);
//		}
//		break;
//	default: {
//		//all header leng + maxbyte
//		int cpy = ctx->max_byte - ctx->sum;
//		if (cpy > size) {
//			cpy = size;
//		} else {
//			if (cpy < size) {
//				PJ_LOG(3, ("msrp ctx", "over data to prase"));
//				ctx->state = lmsrp_prase_state_done;
//				return -1;
//			}
//		}
//		pj_memcpy((ctx->tmp + ctx->sum), buff, cpy);
//		// so data can doc
//		int old_leng = ctx->sum;
//		pj_size_t size2 = cpy + ctx->sum - ctx->data_read;
//		report = lmsrp_stream_prase(ctx, (ctx->tmp + ctx->data_read), size2);
//		if (report == PJ_FALSE) {
//			ctx->state = lmsrp_prase_state_done;
//			return report;
//		}
//		if (ctx->state == lmsrp_prase_state_done) {
//			ctx->export(ctx->data, ctx->mess->contend.ptr,
//					ctx->mess->contend.slen, ctx->mess);
//			/*
//			 * after update ctx , ctx->data_read=total byte are readed
//			 */
//			size2 = ctx->data_read - old_leng;
//			if (size2 < size) {
//				size = size - size2;
//				buff = buff + size2;
//				return lmsrp_context_update(ctx, buff, size);
//			}
//		} else {
//			// khang dinh sum =size
//			pj_memcpy(ctx->tmp + ctx->sum, buff, size2);
//			ctx->sum = ctx->sum + cpy;
//		}
//	}
//		break;
//	}
//	return PJ_TRUE;
//}
void lmsrp_context_init(lmsrp_context *ctx, pj_caching_pool *cp, int max_size,
		void *data,
		void (*export)(void *data, char *buff, int leng, lmsrp_mess *arg)) {
	ctx->pool = pj_pool_create(&cp->factory, "tmp", 4000, 4000, NULL);
	ctx->spool = pj_pool_create(&cp->factory, "tmp", 4000, 4000, NULL);
	ctx->max_byte = (max_size + 500) * 2;
	ctx->tmp = pj_pool_alloc(ctx->spool, ctx->max_byte * sizeof(char));
	ctx->state = lmsrp_prase_state_done;
	ctx->data = data;
	ctx->export = export;
}
void lmsrp_context_clear(lmsrp_context *ctx) {
	pj_pool_release(ctx->pool);
	pj_pool_release(ctx->spool);
}

