#include "include/microbenchmark.h"
#include "include/tsc_x86.h"
#include "include/foo.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#define CYCLES_REQUIRED 1e8
#define NUM_REP 40
#define ARRAY_LEN 1000

double t0, t1, t2, t3, t4, t5, t6, t7;

void initialize_microbenchmark_data (microbenchmark_mode_t mode) {
  /* You can use to initialize some date if needed */
    switch (mode) {
        case FMA_LAT:
            srand(time(NULL)); 
            t0 = rand();
            break;
        case FMA_GAP:
            srand(time(NULL)); 
            t0 = rand();t1 = rand();t2 = rand();t3 = rand();
            t4 = rand();t5 = rand();t6 = rand();t7 = rand();
            break;
        case SQRT_LAT:
            t0 = 0.1;
            break;
        case SQRT_GAP:
            t0 = 0.1;t1 = 0.1;t2 = 0.1;t3 = 0.1;
            t4 = 0.1;t5 = 0.1;t6 = 0.1;t7 = 0.1;
            break;
        case SQRT_LAT_MIN:
            srand(time(NULL)); 
            t0 = rand();
            break;
        case SQRT_GAP_MIN:
            srand(time(NULL)); 
            t0 = rand();t1 = rand();t2 = rand();t3 = rand();
            t4 = rand();t5 = rand();t6 = rand();t7 = rand();
            break;
        case FOO_LAT:
            t0 = 0.1;
            break;
        case FOO_GAP:
            t0 = 0.1;t1 = 0.1;t2 = 0.1;t3 = 0.1;
            t4 = 0.1;t5 = 0.1;t6 = 0.1;t7 = 0.1;
            break;
        case FOO_LAT_MIN:
            t0 = 0;
            break;
        case FOO_GAP_MIN:
            t0 = 0;t1 = 0;t2 = 0;t3 = 0;
            t4 = 0;t5 = 0;t6 = 0;t7 = 0;
            break;
        default: break;
    }
}

double microbenchmark_get_fma_latency() {
    /* Implement your microbenchmark benchmark here */
    myInt64 cycles;
    myInt64 start;
    int num_runs = (1 << 10);
    start = start_tsc();
    for(int i = 0; i < num_runs; i++){
        t0 += t0 * t0;
    }
    cycles = stop_tsc(start) / num_runs;
    return cycles;
}

double microbenchmark_get_fma_gap() {
    /* Implement your microbenchmark benchmark here */
    double gap;
    myInt64 start;
    int num_runs = (1 << 10);

    start = start_tsc();
    for(int i = 0; i < num_runs; i++){
        t0 += t0 * t0;
        t1 += t1 * t1;
        t2 += t2 * t2;
        t3 += t3 * t3;
        t4 += t4 * t4;
        t5 += t5 * t5;
        t6 += t6 * t6;
        t7 += t7 * t7;
    }
    double cycle = stop_tsc(start);
    gap = cycle / (8.0*num_runs);
    return gap;
}

double microbenchmark_get_sqrt_latency() {
    /* Implement your microbenchmark benchmark here */
    myInt64 cycles;
    myInt64 start;
    int num_runs = (1 << 10);
    start = start_tsc();
    for(int i = 0; i < num_runs; i++){
        t0 = sqrtsd(t0);
    }
    printf("%lf\n", t0);
    cycles = stop_tsc(start) / num_runs;
    return (double)cycles;
}

double microbenchmark_get_sqrt_gap() {
    /* Implement your microbenchmark benchmark here */
    double gap;
    myInt64 start;
    int num_runs = (1 << 10);

    start = start_tsc();
    for(int i = 0; i < num_runs; i++){
        t0 = sqrtsd(t0);
        t1 = sqrtsd(t1);
        t2 = sqrtsd(t2);
        t3 = sqrtsd(t3);
        t4 = sqrtsd(t4);
        t5 = sqrtsd(t5);
        t6 = sqrtsd(t6);
        t7 = sqrtsd(t7);
    }
    double cycle = stop_tsc(start);
    gap = cycle / (8.0*num_runs);
    return gap;
}

double microbenchmark_get_foo_latency() {
    /* Implement your microbenchmark benchmark here */
    myInt64 cycles;
    myInt64 start;
    int num_runs = (1 << 10);
    start = start_tsc();
    for(int i = 0; i < num_runs; i++){
        t0 = foo(t0, t0);
    }
    cycles = stop_tsc(start) / num_runs;
    return (double)cycles;
}

double microbenchmark_get_foo_gap() {
    /* Implement your microbenchmark benchmark here */
    double gap;
    myInt64 start;
    int num_runs = (1 << 10);

    start = start_tsc();
    for(int i = 0; i < num_runs; i++){
        t0 = foo(t0, t0);
        t1 = foo(t1, t0);
        t2 = foo(t2, t2);
        t3 = foo(t3, t3);
        t4 = foo(t4, t4);
        t5 = foo(t5, t5);
        t6 = foo(t6, t6);
        t7 = foo(t7, t7);
    }
    double cycle = stop_tsc(start);
    gap = cycle / (8.0*num_runs);
    return gap;
}