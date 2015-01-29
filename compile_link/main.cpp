

#include <cstdlib>
#include <cstring>
#include <cmath>

#include <vector>

#include "module.hpp"

#include <amp.h>
using namespace concurrency;


#define NUM   (16*1024*1024)


unsigned int verify(float* inputA, float* inputB, float* output, int num) {
  unsigned int errors = 0;
  
  for (unsigned int i = 0; i < num; i++) {
    float expected = inputA[i] + inputB[i];
    if (fabs(output[i] - expected) >= 0.001f)  {
      //printf("error output[%d]=%f, expected %f\n",i,output[i],expected);
      errors++;
    }
  }

  return errors;
}


int main(int argc, char* argv[]) {

  int errors;
  float *a, *b, *c;
  a = new float[NUM];
  b = new float[NUM];
  c = new float[NUM];
  memset(a, 0, NUM * sizeof(float));
  for (unsigned int i = 0; i < NUM; i++) {
    b[i] = (float)i;
    c[i] = (float)(i*100);
  }
 
  // TEST 1 - SVM
  
  // NOTE: In C++AMP specification, the CPU synchronizes with parallel_for_each (pfe)
  // through an array_view object.  With SVM, a CPU pointer can be dereferenced
  // in a pfe directly (therefore, array_view is needed).  This creates a problem for 
  // pfe that only uses SVM because the CPU has no mechanism to synchronize with the
  // pfe anymore.  To ensure correctness, we create an artifial flag wrapped around
  // with a array_view for synchronization.  A new async_pfe is being designed to
  // solve this issue.
  int flag = 0;
  array_view<int> flag_v(extent<1>(1),&flag);

  // kernel using SVM
  parallel_for_each(extent<1>(NUM), [=] (index<1> id) restrict(amp) {
    a[id[0]] = b[id[0]] + c[id[0]];

    if (id[0] == NUM-1)
      flag_v[0] = 1;
  });
  flag_v.synchronize_async().wait();

  errors = verify(b,c,a,NUM);
  printf("Test 1 - SVM: %d errors\n", errors);



  // TEST 2 - Accelerated library
  // Calling an accelerated function in a different compilation unit

  memset(a, 0, NUM * sizeof(float));
  accelerated_lib(a, b, c, NUM);
  errors = verify(b,c,a,NUM);
  printf("Test 2 - Accelerated library: %d errors\n", errors);


  // TEST 3 - Function call in pfe
  // Calling a function that is in a different compilation unit from pfe
  flag_v[0];

  // kernel using SVM
  parallel_for_each(extent<1>(NUM), [=] (index<1> id) restrict(amp) {
    a[id[0]] = compute_restrict(b[id[0]], c[id[0]]);

    if (id[0] == NUM-1)
      flag_v[0] = 1;
  });
  flag_v.synchronize_async().wait();

  errors = verify(b,c,a,NUM);
  printf("Test 3 - Function call in pfe: %d errors\n", errors);



#ifdef TEST_AUTO_AUTO

  // TEST 4 - "auto auto"
  flag_v[0];

  // kernel using SVM
  parallel_for_each(extent<1>(NUM), [=] (index<1> id) {
    a[id[0]] = compute(b[id[0]], c[id[0]]);

    if (id[0] == NUM-1)
      flag_v[0] = 1;
  });
  flag_v.synchronize_async().wait();

  errors = verify(b,c,a,NUM);
  printf("Test 4 - auto auto: %d errors\n", errors);
#endif


  delete[] a;
  delete[] b;
  delete[] c;
  return 0;
}


