

#include <cstdlib>
#include <cstring>
#include <cmath>

#include <vector>

#include "module.hpp"

#include <amp.h>
using namespace concurrency;


#define NUM   (16*1024*1024)

int main(int argc, char* argv[]) {

  float *a, *b, *c;
  a = new float[NUM];
  b = new float[NUM];
  c = new float[NUM];
  memset(a, 0, NUM * sizeof(float));
  for (unsigned int i = 0; i < NUM; i++) {
    b[i] = (float)i;
    c[i] = (float)(i*100);
  }

  int flag = 0;
  array_view<int> flag_v(extent<1>(1),&flag);
  parallel_for_each(extent<1>(NUM), [=] (index<1> id) restrict(amp) {
    a[id[0]] = b[id[0]] + c[id[0]];

    if (id[0] == NUM-1)
      flag_v[0] = 1;
  });
  flag_v.synchronize_async().wait();


  for (unsigned int i = 0; i < NUM; i++) {
    float expected = b[i] + c[i];
    if (fabs(a[i] - expected) >= 0.001f)  {
      printf("error a[%d]=%f, expected %f\n",i,a[i],expected);
    }
  }



  float *aa;
  aa = new float[NUM];
  memset(aa, 0, NUM * sizeof(float));
  calculate(aa, b, c, NUM);

  for (unsigned int i = 0; i < NUM; i++) {
    float expected = b[i] + c[i];
    if (fabs(aa[i] - expected) >= 0.001f)  {
      printf("error a[%d]=%f, expected %f\n",i,a[i],expected);
    }
  }


  delete[] a;
  delete[] b;
  delete[] c;
  delete[] aa;
  return 0;
}


