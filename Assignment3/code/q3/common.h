#pragma once

#include <string>
#include "complex.h"

typedef void(*comp_func)(complex_t* A, complex_t* x, complex_t* y, int n);
void add_function(comp_func f, std::string name, int flop);