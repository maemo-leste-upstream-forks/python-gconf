-*- mode: autoconf -*-
AC_PREREQ(2.52)

dnl the gnome-python version number
m4_define(gnome_python_major_version, 2)
m4_define(gnome_python_minor_version, 28)
m4_define(gnome_python_micro_version, 1)
m4_define(gnome_python_version, dnl
  gnome_python_major_version.gnome_python_minor_version.gnome_python_micro_version)

dnl required versions of other packages
m4_define(pygtk_required_major_version, 2)
m4_define(pygtk_required_minor_version, 10)
m4_define(pygtk_required_micro_version, 3)
m4_define(pygtk_required_version, pygtk_required_major_version.pygtk_required_minor_version.pygtk_required_micro_version)
m4_define(pyorbit_required_version,           2.0.1)
m4_define(glib_required_version,              2.6.0)
m4_define(gtk_required_version,               2.6.0)
m4_define(libgnome_required_version,          2.8.0)
m4_define(libgnomeui_required_version,        2.8.0)
m4_define(libgnomecanvas_required_version,    2.8.0)
m4_define(libgnomevfs_required_version,       2.14.0)
m4_define(gconf_required_version,             2.11.1)
m4_define(bonobo_activation_required_version, 2.8.0)
m4_define(libbonobo_required_version,         2.8.0)
m4_define(libbonoboui_required_version,       2.8.0)
m4_define(pygobject_required_version,         2.17.0)

AC_INIT(gnome-python, gnome_python_version,
        [http://bugzilla.gnome.org/enter_bug.cgi?product=gnome-python])
dnl AC_CONFIG_SRCDIR([gnome/uimodule.c])
AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE

AC_DEFINE(GNOME_PYTHON_MAJOR_VERSION, gnome_python_major_version, [Gnome-Python major version])
AC_DEFINE(GNOME_PYTHON_MINOR_VERSION, gnome_python_minor_version, [Gnome-Python minor version])
AC_DEFINE(GNOME_PYTHON_MICRO_VERSION, gnome_python_micro_version, [Gnome-Python macro version])

dnl Define pygtk required version, for runtime check
AC_DEFINE(PYGTK_REQUIRED_MAJOR_VERSION, pygtk_required_major_version, [PyGTK required major version])
AC_DEFINE(PYGTK_REQUIRED_MINOR_VERSION, pygtk_required_minor_version, [PyGTK required minor version])
AC_DEFINE(PYGTK_REQUIRED_MICRO_VERSION, pygtk_required_micro_version, [PyGTK required micro version])

dnl AC_ARG_BINDING(NAME, DEFAULT [, MODULENAME])
AC_DEFUN([AC_ARG_BINDING],
[AC_ARG_ENABLE([$1],
             AC_HELP_STRING([--ifelse([$2], [YES], dis, en)able-[$1]],
                            [build ifelse([$3], , [$1], [$3]) module (default is [$2])]),
             [if test "$enable_[$1]" = "yes"; then 
	     		build_[$1]=true
			  else
				build_[$1]=false
			 fi])
])

GNOME_COMMON_INIT

dnl put the ACLOCAL flags in the makefile
ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

AC_DISABLE_STATIC
AC_PROG_LIBTOOL
dnl when using libtool 2.x create libtool early, because it's used in configure
m4_ifdef([LT_OUTPUT], [LT_OUTPUT])

dnl check for python
AM_PATH_PYTHON(2.2)
AM_CHECK_PYTHON_HEADERS(,[AC_MSG_ERROR(could not find Python headers or library)])
dnl make the python compiler used globally
CC="$PYTHON_CC"

dnl check for pygobject
PKG_CHECK_MODULES(PYGOBJECT, pygobject-2.0 >= pygobject_required_version)

dnl check for pygtk
PKG_CHECK_MODULES(PYGTK, pygtk-2.0 >= pygtk_required_version)
AC_SUBST(PYGTK_CFLAGS)

dnl check for codegen script
AC_PATH_PROG(PYGOBJECT_CODEGEN, pygobject-codegen-2.0, no)
if test "x$PYGOBJECT_CODEGEN" = xno; then
  dnl This is for compat with older releases when codegen was shipped
  dnl in pygtk. It should be removed in future releases.
  AC_PATH_PROG(PYGTK_CODEGEN, pygtk-codegen-2.0, no)
  if test "x$PYGTK_CODEGEN" = xno; then
    AC_MSG_ERROR(could not find pygobject-codegen-2.0 script)
  else
    AC_SUBST(PYGNOME_CODEGEN, $PYGTK_CODEGEN)
  fi
else
  AC_SUBST(PYGNOME_CODEGEN, $PYGOBJECT_CODEGEN)
fi

AC_MSG_CHECKING([for conflicting pygtk versions])
pygtk_version=`$PKG_CONFIG --modversion pygtk-2.0`
case $pygtk_version in
     2.9.0|2.9.1)
        AC_MSG_RESULT([found $pygtk_version])
        AC_MSG_ERROR([invalid pygtk version found; please upgrade])
        ;;
     *)
        AC_MSG_RESULT([none])
        ;;
