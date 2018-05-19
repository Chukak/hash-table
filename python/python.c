#include <Python.h>
#include <structmember.h>
#include "../hashtable.h"

#if PY_MAJOR_VERSION >= 3
#define INIT_ERROR return NULL
#else
#define INIT_ERROR return
#endif

/*
    A macro for the the value from the pointer;
 */
#define get_pointer(type, pointer) ({ \
    __typeof__(type*) value = pointer; \
    *value; \
})

/*
    The hash table struct.
    `table` is a pointer to a table.
    `size` is the size of a table.
    `count` is the sum of the elements in a table.
    `empty` is 1 or 0.
 */
typedef struct {
    PyObject_HEAD
    tb_hash_table *table;
    uint32_t size;
    uint32_t count;
    uint32_t empty;
} PyHashTable;

/*
    The hash table iterator struct.
    `index` is a current index.
    `size` is the size of a table.
    `items` is an array of pointers.
 */
typedef struct {
    PyObject_HEAD
    uint32_t index;
    uint32_t size;
    tb_hash_table_item **items;
} PyHashTableItems;

// A module state.
struct module_state {
    PyObject *error;
};

/*
    A static function, creates a new hash table.
    Returns a pointer to the table.
 */
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

/*
    A static function, removes a hash table from memory.
    Nothing to returns.
 */
static void 
PyHashTable_dealloc(PyHashTable *self) 
{
    tb_delete_hash_table(self->table);
#if PY_MAJOR_VERSION >=3
    self->ob_base.ob_type->tp_free((PyObject *)self);
#else
    self->ob_type->tp_free((PyObject *)self);
#endif
}

/*
    A static function, initializes a new hash table.
    Returns 0 if success, otherwise -1.
 */
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

/*
    A static function, gets a value by key.
    Creates a python object from this value and returns it.
 */
static PyObject *
PyHashTable_get(PyObject *self, PyObject *args)
{
    
    char *key;
    if (!PyArg_Parse(args, "s", &key)) {
        PyErr_SetString(PyExc_TypeError, "The key must be a string.");
        return NULL;
    }
    PyHashTable *h_table = (PyHashTable *)self;
    if (h_table == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "The pointer on the hashtable is NULL.");
        PyErr_Print();
        return NULL;
    }
    void *p = tb_get_value(h_table->table, key);
    if (p == NULL) {
        Py_RETURN_NONE;
    }
    PyObject *value = get_pointer(PyObject *, p);
    return Py_BuildValue("O", value);
}

/*
    A static function, inserts a value by key into the table.
    Returns the position.
 */
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
    if (h_table == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "The pointer on the hashtable is NULL.");
        return NULL;
    }
    Py_INCREF(value);
    if (value == NULL) {
        return NULL;
    }
    uint32_t pos = tb_insert_item(h_table->table, key, &value);
    h_table->count = h_table->table->count;
    h_table->empty = h_table->table->empty;
    return Py_BuildValue("I", pos);
}

/*
    A static function, removes a value by key.
    Returns `True` if success, otherwise `False`.
 */
static PyObject *
PyHashTable_delete(PyObject *self, PyObject *args)
{
    char *key;
    if (!PyArg_Parse(args, "s", &key)) {
        PyErr_SetString(PyExc_TypeError, "The key must be a string.");
        return NULL;
    }
    PyHashTable *h_table = (PyHashTable *)self;
    if (h_table == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "The pointer on the hashtable is NULL.");
        return NULL;
    }
    int del = tb_delete_item(h_table->table, key);
    
    h_table->count = h_table->table->count;
    h_table->empty = h_table->table->empty;
    return Py_BuildValue("N", PyBool_FromLong(del));
}

/*
    A static function, searches an element by key.
    Returns a tuple in the format `(key, value)`.
 */
static PyObject *
PyHashTable_find(PyObject *self, PyObject *args)
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
    tb_hash_table_item *item = tb_find_item(h_table->table, key);
    if (item == NULL) {
        return Py_BuildValue("(s, O)", "", Py_None);
    }
    void *p = item->val;
    if (p == NULL) {
        return Py_BuildValue("(s, O)", key, Py_None);
    }
    PyObject *value = get_pointer(PyObject *, p);
    return Py_BuildValue("(s, O)", key, value);
}

