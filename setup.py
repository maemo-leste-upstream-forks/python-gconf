#!/usr/bin/env python
#
# setup.py - distutils configuration for pygtk
#
# TODO:
# pygtk.spec(.in)
# win32 testing
# install *.pyc for codegen
#
"""Python Bindings for Gnome.

TODO: Write a good long description"""

from distutils.command.build import build
from distutils.core import setup
import os
import sys

sys.stderr.write('''-- WARNING --
The distutils method (setup.py) of building gnome-python is not
supported.  setup.py is *unmaintained* and therefore not up-to-date
with the latest developments.  Use at your own risk.
-- 
''')

try:
    import pygtk
    pygtk.require('2.0')
except (ImportError, AssertionError):
    raise SystemExit, 'ERROR: Could not find a recent version of pygtk.'

from dsextras import list_files, have_pkgconfig, GLOBAL_INC, GLOBAL_MACROS
from dsextras import InstallLib, BuildExt, PkgConfigExtension
from dsextras import Template, TemplateExtension, pkgc_version_check
from dsextras import getoutput

MAJOR_VERSION = 2
MINOR_VERSION = 5
MICRO_VERSION = 91

VERSION = "%d.%d.%d" % (MAJOR_VERSION,
                        MINOR_VERSION,
                        MICRO_VERSION)

PYGTK_REQUIRED_VERSION              = '2.0.0'
PYORBIT_REQUIRED_VERSION            = '2.0.0'
LIBGNOME_REQUIRED_VERSION           = '2.0.0'
LIBGNOMEUI_REQUIRED_VERSION         = '2.0.0'
LIBGNOMECANVAS_REQUIRED_VERSION     = '2.0.0'
LIBGNOMEVFS_REQUIRED_VERSION        = '2.0.0'
LIBZVT_REQUIRED_VERSION             = '2.0.0'
GCONF_REQUIRED_VERSION              = '1.2.0'
BONOBO_ACTIVATION_REQUIRED_VERSION  = '1.0.0'
LIBBONOBO_REQUIRED_VERSION          = '2.0.0'
LIBBONOBOUI_REQUIRED_VERSION        = '2.0.0'
LIBNAUTILUS_REQUIRED_VERSION        = '2.0.0'
LIBPANELAPPLET_REQUIRED_VERSION     = '2.0.0'
GTKHTML2_REQUIRED_VERSION           = '2.0.0'
LIBGNOMEPRINT_REQUIRED_VERSION      = '2.2.0'
LIBGNOMEPRINTUI_REQUIRED_VERSION    = '2.2.0'

PYGTK_SUFFIX = '2.0'
PYGTK_SUFFIX_LONG = 'gtk-' + PYGTK_SUFFIX

GLOBAL_INC += ['gconf', 'gnome', 'gnomeprint']

DEFS_DIR    = os.path.join('share', 'pygtk', PYGTK_SUFFIX, 'defs')
CODEGEN_DIR = os.path.join('share', 'pygtk', PYGTK_SUFFIX, 'codegen')
INCLUDE_DIR = os.path.join('include', 'pygtk-%s' % PYGTK_SUFFIX)

str_version = sys.version[:3]
version = map(int, str_version.split('.'))
if version < [2, 2]:
    raise SystemExit, \
          "Python 2.2 or higher is required, %s found" % str_version

install_vfsmethod = 0
class GnomePythonInstallLib(InstallLib):
    def run(self):
        self.add_template_option('VERSION', VERSION)
        self.prepare()

        self.install_template('gnome-python-2.0.pc.in',
                              os.path.join(self.libdir, 'pkgconfig'))

        if install_vfsmethod:
            self.install_vfsmethod()
            
        # Modify the base installation dir
        install_dir = os.path.join(self.install_dir, PYGTK_SUFFIX_LONG)
        self.set_install_dir(install_dir)
                                          
        InstallLib.run(self)
        
    def install_vfsmethod(self):
        libdir = '/'.join(self.install_dir.split('/')[:-3])
        module_dir = os.path.join(libdir, 'gnome-vfs-2.0', 'modules')
        module_file = os.path.join(self.build_dir, 'libpythonmethod.so')
        if (os.path.exists(module_file) and
            not os.path.exists(os.path.join(self.build_dir, module_file))):
            self.copy_file(module_file, module_dir)
        
