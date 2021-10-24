/*
 * lmsrp.h
 *
 *  Created on: Jan 9, 2021
 *      Author: lmsrpneiht
 */

#ifndef LMSRP_FILE_LMSRP_MODULE_H_
#define LMSRP_FILE_LMSRP_MODULE_H_

#include <lmsrp.h>
#include <pjlib.h>

#if !defined ( PJ_HAS_SSL_SOCK) || PJ_HAS_SSL_SOCK !=1
#define lmsrp_HAS_TLS 0
#else
#define lmsrp_HAS_TLS 0
#endif
typedef enum {
	lmsrp_block_state_free,  /// no user block
	lmsrp_block_state_recv, /// this block is use for recv data
	lmsrp_block_state_raw, /// this block have new raw data and not handling
	lmsrp_block_state_ready /// this block is ready to use
} lmsrp_block_state;
typedef enum {
	lmsrp_flag_end, /// nguoi dung bao gui het goi tin
	lmsrp_flag_continue, /// phien dang hoat dong
	lmsrp_flag_wait, /// phien cho ket thuc
	lmsrp_flag_suspend, /// ngat phien
	lmsrp_flag_close /// phien thanh cong
} lmsrp_flag;

typedef struct lmsrp_block_list {
	PJ_DECL_LIST_MEMBER( struct lmsrp_block_list)
	;
	char *data;
	lmsrp_byte_range range;
	pj_uint32_t size;
	lmsrp_block_state state;
	char flag;
} lmsrp_block_list;

typedef struct lmsrp_report_list {
	PJ_DECL_LIST_MEMBER(struct lmsrp_report_list)
	;
	lmsrp_byte_range rang;
	pj_bool_t active;
} lmsrp_report_list;
//extern struct lmsrp_recv_data_block;

typedef struct lmsrp_recv_data_block {
	PJ_DECL_LIST_MEMBER(struct lmsrp_recv_data_block)
	;
	char *data;
	int size;
	int package_count;
	lmsrp_block_state state;
} lmsrp_recv_data_block;
#if !defined ( lmsrp_USE_TLS) || lmsrp_USE_TLS !=1
typedef struct lmsrp_transport_module {
	/**
	 * init socker and bind to port
	 * @param pool
	 * @param local
	 */
	void* (*init)(pj_pool_t *pool, pj_sockaddr_in *local);
	/**
	 * connect to server and return socket
	 * @param addr
	 */
	pj_status_t (*connect)(void *sock, pj_sockaddr_in *addr);
	/**
	 * send data to addr
	 * @param sock socket object
	 * @param addr destination address
	 * @param data data to send
	 * @param length leng of data
	 * @return pj_sucsess
	 */
	pj_status_t (*send)(void *sock, void *data, pj_ssize_t length);

	/**
	 * recv data from server
	 * @param sock
	 * @param addr
	 * @param data
	 * @param length
	 * @return
	 */
	pj_status_t (*recv)(void *sock, void *data, pj_ssize_t *length);

	/**
	 * recv data from server with time out handle
	 * @param sock
	 * @param addr
	 * @param data
	 * @param length
	 * @return 0 if sucess , 1 if time out
	 */
	pj_status_t (*recv_t)(void *sock, void *data, pj_ssize_t *length, int msec);
	/**
	 * accept to new connect
	 * @param sock server soc
	 * @param tmp tempory pool
	 * @return new sock
	 */
	void* (*accept)(void *sock, pj_pool_t *tmp);
	void (*close)(void *sock);
} lmsrp_transport_module;
#else
typedef struct msrp_transport {
	/**
		 * init socker and bind to port
		 * @param pool
		 * @param local
		 */
		void* (*init)(pj_pool_t *pool, pj_sockaddr_in *local);
		/**
		 * connect to server and return socket
		 * @param addr
		 */
		pj_status_t (*connect)(void *sock, pj_sockaddr_in *addr);
		/**
		 * send data to addr
		 * @param sock socket object
		 * @param addr destination address
		 * @param data data to send
		 * @param length leng of data
		 * @return pj_sucsess
		 */
		pj_status_t (*send)(void *sock, void *data, pj_ssize_t length);

		/**
		 * recv data from server
		 * @param sock
		 * @param addr
		 * @param data
		 * @param length
		 * @return
		 */
		pj_status_t (*recv)(void *sock, void *data, pj_ssize_t *length);

		/**
		 * recv data from server with time out handle
		 * @param sock
		 * @param addr
		 * @param data
		 * @param length
		 * @return 0 if sucess , 1 if time out
		 */
		pj_status_t (*recv_t)(void *sock, void *data, pj_ssize_t *length, int msec);
		/**
		 * accept to new connect
		 * @param sock server soc
		 * @param tmp tempory pool
		 * @return new sock
		 */
		void* (*accept)(void *sock, pj_pool_t *tmp);
		void (*close)(void *sock);
};
#endif
typedef struct lmsrp_session_info {
	lmsrp_uri *caller;
	lmsrp_uri *callee;
	pj_str_t *filename;
	pj_str_t *filepath;
} lmsrp_session_info;

