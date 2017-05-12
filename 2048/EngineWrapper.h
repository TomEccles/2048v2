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
    float* getMoveLikelihoods(Board board);
    float getValue(Board board);


private:
    float getValue(std::array<int, 37> input);
    int runPython(int argc, char **argv);
    float* getMoveLikelihoods(std::array<int, 36> input);
    void initModule();
    PyObject *predictFunction;
    PyObject *valueFunction;
};

