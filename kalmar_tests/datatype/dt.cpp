
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


#define TEST_TYPE(T,num)  runTypeTest<T>(#T,num)

template <typename T>
int runTypeTest(std::string typenameString, unsigned int num) {

  std::vector<T> inputA(num);
  std::vector<T> inputB(num);
  std::vector<T> output(num);

  for (unsigned int i = 0; i < num; i++) {
    inputA[i] = i;
    inputB[i] = i+100;
    output[i] = 0;
  }

  T* inputA_p = inputA.data();
  T* inputB_p = inputB.data();
  T* output_p = output.data();
  parallel_for_each(extent<1>(num), [=](index<1>idx) RESTRICT {
    T result = inputA_p[idx[0]] + inputB_p[idx[0]];
    output_p[idx[0]] = result;
  });

  int errors = 0;
  for (int i = 0; i < num; i++) {
    T expected = inputA[i]+inputB[i];
    if (fabs((double)(output[i] - expected)) > 0.001) {
      errors++;
    }
  }
  std::cout << "runTypeTest<" << typenameString << ">: " << errors << " errors" << std::endl;
  return errors;
}


#ifdef TEST_BOOL
bool isOdd(unsigned int i) RESTRICT {
  return (i%2!=0);
}

int runBoolTest(unsigned int num) {

  std::vector<unsigned int> output(num);
  unsigned int* output_p = output.data();

  parallel_for_each(extent<1>(num), [=](index<1>idx) RESTRICT {
    bool odd = isOdd(idx[0]);
    output_p[idx[0]] = odd?idx[0]:idx[0]+100;
  });

  int errors = 0;
  for (int i = 0; i < num; i++) {
    bool odd = isOdd(i);
    unsigned int expected = odd?i:i+100;
    if (output[i]!=expected) {
      errors++;
    }
  }
  std::cout << "runTypeTest<" << "bool" << ">: " << errors << " errors" << std::endl;
  return errors;
}
#endif

int main(int argc, char*argv[]) {

  unsigned int num = 100;
  if (argc == 2) {
    num = atoi(argv[1]);
  }

  int errors = 0;

#ifdef TEST_BOOL
  errors += runBoolTest(num);
#endif

  errors += TEST_TYPE(unsigned char,num);
  errors += TEST_TYPE(char,num);

  errors += TEST_TYPE(unsigned short,num);
  errors += TEST_TYPE(short,num);

  errors += TEST_TYPE(unsigned int,num);
  errors += TEST_TYPE(int,num);

  errors += TEST_TYPE(unsigned long,num);
  errors += TEST_TYPE(long,num);

  errors += TEST_TYPE(unsigned long long,num);
  errors += TEST_TYPE(long long,num);

  errors += TEST_TYPE(float,num);
  errors += TEST_TYPE(double,num);

  return errors;
}


