/* -*- mode: C; c-basic-offset: 4 -*- */
#include <pygobject.h>
#include "pygnomevfs-private.h"
#include <libgnomevfs/gnome-vfs-utils.h>
#include <libgnomevfs/gnome-vfs-find-directory.h>
#include <libgnomevfs/gnome-vfs-address.h>
#include <libgnomevfs/gnome-vfs-resolve.h>
#include <libgnomevfs/gnome-vfs-dns-sd.h>
#include <libgnomevfs/gnome-vfs-async-ops.h>
#include <libgnomevfs/gnome-vfs-mime.h>
#include "pygnomevfsbonobo.h"

static GHashTable *monitor_hash;
static gint monitor_id_counter = 0;

static PyObject *pygnomevfs_exc;
static PyObject *pygnomevfs_not_found_exc;
static PyObject *pygnomevfs_generic_exc;
static PyObject *pygnomevfs_internal_exc;
static PyObject *pygnomevfs_bad_parameters_exc;
static PyObject *pygnomevfs_not_supported_exc;
static PyObject *pygnomevfs_io_exc;
static PyObject *pygnomevfs_corrupted_data_exc;
static PyObject *pygnomevfs_wrong_format_exc;
static PyObject *pygnomevfs_bad_file_exc;
static PyObject *pygnomevfs_too_big_exc;
static PyObject *pygnomevfs_no_space_exc;
static PyObject *pygnomevfs_read_only_exc;
static PyObject *pygnomevfs_invalid_uri_exc;
static PyObject *pygnomevfs_not_open_exc;
static PyObject *pygnomevfs_invalid_open_mode_exc;
static PyObject *pygnomevfs_access_denied_exc;
static PyObject *pygnomevfs_too_many_open_files_exc;
static PyObject *pygnomevfs_eof_exc;
static PyObject *pygnomevfs_not_a_directory_exc;
static PyObject *pygnomevfs_in_progress_exc;
static PyObject *pygnomevfs_interrupted_exc;
static PyObject *pygnomevfs_file_exists_exc;
static PyObject *pygnomevfs_loop_exc;
static PyObject *pygnomevfs_not_permitted_exc;
static PyObject *pygnomevfs_is_directory_exc;
static PyObject *pygnomevfs_no_memory_exc;
static PyObject *pygnomevfs_host_not_found_exc;
static PyObject *pygnomevfs_invalid_host_name_exc;
static PyObject *pygnomevfs_host_has_no_address_exc;
static PyObject *pygnomevfs_login_failed_exc;
static PyObject *pygnomevfs_cancelled_exc;
static PyObject *pygnomevfs_directory_busy_exc;
static PyObject *pygnomevfs_directory_not_empty_exc;
static PyObject *pygnomevfs_too_many_links_exc;
static PyObject *pygnomevfs_read_only_file_system_exc;
static PyObject *pygnomevfs_not_same_file_system_exc;
static PyObject *pygnomevfs_name_too_long_exc;
static PyObject *pygnomevfs_service_not_available_exc;
static PyObject *pygnomevfs_service_obsolete_exc;
static PyObject *pygnomevfs_protocol_error_exc;
static PyObject *pygnomevfs_no_master_browser_exc;
#if 0
static PyObject *pygnomevfs_no_default_exc;
static PyObject *pygnomevfs_no_handler_exc;
static PyObject *pygnomevfs_parse_exc;
static PyObject *pygnomevfs_launch_exc;
#endif

static PyTypeObject *_PyGObject_Type;
#define PyGObject_Type (*_PyGObject_Type)


extern void pygvvolume_add_constants(PyObject *m);

static void
pygvfs_lazy_load_pygnomevfsbonobo(void)
{
    static PyObject *pygnomevfs_module = NULL;
    if (!pygnomevfs_module)
        pygnomevfs_module = pygnome_vfs_bonobo_init();
}


gboolean
pygnome_vfs_result_check(GnomeVFSResult result)
{
    PyObject *exc;

    /* be optimistic */
    if(result == GNOME_VFS_OK)
	return FALSE;

    switch(result)
    {
    case GNOME_VFS_ERROR_NOT_FOUND:
	exc = pygnomevfs_not_found_exc;
	break;
    case GNOME_VFS_ERROR_GENERIC:
	exc = pygnomevfs_generic_exc;
	break;
    case GNOME_VFS_ERROR_INTERNAL:
	exc = pygnomevfs_internal_exc;
	break;
    case GNOME_VFS_ERROR_BAD_PARAMETERS:
	exc = pygnomevfs_bad_parameters_exc;
	break;
    case GNOME_VFS_ERROR_NOT_SUPPORTED:
	exc = pygnomevfs_not_supported_exc;
	break;
    case GNOME_VFS_ERROR_IO:
	exc = pygnomevfs_io_exc;
	break;
    case GNOME_VFS_ERROR_CORRUPTED_DATA:
	exc = pygnomevfs_corrupted_data_exc;
	break;
    case GNOME_VFS_ERROR_WRONG_FORMAT:
	exc = pygnomevfs_wrong_format_exc;
	break;
    case GNOME_VFS_ERROR_BAD_FILE:
	exc = pygnomevfs_bad_file_exc;
	break;
    case GNOME_VFS_ERROR_TOO_BIG:
	exc = pygnomevfs_too_big_exc;
	break;
    case GNOME_VFS_ERROR_NO_SPACE:
	exc = pygnomevfs_no_space_exc;
	break;
    case GNOME_VFS_ERROR_READ_ONLY:
	exc = pygnomevfs_read_only_exc;
	break;
    case GNOME_VFS_ERROR_INVALID_URI:
	exc = pygnomevfs_invalid_uri_exc;
	break;
    case GNOME_VFS_ERROR_NOT_OPEN:
	exc = pygnomevfs_not_open_exc;
	break;
    case GNOME_VFS_ERROR_INVALID_OPEN_MODE:
	exc = pygnomevfs_invalid_open_mode_exc;
	break;
    case GNOME_VFS_ERROR_ACCESS_DENIED:
	exc = pygnomevfs_access_denied_exc;
	break;
    case GNOME_VFS_ERROR_TOO_MANY_OPEN_FILES:
	exc = pygnomevfs_too_many_open_files_exc;
	break;
    case GNOME_VFS_ERROR_EOF:
	exc = pygnomevfs_eof_exc;
	break;
    case GNOME_VFS_ERROR_NOT_A_DIRECTORY:
	exc = pygnomevfs_not_a_directory_exc;
	break;
    case GNOME_VFS_ERROR_IN_PROGRESS:
	exc = pygnomevfs_in_progress_exc;
	break;
    case GNOME_VFS_ERROR_INTERRUPTED:
	exc = pygnomevfs_interrupted_exc;
	break;
    case GNOME_VFS_ERROR_FILE_EXISTS:
	exc = pygnomevfs_file_exists_exc;
	break;
    case GNOME_VFS_ERROR_LOOP:
	exc = pygnomevfs_loop_exc;
	break;
    case GNOME_VFS_ERROR_NOT_PERMITTED:
	exc = pygnomevfs_not_permitted_exc;
	break;
    case GNOME_VFS_ERROR_IS_DIRECTORY:
	exc = pygnomevfs_is_directory_exc;
	break;
    case GNOME_VFS_ERROR_NO_MEMORY:
	exc = pygnomevfs_no_memory_exc;
	break;
    case GNOME_VFS_ERROR_HOST_NOT_FOUND:
	exc = pygnomevfs_host_not_found_exc;
	break;
    case GNOME_VFS_ERROR_INVALID_HOST_NAME:
	exc = pygnomevfs_invalid_host_name_exc;
	break;
    case GNOME_VFS_ERROR_HOST_HAS_NO_ADDRESS:
	exc = pygnomevfs_host_has_no_address_exc;
	break;
    case GNOME_VFS_ERROR_LOGIN_FAILED:
	exc = pygnomevfs_login_failed_exc;
	break;
    case GNOME_VFS_ERROR_CANCELLED:
	exc = pygnomevfs_cancelled_exc;
	break;
    case GNOME_VFS_ERROR_DIRECTORY_BUSY:
	exc = pygnomevfs_directory_busy_exc;
	break;
    case GNOME_VFS_ERROR_DIRECTORY_NOT_EMPTY:
	exc = pygnomevfs_directory_not_empty_exc;
	break;
    case GNOME_VFS_ERROR_TOO_MANY_LINKS:
	exc = pygnomevfs_too_many_links_exc;
	break;
    case GNOME_VFS_ERROR_READ_ONLY_FILE_SYSTEM:
	exc = pygnomevfs_read_only_file_system_exc;
	break;
    case GNOME_VFS_ERROR_NOT_SAME_FILE_SYSTEM:
	exc = pygnomevfs_not_same_file_system_exc;
	break;
    case GNOME_VFS_ERROR_NAME_TOO_LONG:
	exc = pygnomevfs_name_too_long_exc;
	break;
    case GNOME_VFS_ERROR_SERVICE_NOT_AVAILABLE:
	exc = pygnomevfs_service_not_available_exc;
	break;
    case GNOME_VFS_ERROR_SERVICE_OBSOLETE:
	exc = pygnomevfs_service_obsolete_exc;
	break;
    case GNOME_VFS_ERROR_PROTOCOL_ERROR:
	exc = pygnomevfs_protocol_error_exc;
	break;
    case GNOME_VFS_ERROR_NO_MASTER_BROWSER:
	exc = pygnomevfs_no_master_browser_exc;
	break;
#if 0	
    case GNOME_VFS_ERROR_NO_DEFAULT:
	exc = pygnomevfs_no_default_exc;
	break;
    case GNOME_VFS_ERROR_NO_HANDLER:
	exc = pygnomevfs_no_handler_exc;
	break;
    case GNOME_VFS_ERROR_PARSE:
	exc = pygnomevfs_parse_exc;
	break;
    case GNOME_VFS_ERROR_LAUNCH:
	exc = pygnomevfs_launch_exc;
	break;
#endif	
    default:
	exc = NULL;
	break;
    }


    if(exc)
    {
	const char *msg;
	msg = gnome_vfs_result_to_string(result);
	PyErr_SetString(exc, (char*)msg);
	return TRUE;
    }

    return FALSE;
}


