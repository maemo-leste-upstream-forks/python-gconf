/* -*- Mode: C; c-basic-offset: 4 -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <pygobject.h>
#include "pygnomevfs.h"

#include <libgnomeui/libgnomeui.h>

void pyui_register_classes (PyObject *d);
void pyui_add_constants(PyObject *module, const gchar *strip_prefix);
extern PyMethodDef pyui_functions[];

DL_EXPORT(void)
initui (void)
{
    PyObject *m, *d;
	
    init_pygobject ();
    init_pygnomevfs ();

    m = Py_InitModule ("ui", pyui_functions);
    d = PyModule_GetDict (m);
	
    pyui_register_classes (d);
    pyui_add_constants (m, "GNOME_");
    
    PyDict_SetItemString(d, "PAD", PyInt_FromLong(GNOME_PAD));
    PyDict_SetItemString(d, "PAD_SMALL", PyInt_FromLong(GNOME_PAD_SMALL));
    PyDict_SetItemString(d, "PAD_BIG", PyInt_FromLong(GNOME_PAD_BIG));
        
    if (!gnome_program_module_registered(LIBGNOMEUI_MODULE))
        gnome_program_module_register(LIBGNOMEUI_MODULE);
}
