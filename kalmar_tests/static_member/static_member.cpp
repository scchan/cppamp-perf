#include <atomic>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <typeinfo>
#include <string>
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

#define TEST_STATIC_MEMBER(T,num)  test_static_member<T>(#T,num)


using namespace concurrency;


template <typename T>
class S {
public:
  unsigned int data;
  static std::atomic<T> s_data;
  void increment(T v) RESTRICT {
    s_data.fetch_add(v);
  }
};

template <typename T>
std::atomic<T> S<T>::s_data;


template <typename T>
unsigned int test_static_member(std::string typenameString, unsigned int numThreads) {

  S<T>::s_data.store(0,std::memory_order_release);

  unsigned int sum = 0; 
  std::vector<S<T>> input(numThreads);
  for (int i = 0; i < numThreads; i++) {
    input[i].data = i;
    sum+=i;
  }

  S<T>* input_p = input.data();
  parallel_for_each(extent<1>(numThreads),[=](index<1>idx) RESTRICT {
    input_p[idx[0]].increment(input_p[idx[0]].data);
  });

  unsigned int error = 0;
  if (fabs(S<T>::s_data-sum) > 0.001) {
    error = 1;
    std::cout << "Error test_static_member<" <<  typenameString << "> expected: " << sum << " actual: " << S<T>::s_data << std::endl;
  }
  return error;
}


int main() {
  int num = 100;
  unsigned int error = 0;

/*
  error += TEST_STATIC_MEMBER(unsigned char,num);
  error += TEST_STATIC_MEMBER(char,num);

  error += TEST_STATIC_MEMBER(unsigned short,num);
  error += TEST_STATIC_MEMBER(short,num);
*/


  error += TEST_STATIC_MEMBER(unsigned int,num);
  error += TEST_STATIC_MEMBER(int,num);

/*
  error += TEST_STATIC_MEMBER(unsigned long,num);
  error += TEST_STATIC_MEMBER(long,num);

  error += TEST_STATIC_MEMBER(unsigned long long,num);
  error += TEST_STATIC_MEMBER(long long,num);
*/

  return error;
}


