
#include <cstdio>
#include <amp.h>

// Number of array elements
#ifndef NUM
#define NUM (32 * 64 * 2)
#endif

#ifndef USE_AMP_ATTRIBUTE
#define RESTRICT_AMP restrict(amp)
#else
#define RESTRICT_AMP __attribute__((amp))
#endif

using namespace concurrency;

int main(void) { 

  int a[NUM];
  for (unsigned int i = 0; i < NUM; i++) {
    a[i] = i;
  }

  // create array views from host buffers
  array_view<int> aa(NUM,a);

  int b[32];
  array_view<int> bb(NUM,b);

  // specify the compute grid size and dimension
  extent<1> grid(32*64);

  // AMP style parallel for construct
  parallel_for_each(grid.tile<64>(), 
                    [=](tiled_index<64> t_idx) RESTRICT_AMP {

    tile_static int tmp[64];
    int r = aa[t_idx.global] + aa[t_idx.global[0] + grid[0]];
    tmp[t_idx.local[0]] = r;
    t_idx.barrier.wait();

    extent<1> tile_extent = t_idx.get_tile_extent();
    for (int i = tile_extent[0]/2; i > 0; i/=2) {
      if (t_idx.local[0] < i) {
        r = tmp[t_idx.local[0]] + tmp[t_idx.local[0] + i];
        tmp[t_idx.local[0]] = r;
      }
      t_idx.barrier.wait();
    }
    if (t_idx.local[0] == 0) {
      bb[t_idx.tile] = tmp[0];
    }

  });

  // synchronize the host buffer with updated data from the view
  bb.synchronize();

  int reduce = 0;
  for (int i = 0; i < 32; i++) {
    reduce += b[i];
  }

  printf("gpu reduction: %d\n",reduce);


  reduce = 0;
  for (int i = 0; i < NUM; i++) {
    reduce+=a[i];
  }
  printf("cpu reduction: %d\n",reduce);


  return 0;
}