GnomeVFSResult pygnome_vfs_exception_check(void)
{
    if (!PyErr_Occurred()) {
	return -1;
    }
    
    if (PyErr_ExceptionMatches(pygnomevfs_not_found_exc)) {
	return GNOME_VFS_ERROR_NOT_FOUND;
    } else if (PyErr_ExceptionMatches(pygnomevfs_generic_exc)) {
	return GNOME_VFS_ERROR_GENERIC;
    } else if (PyErr_ExceptionMatches(pygnomevfs_internal_exc)) {
	return GNOME_VFS_ERROR_INTERNAL;
    } else if (PyErr_ExceptionMatches(pygnomevfs_bad_parameters_exc)) {
	return GNOME_VFS_ERROR_BAD_FILE;
    } else if (PyErr_ExceptionMatches(pygnomevfs_not_supported_exc)) {
	return GNOME_VFS_ERROR_NOT_SUPPORTED;
    } else if (PyErr_ExceptionMatches(pygnomevfs_io_exc)) {
	return GNOME_VFS_ERROR_IO;
    } else if (PyErr_ExceptionMatches(pygnomevfs_corrupted_data_exc)) {
	return GNOME_VFS_ERROR_CORRUPTED_DATA;
    } else if (PyErr_ExceptionMatches(pygnomevfs_wrong_format_exc)) {
	return GNOME_VFS_ERROR_WRONG_FORMAT;
    } else if (PyErr_ExceptionMatches(pygnomevfs_bad_file_exc)) {
	return GNOME_VFS_ERROR_BAD_FILE;
    } else if (PyErr_ExceptionMatches(pygnomevfs_too_big_exc)) {
	return GNOME_VFS_ERROR_TOO_BIG;
    } else if (PyErr_ExceptionMatches(pygnomevfs_no_space_exc)) {
	return GNOME_VFS_ERROR_NO_SPACE;
    } else if (PyErr_ExceptionMatches(pygnomevfs_read_only_exc)) {
	return GNOME_VFS_ERROR_READ_ONLY;
    } else if (PyErr_ExceptionMatches(pygnomevfs_invalid_uri_exc)) {
	return GNOME_VFS_ERROR_INVALID_URI;
    } else if (PyErr_ExceptionMatches(pygnomevfs_not_open_exc)) {
	return GNOME_VFS_ERROR_NOT_OPEN;
    } else if (PyErr_ExceptionMatches(pygnomevfs_invalid_open_mode_exc)) {
	return GNOME_VFS_ERROR_INVALID_OPEN_MODE;
    } else if (PyErr_ExceptionMatches(pygnomevfs_access_denied_exc)) {
	return GNOME_VFS_ERROR_ACCESS_DENIED;
    } else if (PyErr_ExceptionMatches(pygnomevfs_too_many_open_files_exc)) {
	return GNOME_VFS_ERROR_TOO_MANY_OPEN_FILES;
    } else if (PyErr_ExceptionMatches(pygnomevfs_eof_exc)) {
	return GNOME_VFS_ERROR_EOF;
    } else if (PyErr_ExceptionMatches(pygnomevfs_not_a_directory_exc)) {
	return GNOME_VFS_ERROR_NOT_A_DIRECTORY;
    } else if (PyErr_ExceptionMatches(pygnomevfs_in_progress_exc)) {
	return GNOME_VFS_ERROR_IN_PROGRESS;
    } else if (PyErr_ExceptionMatches(pygnomevfs_interrupted_exc)) {
	return GNOME_VFS_ERROR_INTERRUPTED;
    } else if (PyErr_ExceptionMatches(pygnomevfs_file_exists_exc)) {
	return GNOME_VFS_ERROR_FILE_EXISTS;
    } else if (PyErr_ExceptionMatches(pygnomevfs_loop_exc)) {
	return GNOME_VFS_ERROR_LOOP;
    } else if (PyErr_ExceptionMatches(pygnomevfs_not_permitted_exc)) {
	return GNOME_VFS_ERROR_NOT_PERMITTED;
    } else if (PyErr_ExceptionMatches(pygnomevfs_is_directory_exc)) {
	return GNOME_VFS_ERROR_IS_DIRECTORY;
    } else if (PyErr_ExceptionMatches(pygnomevfs_no_memory_exc)) {
	return GNOME_VFS_ERROR_NO_MEMORY;
    } else if (PyErr_ExceptionMatches(pygnomevfs_host_not_found_exc)) {
	return GNOME_VFS_ERROR_HOST_NOT_FOUND;
    } else if (PyErr_ExceptionMatches(pygnomevfs_invalid_host_name_exc)) {
	return GNOME_VFS_ERROR_INVALID_HOST_NAME;
    } else if (PyErr_ExceptionMatches(pygnomevfs_host_has_no_address_exc)) {
	return GNOME_VFS_ERROR_HOST_HAS_NO_ADDRESS;
    } else if (PyErr_ExceptionMatches(pygnomevfs_login_failed_exc)) {
	return GNOME_VFS_ERROR_LOGIN_FAILED;
    } else if (PyErr_ExceptionMatches(pygnomevfs_cancelled_exc)) {
	return GNOME_VFS_ERROR_CANCELLED;
    } else if (PyErr_ExceptionMatches(pygnomevfs_directory_busy_exc)) {
	return GNOME_VFS_ERROR_DIRECTORY_BUSY;
    } else if (PyErr_ExceptionMatches(pygnomevfs_directory_not_empty_exc)) {
	return GNOME_VFS_ERROR_DIRECTORY_NOT_EMPTY;
    } else if (PyErr_ExceptionMatches(pygnomevfs_too_many_links_exc)) {
	return GNOME_VFS_ERROR_TOO_MANY_LINKS;
    } else if (PyErr_ExceptionMatches(pygnomevfs_read_only_file_system_exc)) {
	return GNOME_VFS_ERROR_READ_ONLY_FILE_SYSTEM;
    } else if (PyErr_ExceptionMatches(pygnomevfs_not_same_file_system_exc)) {
	return GNOME_VFS_ERROR_NOT_SAME_FILE_SYSTEM;
    } else if (PyErr_ExceptionMatches(pygnomevfs_name_too_long_exc)) {
	return GNOME_VFS_ERROR_NAME_TOO_LONG;
    } else if (PyErr_ExceptionMatches(pygnomevfs_service_not_available_exc)) {
	return GNOME_VFS_ERROR_SERVICE_NOT_AVAILABLE;
    } else if (PyErr_ExceptionMatches(pygnomevfs_service_obsolete_exc)) {
	return GNOME_VFS_ERROR_SERVICE_OBSOLETE;
    } else if (PyErr_ExceptionMatches(pygnomevfs_protocol_error_exc)) {
	return GNOME_VFS_ERROR_PROTOCOL_ERROR;
    } else if (PyErr_ExceptionMatches(pygnomevfs_no_master_browser_exc)) {
	return GNOME_VFS_ERROR_NO_MASTER_BROWSER;
    }
    
    return -2;
}

static PyObject *
pygvfs_create(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "uri", "open_mode", "exclusive", "perm", NULL };
    PyObject *uri;
    GnomeVFSOpenMode open_mode = GNOME_VFS_OPEN_NONE;
    gboolean exclusive = FALSE;
    guint perm = 0666;
    GnomeVFSHandle *handle;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|iii:gnomevfs.create",
				     kwlist, &uri, &open_mode, &exclusive,
				     &perm))
	return NULL;

    if (PyObject_TypeCheck(uri, &PyGnomeVFSURI_Type)) {
        pyg_begin_allow_threads;
	result = gnome_vfs_create_uri(&handle, pygnome_vfs_uri_get(uri),
				      open_mode, exclusive, perm);
        pyg_end_allow_threads;
    } else if (PyString_Check(uri)) {
        pyg_begin_allow_threads;
	result = gnome_vfs_create(&handle, PyString_AsString(uri),
				  open_mode, exclusive, perm);
        pyg_end_allow_threads;
    } else {
	PyErr_SetString(PyExc_TypeError,
			"uri must be a gnomevfs.URI or a string");
	return NULL;
    }

    if (pygnome_vfs_result_check(result))
	return NULL;

    return pygnome_vfs_handle_new(handle);
}

static PyObject *
pygvfs_get_file_info(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "uri", "options", NULL };
    PyObject *uri;
    GnomeVFSFileInfo *finfo;
    GnomeVFSFileInfoOptions options = GNOME_VFS_FILE_INFO_DEFAULT;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "O|i:gnomevfs.get_file_info",
				     kwlist, &uri, &options))
	return NULL;

    finfo = gnome_vfs_file_info_new();
    if (PyObject_TypeCheck(uri, &PyGnomeVFSURI_Type)) {
        pyg_begin_allow_threads;
	result = gnome_vfs_get_file_info_uri(pygnome_vfs_uri_get(uri), finfo,
					     options);
        pyg_end_allow_threads;
    } else if (PyString_Check(uri)) {
        pyg_begin_allow_threads;
	result = gnome_vfs_get_file_info(PyString_AsString(uri), finfo,
					 options);
        pyg_end_allow_threads;
    } else {
	PyErr_SetString(PyExc_TypeError,
			"uri must be a gnomevfs.URI or a string");
	gnome_vfs_file_info_unref(finfo);
	return NULL;
    }

    if (pygnome_vfs_result_check(result)) {
	gnome_vfs_file_info_unref(finfo);
	return NULL;
    }
    return pygnome_vfs_file_info_new(finfo);
}

