
#include <amp.h>
using namespace concurrency;

void calculate(float* output, float* inputA, float* inputB, const unsigned int num) {

  int flag = 0;
  array_view<int> flag_v(extent<1>(1),&flag);
  parallel_for_each(extent<1>(num), [=] (index<1> id) restrict(amp) {
    output[id[0]] = inputA[id[0]] + inputB[id[0]];

    if (id[0] == num-1)
      flag_v[0] = 1;
  });
  flag_v.synchronize_async().wait();

  return;
}
