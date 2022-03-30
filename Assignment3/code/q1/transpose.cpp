#include <immintrin.h>
#include "common.h"
#include "complex.h"

void slow_performance1(complex_t x[4][4], complex_t y[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            y[i][j].re =   x[j][i].re;
            y[i][j].im =  -x[j][i].im;
        }
    }
}

void slow_performance2(complex_t x[4][4], complex_t y[4][4]){
    //  vectorize the inner loop
    __m256d x_01, x_23, coef = _mm256_set_pd(-1, 1, -1, 1);
    for(int i = 0; i < 4; i++){
        x_01 = _mm256_set_pd(x[1][i].im, x[1][i].re, x[0][i].im, x[0][i].re);
        x_23 = _mm256_set_pd(x[3][i].im, x[3][i].re, x[2][i].im, x[2][i].re);
        x_01 = _mm256_mul_pd(x_01, coef); x_23 = _mm256_mul_pd(x_23, coef);
        _mm256_store_pd((double *)*(y+i), x_01);
        _mm256_store_pd((double *)*(y + i) + 4, x_23);
    }
}


void maxperformance(complex_t x[4][4], complex_t y[4][4]) {
    const char SWITCH_COMPLEX = 0b01001110; // 0, 1, 2, 3 => 2, 3, 0, 1
    const char BLEND_MASK_SMALL = 0b1100;   // (a, b, c, d), (1, 2, 3, 4) => a, b, 3, 4
    const char BLEND_MASK_BIG = 0b0011;     // (a, b, c, d), (1, 2, 3, 4) => 1, 2, c, d
    __m256d x_0, x_1, x_2, x_3, yi_0, yi1_0, yi_2, yi1_2, coef = _mm256_set_pd(-1, 1, -1, 1);
    for(int i = 0; i + 1 < 4; i+=2){
        x_0 = _mm256_load_pd((double*)&x[0][i]);
        x_1 = _mm256_load_pd((double*)&x[1][i]);
        x_2 = _mm256_load_pd((double*)&x[2][i]);
        x_3 = _mm256_load_pd((double*)&x[3][i]);

        x_0 = _mm256_mul_pd(x_0, coef); x_1 = _mm256_mul_pd(x_1, coef);
        x_2 = _mm256_mul_pd(x_2, coef); x_3 = _mm256_mul_pd(x_3, coef);

        x_1 = _mm256_permute4x64_pd(x_1, SWITCH_COMPLEX);
        x_3 = _mm256_permute4x64_pd(x_3, SWITCH_COMPLEX);

        yi_0 = _mm256_blend_pd(x_0, x_1, BLEND_MASK_SMALL);
        yi1_0 = _mm256_blend_pd(x_0, x_1, BLEND_MASK_BIG);
        yi_2 = _mm256_blend_pd(x_2, x_3, BLEND_MASK_SMALL);
        yi1_2 = _mm256_blend_pd(x_2, x_3, BLEND_MASK_BIG);

        yi1_0 = _mm256_permute4x64_pd(yi1_0, SWITCH_COMPLEX);
        yi1_2 = _mm256_permute4x64_pd(yi1_2, SWITCH_COMPLEX);

        _mm256_store_pd((double*)(&y[i][0]), yi_0);
        _mm256_store_pd((double*)(&y[i + 1][0]), yi1_0);
        _mm256_store_pd((double*)(&y[i][2]), yi_2);
        _mm256_store_pd((double*)(&y[i + 1][2]), yi1_2);
    }
}

/*
* Called by the driver to register your functions
* Use add_function(func, description) to add your own functions
*/
void register_functions() {
  add_function(&slow_performance1, "slow_performance1",1);
  add_function(&slow_performance2, "slow_performance2",1);
  add_function(&maxperformance, "maxperformance",1);
}