static PyObject *
pygvfs_set_file_info(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "uri", "info", "mask", NULL };
    PyObject *uri;
    PyGnomeVFSFileInfo *finfo;
    GnomeVFSSetFileInfoMask mask = GNOME_VFS_SET_FILE_INFO_NONE;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "OO!i:gnomevfs.set_file_info",
				     kwlist, &uri,
                                     &PyGnomeVFSFileInfo_Type, &finfo,
                                     &mask))
	return NULL;

    if (PyObject_TypeCheck(uri, &PyGnomeVFSURI_Type)) {
        pyg_begin_allow_threads;
	result = gnome_vfs_set_file_info_uri(pygnome_vfs_uri_get(uri),
                                             finfo->finfo, mask);
        pyg_end_allow_threads;
    } else if (PyString_Check(uri)) {
        pyg_begin_allow_threads;
	result = gnome_vfs_set_file_info(PyString_AsString(uri),
                                         finfo->finfo, mask);
        pyg_end_allow_threads;
    } else {
	PyErr_SetString(PyExc_TypeError,
			"uri must be a gnomevfs.URI or a string");
	return NULL;
    }
    if (pygnome_vfs_result_check(result))
	return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_truncate(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "uri", "length", NULL };
    PyObject *uri, *py_length;
    GnomeVFSFileSize length;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO:gnomevfs.truncate",
				     kwlist, &uri, &py_length))
	return NULL;

    length = PyLong_Check(py_length) ? PyLong_AsUnsignedLongLong(py_length)
	: PyInt_AsLong(py_length);
    if (PyErr_Occurred()) return NULL;

    if (PyObject_TypeCheck(uri, &PyGnomeVFSURI_Type))
	result = gnome_vfs_truncate_uri(pygnome_vfs_uri_get(uri), length);
    else if (PyString_Check(uri))
	result = gnome_vfs_truncate(PyString_AsString(uri), length);
    else {
	PyErr_SetString(PyExc_TypeError,
			"uri must be a gnomevfs.URI or a string");
	return NULL;
    }

    if (pygnome_vfs_result_check(result))
        return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_create_symbolic_link(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "uri", "target", NULL };
    PyObject *uri;
    char *target;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
                                     "Os:gnomevfs.create_symbolic_link", kwlist,
                                     &uri, &target))
        return NULL;
    
    if (!PyObject_TypeCheck(uri, &PyGnomeVFSURI_Type)) {
        PyErr_SetString(PyExc_TypeError, "uri must be a gnomevfs.URI");
        return NULL;
    }

    pyg_begin_allow_threads;
    result = gnome_vfs_create_symbolic_link(pygnome_vfs_uri_get(uri), target);
    pyg_end_allow_threads;
        
    if (pygnome_vfs_result_check(result))
        return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_make_directory(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "uri", "perm", NULL };
    PyObject *uri;
    gint perm;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "Oi:gnomevfs.make_directory", kwlist,
				     &uri, &perm))
	return NULL;

    if (PyObject_TypeCheck(uri, &PyGnomeVFSURI_Type)) {
        pyg_begin_allow_threads;
	result = gnome_vfs_make_directory_for_uri(pygnome_vfs_uri_get(uri),
						  perm);
        pyg_end_allow_threads;
    } else if (PyString_Check(uri)) {
        pyg_begin_allow_threads;
	result = gnome_vfs_make_directory(PyString_AsString(uri), perm);
        pyg_end_allow_threads;
    } else {
	PyErr_SetString(PyExc_TypeError,
			"uri must be a gnomevfs.URI or a string");
	return NULL;
    }

    if (pygnome_vfs_result_check(result))
	return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_remove_directory(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "uri", NULL };
    PyObject *uri;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "O:gnomevfs.remove_directory", kwlist,
				     &uri))
	return NULL;

    if (PyObject_TypeCheck(uri, &PyGnomeVFSURI_Type)) {
        pyg_begin_allow_threads;
	result = gnome_vfs_remove_directory_from_uri(pygnome_vfs_uri_get(uri));
        pyg_end_allow_threads;
    } else if (PyString_Check(uri)) {
        pyg_begin_allow_threads;
	result = gnome_vfs_remove_directory(PyString_AsString(uri));
        pyg_end_allow_threads;
    } else {
	PyErr_SetString(PyExc_TypeError,
			"uri must be a gnomevfs.URI or a string");
	return NULL;
    }

    if (pygnome_vfs_result_check(result))
	return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_unlink(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "uri", NULL };
    PyObject *uri;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "O:gnomevfs.unlink", kwlist, &uri))
	return NULL;

    if (PyObject_TypeCheck(uri, &PyGnomeVFSURI_Type)) {
        pyg_begin_allow_threads;
	result = gnome_vfs_unlink_from_uri(pygnome_vfs_uri_get(uri));
        pyg_end_allow_threads;
    } else if (PyString_Check(uri)) {
        pyg_begin_allow_threads;
	result = gnome_vfs_unlink(PyString_AsString(uri));
        pyg_end_allow_threads;
    } else {
	PyErr_SetString(PyExc_TypeError,
			"uri must be a gnomevfs.URI or a string");
	return NULL;
    }

    if (pygnome_vfs_result_check(result))
	return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_exists(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "uri", NULL };
    PyObject *py_uri;
    GnomeVFSURI *uri = NULL;
    gboolean exists;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O:gnomevfs.exists",
				     kwlist, &py_uri))
	return NULL;

    if (PyObject_TypeCheck(py_uri, &PyGnomeVFSURI_Type)) {
        pyg_begin_allow_threads;
	uri = gnome_vfs_uri_ref(pygnome_vfs_uri_get(py_uri));
        pyg_end_allow_threads;
    } else if (PyString_Check(py_uri)) {
        pyg_begin_allow_threads;
	uri = gnome_vfs_uri_new(PyString_AsString(py_uri));
        pyg_end_allow_threads;
    }
    if (!uri) {
	PyErr_SetString(PyExc_TypeError,
			"uri must be a gnomevfs.URI or a string");
	return NULL;
    }
    exists = gnome_vfs_uri_exists(uri);
    gnome_vfs_uri_unref(uri);

    return PyInt_FromLong(exists);
}

static PyObject *
pygvfs_get_file_mime_type(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"path", "fast", "suffix_only", NULL };
    const char *path;
    const char *mime;
    gboolean fast = TRUE;
    gboolean suffix = FALSE;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
                                    "s|ii:gnomevfs.get_file_mime_type",
                                    kwlist,
                                    &path, &fast, &suffix))
    return NULL;

    if (fast)
        mime = gnome_vfs_get_file_mime_type_fast(path, NULL);
    else
        mime = gnome_vfs_get_file_mime_type(path, NULL, suffix);

    if (mime == NULL) {
        Py_XINCREF(Py_None);
        return Py_None;
    }

    return PyString_FromString(mime);
}

static PyObject *
pygvfs_get_mime_type(PyObject *self, PyObject *args)
{
    char *text_uri, *mime;
    
    if(!PyArg_ParseTuple(args, "s:gnomevfs.get_mime_type",
			 &text_uri))
	return NULL;

    pyg_begin_allow_threads;
    mime = gnome_vfs_get_mime_type(text_uri);
    pyg_end_allow_threads;
    if (mime)
	return PyString_FromString(mime);
    else {
	PyErr_SetString(PyExc_RuntimeError,
			"there was an error reading the file");
	return NULL;
    }
}

static PyObject *
pygvfs_get_mime_type_for_data(PyObject *self, PyObject *args)
{
    char *data;
    char const *mime;
    Py_ssize_t data_size, data_size1 = PY_SSIZE_T_MIN;
    
    if(!PyArg_ParseTuple(args, "s#|i:gnomevfs.get_mime_type_for_data",
			 &data, &data_size, &data_size1))
	return NULL;
    if (data_size1 != PY_SSIZE_T_MIN)
	PyErr_Warn(PyExc_DeprecationWarning, "ignoring deprecated argument data_size");
    pyg_begin_allow_threads;
    mime = gnome_vfs_get_mime_type_for_data(data, data_size);
    pyg_end_allow_threads;
    if (mime)
	return PyString_FromString(mime);
    else {
	PyErr_SetString(PyExc_RuntimeError,
			"there was an error reading the file");
	return NULL;
    }
}

static PyObject *
pygvfs_mime_get_icon(PyObject *self, PyObject *args)
{
    char *mime_type;
    const char *retval;
        
    if(!PyArg_ParseTuple(args, "s:gnomevfs.mime_get_icon",
			 &mime_type))
	return NULL;
        
    retval = gnome_vfs_mime_get_icon(mime_type);
    if (retval == NULL) {
            Py_INCREF(Py_None);
            return Py_None;
    }
    return PyString_FromString(retval);
}