/*
    A static function, gets an element.
    Returns a tuple in the format `(key, value)`.
 */
static PyObject *
PyHashTable_at(PyObject *self, PyObject *args)
{
    int pos;
    if (!PyArg_Parse(args, "i", &pos)) {
        PyErr_SetString(PyExc_TypeError, "The position must be an integer.");
        return NULL;
    } 
    PyHashTable *h_table = (PyHashTable *)self;
    if (!h_table) {
        PyErr_SetString(PyExc_RuntimeError, "The pointer on the hashtable is NULL.");
        return NULL;
    }
    tb_hash_table_item *item = tb_item_at(h_table->table, pos);
    if (item == NULL) {
        return Py_BuildValue("(s, O)", "", Py_None);
    }
    if (item->val == NULL && item->key == NULL) {
        return Py_BuildValue("(O, O)", Py_None, Py_None);
    }
    void *p = item->val;
    if (p == NULL) {
        return Py_BuildValue("(s, O)", item->key, Py_None);
    }
    PyObject *value = get_pointer(PyObject *, p);
    return Py_BuildValue("(s, O)", item->key, value);
}

/*
    A static function, creates pointer to the iterator.
 */
static PyObject *
PyHashTableItems_iter(PyObject *self) 
{
    Py_INCREF(self);
    return self;
}

/*
    A static function, returns a next element from iterator.
 */
static PyObject *
PyHashTableItems_iternext(PyObject *self)
{
    PyHashTableItems *iter = (PyHashTableItems *)self;
    PyObject *value = NULL;
    if (iter == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "The pointer on the hashtable is NULL.");
        return NULL;
    }
    // get an element from the index
    // if an element is deleted, increase the index and check again
    while (iter->index < iter->size && iter->index >= 0) {
        if (iter->items[iter->index] != NULL 
                && iter->items[iter->index]->key != NULL
                && iter->items[iter->index]->val != NULL) {
            void *p = iter->items[iter->index]->val;
            if (p == NULL) {
                return Py_BuildValue("(s, O)", iter->items[iter->index]->key, Py_None);
            }       
            value = get_pointer(PyObject *, p);
            PyObject *obj = Py_BuildValue("sO", iter->items[iter->index]->key, value);
            iter->index++;
            return obj;
        } else {
            iter->index++;
        }
    }
    PyErr_SetNone(PyExc_StopIteration);
    return NULL;
}

/*
    The full definition of the iterator object of the hash table.
 */
static PyTypeObject PyHashTableItems_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "_iter()",                                          /* tp_name */
    sizeof(PyHashTableItems),                           /* tp_basicsize */
    0,                                                  /*  itemsize */
    (destructor)0,                                      /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT,                                 /* tp_flags */
    0,                                                  /* tp_doc */
    (traverseproc)0,                                    /* tp_traverse */
    (inquiry)0,                                         /* tp_clear */
    (richcmpfunc)0,                                     /* tp_richcompare */
    0,                                                  /* tp_weaklistoffset */
    (getiterfunc)PyHashTableItems_iter,                 /* tp_iter */
    (iternextfunc)PyHashTableItems_iternext,            /* tp_iternext */
    (struct PyMethodDef *)0,                            /* tp_methods */
    (struct PyMemberDef *)0,                            /* tp_members */
    0,                                                  /* tp_getset */
    0,                                                  /* tp_base */
    0,                                                  /* tp_dict */
    (descrgetfunc)0,                                    /* tp_descr_get */
    (descrsetfunc)0,                                    /* tp_descr_set */
    0,                                                  /* tp_dictoffset */
    (initproc)0,                                        /* tp_init */
    0,                                                  /* tp_alloc */
    (newfunc)0,                                         /* tp_new */
    (freefunc)0,                                        /* tp_free */
    (inquiry)0,                                         /* tb_is_gc */
    0,                                                  /* tp_bases */
    0,                                                  /* tp_mro */
    0,                                                  /* tp_cache */
    0,                                                  /* tp_subclasses */
    0,                                                  /* tp_weaklist */
    (destructor)0                                       /* tp_del */
};

/*
    A static function, initializes a new iterator of the hash table.
    Returns the iteterator if success, otherwise `NULL`.
 */
