/*
 * lmsrp_base64.c
 *
 *  Created on: Oct 13, 2021
 *      Author: amneiht
 */
#include <lmsrp.h>

#if defined (LMSRP_ONLY_PJLIB) && LMSRP_ONLY_PJLIB == 1
#define INV	    -1
#define PADDING	    '='

/**
 * Helper macro to calculate the approximate length required for base256 to
 * base64 conversion.
 */
#define LMSRP_BASE256_TO_BASE64_LEN(len)	(len * 4 / 3 + 3)

/**
 * Helper macro to calculate the approximage length required for base64 to
 * base256 conversion.
 */
#define LMSRP_BASE64_TO_BASE256_LEN(len)	(len * 3 / 4)

static const char base64_char[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
		'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
		'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
		'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
		'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

static int base256_char(char c) {
	if (c >= 'A' && c <= 'Z')
		return (c - 'A');
	else if (c >= 'a' && c <= 'z')
		return (c - 'a' + 26);
	else if (c >= '0' && c <= '9')
		return (c - '0' + 52);
	else if (c == '+')
		return (62);
	else if (c == '/')
		return (63);
	else {
		/* It *may* happen on bad input, so this is not a good idea.
		 * pj_assert(!"Should not happen as '=' should have been filtered");
		 */
		return INV;
	}
}
static void base256to64(pj_uint8_t c1, pj_uint8_t c2, pj_uint8_t c3,
		int padding, char *output) {
	*output++ = base64_char[c1 >> 2];
	*output++ = base64_char[((c1 & 0x3) << 4) | ((c2 & 0xF0) >> 4)];
	switch (padding) {
	case 0:
		*output++ = base64_char[((c2 & 0xF) << 2) | ((c3 & 0xC0) >> 6)];
		*output = base64_char[c3 & 0x3F];
		break;
	case 1:
		*output++ = base64_char[((c2 & 0xF) << 2) | ((c3 & 0xC0) >> 6)];
		*output = PADDING;
		break;
	case 2:
	default:
		*output++ = PADDING;
		*output = PADDING;
		break;
	}
}
pj_status_t lmsrp_base64_encode(const pj_uint8_t *input, int in_len,
		char *output, int *out_len) {
	const pj_uint8_t *pi = input;
	pj_uint8_t c1, c2, c3;
	int i = 0;
	char *po = output;

	PJ_ASSERT_RETURN(input && output && out_len, PJ_EINVAL);
	PJ_ASSERT_RETURN(*out_len >= LMSRP_BASE256_TO_BASE64_LEN(in_len),
			PJ_ETOOSMALL);

	while (i < in_len) {
		c1 = *pi++;
		++i;

		if (i == in_len) {
			base256to64(c1, 0, 0, 2, po);
			po += 4;
			break;
		} else {
			c2 = *pi++;
			++i;

			if (i == in_len) {
				base256to64(c1, c2, 0, 1, po);
				po += 4;
				break;
			} else {
				c3 = *pi++;
				++i;
				base256to64(c1, c2, c3, 0, po);
			}
		}

		po += 4;
	}

	*out_len = (int) (po - output);
	return PJ_SUCCESS;
}

pj_status_t lmsrp_base64_decode(const pj_str_t *input, pj_uint8_t *out,
		int *out_len) {
	const char *buf;
	int len;
	int i, j, k;
	int c[4];

	PJ_ASSERT_RETURN(input && out && out_len, PJ_EINVAL);

	buf = input->ptr;
	len = (int) input->slen;
	while (len && buf[len - 1] == '=')
		--len;

	PJ_ASSERT_RETURN(*out_len >= LMSRP_BASE64_TO_BASE256_LEN(len),
			PJ_ETOOSMALL);

	for (i = 0, j = 0; i < len;) {
		/* Fill up c, silently ignoring invalid characters */
		for (k = 0; k < 4 && i < len; ++k) {
			do {
				c[k] = base256_char(buf[i++]);
			} while (c[k] == INV && i < len);
		}

		if (k < 4) {
			if (k > 1) {
				out[j++] = (pj_uint8_t) ((c[0] << 2) | ((c[1] & 0x30) >> 4));
				if (k > 2) {
					out[j++] = (pj_uint8_t) (((c[1] & 0x0F) << 4)
							| ((c[2] & 0x3C) >> 2));
				}
			}
			break;
		}

		out[j++] = (pj_uint8_t) ((c[0] << 2) | ((c[1] & 0x30) >> 4));
		out[j++] = (pj_uint8_t) (((c[1] & 0x0F) << 4) | ((c[2] & 0x3C) >> 2));
		out[j++] = (pj_uint8_t) (((c[2] & 0x03) << 6) | (c[3] & 0x3F));
	}

	pj_assert(j <= *out_len);
	*out_len = j;

	return PJ_SUCCESS;
}
#endif
