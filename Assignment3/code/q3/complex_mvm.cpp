#include <immintrin.h>
#include "common.h"
#include "complex.h"

void slow_performance1(complex_t* A, complex_t* x, complex_t* y, int n) {
    complex_t t, sum;
    for (int i = 0; i < n; i++) {
        sum = {0.0, 0.0};
        for (int j = 0; j < n; j++) {
            t = mul(A[i*n + j], x[j]);
            sum = add(sum, t);
        }
        y[i] = sum;
    }
}

void slow_performance2(complex_t* A, complex_t* x, complex_t* y, int n) {
    __m256d A_vec, x_vec, twos, coef, x_re, x_im, A_im_re, r, sum;
    twos = _mm256_set1_pd(2);
    coef = _mm256_set_pd(2, -1, 2, -1);
    double sum_array[] = {0.0, 0.0, 0.0, 0.0};
    for(int i = 0; i < n; i++){
        sum = _mm256_setzero_pd();
        for(int j = 0; j + 1 < n; j+=2){
            A_vec = _mm256_load_pd((double*)(A + i * n + j));
            x_vec = _mm256_load_pd((double*)(x + j));
            A_im_re = _mm256_permute_pd(A_vec, 0b0101);
            x_re = _mm256_permute_pd(x_vec, 0b0000);
            x_im = _mm256_permute_pd(x_vec, 0b1111);
            r = _mm256_fmadd_pd(
                _mm256_mul_pd(twos, A_vec),
                x_re,
                _mm256_mul_pd(_mm256_mul_pd(coef, A_im_re), x_im)
            );
            sum = _mm256_add_pd(sum, r);
        }
        _mm256_store_pd(sum_array, sum);
        y[i].re = sum_array[0] + sum_array[2];
        y[i].im = sum_array[1] + sum_array[3];
    }
}

void slow_performance3(complex_t* A, complex_t* x, complex_t* y, int n) {
    __m256d twos, coef;
    __m256d x_vec, x_re, x_im, sum01, sum23;
    __m256d A0_vec, A0_im_re, r0, sum0;
    __m256d A1_vec, A1_im_re, r1, sum1;
    __m256d A2_vec, A2_im_re, r2, sum2;
    __m256d A3_vec, A3_im_re, r3, sum3;
    twos = _mm256_set1_pd(2);
    coef = _mm256_set_pd(2, -1, 2, -1);
    for(int i = 0; i + 3 < n; i += 4){
        sum0 = _mm256_setzero_pd();
        sum1 = _mm256_setzero_pd();
        sum2 = _mm256_setzero_pd();
        sum3 = _mm256_setzero_pd();
        for(int j = 0; j + 1 < n; j+=2){
            A0_vec = _mm256_load_pd((double*)(A + i * n + j));
            A1_vec = _mm256_load_pd((double*)(A + (i + 1) * n + j));
            A2_vec = _mm256_load_pd((double*)(A + (i + 2) * n + j));
            A3_vec = _mm256_load_pd((double*)(A + (i + 3) * n + j));

            x_vec = _mm256_load_pd((double*)(x + j));
            x_re = _mm256_permute_pd(x_vec, 0b0000);
            x_im = _mm256_permute_pd(x_vec, 0b1111);

            A0_im_re = _mm256_permute_pd(A0_vec, 0b0101);
            A1_im_re = _mm256_permute_pd(A1_vec, 0b0101);
            A2_im_re = _mm256_permute_pd(A2_vec, 0b0101);
            A3_im_re = _mm256_permute_pd(A3_vec, 0b0101);
            
            r0 = _mm256_fmadd_pd(
                _mm256_mul_pd(twos, A0_vec),
                x_re,
                _mm256_mul_pd(_mm256_mul_pd(coef, A0_im_re), x_im)
            );
            r1 = _mm256_fmadd_pd(
                _mm256_mul_pd(twos, A1_vec),
                x_re,
                _mm256_mul_pd(_mm256_mul_pd(coef, A1_im_re), x_im)
            );
            r2 = _mm256_fmadd_pd(
                _mm256_mul_pd(twos, A2_vec),
                x_re,
                _mm256_mul_pd(_mm256_mul_pd(coef, A2_im_re), x_im)
            );
            r3 = _mm256_fmadd_pd(
                _mm256_mul_pd(twos, A3_vec),
                x_re,
                _mm256_mul_pd(_mm256_mul_pd(coef, A3_im_re), x_im)
            );

            sum0 = _mm256_add_pd(sum0, r0);
            sum1 = _mm256_add_pd(sum1, r1);
            sum2 = _mm256_add_pd(sum2, r2);
            sum3 = _mm256_add_pd(sum3, r3);
        }
        sum01 = _mm256_hadd_pd(
            _mm256_permute4x64_pd(sum0, 0b11011000), 
            _mm256_permute4x64_pd(sum1, 0b11011000)
        );
        sum23 = _mm256_hadd_pd(
            _mm256_permute4x64_pd(sum2, 0b11011000), 
            _mm256_permute4x64_pd(sum3, 0b11011000)
        );
        sum01 = _mm256_permute4x64_pd(sum01, 0b11011000);
        sum23 = _mm256_permute4x64_pd(sum23, 0b11011000);
        _mm256_store_pd((double*)(y + i), sum01);
        _mm256_store_pd((double*)(y + i + 2), sum23);
    }
}

void maxperformance(complex_t* A, complex_t* x, complex_t* y, int n) {
    // __m256d A_vec, x_vec, coef;
    // coef = _mm256_set_pd();
    // for(int i = 0; i < n; i++){
    //     for(int j = 0; j + 1 < n; j+=2){
    //         A_vec = _mm256_load_pd((double*)(A + i * n + j));
    //         x_vec = _mm256_load_pd((double*)(x + j));

    //     }
    // }
}


/*
* Called by the driver to register your functions
* Use add_function(func, description) to add your own functions
*/
void register_functions() {
  add_function(&slow_performance1, "slow_performance1",1);
  add_function(&slow_performance2, "slow_performance2",1);
  add_function(&slow_performance3, "slow_performance3",1);
  add_function(&maxperformance, "maxperformance",1);
}