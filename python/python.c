#include <python3.5/Python.h>
#include <python3.5/structmember.h>
#include "../hashtable.h"
#include <stdio.h>

#define get_pointer(type, pointer) ({ \
    __typeof__(type*) value = pointer; \
    *value; \
})

typedef struct {
    PyObject_HEAD
    tb_hash_table *table;
    uint32_t size;
    uint32_t count;
    uint32_t empty;
} PyHashTable;


struct module_state {
    PyObject *error;
};

static PyObject *
PyHashTable_new(PyTypeObject *type, PyObject *args)
{
    PyHashTable *hash_table;
    hash_table = (PyHashTable *)type->tp_alloc(type, 0);
    if (!hash_table) {
        PyErr_SetString(PyExc_RuntimeError, "Can not allocate memory for an object");
        PyErr_Print();
        return NULL;
    }
    return (PyObject *)hash_table;
}

static void 
PyHashTable_dealloc(PyHashTable *self) 
{
    tb_delete_hash_table(self->table);
    self->ob_base.ob_type->tp_free((PyObject *)self);
}

static int
PyHashTable_init(PyHashTable *hash_table, PyObject *args)
{
    uint32_t size = 0;
    if (!PyTuple_Size(args)) {
        PyErr_SetString(PyExc_TypeError, 
                "__init__ missing 1 positional argument `size`.");
        PyErr_Print();
        return -1;
    }
    if (!PyArg_ParseTuple(args, "i", &size)) {
        PyErr_SetString(PyExc_TypeError, "The size must be an integer.");
        PyErr_Print();
        return -1;
    }
    if (size <= 0) {
        PyErr_SetString(PyExc_ValueError, "The size must be greatest than zero.");
        PyErr_Print();
        return -1;
    }
    tb_hash_table *table = tb_create_hash_table((size_t)size);
    if (!table) {
        PyErr_SetString(PyExc_RuntimeError, "The pointer on the hashtable is NULL.");
        PyErr_Print();
        return -1;
    }
    hash_table->table = table;
    hash_table->size = table->size;
    hash_table->count = 0;
    hash_table->empty = 1;
    return 0;
}

static PyObject *
PyHashTable_get(PyObject *self, PyObject *args)
{
    
    char *key;
    if (!PyArg_Parse(args, "s", &key)) {
        PyErr_SetString(PyExc_TypeError, "The key must be a string.");
        return NULL;
    }
    PyHashTable *h_table = (PyHashTable *)self;
    if (!h_table) {
        PyErr_SetString(PyExc_RuntimeError, "The pointer on the hashtable is NULL.");
        PyErr_Print();
        return NULL;
    }
    void *p = tb_get_value(h_table->table, key);
    if (!p) {
        Py_RETURN_NONE;
    }
    PyObject *value = get_pointer(PyObject *, p);
    return Py_BuildValue("O", value);
}

static PyObject *
PyHashTable_insert(PyObject *self, PyObject *args)
{
    PyObject *value = NULL;
    char *key;
    if (!PyTuple_Size(args)) {
        PyErr_SetString(PyExc_TypeError, 
                "insert missing 2 positional arguments.");
        PyErr_Print();
        return NULL;
    } else if (PyTuple_Size(args) == 1) {
        PyErr_SetString(PyExc_TypeError, 
                "insert missing 1 positional argument `value`.");
        PyErr_Print();
        return NULL;
    }
    if (!PyArg_ParseTuple(args, "sO", &key, &value)) {
        PyErr_SetString(PyExc_TypeError, 
                "The invalid parameters in the function. "
                "The first parameter must be a string. "
                "The second parameter must be an any object.");
        return NULL;
    }
    PyHashTable *h_table = (PyHashTable *)self;
    if (!h_table) {
        PyErr_SetString(PyExc_RuntimeError, "The pointer on the hashtable is NULL.");
        return NULL;
    }
    Py_INCREF(value);
    if (!value) {
        return NULL;
    }
    tb_insert_item(h_table->table, key, &value);
    h_table->count = h_table->table->count;
    h_table->empty = h_table->table->empty;
    return Py_BuildValue("N", PyBool_FromLong(1));
}

static PyObject *
PyHashTable_delete(PyObject *self, PyObject *args)
{
    char *key;
    if (!PyArg_Parse(args, "s", &key)) {
        PyErr_SetString(PyExc_TypeError, "The key must be a string.");
        return NULL;
    }
    PyHashTable *h_table = (PyHashTable *)self;
    if (!h_table) {
        PyErr_SetString(PyExc_RuntimeError, "The pointer on the hashtable is NULL.");
        return NULL;
    }
    int del = tb_delete_item(h_table->table, key);
    
    h_table->count = h_table->table->count;
    h_table->empty = h_table->table->empty;
    return Py_BuildValue("N", PyBool_FromLong(del));
}


