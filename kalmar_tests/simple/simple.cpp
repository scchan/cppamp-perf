
#include <cstdio>
#include <amp.h>


// Number of array elements
#ifndef NUM
#define NUM (16 * 1024)
#endif


// Number of iterations
#ifndef ITER
#define ITER 10
#endif


#ifdef USE_RESTRICT
#define RESTRICT_AMP restrict(amp)
#else
#define RESTRICT_AMP
#endif


using namespace concurrency;


#ifdef USE_ARRAY_VIEW

void add(float* a, float* b, float* c, unsigned int num) {
 
  array_view<float> aa(num,a);
  array_view<float> bb(num,b);
  array_view<float> cc(num,c);
 
  for (int i = 0; i < ITER; i++) {
    parallel_for_each(extent<1>(num), [=](index<1> idx) RESTRICT_AMP {
      cc[idx] = aa[idx] + bb[idx];
    });
  }
  completion_future aaf = aa.synchronize_async();
  completion_future bbf = bb.synchronize_async();
  completion_future ccf = cc.synchronize_async();

  aaf.wait();
  bbf.wait();
  ccf.wait();
}

#else

void add_array(float* a, float* b, float* c, unsigned int num) {

  accelerator accelerator(accelerator::default_accelerator);
  accelerator_view av = accelerator.create_view();

  array<float,1> aa(num,av);
  array<float,1> bb(num,av);
  array<float,1> cc(num,av);
 
#if 0
  completion_future fa = copy_async(a, a+num, aa);
  completion_future fb = copy_async(b, b+num, bb);
  fa.wait();
  fb.wait();
#endif

  copy(a, a+num, aa);
  copy(b, b+num, bb);


  for (int i = 0; i < ITER; i++) {
    parallel_for_each (extent<1>(num), [&](index<1> idx) RESTRICT_AMP {
      cc[idx] = aa[idx] + bb[idx];
    });
  }

#if 0
  copy_async(cc,c).wait();
#endif
  copy(cc, c);

  return;
}

#endif


int main(void) { 
  float a[NUM];
  float b[NUM];
  float c[NUM];

  for (unsigned int i = 0; i < NUM; i++) {
    a[i] = (float)i;
    b[i] = i * 10.0f;
  }

#ifdef USE_ARRAY_VIEW
  add(a,b,c,NUM);
#else
  add_array(a,b,c,NUM);
#endif

  int errors = 0;
  for (unsigned int i = 0; i < NUM; i++) {
    if (c[i] != (a[i]+b[i]))
      errors++;
  }

  printf("%d errors\n",errors);

  return errors;
}
