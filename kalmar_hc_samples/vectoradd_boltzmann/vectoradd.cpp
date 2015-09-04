#include <vector>
#include <algorithm>
#include <iostream>

#include <hc.hpp>
#include "hsa.h"
#include <kalmar_runtime.h>


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

  Kalmar::KalmarContext* context = Kalmar::getContext();
  Kalmar::KalmarDevice* device = context->getDevice();
  std::wcout << L"Device: " << device->get_description() << std::endl;



  int errors = 0;
  auto ia = va.begin();
  auto ib = vb.begin();
  errors = std::count_if(vc.begin(), vc.end(), [&] (int i) { return (i!=(*ia++ + *ib++)); });

  std::cout << errors << " errors" << std::endl;  
  return errors;
}

