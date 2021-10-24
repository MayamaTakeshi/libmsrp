/*
 * lmsrp_util.c
 *
 *  Created on: Oct 13, 2021
 *      Author: amneiht
 */

#include <lmsrp/lmsrp_config.h>

#if defined (LMSRP_ONLY_PJLIB) && LMSRP_ONLY_PJLIB == 1
#include <pjlib.h>
#ifndef LMSRP_LMSRP_UTIL_H_
#define LMSRP_LMSRP_UTIL_H_
#define LMSRP_MD5STRLEN 32

// modyfy from pj/md5.h
/** MD5 context. */
typedef struct lmsrp_md5_context {
	pj_uint32_t buf[4]; /**< buf    */
	pj_uint32_t bits[2]; /**< bits   */
	pj_uint8_t in[64]; /**< in	    */
} lmsrp_md5_context;

/** Initialize the algorithm.
 *  @param pms		MD5 context.
 */
PJ_DECL(void) lmsrp_md5_init(lmsrp_md5_context *pms);

/** Append a string to the message.
 *  @param pms		MD5 context.
 *  @param data		Data.
 *  @param nbytes	Length of data.
 */
PJ_DECL(void) lmsrp_md5_update(lmsrp_md5_context *pms, const pj_uint8_t *data,
		unsigned nbytes);

/** Finish the message and return the digest.
 *  @param pms		MD5 context.
 *  @param digest	16 byte digest.
 */
PJ_DECL(void) lmsrp_md5_final(lmsrp_md5_context *pms, pj_uint8_t digest[16]);

pj_status_t lmsrp_base64_decode(const pj_str_t *input, pj_uint8_t *out,
		int *out_len) ;

pj_status_t lmsrp_base64_encode(const pj_uint8_t *input, int in_len,
		char *output, int *out_len) ;


#endif /* LMSRP_LMSRP_UTIL_C_ */

#endif
