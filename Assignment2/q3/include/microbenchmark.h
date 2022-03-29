#ifndef MICROBENCHMARK_H
#define MICROBENCHMARK_H

typedef enum {
    FMA_LAT,
    FMA_GAP,
    SQRT_LAT,
    SQRT_GAP,
    SQRT_LAT_MIN,
    SQRT_GAP_MIN,
    FOO_LAT,
    FOO_GAP,
    FOO_LAT_MIN,
    FOO_GAP_MIN,
    
    /* Limits */
    START_TEST = FMA_LAT,
    END_TEST = FOO_GAP_MIN,
} microbenchmark_mode_t;


void    initialize_microbenchmark_data(microbenchmark_mode_t mode);
double  microbenchmark_get_fma_latency ();
double  microbenchmark_get_fma_gap     ();
double  microbenchmark_get_sqrt_latency();
double  microbenchmark_get_sqrt_gap    ();
double  microbenchmark_get_sqrt_latency();
double  microbenchmark_get_sqrt_gap    ();
double  microbenchmark_get_foo_latency ();
double  microbenchmark_get_foo_gap     ();


#endif /* MICROBENCHMARK_H */
