INCLUDES = $(PYTHON_INCLUDES) $(PYGTK_CFLAGS)

defsdir = $(datadir)/pygtk/2.0/defs
defs_DATA = canvas.defs

EXTRA_DIST = $(defs_DATA)

pkgpythondir = $(pyexecdir)/gtk-2.0
pkgpyexecdir = $(pyexecdir)/gtk-2.0

common_ldflags = -module -avoid-version $(PYTHON_LDFLAGS)

# libgnomecanvas binding
pkgpyexec_LTLIBRARIES = gnomecanvas.la
gnomecanvas_la_CFLAGS = $(GNOMECANVAS_CFLAGS)
gnomecanvas_la_LDFLAGS = $(common_ldflags) -export-symbols-regex initgnomecanvas
gnomecanvas_la_LIBADD = $(GNOMECANVAS_LIBS) $(PYTHON_LIBS)
gnomecanvas_la_SOURCES = canvasmodule.c
nodist_gnomecanvas_la_SOURCES = canvas.c
CLEANFILES = canvas.c
EXTRA_DIST += canvas.override
canvas.c: canvas.defs canvas.override


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

EXTRA_DIST += wscript
