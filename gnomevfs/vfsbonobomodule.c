/* -*- mode: C; c-basic-offset: 4 -*- */
#include <pygobject.h>
#include <libgnomevfs/gnome-vfs-utils.h>
#include <libgnomevfs/gnome-vfs-find-directory.h>
#include <libgnomevfs/gnome-vfs-address.h>
#include <libgnomevfs/gnome-vfs-resolve.h>
#include <libgnomevfs/gnome-vfs-dns-sd.h>
#include <libgnomevfs/gnome-vfs-async-ops.h>
#include <pyorbit.h>
#define NO_IMPORT_PYGNOMEVFSBONOBO
#include "pygnomevfsbonobo.h"

#include <bonobo-activation/Bonobo_Activation_types.h>


static PyObject *
pygvfs_mime_component_action_new(GnomeVFSMimeAction *action)
{
    CORBA_any any;
    PyObject *component;

    g_return_val_if_fail(action->action_type == GNOME_VFS_MIME_ACTION_TYPE_COMPONENT, NULL);

    any._type = TC_Bonobo_ServerInfo;
    any._value = action->action.application;
    component = pyorbit_demarshal_any(&any);
    if (!component) {
        PyErr_SetString(PyExc_TypeError, "unable to convert Bonobo_ServerInfo of component");
        return NULL;
    }
    return Py_BuildValue("(iN)", action->action_type, component);
}

static PyObject *
pygvfs_mime_get_default_component(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", NULL };
    char *mime_type;
    Bonobo_ServerInfo *component;
    PyObject *py_component;
    CORBA_any any;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s:gnomevfs.mime_get_default_component",
				     kwlist, &mime_type))
	return NULL;
    if (!(component = gnome_vfs_mime_get_default_component(mime_type))) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    any._type = TC_Bonobo_ServerInfo;
    any._value = component;
    py_component = pyorbit_demarshal_any(&any);
    if (!component) {
	PyErr_SetString(PyExc_TypeError, "unable to convert Bonobo_ServerInfo of component");
	return NULL;
    }
    CORBA_free(component);
    return py_component;
}

static PyObject *
pygvfs_mime_components_list_new(GList *list)
{
    PyObject *retval;
    Bonobo_ServerInfo *component;
    PyObject *py_component;
    CORBA_any any;
    guint i, len = g_list_length(list);
    
    retval = PyList_New(len);
    for (i = 0; list; ++i, list = list->next) {
	g_assert(i < len);
	component = (Bonobo_ServerInfo *) list->data;
	any._value = component;
	py_component = pyorbit_demarshal_any(&any);
	if (!component) {
	    PyErr_SetString(PyExc_TypeError, "unable to convert Bonobo_ServerInfo of component");
	    Py_DECREF(retval);
	    return NULL;
	}
	PyList_SET_ITEM(retval, i, py_component);
    }
    return retval;
}

static PyObject *
pygvfs_mime_get_short_list_components(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", NULL };
    char *mime_type;
    GList *list;
    PyObject *py_list;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s:gnomevfs.mime_get_short_list_components",
				     kwlist, &mime_type))
	return NULL;
    list = gnome_vfs_mime_get_short_list_components(mime_type);
    py_list = pygvfs_mime_components_list_new(list);
    gnome_vfs_mime_component_list_free(list);
    return py_list;
}

static PyObject *
pygvfs_mime_get_all_components(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", NULL };
    char *mime_type;
    GList *list;
    PyObject *py_list;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s:gnomevfs.mime_get_all_components",
				     kwlist, &mime_type))
	return NULL;
    list = gnome_vfs_mime_get_all_components(mime_type);
    py_list = pygvfs_mime_components_list_new(list);
    gnome_vfs_mime_component_list_free(list);
    return py_list;
}


static PyMethodDef pygnomevfs_bonobo_functions[] = {
    { "mime_get_default_component", (PyCFunction)pygvfs_mime_get_default_component,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_get_short_list_components", (PyCFunction)pygvfs_mime_get_short_list_components,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_get_all_components", (PyCFunction)pygvfs_mime_get_all_components,
      METH_VARARGS|METH_KEYWORDS },

    { NULL, NULL, 0 }
};


struct _PyGnomeVFSBonobo_Functions pygnomevfs_bonobo_api_functions = {
    pygvfs_mime_component_action_new,

};


DL_EXPORT(void)
initgnomevfsbonobo(void)
{
    PyObject *m, *d, *o;

    init_pygobject();
    init_pyorbit();
    if (!gnome_vfs_init()) {
	PyErr_SetString(PyExc_RuntimeError, "could not initialise gnomevfs");
	return;
    }

    m = Py_InitModule("gnomevfs.gnomevfsbonobo", pygnomevfs_bonobo_functions);
    d = PyModule_GetDict(m);

    PyDict_SetItemString(d, "_PyGnomeVFSBonobo_API",
			 o=PyCObject_FromVoidPtr(&pygnomevfs_bonobo_api_functions,NULL));
    Py_DECREF(o);
}

