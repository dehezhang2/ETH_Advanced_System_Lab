#include <iostream>
#include <iomanip>
#include <cmath>
#include "include/microbenchmark.h"

using namespace std;

typedef double (*funPtr_t)();

void run_microbenchmark(microbenchmark_mode_t test) {
    funPtr_t f;
    
    string test_name;
    switch (test) {
      case FMA_LAT        : test_name = "fma latency       "; f = microbenchmark_get_fma_latency ; break;
      case FMA_GAP        : test_name = "fma gap           "; f = microbenchmark_get_fma_gap     ; break;
      case SQRT_LAT       : test_name = "sqrt latency      "; f = microbenchmark_get_sqrt_latency; break;
      case SQRT_GAP       : test_name = "sqrt gap          "; f = microbenchmark_get_sqrt_gap    ; break;
      case SQRT_LAT_MIN   : test_name = "sqrt latency (min)"; f = microbenchmark_get_sqrt_latency; break;
      case SQRT_GAP_MIN   : test_name = "sqrt gap     (min)"; f = microbenchmark_get_sqrt_gap    ; break;
      case FOO_LAT        : test_name = "foo latency       "; f = microbenchmark_get_foo_latency ; break;
      case FOO_GAP        : test_name = "foo gap           "; f = microbenchmark_get_foo_gap     ; break;
      case FOO_LAT_MIN    : test_name = "foo latency (min) "; f = microbenchmark_get_foo_latency ; break;
      case FOO_GAP_MIN    : test_name = "foo gap     (min) "; f = microbenchmark_get_foo_gap     ; break;
      default: cout << "Out of range" << endl;  return;
    }
    
    /* Initialize and run microbenchmark */
    initialize_microbenchmark_data(test);
    double cycles = f();
    
    /* Print results */
    cout << "Measured "  << test_name << " : ";
    cout << "\033[1;35m" << setw(8)  << cycles << "\033[0m" << " cyc";
    cout << endl;
}

int main () {
    for (int i = START_TEST; i <= END_TEST; ++i) {
      auto test = (microbenchmark_mode_t) i;
      run_microbenchmark(test);
    }    
    cout << "End of benchmark" << endl;
    return 0;
}