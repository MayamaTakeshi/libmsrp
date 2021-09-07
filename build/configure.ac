#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
#AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_INIT(RCS_AS,0.1)



AM_INIT_AUTOMAKE([ foreign -Wnone ])
AC_CONFIG_SRCDIR([../src/libmsrp.c])
AM_SILENT_RULES([yes])
# Checks for programs.
AC_PROG_CC
AM_PROG_AR
AC_PROG_RANLIB
# Checks for libraries.
PKG_CHECK_MODULES([PJSIP],[libpjproject])

AC_SUBST([BDIR])
AC_SUBST([LIB_NAME])


#AC_SUBST([NAME])


LIB_NAME="lmsrp"



if test -z $host
    then
     BDIR="unknow"
    else
     BDIR="$host"
fi
#AC_CONFIG_FILES([Makefile
#                 build/Makefile
#                build/output/Makefile
#])

AC_CONFIG_FILES([
			Makefile
               output/Makefile
])
AC_OUTPUT

