/* -*- mode: C; c-basic-offset: 4 -*- */
#ifndef __PYGNOMEVFS_PRIVATE_H_
#define __PYGNOMEVFS_PRIVATE_H_

#ifdef _PYGNOMEVFS_H_
#  error "include pygnomevfs.h or pygnomevfs-private.h, but not both"
#endif

#define _INSIDE_PYGNOMEVFS_
#include "pygnomevfs.h"
#include <libgnomevfs/gnome-vfs-volume.h>
#include <libgnomevfs/gnome-vfs-volume-monitor.h>

#if PY_VERSION_HEX < 0x02050000
typedef int Py_ssize_t;
#define PY_SSIZE_T_MAX INT_MAX
#define PY_SSIZE_T_MIN INT_MIN
typedef inquiry lenfunc;
typedef intargfunc ssizeargfunc;
typedef intobjargproc ssizeobjargproc;
#endif


G_BEGIN_DECLS

/* vfsmodule.c */
extern struct _PyGnomeVFS_Functions pygnomevfs_api_functions;

GnomeVFSResult pygnome_vfs_exception_check(void);
gboolean pygnome_vfs_result_check(GnomeVFSResult result);

/* vfs-contexti.c */
extern PyTypeObject PyGnomeVFSContext_Type;

PyObject *pygnome_vfs_context_new(GnomeVFSContext *context);

/* vfs-uri.c */
extern PyTypeObject PyGnomeVFSURI_Type;

/* takes ownership of URI */
PyObject *pygnome_vfs_uri_new(GnomeVFSURI *uri);

/* vfs-file-info.c */
extern PyTypeObject PyGnomeVFSFileInfo_Type;

PyObject *pygnome_vfs_file_info_new(GnomeVFSFileInfo *finfo);


/* vfs-dir-handle.c */
typedef struct {
    PyObject_HEAD
    GnomeVFSDirectoryHandle *dir;
} PyGnomeVFSDirectoryHandle;
extern PyTypeObject PyGnomeVFSDirectoryHandle_Type;

#define pygnome_vfs_directory_handle_get(v) (((PyGnomeVFSDirectoryHandle *)(v))->dir)
PyObject *pygnome_vfs_directory_handle_new(GnomeVFSDirectoryHandle *dir);


/* vfs-handle.c */
typedef struct {
    PyObject_HEAD
    GnomeVFSHandle *fd;
} PyGnomeVFSHandle;
extern PyTypeObject PyGnomeVFSHandle_Type;

#define pygnome_vfs_handle_get(v) (((PyGnomeVFSHandle *)(v))->fd)
PyObject *pygnome_vfs_handle_new(GnomeVFSHandle *fd);

/* vfs-async-handle.c */
PyObject *pygvfs_async_module_init (void);

typedef struct {
    PyObject *func, *data;
} PyGVFSCustomNotify;


/* vfs-xfer-progress-info.c */
typedef struct {
    PyObject_HEAD
    GnomeVFSXferProgressInfo *info; /* not owned */
} PyGnomeVFSXferProgressInfo;

extern PyTypeObject PyGnomeVFSXferProgressInfo_Type;
PyObject *pygnome_vfs_xfer_progress_info_new(GnomeVFSXferProgressInfo *info);
#define pygnome_vfs_xfer_progress_info_set(self, info_)\
       ((PyGnomeVFSXferProgressInfo *) (self))->info = info_


/* vfs-volume.c (PyGObject) */
extern PyTypeObject PyGnomeVFSVolume_Type;
/* vfs-drive.c (PyGObject) */
extern PyTypeObject PyGnomeVFSDrive_Type;
/* vfs-drive-monitor.c (PyGObject) */
extern PyTypeObject PyGnomeVFSVolumeMonitor_Type;

typedef struct {
    PyObject *callback;
    PyObject *user_data;
} PyGnomeVFSVolumeOpCallback;

void wrap_gnomevfs_volume_op_callback(gboolean succeeded,
                                      char *error,
                                      char *detailed_error,
                                      gpointer data);

G_END_DECLS

#endif /* __PYGNOMEVFS_PRIVATE_H_ */