esac

AC_MSG_CHECKING(for pygtk defs)
PYGTK_DEFSDIR=`$PKG_CONFIG --variable=defsdir pygtk-2.0`
AC_SUBST(PYGTK_DEFSDIR)
AC_MSG_RESULT($PYGTK_DEFSDIR)

dnl required version substitutions for use in the spec file
PYGTK_VERSION=pygtk_required_version
LIBGNOMECANVAS_VERSION=libgnomecanvas_required_version
LIBGNOME_VERSION=libgnome_required_version
LIBGNOMEVFS_VERSION=libgnomevfs_required_version
GTK_VERSION=gtk_required_version
PYORBIT_VERSION=pyorbit_required_version
BONOBO_ACTIVATION_VERSION=bonobo_activation_required_version
LIBBONOBO_VERSION=libbonobo_required_version
LIBBONOBOUI_VERSION=libbonoboui_required_version
GCONF_VERSION=gconf_required_version
AC_SUBST(PYGTK_VERSION)
AC_SUBST(LIBGNOMECANVAS_VERSION)
AC_SUBST(LIBGNOME_VERSION)
AC_SUBST(LIBGNOMEVFS_VERSION)
AC_SUBST(GLIB_VERSION)
AC_SUBST(GTK_VERSION)
AC_SUBST(PYORBIT_VERSION)
AC_SUBST(BONOBO_ACTIVATION_VERSION)
AC_SUBST(LIBBONOBO_VERSION)
AC_SUBST(LIBBONOBOUI_VERSION)
AC_SUBST(GCONF_VERSION)

dnl get rid of the -export-dynamic stuff from the configure flags ...
export_dynamic=`(./libtool --config; echo eval echo \\$export_dynamic_flag_spec) | sh`

dnl check for glib
AM_PATH_GLIB_2_0(glib_required_version,,[AC_MSG_ERROR(maybe you want the gtk-gnome-1-2 branch?)],$extra_mods)
if test -n "$export_dynamic"; then
  GLIB_LIBS=`echo $GLIB_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl check to make sure we can find gtk
AM_PATH_GTK_2_0(gtk_required_version,,[AC_MSG_ERROR(maybe you want the gtk-gnome-1-2 branch?)],$extra_mods)
if test -n "$export_dynamic"; then
  GTK_LIBS=`echo $GTK_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl ****************************************************************************
dnl * Check for prctl() or setproctitle()
dnl ****************************************************************************
AC_CHECK_FUNCS(prctl setproctitle)

AC_ARG_ENABLE([gtk-doc],
             AC_HELP_STRING([--disable-gtk-doc],
                            [Disables gtk-doc documentation generation]),,
		[enable_gtk_doc="yes"])
AM_CONDITIONAL(BUILD_DOCS, [test "$enable_gtk_doc" = "yes"])

dnl default bindings to build:
default_bindings_on="gnome gnomeui gnomecanvas gnomevfs gnomevfsbonobo pyvfsmodule gconf bonobo_activation bonobo bonoboui"
default_bindings_off=""
default_bindings_all="$default_bindings_on $default_bindings_off"

AC_ARG_ENABLE([allbindings],
             AC_HELP_STRING([--disable-allbindings],
                            [Changes all the bindings default values to be either enabled or disabled, overridden by any explicit bindings on the commandline.]),
             [if test "$enable_allbindings" = "no"; then
			   default_bindings_off="$default_bindings_all"
			   default_bindings_on=""
		  	 else
			   default_bindings_off=""
			   default_bindings_on="$default_bindings_all"
		     fi])
for binding in $default_bindings_on; do
	eval build_$binding=true
done
for binding in $default_bindings_off; do
	eval build_$binding=false
done


have_pyorbit=false
PKG_CHECK_MODULES(PYORBIT,
  pyorbit-2 >= pyorbit_required_version,
  have_pyorbit=true, have_pyorbit=false)
AC_SUBST(PYORBIT_CFLAGS)

AC_ARG_BINDING([gnome], [ON])
if $build_gnome; then
  PKG_CHECK_MODULES(GNOME, libgnome-2.0 >= libgnome_required_version,,
    [AC_MSG_ERROR(libgnome-2.0 is required.)])
  AC_SUBST(GNOME_CFLAGS)
  AC_SUBST(GNOME_LIBS)
  if test -n "$export_dynamic"; then
    GNOME_LIBS=`echo $GNOME_LIBS | sed -e "s/$export_dynamic//"`
  fi