typedef enum lmsrp_report_code {
	lmsrp_REPORT_OK = 200,
	lmsrp_REPORT_RESEND = 413,
	lmsrp_REPORT_NOTSUPPORT = 501
} lmsrp_report_code;

/**
 * @note : info must custom in each session . this is fisrt verion , and it is not support for mutil session in one time
 */
typedef struct lmsrp_send_module {
	pj_pool_t *pool; /// session pool , it will be reset after session close
	/**
	 * init data modules
	* @param module_data pointer to module data
	 * @param filepath filepath to send if you want
	 * @return pj_susscess if ook
	 * @note this function is optional
	 * @return total size of octet
	 */
	pj_int64_t (*init)(void *module_data, pj_str_t *filepath);
	/**
	 * read data from source
	 * @param module_data pointer to module data
	 * @param buf array buffer
	 * @param maxsize max data in one message
	 * @return number of byte is read , -1 if error
	 */

	size_t (*read)(void *module_data, char *buf, size_t start, size_t end);
	/**
	 * send msrp message by user's transport
	 * @param module_data pointer to  module data
	 * @param buff message
	 * @param leng length of message
	 * @return 0 if succcess full
	 */
	int (*send)(void *module_data, char *buff, long leng);

	/**
	 * get report from callee
	 * @param module_data pointer to module data
	 * @param buff buffer to store data
	 * @param leng leng of buff
	 * @return respone cose
	 */
	pj_status_t (*get_report)(void *module_data, char *buff, pj_ssize_t *leng);

	/**
	 * close all resource to use
	 * @param module_data pointer to  module data
	 * @param report report it is done [0] or fail [1]
	 * @return return 0 if done , otherwise is fail
	 * @note this function is optional
	 */
	int (*close)(void *module_data);

	/**
	 * suppend data if error
	 * @param module_data pointer to  module data
	 * @return return 0 if done , otherwise is fail
	 * @note this function is optional
	 */

	int (*suppend)(void *module_data);
	/**
	 * module data
	 */
	void *data;

} lmsrp_send_module;

typedef struct lmsrp_recv_module {
	pj_pool_t *pool;  /// session pool , it will be reset after session close
	/**
	 * init msrp recv data module
	 * @param init_data user data pointer
	 * @param filename name of file
	 * @param inf list info
	 * @note this function is primary
	 */
	void (*init)(void *init_data, pj_str_t *filename);
	/**
	 * write data to byte
	 * @param getdata user_data pointer
	 * @param info byte-rang info of mess
	 * @param buf buffer to wirte
	 * @param length length of buffered
	 * @return 0 if done , otherwise is fail
	 * @note : you should free memnory of write data , when it is no use
	 */

	int (*write)(void *getdata, char *buff, pj_int64_t start, pj_int64_t end);
	/**
	 * write data file header
	 * @param getdata user data
	 * @param header data buffer
	 * @param size size of header
	 * @return
	 */
	int (*write_header)(void *getdata, char *header, size_t size);
	/**
	 * send report to caller
	 * @param userdata data
	 * @param buff report message buffer
	 * @param leng leng of buffer
	 * @param sock , socket connet ,  if in MSRP_OVER_SIP . it is null
	 */
	void (*send_report)(void *userdata, char *buff, int leng);

	/**
	 * get data abd put in block, for each block are wrote , you should set flag to
	 * @param pool
	 * @param userdata
	 * @param block
	 * @param report report it done or false
	 * @return
	 */
	pj_status_t (*get_mess)(pj_pool_t *pool, void *userdata,
			lmsrp_recv_data_block *block);

	/**
	 * close all resource to use
	 * @param module_data pointer to user data
	 * @param report report it is done [0] or fail [1]
	 * @return return 0 if done , otherwise is fail
	 * @note this function is optional
	 */
	int (*close)(void *module_data, int report);

	/** get pj_caching_pool
	 *
	 */
	pj_caching_pool* (*get_cp)(void *module_data);

	/**
	 * create user define data header
	 * @param buff
	 * @param user
	 * @param size
	 * @param time
	 * @return
	 */
	int (*create_header)(char *buff, pj_str_t *user, pj_ssize_t size, long time);

	void *data;
} lmsrp_recv_module;

/**
 * write data to list . this is thread safe
 * @param inf
 * @param dtx
 */

void lmsrp_init_tcp_transport_module(lmsrp_transport_module *trans);
void lmsrp_init_udp_transport_module(lmsrp_transport_module *trans);

pj_str_t* lmsrp_login(pj_pool_t *pool, void *sock, lmsrp_transport_module *car,
		lmsrp_cred_info *cred, pj_sockaddr_in *server, int local_port,
		pj_str_t *sessid);

//TODO : doi ten sau

#endif /* SIP_lmsrp_H_ */
