/* -*- Mode: C; c-basic-offset: 4 -*- */
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include <Python.h>
#include <pygobject.h>
#include <pygtk/pygtk.h>
#include <bonobo/bonobo-ui-main.h>

/* include any extra headers needed here */

void pybonoboui_register_classes(PyObject *d);
void pybonoboui_add_constants(PyObject *module, const gchar *strip_prefix);
extern PyMethodDef pybonoboui_functions[];

DL_EXPORT(void)
initui(void)
{
    PyObject *m, *d;

    init_pygobject();

    m = Py_InitModule("bonobo.ui", pybonoboui_functions);
    d = PyModule_GetDict(m);

    init_pygtk();

    /* we don't call bonobo_ui_init() here, as all it does is call
     * bonobo_init() (done by the bonobo module we import),
     * gnome_program_init() (meant to be done by the user), and
     * gtk_init() (done by the gtk module we import), and call
     * bonobo_setup_x_error_handler().  This last call is all that is
     * left. */
    bonobo_setup_x_error_handler();


    pybonoboui_register_classes(d);

    /* add anything else to the module dictionary (such as constants) */
    pybonoboui_add_constants(m, "BONOBO_");
}
