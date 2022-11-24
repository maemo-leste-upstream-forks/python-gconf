/* -*- Mode: C; c-basic-offset: 4 -*- */

#define NO_IMPORT_PYGOBJECT
#include <pygobject.h>

#define NO_IMPORT_PYORBIT
#include <pyorbit.h>

#include <bonobo/bonobo-types.h>

static PyObject *
pybonobo_corbaany_from_value(const GValue *value)
{
    return pycorba_any_new(g_value_get_boxed(value));

}
static int
pybonobo_corbaany_to_value(GValue *value, PyObject *object)
{
    CORBA_any *any;

    if (!PyObject_TypeCheck(object, &PyCORBA_Any_Type))
	return -1;

    any = &((PyCORBA_Any*)object)->any;
    g_value_set_boxed(value, any);
    return 0;
}

static PyObject *
pybonobo_corbatypecode_from_value(const GValue *value)
{
    return pycorba_typecode_new(g_value_get_boxed(value));

}
static int
pybonobo_corbatypecode_to_value(GValue *value, PyObject *object)
{
    CORBA_TypeCode tc;

    if (!PyObject_TypeCheck(object, &PyCORBA_TypeCode_Type))
	return -1;

    tc = ((PyCORBA_TypeCode *)object)->tc;
    g_value_set_boxed(value, tc);
    return 0;
}

static PyObject *
pybonobo_corbaobject_from_value(const GValue *value)
{
    return pycorba_object_new((CORBA_Object)g_value_get_boxed(value));
}
static int
pybonobo_corbaobject_to_value(GValue *value, PyObject *object)
{
    CORBA_Object objref;

    if (!PyObject_TypeCheck(object, &PyCORBA_Object_Type))
	return -1;

    objref = ((PyCORBA_Object *)object)->objref;
    g_value_set_boxed(value, objref);
    return 0;
}

static int
pybonobo_unknown_to_value(GValue *value, PyObject *object)
{
    CORBA_Object objref;
    CORBA_Environment ev;
    gboolean type_matches;

    if (!PyObject_TypeCheck(object, &PyCORBA_Object_Type))
	return -1;

    objref = ((PyCORBA_Object *)object)->objref;

    /* check if it is a Bonobo::Unknown */
    CORBA_exception_init(&ev);
    type_matches = CORBA_Object_is_a(objref, "IDL:Bonobo/Unknown:1.0", &ev);
    if (pyorbit_check_ex(&ev))
	return -1;
    if (!type_matches)
	return -1;

    g_value_set_boxed(value, objref);
    return 0;
}

void
_pybonobo_register_boxed_types(PyObject *moddict)
{
	pyg_register_boxed_custom(BONOBO_TYPE_CORBA_ANY,
				  pybonobo_corbaany_from_value,
				  pybonobo_corbaany_to_value);
	pyg_register_boxed_custom(BONOBO_TYPE_CORBA_TYPECODE,
				  pybonobo_corbatypecode_from_value,
				  pybonobo_corbatypecode_to_value);
	pyg_register_boxed_custom(BONOBO_TYPE_CORBA_OBJECT,
				  pybonobo_corbaobject_from_value,
				  pybonobo_corbaobject_to_value);
	pyg_register_boxed_custom(BONOBO_TYPE_UNKNOWN,
				  pybonobo_corbaobject_from_value,
				  pybonobo_unknown_to_value);
}

