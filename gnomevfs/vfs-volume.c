/* -*- mode: C; c-basic-offset: 4 -*- */

#include "pygnomevfs-private.h"
#define NO_IMPORT_PYGOBJECT
#include <pygobject.h>


static PyObject *
pygvvolume_get_id(PyGObject *self)
{
    return PyLong_FromUnsignedLong(gnome_vfs_volume_get_id(GNOME_VFS_VOLUME(self->obj)));
}

static PyObject *
pygvvolume_get_volume_type(PyGObject *self)
{
    return PyInt_FromLong(gnome_vfs_volume_get_volume_type(GNOME_VFS_VOLUME(self->obj)));
}

static PyObject *
pygvvolume_get_device_type(PyGObject *self)
{
    return PyInt_FromLong(gnome_vfs_volume_get_device_type(GNOME_VFS_VOLUME(self->obj)));
}

static PyObject *
pygvvolume_get_device_path(PyGObject *self)
{
    char *retval;

    retval = gnome_vfs_volume_get_device_path(GNOME_VFS_VOLUME(self->obj));
    if (retval)
        return PyString_FromString(retval);
    else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

static PyObject *
pygvvolume_get_activation_uri(PyGObject *self)
{
    char *retval;

    retval = gnome_vfs_volume_get_activation_uri(GNOME_VFS_VOLUME(self->obj));
    if (retval)
        return PyString_FromString(retval);
    else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

static PyObject *
pygvvolume_get_filesystem_type(PyGObject *self)
{
    char *retval;

    retval = gnome_vfs_volume_get_filesystem_type(GNOME_VFS_VOLUME(self->obj));
    if (retval)
        return PyString_FromString(retval);
    else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

static PyObject *
pygvvolume_get_display_name(PyGObject *self)
{
    char *retval;

    retval = gnome_vfs_volume_get_display_name(GNOME_VFS_VOLUME(self->obj));
    if (retval)
        return PyString_FromString(retval);
    else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

static PyObject *
pygvvolume_get_icon(PyGObject *self)
{
    char *retval;

    retval = gnome_vfs_volume_get_icon(GNOME_VFS_VOLUME(self->obj));
    if (retval)
        return PyString_FromString(retval);
    else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

static PyObject *
pygvvolume_get_hal_udi(PyGObject *self)
{
    char *retval;

    retval = gnome_vfs_volume_get_hal_udi(GNOME_VFS_VOLUME(self->obj));
    if (retval)
        return PyString_FromString(retval);
    else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

static PyObject *
pygvvolume_is_user_visible(PyGObject *self)
{
    gboolean retval;

    retval = gnome_vfs_volume_is_user_visible(GNOME_VFS_VOLUME(self->obj));
    if (retval) {
        Py_INCREF(Py_True);
        return Py_True;
    } else {
        Py_INCREF(Py_False);
        return Py_False;
    }
}

static PyObject *
pygvvolume_is_read_only(PyGObject *self)
{
    gboolean retval;

    retval = gnome_vfs_volume_is_read_only(GNOME_VFS_VOLUME(self->obj));
    if (retval) {
        Py_INCREF(Py_True);
        return Py_True;
    } else {
        Py_INCREF(Py_False);
        return Py_False;
    }
}

static PyObject *
pygvvolume_is_mounted(PyGObject *self)
{
    gboolean retval;

    retval = gnome_vfs_volume_is_mounted(GNOME_VFS_VOLUME(self->obj));
    if (retval) {
        Py_INCREF(Py_True);
        return Py_True;
    } else {
        Py_INCREF(Py_False);
        return Py_False;
    }
}

static PyObject *
pygvvolume_handles_trash(PyGObject *self)
{
    gboolean retval;

    retval = gnome_vfs_volume_handles_trash(GNOME_VFS_VOLUME(self->obj));
    if (retval) {
        Py_INCREF(Py_True);
        return Py_True;
    } else {
        Py_INCREF(Py_False);
        return Py_False;
    }
}

static PyObject *
pygvvolume_get_drive(PyGObject *self)
{
    GnomeVFSDrive *drive = gnome_vfs_volume_get_drive(GNOME_VFS_VOLUME(self->obj));
    PyObject *retval = pygobject_new((GObject *) drive);
    g_object_unref(G_OBJECT(drive));
    return retval;
}

static PyObject *
pygvvolume_unmount(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "callback", "user_data", NULL };
    PyObject *py_callback, *py_user_data = NULL;
    PyGnomeVFSVolumeOpCallback *callback_context;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
                                     "O|O:gnomevfs.Volume.unmount",
                                     kwlist, &py_callback, py_user_data))
        return NULL;

    if (!PyCallable_Check(py_callback)) {
        PyErr_SetString(PyExc_TypeError, "first argument must be callable");
        return NULL;
    }

    callback_context = g_new(PyGnomeVFSVolumeOpCallback, 1);
    callback_context->callback = py_callback;
    Py_INCREF(py_callback);
    callback_context->user_data = py_user_data;
    Py_XINCREF(py_user_data);

    gnome_vfs_volume_unmount(GNOME_VFS_VOLUME(self->obj),
                             wrap_gnomevfs_volume_op_callback,
                             callback_context);
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *
pygvvolume_eject(PyGObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "callback", "user_data", NULL };
    PyObject *py_callback, *py_user_data = NULL;
    PyGnomeVFSVolumeOpCallback *callback_context;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
                                     "O|O:gnomevfs.Volume.eject",
                                     kwlist, &py_callback, py_user_data))
        return NULL;

    if (!PyCallable_Check(py_callback)) {
        PyErr_SetString(PyExc_TypeError, "first argument must be callable");
        return NULL;
    }

    callback_context = g_new(PyGnomeVFSVolumeOpCallback, 1);
    callback_context->callback = py_callback;
    Py_INCREF(py_callback);
    callback_context->user_data = py_user_data;
    Py_XINCREF(py_user_data);

    gnome_vfs_volume_eject(GNOME_VFS_VOLUME(self->obj),
                           wrap_gnomevfs_volume_op_callback,
                           callback_context);
    Py_INCREF(Py_None);
    return Py_None;
}


static PyMethodDef pygvvolume_methods[] = {
    { "get_id", (PyCFunction)pygvvolume_get_id, METH_NOARGS },
    { "get_volume_type", (PyCFunction)pygvvolume_get_volume_type, METH_NOARGS },
    { "get_device_type", (PyCFunction)pygvvolume_get_device_type, METH_NOARGS },
    { "get_drive", (PyCFunction)pygvvolume_get_drive, METH_NOARGS },
    { "get_device_path", (PyCFunction)pygvvolume_get_device_path, METH_NOARGS },
    { "get_activation_uri", (PyCFunction)pygvvolume_get_activation_uri, METH_NOARGS },
    { "get_filesystem_type", (PyCFunction)pygvvolume_get_filesystem_type, METH_NOARGS },
    { "get_display_name", (PyCFunction)pygvvolume_get_display_name, METH_NOARGS },
    { "get_icon", (PyCFunction)pygvvolume_get_icon, METH_NOARGS },
    { "get_hal_udi", (PyCFunction)pygvvolume_get_hal_udi, METH_NOARGS },
    { "is_user_visible", (PyCFunction)pygvvolume_is_user_visible, METH_NOARGS },
    { "is_read_only", (PyCFunction)pygvvolume_is_read_only, METH_NOARGS },
    { "is_mounted", (PyCFunction)pygvvolume_is_mounted, METH_NOARGS },
    { "handles_trash", (PyCFunction)pygvvolume_handles_trash, METH_NOARGS },
    { "unmount", (PyCFunction)pygvvolume_unmount, METH_VARARGS|METH_KEYWORDS },
    { "eject", (PyCFunction)pygvvolume_eject, METH_VARARGS|METH_KEYWORDS },
    { NULL, NULL, 0 }
};


static int
pygvvolume_compare(PyGObject *self, PyGObject *other)
{
    return gnome_vfs_volume_compare(GNOME_VFS_VOLUME(self->obj),
                                    GNOME_VFS_VOLUME(other->obj));
}


PyTypeObject PyGnomeVFSVolume_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "gnomevfs.Volume",                  /* tp_name */
    sizeof(PyGObject),                  /* tp_basicsize */
    0,                                  /* tp_itemsize */
    /* methods */
    (destructor)0,     			/* tp_dealloc */
    (printfunc)0,                       /* tp_print */
    (getattrfunc)0,                     /* tp_getattr */
    (setattrfunc)0,                     /* tp_setattr */
    (cmpfunc)pygvvolume_compare,        /* tp_compare */
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
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,		/* tp_flags */
    NULL, /* Documentation string */
    (traverseproc)0,                    /* tp_traverse */
    (inquiry)0,                         /* tp_clear */
    (richcmpfunc)0,                     /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    (getiterfunc)0,                     /* tp_iter */
    (iternextfunc)0,                    /* tp_iternext */
    pygvvolume_methods,                 /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    (PyTypeObject *)0,                  /* tp_base */
    (PyObject *)0,                      /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc)0,          		/* tp_init */
    PyType_GenericAlloc,                /* tp_alloc */
    PyType_GenericNew,                  /* tp_new */
    0,                                  /* tp_free */
    (inquiry)0,                         /* tp_is_gc */
    (PyObject *)0,                      /* tp_bases */
};


