
#include <cstdio>
#include <amp.h>

// Number of array elements
#ifndef NUM
#define NUM (16 * 1024)
#endif

#ifndef USE_AMP_ATTRIBUTE
#define RESTRICT_AMP restrict(amp)
#else
#define RESTRICT_AMP __attribute__((amp))
#endif

using namespace concurrency;

int main(void) { 

  float a[NUM];
  float b[NUM];
  float c[NUM];
  for (unsigned int i = 0; i < NUM; i++) {
    a[i] = (float)i;
    b[i] = i * 10.0f;
  }

  // create array views from host buffers
  array_view<float> aa(NUM,a);
  array_view<float> bb(NUM,b);
  array_view<float> cc(NUM,c);

  // specify the compute grid size and dimension
  extent<1> grid(NUM);

  // AMP style parallel for construct
  parallel_for_each(grid, [=](index<1> idx) RESTRICT_AMP {
    cc[idx] = aa[idx] + bb[idx];
  });

  // synchronize the host buffer with updated data from the view
  cc.synchronize();

  int errors = 0;
  for (unsigned int i = 0; i < NUM; i++) {
    if (c[i] != (a[i]+b[i]))
      errors++;
  }

  printf("%d errors\n",errors);

  return errors;
}
