#include <lmsrp_file.h>
#include "msrp_local.h"
#include <lmsrp.h>
#define this "msrp_frecv.c"
//int getblock_by_state(void *value, const pj_list_type *node);
static lmsrp_block_list* newblock(pj_pool_t *pool, int block_size);

static lmsrp_recv_data_block* new_recv_block(pj_pool_t *pool, int block_size) {
	lmsrp_recv_data_block *res = PJ_POOL_ALLOC_T(pool, lmsrp_recv_data_block);
	res->package_count = 0;
	res->data = pj_pool_alloc(pool, block_size * 3 * sizeof(char));
	res->state = lmsrp_block_state_free;
	return res;
}
void lmsrp_init_recv_block(pj_pool_t *pool, lmsrp_recv_data_block **block,
		int size, int block_size) {
	lmsrp_recv_data_block *bl = new_recv_block(pool, block_size);
	pj_list_init(bl);
	for (int i = 1; i < size; i++) {
		lmsrp_recv_data_block *v = new_recv_block(pool, block_size);
		pj_list_insert_after(bl, v);
	}
	*block = bl;
}
static int get_recv_by_count(void *value, const pj_list_type *node) {
	int d = *(int*) value;
	const lmsrp_recv_data_block *arg = node;
	if (d == arg->package_count && arg->state == lmsrp_block_state_recv)
		return 0;

	return 1;
}
static int get_recv_by_state(void *value, const pj_list_type *node) {
	int d = *(int*) value;
	const lmsrp_recv_data_block *bl = (const lmsrp_recv_data_block*) node;
	if (d == bl->state)
		return 0;
	return 1;
}
lmsrp_recv_data_block* lmsrp_get_recv_data_block(pj_pool_t *pool,
		lmsrp_recv_data_block *block, int count) {
	//TODO toi uu hoa sau
	pj_status_t st = get_recv_by_count(&count, block);
	if (st == 0) {
		return block;
	}
	lmsrp_recv_data_block *res = pj_list_search(block, &count,
			get_recv_by_count);
	return res;
}
lmsrp_recv_data_block* lmsrp_get_recv_free_block(pj_pool_t *pool,
		lmsrp_recv_data_block *block, int block_size) {
	int d = lmsrp_block_state_free;
	lmsrp_recv_data_block *res = pj_list_search(block, &d, &get_recv_by_state);
	if (res == NULL) {
		res = new_recv_block(pool, block_size);
//		PJ_LOG(2, (this,"new block recv"));
		pj_list_insert_after(block, res);
	}
	return res;
}

int lmsrp_getblock_data(void *value, const pj_list_type *node) {
	int d = *(int*) value;
	const lmsrp_block_list *blist = node;
	if (blist->state == lmsrp_block_state_raw) {
		return 0;
	} else if (blist->state == lmsrp_block_state_ready) {
		const lmsrp_block_list *dtl = blist;
		if (dtl->range.start <= d) {
			return 0;
		}
	}
	return 1;
}
lmsrp_block_list* lmsrp_get_free_block(pj_pool_t *pool, lmsrp_block_list *block,
		int block_size) {
	int d = lmsrp_block_state_free;
	lmsrp_block_list *res = pj_list_search(block, &d, &getblock_by_state);
	if (res == NULL) {
		res = newblock(pool, block_size);
		res->state = lmsrp_block_state_recv;
		pj_list_insert_after(block, res);
	} else {
		res->state = lmsrp_block_state_recv;
	}
	return res;
}

static lmsrp_block_list* newblock(pj_pool_t *pool, int block_size) {
	lmsrp_block_list *app = pj_pool_alloc(pool, sizeof(lmsrp_block_list));

	app->data = pj_pool_alloc(pool, 3 * block_size * sizeof(char));
	if (app->data == NULL)
		return NULL;
	app->state = lmsrp_block_state_free;
	return app;
}

pj_status_t lmsrp_init_data_block(pj_pool_t *pool, lmsrp_block_list **block,
		pj_int64_t size, int block_size) {
	lmsrp_block_list *app = newblock(pool, block_size);
	*block = app;
	pj_list_init(app);
	lmsrp_block_list *node;
	for (int i = 1; i < size; i++) {
		node = newblock(pool, block_size);
		pj_list_insert_after(app, node);
	}
	return PJ_SUCCESS;
}
