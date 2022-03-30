#pragma once

typedef struct{
    double re;
    double im;
} complex_t;

static complex_t add(complex_t a, complex_t b) {
    complex_t r;
    r.re = a.re + b.re;
    r.im = a.im + b.im;
    return r;
}

static complex_t mul(complex_t a, complex_t b) {
    complex_t r;
    r.re = 2.0 * a.re * b.re - 1.0 * a.im * b.im;
    r.im = 2.0 * a.im * b.re + 2.0 * a.re * b.im;
    return r;
}