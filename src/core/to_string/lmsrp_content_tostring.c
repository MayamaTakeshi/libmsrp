/*
 * lmsrp_content_tostring.c
 *
 *  Created on: Sep 5, 2021
 *      Author: amneiht
 */

#include <lmsrp.h>

int lmsrp_content_type_to_string(lmsrp_content_type_h *con, char *data,
		int size) {
	if (!con || !con->type)
		return -1;
	int dem = 0;
	int vt;
	lmsrp_list_param *ls = con->type;
	do {
		vt = sprintf(data, " %.*s/%.*s", (int) ls->var.name.slen,
				ls->var.name.ptr, (int) ls->var.value.slen, ls->var.value.ptr);
		dem = dem + vt;
		if (dem > size)
			return -1;
		data = data + vt;
		ls = ls->next;
	} while (ls != con->type);
	return dem;
}
