/*
 * msrp_udp.c
 *
 *  Created on: Mar 1, 2021
 *      Author: amneiht
 */
#include <lmsrp_file.h>
typedef struct {
	pj_sock_t sock;
	pj_sockaddr_in server;
} udp_sockinfo;
static void* msrp_udp_init(pj_pool_t *pool, pj_sockaddr_in *local) {
	udp_sockinfo *inf = PJ_POOL_ALLOC_T(pool, udp_sockinfo);
//	pj_sock_t *sock = pj_pool_alloc(pool, sizeof(pj_sock_t));
	pj_status_t st;
	st = pj_sock_socket(pj_AF_INET(), PJ_SOCK_DGRAM, 0, &inf->sock);
	if (st != PJ_SUCCESS)
		return NULL;
	if (local != NULL) {
		st = pj_sock_bind(inf->sock, local, sizeof(pj_sockaddr_in));
		if (st != PJ_SUCCESS) {
			pj_sock_close(inf->sock);
			return NULL;
		}
	}

	return inf;
}
static pj_status_t msrp_udp_connect(void *socks, pj_sockaddr_in *addr) {
	udp_sockinfo *inf = socks;
	pj_memcpy(&inf->server, addr, sizeof(pj_sockaddr_in));
	return PJ_SUCCESS;
}
static pj_status_t msrp_udp_send(void *socks, void *data, pj_ssize_t length) {
	udp_sockinfo *sock = socks;
//	length = length -2 ;
	pj_status_t st = pj_sock_sendto(sock->sock, data, &length, 0, &sock->server,
			sizeof(pj_sockaddr_in));
//	printf("length %ld\n", length);
	return st;
}
static pj_status_t msrp_udp_recv(void *socks, void *data, pj_ssize_t *length) {
	udp_sockinfo *sock = socks;
	return pj_sock_recvfrom(sock->sock, data, length, 0, NULL, NULL);
}
static void msrp_udp_close(void *socks) {
	udp_sockinfo *sock = socks;
	pj_sock_close(sock->sock);
}
void lmsrp_init_udp_transport_module(lmsrp_transport_module *trans) {
	trans->connect = &msrp_udp_connect;
	trans->init = &msrp_udp_init;
	trans->recv = &msrp_udp_recv;
	trans->send = &msrp_udp_send;
	trans->close = &msrp_udp_close;
}
