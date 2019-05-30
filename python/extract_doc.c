#include <Python.h>

#include <stdlib.h>
#include <libdoc.h>

static PyObject* extract_doc_text(PyObject* self, PyObject *args)
{
    PyByteArrayObject *buf;
    long int len;
    if (!PyArg_ParseTuple(args, "Ol", &buf, &len))
    {
        PyErr_SetString(PyExc_ValueError, "Failed parse Python arg");
        return NULL;
    }
    char *doc_buf = PyByteArray_AsString((PyObject*) buf);
    char *result;
    if (doc2text(doc_buf, len, &result) == -1)
    {
        char buf[1024];
        sprintf(buf, "error %lu", len);
//        PyErr_SetString(PyExc_ValueError, "doc2text failed");
        PyErr_SetString(PyExc_ValueError, buf);
        return NULL;
    }
    PyObject *ret = Py_BuildValue("s", result);
    free(result);

    return ret;
}

static PyMethodDef funcs[] = {
	{"extract_doc_text", (PyCFunction) extract_doc_text, 
	 METH_VARARGS, NULL},
	{NULL}
};

static struct PyModuleDef doc =
{
	PyModuleDef_HEAD_INIT,
	"extract_doc", /* name of module */
	NULL,          /* module documentation, may be NULL */
	-1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
	funcs,
	NULL,
	NULL,
	NULL,
    NULL
};

PyMODINIT_FUNC PyInit_extract_doc(void)
{
	return PyModule_Create(&doc);
}
