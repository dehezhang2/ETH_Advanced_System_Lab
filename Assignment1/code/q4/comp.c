//#error Please comment out the next two lines under linux, then comment this error
//#include "stdafx.h"  //Visual studio expects this line to be the first one, comment out if different compiler
//#include <windows.h> // Include if under windows

#ifndef WIN32
#include <sys/time.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef __x86_64__
#include "tsc_x86.h"
#endif

#define NUM_RUNS 30
#define CYCLES_REQUIRED 1e8
#define FREQUENCY 2.7e9
#define CALIBRATE

/*
 *	Initialize the input
 */

void fill_vector(double * x, int n) {
    srand(time(NULL));
    for(int i=0; i < n; i++) {
        x[i] = rand();
    }
}

/* 
 * Comp function
 */

void compute(double *x, double *y, int n) {
    double s = 0.0;
    for (int i = 0; i < n; i++) {
        s += x[i] * x[i] + y[i]; 
    }
    x[0] = s; 
}

/* 
 * Comparison function for sorting
 */

int cmpfunc (const void * a, const void * b) {
   return ( *(myInt64*)a - *(myInt64*)b );
}

/* 
 * Timing function based on the TimeStep Counter of the CPU.
 */

#ifdef __x86_64__
double rdtsc(double A[], double B[], int n) {
    int i, num_runs;
    myInt64 cycles;
    myInt64 start;
    num_runs = NUM_RUNS;

    /* 
     * The CPUID instruction serializes the pipeline.
     * Using it, we can create execution barriers around the code we want to time.
     * The calibrate section is used to make the computation large enough so as to 
     * avoid measurements bias due to the timing overhead.
     */
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {
        start = start_tsc();
        for (i = 0; i < num_runs; ++i) {
            compute(A, B, n);
        }
        cycles = stop_tsc(start);

        if(cycles >= CYCLES_REQUIRED) break;

        num_runs *= 2;
    }
#endif

    start = start_tsc();
    for (i = 0; i < num_runs; ++i) {
        compute(A, B, n);
    }

    cycles = stop_tsc(start)/num_runs;
    return (double) cycles;
}
#endif

double c_clock(double A[], double B[], int n) {
    int i, num_runs;
    double cycles;
    clock_t start, end;

    num_runs = NUM_RUNS;
// #ifdef CALIBRATE
//     while(num_runs < (1 << 14)) {
//         start = clock();
//         for (i = 0; i < num_runs; ++i) {
//             compute(A, B, n);
//         }
//         end = clock();

//         cycles = (double)(end-start);

//         // Same as in c_clock: CYCLES_REQUIRED should be expressed accordingly to the order of magnitude of CLOCKS_PER_SEC
//         if(cycles >= CYCLES_REQUIRED/(FREQUENCY/CLOCKS_PER_SEC)) break;

//         num_runs *= 2;
//     }
// #endif
    

    myInt64* cycles_list = (myInt64 *)malloc(num_runs*sizeof(myInt64));
    for (i = 0; i < num_runs; ++i) {
        start = clock();
        compute(A, B, n);
        end = clock();
        cycles_list[i] = end - start;
    }
    // for(int i = 0; i < num_runs; i++){
    //     printf("%d ", cycles_list[i]);
    // }
    qsort(cycles_list, num_runs, sizeof(myInt64), cmpfunc);
    return ((double) cycles_list[num_runs/2] + (double) cycles_list[num_runs/2-1])/2.0;
}

#ifndef WIN32
double timeofday(double A[], double B[], int n) {
    int i, num_runs;
    double cycles;
    struct timeval start, end;

    num_runs = NUM_RUNS;
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {
        gettimeofday(&start, NULL);
        for (i = 0; i < num_runs; ++i) {
            compute(A, B, n);
        }
        gettimeofday(&end, NULL);

        cycles = (double)((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1e6)*FREQUENCY;

        if(cycles >= CYCLES_REQUIRED) break;

        num_runs *= 2;
    }
#endif

    gettimeofday(&start, NULL);
    for(i=0; i < num_runs; ++i) {
        compute(A, B, n);
    }
    gettimeofday(&end, NULL);

    return (double)((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1e6)/ num_runs;
}

#else

double gettickcount(double A[], double B[], int n) {
    int i, num_runs;
    double cycles, start, end;

    num_runs = NUM_RUNS;
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {
        start = (double)GetTickCount();
        for (i = 0; i < num_runs; ++i) {
            compute(A, B, n);
        }
        end = (double)GetTickCount();

        cycles = (end-start)*FREQUENCY/1e3; // end-start provides a measurement in the order of milliseconds

        if(cycles >= CYCLES_REQUIRED) break;

        num_runs *= 2;
    }
#endif

    start = (double)GetTickCount();
    for(i=0; i < num_runs; ++i) {
        compute(A, B, n); 
    }
    end = (double)GetTickCount();

    return (end-start)/num_runs;
}

double queryperfcounter(double A[], double B[], int n, LARGE_INTEGER f) {
    int i, num_runs;
    double cycles;
    LARGE_INTEGER start, end;

    num_runs = NUM_RUNS;
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {
        QueryPerformanceCounter(&start);
        for (i = 0; i < num_runs; ++i) {
            compute(A, B, n);
        }
        QueryPerformanceCounter(&end);

        cycles = (double)(end.QuadPart - start.QuadPart);

        // Same as in c_clock: CYCLES_REQUIRED should be expressed accordingly to the order of magnitude of f
        if(cycles >= CYCLES_REQUIRED/(FREQUENCY/f.QuadPart)) break; 

        num_runs *= 2;
    }
#endif

    QueryPerformanceCounter(&start);
    for(i=0; i < num_runs; ++i) {
        compute(A, B, n); 
    }
    QueryPerformanceCounter(&end);

    return (double)(end.QuadPart - start.QuadPart)/num_runs;
}

#endif

int main(int argc, char **argv) {
    if (argc!=2) {printf("usage: FW <n>\n"); return -1;}
    int n = atoi(argv[1]);
    printf("n=%d \n",n);
    double* x = (double *)malloc(n*sizeof(double));
    double* y = (double *)malloc(n*sizeof(double));
    fill_vector(x, n);
    fill_vector(y, n);
#ifdef __x86_64__
    myInt64* cycles_list = (myInt64 *)malloc(30*sizeof(myInt64));
    for(int i = 0; i < 30; i++){
        cycles_list[i] = rdtsc(x, y, n);
        
    }
    
    qsort(cycles_list, 30, sizeof(myInt64), cmpfunc);
    double r = (cycles_list[14] + cycles_list[15])/2.0;
    printf("RDTSC instruction:\n %lf cycles measured => %lf seconds, assuming frequency is %lf MHz. (change in source file if different)\n\n", r, r/(FREQUENCY), (FREQUENCY)/1e6);
#endif

    return 0;
}
