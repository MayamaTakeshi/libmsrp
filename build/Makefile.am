gen_lib: all create_lib

create_lib:
	-./libtool --mode=install /usr/bin/install -c ./@BDIR@/liblmsrp.la  @abs_top_srcdir@/@BDIR@
	-./libtool --mode=finish @abs_top_srcdir@/@BDIR@

clean-lib: 
	-rm -rf ./@BDIR@/.*
	-rm -f ./@BDIR@/*.la

clean-local: clean-lib
	
SUBDIRS = output .

obj-clean:
	-rm -f output/*.o


