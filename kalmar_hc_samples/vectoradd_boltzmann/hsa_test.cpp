
#include <cassert>
#include <cstdio>
#include "hsa.h"

#define HSA_ASSERT(X) (assert((X)==HSA_STATUS_SUCCESS))

struct region_iterator
{
    hsa_region_t _am_region;

    hsa_region_t _kernarg_region;
    hsa_region_t _finegrained_region;
    hsa_region_t _coarsegrained_region;

    bool        _found_kernarg_region;
    bool        _found_finegrained_region;
    bool        _found_coarsegrained_region;

    region_iterator() ;
};


region_iterator::region_iterator()
{
    _kernarg_region.handle=(uint64_t)-1;
    _finegrained_region.handle=(uint64_t)-1;
    _coarsegrained_region.handle=(uint64_t)-1;

    _found_kernarg_region = false;
    _found_finegrained_region = false;
    _found_coarsegrained_region = false;
}

    static hsa_status_t get_memory_regions(hsa_region_t region, void* data)
    {
    
        hsa_region_segment_t segment;
        hsa_region_get_info(region, HSA_REGION_INFO_SEGMENT, &segment);
    
        region_iterator *ri = (region_iterator*) (data);
    
        hsa_region_global_flag_t flags;
        hsa_region_get_info(region, HSA_REGION_INFO_GLOBAL_FLAGS, &flags);
    
        if (segment == HSA_REGION_SEGMENT_GLOBAL) {
            if (flags & HSA_REGION_GLOBAL_FLAG_KERNARG) {
                ri->_kernarg_region = region;
                ri->_found_kernarg_region = true;

                printf("found kernarg region\n");
            }
    
            if (flags & HSA_REGION_GLOBAL_FLAG_FINE_GRAINED) {
                ri->_finegrained_region = region;
                ri->_found_finegrained_region = true;

                printf("found fine grained region\n");
            }
    
            if (flags & HSA_REGION_GLOBAL_FLAG_COARSE_GRAINED) {
                ri->_coarsegrained_region = region;
                ri->_found_coarsegrained_region = true;

                printf("found coarse grained region\n");
            }
        }
    
        return HSA_STATUS_SUCCESS;
    }


static hsa_status_t get_gpu_agent(hsa_agent_t agent, void *data) {
    hsa_status_t status;
    hsa_device_type_t device_type;
    status = hsa_agent_get_info(agent, HSA_AGENT_INFO_DEVICE, &device_type);
    if (HSA_STATUS_SUCCESS == status && HSA_DEVICE_TYPE_GPU == device_type) {
        hsa_agent_t* ret = (hsa_agent_t*)data;
        *ret = agent;
        return HSA_STATUS_INFO_BREAK;
    }
    return HSA_STATUS_SUCCESS;
}

int main() {

  hsa_status_t status;

  HSA_ASSERT(hsa_init());

  hsa_agent_t agent;
  status = hsa_iterate_agents(get_gpu_agent, &agent);
  if(status == HSA_STATUS_INFO_BREAK) { status = HSA_STATUS_SUCCESS; }
  HSA_ASSERT(status);

  char name[64] = { 0 };
  HSA_ASSERT(hsa_agent_get_info(agent, HSA_AGENT_INFO_NAME, name));
  printf("The agent name is %s.\n", name);

  region_iterator ri;
  HSA_ASSERT(hsa_agent_iterate_regions(agent, &get_memory_regions, &ri));



  HSA_ASSERT(hsa_shut_down());
  return 0;
}
