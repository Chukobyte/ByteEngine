#ifndef PYHELPER_HPP
#define PYHELPER_HPP
#pragma once

#include <Python.h>
#include <iostream>

#include "../EnginePythonModules.h"

class CPyInstance {
	public:
		CPyInstance() {
			Py_SetProgramName(L"byte_engine_scripting");
			PyImport_AppendInittab("game", &PyInit_game); // Load engine modules
			Py_Initialize();
			PyRun_SimpleString("import sys");
  			PyRun_SimpleString("sys.path.append(\".\")");
		}

		~CPyInstance() {
			Py_Finalize();
		}
};

class CPyObject {
	private:
		PyObject* pyObj;
	public:
		CPyObject(): pyObj(NULL) {}

		CPyObject(PyObject* p) : pyObj(p) {}

		~CPyObject() {
			Release();
		}	

		PyObject* GetObject() {
			return pyObj;
		}

		PyObject* SetObject(PyObject* p) {
			return (pyObj=p);
		}

		PyObject* AddRef() {
			if(pyObj) {
				Py_INCREF(pyObj);
			}
			return pyObj;
		}

		void Release() {
			if(pyObj) {
				Py_DECREF(pyObj);
			}
			pyObj = NULL;
		}

		PyObject* operator ->() {
			return pyObj;
		}

		bool is() {
			return pyObj ? true : false;
		}

		operator PyObject*() {
			return pyObj;
		}

		PyObject* operator = (PyObject* p) {
			pyObj = p;
			return pyObj;
		}
};

#endif
