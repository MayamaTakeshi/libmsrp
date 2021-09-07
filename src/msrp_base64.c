/*
 * msrp_base64.c
 *
 *  Created on: Sep 6, 2021
 *      Author: amneiht
 */

#include <pjlib-util.h>
#include <pjlib.h>
void test_base64() {
	char *lsp = "pika pika chhhuu\n\r\nsssssssss ";
	int st = 1000;
	char lol[1000];
	pj_bzero(lol, st);
	printf("in pur lang is : %ld\n", strlen(lsp));
	pj_base64_encode((pj_uint8_t*) lsp, strlen(lsp), lol, &st);
	// cGlrYSBwaWthIGNoaGh1dQoNCnNzc3Nzc3Nz
	pj_str_t in =pj_str( "cGlrYSBwaWthIGNoaGh1dQoNCnNzc3Nzc3N");
	st = 1000;
	pj_bzero(lol, st);
//	printf("in pur lang is : %ld\n", strlen(lsp));
	pj_base64_decode(&in, lol, &st);
	puts(lol);

}