fi
AM_CONDITIONAL(BUILD_GNOME, $build_gnome)

AC_ARG_BINDING([gnomeui], [ON], [gnome.ui])
if $build_gnomeui; then
  PKG_CHECK_MODULES(GNOMEUI, libgnomeui-2.0 >= libgnomeui_required_version,
    build_gnomeui=true,
    build_gnomeui=false)
  AC_SUBST(GNOMEUI_CFLAGS)
  AC_SUBST(GNOMEUI_LIBS)
fi
AM_CONDITIONAL(BUILD_GNOMEUI, $build_gnome && $have_pyorbit && $build_gnomeui)
if test -n "$export_dynamic"; then
  GNOMEUI_LIBS=`echo $GNOMEUI_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl should we build the gnome.canvas module?
AC_ARG_BINDING([gnomecanvas], [ON])
if $build_gnomecanvas; then
  PKG_CHECK_MODULES(GNOMECANVAS, libgnomecanvas-2.0 >= libgnomecanvas_required_version,
    build_gnomecanvas=true,
    build_gnomecanvas=false)
  AC_SUBST(GNOMECANVAS_CFLAGS)
  AC_SUBST(GNOMECANVAS_LIBS)
fi
AM_CONDITIONAL(BUILD_GNOMECANVAS, $build_gnomecanvas)
if test -n "$export_dynamic"; then
  GNOMECANVAS_LIBS=`echo $GNOMECANVAS_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl should we build the gnomevfs module?
AC_ARG_BINDING([gnomevfs], [ON])
if $build_gnomevfs; then
  PKG_CHECK_MODULES(GNOMEVFS, [gnome-vfs-2.0 >= libgnomevfs_required_version],
    build_gnomevfs=true,
    build_gnomevfs=false)
  AC_SUBST(GNOMEVFS_CFLAGS)
  AC_SUBST(GNOMEVFS_LIBS)
fi
AM_CONDITIONAL(BUILD_GNOMEVFS, $build_gnomevfs)
if test -n "$export_dynamic"; then
  GNOMEVFS_LIBS=`echo $GNOMEVFS_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl should we build bonobo support into the gnomevfs module?
AC_ARG_BINDING([gnomevfsbonobo], [ON], [gnomevfs.bonobo])
if $build_gnomevfsbonobo; then
  PKG_CHECK_MODULES(GNOMEVFSBONOBO, [gnome-vfs-2.0 >= libgnomevfs_required_version dnl
				     bonobo-activation-2.0 >= bonobo_activation_required_version
				     pyorbit-2 >= pyorbit_required_version],
    build_gnomevfsbonobo=true,
    build_gnomevfsbonobo=false)
  AC_SUBST(GNOMEVFSBONOBO_CFLAGS)
  AC_SUBST(GNOMEVFSBONOBO_LIBS)
fi
AM_CONDITIONAL(BUILD_GNOMEVFSBONOBO, $build_gnomevfs && $build_gnomevfsbonobo)
if test -n "$export_dynamic"; then
  GNOMEVFSBONOBO_LIBS=`echo $GNOMEVFSBONOBO_LIBS | sed -e "s/$export_dynamic//"`
fi


dnl should we build the gnomevfs python module bridge?
AC_ARG_BINDING([pyvfsmodule], [ON], [bridge])
if $build_pyvfsmodule; then
  PKG_CHECK_MODULES(GNOME_VFS_MODULE, [gnome-vfs-module-2.0 >= libgnomevfs_required_version dnl
				       bonobo-activation-2.0 >= bonobo_activation_required_version],
    build_pyvfsmodule=true,
    build_pyvfsmodule=false)
fi
AM_CONDITIONAL(BUILD_PYVFSMODULE, $build_gnomevfs && $build_pyvfsmodule)
if test -n "$export_dynamic"; then
  GNOME_VFS_MODULE_LIBS=`echo $GNOME_VFS_MODULE_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl should we build the gconf module?
AC_ARG_BINDING([gconf], [ON])
if $build_gconf; then
  PKG_CHECK_MODULES(GCONF, gconf-2.0 >= gconf_required_version,
    build_gconf=true,
    build_gconf=false)
  GCONF_INCLUDEDIR="`pkg-config gconf-2.0 --variable includedir`/gconf/2/gconf"
  AC_SUBST(GCONF_INCLUDEDIR)
  AC_SUBST(GCONF_CFLAGS)
  AC_SUBST(GCONF_LIBS)
