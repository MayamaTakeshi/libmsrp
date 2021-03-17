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
static void change_point(lmsrp_mess *mess, char *str_old, char *str) {

	if (mess->tid.slen > 0) {
		convert_str(&mess->tid, str_old, str);
	}
	if (mess->contend.slen > 0) {
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

}
pj_bool_t lmsrp_context_update(lmsrp_context *ctx, char *buff, pj_int32_t size) {

	pj_bool_t report;
	switch (ctx->state) {
	case lmsrp_prase_state_done:
		;
		ctx->mess = pj_pool_zalloc(ctx->pool, sizeof(lmsrp_mess));
		ctx->mess->pool = ctx->pool;
		ctx->content_leng = 0;
		ctx->data_read = 0;
		ctx->sum = 0;
		report = lmsrp_stream_prase(ctx, buff, size);
		if (report == PJ_FALSE) {
			ctx->state = lmsrp_prase_state_done ;
			return report;
		}
		if (ctx->state == lmsrp_prase_state_done) {
			ctx->export(ctx->data, ctx->mess->contend.ptr,
					ctx->mess->contend.slen, ctx->mess);
			if (ctx->data_read < size) {
				buff = buff + ctx->data_read;
				size = size - ctx->data_read;
				return lmsrp_context_update(ctx, buff, size);
			}
		} else {
			// perpare for new update
			pj_memcpy(ctx->tmp, buff, size);
			ctx->sum = size;
			change_point(ctx->mess, buff, ctx->tmp);
		}
		break;
	default: {
		//all header leng + maxbyte
		int cpy = (ctx->max_byte +500)+ 20 - ctx->sum;
		if (cpy > size)
			cpy = size;
		pj_memcpy((ctx->tmp + ctx->sum), buff, cpy);
		// so data can doc
		int old_leng = ctx->sum;
		pj_size_t size2 = cpy + ctx->sum - ctx->data_read;
		report = lmsrp_stream_prase(ctx, (ctx->tmp + ctx->data_read), size2);
		if (report == PJ_FALSE) {
			ctx->state = lmsrp_prase_state_done ;
			return report;
		}
		if (ctx->state == lmsrp_prase_state_done) {
			ctx->export(ctx->data, ctx->mess->contend.ptr,
					ctx->mess->contend.slen, ctx->mess);
			size2 = ctx->data_read - old_leng;
			if (size2 < size) {
				size = size - size2;
				buff = buff + size2;
				return lmsrp_context_update(ctx, buff, size);
			}
		} else {
			// khang dinh sum =size
			pj_memcpy(ctx->tmp + ctx->sum, buff, size2);
			ctx->sum = ctx->sum + cpy;
		}
	}
		break;
	}
	return PJ_TRUE;
}
void lmsrp_context_init(lmsrp_context *ctx, pj_pool_t *pool, int max_size,
		void *data, void *export) {
	ctx->pool = pool;
	ctx->max_byte = max_size;
	ctx->tmp = pj_pool_alloc(pool, (max_size + 500) * 2 * sizeof(char));
	ctx->state = lmsrp_prase_state_done;
	ctx->data = data;
	ctx->export = export;
}
