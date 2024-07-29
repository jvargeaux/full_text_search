#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include <string>
#include <stdio.h>
#include "suffix_tree.hpp"


Node *root;
const char *build_string;


static PyObject *build(PyObject *self, PyObject *args, PyObject *kwargs) {
	static char *keywords[] = {"build_string", NULL};

	// Parse arguments
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", keywords, &build_string)) {
        return NULL;
    }

	if (root) delete root;
	root = new Node();
	build_suffix_tree_naive(std::string(build_string), root);

	return PyLong_FromLong(0);
};


static PyObject *query(PyObject *self, PyObject *args, PyObject *kwargs) {
	static char *keywords[] = {"query_string", NULL};

	// Parse arguments
	const char *query_string;
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", keywords, &query_string)) {
        return NULL;
    }

	if (!root) {
		return PyLong_FromLong(0);
	}

	std::vector<int> matches {};
	query_suffix_tree(root, build_string, query_string, &matches);

	PyObject *python_matches = PyList_New(matches.size());
	for (int i = 0; i < matches.size(); i++) {
		PyObject *match_value = Py_BuildValue("i", matches[i]);
		PyList_SetItem(python_matches, i, match_value);
	}
	return python_matches;
};


static PyMethodDef SuffixTreeMethods[] = {
	{"build", (PyCFunction)build, METH_VARARGS | METH_KEYWORDS, "Build a suffix tree"},
	{"query", (PyCFunction)query, METH_VARARGS | METH_KEYWORDS, "Query a suffix tree"},
	{NULL, NULL, 0, NULL}     // sentinel
};


static struct PyModuleDef suffixtreemodule = {
	PyModuleDef_HEAD_INIT,
	"suffixtree",       // name of module
	"doc test yay",           // module documentation, may be NULL 
	-1,                       // size of per-interpreter state of the module, or -1 if the module keeps state in global variables
	SuffixTreeMethods
};

PyMODINIT_FUNC PyInit_suffixtree(void) {
	// puts("Init suffix tree yayyy...");
	return PyModule_Create(&suffixtreemodule);
};