#include <atomic>
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


class S {
public:
  unsigned int data;
  static std::atomic<unsigned int> s_data;
  void increment(unsigned int v) RESTRICT {
    s_data.fetch_add(v);
  }
};
std::atomic<unsigned int> S::s_data;


bool test_static_member(unsigned int numThreads) {

  S::s_data.store(0,std::memory_order_release);

  unsigned int sum = 0; 
  std::vector<S> input(numThreads);
  for (int i = 0; i < numThreads; i++) {
    input[i].data = i;
    sum+=i;
  }

  S* input_p = input.data();
  parallel_for_each(extent<1>(numThreads),[=](index<1>idx) RESTRICT {
    input_p[idx[0]].increment(input_p[idx[0]].data);
  });

  bool error = false;
  if (fabs(S::s_data-sum) > 0.001) {
    error = true;
    std::cout << "Error, expected: " << sum << " actual: " << S::s_data << std::endl;
  }

  return error;
}


int main() {
  int num = 100;
  bool error;
  error = test_static_member(num);
  return error;
}