static PyMemberDef PyHashTable_members[] = {
    {
        "size",
        T_INT,
        offsetof(PyHashTable, size),
        READONLY,
        ""
    },
    {
        "empty",
        T_INT,
        offsetof(PyHashTable, empty),
        READONLY,
        ""
    },
    {
        "count",
        T_INT,
        offsetof(PyHashTable, count),
        READONLY,
        ""
    }
};

static PyMethodDef PyHashTable_methods[] = {
    {
        "get",
        (PyCFunction)PyHashTable_get,
        METH_O,
        ""
    },
    {
        "insert",
        (PyCFunction)PyHashTable_insert,
        METH_VARARGS,
        ""
    },
    {
        "delete",
        (PyCFunction)PyHashTable_delete,
        METH_O,
        ""
    },
    {NULL}
};

static PyTypeObject PyHashTable_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "hashtable.Table",                                  /* tp_name */
    sizeof(PyHashTable),                                /* tp_basicsize */
    0,                                                  /*  itemsize */
    (destructor)PyHashTable_dealloc,                    /* tp_dealloc */
    (printfunc)0,                                       /* tp_print */
    (getattrfunc)0,                                     /* tp_getattr */
    (setattrfunc)0,                                     /* tp_setattr */
#if PY_VERSION_HEX >= 0x03050000
    (PyAsyncMethods *)0,                                /* tp_as_async */
#elif PY_VERSION_HEX >= 0x03000000
    (void *)0,                                          /* tp_reserved */
#else
    0,                                                  /* tp_compare */
#endif
    (reprfunc)0,                                        /* tp_repr */
    (PyNumberMethods *)0,                               /* tp_as_number */
    (PySequenceMethods *)0,                             /* tp_as_sequence */
    (PyMappingMethods *)0,                              /* tp_as_mapping */
    (hashfunc)0,                                        /* tp_hash */
    (ternaryfunc)0,                                     /* tp_call */
    (reprfunc)0,                                        /* tp_str */
    (getattrofunc)0,                                    /* tp_getattro */
    (setattrofunc)0,                                    /* tp_setattro */
    (PyBufferProcs *)0,                                 /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,             /* tp_flags */
    0,                                                  /* tp_doc */
    (traverseproc)0,                                    /* tp_traverse */
    (inquiry)0,                                         /* tp_clear */
    (richcmpfunc)0,                                     /* tp_richcompare */
    0,                                                  /* tp_weaklistoffset */
    (getiterfunc)0,                                     /* tp_iter */
    (iternextfunc)0,                                    /* tp_iternext */
    (struct PyMethodDef *)PyHashTable_methods,          /* tp_methods */
    (struct PyMemberDef *)PyHashTable_members,          /* tp_members */
    0,                                                  /* tp_getset */
    0,                                                  /* tp_base */
    0,                                                  /* tp_dict */
    (descrgetfunc)0,                                    /* tp_descr_get */
    (descrsetfunc)0,                                    /* tp_descr_set */
    0,                                                  /* tp_dictoffset */
    (initproc)PyHashTable_init,                         /* tp_init */
    0,                                                  /* tp_alloc */
    (newfunc)PyHashTable_new,                           /* tp_new */
    (freefunc)0,                                        /* tp_free */
    (inquiry)0,                                         /* tb_is_gc */
    0,                                                  /* tp_bases */
    0,                                                  /* tp_mro */
    0,                                                  /* tp_cache */
    0,                                                  /* tp_subclasses */
    0,                                                  /* tp_weaklist */
    (destructor)0                                       /* tp_del */
};

static PyMethodDef hashtable_methods[] = {
    {NULL}
};

static PyModuleDef py_module = {
    PyModuleDef_HEAD_INIT,
    "hashtable",
    NULL,
    sizeof(struct module_state),
    hashtable_methods,
    NULL
}; 


PyMODINIT_FUNC PyInit_hashtable(void) 
{
    PyObject *m = PyModule_Create(&py_module);
    if (!m) {
        return NULL;
    }
    if (PyType_Ready(&PyHashTable_type) > 0) {
        return NULL;
    }
    PyObject *table_class = (PyObject *)&PyHashTable_type;
    if (!table_class) {
        return NULL;
    }
    Py_INCREF(table_class);
    if (PyModule_AddObject(m, "Table", table_class) < 0) {
        return NULL;
    };
    return m;
}


