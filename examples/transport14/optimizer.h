#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "gams.h"

#include <string>

class Optimizer
{
public:
    Optimizer(int argc, char *argv[]);

    double solve(double mult);

private:
    std::string getModelText();

private:
    gams::GAMSWorkspace ws;
};

#endif // OPTIMIZER_H
