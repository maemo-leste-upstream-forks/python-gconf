
INCLUDES = $(PYTHON_INCLUDES) $(PYGTK_CFLAGS) $(PYORBIT_CFLAGS) \
  $(BONOBO_CFLAGS) $(BONOBOUI_CFLAGS)

defsdir = $(datadir)/pygtk/2.0/defs
defs_DATA = \
  bonobo.defs \
  bonobo-types.defs \
  bonoboui.defs \
  bonoboui-types.defs

EXTRA_DIST = $(defs_DATA)

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

pybonobodir = $(pkgpythondir)/bonobo
pybonobo_PYTHON = __init__.py

pybonoboexecdir = $(pkgpyexecdir)/bonobo

common_ldflags = -module -avoid-version $(PYTHON_LDFLAGS)

CLEANFILES =

argtypesdir = $(datadir)/pygtk/2.0/argtypes
argtypes_PYTHON = bonobo-arg-types.py

pybonoboexec_LTLIBRARIES =

if BUILD_BONOBO_ACTIVATION
pybonoboexec_LTLIBRARIES += activation.la
endif
activation_la_CFLAGS = $(BONOBO_ACTIVATION_CFLAGS)
activation_la_LDFLAGS = $(common_ldflags) -export-symbols-regex initactivation
activation_la_LIBADD = $(BONOBO_ACTIVATION_LIBS) $(PYTHON_LIBS)
activation_la_SOURCES = activationmodule.c

if BUILD_BONOBO
pybonoboexec_LTLIBRARIES += _bonobo.la
endif
_bonobo_la_CFLAGS = $(BONOBO_CFLAGS)
_bonobo_la_LDFLAGS = $(common_ldflags) -export-symbols-regex init_bonobo
_bonobo_la_LIBADD = $(BONOBO_LIBS) $(PYTHON_LIBS)
_bonobo_la_SOURCES = bonobomodule.c bonobo-arg.c pybonobo.h pybonobo.c
nodist__bonobo_la_SOURCES = bonobo.c
CLEANFILES += bonobo.c
EXTRA_DIST += bonobo.override
bonobo.c: bonobo.defs bonobo-types.defs bonobo.override

if BUILD_BONOBOUI
pybonoboexec_LTLIBRARIES += ui.la
endif
ui_la_CFLAGS = $(BONOBOUI_CFLAGS)
ui_la_LDFLAGS = $(common_ldflags) -export-symbols-regex initui
ui_la_LIBADD = $(BONOBOUI_LIBS) $(PYTHON_LIBS)
ui_la_SOURCES = bonobouimodule.c
nodist_ui_la_SOURCES = bonoboui.c
CLEANFILES += bonoboui.c
EXTRA_DIST += bonoboui.override
bonoboui.c: bonoboui.defs bonoboui-types.defs bonobo-types.defs bonoboui.override

.defs.c:
	(cd $(srcdir) \
	 && $(PYGNOME_CODEGEN) \
	    --py_ssize_t-clean \
	    --load-types bonobo-arg-types.py \
	    --register $(PYGTK_DEFSDIR)/pango-types.defs \
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --register ../gnomecanvas/canvas.defs \
	    --register ../gnome/gnome-types.defs \
	    --register bonobo-types.defs \
	    --override $*.override \
	    --prefix py$* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c

EXTRA_DIST += wscript
