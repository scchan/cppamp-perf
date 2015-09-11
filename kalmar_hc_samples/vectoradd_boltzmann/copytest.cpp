#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

#include <hc.hpp>
#include "hsa.h"
#include <kalmar_runtime.h>

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

  hc::accelerator acc;
  hc::accelerator_view av = acc.get_auto_selection_view();

  //Kalmar::KalmarContext* context = Kalmar::getContext();
  //Kalmar::KalmarDevice* device = context->getDevice();
  //std::wcout << L"Device: " << device->get_description() << std::endl;

  //hsa_region_t mem_region = device->getHSAAMRegion();

  hsa_region_t* mem_region = static_cast<hsa_region_t*>(av.get_hsa_am_region());
  hsa_agent_t* agent = static_cast<hsa_agent_t*>(av.get_hsa_agent());

  int *device_a, *device_b, *device_c;

  HSA_ASSERT(hsa_memory_allocate(*mem_region, NUM*sizeof(int), (void**)&device_a));
  HSA_ASSERT(hsa_memory_assign_agent(device_a, *agent, HSA_ACCESS_PERMISSION_RW));

  HSA_ASSERT(hsa_memory_allocate(*mem_region, NUM*sizeof(int), (void**)&device_b));
  HSA_ASSERT(hsa_memory_assign_agent(device_b, *agent, HSA_ACCESS_PERMISSION_RW));

  // copy the input data 
  HSA_ASSERT(hsa_memory_copy(device_a, va.data(), NUM*sizeof(int)));
  
  hc::parallel_for_each(hc::extent<1>(NUM),[&](hc::index<1> idx) __attribute((hc)) {
      int i = idx[0];
      //device_b[i] = device_a[i];
      device_b[i] = i;
  }).wait();

  // copy the output data back to the vector
  HSA_ASSERT(hsa_memory_copy(vb.data(), device_b, NUM*sizeof(int)));

  int errors = 0;
  auto ia = va.begin();
  //errors = std::count_if(vb.begin(), vb.end(), [&] (int i) { return (i!=(*ia++)); });

  n = 0;
  errors = std::count_if(vb.begin(), vb.end(), [&] (int i) { return (i!=(n++)); });


  std::cout << errors << " errors" << std::endl;  


  HSA_ASSERT(hsa_memory_free(device_a));
  HSA_ASSERT(hsa_memory_free(device_b));

  return errors;
}

