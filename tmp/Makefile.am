#AUTOMAKE_OPTIONS = subdir-objects
#noinst_PROGRAMS = testmsrp
#testmsrp_SOURCES = $(top_srcdir)/src/libmsrp.c
#testmsrp_CFLAGS = -I $(top_srcdir)/include  $(PJSIP_CFLAGS) 
#testmsrp_LDADD =  liblmsrp"$(HOST_PREFIX)".a  $(PJSIP_LIBS)


noinst_LIBRARIES =  lib@LIB_NAME@.a
lib@LIB_NAME@_a_SOURCES = $(abs_top_srcdir)/src/to_string/lmsrp_www_tostring.c \
$(abs_top_srcdir)/src/to_string/lmsrp_mess_tostring.c \
$(abs_top_srcdir)/src/to_string/lmsrp_auth_tostring.c \
$(abs_top_srcdir)/src/to_string/lmsrp_list_uri_tostring.c \
$(abs_top_srcdir)/src/stream/lmsrp_ctx.c \
$(abs_top_srcdir)/src/prase/lmsrp_www_h.c \
$(abs_top_srcdir)/src/prase/lmsrp_authorization.c \
$(abs_top_srcdir)/src/prase/lmsrp_common.c \
$(abs_top_srcdir)/src/prase/lmsrp_auth_common.c \
$(abs_top_srcdir)/src/prase/lmsrp_info.c \
$(abs_top_srcdir)/src/prase/lmsrp_mess.c \
$(abs_top_srcdir)/src/prase/lmsrp_status.c \
$(abs_top_srcdir)/src/prase/lmsrp_byte_range.c \
$(abs_top_srcdir)/src/prase/lmsrp_content_type.c \
$(abs_top_srcdir)/src/create/lmsrp_mess_create.c \
$(abs_top_srcdir)/src/create/lmsrp_auth_create.c

lib@LIB_NAME@_a_CFLAGS = -I $(abs_top_srcdir)/include  $(PJSIP_CFLAGS)


