/**
 * convet data from 7 byte to 8 byte aka base 128
 * this is convert to non accii data
 */

#include <lmsrp_file.h>
/*
 *  (x * 7) /8
 */
static pj_uint32_t r128_decode_size(pj_uint32_t size) {
	return (size * 7) / 8;
}
/*
 *  (X * 8 + 6) /7
 */
static pj_uint32_t r128_encode_size(pj_uint32_t size) {
	return (size * 8 + 6) / 7;
}

static pj_status_t r128_decode(void *input, pj_uint32_t input_length, void *output,
		pj_uint32_t *output_length) {
	pj_uint32_t cp = (input_length * 7) / 8;
	if (cp > *output_length)
		return -1;
	unsigned char *p = input;
	unsigned char *s = output;
	pj_uint32_t step = input_length / 8;
	pj_uint32_t j = 0, k = 0;
	pj_int64_t byte;
	pj_uint8_t res;
	for (int i = 0; i < step; i++) {
		byte = 0;
		for (int m = 0; m < 8; m++) {
			byte = byte << 7 | (p[j + m] & 0x7f);
		}
		for (int l = 6; l > -1; l--) {
			res = (unsigned char) (byte & 0xff);
			byte = byte >> 8;
			s[k + l] = res;
		}
		j = j + 8;
		k = k + 7;
	}
	step = input_length - j;
	if (step > 0) {
		byte = 0;
		for (int m = 0; m < step; m++) {
			byte = byte << 7 | (p[j + m] & 0x7f);
		}
		// xoa bit thua
		byte = byte >> (8 - step);
		for (int l = step - 2; l > -1; l--) {
			res = (unsigned char) (byte & 0xff);
			byte = byte >> 8;
			s[k + l] = res;
		}
	}
	*output_length = cp;
	return 0;
}
static pj_status_t r128_encode(void *input,pj_uint32_t input_length, void *output,
		pj_uint32_t *output_length) {
	pj_uint32_t cp = (input_length * 8 + 6) / 7;

	if (cp > *output_length)
		return -1;
	pj_int32_t step = input_length / 7;
	unsigned char *p = input;
	unsigned char *s = output;
	int j = 0, k = 0;
	pj_int64_t byte;
	pj_uint8_t res;
	for (int i = 0; i < step; i++) {
		byte = 0;
		for (int l = 0; l < 7; l++) {
			byte = byte << 8 | (p[l + k]);
		}
		for (int m = 7; m >= 0; m--) {
			res = (unsigned char) (byte & 0x7f);
			byte = byte >> 7;
			s[j + m] = res | 0x80;
		}
		j = j + 8;
		k = k + 7;
	}
	step = input_length - k;
	if (step > 0) {
		byte = 0;
		for (int i = 0; i < step; i++) {
			byte = byte << 8 | (p[k + i]);
		}
		// them byte phu
		byte = byte << (7 - step);
		for (int m = step; m >= 0; m--) {
			res = (unsigned char) (byte & 0x7f);
			byte = byte >> 7;
			s[j + m] = res | 0x80;
		}
	}
	*output_length = cp;
	return 0;
}

lmsrp_codec* lmsrp_r128_create(pj_pool_t *pool) {
	lmsrp_codec *code = PJ_POOL_ALLOC_T(pool, lmsrp_codec);
	code->decode = r128_decode;
	code->encode = r128_encode;
	code->decode_leng = r128_decode_size;
	code->encode_leng = r128_encode_size;
	return code;
}
