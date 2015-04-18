
#include <cstdio>
#include <amp.h>

using namespace concurrency;

#define NUM 100

void add(float* a, float* b, float* c, unsigned int num) {
 
  array_view<float> aa(num,a);
  array_view<float> bb(num,b);
  array_view<float> cc(num,c);
 
  for (int i = 0; i < 10; i++) {
    parallel_for_each(extent<1>(num), [=](index<1> idx) {
      cc[idx] = aa[idx] + bb[idx];
    });
  }

}

int main(void) { 
  float a[NUM];
  float b[NUM];
  float c[NUM];

  for (unsigned int i = 0; i < NUM; i++) {
    a[i] = (float)i;
    b[i] = i * 10.0f;
  }

  add(a,b,c,NUM);
  
  int errors = 0;
  for (unsigned int i = 0; i < NUM; i++) {
    if (c[i] != (a[i]+b[i]))
      errors++;
  }

  printf("%d errors\n",errors);

  return errors;
}