static PyObject *
PyHashTableItems_iter_init(PyObject *self) 
{
    PyHashTable *h_table = (PyHashTable *)self;
    if (!h_table) {
        PyErr_SetString(PyExc_RuntimeError, "The pointer on the hashtable is NULL.");
        return NULL;
    }
    PyHashTableItems *iter = PyObject_New(PyHashTableItems, &PyHashTableItems_type);
    if (iter == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "The pointer on the iterator is NULL.");
        return NULL;
    }
    iter->items = h_table->table->items;
    iter->index = 0;
    iter->size = h_table->table->size;
    return (PyObject *)iter;
}

/*
    Members of the hash table class.
    `size` - a size of the hash table.
    `empty` - `True` if the hash table is empty, otherwise `False`.
    `count` - number of elements in the hash table.
 */
static PyMemberDef PyHashTable_members[] = {
    {
        "size",
        T_INT,
        offsetof(PyHashTable, size),
        READONLY,
        "The size of the table."
    },
    {
        "empty",
        T_INT,
        offsetof(PyHashTable, empty),
        READONLY,
        "`True` if the table is empty, otherwise `False`."
    },
    {
        "count",
        T_INT,
        offsetof(PyHashTable, count),
        READONLY,
        "The count of elements in the table."
    }
};

/*
    Methods of the hash table class.
    `get` - gets a value by key from the hash table.
    `insert` - inserts a value by key into the hash table.
    `delete` - removes a value by key from the hash table.
    `find` - searches an element by key in the hash table.
    `at` - returns an element in this position.
    `items` - returns the iterator of all the elements in the hash table.
 */
static PyMethodDef PyHashTable_methods[] = {
    {
        "get",
        (PyCFunction)PyHashTable_get,
        METH_O,
        "Returns a value by key."
    },
    {
        "insert",
        (PyCFunction)PyHashTable_insert,
        METH_VARARGS,
        "Inserts a value by key into the table. Returns the position."
    },
    {
        "delete",
        (PyCFunction)PyHashTable_delete,
        METH_O,
        "Delete a value by key from the table. "
        "Returns `True` if the deletion is successful, otherwise `False`."
    },
    {
        "find",
        (PyCFunction)PyHashTable_find,
        METH_O,
        "Search the item in the table. "
        "Returns a tuple in the format `(key, value)`.",
    },
    {
        "at",
        (PyCFunction)PyHashTable_at,
        METH_O,
        "Returns the element in this position.",
    },
    {
        "items",
        (PyCFunction)PyHashTableItems_iter_init,
        METH_NOARGS,
        "Returns the iterator of all elements.",
    },
    {NULL}
};

/*
    The full definition of the hash table class.
 */
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

/*
    Methods of the `hashtable` module.
 */
static PyMethodDef hashtable_methods[] = {
    {NULL}
};

#if PY_MAJOR_VERSION >= 3
/*
    The definition of the `hashtable` module.
 */
static PyModuleDef py_module = {
    PyModuleDef_HEAD_INIT,
    "hashtable",
    NULL,
    sizeof(struct module_state),
    hashtable_methods,
    NULL
}; 

/* 
    The function initializes the `hashtable` module. Python 3+ version.
 */
PyMODINIT_FUNC PyInit_hashtable(void) 
#else
/*
    The function initializes the `hashtable` module. Python 2 version.
 */
PyMODINIT_FUNC inithashtable(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *m = PyModule_Create(&py_module);
#else
    PyObject *m = Py_InitModule("hashtable", hashtable_methods);
#endif
    if (!m) {
        INIT_ERROR;
    }
    // Init iterator
    if (PyType_Ready(&PyHashTableItems_type) > 0) {
        INIT_ERROR;
    }
    PyObject *iterator = (PyObject *)&PyHashTableItems_type;
    if (!iterator) {
        INIT_ERROR;
    }
    Py_INCREF(iterator);
    if (PyModule_AddObject(m, "Iterator", iterator) < 0) {
        INIT_ERROR;
    };
    // Init hashtable class
    if (PyType_Ready(&PyHashTable_type) > 0) {
        INIT_ERROR;
    }
    PyObject *table_class = (PyObject *)&PyHashTable_type;
    if (!table_class) {
        INIT_ERROR;
    }
    Py_INCREF(table_class);
    if (PyModule_AddObject(m, "Table", table_class) < 0) {
        INIT_ERROR;
    };
#if PY_MAJOR_VERSION >= 3
    return m;
#endif
}
