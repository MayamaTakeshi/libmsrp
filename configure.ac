#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
#AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_INIT(RCS_AS,0.1)

AC_SUBST([BDIR])


AM_INIT_AUTOMAKE([ foreign -Wsyntax ])
AC_CONFIG_SRCDIR([src/libmsrp.c])
AM_SILENT_RULES([yes])
# Checks for programs.
AC_PROG_CC
AM_PROG_AR
AC_PROG_RANLIB
# Checks for libraries.
PKG_CHECK_MODULES([PJSIP],[libpjproject])

AC_SUBST([LIB_NAME])

#AC_SUBST([NAME])
if [ -z $host ]
    then
      LIB_NAME="lmsrp_$host"
    else
      LIB_NAME="lmsrp_unknow"
fi

if [ -z $host ]
    then
     BDIR="$host"
    else
     BDIR="unknow"
fi

mkdir -p build/"$BDIR"/

AC_CONFIG_FILES([Makefile
                 build/Makefile
])
AC_OUTPUT

#if [ ! -d ./build/"$BDIR"/ ]
#then
#mkdir ./build/"$BDIR"/
#fi
#cp -r  tmp/.  build/"$BDIR"/.
