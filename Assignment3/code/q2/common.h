#pragma once

#include <string>
#include "complex.h"

typedef void(*comp_func)(double* x, double* y, int n);
void add_function(comp_func f, std::string name, int flop);