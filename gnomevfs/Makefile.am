INCLUDES = $(PYTHON_INCLUDES) $(PYGTK_CFLAGS) -DPY_SSIZE_T_CLEAN


EXTRA_DIST =

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0
gnomevfsdir = $(pkgpyexecdir)/gnomevfs
gnomevfs_PYTHON = __init__.py

common_ldflags = -module -avoid-version $(PYTHON_LDFLAGS)

# libgnomevfs binding
gnomevfs_LTLIBRARIES = _gnomevfs.la
_gnomevfs_la_CFLAGS = $(GNOMEVFS_CFLAGS)
_gnomevfs_la_LDFLAGS = $(common_ldflags) -export-symbols-regex init_gnomevfs
_gnomevfs_la_LIBADD = $(GNOMEVFS_LIBS) $(PYTHON_LIBS)
_gnomevfs_la_SOURCES = vfsmodule.c vfs-uri.c vfs-file-info.c vfs-dir-handle.c \
 vfs-handle.c pygnomevfs.h pygnomevfs-private.h vfs-xfer-progress-info.c \
 vfs-context.c vfs-async-handle.c vfs-volume.c vfs-drive.c vfs-volume-monitor.c

# libgnomevfs binding
if BUILD_GNOMEVFSBONOBO
gnomevfs_LTLIBRARIES += gnomevfsbonobo.la
endif
gnomevfsbonobo_la_CFLAGS = $(GNOMEVFSBONOBO_CFLAGS)  -DGNOME_VFS_INCLUDE_BONOBO
gnomevfsbonobo_la_LDFLAGS = $(common_ldflags) -export-symbols-regex initgnomevfsbonobo
gnomevfsbonobo_la_LIBADD = $(GNOMEVFSBONOBO_LIBS) $(PYTHON_LIBS)
gnomevfsbonobo_la_SOURCES = vfsbonobomodule.c pygnomevfsbonobo.h


gpincdir = $(includedir)/gnome-python-2.0
gpinc_HEADERS = pygnomevfs.h pygnomevfsbonobo.h

## Python GnomeVFS modules
vfsmethoddir = $(libdir)/gnome-vfs-2.0/modules
if BUILD_PYVFSMODULE
  vfsmethod_LTLIBRARIES = libpythonmethod.la
else
  vfsmethod_LTLIBRARIES =
endif

libpythonmethod_la_CFLAGS = $(GNOME_VFS_MODULE_CFLAGS) -DGNOME_VFS_PYTHON_DIR="\"$(vfsmethoddir)\""
libpythonmethod_la_SOURCES = gnome-vfs-python-method.c
libpythonmethod_la_LDFLAGS = $(common_ldflags) -export-symbols-regex "vfs_module_(init|shutdown)"
libpythonmethod_la_LIBADD = $(GNOME_VFS_MODULE_LIBS) $(PYTHON_EMBED_LIBS)

EXTRA_DIST += wscript
