/*
 * msrp_base64.c
 *
 *  Created on: Sep 6, 2021
 *      Author: amneiht
 */
#include <lmsrp_file.h>

static pj_uint32_t base64_decode_size(pj_uint32_t size) {
//	return (size / 4) * 3;
	size = size -3 ;
	return PJ_BASE64_TO_BASE256_LEN(size) ;
}
static pj_uint32_t base64_encode_size(pj_uint32_t size) {
//	return (size * 4 + 2) / 3;
	return PJ_BASE256_TO_BASE64_LEN(size);
}
#if !defined (LMSRP_ONLY_PJLIB) || LMSRP_ONLY_PJLIB != 1
static pj_status_t base64_encode(void *input, pj_uint32_t input_length,
		void *output, pj_uint32_t *output_length) {
//	printf("status %d encode size :%d\n", input_length, *output_length);
	return pj_base64_encode(input, input_length, output, (int *)output_length);
}

static pj_status_t base64_decode(void *input, pj_uint32_t input_length,
		void *output, pj_uint32_t *output_length) {
	pj_str_t inp;
	inp.slen = input_length;
	inp.ptr = (char*) input;
	return pj_base64_decode(&inp, output,(int *) output_length);
}
#else
static pj_status_t base64_encode(void *input, pj_uint32_t input_length,
		void *output, pj_uint32_t *output_length) {
	return lmsrp_base64_encode(input, input_length, output, (int*) output_length);
}

static pj_status_t base64_decode(void *input, pj_uint32_t input_length,
		void *output, pj_uint32_t *output_length) {
	pj_str_t inp;
	inp.slen = input_length;
	inp.ptr = (char*) input;
	return lmsrp_base64_decode(&inp, output, (int*) output_length);
}
#endif
lmsrp_codec* lmsrp_base64_create(pj_pool_t *pool) {
	lmsrp_codec *code = PJ_POOL_ALLOC_T(pool, lmsrp_codec);
	code->decode = base64_decode;
	code->encode = base64_encode;
	code->decode_leng = base64_decode_size;
	code->encode_leng = base64_encode_size;
	return code;
}
