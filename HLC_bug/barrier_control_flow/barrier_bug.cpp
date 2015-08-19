
#include <cstdio>
#include <amp.h>

#define NUM_ITEMS 2
#define ITEMS_PER_GROUP 2


using namespace concurrency;

int main() {


  int data[NUM_ITEMS];
  array_view<int> av_data(extent<1>(NUM_ITEMS), data);


  parallel_for_each(tiled_extent<ITEMS_PER_GROUP>(av_data.get_extent())
                    ,[=](tiled_index<ITEMS_PER_GROUP> idx) restrict(amp) {

                    
                    tile_static int t[ITEMS_PER_GROUP];

                    int tid = idx.local[0];
                    int val;

                    t[tid] = tid;
                    idx.barrier.wait();

                    if (tid == 0) {
                      t[0] = 123;
                      t[1] = 321;
                    }
                    idx.barrier.wait();

                    if (tid == 1) {
                      val = t[tid];
                    }
                    else {
                      val = 456;
                    }
                    idx.barrier.wait();

                    av_data[tid] = val;

                    });
  

  for (int i = 0; i < NUM_ITEMS; i++) {
    printf("%d: %d\n", i, av_data[i]);
  }


  return 0;
}
