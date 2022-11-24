AUTOMAKE_OPTIONS = 1.6

SUBDIRS = 

if BUILD_DOCS
SUBDIRS += docs
endif

if BUILD_GNOME
SUBDIRS += gnome
endif

if BUILD_BONOBO_ANY
SUBDIRS += bonobo
endif

if BUILD_GCONF
SUBDIRS += gconf
endif

if BUILD_GNOMEVFS
SUBDIRS += gnomevfs
endif

if BUILD_GNOMECANVAS
SUBDIRS += gnomecanvas
endif

SUBDIRS += tests



pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = gnome-python-2.0.pc

EXTRA_DIST = \
  INSTALL.WAF \
  waf \
  wscript \
  gnome-python.spec \
  gnome-python.spec.in \
  gnome-python-2.0.pc.in \
  PKG-INFO \
  PKG-INFO.in \
  ChangeLog.pre-2-0 \
  setup.py \
  examples/bug-buddy-integration.py \
  examples/bonobo/bonoboui/Bonobo_Sample_Hello.xml \
  examples/bonobo/bonoboui/hello.py \
  examples/bonobo/echo/README \
  examples/bonobo/echo/echo-client.py \
  examples/bonobo/unknown.py \
  examples/bonobo/bonobo-application.py \
  examples/canvas/canvas-example.py \
  examples/canvas/canvas-curve.py \
  examples/gconf/basic-gconf-app.py \
  examples/gconf/simple-controller.py \
  examples/gconf/simple-view.py \
  examples/vfs/shell.py \
  examples/vfs/sync-xfer.py \
  examples/vfs/async-xfer.py \
  examples/vfs/async.py \
  examples/vfs/cancellation.py \
  examples/vfs/pygvfsmethod/README \
  examples/vfs/pygvfsmethod/pyfs.conf \
  examples/vfs/pygvfsmethod/pyfs.py \
  examples/popt/popt.py \
  examples/popt/goption.py


$(srcdir)/ChangeLog:
	-rm -f $(srcdir)/ChangeLog
	$(srcdir)/generate-ChangeLog

dist-hook: dist-hook-local $(srcdir)/ChangeLog

.PHONY : dist-hook-local
