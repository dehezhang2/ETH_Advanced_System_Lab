/**
*      _________   _____________________  ____  ______
*     / ____/   | / ___/_  __/ ____/ __ \/ __ \/ ____/
*    / /_  / /| | \__ \ / / / /   / / / / / / / __/
*   / __/ / ___ |___/ // / / /___/ /_/ / /_/ / /___
*  /_/   /_/  |_/____//_/  \____/\____/_____/_____/
*
*  http://www.acl.inf.ethz.ch/teaching/fastcode
*  How to Write Fast Numerical Code 263-2300 - ETH Zurich
*  Copyright (C) 2019 
*                   Tyler Smith        (smitht@inf.ethz.ch) 
*                   Alen Stojanov      (astojanov@inf.ethz.ch)
*                   Gagandeep Singh    (gsingh@inf.ethz.ch)
*                   Markus Pueschel    (pueschel@inf.ethz.ch)
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program. If not, see http://www.gnu.org/licenses/.
*/

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "tsc_x86.h"
#include "utils.h"
#include "complex.h"

using namespace std;

#define NR 32
#define CYCLES_REQUIRED 1e8
#define REP 50
#define EPS (1e-3)

void kernel_base(complex_t x[4][4], complex_t y[4][4]) {
    for ( int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            y[i][j].re =   x[j][i].re;
            y[i][j].im =  -x[j][i].im;
        }
    }
}

/* prototype of the function you need to optimize */
typedef void(*comp_func)(complex_t x[4][4], complex_t y[4][4]);

void   register_functions();
double get_perf_score(comp_func f);
double perf_test(comp_func f, string desc, int flops);
void   add_function(comp_func f, string name, int flop);

/* Global vars, used to keep track of student functions */
vector<comp_func> userFuncs;
vector<string> funcNames;
vector<int> funcFlops;
int numFuncs = 0;

/*
* Registers a user function to be tested by the driver program. Registers a
* string description of the function as well
*/
void add_function(comp_func f, string name, int flops) {
    userFuncs.push_back(f);
    funcNames.emplace_back(name);
    funcFlops.push_back(flops);
    numFuncs++;
}


/*
* Checks the given function for validity. If valid, then computes and
* reports and returns the number of cycles required per iteration
*/
double perf_test(comp_func f, string desc, int flops) {
    double cycles = 0.;
    long num_runs = 100;
    double multiplier = 1;
    myInt64 start, end;

    alignas(32) complex_t x[4][4];
    alignas(32) complex_t y[4][4];
    rands((double*) x, 8, 4);
    rands((double*) y, 8, 4);

    // Warm-up phase: we determine a number of executions that allows
    // the code to be executed for at least CYCLES_REQUIRED cycles.
    // This helps excluding timing overhead when measuring small runtimes.
    do {
        num_runs = num_runs * multiplier;
        start = start_tsc();
        for (size_t i = 0; i < num_runs; i++) {
            f(x, y);           
        }
        end = stop_tsc(start);

        cycles = (double)end;
        multiplier = (CYCLES_REQUIRED) / (cycles);
        
    } while (multiplier > 2);

    list<double> cyclesList;

    // Actual performance measurements repeated REP times.
    // We simply store all results and compute medians during post-processing.
    double total_cycles = 0;
    for (size_t j = 0; j < REP; j++) {

        start = start_tsc();
        for (size_t i = 0; i < num_runs; ++i) {
            f(x, y);           
        }
        end = stop_tsc(start);

        cycles = ((double)end) / num_runs;
        total_cycles += cycles;

        cyclesList.push_back(cycles);
    }
    total_cycles /= REP;

    cycles = total_cycles;
    return  cycles; //round((100.0 * flops) / cycles) / 100.0;
}

int main(int argc, char **argv) {
  cout << "Starting program. ";
  double perf;
  int i;

  register_functions();

  if (numFuncs == 0){
    cout << endl;
    cout << "No functions registered - nothing for driver to do" << endl;
    cout << "Register functions by calling register_func(f, name)" << endl;
    cout << "in register_funcs()" << endl;

    return 0;
  }
  cout << numFuncs << " functions registered." << endl;
   
  //Check validity of functions.
  alignas(32) complex_t x[4][4];
  alignas(32) complex_t y[4][4];
  alignas(32) complex_t y_base[4][4];
  rands((double*) x, 8, 4);
  rands((double*) y, 8, 4);
  
  kernel_base(x, y_base);

  for (i = 0; i < numFuncs; i++) {
    comp_func f = userFuncs[i];
    f(x, y);
    
    double error = nrm_sqr_diff((double*) y, (double*) y_base, 32);
    if (error > EPS) {
      cout << error << endl;
      cout << "ERROR!!!!  the results for the " << i+1 << "th function are different to the previous" << std::endl;
    }
    rands((double*) y, 8, 4);
  }

  for (i = 0; i < numFuncs; i++) {
    perf = perf_test(userFuncs[i], funcNames[i], 1);
    cout << endl << "Running: " << funcNames[i] << endl;
    cout << perf << " cycles" << endl;
  }

  return 0;
}