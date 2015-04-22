
#include <cstdio>
#include <amp.h>




#ifdef USE_RESTRICT
#define RESTRICT_AMP restrict(amp)
#else
#define RESTRICT_AMP
#endif

using namespace concurrency;

#define NUM 100


#ifdef USE_ARRAY_VIEW

void add(float* a, float* b, float* c, unsigned int num) {
 
  array_view<float> aa(num,a);
  array_view<float> bb(num,b);
  array_view<float> cc(num,c);
 
  for (int i = 0; i < 10; i++) {
    parallel_for_each(extent<1>(num), [=](index<1> idx) RESTRICT_AMP {
      cc[idx] = aa[idx] + bb[idx];
    });
  }

}

#else

void add_array(float* a, float* b, float* c, unsigned int num) {

  accelerator accelerator(accelerator::default_accelerator);
  a

  array<float> aa(num);
  array<float> bb(num);
  array<float> cc(num);
 
  completion_future fa = copy_async(a, a+num, aa);
  completion_future fb = copy_async(b, b+num, bb);

  fa.wait();
  fb.wait();

  for (int i = 0; i < 10; i++) {
    parallel_for_each (extent<1>(num), [=,&aa,&bb,&cc](index<1> idx) RESTRICT_AMP {
      cc[idx] = aa[idx] + bb[idx];
    });
  }

  copy_async(cc,c).wait();
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