static PyObject *
pygvfs_mime_set_icon(PyObject *self, PyObject *args)
{
    char *mime_type, *filename;
    GnomeVFSResult result;
        
    if(!PyArg_ParseTuple(args, "ss:gnomevfs.mime_set_icon",
			 &mime_type, &filename))
	return NULL;

    result = gnome_vfs_mime_set_icon(mime_type, filename);
    if (pygnome_vfs_result_check(result)) {
	return NULL;
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_get_description(PyObject *self, PyObject *args)
{
    char *mime_type;
    const char *description;
    
    if(!PyArg_ParseTuple(args, "s:gnomevfs.mime_get_description",
			 &mime_type))
	return NULL;
    description = gnome_vfs_mime_get_description(mime_type);
    if (description)
	return PyString_FromString(description);
    else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

static PyObject *
pygvfs_mime_set_description(PyObject *self, PyObject *args)
{
    char *mime_type, *description;
    GnomeVFSResult result;
        
    if(!PyArg_ParseTuple(args, "ss:gnomevfs.mime_set_description",
			 &mime_type, &description))
	return NULL;

    result = gnome_vfs_mime_set_description(mime_type, description);
    if (pygnome_vfs_result_check(result)) {
	return NULL;
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_can_be_executable(PyObject *self, PyObject *args)
{
    char *mime_type;
    
    if(!PyArg_ParseTuple(args, "s:gnomevfs.mime_can_be_executable",
			 &mime_type))
	return NULL;

    return PyInt_FromLong(gnome_vfs_mime_can_be_executable(mime_type));
}

static PyObject *
pygvfs_mime_set_can_be_executable(PyObject *self, PyObject *args)
{
    char *mime_type;
    gboolean new_value;
    GnomeVFSResult result;
        
    if(!PyArg_ParseTuple(args, "si:gnomevfs.mime_set_description",
			 &mime_type, &new_value))
	return NULL;

    result = gnome_vfs_mime_set_can_be_executable(mime_type, new_value);
    if (pygnome_vfs_result_check(result)) {
	return NULL;
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

static void
pygvfs_monitor_marshal(GnomeVFSMonitorHandle *handle,
		       const gchar *monitor_uri,
		       const gchar *info_uri,
		       GnomeVFSMonitorEventType event_type,
		       PyGVFSCustomNotify *cunote)
{
    PyObject *retobj;
    PyGILState_STATE state;
    
    state = pyg_gil_state_ensure();

    if (cunote->data)
	retobj = PyEval_CallFunction(cunote->func, "(ssiO)", monitor_uri,
				     info_uri, event_type, cunote->data);
    else
	retobj = PyObject_CallFunction(cunote->func, "(ssi)", monitor_uri,
				       info_uri, event_type);

    if (retobj == NULL) {
	PyErr_Print();
	PyErr_Clear();
    }
    
    Py_XDECREF(retobj);
    
    pyg_gil_state_release(state);
}

static PyObject*
pygvfs_monitor_add(PyObject *self, PyObject *args)
{
    char *text_uri;
    int monitor_type;
    PyObject *callback;
    PyObject *extra = NULL;
    PyGVFSCustomNotify *cunote;
    GnomeVFSMonitorHandle *handle;
    GnomeVFSResult result;
    gint monitor_id;
    
    if (!PyArg_ParseTuple(args, "siO|O:gnomevfs.monitor_add",
			  &text_uri, &monitor_type,
			  &callback, &extra)) {
	return NULL;
    }
    
    if (!PyCallable_Check(callback)) {
        PyErr_SetString(PyExc_TypeError, "third argument not callable");
        return NULL;
    }
    
    cunote = g_new0(PyGVFSCustomNotify, 1);
    cunote->func = callback;
    cunote->data = extra;
    Py_INCREF(cunote->func);
    Py_XINCREF(cunote->data);
    pyg_begin_allow_threads;
    result = gnome_vfs_monitor_add(&handle, text_uri, monitor_type,
		   (GnomeVFSMonitorCallback)pygvfs_monitor_marshal,
		   cunote);
    pyg_end_allow_threads;

    if (pygnome_vfs_result_check(result)) {
	return NULL;
    }

    do
        monitor_id = ++monitor_id_counter;
    while (g_hash_table_lookup(monitor_hash, GINT_TO_POINTER(monitor_id)));

    g_hash_table_insert(monitor_hash,
			GINT_TO_POINTER(monitor_id),
			handle);
    
    return PyInt_FromLong(monitor_id);
}

static PyObject*
pygvfs_monitor_cancel(PyObject *self, PyObject *args)
{
    gint monitor_id;
    GnomeVFSMonitorHandle *handle;
    
    if (!PyArg_ParseTuple(args, "i:gnomevfs.monitor_cancel",
			  &monitor_id)) {
	return NULL;
    }
    
    handle = g_hash_table_lookup(monitor_hash, GINT_TO_POINTER(monitor_id));
    if (handle == NULL) {
	PyErr_SetString(PyExc_ValueError, "Invalid monitor id");
	return NULL;
    }

    gnome_vfs_monitor_cancel(handle);
    g_hash_table_remove(monitor_hash, GINT_TO_POINTER(monitor_id));
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_read_entire_file(PyObject *self, PyObject *args)
{
    GnomeVFSResult  result;
    char           *uri;
    char           *file_contents;
    int             file_size;
    PyObject       *rv;

    if (!PyArg_ParseTuple(args, "s:gnomevfs.read_entire_file", &uri))
	return NULL;

    pyg_begin_allow_threads;
    result = gnome_vfs_read_entire_file (uri, &file_size, &file_contents);
    pyg_end_allow_threads;

    if (pygnome_vfs_result_check(result))
	return NULL;
    rv = PyString_FromStringAndSize(file_contents, file_size);
    g_free(file_contents);
    return rv;
}

static PyObject *
pygvfs_mime_application_new(GnomeVFSMimeApplication *mimeapp)
{
    PyObject *uri_schemes;
    GList *l;
    int i;

    uri_schemes = PyList_New(g_list_length(mimeapp->supported_uri_schemes));
    for (i = 0, l = mimeapp->supported_uri_schemes; l; ++i, l = l->next)
	PyList_SET_ITEM(uri_schemes, i, PyString_FromString((const char *) l->data));

    return Py_BuildValue("sssOiNO", mimeapp->id, mimeapp->name,
			 mimeapp->command,
			 mimeapp->can_open_multiple_files? Py_True : Py_False, 
			 mimeapp->expects_uris,
			 uri_schemes,
			 mimeapp->requires_terminal? Py_True : Py_False);
}

static PyObject *
pygvfs_mime_get_default_application(PyObject *self, PyObject *args)
{
    const char *mime_type;
    GnomeVFSMimeApplication *mimeapp;
    PyObject *retval;

    if(!PyArg_ParseTuple(args, "s:gnomevfs.mime_get_default_application",
			 &mime_type))
	return NULL;
    if (!(mimeapp = gnome_vfs_mime_get_default_application(mime_type))) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    retval = pygvfs_mime_application_new(mimeapp);
    gnome_vfs_mime_application_free(mimeapp);
    return retval;
}


gint
pygvfs_xfer_progress_callback(GnomeVFSXferProgressInfo *info, gpointer _data)
{
    PyGVFSCustomNotify *data = _data;
    PyObject *py_info, *callback_return;
    gint retval;
    PyGILState_STATE state;

    state = pyg_gil_state_ensure();
    py_info = pygnome_vfs_xfer_progress_info_new(info);
    if (data->data)
	callback_return = PyObject_CallFunction(data->func, "OO", py_info, data->data);
    else
	callback_return = PyObject_CallFunction(data->func, "O", py_info);

      /* because the programmer may keep a reference to the
       * VFSXferProgressInfo python wrapper but we don't own the
       * VFSXferProgressInfo itself, we remove the link from the
       * python wrapper to the C structure */
    pygnome_vfs_xfer_progress_info_set(py_info, NULL);

    Py_DECREF(py_info);

    if (callback_return == NULL) {
        PyErr_Print();
        pyg_gil_state_release(state);
	return GNOME_VFS_XFER_ERROR_ACTION_ABORT;
    }
    if (!PyInt_Check(callback_return)) {
	PyErr_SetString(PyExc_TypeError, "progress callback must return an int");
        PyErr_Print();
        pyg_gil_state_release(state);
	return GNOME_VFS_XFER_ERROR_ACTION_ABORT;
    }
    retval = PyInt_AsLong(callback_return);
    Py_DECREF(callback_return);
    pyg_gil_state_release(state);
    return retval;
}

static PyObject *
pygvfs_xfer_uri(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "source_uri", "target_uri", "xfer_options",
			      "error_mode", "overwrite_mode",
			      "progress_callback", "data", NULL };
    PyObject *source_uri, *target_uri;
    int xfer_options = -1, error_mode = -1, overwrite_mode = -1;
    PyGVFSCustomNotify custom_data = {NULL, NULL};
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!O!iii|OO:gnomevfs.xfer_uri",
				     kwlist,
				     &PyGnomeVFSURI_Type, &source_uri,
				     &PyGnomeVFSURI_Type, &target_uri,
				     &xfer_options, &error_mode, &overwrite_mode,
				     &custom_data.func, &custom_data.data))
	return NULL;

    if (custom_data.func == Py_None)
        custom_data.func = NULL;
    if (!custom_data.func) {
        if (error_mode == GNOME_VFS_XFER_ERROR_MODE_QUERY) {
            PyErr_SetString(PyExc_ValueError, "callback is required with QUERY error mode");
            return NULL;
        }
    } else if (!PyCallable_Check(custom_data.func)) {
	PyErr_SetString(PyExc_TypeError, "progress_callback must be callable");
	return NULL;
    }

    pyg_begin_allow_threads;
    result = gnome_vfs_xfer_uri(pygnome_vfs_uri_get(source_uri),
				pygnome_vfs_uri_get(target_uri),
				xfer_options,  error_mode, overwrite_mode,
				(custom_data.func?
                                 pygvfs_xfer_progress_callback : NULL),
                                &custom_data);
    pyg_end_allow_threads;
    if (pygnome_vfs_result_check(result))
	return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}

gboolean
_pygvfs_uri_sequence_to_glist(PyObject *seq, GList **list)
{
    int len, i;
    PyObject *item;

    if (!PySequence_Check(seq))
	return FALSE;
    *list = NULL;
    len = PySequence_Length(seq);
    for (i = 0; i < len; ++i) {
	item = PySequence_GetItem(seq, i);
	if (!PyObject_TypeCheck(item, &PyGnomeVFSURI_Type)) {
	    Py_DECREF(item);
	    if (*list)
		g_list_free(*list);
	    return FALSE;
	}
	*list = g_list_append(*list, pygnome_vfs_uri_get(item));
	Py_DECREF(item);
    }
    return TRUE;
}

static PyObject *
pygvfs_xfer_uri_list(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "source_uri_list", "target_uri_list", "xfer_options",
			      "error_mode", "overwrite_mode",
			      "progress_callback", "data", NULL };
    PyObject *py_source_uri_list, *py_target_uri_list;
    GList *source_uri_list = NULL, *target_uri_list = NULL;
    int xfer_options = -1, error_mode = -1, overwrite_mode = -1;
    PyGVFSCustomNotify custom_data = {NULL, NULL};
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OOiii|OO:gnomevfs.xfer_uri_list",
				     kwlist,
				     &py_source_uri_list, &py_target_uri_list,
				     &xfer_options, &error_mode, &overwrite_mode,
				     &custom_data.func, &custom_data.data))
	return NULL;

    if (!_pygvfs_uri_sequence_to_glist(py_source_uri_list, &source_uri_list)) {
	PyErr_SetString(PyExc_TypeError, "source_uri_list "
			" must be a sequence of gnomevfs.URI");
	return NULL;
    }
    if (!_pygvfs_uri_sequence_to_glist(py_target_uri_list, &target_uri_list)) {
	PyErr_SetString(PyExc_TypeError, "target_uri_list "
			" must be a sequence of gnomevfs.URI");
	g_list_free(source_uri_list);
	return NULL;
    }

    if (custom_data.func == Py_None)
        custom_data.func = NULL;
    if (!custom_data.func) {
        if (error_mode == GNOME_VFS_XFER_ERROR_MODE_QUERY) {
            PyErr_SetString(PyExc_ValueError, "callback is required with QUERY error mode");
            return NULL;
        }
    } else if (!PyCallable_Check(custom_data.func)) {
	PyErr_SetString(PyExc_TypeError, "progress_callback must be callable");
	g_list_free(source_uri_list);
	g_list_free(target_uri_list);
	return NULL;
    }

    pyg_begin_allow_threads;
    result = gnome_vfs_xfer_uri_list(source_uri_list, target_uri_list,
				     xfer_options,  error_mode, overwrite_mode,
				     (custom_data.func?
                                      pygvfs_xfer_progress_callback : NULL),
                                     &custom_data);
    pyg_end_allow_threads;

    g_list_free(source_uri_list);
    g_list_free(target_uri_list);
    if (pygnome_vfs_result_check(result))
	return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_xfer_delete_list(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "delete_uri_list",
			      "xfer_options", "error_mode",
			      "progress_callback", "data", NULL };
    PyObject *py_delete_uri_list;
    GList *delete_uri_list = NULL;
    int xfer_options = -1, error_mode = -1;
    PyGVFSCustomNotify custom_data = {NULL, NULL};
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "Oii|OO:gnomevfs.xfer_delete_list",
				     kwlist,
				     &py_delete_uri_list,
				     &error_mode, &xfer_options,
				     &custom_data.func, &custom_data.data))
	return NULL;

    if (!_pygvfs_uri_sequence_to_glist(py_delete_uri_list, &delete_uri_list)) {
	PyErr_SetString(PyExc_TypeError, "delete_uri_list "
			" must be a sequence of gnomevfs.URI");
	return NULL;
    }

    if (custom_data.func == Py_None)
        custom_data.func = NULL;
    if (!custom_data.func) {
        if (error_mode == GNOME_VFS_XFER_ERROR_MODE_QUERY) {
            PyErr_SetString(PyExc_ValueError, "callback is required with QUERY error mode");
            return NULL;
        }
    } else if (!PyCallable_Check(custom_data.func)) {
	PyErr_SetString(PyExc_TypeError, "progress_callback must be callable");
	g_list_free(delete_uri_list);
	return NULL;
    }

    pyg_begin_allow_threads;
    result = gnome_vfs_xfer_delete_list(delete_uri_list,
					error_mode, xfer_options,
					(custom_data.func?
                                         pygvfs_xfer_progress_callback : NULL),
                                        &custom_data);
    pyg_end_allow_threads;

    g_list_free(delete_uri_list);
    if (pygnome_vfs_result_check(result))
	return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_get_default_action_type(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", NULL };
    char *mime_type;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s:gnomevfs.mime_get_default_action_type",
				     kwlist, &mime_type))
	return NULL;
    return PyInt_FromLong(gnome_vfs_mime_get_default_action_type(mime_type));
}

static PyObject *
pygvfs_mime_action_new(GnomeVFSMimeAction *action)
{
    switch (action->action_type)
    {
    case GNOME_VFS_MIME_ACTION_TYPE_NONE:
	return Py_BuildValue("(iO)", action->action_type, Py_None);

    case GNOME_VFS_MIME_ACTION_TYPE_APPLICATION:
	return Py_BuildValue("(iN)", action->action_type,
			     pygvfs_mime_application_new(action->action.application));

    case GNOME_VFS_MIME_ACTION_TYPE_COMPONENT: {
        pygvfs_lazy_load_pygnomevfsbonobo();
        return pygnomevfs_bonobo_mime_component_action_new(action);
    }
    default:
	PyErr_SetString(PyExc_ValueError, "unknown action type returned");
	return NULL;
    }
}

static PyObject *
pygvfs_mime_get_default_action(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", NULL };
    char *mime_type;
    GnomeVFSMimeAction *action;
    PyObject *retval;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s:gnomevfs.mime_get_default_action",
				     kwlist, &mime_type))
	return NULL;
    if (!(action = gnome_vfs_mime_get_default_action(mime_type))) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    retval = pygvfs_mime_action_new(action);
    gnome_vfs_mime_action_free(action);
    return retval;
}


static PyObject *
pygvfs_mime_applications_list_new(GList *list)
{
    PyObject *retval;
    PyObject *py_app;
    guint i, len = g_list_length(list);
    
    retval = PyList_New(len);
    for (i = 0; list; ++i, list = list->next) {
	g_assert(i < len);
	py_app = pygvfs_mime_application_new((GnomeVFSMimeApplication *) list->data);
	PyList_SET_ITEM(retval, i, py_app);
    }
    return retval;
}

