#pragma once
#include "EngineWrapper.h"

class Valuer
{
private:
    EngineWrapper *wrapper;
    float valueWeight;

public:
    Valuer(EngineWrapper *wrapper, float valueWeight);
    ~Valuer();
    float value(Board b, Move m);
};

