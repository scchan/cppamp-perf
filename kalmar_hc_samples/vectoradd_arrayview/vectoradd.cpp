#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

#include <hc.hpp>

#define HSA_ASSERT(X) (assert((X)==HSA_STATUS_SUCCESS))

#define NUM 1000

int main() {
  std::vector<int> va(NUM);
  std::vector<int> vb(NUM);
  std::vector<int> vc(NUM);
 
  int n = 0;
  std::generate(va.begin(), va.end(), [&]() { return n++; });

  n = 100;
  std::generate(vb.begin(), vb.end(), [&]() { return n++; });

#if 0
    auto ia = va.begin();
    auto ib = vb.begin();
    std::generate(vc.begin(), vc.end(), [&]() { return *ia++ + *ib++; });
#endif

  hc::array_view<int,1> av_a(hc::extent<1>(NUM), va);
  hc::array_view<int,1> av_b(hc::extent<1>(NUM), vb);
  hc::array_view<int,1> av_c(hc::extent<1>(NUM), vc);

  hc::parallel_for_each(hc::extent<1>(NUM),[=](hc::index<1> idx) __attribute((hc)) {
      int i = idx[0];
      av_c[i] = av_a[i] + av_b[i];
  }).wait();

  int errors = 0;
  auto ia = va.begin();
  auto ib = vb.begin();

  av_c.synchronize();
  errors = std::count_if(vc.begin(), vc.end(), [&] (int i) { return (i!=(*ia++ + *ib++)); });

  std::cout << errors << " errors" << std::endl;  
  return errors;
}

