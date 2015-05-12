#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include "hc.h"

#define WIDTH     1024
#define HEIGHT    1024

#define NUM       (WIDTH*HEIGHT)


__KERNEL void vectoradd_float(float* a, const float* b, const float* c, int width, int height) {
  
  int x = blockDim.x * blockIdx.x + threadIdx.x;
  int y = blockDim.y * blockIdy.y + threadIdx.y;

  int i = y * width + x;
  if ( i < (width * height)) {
    a[i] = b[i] + c[i];
  }
}


int main() {
  
  float* hostA;
  float* hostB;
  float* hostC;

  float* deviceA;
  float* deviceB;
  float* deviceC;

  hcError deviceError;

  int i;

  hostB = (float*)malloc(NUM * sizeof(float));
  hostC = (float*)malloc(NUM * sizeof(float));
  
  // initialize the input data
  for(i = 0; i < NUM; i++) {
    hostB = (float)i;
    hostC = (float)i*100.0f;
  }
  
  HC_ASSERT(hcMalloc((void**)&deviceA, NUM * sizeof(float)));
  HC_ASSERT(hcMalloc((void**)&deviceB, NUM * sizeof(float)));
  HC_ASSERT(hcMalloc((void**)&deviceC, NUM * sizeof(float)));
  
  HC_ASSERT(hcMemcpy(deviceB, hostB, NUM*sizeof(float), hcMemcpyHostToAccelerator);
  HC_ASSERT(hcMemcpy(deviceC, hostC, NUM*sizeof(float), hcMemcpyHostToAccelerator);

  hcLaunchKernel(vectoradd_float
                ,hcLaunchParam(
                   DIM3(), DIM3()
                 )
                ,deviceA ,deviceB ,deviceC ,WIDTH ,HEIGHT);

  HC_ASSERT(hcMemcpy(hostA, deviceA, NUM*sizeof(float), hcMemcpyAcceleratorToHost));

  return 0;
}
