/* -*- mode: C; c-basic-offset: 4 -*- */

#include "pygnomevfs-private.h"

PyObject *
pygnome_vfs_uri_new(GnomeVFSURI *uri)
{
    PyGnomeVFSURI *self;

    self = PyObject_NEW(PyGnomeVFSURI, &PyGnomeVFSURI_Type);
    if (self == NULL) return NULL;

    self->uri = uri;

    return (PyObject *)self;
}

static void
pygvuri_dealloc(PyGnomeVFSURI *self)
{
    if (self->uri)
	gnome_vfs_uri_unref(self->uri);
    PyObject_FREE(self);
}

static int
pygvuri_compare(PyGnomeVFSURI *self, PyGnomeVFSURI *v)
{
    if (gnome_vfs_uri_equal(self->uri, v->uri)) return 0;
    if (self->uri > v->uri) return -1;
    return 1;
}

static PyObject *
pygvuri_repr(PyGnomeVFSURI *self)
{
    PyObject *ret;
    gchar *uri;

    uri = gnome_vfs_uri_to_string(self->uri, GNOME_VFS_URI_HIDE_NONE);
    if (uri) {
	ret = PyString_FromFormat("<gnomevfs.URI '%s'>", uri);
	g_free(uri);
    } else {
	ret = PyString_FromString("<gnomevfs.URI (null)>");
    }
    return ret;
}

static long
pygvuri_hash(PyGnomeVFSURI *self)
{
    return (long)gnome_vfs_uri_hash(self->uri);
}

static PyObject *
pygvuri_str(PyGnomeVFSURI *self)
{
    PyObject *ret;
    gchar *uri;

    uri = gnome_vfs_uri_to_string(self->uri, GNOME_VFS_URI_HIDE_NONE);
    if (!uri) {
	PyErr_SetString(PyExc_ValueError, "could not convert uri to string");
	return NULL;
    }
    ret = PyString_FromString(uri);
    g_free(uri);
    return ret;
}

static PyObject *
pygvuri_getattr(PyGnomeVFSURI *self, const gchar *attr)
{
    GnomeVFSURI *uri;

    uri = self->uri;
    if (!strcmp(attr, "__members__")) {
	return Py_BuildValue("[ssssssssssss]", "dirname",
			     "fragment_identifier", "host_name", "host_port",
			     "is_local", "parent", "password", "path",
			     "scheme", "short_name", "short_path_name",
			     "toplevel", "user_name");
    } else if (!strcmp(attr, "is_local")) {
	return PyInt_FromLong(gnome_vfs_uri_is_local(uri));
    } else if (!strcmp(attr, "parent")) {
	GnomeVFSURI *parent;

	parent = gnome_vfs_uri_get_parent(uri);
	if (parent) {
	    return pygnome_vfs_uri_new(parent);
	}
	Py_INCREF(Py_None);
	return Py_None;
    } else if (!strcmp(attr, "toplevel")) {
	GnomeVFSToplevelURI *toplevel;

	toplevel = gnome_vfs_uri_get_toplevel(uri);
	if (toplevel) {
	    return pygnome_vfs_uri_new(gnome_vfs_uri_ref((GnomeVFSURI *)toplevel));
	}
	Py_INCREF(Py_None);
	return Py_None;
    } else if (!strcmp(attr, "host_name")) {
	const gchar *ret = gnome_vfs_uri_get_host_name(uri);

	if (ret)
	    return PyString_FromString(ret);
	Py_INCREF(Py_None);
	return Py_None;
    } else if (!strcmp(attr, "scheme")) {
	const gchar *ret = gnome_vfs_uri_get_scheme(uri);

	if (ret)
	    return PyString_FromString(ret);
	Py_INCREF(Py_None);
	return Py_None;
    } else if (!strcmp(attr, "host_port")) {
	return PyInt_FromLong(gnome_vfs_uri_get_host_port(uri));
    } else if (!strcmp(attr, "user_name")) {
	const gchar *ret = gnome_vfs_uri_get_user_name(uri);

	if (ret)
	    return PyString_FromString(ret);
	Py_INCREF(Py_None);
	return Py_None;
    } else if (!strcmp(attr, "password")) {
	const gchar *ret = gnome_vfs_uri_get_password(uri);

	if (ret)
	    return PyString_FromString(ret);
	Py_INCREF(Py_None);
	return Py_None;
    } else if (!strcmp(attr, "path")) {
	const gchar *ret = gnome_vfs_uri_get_path(uri);

	if (ret)
	    return PyString_FromString(ret);
	Py_INCREF(Py_None);
	return Py_None;
    } else if (!strcmp(attr, "fragment_identifier")) {
	const gchar *ret = gnome_vfs_uri_get_fragment_identifier(uri);

	if (ret)
	    return PyString_FromString(ret);
	Py_INCREF(Py_None);
	return Py_None;
    } else if (!strcmp(attr, "dirname")) {
	gchar *ret = gnome_vfs_uri_extract_dirname(uri);

	if (ret) {
	    PyObject *pyret;

	    pyret = PyString_FromString(ret);
	    g_free(ret);
	    return pyret;
	}
	Py_INCREF(Py_None);
	return Py_None;
    } else if (!strcmp(attr, "short_name")) {
	gchar *ret = gnome_vfs_uri_extract_short_name(uri);

	if (ret) {
	    PyObject *pyret;

	    pyret = PyString_FromString(ret);
	    g_free(ret);
	    return pyret;
	}
	Py_INCREF(Py_None);
	return Py_None;
    } else if (!strcmp(attr, "short_path_name")) {
	gchar *ret = gnome_vfs_uri_extract_short_path_name(uri);

	if (ret) {
	    PyObject *pyret;

	    pyret = PyString_FromString(ret);
	    g_free(ret);
	    return pyret;
	}
	Py_INCREF(Py_None);
	return Py_None;
    } else {
	PyObject *name = PyString_FromString(attr);
	PyObject *ret = PyObject_GenericGetAttr((PyObject *)self, name);

	Py_DECREF(name);
	return ret;
    }
}