static PyObject *
pygvfs_mime_get_short_list_applications(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", NULL };
    char *mime_type;
    GList *list;
    PyObject *py_list;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "s:gnomevfs.mime_get_short_list_applications",
				     kwlist, &mime_type))
	return NULL;
    list = gnome_vfs_mime_get_short_list_applications(mime_type);
    py_list = pygvfs_mime_applications_list_new(list);
    gnome_vfs_mime_application_list_free(list);
    return py_list;
}

static PyObject *
pygvfs_mime_get_all_applications(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", NULL };
    char *mime_type;
    GList *list;
    PyObject *py_list;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "s:gnomevfs.mime_get_all_applications",
				     kwlist, &mime_type))
	return NULL;
    list = gnome_vfs_mime_get_all_applications(mime_type);
    py_list = pygvfs_mime_applications_list_new(list);
    gnome_vfs_mime_application_list_free(list);
    return py_list;
}

static PyObject *
pygvfs_mime_set_default_action_type(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "action_type", NULL };
    char *mime_type;
    GnomeVFSMimeActionType action_type;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "si:gnomevfs.mime_set_default_action_type",
				     kwlist, &mime_type, &action_type))
	return NULL;
    result = gnome_vfs_mime_set_default_action_type(mime_type, action_type);
    if (pygnome_vfs_result_check(result))
	return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_set_default_application(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "application_id", NULL };
    char *mime_type, *application_id;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "ss:gnomevfs.mime_set_default_application",
				     kwlist, &mime_type, &application_id))
	return NULL;
    result = gnome_vfs_mime_set_default_application(mime_type, application_id);
    if (pygnome_vfs_result_check(result))
	return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_set_default_component(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "component_iid", NULL };
    char *mime_type, *component_iid;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "ss:gnomevfs.mime_set_default_component",
				     kwlist, &mime_type, &component_iid))
	return NULL;
    result = gnome_vfs_mime_set_default_component(mime_type, component_iid);
    if (pygnome_vfs_result_check(result))
	return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static int
string_list_converter(PyObject *in, void *out)
{
    guint len, i;
    GList *list = NULL;
    PyObject *item;

    if (!PySequence_Check(in)) {
	PyErr_SetString(PyExc_TypeError, "argument must be a sequence");
	return 0;
    }
    len = PySequence_Length(in);
    for (i = 0; i < len; ++i) {
	item = PySequence_GetItem(in, i);
	if (!PyString_Check(item)) {
	    Py_DECREF(item);
	    g_list_free(list);
	    return 0;
	}
	list = g_list_append(list, PyString_AsString(item));
	Py_DECREF(item);
    }
    *((GList **) out) = list;
    return 1;
}

static PyObject *
pygvfs_mime_set_short_list_applications(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "application_ids", NULL };
    char *mime_type;
    GList *application_ids;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "sO&:gnomevfs.mime_set_short_list_applications",
				     kwlist, &mime_type,
				     string_list_converter, &application_ids))
	return NULL;
    result = gnome_vfs_mime_set_short_list_applications(mime_type, application_ids);
    g_list_free(application_ids);
    if (pygnome_vfs_result_check(result))
	return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_set_short_list_components(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "component_iids", NULL };
    char *mime_type;
    GList *component_iids;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "sO&:gnomevfs.mime_set_short_list_components",
				     kwlist, &mime_type,
				     string_list_converter, &component_iids))
	return NULL;
    result = gnome_vfs_mime_set_short_list_components(mime_type, component_iids);
    g_list_free(component_iids);
    if (pygnome_vfs_result_check(result))
	return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *
pygvfs_mime_add_application_to_short_list(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "application_id", NULL };
    char *mime_type, *application_id;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "ss:gnomevfs.mime_add_application_to_short_list",
				     kwlist, &mime_type, &application_id))
	return NULL;
    result = gnome_vfs_mime_add_application_to_short_list(mime_type, application_id);
    if (pygnome_vfs_result_check(result))
	return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_remove_application_from_short_list(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "application_id", NULL };
    char *mime_type, *application_id;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "ss:gnomevfs.mime_remove_application_from_short_list",
				     kwlist, &mime_type, &application_id))
	return NULL;
    result = gnome_vfs_mime_remove_application_from_short_list(mime_type, application_id);
    if (pygnome_vfs_result_check(result))
	return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_add_component_to_short_list(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "component_iid", NULL };
    char *mime_type, *component_iid;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "ss:gnomevfs.mime_add_component_to_short_list",
				     kwlist, &mime_type, &component_iid))
	return NULL;
    result = gnome_vfs_mime_add_component_to_short_list(mime_type, component_iid);
    if (pygnome_vfs_result_check(result))
	return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_remove_component_from_short_list(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "component_iid", NULL };
    char *mime_type, *component_iid;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "ss:gnomevfs.mime_remove_component_from_short_list",
				     kwlist, &mime_type, &component_iid))
	return NULL;
    result = gnome_vfs_mime_remove_component_from_short_list(mime_type, component_iid);
    if (pygnome_vfs_result_check(result))
	return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_add_extension(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "extension", NULL };
    char *mime_type, *extension;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "ss:gnomevfs.mime_add_extension",
				     kwlist, &mime_type, &extension))
	return NULL;
    result = gnome_vfs_mime_add_extension(mime_type, extension);
    if (pygnome_vfs_result_check(result))
	return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_remove_extension(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "extension", NULL };
    char *mime_type, *extension;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "ss:gnomevfs.mime_remove_extension",
				     kwlist, &mime_type, &extension))
	return NULL;
    result = gnome_vfs_mime_remove_extension(mime_type, extension);
    if (pygnome_vfs_result_check(result))
	return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_extend_all_applications(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "application_ids", NULL };
    char *mime_type;
    GList *application_ids;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "sO&:gnomevfs.mime_extend_all_applications",
				     kwlist, &mime_type,
				     string_list_converter, &application_ids))
	return NULL;
    result = gnome_vfs_mime_extend_all_applications(mime_type, application_ids);
    g_list_free(application_ids);
    if (pygnome_vfs_result_check(result))
	return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_remove_from_all_applications(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "application_ids", NULL };
    char *mime_type;
    GList *application_ids;
    GnomeVFSResult result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "sO&:gnomevfs.mime_remove_from_all_applications",
				     kwlist, &mime_type,
				     string_list_converter, &application_ids))
	return NULL;
    result = gnome_vfs_mime_remove_from_all_applications(mime_type, application_ids);
    g_list_free(application_ids);
    if (pygnome_vfs_result_check(result))
	return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygvfs_mime_application_new_from_id(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "id", NULL };
    char *id;
    GnomeVFSMimeApplication *app;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "s:gnomevfs.mime_application_new_from_id",
				     kwlist, &id))
	return NULL;
    app = gnome_vfs_mime_application_new_from_id(id);

    if (!app) {
	  /* not sure if NULL is a valid return value, but we handle
	   * it just in case... */
	PyErr_SetString(PyExc_ValueError, "unknown application id");
	return NULL;
    }
    return pygvfs_mime_application_new(app);
}

static PyObject*
pyvfs_format_file_size_for_display(PyObject *self, PyObject *args)
{
    guint64 size;
    char *cstring;
    PyObject *string;

    if(!PyArg_ParseTuple(args, "K", &size))
	return NULL;

    cstring = gnome_vfs_format_file_size_for_display(size);
    string = PyString_FromString(cstring);
    g_free(cstring);

    return string;
} 



static PyObject *
pygvfs_resolve(PyObject *self, PyObject *args)
{
    char *hostname;
    PyObject *retval;
    PyObject *list;
    GnomeVFSResult res;
    GnomeVFSResolveHandle *handle;
    GnomeVFSAddress *address;

    if (!PyArg_ParseTuple(args, "s", &hostname))
	return NULL;

    pyg_begin_allow_threads;

    res = gnome_vfs_resolve(hostname, &handle);

    if (pygnome_vfs_result_check(res)) {
	retval = NULL;
	goto out;
    }

    list = PyList_New(0);

    while (gnome_vfs_resolve_next_address(handle, &address))
    {
	int type;
	char *str;
	PyObject *pair;

	type = gnome_vfs_address_get_family_type(address);
	str = gnome_vfs_address_to_string(address);

	pair = Py_BuildValue("(is)", type, str);
	g_free(str);

	PyList_Append(list, pair);
	Py_DECREF(pair);
    }

    gnome_vfs_resolve_free(handle);

    retval = list;

 out:
    pyg_end_allow_threads;
    return retval;
}


static PyObject *
pygvfs_connect_to_server(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "uri", "display_name", "icon", NULL };
    char *uri, *display_name, *icon;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "s:gnomevfs.connect_to_server",
				     kwlist, &uri, &display_name, &icon))
	return NULL;
    gnome_vfs_connect_to_server(uri, display_name, icon);
    Py_INCREF(Py_None);
    return Py_None;
}

#define pygvfs_generic_str_map_func(name, argname)              \
static PyObject *                                               \
pygvfs_##name(PyObject *self, PyObject *args, PyObject *kwargs) \
{                                                               \
    static char *kwlist[] = { #argname, NULL };                 \
    char *str;                                                  \
    PyObject *retval;                                           \
                                                                \
    if (!PyArg_ParseTupleAndKeywords(args, kwargs,              \
				     "s:gnomevfs."#name,        \
				     kwlist, &str))             \
	return NULL;                                            \
    str = gnome_vfs_##name(str);                                \
    if (!str) {                                                 \
        PyErr_SetString(PyExc_RuntimeError, "unknown error");   \
        return NULL;                                            \
    }                                                           \
    retval = PyString_FromString(str);                          \
    g_free(str);                                                \
    return retval;                                              \
}

pygvfs_generic_str_map_func(escape_string, string);
pygvfs_generic_str_map_func(escape_path_string, path);
pygvfs_generic_str_map_func(escape_host_and_path_string, string);
pygvfs_generic_str_map_func(escape_slashes, string);

static PyObject *
pygvfs_escape_set(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "string", "match_set", NULL };
    char *str, *str1;
    PyObject *retval;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "ss:gnomevfs.escape_set",
				     kwlist, &str, &str1))
	return NULL;
    str = gnome_vfs_escape_set(str, str1);
    if (!str) {
        PyErr_SetString(PyExc_RuntimeError, "unknown error");
        return NULL;
    }
    retval = PyString_FromString(str);
    g_free(str);
    return retval;
}

static PyObject *
pygvfs_unescape_string(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "escaped_string", "illegal_characters", NULL };
    char *str, *str1;
    PyObject *retval;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "ss:gnomevfs.unescape_string",
				     kwlist, &str, &str1))
	return NULL;
    str = gnome_vfs_escape_set(str, str1);
    if (!str) {
        PyErr_SetString(PyExc_RuntimeError, "unknown error");
        return NULL;
    }
    retval = PyString_FromString(str);
    g_free(str);
    return retval;
}

