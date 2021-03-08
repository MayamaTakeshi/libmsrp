#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([src/libmsrp.c])
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall])

# Checks for programs.
AC_PROG_CC
AM_PROG_AR
AC_PROG_RANLIB
# Checks for libraries.
PKG_CHECK_MODULES([PJSIP],[libpjproject])
# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 build/Makefile])
AC_OUTPUT
