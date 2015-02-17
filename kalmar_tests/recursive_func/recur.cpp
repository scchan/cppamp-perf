
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <typeinfo>
#include <amp.h>

#if defined(RESTRICT_AMP)
#define RESTRICT restrict(amp)
#elif defined(RESTRICT_CPU)
#define RESTRICT restrict(cpu)
#elif defined(RESTRICT_AUTO)
#define RESTRICT restrict(auto)
#else
#define RESTRICT 
#endif

using namespace concurrency;


unsigned int r(unsigned int i, int level) RESTRICT {
  if (i == 0)
    return level;
  else
    return r(i >> 1, level+1);
}

unsigned int test_recursive_function(unsigned int numThreads) {
  
  std::vector<unsigned int> output(numThreads);

  unsigned int* output_p = output.data();
  parallel_for_each(extent<1>(numThreads),[=](index<1>idx) RESTRICT {
    output_p[idx[0]] = r(idx[0],0);
  });

  unsigned int errors = 0;
  for (unsigned int i = 0; i < numThreads; i++) {
    unsigned int expected = r(i,0);
    if (output[i]!=expected) {
      errors++;
      std::cout << "i: " << i << " expected level: " << expected << " actual level: " << output[i] << std::endl; 
    }
  }
  return errors;
}

int main() {

  unsigned int num = 100;
  unsigned int errors = 0;

  errors = test_recursive_function(num);

  std::cout << "errors: " << errors << std::endl;
  return errors;
}
