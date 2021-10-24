#include<lmsrp.h>

int lmsrp_www_h_to_string(lmsrp_www_h *header, char *buff, int size) {
	int dem = 0;
	int tong = 0;
	char *point = buff;

	if (header->scheme.slen>0) {
		dem = sprintf(point, "%.*s ", (int) header->scheme.slen,
				header->scheme.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->realm.slen>0) {
		dem = sprintf(point, "realm=\"%.*s\"", (int) header->realm.slen,
				header->realm.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->algorithm.slen>0) {
		dem = sprintf(point, ",algorithm=\"%.*s\"",
				(int) header->algorithm.slen, header->algorithm.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->nonce.slen>0) {
		dem = sprintf(point, ",nonce=\"%.*s\"", (int) header->nonce.slen,
				header->nonce.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->opaque.slen>0) {
		dem = sprintf(point, ",opaque=\"%.*s\"", (int) header->opaque.slen,
				header->opaque.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->qop.slen>0) {
		dem = sprintf(point, ",qop=%.*s", (int) header->qop.slen,
				header->qop.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	if (header->domain.slen>0) {
		dem = sprintf(point, ",domain=%.*s", (int) header->domain.slen,
				header->domain.ptr);
		tong = tong + dem;
		if (tong > size)
			return -1;
		point = point + dem;
	}
	return tong;
}
