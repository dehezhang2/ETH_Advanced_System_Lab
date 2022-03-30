#include <immintrin.h>
#include "common.h"

void slow_performance1(double* x, double* y, int n) {
    for (int i = 0; i < n; i++) {
        if (fabs(x[i]) > fabs(y[i])) {
            y[i] += x[i] + 2*i;
        }
        else {
            y[i] = x[i]*x[i] - y[i];
        }
    }
}

void maxperformance(double* x, double* y, int n) {
    __m256d x_vec, y_vec, incr, ind, y_a, y_b, mask;
    ind = _mm256_set_pd(6, 4, 2, 0);
    incr = _mm256_set1_pd(8);
    for(int i = 0; i + 3 < n; i+=4){
        x_vec = _mm256_load_pd(x + i);
        y_vec = _mm256_load_pd(y + i);

        y_a = _mm256_add_pd(_mm256_add_pd(x_vec, ind), y_vec);
        y_b = _mm256_fmsub_pd(x_vec, x_vec, y_vec);

        mask = _mm256_cmp_pd(
            _mm256_mul_pd(x_vec, x_vec),
            _mm256_mul_pd(y_vec, y_vec),
            _CMP_GT_OQ
        );
        y_a = _mm256_and_pd(mask, y_a);
        y_b = _mm256_andnot_pd(mask, y_b);
        _mm256_store_pd(y + i, _mm256_or_pd(y_a, y_b));
        ind = _mm256_add_pd(ind, incr);
    }
}



/*
* Called by the driver to register your functions
* Use add_function(func, description) to add your own functions
*/
void register_functions() {
  add_function(&slow_performance1, "slow_performance1",1);
  add_function(&maxperformance, "maxperformance",1);
}