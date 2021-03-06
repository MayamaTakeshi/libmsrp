/*
 * lmsrp_line.h
 *
 *  Created on: Mar 4, 2021
 *      Author: amneiht
 */

#ifndef LMSRP_INFO_H_
#define LMSRP_INFO_H_
typedef enum lmsrp_mess_type {
	lmsrp_mess_type_request, //
	lmsrp_mess_type_respone
} lmsrp_mess_type;
typedef union _lmsrp_info {
	struct {
		pj_str_t method;
	} request;

	struct {
		pj_str_t reason;
		int code;
	} respone;
} lmsrp_info;

int lmsrp_info_prase(lmsrp_info *inf, char *data, int end);

#endif /* LMSRP_INFO_H_ */