pygvfs_generic_str_map_func(make_uri_canonical, uri);
pygvfs_generic_str_map_func(make_path_name_canonical, path);
pygvfs_generic_str_map_func(unescape_string_for_display, escaped);
pygvfs_generic_str_map_func(get_local_path_from_uri, uri);
pygvfs_generic_str_map_func(get_uri_from_local_path, local_full_path);

static PyObject *
pygvfs_is_executable_command_string(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "command_string", NULL };
    char *str;
    gboolean retval;
    PyObject *py_retval;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "s:gnomevfs.is_executable_command_string",
				     kwlist, &str))
	return NULL;
    retval = gnome_vfs_is_executable_command_string(str);
    py_retval = retval? Py_True : Py_False;
    Py_INCREF(py_retval);
    return py_retval;
}

static PyObject *
pygvfs_get_volume_free_space(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "vfs_uri", NULL };
    PyObject *py_uri;
    GnomeVFSFileSize size = 0;
    GnomeVFSResult result;
    PyObject *py_retval;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "O!:gnomevfs.get_volume_free_space",
				     kwlist,
                                     &PyGnomeVFSURI_Type, &py_uri))
	return NULL;
    result = gnome_vfs_get_volume_free_space(pygnome_vfs_uri_get(py_uri), &size);
    if (pygnome_vfs_result_check(result))
        return NULL;
    py_retval = PyLong_FromUnsignedLongLong(size);
    return py_retval;
}

pygvfs_generic_str_map_func(icon_path_from_filename, filename);

static PyObject *
pygvfs_open_fd(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "filedes", NULL };
    int filedes;
    GnomeVFSResult result;
    PyObject *py_retval;
    GnomeVFSHandle *handle = NULL;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "i:gnomevfs.open_fd",
				     kwlist, &filedes))
	return NULL;
    result = gnome_vfs_open_fd(&handle, filedes);
    if (pygnome_vfs_result_check(result))
        return NULL;
    py_retval = pygnome_vfs_handle_new(handle);
    return py_retval;
}

static PyObject *
pygvfs_is_primary_thread(void)
{
    gboolean retval;
    PyObject *py_retval;

    retval = gnome_vfs_is_primary_thread();
    py_retval = retval? Py_True : Py_False;
    Py_INCREF(py_retval);
    return py_retval;
}

pygvfs_generic_str_map_func(format_uri_for_display, uri);
pygvfs_generic_str_map_func(make_uri_from_input, uri);

static PyObject *
pygvfs_make_uri_from_input_with_dirs(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "escaped_string", "illegal_characters", NULL };
    char *str;
    int dirs;
    PyObject *retval;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "si:gnomevfs.make_uri_from_input_with_dirs",
				     kwlist, &str, &dirs))
	return NULL;
    str = gnome_vfs_make_uri_from_input_with_dirs(str, dirs);
    if (!str) {
        PyErr_SetString(PyExc_RuntimeError, "unknown error");
        return NULL;
    }
    retval = PyString_FromString(str);
    g_free(str);
    return retval;
}

pygvfs_generic_str_map_func(make_uri_canonical_strip_fragment, uri);

static PyObject *
pygvfs_uris_match(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "uri_1", "uri_2", NULL };
    char *uri1, *uri2;
    gboolean retval;
    PyObject *py_retval;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "ss:gnomevfs.uris_match",
				     kwlist, &uri1, &uri2))
	return NULL;
    retval = gnome_vfs_uris_match(uri1, uri2);
    py_retval = retval? Py_True : Py_False;
    Py_INCREF(py_retval);
    return py_retval;
}

pygvfs_generic_str_map_func(get_uri_scheme, uri);
pygvfs_generic_str_map_func(make_uri_from_shell_arg, uri);

static PyObject *
pygvfs_url_show(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "url", "env", NULL };
    char *url, **env;
    GnomeVFSResult result;
    PyObject *py_env = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "s|O!:gnomevfs.url_show",
				     kwlist, &url, &PyList_Type, &py_env))
	return NULL;

    if (!py_env)
        env = NULL;
    else {
        int len = PyList_Size(py_env);
        int i;
        env = g_new(char *, len + 1);
        for (i = 0; i < len; ++i) {
            PyObject *item = PyList_GET_ITEM(py_env, i);
            if (!PyString_Check(item)) {
                PyErr_SetString(PyExc_TypeError, "second argument (env) "
                                "must be a list of strings");
                g_free(env);
                return NULL;
            }
            env[i] = PyString_AsString(item);
        }
        env[len] = NULL;
    }

    result = gnome_vfs_url_show_with_env(url, env);
    if (env) g_free(env);
    if (pygnome_vfs_result_check(result))
        return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}

static inline PyObject *
pygvfs_dns_service_new(GnomeVFSDNSSDService *service)
{
    return Py_BuildValue("sss", service->name, service->type, service->domain);
}

static PyObject *
_wrap_gnome_vfs_dns_sd_browse_sync(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "domain", "type", "timeout_msec", NULL };
    char *domain, *type;
    int timeout_msec;
    int n_services;
    GnomeVFSDNSSDService *services = NULL;
    GnomeVFSResult result;
    PyObject *py_services;
    int i;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "ssi:gnomevfs.dns_sd_browse_sync", kwlist,
                                     &domain, &type, &timeout_msec))
	return NULL;

    pyg_unblock_threads();
    result = gnome_vfs_dns_sd_browse_sync(domain, type, timeout_msec,
                                          &n_services, &services);
    pyg_block_threads();
    if (pygnome_vfs_result_check(result))
        return NULL;
    py_services = PyList_New(n_services);
    for (i = 0; i < n_services; ++i)
        PyList_SET_ITEM(py_services, i, pygvfs_dns_service_new(services + i));
    gnome_vfs_dns_sd_service_list_free(services, n_services);
    return py_services;
}

static void
__text_hash_to_dict(char *key, char *value, PyObject *dict)
{
    PyObject *pyval = PyString_FromString(value);
    PyDict_SetItemString(dict, key, pyval);
    Py_DECREF(pyval);
}

static PyObject *
_wrap_gnome_vfs_dns_sd_resolve_sync(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "name", "type", "domain", "timeout_msec", NULL };
    char *name, *domain, *type;
    int timeout_msec;
    GnomeVFSResult result;
    char *host, *text_raw;
    int port, text_raw_len;
    GHashTable *hash;
    PyObject *py_hash;
    PyObject *retval;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "sssi:gnomevfs.dns_sd_resolve_sync", kwlist,
                                     &name, &type, &domain, &timeout_msec))
	return NULL;

    pyg_unblock_threads();
    result = gnome_vfs_dns_sd_resolve_sync(name, type, domain, timeout_msec,
                                           &host, &port, &hash,
                                           &text_raw_len, &text_raw);
    pyg_block_threads();

    if (pygnome_vfs_result_check(result))
        return NULL;

    py_hash = PyDict_New();
    g_hash_table_foreach(hash, (GHFunc) __text_hash_to_dict, py_hash);
    g_hash_table_destroy(hash);
    retval = Py_BuildValue("Ns#", py_hash, text_raw, (Py_ssize_t) text_raw_len);
    g_free(text_raw);
    return retval;
}

static PyObject *
_wrap_gnome_vfs_dns_sd_list_browse_domains_sync(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "domain", "timeout_msec", NULL };
    char *domain;
    int timeout_msec;
    GnomeVFSResult result;
    GList *domains, *l;
    PyObject *retval;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "si:gnomevfs.dns_sd_list_browse_domains_sync", kwlist,
                                      &domain, &timeout_msec))
	return NULL;

    pyg_unblock_threads();
    result = gnome_vfs_dns_sd_list_browse_domains_sync(domain,
                                                       timeout_msec,
                                                       &domains);
    pyg_block_threads();
    if (pygnome_vfs_result_check(result))
        return NULL;
    retval = PyList_New(0);
    for (l = domains; l; l = l->next) {
        PyObject *item = PyString_FromString((char *) l->data);
        PyList_Append(retval, item);
        Py_DECREF(item);
        g_free(l->data);
    }
    g_list_free(domains);
    return retval;
}

static PyObject *
_wrap_gnome_vfs_get_default_browse_domains(PyObject *self, PyObject *args, PyObject *kwargs)
{
    GList *domains, *l;
    PyObject *retval;

    pyg_unblock_threads();
    domains = gnome_vfs_get_default_browse_domains();
    pyg_block_threads();

    retval = PyList_New(0);
    for (l = domains; l; l = l->next) {
        PyObject *item = PyString_FromString((char *) l->data);
        PyList_Append(retval, item);
        Py_DECREF(item);
        g_free(l->data);
    }
    g_list_free(domains);
    return retval;
}


static PyObject *
_wrap_gnome_vfs_mime_type_get_equivalence(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { "mime_type", "base_mime_type", NULL };
    char *mime_type, *base_mime_type;
    GnomeVFSMimeEquivalence result;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
				     "ss:gnomevfs.mime_type_get_equivalence", kwlist,
                                      &mime_type, &base_mime_type))
	return NULL;

    result = gnome_vfs_mime_type_get_equivalence(mime_type, base_mime_type);
    return PyInt_FromLong(result);
}


