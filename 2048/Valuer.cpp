#include "stdafx.h"
#include "Valuer.h"


Valuer::Valuer(EngineWrapper *inWrapper, float inValueWeight)
{
    wrapper = inWrapper;
    valueWeight = inValueWeight;
}


Valuer::~Valuer()
{
}

float Valuer::value(Board b, Move m)
{
    int index = static_cast<int>(m);
    return wrapper->value(b)[index] * valueWeight;
}
