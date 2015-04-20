#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <amp.h>

#define SIZE 128*1048576
#define ITER 5

#define BLOCKSIZE 64
#define WGSIZE 64
#define GLOBAL_THREADS 81920 //32*64*40 Tahiti
#define USE_ARRAY 1

using namespace concurrency;
#define Real_t float

Real_t diff_time(struct timespec et, struct timespec st)
{
	Real_t diff_t = ((et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec)*1e-9);
	return diff_t;
}

#if USE_ARRAY
void for_loop(std::vector<Real_t>& a, std::vector<Real_t>& b, array<Real_t>& av_a, array<Real_t>& av_b){
#else
void for_loop(std::vector<Real_t>& a, std::vector<Real_t>& b, array_view<Real_t>& av_a, array_view<Real_t>& av_b){
#endif

	Real_t time = 0;  
	struct timespec st, et;

	// If using arrays, copy data to device
#if USE_ARRAY
	clock_gettime(CLOCK_MONOTONIC_RAW, &st);
	copy(a.begin(), av_a);
	copy(b.begin(), av_b);  
	clock_gettime(CLOCK_MONOTONIC_RAW, &et);
	printf("Copy In Time: %f seconds\n", diff_time(et,st));
#endif

	int global_threads = 32 * 64 * 40;
	extent<1> ext(global_threads);

	parallel_for_each(ext.tile<WGSIZE>(),
#if USE_ARRAY
	    [=, &av_a, &av_b] (tiled_index<WGSIZE> t_idx) restrict (amp) {			
#else
		[=] (tiled_index<WGSIZE> t_idx) restrict (amp) {			
#endif
		int tid = t_idx.global[0];
		int gsize = WGSIZE;
		int st_idx = t_idx.tile[0] * BLOCKSIZE * gsize;

		Real_t sum = 0.;
		while(tid < SIZE){
			sum = 0.0;
			for (int i = 0; i < BLOCKSIZE; i++)
				sum += av_a[st_idx + t_idx.local[0] + gsize * i];

				av_b[tid] = sum;
				tid += global_threads*BLOCKSIZE;
			}
		}
	);

	// If using arrays, copy data back to host else synchronize on the array_view
#if USE_ARRAY
	clock_gettime(CLOCK_MONOTONIC_RAW, &st);
	copy(av_b, b.begin());
	clock_gettime(CLOCK_MONOTONIC_RAW, &et);
	printf("Copy Out Time: %f seconds\n", diff_time(et,st));	
#else
	av_b.synchronize();
#endif
}

int main(int argc, char *argv[])
{
	std::vector<Real_t> a(SIZE);
	std::vector<Real_t> b(SIZE);
	std::vector<Real_t> cpu(SIZE);

	Real_t time = 0;
	accelerator refAccl;
	static const int TS = 64;
	struct timespec st, et;

	for(int64_t i = 0; i < SIZE; i++){
		a[i] = (Real_t)(rand()%100);
		b[i] = 0.f;
		cpu[i] = 0.f;
	}

	// MD: Horrible hack which works until SIZE == number of GPU threads launched times BLOCKSIZE.
	for(int k = 0; k < SIZE; k += 64 * 64){
		for(int i = 0; i < 64; i ++) {
			Real_t sum = 0.0;
				for(int j = 0; j < 64; j++) {
				sum += a[k + i + 64*j];	
			}
			cpu[k/64 + i] = sum;
		}
	}

	// Create or concurrency::array or concurrency::array_view
#if USE_ARRAY
	array<Real_t, 1> av_a(SIZE, refAccl.get_default_view());	
	array<Real_t, 1> av_b(SIZE, refAccl.get_default_view());	
#else
	array_view<Real_t, 1> av_a(SIZE, a);	
	array_view<Real_t, 1> av_b(SIZE, b);	
#endif

	// Launch parallel_for_each 
	for(int k = 0; k < ITER; k++) {
		clock_gettime(CLOCK_MONOTONIC_RAW, &st);
		for_loop(a, b, av_a, av_b);
		clock_gettime(CLOCK_MONOTONIC_RAW, &et);
		time += diff_time(et,st);
	}

	// Verification
	for(int i = 0; i < SIZE/64; ++i){
		if(cpu[i] != b[i]){
			printf("Bad data found at %i. Value %f supposed to be %f\n",
				i, b[i], cpu[i]);
			break;
		}
	}

	printf("Useful Time: %f msecs\n", (Real_t)(time / ITER) * 1e3);
	Real_t bw = ((Real_t)((SIZE*sizeof(Real_t))*1e-9) / ((Real_t)time/ITER));	
	printf("Bandwidth: %f Gb/s\n", bw);

	return 0;
}
