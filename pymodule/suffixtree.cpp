#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <tuple>
#include "suffix_tree.hpp"


Node *root;
// const char *build_string;
std::vector<std::string> build_strings;


static PyObject *build(PyObject *self, PyObject *args, PyObject *kwargs) {
	static char *keywords[] = {"build_string", NULL};

	PyObject *listObj;

	// Parse arguments
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords, &PyList_Type, &listObj)) {
        return NULL;
    }

	if (root) delete root;
	root = new Node();
	
	// Parse list of strings
	for (int i = 0; i < PyList_Size(listObj); i++) {
		PyObject *item;
		item = PyList_GetItem(listObj, i);
		PyObject* repr = PyObject_Repr(item);
		PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
		const char *bytes = PyBytes_AS_STRING(str);
		build_strings.push_back(bytes);
		// PyObject *str;
		// str = PyObject_Str(item);
		// PyObject_Print(str, stdout, Py_PRINT_RAW);
		// std::cout << "\n";
	}
	for (int i = 0; i < build_strings.size(); i++) {
		std::cout << build_strings[i] << '\n';
	}

	build_suffix_tree_naive(build_strings, root);

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

	std::vector<std::tuple<int, int>> matches {};
	query_suffix_tree(root, build_strings, query_string, &matches);

	// for (std::tuple<int, int> match : matches) {
	// 	int string_id = std::get<0>(match);
	// 	int string_offset = std::get<1>(match);
	// 	std::cout << "[" << string_id << ", " << string_offset << "]\n";
	// }

	PyObject *python_matches = PyList_New(matches.size());
	for (int i = 0; i < matches.size(); i++) {
		PyObject *match_value = Py_BuildValue("(i,i)", std::get<0>(matches[i]), std::get<1>(matches[i]));
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