static PyMethodDef pygnomevfs_functions[] = {
    { "create", (PyCFunction)pygvfs_create, METH_VARARGS|METH_KEYWORDS },
    { "get_file_info", (PyCFunction)pygvfs_get_file_info,
      METH_VARARGS|METH_KEYWORDS },
    { "set_file_info", (PyCFunction)pygvfs_set_file_info,
      METH_VARARGS|METH_KEYWORDS },
    { "truncate", (PyCFunction)pygvfs_truncate, METH_VARARGS|METH_KEYWORDS },
    { "make_directory", (PyCFunction)pygvfs_make_directory,
      METH_VARARGS|METH_KEYWORDS },
    { "remove_directory", (PyCFunction)pygvfs_remove_directory,
      METH_VARARGS|METH_KEYWORDS },
    { "create_symbolic_link", (PyCFunction)pygvfs_create_symbolic_link,
      METH_VARARGS|METH_KEYWORDS },
    { "unlink", (PyCFunction)pygvfs_unlink, METH_VARARGS|METH_KEYWORDS },
    { "exists", (PyCFunction)pygvfs_exists, METH_VARARGS|METH_KEYWORDS },
    { "format_file_size_for_display", pyvfs_format_file_size_for_display, METH_VARARGS},
    { "get_file_mime_type", (PyCFunction) pygvfs_get_file_mime_type, METH_VARARGS|METH_KEYWORDS},
    { "get_mime_type", (PyCFunction)pygvfs_get_mime_type, METH_VARARGS },
    { "get_mime_type_for_data", (PyCFunction)pygvfs_get_mime_type_for_data,
      METH_VARARGS },
    { "mime_get_icon", (PyCFunction)pygvfs_mime_get_icon, METH_VARARGS },
    { "mime_set_icon", (PyCFunction)pygvfs_mime_set_icon, METH_VARARGS },
    { "mime_get_description", (PyCFunction)pygvfs_mime_get_description, 
      METH_VARARGS },
    { "mime_set_description", (PyCFunction)pygvfs_mime_set_description, 
      METH_VARARGS },
    { "mime_can_be_executable", (PyCFunction)pygvfs_mime_can_be_executable, 
      METH_VARARGS },
    { "mime_set_can_be_executable", (PyCFunction)pygvfs_mime_set_can_be_executable,
      METH_VARARGS },
    { "monitor_add", pygvfs_monitor_add, METH_VARARGS},
    { "monitor_cancel", pygvfs_monitor_cancel, METH_VARARGS},
    { "read_entire_file", pygvfs_read_entire_file, METH_VARARGS},
    { "mime_get_default_application", (PyCFunction)pygvfs_mime_get_default_application, 
      METH_VARARGS },
    { "xfer_uri", (PyCFunction)pygvfs_xfer_uri,  METH_VARARGS|METH_KEYWORDS },
    { "xfer_uri_list", (PyCFunction)pygvfs_xfer_uri_list,  METH_VARARGS|METH_KEYWORDS },
    { "xfer_delete_list", (PyCFunction)pygvfs_xfer_delete_list,  METH_VARARGS|METH_KEYWORDS },
    { "mime_get_default_action_type", (PyCFunction)pygvfs_mime_get_default_action_type,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_get_default_action", (PyCFunction)pygvfs_mime_get_default_action,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_get_short_list_applications", (PyCFunction)pygvfs_mime_get_short_list_applications,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_get_all_applications", (PyCFunction)pygvfs_mime_get_all_applications,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_set_default_action_type", (PyCFunction)pygvfs_mime_set_default_action_type,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_set_default_application", (PyCFunction)pygvfs_mime_set_default_application,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_set_default_component", (PyCFunction)pygvfs_mime_set_default_component,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_set_short_list_applications", (PyCFunction)pygvfs_mime_set_short_list_applications,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_set_short_list_components", (PyCFunction)pygvfs_mime_set_short_list_components,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_add_application_to_short_list", (PyCFunction)pygvfs_mime_add_application_to_short_list,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_remove_application_from_short_list",
      (PyCFunction)pygvfs_mime_remove_application_from_short_list, METH_VARARGS|METH_KEYWORDS },
    { "mime_add_component_to_short_list", (PyCFunction)pygvfs_mime_add_component_to_short_list,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_remove_component_from_short_list", (PyCFunction)pygvfs_mime_remove_component_from_short_list,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_add_extension", (PyCFunction)pygvfs_mime_add_extension,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_remove_extension", (PyCFunction)pygvfs_mime_remove_extension,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_extend_all_applications", (PyCFunction)pygvfs_mime_extend_all_applications,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_remove_from_all_applications", (PyCFunction)pygvfs_mime_remove_from_all_applications,
      METH_VARARGS|METH_KEYWORDS },
    { "mime_application_new_from_id", (PyCFunction)pygvfs_mime_application_new_from_id,
      METH_VARARGS|METH_KEYWORDS },
    { "connect_to_server", (PyCFunction)pygvfs_connect_to_server,
      METH_VARARGS|METH_KEYWORDS },
    { "escape_string", (PyCFunction)pygvfs_escape_string,
      METH_VARARGS|METH_KEYWORDS },
    { "escape_path_string", (PyCFunction)pygvfs_escape_path_string,
      METH_VARARGS|METH_KEYWORDS },
    { "escape_host_and_path_string", (PyCFunction)pygvfs_escape_host_and_path_string,
      METH_VARARGS|METH_KEYWORDS },
    { "escape_slashes", (PyCFunction)pygvfs_escape_slashes,
      METH_VARARGS|METH_KEYWORDS },
    { "escape_set", (PyCFunction)pygvfs_escape_set,
      METH_VARARGS|METH_KEYWORDS },
    { "unescape_string", (PyCFunction)pygvfs_unescape_string,
      METH_VARARGS|METH_KEYWORDS },
    { "make_uri_canonical", (PyCFunction)pygvfs_make_uri_canonical,
      METH_VARARGS|METH_KEYWORDS },
    { "make_path_name_canonical", (PyCFunction)pygvfs_make_path_name_canonical,
      METH_VARARGS|METH_KEYWORDS },
    { "unescape_string_for_display", (PyCFunction)pygvfs_unescape_string_for_display,
      METH_VARARGS|METH_KEYWORDS },
    { "get_local_path_from_uri", (PyCFunction)pygvfs_get_local_path_from_uri,
      METH_VARARGS|METH_KEYWORDS },
    { "get_uri_from_local_path", (PyCFunction)pygvfs_get_uri_from_local_path,
      METH_VARARGS|METH_KEYWORDS },
    { "is_executable_command_string", (PyCFunction)pygvfs_is_executable_command_string,
      METH_VARARGS|METH_KEYWORDS },
    { "get_volume_free_space", (PyCFunction)pygvfs_get_volume_free_space,
      METH_VARARGS|METH_KEYWORDS },
    { "icon_path_from_filename", (PyCFunction)pygvfs_icon_path_from_filename,
      METH_VARARGS|METH_KEYWORDS },
    { "open_fd", (PyCFunction)pygvfs_open_fd,
      METH_VARARGS|METH_KEYWORDS },
    { "is_primary_thread", (PyCFunction)pygvfs_is_primary_thread,
      METH_NOARGS },
    { "format_uri_for_display", (PyCFunction)pygvfs_format_uri_for_display,
      METH_VARARGS|METH_KEYWORDS },
    { "make_uri_from_input", (PyCFunction)pygvfs_make_uri_from_input,
      METH_VARARGS|METH_KEYWORDS },
    { "make_uri_from_input_with_dirs", (PyCFunction)pygvfs_make_uri_from_input_with_dirs,
      METH_VARARGS|METH_KEYWORDS },
    { "make_uri_canonical_strip_fragment", (PyCFunction)pygvfs_make_uri_canonical_strip_fragment,
      METH_VARARGS|METH_KEYWORDS },
    { "uris_match", (PyCFunction)pygvfs_uris_match,
      METH_VARARGS|METH_KEYWORDS },
    { "get_uri_scheme", (PyCFunction)pygvfs_get_uri_scheme,
      METH_VARARGS|METH_KEYWORDS },
    { "make_uri_from_shell_arg", (PyCFunction)pygvfs_make_uri_from_shell_arg,
      METH_VARARGS|METH_KEYWORDS },
    { "url_show", (PyCFunction)pygvfs_url_show,
      METH_VARARGS|METH_KEYWORDS },
    { "resolve", pygvfs_resolve, METH_VARARGS},
    { "dns_sd_browse_sync", (PyCFunction)_wrap_gnome_vfs_dns_sd_browse_sync,
      METH_VARARGS|METH_KEYWORDS},
    { "dns_sd_resolve_sync", (PyCFunction)_wrap_gnome_vfs_dns_sd_resolve_sync,
      METH_VARARGS|METH_KEYWORDS},
    { "dns_sd_list_browse_domains_sync",
      (PyCFunction)_wrap_gnome_vfs_dns_sd_list_browse_domains_sync,
      METH_VARARGS|METH_KEYWORDS},
    { "get_default_browse_domains",
      (PyCFunction)_wrap_gnome_vfs_get_default_browse_domains, METH_NOARGS},
    { "mime_type_get_equivalence",
      (PyCFunction)_wrap_gnome_vfs_mime_type_get_equivalence,
      METH_VARARGS|METH_KEYWORDS},

    { NULL, NULL, 0 }
    
};

static void
register_constants(PyObject *module)
{
#define regconst(x) PyModule_AddIntConstant(module, #x, GNOME_VFS_ ## x)
    regconst(FILE_FLAGS_NONE);
    regconst(FILE_FLAGS_SYMLINK);
    regconst(FILE_FLAGS_LOCAL);
    regconst(FILE_TYPE_UNKNOWN);
    regconst(FILE_TYPE_REGULAR);
    regconst(FILE_TYPE_DIRECTORY);
    regconst(FILE_TYPE_FIFO);
    regconst(FILE_TYPE_SOCKET);
    regconst(FILE_TYPE_CHARACTER_DEVICE);
    regconst(FILE_TYPE_BLOCK_DEVICE);
    regconst(FILE_TYPE_SYMBOLIC_LINK);
    regconst(FILE_INFO_FIELDS_NONE);
    regconst(FILE_INFO_FIELDS_TYPE);
    regconst(FILE_INFO_FIELDS_PERMISSIONS);
    regconst(FILE_INFO_FIELDS_FLAGS);
    regconst(FILE_INFO_FIELDS_DEVICE);
    regconst(FILE_INFO_FIELDS_INODE);
    regconst(FILE_INFO_FIELDS_LINK_COUNT);
    regconst(FILE_INFO_FIELDS_SIZE);
    regconst(FILE_INFO_FIELDS_BLOCK_COUNT);
    regconst(FILE_INFO_FIELDS_IO_BLOCK_SIZE);
    regconst(FILE_INFO_FIELDS_ATIME);
    regconst(FILE_INFO_FIELDS_MTIME);
    regconst(FILE_INFO_FIELDS_CTIME);
    regconst(FILE_INFO_FIELDS_SYMLINK_NAME);
    regconst(FILE_INFO_FIELDS_MIME_TYPE);
    regconst(FILE_INFO_FIELDS_ACCESS);
    regconst(FILE_INFO_FIELDS_IDS);
    regconst(PERM_SUID);
    regconst(PERM_SGID);
    regconst(PERM_STICKY);
    regconst(PERM_USER_READ);
    regconst(PERM_USER_WRITE);
    regconst(PERM_USER_EXEC);
    regconst(PERM_USER_ALL);
    regconst(PERM_GROUP_READ);
    regconst(PERM_GROUP_WRITE);
    regconst(PERM_GROUP_EXEC);
    regconst(PERM_GROUP_ALL);
    regconst(PERM_OTHER_READ);
    regconst(PERM_OTHER_WRITE);
    regconst(PERM_OTHER_EXEC);
    regconst(PERM_OTHER_ALL);
    regconst(FILE_INFO_DEFAULT);
    regconst(FILE_INFO_GET_MIME_TYPE);
    regconst(FILE_INFO_FORCE_FAST_MIME_TYPE);
    regconst(FILE_INFO_FORCE_SLOW_MIME_TYPE);
    regconst(FILE_INFO_FOLLOW_LINKS);
    regconst(FILE_INFO_GET_ACCESS_RIGHTS);
    regconst(FILE_INFO_NAME_ONLY);
    regconst(SET_FILE_INFO_NONE);
    regconst(SET_FILE_INFO_NAME);
    regconst(SET_FILE_INFO_PERMISSIONS);
    regconst(SET_FILE_INFO_OWNER);
    regconst(SET_FILE_INFO_TIME);
    regconst(DIRECTORY_VISIT_DEFAULT);
    regconst(DIRECTORY_VISIT_SAMEFS);
    regconst(DIRECTORY_VISIT_LOOPCHECK);
    regconst(OPEN_NONE);
    regconst(OPEN_READ);
    regconst(OPEN_WRITE);
    regconst(OPEN_RANDOM);
    regconst(OPEN_TRUNCATE);
    regconst(SEEK_START);
    regconst(SEEK_CURRENT);
    regconst(SEEK_END);
    regconst(MONITOR_FILE);
    regconst(MONITOR_DIRECTORY);
    regconst(MONITOR_EVENT_CHANGED);
    regconst(MONITOR_EVENT_DELETED);
    regconst(MONITOR_EVENT_STARTEXECUTING);
    regconst(MONITOR_EVENT_STOPEXECUTING);
    regconst(MONITOR_EVENT_CREATED);
    regconst(MONITOR_EVENT_METADATA_CHANGED);
    regconst(MIME_APPLICATION_ARGUMENT_TYPE_URIS);
    regconst(MIME_APPLICATION_ARGUMENT_TYPE_PATHS);
    regconst(MIME_APPLICATION_ARGUMENT_TYPE_URIS_FOR_NON_FILES);
    regconst(XFER_DEFAULT);
    regconst(XFER_FOLLOW_LINKS);
    regconst(XFER_RECURSIVE);
    regconst(XFER_SAMEFS);
    regconst(XFER_DELETE_ITEMS);
    regconst(XFER_EMPTY_DIRECTORIES);
    regconst(XFER_NEW_UNIQUE_DIRECTORY);
    regconst(XFER_REMOVESOURCE);
    regconst(XFER_USE_UNIQUE_NAMES);
    regconst(XFER_LINK_ITEMS);
    regconst(XFER_FOLLOW_LINKS_RECURSIVE);
    regconst(XFER_PROGRESS_STATUS_OK);
    regconst(XFER_PROGRESS_STATUS_VFSERROR);
    regconst(XFER_PROGRESS_STATUS_OVERWRITE);
    regconst(XFER_PROGRESS_STATUS_DUPLICATE);
    regconst(XFER_OVERWRITE_MODE_ABORT);
    regconst(XFER_OVERWRITE_MODE_QUERY);
    regconst(XFER_OVERWRITE_MODE_REPLACE);
    regconst(XFER_OVERWRITE_MODE_SKIP);
    regconst(XFER_OVERWRITE_ACTION_ABORT);
    regconst(XFER_OVERWRITE_ACTION_REPLACE);
    regconst(XFER_OVERWRITE_ACTION_REPLACE_ALL);
    regconst(XFER_OVERWRITE_ACTION_SKIP);
    regconst(XFER_OVERWRITE_ACTION_SKIP_ALL);
    regconst(XFER_ERROR_MODE_ABORT);
    regconst(XFER_ERROR_MODE_QUERY);
    regconst(XFER_ERROR_ACTION_ABORT);
    regconst(XFER_ERROR_ACTION_RETRY);
    regconst(XFER_ERROR_ACTION_SKIP);
    regconst(XFER_PHASE_INITIAL);
    regconst(XFER_CHECKING_DESTINATION);
    regconst(XFER_PHASE_COLLECTING);
    regconst(XFER_PHASE_READYTOGO);
    regconst(XFER_PHASE_OPENSOURCE);
    regconst(XFER_PHASE_OPENTARGET);
    regconst(XFER_PHASE_COPYING);
    regconst(XFER_PHASE_MOVING);
    regconst(XFER_PHASE_READSOURCE);
    regconst(XFER_PHASE_WRITETARGET);
    regconst(XFER_PHASE_CLOSESOURCE);
    regconst(XFER_PHASE_CLOSETARGET);
    regconst(XFER_PHASE_DELETESOURCE);
    regconst(XFER_PHASE_SETATTRIBUTES);
    regconst(XFER_PHASE_FILECOMPLETED);
    regconst(XFER_PHASE_CLEANUP);
    regconst(XFER_PHASE_COMPLETED);
    regconst(DIRECTORY_KIND_DESKTOP);
    regconst(DIRECTORY_KIND_TRASH);
    regconst(PERM_ACCESS_READABLE);
    regconst(PERM_ACCESS_WRITABLE);
    regconst(PERM_ACCESS_EXECUTABLE);
    regconst(PRIORITY_MIN);
    regconst(PRIORITY_MAX);
    regconst(PRIORITY_DEFAULT);
    regconst(MIME_UNRELATED);
    regconst(MIME_IDENTICAL);
    regconst(MIME_PARENT);

#undef regconst
}

