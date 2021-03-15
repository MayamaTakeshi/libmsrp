AUTOMAKE_OPTIONS = foreign
noinst_LIBRARIES =  liblmsrp.a

liblmsrp_a_SOURCES = $(top_srcdir)/src/to_string/lmsrp_www_tostring.c \
$(top_srcdir)/src/to_string/lmsrp_mess_tostring.c \
$(top_srcdir)/src/to_string/lmsrp_auth_tostring.c \
$(top_srcdir)/src/to_string/lmsrp_list_uri_tostring.c \
$(top_srcdir)/src/stream/lmsrp_ctx.c \
$(top_srcdir)/src/prase/lmsrp_www_h.c \
$(top_srcdir)/src/prase/lmsrp_authorization.c \
$(top_srcdir)/src/prase/lmsrp_common.c \
$(top_srcdir)/src/prase/lmsrp_auth_common.c \
$(top_srcdir)/src/prase/lmsrp_info.c \
$(top_srcdir)/src/prase/lmsrp_mess.c \
$(top_srcdir)/src/prase/lmsrp_status.c \
$(top_srcdir)/src/prase/lmsrp_byte_range.c \
$(top_srcdir)/src/prase/lmsrp_content_type.c \
$(top_srcdir)/src/create/lmsrp_mess_create.c \
$(top_srcdir)/src/create/lmsrp_auth_create.c 


liblmsrp_a_CFLAGS = -I $(top_srcdir)/include  $(PJSIP_CFLAGS)