static int
pygvuri_setattr(PyGnomeVFSURI *self, const gchar *attr, PyObject *value)
{
    GnomeVFSURI *uri;

    uri = self->uri;
    if (!strcmp(attr, "__members__") ||
	!strcmp(attr, "dirname") ||
	!strcmp(attr, "fragment_identifier") ||
	!strcmp(attr, "is_local") ||
	!strcmp(attr, "parent") ||
	!strcmp(attr, "path") ||
	!strcmp(attr, "scheme") ||
	!strcmp(attr, "short_name") ||
	!strcmp(attr, "short_path_name") ||
	!strcmp(attr, "toplevel")) {
	PyErr_SetString(PyExc_TypeError, "readonly attribute");
	return -1;
    } else if (!strcmp(attr, "host_name")) {
	if (!value) {
	    PyErr_SetString(PyExc_TypeError, "can not delete attribute");
	    return -1;
	}
	if (!PyString_Check(value)) {
	    PyErr_SetString(PyExc_TypeError, "host_name must be a string");
	    return -1;
	}
	gnome_vfs_uri_set_host_name(uri, PyString_AsString(value));
	return 0;
    } else if (!strcmp(attr, "host_port")) {
	if (!value) {
	    PyErr_SetString(PyExc_TypeError, "can not delete attribute");
	    return -1;
	}
	if (!PyInt_Check(value)) {
	    PyErr_SetString(PyExc_TypeError, "host_port must be an integer");
	    return -1;
	}
	gnome_vfs_uri_set_host_port(uri, PyInt_AsLong(value));
	return 0;
    } else if (!strcmp(attr, "user_name")) {
	if (!value) {
	    PyErr_SetString(PyExc_TypeError, "can not delete attribute");
	    return -1;
	}
	if (!PyString_Check(value)) {
	    PyErr_SetString(PyExc_TypeError, "user_name must be a string");
	    return -1;
	}
	gnome_vfs_uri_set_user_name(uri, PyString_AsString(value));
	return 0;
    } else if (!strcmp(attr, "password")) {
	if (!value) {
	    PyErr_SetString(PyExc_TypeError, "can not delete attribute");
	    return -1;
	}
	if (!PyString_Check(value)) {
	    PyErr_SetString(PyExc_TypeError, "password must be a string");
	    return -1;
	}
	gnome_vfs_uri_set_password(uri, PyString_AsString(value));
	return 0;
    } else {
	PyObject *name = PyString_FromString(attr);
	int ret = PyObject_GenericSetAttr((PyObject *)self, name, value);

	Py_DECREF(name);
	return ret;
    }
}

static int
pygvuri_init(PyGnomeVFSURI *self, PyObject *args, PyObject *kwargs)
{
    static gchar *kwlist[] = { "text_uri", NULL };
    const gchar *text_uri;
 
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s:gnomevfs.URI.__init__",
				     kwlist, &text_uri))
	return -1;

    self->uri = gnome_vfs_uri_new(text_uri);
    if (!self->uri) {
	PyErr_SetString(PyExc_TypeError, "could not parse URI");
	return -1;
    }

    return 0;
}

static PyObject *
pygvuri_resolve_relative(PyGnomeVFSURI *self, PyObject *args,
			 PyObject *kwargs)
{
    static gchar *kwlist[] = { "relative_reference", NULL };
    const gchar *relative_reference;
    GnomeVFSURI *uri;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "s:gnomevfs.URI.resolve_relative",
				     kwlist, &relative_reference))
	return NULL;

    uri = gnome_vfs_uri_resolve_relative(self->uri, relative_reference);
    if (!uri) {
	PyErr_SetString(PyExc_TypeError, "could not build URI");
	return NULL;
    }
    return pygnome_vfs_uri_new(uri);
}

