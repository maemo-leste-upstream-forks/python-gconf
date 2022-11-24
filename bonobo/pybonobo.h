/* -*- Mode: C; c-basic-offset: 4 -*- */
#ifndef _PYBONOBO_H_
#define _PYBONOBO_H_

#include <Python.h>

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

GClosure * pybonobo_closure_new (PyObject *callback,
                                 PyObject *extra_args,
                                 PyObject *swap_data);
G_END_DECLS

#endif
