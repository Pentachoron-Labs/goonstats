# Process this file with autoconf to produce a configure script.

# start out
AC_INIT([goonstats], [0.1])
AC_CONFIG_SRCDIR([librofl/rofl.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_MACRO_DIRS([m4])

# Check for availability of various components
AC_PROG_CC
AM_PROG_AR
AC_CHECK_HEADERS([stdint.h])
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# other things
LT_INIT

# build our environment
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile librofl/Makefile])

AC_OUTPUT