if not pkgc_version_check('pygtk-2.0', 'PyGTK', PYGTK_REQUIRED_VERSION):
    raise SystemExit, "Aborting"
pygtkincludedir = getoutput('pkg-config --variable pygtkincludedir pygtk-2.0')
codegendir = getoutput('pkg-config --variable codegendir pygtk-2.0')
defsdir = getoutput('pkg-config --variable defsdir pygtk-2.0')

GLOBAL_INC.append(pygtkincludedir)
GTKDEFS = [os.path.join(defsdir, 'pango-types.defs'),
           os.path.join(defsdir, 'gdk-types.defs'),
           os.path.join(defsdir, 'gtk-types.defs')]
BONOBODEFS = [os.path.join('bonobo', 'bonobo-types.defs'),
              os.path.join('bonobo', 'bonoboui-types.defs')]
CANVASDEFS = [os.path.join('gnome', 'canvas.defs')]
sys.path.append(codegendir)
try:
    from override import Overrides
except ImportError:
    raise SystemExit, \
'Could not find code generator in %s, do you have installed pygtk correctly?'

have_pyorbit = pkgc_version_check('pyorbit-2', 'PyORBit.',
                                 PYORBIT_REQUIRED_VERSION)
if have_pyorbit:
    includedir = getoutput('pkg-config --variable includedir pyorbit-2')
    GLOBAL_INC.append(os.path.join(includedir, 'pyorbit-2')) 

libgnome = TemplateExtension(name='gnome', pkc_name='libgnome-2.0',
                             pkc_version=LIBGNOME_REQUIRED_VERSION,
                             output='gnome._gnome',
                             defs='gnome/gnome.defs',
                             sources=['gnome/gnomemodule.c',
                                      'gnome/gnome.c'],
                             register=GTKDEFS+['gnome/gnome.defs'],
                             override='gnome/gnome.override')

libgnomeui = TemplateExtension(name='ui', pkc_name='libgnomeui-2.0',
                               pkc_version=LIBGNOMEUI_REQUIRED_VERSION,
                               output='gnome.ui',
                               defs='gnome/ui.defs',
                               sources=['gnome/uimodule.c',
                                        'gnome/ui.c'],
                               register=GTKDEFS+BONOBODEFS + \
                                        ['gnome/gnome.defs',
                                         'gnome/ui.defs'],
                               override='gnome/ui.override')

libgnomecanvas = TemplateExtension(name='canvas',
                                   pkc_name='libgnomecanvas-2.0',
                                   pkc_version=LIBGNOMECANVAS_REQUIRED_VERSION,
                                   output='gnome.canvas',
                                   defs='gnome/canvas.defs',
                                   sources=['gnome/canvasmodule.c',
                                            'gnome/canvas.c'],
                                   register=['gnome/canvas.defs'],
                                   override='gnome/canvas.override')

libgnomevfs = PkgConfigExtension(name='gnome.vfs',
                                 pkc_name='gnome-vfs-2.0',
                                 pkc_version=LIBGNOMEVFS_REQUIRED_VERSION,
                                 sources=['gnome/vfs-dir-handle.c',
                                          'gnome/vfs-file-info.c',
                                          'gnome/vfs-handle.c',
                                          'gnome/vfsmodule.c',
                                          'gnome/vfs-uri.c',
                                          'gnome/vfs-context.c'])

libzvt = TemplateExtension(name='zvt',
                           pkc_name='libzvt-2.0',
                           pkc_version=LIBZVT_REQUIRED_VERSION,
                           output='gnome.zvt',
                           defs='gnome/zvt.defs',
                           sources=['gnome/zvtmodule.c',
                                    'gnome/zvt.c'],
                           register=['gnome/zvt.defs'],
                           override='gnome/zvt.override')

