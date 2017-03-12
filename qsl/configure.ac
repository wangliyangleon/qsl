#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])

m4_define([qsl_version_str], m4_esyscmd_s([cat VERSION]))
m4_define([author_email_str], m4_esyscmd_s([awk '{print $2}' AUTHOR]))
AC_INIT([qsl], qsl_version_str, author_email_str)

AC_CONFIG_SRCDIR([CommonInclude.h])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.9 foreign tar-ustar nostdinc subdir-objects])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_LANG([C++])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AX_CXX_COMPILE_STDCXX_14

CXXFLAGS="-fno-strict-aliasing $CXXFLAGS"
CXXFLAGS="-O3 -g -W -Wall -Wextra -Wno-unused-parameter $CXXFLAGS"
CXXFLAGS=" -Wno-missing-field-initializers -Wno-deprecated $CXXFLAGS"
CXXFLAGS="-DLIBMC_FBTRACE_DISABLE $CXXFLAGS"

CFLAGS="-DLIBMC_FBTRACE_DISABLE $CFLAGS"

# Checks for libraries.
AC_CHECK_LIB([glog],[openlog],[],[AC_MSG_ERROR(
             [Please install google-glog library])])
AC_CHECK_LIB([gflags],[getenv],[],AC_MSG_ERROR(
             [Please install google-gflags library]))


# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T


# Checks for library functions.
AC_FUNC_MALLOC

AM_CPPFLAGS='-I$(top_srcdir) -I$(top_srcdir)/GSL/include -I$(top_srcdir)/test/googletest-release-1.8.0/googlemock/include -I$(top_srcdir)/test/googletest-release-1.8.0/googletest/include'
AM_CPPFLAGS="$AM_CPPFLAGS $CXX_FLAGS"
AC_SUBST([AM_CPPFLAGS])


AC_CONFIG_FILES([Makefile
                 test/Makefile])
AC_OUTPUT
