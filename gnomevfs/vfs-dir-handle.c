/* -*- mode: C; c-basic-offset: 4 -*- */

#include "pygnomevfs-private.h"

PyObject *
pygnome_vfs_directory_handle_new(GnomeVFSDirectoryHandle *dir)
{
    PyGnomeVFSDirectoryHandle *self;

    self = PyObject_NEW(PyGnomeVFSDirectoryHandle, &PyGnomeVFSDirectoryHandle_Type);
    if (self == NULL) return NULL;

    self->dir = dir;

    return (PyObject *)self;
}

static void
pygvdir_dealloc(PyGnomeVFSDirectoryHandle *self)
{
    /* XXXX: unblock threads here */

    if (self->dir)
	gnome_vfs_directory_close(self->dir);
    PyObject_FREE(self);
}

static PyObject *
pygvdir_iter(PyGnomeVFSDirectoryHandle *self)
{
    Py_INCREF(self);
    return (PyObject *)self;
}

static PyObject *
pygvdir_iternext(PyGnomeVFSDirectoryHandle *self)
{
    GnomeVFSFileInfo *finfo;
    GnomeVFSResult result;

    /* XXXX: unblock threads here */

    finfo = gnome_vfs_file_info_new();
    result = gnome_vfs_directory_read_next(self->dir, finfo);

    if (result == GNOME_VFS_ERROR_EOF) {
	PyErr_SetNone(PyExc_StopIteration);
	gnome_vfs_file_info_unref(finfo);
	return NULL;
    }
    if (pygnome_vfs_result_check(result)) {
	gnome_vfs_file_info_unref(finfo);
	return NULL;
    }
    return pygnome_vfs_file_info_new(finfo);
}

static int
pygvdir_init(PyGnomeVFSDirectoryHandle *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "uri", "options", NULL };
    PyObject *uri;
    GnomeVFSDirectoryVisitOptions options = GNOME_VFS_DIRECTORY_VISIT_DEFAULT;
    GnomeVFSDirectoryHandle *handle = NULL;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "O|i:gnomevfs.DirectoryHandle.__init__",
				     kwlist, &uri, &options))
	return -1;

    /* XXXX: unblock threads here */

    if (PyObject_TypeCheck(uri, &PyGnomeVFSURI_Type))
	result = gnome_vfs_directory_open_from_uri(&handle,
						   pygnome_vfs_uri_get(uri),
						   options);
    else if (PyString_Check(uri))
	result = gnome_vfs_directory_open(&handle,
					  PyString_AsString(uri),
					  options);
    else {
	PyErr_SetString(PyExc_TypeError,
			"uri must be a gnomevfs.URI or a string");
	return -1;
    }

    if (pygnome_vfs_result_check(result))
	return -1;

    self->dir = handle;

    return 0;
}


static PyMethodDef pygvdir_methods[] = {
    { NULL, NULL, 0 }
};


PyTypeObject PyGnomeVFSDirectoryHandle_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "gnomevfs.DirectoryHandle",        /* tp_name */
    sizeof(PyGnomeVFSDirectoryHandle),  /* tp_basicsize */
    0,                                  /* tp_itemsize */
    /* methods */
    (destructor)pygvdir_dealloc,        /* tp_dealloc */
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
    (getiterfunc)pygvdir_iter,          /* tp_iter */
    (iternextfunc)pygvdir_iternext,     /* tp_iternext */
    pygvdir_methods,                    /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    (PyTypeObject *)0,                  /* tp_base */
    (PyObject *)0,                      /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc)pygvdir_init,             /* tp_init */
    PyType_GenericAlloc,                /* tp_alloc */
    PyType_GenericNew,                  /* tp_new */
    0,                                  /* tp_free */
    (inquiry)0,                         /* tp_is_gc */
    (PyObject *)0,                      /* tp_bases */
};