gconf = TemplateExtension(name='gconf',
                          pkc_name='gconf-2.0',
                          pkc_version=GCONF_REQUIRED_VERSION,
                          output='gnome.gconf',
                          defs='gconf/gconf.defs',
                          sources=['gconf/gconfmodule.c',
                                   'gconf/gconf-fixes.c',
                                   'gconf/gconf.c'],
                          register=['gconf/gconf.defs'],
                          override='gconf/gconf.override')

bonobo_activation = PkgConfigExtension(name='bonobo.activation',
                           pkc_name='bonobo-activation-2.0',
                           pkc_version=BONOBO_ACTIVATION_REQUIRED_VERSION,
                           sources=['bonobo/activationmodule.c'])

libbonobo = TemplateExtension(name='bonobo',
                              pkc_name='libbonobo-2.0',
                              pkc_version=LIBBONOBO_REQUIRED_VERSION,
                              output='bonobo._bonobo',
                              defs='bonobo/bonobo.defs',
                              sources=['bonobo/bonobomodule.c',
                                       'bonobo/bonobo-arg.c',
                                       'bonobo/bonobo.c'],
                              register=BONOBODEFS + \
                                       ['bonobo/bonoboui.defs'],
                              load_types='bonobo/bonobo-arg-types.py',
                              override='bonobo/bonobo.override')

libbonoboui = TemplateExtension(name='bonoboui',
                                pkc_name='libbonoboui-2.0',
                                pkc_version=LIBBONOBOUI_REQUIRED_VERSION,
                                output='bonobo.ui',
                                defs='bonobo/bonoboui.defs',
                                sources=['bonobo/bonobouimodule.c',
                                         'bonobo/bonoboui.c'],
                                register=GTKDEFS + CANVASDEFS + \
                                         BONOBODEFS + \
                                         ['bonobo/bonoboui.defs'],
                                load_types='bonobo/bonobo-arg-types.py',
                                override='bonobo/bonoboui.override')

libnautilus = TemplateExtension(name='nautilus',
                                pkc_name='libnautilus',
                                pkc_version=LIBNAUTILUS_REQUIRED_VERSION,
                                output='gnome.nautilus',
                                defs='gnome/nautilus.defs',
                                sources=['gnome/nautilusmodule.c',
                                         'gnome/nautilus.c'],
                                register=['gnome/nautilus.defs'],
                                override='gnome/nautilus.override')

libpanelapplet = TemplateExtension(name='applet',
                                pkc_name='libpanelapplet-2.0',
                                pkc_version=LIBPANELAPPLET_REQUIRED_VERSION,
                                output='gnome.applet',
                                defs='gnome/applet.defs',
                                sources=['gnome/appletmodule.c',
                                         'gnome/applet.c'],
                                register=['gnome/applet.defs'],
                                override='gnome/applet.override')

gtkhtml2 = TemplateExtension(name='gtkhtml2',
                           pkc_name='libgtkhtml-2.0',
                           pkc_version=GTKHTML2_REQUIRED_VERSION,
                           output='gtkhtml2',
                           defs='gtkhtml2/gtkhtml2.defs',
                           sources=['gtkhtml2/gtkhtml2module.c',
                                    'gtkhtml2/gtkhtml2.c'],
                           register=['gtkhtml2/gtkhtml2.defs'],
                           override='gtkhtml2/gtkhtml2.override')

libgnomeprint = TemplateExtension(name='print',
                           pkc_name='libgnomeprint-2.2',
                           pkc_version=LIBGNOMEPRINT_REQUIRED_VERSION,
                           output='gnomeprint._print',
                           defs='gnomeprint/print.defs',
                           sources=['gnomeprint/printmodule.c',
                                    'gnomeprint/print.c'],
                           register=GTKDEFS + ['gnomeprint/print.defs'],
                           override='gnomeprint/print.override')

libgnomeprintui = TemplateExtension(name='printui',
                           pkc_name='libgnomeprintui-2.2',
                           pkc_version=LIBGNOMEPRINTUI_REQUIRED_VERSION,
                           output='gnomeprint.printui',
                           defs='gnomeprint/printui.defs',
                           sources=['gnomeprint/printuimodule.c',
                                    'gnomeprint/printui.c'],
                           register=GTKDEFS + CANVASDEFS + \
                                    ['gnomeprint/print.defs',
                                     'gnomeprint/printui.defs'],
                           override='gnomeprint/printui.override')