static PyObject *
pygvuri_append_string(PyGnomeVFSURI *self, PyObject *args,
		      PyObject *kwargs)
{
    static gchar *kwlist[] = { "path", NULL };
    const gchar *path;
    GnomeVFSURI *uri;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "s:gnomevfs.URI.append_string",
				     kwlist, &path))
	return NULL;

    uri = gnome_vfs_uri_append_string(self->uri, path);
    if (!uri) {
	PyErr_SetString(PyExc_TypeError, "could not build URI");
	return NULL;
    }
    return pygnome_vfs_uri_new(uri);
}

static PyObject *
pygvuri_append_path(PyGnomeVFSURI *self, PyObject *args,
		    PyObject *kwargs)
{
    static gchar *kwlist[] = { "path", NULL };
    const gchar *path;
    GnomeVFSURI *uri;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "s:gnomevfs.URI.append_path",
				     kwlist, &path))
	return NULL;

    uri = gnome_vfs_uri_append_path(self->uri, path);
    if (!uri) {
	PyErr_SetString(PyExc_TypeError, "could not build URI");
	return NULL;
    }
    return pygnome_vfs_uri_new(uri);
}

static PyObject *
pygvuri_append_file_name(PyGnomeVFSURI *self, PyObject *args,
			 PyObject *kwargs)
{
    static gchar *kwlist[] = { "file_name", NULL };
    const gchar *file_name;
    GnomeVFSURI *uri;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "s:gnomevfs.URI.append_file_name",
				     kwlist, &file_name))
	return NULL;

    uri = gnome_vfs_uri_append_file_name(self->uri, file_name);
    if (!uri) {
	PyErr_SetString(PyExc_TypeError, "could not build URI");
	return NULL;
    }
    return pygnome_vfs_uri_new(uri);
}

static PyObject *
pygvuri_copy(PyGnomeVFSURI *self)
{
    return pygnome_vfs_uri_new(gnome_vfs_uri_dup(self->uri));
}

static PyObject *
pygvuri_is_parent(PyGnomeVFSURI *self, PyObject *args,
		  PyObject *kwargs)
{
    static gchar *kwlist[] = { "item", "recursive", NULL };
    PyObject *item;
    gboolean recursive = TRUE, ret;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "O!|i:gnomevfs.URI.is_parent", kwlist,
				     &PyGnomeVFSURI_Type, &item, &recursive))
	return NULL;
    ret = gnome_vfs_uri_is_parent(self->uri, pygnome_vfs_uri_get(item),
				  recursive);
    return PyInt_FromLong(ret);
}

static PyMethodDef pygvuri_methods[] = {
    { "resolve_relative", (PyCFunction)pygvuri_resolve_relative,
      METH_VARARGS|METH_KEYWORDS },
    { "append_string", (PyCFunction)pygvuri_append_string,
      METH_VARARGS|METH_KEYWORDS },
    { "append_path", (PyCFunction)pygvuri_append_path,
      METH_VARARGS|METH_KEYWORDS },
    { "append_file_name", (PyCFunction)pygvuri_append_file_name,
      METH_VARARGS|METH_KEYWORDS },
    { "copy", (PyCFunction)pygvuri_copy, METH_NOARGS },
    { "is_parent", (PyCFunction)pygvuri_is_parent, METH_VARARGS|METH_KEYWORDS},
    { NULL, NULL, 0 }
};


PyTypeObject PyGnomeVFSURI_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "gnomevfs.URI",                    /* tp_name */
    sizeof(PyGnomeVFSURI),              /* tp_basicsize */
    0,                                  /* tp_itemsize */
    /* methods */
    (destructor)pygvuri_dealloc,        /* tp_dealloc */
    (printfunc)0,                       /* tp_print */
    (getattrfunc)pygvuri_getattr,       /* tp_getattr */
    (setattrfunc)pygvuri_setattr,       /* tp_setattr */
    (cmpfunc)pygvuri_compare,           /* tp_compare */
    (reprfunc)pygvuri_repr,             /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    (hashfunc)pygvuri_hash,             /* tp_hash */
    (ternaryfunc)0,                     /* tp_call */
    (reprfunc)pygvuri_str,              /* tp_str */
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
    pygvuri_methods,                    /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    (PyTypeObject *)0,                  /* tp_base */
    (PyObject *)0,                      /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc)pygvuri_init,             /* tp_init */
    PyType_GenericAlloc,                /* tp_alloc */
    PyType_GenericNew,                  /* tp_new */
    0,                                  /* tp_free */
    (inquiry)0,                         /* tp_is_gc */
    (PyObject *)0,                      /* tp_bases */
};
