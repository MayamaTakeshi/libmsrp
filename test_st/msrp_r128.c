/**
 * convet data from 7 byte to 8 byte aka base 128
 * this is convert to non accii data
 */

#include <pjlib.h>

static int r128_in_compute(int output_size) {
	int ret = (output_size * 7 + 7) / 8;
	if (output_size % 8 == 0)
		ret--;
	return ret ;
}
static int r128_out_compute(int input_size) {
	return (input_size * 8 + 6) / 7;
}

static pj_status_t r128_decode(void *input, int input_length, void *output,
		int *output_length) {
	int cp = (input_length * 7 + 7) / 8;
	if (cp > *output_length)
		return -1;
	unsigned char *p = input;
	unsigned char *s = output;
	int step = input_length / 8;
	int j = 0, k = 0;
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
		cp--;
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
static pj_status_t r128_encode(void *input, int input_length, void *output,
		int *output_length) {
	int cp = (input_length * 8 + 6) / 7;

	if (cp > *output_length)
		return -1;
	int step = input_length / 7;
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
	return 1;
}
void test_encode() {
	char buff[500];
	char buff2[500];
	char text[] = "ngua mat len thay doi voi goi, hoi tai sao lai pika";
	int out = 500;
	int pika = 500;
	int dem = r128_encode(text, strlen(text), buff, &out);
	dem = r128_decode(buff, out, buff2, &pika);
	puts(buff2);
	int z = r128_out_compute(874);
	printf("out compute is %d\n", z);
	z = r128_in_compute(1000);
	printf("int compute is %d\n", z);
}

