
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <amp.h>

#define NUM 1024

#define GROUP_SIZE   64  
#define __GROUP__ __attribute__((address_space(3)))

extern "C" __attribute__((amp)) __GROUP__ void* getGroupBasePtr();

using namespace concurrency;

int main() {


  std::vector<int> a(NUM);
  int n(0);
  std::generate(a.begin(), a.end(),[&] { return n++; });
  array_view<int>  av_a(NUM,a);

  std::vector<int> b(NUM);
  std::generate(b.begin(), b.end(),[&] { return n++; });
  array_view<int>  av_b(NUM,b);

  std::vector<int> c(NUM);
  std::generate(c.begin(), c.end(),[&] { return n++; });
  array_view<int>  av_c(NUM,c);

  parallel_for_each(extent<1>(NUM).tile<GROUP_SIZE>()
                    , [=](tiled_index<GROUP_SIZE> idx) restrict(amp) {


    tile_static int ts_s[GROUP_SIZE];
    ts_s[idx.local[0]] = av_a[idx.global];
    idx.barrier.wait();


    // store a value to the dynamic tile static buffer
    __GROUP__ int* dyn_ts_s =  (__GROUP__ int* )(
                                                  (__GROUP__ char*)getGroupBasePtr()
                                                                  + GROUP_SIZE * sizeof(int)    // size of statically allocated array
                                                );        
    dyn_ts_s[idx.local[0]] = av_b[idx.global];
    idx.barrier.wait();


    int local_id = idx.local[0];
    int v0 = ts_s[(local_id + 1)%GROUP_SIZE];
    int v1 = dyn_ts_s[(local_id + 2)%GROUP_SIZE];
    av_c[idx.global] = v0 + v1;
  });



}

#define GROUP_WIDTH 8
#define GROUP_HEIGHT 8
void convolve(int* img, const unsigned int width, const unsigned height, int* convolveKernel, const unsigned int kernelDim
                       int* imgOut) {

  extent<2> imgExtent(width, height);i
  array_view<int> avImg(imgExtent, img);

  extent<1> convolveKernelExtent(kernelDim * kernelDim);  // 2D kernel but it's simpler to use a 1D array_view 
  array_view<int> avKernel(convolveKernelExtent, convolveKernel);

  array_view<int> avImgOut(imgExtent, imgOut);
  avImgOut.discard_data();

  const unsigned int pixelCacheWidth = GROUP_WIDTH + kernelDim;
  const unsigned int pixelCacheHeight = GROUP_HEIGHT +  kernelDim;
  ts_allocator tsa;
  tsa.setDynamicGroupSegmentSize( pixelCacheWidth * pixelCacheHeight * sizeof(unsigned int)  // cache of the image pixels

                                  + (kernelDim * kernelDim) * sizeof(unsigned int)  // cache for the convolution kernel
                                );
  
  parallel_for_each(extent<2>(width, height).tile<GROUP_WIDTH,GROUP_HEIGHT>(), tsa
                    [=](tiled_index<GROUP_WIDTH,GROUP_HEIGHT> idx) restrict(amp) {

    // allocate group memory for the pixel cache
    __GROUP__ unsigned int* pixelCache = (__GROUP__ unsigned int*) 
                                          tsa.group_alloc(pixelCacheWidth * pixelCacheHeight * sizeof(unsigned int));

    // allocate group memory for the convolution kernel
    __GROUP__ unsigned int* kernelCache = (__GROUP__ unsigned int*) 
                                          tsa.group_alloc(kernelDim * kernelDim * sizeof(unsigned int));

    int pixelCacheOriginX = idx.tile_origin[0] - kernelDim/2;
    int pixelCacheOriginY = idx.tile_origin[1] - kernelDim/2;
    unsigned flattenedLocalID = idx.local[1] * GROUP_WIDTH + idx.local[0];

    // cache the image pixels in group memory
    for (unsigned int pixelCacheIndex = flattenedLocalID; pixelCacheIndex < pixelCacheWidth * pixelCacheHeight; 
          pixelCacheIndex+=(GROUP_WIDTH * GROUP_HEIGHT)) {

      int pixelY = clamp(pixelCacheOriginY + (pixelCacheIndex / GROUP_WIDTH), 0, height - 1);
      int pixelX = clamp(pixelCacheOriginX + (pixelCacheIndex % GROUP_WIDTH), 0, width - 1);
      index<2> pixelIndex(pixelX, pixelY);

      pixelCache[pixelCacheIndex] = avImg[pixelIndex];

    }

    // cache the convolution kernel
    for (unsigned int kernelCacheIndex = flattenedLocalID; kernelCacheIndex < kernelDim * kernelDim; 
          kernelCacheIndex+=(GROUP_WIDTH * GROUP_HEIGHT)) {
      kernelCache[kernelCacheIndex] = avKernel[kernelCacheIndex];
    }

    // synchronize all the workitems
    idx.barrier.wait();

    // computing the convolution here
    newPixel = 0;
    unsigned int kernelCacheIndex = 0;
    for (unsigned int y = idx.local[1];  y < idx.local[1] + kernelDim;  y++) {
      for (unsigned int x = idx.local[0]; x < idx.local[0] + kernelDim; x++) {
        newPixel += pixelCache[y * pixelCacheWidth + x] * kernelCache[kernelCacheIndex++];
      }
    }
    avImgOut[idx.global] = newPixel;
  });
}






int main() {


  std::vector<int> a(NUM);
  int n(0);
  std::generate(a.begin(), a.end(),[&] { return n++; });
  array_view<int>  av_a(NUM,a);

  std::vector<int> b(NUM);
  std::generate(b.begin(), b.end(),[&] { return n++; });
  array_view<int>  av_b(NUM,b);

  std::vector<int> c(NUM);
  std::generate(c.begin(), c.end(),[&] { return n++; });
  array_view<int>  av_c(NUM,c);

  parallel_for_each(extent<1>(NUM).tile<GROUP_SIZE>()
                    , [=](tiled_index<GROUP_SIZE> idx) restrict(amp) {


    tile_static int ts_s[GROUP_SIZE];
    ts_s[idx.local[0]] = av_a[idx.global];
    idx.barrier.wait();


    // store a value to the dynamic tile static buffer
    __GROUP__ int* dyn_ts_s =  (__GROUP__ int* )(
                                                  (__GROUP__ char*)getGroupBasePtr()
                                                                  + GROUP_SIZE * sizeof(int)    // size of statically allocated array
                                                );        
    dyn_ts_s[idx.local[0]] = av_b[idx.global];
    idx.barrier.wait();


    int local_id = idx.local[0];
    int v0 = ts_s[(local_id + 1)%GROUP_SIZE];
    int v1 = dyn_ts_s[(local_id + 2)%GROUP_SIZE];
    av_c[idx.global] = v0 + v1;
  });



}
