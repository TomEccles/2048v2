#pragma once
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#include <array>
#include "Board.h"

class EngineWrapper
{
public:
    EngineWrapper(int argc, char **argv);
    ~EngineWrapper();
    float* value(Board board);


private:
    int runPython(int argc, char **argv);
    float* run(std::array<int, 36> input);
    void initModule();
    PyObject *module;
    PyObject *func;
};

