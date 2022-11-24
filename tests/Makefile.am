tests =  \
	testvfs.py testbonobo.py testgnome.py

PYTHONPATH = $(top_builddir):$(top_builddir)/gnomecanvas:$(top_builddir):$(top_builddir)/bonobo:$(top_builddir)/gconf:$(shell echo $$PYTHONPATH)

check-local:
	@if ! test -e $(top_builddir)/gnome/__init__.py; then cp $(top_srcdir)/gnome/__init__.py $(top_builddir)/gnome/__init__.py; fi
	@if ! test -e $(top_builddir)/gnomevfs/__init__.py; then cp $(top_srcdir)/gnomevfs/__init__.py $(top_builddir)/gnomevfs/__init__.py; fi
	@if ! test -e $(top_builddir)/bonobo/__init__.py; then cp $(top_srcdir)/bonobo/__init__.py $(top_builddir)/bonobo/__init__.py; fi
	@PYTHONPATH=$(PYTHONPATH) DIST_CHECK=1 $(PYTHON) -W error $(srcdir)/runtests.py

	@if test "$(top_builddir)" != "$(top_srcdir)"; then\
		rm -f $(top_builddir)/gnome/__init__.py*;\
		rm -f $(top_builddir)/gnomevfs/__init__.py*;\
		rm -f $(top_builddir)/bonobo/__init__.py*;\
	fi
	@rm -fr *.pyc 

EXTRA_DIST = $(tests) common.py runtests.py ltihooks.py