data_files = []
ext_modules = []
py_modules = []
if libgnome.can_build() or libgnomecanvas.can_build():
    py_modules.append('gnome.__init__')    
if libgnome.can_build():
    ext_modules.append(libgnome)
    data_files.append((DEFS_DIR, ('gnome/gnome.defs',)))
if have_pyorbit and libgnomeui.can_build():
    ext_modules.append(libgnomeui)
    data_files.append((DEFS_DIR, ('gnome/ui.defs',)))    
if libgnomecanvas.can_build():
    ext_modules.append(libgnomecanvas)
    data_files.append((DEFS_DIR, ('gnome/canvas.defs',)))    
if libgnomevfs.can_build():
    #libdir = getoutput('pkg-config --variable=libdir gnome-vfs-module-2.0')
    #pythondir = os.path.join(libdir, 'gnome-vfs-2.0', 'modules', 'python')
    #GLOBAL_MACROS.append(('GNOME_VFS_PYTHON_DIR', '"%s"' % pythondir))
    #libpythonmethod = PkgConfigExtension(name='libpythonmethod',
    #                   pkc_name='gnome-vfs-module-2.0',
    #                   pkc_version=LIBGNOMEVFS_REQUIRED_VERSION,
    #                   sources=['gnome/gnome-vfs-python-method.c'])
    #if libpythonmethod.can_build():
    #    install_vfsmethod = 1
    #    ext_modules.append(libpythonmethod)
        
    ext_modules.append(libgnomevfs)
if libzvt.can_build():
    ext_modules.append(libzvt)
    data_files.append((DEFS_DIR, ('gnome/zvt.defs',)))
if gconf.can_build():
    ext_modules.append(gconf)
    data_files.append((DEFS_DIR, ('gconf/gconf.defs',)))
if have_pyorbit and bonobo_activation.can_build():
    ext_modules.append(bonobo_activation)
if have_pyorbit and libbonobo.can_build():
    ext_modules.append(libbonobo)
    data_files.append((DEFS_DIR, ('bonobo/bonobo.defs',
                                  'bonobo/bonobo-types.defs')))
if have_pyorbit and libbonoboui.can_build():
    ext_modules.append(libbonoboui)
    data_files.append((DEFS_DIR, ('bonobo/bonoboui.defs',
                                  'bonobo/bonoboui-types.defs')))
    if libnautilus.can_build():
        ext_modules.append(libnautilus)
        data_files.append((DEFS_DIR, ('gnome/nautilus.defs',)))
if libpanelapplet.can_build():
    ext_modules.append(libpanelapplet)
    data_files.append((DEFS_DIR, ('gnome/applet.defs',)))
if gtkhtml2.can_build():
    ext_modules.append(gtkhtml2)
    data_files.append((DEFS_DIR, ('gtkhtml2/gtkhtml2.defs',)))
if libgnomeprint.can_build():
    ext_modules.append(libgnomeprint)
    data_files.append((DEFS_DIR, ('gnomeprint/print.defs',)))
    py_modules.append('gnomeprint.__init__')
if libgnomeprintui.can_build():
    ext_modules.append(libgnomeprintui)
    data_files.append((DEFS_DIR, ('gnomeprint/printui.defs',)))

# Install bonobo/__init__.py
if have_pyorbit and (bonobo_activation.can_build() or
                     libbonobo.can_build() or
                     libbonoboui.can_build()):
    py_modules.append('bonobo.__init__')

doclines = __doc__.split("\n")

setup(name="gnome-python",
      url='http://www.daa.com.au/~james/gnome/',
      version=VERSION,
      license='LGPL',
      platforms=['yes'],
      maintainer="James Henstridge",
      maintainer_email="james@daa.com.au",
      description = doclines[0],
      long_description = "\n".join(doclines[2:]),
      py_modules=py_modules,
      ext_modules=ext_modules,
      data_files=data_files,
      cmdclass={'install_lib': GnomePythonInstallLib,
                'build_ext': BuildExt })
