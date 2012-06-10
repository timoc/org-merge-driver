# Process this file with autoconf to produce a configure script.
AC_INIT([org-merge-driver], [0.1], [youngar17@gmail.com])
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE([-Wall -Werror gnu])

AC_CONFIG_MACRO_DIR([m4])

# Check for programs
AC_PROG_CC   # c compiler
AM_PROG_LEX  # flex
AC_PROG_YACC # bison

# gnulib macros
gl_EARLY
gl_INIT

# Check for headers
# Check for typedefs, structures, and compilers characteristics
# Check for library functions

# Output files
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
