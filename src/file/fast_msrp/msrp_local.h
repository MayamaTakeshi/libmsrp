/*
 * msrp_local.h
 *
 *  Created on: Aug 31, 2021
 *      Author: amneiht
 */

#ifndef MSRP_FAST_MSRP_MSRP_LOCAL_H_
#define MSRP_FAST_MSRP_MSRP_LOCAL_H_

#include <lmsrp_file.h>
void lmsrp_init_recv_block(pj_pool_t *pool,lmsrp_recv_data_block **block,
		int size, int block_size);
lmsrp_recv_data_block* am_msrp_get_recv_data_block(pj_pool_t *pool,
		lmsrp_recv_data_block *block, int count);
lmsrp_recv_data_block* am_msrp_get_recv_free_block(pj_pool_t *pool,
		lmsrp_recv_data_block *block, int block_size);
pj_status_t am_msrp_init_data_block(pj_pool_t *pool, lmsrp_block_list **block,
		pj_int64_t size, int block_size);
static int getblock_by_state(void *value, const pj_list_type *node) {
	int d = *(int*) value;
	const lmsrp_block_list *bl = (const lmsrp_block_list*) node;
	if (d == bl->state)
		return 0;
	return 1;
}
lmsrp_block_list* am_msrp_get_free_block(pj_pool_t *pool, lmsrp_block_list *block,
		int block_size);

int lmsrp_getblock_data(void *value, const pj_list_type *node);
#endif /* MSRP_FAST_MSRP_MSRP_LOCAL_H_ */
