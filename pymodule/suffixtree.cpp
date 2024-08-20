#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <tuple>
#include "suffix_tree.hpp"
#include "benchmark.hpp"


Node *root;
// const char *build_string;
std::vector<std::string> build_strings;


static PyObject *build(PyObject *self, PyObject *args, PyObject *kwargs) {
	// Parse arguments
	PyObject *listObj;
	static char *keywords[] = {"build_strings", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords, &PyList_Type, &listObj)) {
        return NULL;
    }

	if (root) delete root;
	root = new Node();
	
	// Parse list of strings
	for (int i = 0; i < PyList_Size(listObj); i++) {
		PyObject *item;
		item = PyList_GetItem(listObj, i);
		PyObject* str = PyUnicode_AsEncodedString(item, "utf-8", "~E~");
		const char *bytes = PyBytes_AS_STRING(str);
		build_strings.push_back(bytes);
	}

	build_suffix_tree_naive(build_strings, root);

	return PyLong_FromLong(0);
};


static PyObject *query(PyObject *self, PyObject *args, PyObject *kwargs) {
	// Parse arguments
	static char *keywords[] = {"query_string", NULL};
	const char *query_string;
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s", keywords, &query_string)) {
        return NULL;
    }

	if (!root) {
		return PyLong_FromLong(0);
	}

	std::vector<std::tuple<size_t, size_t>> matches {};
	query_suffix_tree(root, build_strings, query_string, &matches);

	PyObject *python_matches = PyList_New(matches.size());
	for (int i = 0; i < matches.size(); i++) {
		PyObject *match_value = Py_BuildValue("(i,i)", std::get<0>(matches[i]), std::get<1>(matches[i]));
		PyList_SetItem(python_matches, i, match_value);
	}
	return python_matches;
};


static PyObject *benchmark(PyObject *self, PyObject *args, PyObject *kwargs) {
	// Parse arguments
	static char *keywords[] = {"build_strings", "query_strings", "iterations", NULL};
	PyObject *build_list;
	PyObject *query_list;
	PyObject *iterations_list;
	std::vector<std::string> build_strings;
	std::vector<std::string> query_strings;
	std::vector<size_t> iterations;
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!O!O!", keywords, &PyList_Type, &build_list, &PyList_Type, &query_list, &PyList_Type, &iterations_list)) {
        return NULL;
    }

	// Parse build strings
	for (size_t i = 0; i < PyList_Size(build_list); i++) {
		PyObject *item;
		item = PyList_GetItem(build_list, i);
		PyObject* str = PyUnicode_AsEncodedString(item, "utf-8", "~E~");
		const char *bytes = PyBytes_AS_STRING(str);
		build_strings.push_back(bytes);
	}
	// Parse query strings
	for (size_t i = 0; i < PyList_Size(query_list); i++) {
		PyObject *item;
		item = PyList_GetItem(query_list, i);
		PyObject* str = PyUnicode_AsEncodedString(item, "utf-8", "~E~");
		const char *bytes = PyBytes_AS_STRING(str);
		query_strings.push_back(bytes);
	}
	// Parse iterations
	for (size_t i = 0; i < PyList_Size(iterations_list); i++) {
		PyObject *item;
		item = PyList_GetItem(iterations_list, i);
		size_t iteration = PyLong_AsSize_t(item);
		iterations.push_back(iteration);
	}

	BenchmarkResults results = run_benchmark(build_strings, query_strings, iterations);
	PyObject *python_results = PyDict_New();

	// Raw Sequential
	PyObject *raw_iterations = PyDict_New();
	for (int i = 0; i < results.raw_sequential.size(); i++) {
		PyObject *iteration_key = Py_BuildValue("i", iterations[i]);
		PyObject *iteration_results = PyDict_New();
		for (int s = 0; s < results.raw_sequential[i].times_ms.size(); s++) {
			PyObject *time_key = Py_BuildValue("s", query_strings[s].c_str());
			PyObject *time_value = Py_BuildValue("d", results.raw_sequential[i].times_ms[s]);
			PyDict_SetItem(iteration_results, time_key, time_value);
		}
		PyDict_SetItem(raw_iterations, iteration_key, iteration_results);
	}

	// Suffix Tree
	PyObject *suffixtree_iterations = PyDict_New();
	for (int i = 0; i < results.suffix_tree.size(); i++) {
		PyObject *iteration_key = Py_BuildValue("i", iterations[i]);
		PyObject *iteration_results = PyDict_New();
		for (int s = 0; s < results.suffix_tree[i].times_ms.size(); s++) {
			PyObject *time_key = Py_BuildValue("s", query_strings[s].c_str());
			PyObject *time_value = Py_BuildValue("d", results.suffix_tree[i].times_ms[s]);
			PyDict_SetItem(iteration_results, time_key, time_value);
		}
		PyDict_SetItem(suffixtree_iterations, iteration_key, iteration_results);
	}

	PyObject *raw_key = Py_BuildValue("s", "raw_sequential");
	PyObject *suffixtree_key = Py_BuildValue("s", "suffix_tree");
	PyDict_SetItem(python_results, raw_key, raw_iterations);
	PyDict_SetItem(python_results, suffixtree_key, suffixtree_iterations);

	return python_results;
};


static PyMethodDef SuffixTreeMethods[] = {
	{"build", (PyCFunction)build, METH_VARARGS | METH_KEYWORDS, "Build a suffix tree"},
	{"query", (PyCFunction)query, METH_VARARGS | METH_KEYWORDS, "Query a suffix tree"},
	{"benchmark", (PyCFunction)benchmark, METH_VARARGS | METH_KEYWORDS, "Benchmark the suffix tree"},
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
