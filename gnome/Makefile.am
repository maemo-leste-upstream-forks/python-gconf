INCLUDES = $(PYTHON_INCLUDES) $(PYGTK_CFLAGS)

defsdir = $(datadir)/pygtk/2.0/defs
defs_DATA = \
  gnome.defs \
  gnome-types.defs \
  ui.defs

EXTRA_DIST = $(defs_DATA)

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

common_ldflags = -module -avoid-version $(PYTHON_LDFLAGS)

pygnomedir = $(pkgpythondir)/gnome
pygnome_PYTHON = __init__.py canvas.py vfs.py

pygnomeexecdir = $(pkgpyexecdir)/gnome
pygnomeexec_LTLIBRARIES =

# libgnome binding

pygnomeexec_LTLIBRARIES += _gnome.la
_gnome_la_CFLAGS = $(GNOME_CFLAGS)
_gnome_la_LDFLAGS = $(common_ldflags) -export-symbols-regex init_gnome
_gnome_la_LIBADD = $(GNOME_LIBS) $(PYTHON_LIBS)
_gnome_la_SOURCES = gnomemodule.c
nodist__gnome_la_SOURCES = gnome.c
CLEANFILES = gnome.c
EXTRA_DIST += gnome.override  
gnome.c: gnome.defs gnome.override

# libgnomeui binding

if BUILD_GNOMEUI
pygnomeexec_LTLIBRARIES += ui.la
endif
ui_la_CFLAGS = $(GNOMEUI_CFLAGS) -I$(top_srcdir)/gnomevfs
ui_la_LDFLAGS = $(common_ldflags) -export-symbols-regex initui
ui_la_LIBADD = $(GNOMEUI_LIBS) $(PYTHON_LIBS)
ui_la_SOURCES = uimodule.c
nodist_ui_la_SOURCES = ui.c
CLEANFILES += ui.c
EXTRA_DIST += ui.override  
ui.c: ui.defs ui.override


.defs.c:
	(cd $(srcdir) \
	 && $(PYGNOME_CODEGEN) \
	    --py_ssize_t-clean \
	    --load-types ../bonobo/bonobo-arg-types.py \
	    --register $(PYGTK_DEFSDIR)/pango-types.defs \
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --register ../bonobo/bonobo-types.defs \
	    --register ../bonobo/bonoboui-types.defs \
	    --override $*.override \
	    --prefix py$* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c


install-exec-hook: # Remove old deprecated modules
	-$(LIBTOOL) --mode=uninstall rm -f $(pygnomeexecdir)/canvas.la
	-$(LIBTOOL) --mode=uninstall rm -f $(pygnomeexecdir)/vfs.la

EXTRA_DIST += wscript