static void initialize_exceptions (PyObject *d)
{
    pygnomevfs_exc = PyErr_NewException ("gnomevfs.Error",
                                                 PyExc_RuntimeError, NULL);
    PyDict_SetItemString(d, "Error", pygnomevfs_exc);
 
#define register_exception(c_name, py_name)                             \
    pygnomevfs_##c_name##_exc =                                   \
        PyErr_NewException ("gnomevfs." py_name "Error",               \
                            pygnomevfs_exc, NULL);                \
    PyDict_SetItemString(d, py_name "Error", pygnomevfs_##c_name##_exc);
 
    register_exception(not_found,             "NotFound");
    register_exception(generic,               "Generic");
    register_exception(internal,              "Internal");
    register_exception(bad_parameters,        "BadParameters");
    register_exception(not_supported,         "NotSupported");
    register_exception(io,                    "IO");
    register_exception(corrupted_data,        "CorruptedData");
    register_exception(wrong_format,          "WrongFormat");
    register_exception(bad_file,              "BadFile");
    register_exception(too_big,               "TooBig");
    register_exception(no_space,              "NoSpace");
    register_exception(read_only,             "ReadOnly");    
    register_exception(invalid_uri,           "InvalidURI");
    register_exception(not_open,              "NotOpen");    
    register_exception(invalid_open_mode,     "InvalidOpenMode");
    register_exception(access_denied,         "AccessDenied");    
    register_exception(too_many_open_files,   "TooManyOpenFiles");
    register_exception(eof,                   "EOF");    
    register_exception(not_a_directory,       "NotADirectory");
    register_exception(in_progress,           "InProgress");
    register_exception(interrupted,           "Interrupted");
    register_exception(file_exists,           "FileExists");
    register_exception(loop,                  "Loop");
    register_exception(not_permitted,         "NotPermitted");
    register_exception(is_directory,          "IsDirectory");
    register_exception(no_memory,             "NoMemory");
    register_exception(host_not_found,        "HostNotFound");
    register_exception(invalid_host_name,     "InvalidHostName");
    register_exception(host_has_no_address,   "HostHasNoAddress");
    register_exception(login_failed,          "LoginFailed");
    register_exception(cancelled,             "Cancelled");
    register_exception(directory_busy,        "DirectoryBusy");
    register_exception(directory_not_empty,   "DirectoryNotEmpty");
    register_exception(too_many_links,        "TooManyLinks");
    register_exception(read_only_file_system, "ReadOnlyFileSystem");
    register_exception(not_same_file_system,  "NotSameFileSystem");
    register_exception(name_too_long,         "NameTooLong");
    register_exception(service_not_available, "ServiceNotAvailable");
    register_exception(service_obsolete,      "ServiceObsolete");
    register_exception(protocol_error,        "ProtocolError");
    register_exception(no_master_browser,     "NoMasterBrowser");
#if 0    
    register_exception(no_default,            "NoDefault");
    register_exception(no_handler,            "NoHandler");
    register_exception(parse,                 "Parse");
    register_exception(launch,                "Launch");
#endif    
#undef register_exception
}

struct _PyGnomeVFS_Functions pygnomevfs_api_functions = {
    pygnome_vfs_exception_check,
    pygnome_vfs_uri_new,
    &PyGnomeVFSURI_Type,
    pygnome_vfs_file_info_new,
    &PyGnomeVFSFileInfo_Type,
    pygnome_vfs_context_new,
    &PyGnomeVFSContext_Type,

};

/* initialise stuff extension classes */
static void
pygnomefs_register_gobject_based_classes(PyObject *d)
{
    PyObject *module;

    if ((module = PyImport_ImportModule("gobject")) != NULL) {
        PyObject *moddict = PyModule_GetDict(module);

        _PyGObject_Type = (PyTypeObject *)PyDict_GetItemString(moddict, "GObject");
        if (_PyGObject_Type == NULL) {
            PyErr_SetString(PyExc_ImportError,
                "cannot import name GObject from gobject");
            return;
        }
    } else {
        PyErr_SetString(PyExc_ImportError,
            "could not import gobject");
        return;
    }

    pygobject_register_class(d, "Volume", GNOME_VFS_TYPE_VOLUME, &PyGnomeVFSVolume_Type,
                             Py_BuildValue("(O)", &PyGObject_Type));
    pygobject_register_class(d, "Drive", GNOME_VFS_TYPE_DRIVE, &PyGnomeVFSDrive_Type,
                             Py_BuildValue("(O)", &PyGObject_Type));
    pygobject_register_class(d, "VolumeMonitor", GNOME_VFS_TYPE_VOLUME_MONITOR,
                             &PyGnomeVFSVolumeMonitor_Type,
                             Py_BuildValue("(O)", &PyGObject_Type));
}

DL_EXPORT(void)
init_gnomevfs(void)
{
    PyObject *m, *d, *o;

    PyGnomeVFSURI_Type.ob_type = &PyType_Type;
    PyGnomeVFSContext_Type.ob_type = &PyType_Type;
    PyGnomeVFSFileInfo_Type.ob_type = &PyType_Type;
    PyGnomeVFSDirectoryHandle_Type.ob_type = &PyType_Type;
    PyGnomeVFSHandle_Type.ob_type = &PyType_Type;

    init_pygobject();
    if (!gnome_vfs_init()) {
	PyErr_SetString(PyExc_RuntimeError, "could not initialise gnomevfs");
	return;
    }

    if (PyType_Ready(&PyGnomeVFSURI_Type) < 0)
	return;
    if (PyType_Ready(&PyGnomeVFSContext_Type) < 0)
	return;
    if (PyType_Ready(&PyGnomeVFSFileInfo_Type) < 0)
	return;
    if (PyType_Ready(&PyGnomeVFSDirectoryHandle_Type) < 0)
	return;
    if (PyType_Ready(&PyGnomeVFSHandle_Type) < 0)
	return;
    if (PyType_Ready(&PyGnomeVFSXferProgressInfo_Type) < 0)
	return;

    m = Py_InitModule("gnomevfs._gnomevfs", pygnomevfs_functions);
    d = PyModule_GetDict(m);

    register_constants(m);
    initialize_exceptions(d);
    PyDict_SetItemString(d, "Error", pygnomevfs_exc);

    PyDict_SetItemString(d, "URI", (PyObject *)&PyGnomeVFSURI_Type);
    PyDict_SetItemString(d, "Context", (PyObject *)&PyGnomeVFSContext_Type);
    PyDict_SetItemString(d, "FileInfo", (PyObject *)&PyGnomeVFSFileInfo_Type);
    PyDict_SetItemString(d, "DirectoryHandle",
			 (PyObject *)&PyGnomeVFSDirectoryHandle_Type);
    PyDict_SetItemString(d, "Handle", (PyObject *)&PyGnomeVFSHandle_Type);


    pygnomefs_register_gobject_based_classes(d);
    pygvvolume_add_constants(m);

    PyDict_SetItemString(d, "async", pygvfs_async_module_init ());

    PyDict_SetItemString(d, "open_directory",
			 (PyObject *)&PyGnomeVFSDirectoryHandle_Type);
    PyDict_SetItemString(d, "open", (PyObject *)&PyGnomeVFSHandle_Type);

    PyDict_SetItemString(d, "_PyGnomeVFS_API",
			 o=PyCObject_FromVoidPtr(&pygnomevfs_api_functions,NULL));
    Py_DECREF(o);

    monitor_hash = g_hash_table_new(g_direct_hash, g_direct_equal);
}
