#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([nuvoton-ipmi-oem], [0.1], [https://github.com/Nuvoton-Israel/nuvoton-ipmi-oem/issues])
AC_LANG([C++])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror -Wno-portability foreign dist-xz])

# Checks for programs.
AC_PROG_CXX
AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT8_T
AX_CXX_COMPILE_STDCXX_17([noext])
AX_APPEND_COMPILE_FLAGS([-Wall -Werror], [CXXFLAGS])

# Checks for libraries.
PKG_CHECK_MODULES([SDBUSPLUS], [sdbusplus])
PKG_CHECK_MODULES([PHOSPHOR_LOGGING], [phosphor-logging])
PKG_CHECK_MODULES(
    [LIBIPMID],
    [libipmid],
    [],
    [AC_MSG_ERROR([Could not find libipmid...openbmc/phosphor-host-ipmid package required])]
)
AC_CHECK_HEADER(
    nlohmann/json.hpp,
    [],
    [AC_MSG_ERROR([Could not find nlohmann/json.hpp])]
)

# Checks for header files.



# Checks for library functions.
LT_INIT # Required for systemd linking


PKG_PROG_PKG_CONFIG
AC_ARG_WITH([systemdsystemunitdir],
     [AS_HELP_STRING([--with-systemdsystemunitdir=DIR], [Directory for systemd service files])],,
     [with_systemdsystemunitdir=auto])
AS_IF([test "x$with_systemdsystemunitdir" = "xyes" -o "x$with_systemdsystemunitdir" = "xauto"], [
     def_systemdsystemunitdir=$($PKG_CONFIG --variable=systemdsystemunitdir systemd)

     AS_IF([test "x$def_systemdsystemunitdir" = "x"],
   [AS_IF([test "x$with_systemdsystemunitdir" = "xyes"],
    [AC_MSG_ERROR([systemd support requested but pkg-config unable to query systemd package])])
    with_systemdsystemunitdir=no],
   [with_systemdsystemunitdir="$def_systemdsystemunitdir"])])
AS_IF([test "x$with_systemdsystemunitdir" != "xno"],
      [AC_SUBST([systemdsystemunitdir], [$with_systemdsystemunitdir])])
AM_CONDITIONAL([HAVE_SYSTEMD], [test "x$with_systemdsystemunitdir" != "xno"])

AC_ARG_VAR(SOC, [platform target SOC])
AS_IF([test "x$SOC" != "x"],
    AC_MSG_NOTICE([SOC set $SOC])
    [cpp_flags="-D$SOC"]
    AC_SUBST([CPPFLAGS], [$cpp_flags]),
    AC_MSG_WARN([SOC not defined])
)

AC_ARG_VAR(DISABLE_SERACH_MUX_BUS, [set mux channel directly instead of search I2C mux bus])
AS_IF([test "x$DISABLE_SERACH_MUX_BUS" == "x"],
    AC_MSG_NOTICE([search mux bus default set enabled])
    [cpp_flags="-DSEARCH_MUX_BUS"]
    AC_SUBST([CPPFLAGS], [$cpp_flags]),
    AC_MSG_NOTICE([search mux bus set disabled])
)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