fi
AM_CONDITIONAL(BUILD_GCONF, $build_gconf)
if test -n "$export_dynamic"; then
  GCONF_LIBS=`echo $GCONF_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl should we build the bonobo.activation module?
AC_ARG_BINDING([bonobo_activation], [ON], [bonobo.activation])
if $build_bonobo_activation; then
  PKG_CHECK_MODULES(BONOBO_ACTIVATION, bonobo-activation-2.0 >= bonobo_activation_required_version,
    build_bonobo_activation=true, build_bonobo_activation=false)
  AC_SUBST(BONOBO_ACTIVATION_CFLAGS)
  AC_SUBST(BONOBO_ACTIVATION_LIBS)
fi
AM_CONDITIONAL(BUILD_BONOBO_ACTIVATION, $have_pyorbit && $build_bonobo_activation)
if test -n "$export_dynamic"; then
  BONOBO_ACTIVATION_LIBS=`echo $BONOBO_ACTIVATION_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl should we build the bonobo._bonobo module?
AC_ARG_BINDING([bonobo], [ON], [bonobo._bonobo])
if $build_bonobo; then
  PKG_CHECK_MODULES(BONOBO, libbonobo-2.0 >= libbonobo_required_version,
    build_bonobo=true, build_bonobo=false)
  AC_SUBST(BONOBO_CFLAGS)
  AC_SUBST(BONOBO_LIBS)
fi
AM_CONDITIONAL(BUILD_BONOBO, $have_pyorbit && $build_bonobo)
if test -n "$export_dynamic"; then
  BONOBO_LIBS=`echo $BONOBO_LIBS | sed -e "s/$export_dynamic//"`
fi

dnl should we build the bonobo.ui module?
AC_ARG_BINDING([bonoboui], [ON], [bonobo.ui])
if $build_bonoboui; then
  PKG_CHECK_MODULES(BONOBOUI, libbonoboui-2.0 >= libbonoboui_required_version libgnome-2.0 >= libgnome_required_version,
    build_bonoboui=true, build_bonoboui=false)
  AC_SUBST(BONOBOUI_CFLAGS)
  AC_SUBST(BONOBOUI_LIBS)
fi
AM_CONDITIONAL(BUILD_BONOBOUI, $have_pyorbit && $build_bonoboui)
if test -n "$export_dynamic"; then
  BONOBOUI_LIBS=`echo $BONOBOUI_LIBS | sed -e "s/$export_dynamic//"`
fi

AM_CONDITIONAL(BUILD_BONOBO_ANY, $build_bonobo || $build_bonoboui || $build_bonobo_activation)


dnl add required cflags ...
JH_ADD_CFLAG([-Wall])
JH_ADD_CFLAG([-std=c9x])
JH_ADD_CFLAG([-fno-strict-aliasing])

AC_CONFIG_FILES(
  Makefile
  docs/Makefile
  docs/gnomevfs/Makefile
  gconf/Makefile
  gnome/Makefile
  gnomevfs/Makefile
  gnomecanvas/Makefile
  bonobo/Makefile
  tests/Makefile
  gnome-python.spec
  gnome-python-2.0.pc
  PKG-INFO)
AC_OUTPUT

echo
echo "The following modules will be built:"
echo
$build_gnome             && echo     gnome
$build_gnomeui           && $have_pyorbit && echo     gnome.ui
$build_gnomecanvas       && echo     gnomecanvas
$build_gnomevfs          && echo     gnomevfs
$build_gnomevfsbonobo    && echo     gnomevfs.bonobo
$build_pyvfsmodule       && echo     gnome-vfs python module support
$build_gconf             && echo     gconf
$build_bonobo            && $have_pyorbit && echo     bonobo
$build_bonobo_activation && $have_pyorbit && echo     bonobo.activation
$build_bonoboui          && $have_pyorbit && echo     bonobo.ui
echo

echo "The following modules will NOT be built:"
echo
$build_gnome             || echo     gnome
$build_gnomeui           && $have_pyorbit || echo     gnome.ui
$build_gnomecanvas       || echo     gnomecanvas
$build_gnomevfs          || echo     gnomevfs
$build_gnomevfsbonobo    || echo     gnomevfs.bonobo
$build_pyvfsmodule       || echo     gnome-vfs python module support
$build_gconf             || echo     gconf
$build_bonobo            && $have_pyorbit || echo     bonobo
$build_bonobo_activation && $have_pyorbit || echo     bonobo.activation
$build_bonoboui          && $have_pyorbit || echo     bonobo.ui

if $have_pyorbit; then
  :
else
  echo
  echo "Note: PyORBit pyorbit_required_version or later is required by bonobo"
  echo "and gnomeui bindings."
  echo "Download it from: http://ftp.gnome.org/pub/GNOME/sources/pyorbit/2.0/"
fi

echo "*****************************************************************************"
echo "*** WARNING: You are using a deprecated build system (autotools)."
echo "***          You should be using WAF instead; see the file INSTALL.WAF."
echo "***          The autotools build system will be removed in a future version."
echo "*****************************************************************************"