void
pygvvolume_add_constants(PyObject *m)
{
#define add_item(name)\
    PyModule_AddIntConstant(m, #name, GNOME_VFS_##name)

    add_item(DEVICE_TYPE_UNKNOWN);
    add_item(DEVICE_TYPE_AUDIO_CD);
    add_item(DEVICE_TYPE_VIDEO_DVD);
    add_item(DEVICE_TYPE_HARDDRIVE);
    add_item(DEVICE_TYPE_CDROM);
    add_item(DEVICE_TYPE_FLOPPY);
    add_item(DEVICE_TYPE_ZIP);
    add_item(DEVICE_TYPE_JAZ);
    add_item(DEVICE_TYPE_NFS);
    add_item(DEVICE_TYPE_AUTOFS);
    add_item(DEVICE_TYPE_CAMERA);
    add_item(DEVICE_TYPE_MEMORY_STICK);
    add_item(DEVICE_TYPE_SMB);
    add_item(DEVICE_TYPE_APPLE);
    add_item(DEVICE_TYPE_MUSIC_PLAYER);
    add_item(DEVICE_TYPE_WINDOWS);
    add_item(DEVICE_TYPE_LOOPBACK);
    add_item(DEVICE_TYPE_NETWORK);

    add_item(VOLUME_TYPE_MOUNTPOINT);
    add_item(VOLUME_TYPE_VFS_MOUNT);
    add_item(VOLUME_TYPE_CONNECTED_SERVER);

#undef add_item
}
