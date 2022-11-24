/* -*- mode: C; c-basic-offset: 4 -*- */

#include "pygnomevfs-private.h"
#include <libgnomevfs/gnome-vfs-utils.h>

PyObject *
pygnome_vfs_context_new(GnomeVFSContext *context)
{
    PyGnomeVFSContext *self;

    self = PyObject_NEW(PyGnomeVFSContext, &PyGnomeVFSContext_Type);
    if (self == NULL)
	    return NULL;

    self->context = context;

    return (PyObject *)self;
}

static void
pygvcontext_dealloc(PyGnomeVFSContext *self)
{
    if (self->context) {
	    gnome_vfs_context_free(self->context);
    }
    
    PyObject_FREE(self);
}

static int
pygvcontext_init(PyGnomeVFSContext *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     ":gnomevfs.Context.__init__",
				     kwlist))
	return -1;

    /* XXX: unblock threads here */

    self->context = gnome_vfs_context_new();
    if (!self->context) {
        PyErr_SetString(PyExc_TypeError, "could not create Context object");
        return -1;
    }
    return 0;
}

static PyObject *
pygvcontext_get_cancellation(PyGnomeVFSContext *self)
{
	GnomeVFSCancellation *cancel;

	cancel = gnome_vfs_context_get_cancellation (self->context);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
pygvcontext_check_cancellation(PyGnomeVFSContext *self)
{
    gboolean retval;
    PyObject *py_retval;
    GnomeVFSCancellation *cancel;

    cancel = gnome_vfs_context_get_cancellation (self->context);
    retval = gnome_vfs_cancellation_check(cancel);

    py_retval = retval ? Py_True : Py_False;
    Py_INCREF(py_retval);
    return py_retval;
}

static PyObject *
pygvcontext_cancel(PyGnomeVFSContext *self)
{
    GnomeVFSCancellation *cancel;

    if (!gnome_vfs_is_primary_thread()) {
	PyErr_SetString(PyExc_RuntimeError,
			"cancel can only be called from the main thread");
	return NULL;
    }

    cancel = gnome_vfs_context_get_cancellation (self->context);
    gnome_vfs_cancellation_cancel(cancel);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef pygvcontext_methods[] = {
    { "get_cancellation",
      (PyCFunction)pygvcontext_get_cancellation, METH_NOARGS },
    { "check_cancellation",
      (PyCFunction)pygvcontext_check_cancellation, METH_NOARGS },
    { "cancel",
      (PyCFunction)pygvcontext_cancel, METH_NOARGS },
    { NULL, NULL, 0 }
};

PyTypeObject PyGnomeVFSContext_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "gnomevfs.Context",                 /* tp_name */
    sizeof(PyGnomeVFSContext),          /* tp_basicsize */
    0,                                  /* tp_itemsize */
    /* methods */
    (destructor)pygvcontext_dealloc,     /* tp_dealloc */
    (printfunc)0,                       /* tp_print */
    (getattrfunc)0,                     /* tp_getattr */
    (setattrfunc)0,                     /* tp_setattr */
    (cmpfunc)0,                         /* tp_compare */
    (reprfunc)0,                        /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    (hashfunc)0,                        /* tp_hash */
    (ternaryfunc)0,                     /* tp_call */
    (reprfunc)0,                        /* tp_str */
    (getattrofunc)0,                    /* tp_getattro */
    (setattrofunc)0,                    /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,   		/* tp_flags */
    NULL, /* Documentation string */
    (traverseproc)0,                    /* tp_traverse */
    (inquiry)0,                         /* tp_clear */
    (richcmpfunc)0,                     /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    (getiterfunc)0,                     /* tp_iter */
    (iternextfunc)0,                    /* tp_iternext */
    pygvcontext_methods,                 /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    (PyTypeObject *)0,                  /* tp_base */
    (PyObject *)0,                      /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc)pygvcontext_init,         /* tp_init */
    PyType_GenericAlloc,                /* tp_alloc */
    PyType_GenericNew,                  /* tp_new */
    0,                                  /* tp_free */
    (inquiry)0,                         /* tp_is_gc */
    (PyObject *)0,                      /* tp_bases */
};
