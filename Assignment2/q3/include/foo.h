static double sqrtsd (double x) {
    double r;
    __asm__ ("sqrtsd %1, %0" : "=x" (r) : "x" (x));
    return r;
}

static double foo(double a, double b) {
  return sqrtsd(a*a + 0.1*b);
}