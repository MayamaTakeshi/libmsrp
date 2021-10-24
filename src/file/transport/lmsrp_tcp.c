#include <lmsrp_file.h>

#define THIS "msrp_tcp.c"
// old code without activesock framework
#if !defined ( AM_USE_TLS) || AM_USE_TLS !=1
typedef struct {
//	pj_pool_t *pool;
	pj_sock_t sock;
	pj_time_val timeout;
	pj_fd_set_t rset;
} msrp_tcp_sock;
static void* msrp_tcp_init(pj_pool_t *pool, pj_sockaddr_in *local) {
	msrp_tcp_sock *fsock = pj_pool_zalloc(pool, sizeof(msrp_tcp_sock));
//	pj_sock_t *sock = pj_pool_alloc(pool, sizeof(pj_sock_t));
	pj_status_t st;
	st = pj_sock_socket(pj_AF_INET(), PJ_SOCK_STREAM, 0, &fsock->sock);
	if (st != PJ_SUCCESS)
		return NULL;
	if (local != NULL) {
		st = pj_sock_bind(fsock->sock, local, sizeof(pj_sockaddr_in));
		if (st != PJ_SUCCESS) {
			pj_sock_close(fsock->sock);
			return NULL;
		}
	}
	PJ_LOG(4, (THIS ,"init sock is:%ld",fsock->sock));
	fsock->timeout.sec = 0;
	return fsock;
}
static void* msrp_tcp_accept(void *socks, pj_pool_t *pool) {
	msrp_tcp_sock *fsock = (msrp_tcp_sock*) socks;

	int timeout = 2000;
	fsock->timeout.msec = timeout % 1000;
	fsock->timeout.sec = timeout / 1000;
	PJ_FD_ZERO(&fsock->rset);
	PJ_FD_SET(fsock->sock, &fsock->rset);
	pj_status_t st = pj_sock_select((int) fsock->sock + 1, &fsock->rset, NULL,
	NULL, &fsock->timeout);
	if (st != 1) {
		return NULL; // su kien time out
	}

	pj_sock_t rs;
	st = pj_sock_accept(fsock->sock, &rs, NULL, 0);
	if (st != 0)
		return NULL;
	msrp_tcp_sock *fs = pj_pool_zalloc(pool, sizeof(msrp_tcp_sock));
	fs->sock = rs;
	fsock->timeout.sec = 0;
	return fs;

}
static pj_status_t msrp_tcp_connect(void *socks, pj_sockaddr_in *addr) {
	pj_sock_t sock = ((msrp_tcp_sock*) socks)->sock;
	pj_status_t st;
	st = pj_sock_connect(sock, addr, sizeof(pj_sockaddr_in));
	return st;
}
static pj_status_t msrp_tcp_send(void *socks, void *data, pj_ssize_t length) {
	pj_sock_t sock = ((msrp_tcp_sock*) socks)->sock;
	long leng = length;
//	printf("length 1 %ld\n", length);
	pj_status_t st = pj_sock_send(sock, data, &length, 0);
//	printf("length 2 %ld\n", length);
	if (st != 0 || length == 0) {
		puts("send error");
		int dem = 1000;
		char lol[dem];
		pj_strerror(st, lol, dem);
		PJ_LOG(1, ("get report","st :%d ,leng %ld err: %s",st,leng,lol));
	}
	return st;
}
static pj_status_t msrp_tcp_recv(void *socks, void *data, pj_ssize_t *length) {
	pj_sock_t sock = ((msrp_tcp_sock*) socks)->sock;
	return pj_sock_recv(sock, data, length, 0);
}

static pj_status_t msrp_tcp_recv_withtimeout(void *socks, void *data,
		pj_ssize_t *length, int timeout) {

	msrp_tcp_sock *fsock = (msrp_tcp_sock*) socks;
	pj_status_t st;
	if (timeout == 0)
		timeout = 2000;
	fsock->timeout.msec = timeout % 1000;
	fsock->timeout.sec = timeout / 1000;
	PJ_FD_ZERO(&fsock->rset);
	PJ_FD_SET(fsock->sock, &fsock->rset);
	st = pj_sock_select((int) fsock->sock + 1, &fsock->rset, NULL, NULL,
			&fsock->timeout);
	if (st != 1) {
		return 1; // su kien tiem out
	}
	return pj_sock_recv(fsock->sock, data, length, 0);

}
static void msrp_tcp_close(void *socks) {

	if (socks != NULL) {
		msrp_tcp_sock *fsock = (msrp_tcp_sock*) socks;
		PJ_LOG(4, (THIS ,"close sock is:%ld",fsock->sock));
		pj_sock_close(fsock->sock);
	}
}
void lmsrp_init_tcp_transport_module(lmsrp_transport_module *trans) {
	trans->connect = msrp_tcp_connect;
	trans->init = msrp_tcp_init;
	trans->recv = msrp_tcp_recv;
	trans->send = msrp_tcp_send;
	trans->close = msrp_tcp_close;
	trans->accept = msrp_tcp_accept;
	trans->recv_t = msrp_tcp_recv_withtimeout;
}
#endif
