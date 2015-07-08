#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include "hc.h"

#define WIDTH     1024
#define HEIGHT    1024

#define NUM       (WIDTH*HEIGHT)

#define THREADS_PER_BLOCK_X  16
#define THREADS_PER_BLOCK_Y  16

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
  int errors;

  hostB = (float*)malloc(NUM * sizeof(float));
  hostC = (float*)malloc(NUM * sizeof(float));
  
  // initialize the input data
  for (i = 0; i < NUM; i++) {
    hostB = (float)i;
    hostC = (float)i*100.0f;
  }
  
  HC_ASSERT(hcMalloc((void**)&deviceA, NUM * sizeof(float)));
  HC_ASSERT(hcMalloc((void**)&deviceB, NUM * sizeof(float)));
  HC_ASSERT(hcMalloc((void**)&deviceC, NUM * sizeof(float)));
  
  HC_ASSERT(hcMemcpy(deviceB, hostB, NUM*sizeof(float), hcMemcpyHostToAccelerator);
  HC_ASSERT(hcMemcpy(deviceC, hostC, NUM*sizeof(float), hcMemcpyHostToAccelerator);

  hcLaunchKernel(vectoradd_float
                ,hcCreateLaunchParam2(
                     DIM3(WIDTH/THREADS_PER_BLOCK_X, HEIGHT/THREADS_PER_BLOCK_Y)
                   , DIM3(THREADS_PER_BLOCK_X, THREADS_PER_BLOCK_Y)
                 )
                ,deviceA ,deviceB ,deviceC ,WIDTH ,HEIGHT);

  HC_ASSERT(hcMemcpy(hostA, deviceA, NUM*sizeof(float), hcMemcpyAcceleratorToHost));

  // verify the results
  errors = 0;
  for (i = 0; i < NUM; i++) {
    if (hostA[i] != (hostB[i] + hostC[i])) {
      errors++;
    }
  }
  if (errors!=0) {
    printf("%d errors\n",errors);
  }

  HC_ASSERT(hcFree(deviceA));
  HC_ASSERT(hcFree(deviceB));
  HC_ASSERT(hcFree(deviceC));

  free(hostA);
  free(hostB);
  free(hostC);

  hcResetDefaultAccelerator();

  return errors;
}
