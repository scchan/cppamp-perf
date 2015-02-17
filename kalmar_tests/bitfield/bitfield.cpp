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


struct B {
  unsigned int data:7;
};

unsigned int test_bitfield(unsigned int numThreads) {
  
  std::vector<B> output(numThreads);
  B* output_p = output.data();
  parallel_for_each(extent<1>(numThreads),[=](index<1>idx) RESTRICT {
    output_p[idx[0]].data = idx[0];
  });

  unsigned int errors = 0;
  for (unsigned int i = 0; i < numThreads; i++) {
    B e;
    e.data = i;
    unsigned int expected = e.data;
    if (output[i].data!=expected) {
      errors++;
      std::cout << "i: " << i << " expected: " << expected << " actual: " << output[i].data << std::endl; 
    }
  }
  return errors;
}



int main(int argc, char* argv[]) {

  unsigned int num = 100;
  if (argc == 2) {
    num = atoi(argv[1]);
  }

  int errors = test_bitfield(num);
  std::cout << "errors: " << errors << std::endl;

  return errors;
}

