#include "stdafx.h"
#include "EngineWrapper.h"
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#include "numpy/arrayobject.h"
#include <iostream>
#include "Board.h"

wchar_t *nstrws_convert(char *raw) {
    wchar_t *rtn = (wchar_t *)calloc(1, (sizeof(wchar_t) * (strlen(raw) + 1)));
    setlocale(LC_ALL, "en_US.UTF-8");
    mbstowcs(rtn, raw, strlen(raw));
    return rtn;
}

int EngineWrapper::runPython(int argc, char **argv)
{
    // Set up python
    Py_Initialize();

    import_array();
    wchar_t *program_name = nstrws_convert(argv[0]);
    Py_SetProgramName(program_name);
    PySys_SetArgv(0, &program_name);
    return 0;
}

void EngineWrapper::initModule()
{
    auto pName = PyUnicode_DecodeFSDefault("restoreTest");
    module = PyImport_Import(pName);
    func = PyObject_GetAttrString(module, "go");
}

float* EngineWrapper::value(Board board)
{
    return run(board.calcArray());
}

// This method is extremely unsafe.
float *EngineWrapper::run(std::array<int, 36> input)
{
    float test[36];
    for (int i = 0; i < 36; i++) {
        test[i] = input[i];
    }
    npy_intp dims[1]{ 36 };

    auto np_arg = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(
        1, dims, NPY_FLOAT, reinterpret_cast<void*>(test)));
    auto pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, reinterpret_cast<PyObject*>(np_arg));

    auto pValue = PyObject_CallObject(func, pArgs);
    auto np_ret = reinterpret_cast<PyArrayObject*>(pValue);
    auto out = reinterpret_cast<float*>(PyArray_DATA(np_ret));

    return out;
}

EngineWrapper::EngineWrapper(int argc, char **argv)
{
    runPython(argc, argv);
    initModule();
}


EngineWrapper::~EngineWrapper()
{
}
