
#include <cstdio>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cmath>

#include <amp.h>


#define X  128
#define Y  128

#ifndef USE_AMP_ATTRIBUTE
#define RESTRICT_AMP restrict(amp)
#else
#define RESTRICT_AMP __attribute__((amp))
#endif

using namespace concurrency;


void convolve_cpu(float* input, float* mask, float* output) {
  for (int y = 0; y < Y; y++) {
    for (int x = 0; x < X; x++) {
      if (x == 0 || x == (X-1) ||
          y == 0 || y == (Y-1)) {
        output[y * X + x] = input[y * X + x];
      }
      else {
        float convolve = 0.0f;
        int input_tile_y = y - 1;
        int input_tile_x = x - 1;
        int mask_index = 0;
        for (int n = 0; n < 3; n++) {
          for (int m = 0; m < 3; m++) {
            convolve += input[(input_tile_y + n) * X + (input_tile_x + m)] * mask[mask_index];
            mask_index++;
          }
        }
        output[y * X + x] = convolve;
      }
    }
  }
}

void convolve_gpu(float* input, float* mask, float* output) {

  const array_view<float,2> mask_view(3, 3, mask);
  array_view<float, 2> input_view(X, Y, input);
  array_view<float, 2> output_view(X, Y, output);

  extent<2> grid(X, Y);
  parallel_for_each(grid, [=](index<2> idx) RESTRICT_AMP {

    if (idx[0] == 0 || idx[0] == (X-1) ||
        idx[1] == 0 || idx[1] == (Y-1)) {
      output_view[idx] = input_view[idx];
    }
    else {
      float convolve = 0.0f;
      index<2> input_tile_origin = idx + index<2>(-1,-1);
      for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
          index<2> tile_pos(i,j);
          convolve += input_view[input_tile_origin + tile_pos] * mask_view[tile_pos];
        }
      }
      output_view[idx] = convolve;
    }
  });
  output_view.synchronize();
}

int main(void) { 

  // 3x3 convolution kernel
  float mask[] = { 1.0f/16.0f, 1.0f/8.0f, 1.0f/16.0f,
                    1.0f/8.0f,  1.0f/4.0f, 1.0f/8.0f,
                    1.0f/16.0f, 1.0f/8.0f, 1.0f/16.0f };
  float* input = new float[X * Y];
  std::generate(input, input + (X * Y), []() { return rand() % 256; });

  float* output = new float[X * Y];
  float* cpu_output = new float[X * Y];

  convolve_gpu(input, mask, output);

  convolve_cpu(input, mask, cpu_output);

  int errors = 0;
  for (int y = 0; y < Y; y++) {
    for (int x = 0; x < X; x++) {
      if (std::fabs(output[y * X + x] - cpu_output[y * X + x]) > 0.0001f) {
        errors++;
      }
    }
  }
  printf("%d errors\n",errors);

  delete[] input;
  delete[] output;
  delete[] cpu_output;
  return errors;
}
