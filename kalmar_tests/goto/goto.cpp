
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


unsigned int result(unsigned int i) RESTRICT {

  unsigned int value = 0;

  if (i%7==0) {
    goto label_3;
  }
  else if (i%5==0) {
    goto label_2;
  }
  else if (i%3==0) {
    goto label_1;
  }
  else if (i%2==0) {
    goto label_0;
  }

  value+=100;
label_0:
  value+=200;
label_1:
  value+=300;
label_2:
  value+=400;
label_3:
  value+=500;

  return value;
}


unsigned int test_goto(unsigned int numThreads) {
  
  std::vector<unsigned int> output(numThreads);

  unsigned int* output_p = output.data();
  parallel_for_each(extent<1>(numThreads),[=](index<1>idx) RESTRICT {
    output_p[idx[0]] = result(idx[0]);
  });

  unsigned int errors = 0;
  for (unsigned int i = 0; i < numThreads; i++) {
    unsigned int expected = result(i);
    if (output[i]!=expected) {
      errors++;
      std::cout << "i: " << i << " expected: " << expected << " actual: " << output[i] << std::endl; 
    }
  }
  return errors;
}

int main(int argc, char* argv[]) {

  unsigned int num = 100;
  if (argc == 2) {
    num = atoi(argv[1]);
  }

  int errors = test_goto(num);
  std::cout << "errors: " << errors << std::endl;

  return errors;
}
