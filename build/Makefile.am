AUTOMAKE_OPTIONS = foreign
noinst_LIBRARIES =  liblmsrp.a
all:
liblmsrp_a_SOURCES = ../src/to_string/lmsrp_www_tostring.c \
../src/to_string/lmsrp_mess_tostring.c \
../src/to_string/lmsrp_auth_tostring.c \
../src/to_string/lmsrp_list_uri_tostring.c \
../src/prase/lmsrp_www_h.c \
../src/prase/lmsrp_authorization.c \
../src/prase/lmsrp_common.c \
../src/prase/lmsrp_auth_common.c \
../src/prase/lmsrp_info.c \
../src/prase/lmsrp_mess.c \
../src/prase/lmsrp_status.c \
../src/prase/lmsrp_byte_range.c \
../src/prase/lmsrp_content_type.c \
../src/create/lmsrp_mess_create.c \
../src/create/lmsrp_auth_create.c 
liblmsrp_a_CFLAGS = -I $(top_srcdir)/include  $(PJSIP_CFLAGS)


test:
noinst_PROGRAMS = msrp
msrp_SOURCES = $(top_srcdir)/src/libmsrp.c
msrp_CFLAGS = -I $(top_srcdir)/include  $(PJSIP_CFLAGS)
msrp_LDADD  = $(PJSIP_LIBS) liblmsrp.a
