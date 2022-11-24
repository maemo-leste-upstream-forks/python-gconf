/* -*- mode: C; c-basic-offset: 4 -*- */

#include "pygnomevfs-private.h"


PyObject *
pygnome_vfs_xfer_progress_info_getattr(PyGnomeVFSXferProgressInfo *self, char *attr)
{

    if (!self->info) {
	PyErr_SetString(PyExc_RuntimeError, "this XferProgressInfo is no longer valid");
	return NULL;
    }
    
#define add_int_attr(name)				\
    else if (!strcmp(attr, #name)) {			\
	return PyInt_FromLong(self->info->name);	\
    }
#define add_size_attr(name)					\
    else if (!strcmp(attr, #name)) {				\
	return PyLong_FromUnsignedLongLong(self->info->name);	\
    }
#define add_string_attr(name)					\
    else if (!strcmp(attr, #name)) {				\
	if (self->info->name) {					\
	    return PyString_FromString(self->info->name);	\
	} else {						\
	    Py_INCREF(Py_None);					\
	    return Py_None;					\
        }							\
    }
#define add_bool_attr(name)						\
    else if (!strcmp(attr, #name)) {					\
	PyObject *retval = (self->info->name)? Py_True : Py_False;	\
	Py_INCREF(retval);						\
	return retval;							\
    }

    if (!strcmp(attr, "__members__")) {
	return Py_BuildValue("[ssssssssssss]", "status", "vfs_status",
			     "phase", "source_name", "target_name",
			     "file_index", "files_total", "bytes_total",
			     "bytes_copied", "total_bytes_copied", "duplicate_name",
			     "top_level_item");
    }
    add_int_attr(status)
    add_int_attr(vfs_status)
    add_int_attr(phase)
    add_string_attr(source_name)
    add_string_attr(target_name)
    add_int_attr(file_index)
    add_int_attr(files_total)
    add_size_attr(bytes_total)
    add_size_attr(bytes_copied)
    add_size_attr(total_bytes_copied)
    add_string_attr(duplicate_name)
    add_bool_attr(top_level_item)
    else {
	PyObject *name = PyString_FromString(attr);
	PyObject *ret = PyObject_GenericGetAttr((PyObject *)self, name);

	Py_DECREF(name);
	return ret;
    }

#undef add_int_attr
#undef add_string_attr
#undef add_size_attr
#undef add_bool_attr
}

int
pygnome_vfs_xfer_progress_info_setattr(PyGnomeVFSXferProgressInfo *self,
				       char *attr, PyObject *value)
{
    if (!self->info) {
	PyErr_SetString(PyExc_RuntimeError, "this XferProgressInfo is no longer valid");
	return -1;
    }

#define add_int_attr(name)									\
    else if (!strcmp(attr, #name)) {								\
	if (!PyInt_Check(value)) {								\
	    PyErr_SetString(PyExc_TypeError, "XferProgressInfo."#name" must be of type 'int'");	\
	    return -1;										\
	}											\
	self->info->name = PyInt_AsLong(value);							\
    }
#define add_size_attr(name)										\
    else if (!strcmp(attr, #name)) {									\
	if (!PyLong_Check(value)) {									\
	    PyErr_SetString(PyExc_TypeError, "XferProgressInfo."#name" must be of type 'long'");	\
	    return -1;											\
	}												\
	self->info->name = PyLong_AsUnsignedLongLong(value);						\
    }
#define add_string_attr(name)							\
    else if (!strcmp(attr, #name)) {						\
	if (value == Py_None) {							\
	    if (self->info->name) g_free(self->info->name);			\
	    self->info->name = NULL;						\
	} else {								\
	    if (!PyString_Check(value)) {					\
		PyErr_SetString(PyExc_TypeError, "XferProgressInfo."#name	\
				" must be of type 'str' or None");		\
		return -1;							\
	    }									\
	    if (self->info->name) g_free(self->info->name);			\
	    self->info->name = g_strdup(PyString_AsString(value));		\
        }									\
    }
#define add_bool_attr(name)				\
    else if (!strcmp(attr, #name)) {			\
	self->info->name = PyObject_IsTrue(value);	\
    }

    if (0) {}
    add_int_attr(status)
    add_int_attr(vfs_status)
    add_int_attr(phase)
    add_string_attr(source_name)
    add_string_attr(target_name)
    add_int_attr(file_index)
    add_int_attr(files_total)
    add_size_attr(bytes_total)
    add_size_attr(bytes_copied)
    add_size_attr(total_bytes_copied)
    add_string_attr(duplicate_name)
    add_bool_attr(top_level_item)
	;
#undef add_int_attr
#undef add_string_attr
#undef add_size_attr
#undef add_bool_attr
    return -1;
}

PyObject *
pygnome_vfs_xfer_progress_info_new(GnomeVFSXferProgressInfo *info)
{
    PyObject *self;
    self = (PyObject *) PyObject_NEW(PyGnomeVFSXferProgressInfo,
				     &PyGnomeVFSXferProgressInfo_Type);
    if (self == NULL) return NULL;
    pygnome_vfs_xfer_progress_info_set(self, info);
    return self;
}

PyTypeObject PyGnomeVFSXferProgressInfo_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "gnomevfs.XferProgressInfo",       /* tp_name */
    sizeof(PyGnomeVFSXferProgressInfo), /* tp_basicsize */
    0,                                  /* tp_itemsize */
    /* methods */
    (destructor)0,                      /* tp_dealloc */
    (printfunc)0,                       /* tp_print */
    (getattrfunc)pygnome_vfs_xfer_progress_info_getattr,  /* tp_getattr */
    (setattrfunc)pygnome_vfs_xfer_progress_info_setattr,  /* tp_setattr */
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
    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
};
