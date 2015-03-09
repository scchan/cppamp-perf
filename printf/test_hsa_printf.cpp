
#include <cstdlib>

#include "amp.h"
#include "hsa_printf.h"

using namespace concurrency;


const char* hello = "Hello World\n";
const char* test_float = "float: %f\n";
const char* test_int_float = "int: %d, float: %f\n";

int main(int argc, char* argv[]) {
  HSAPrintfPacketQueue* queue = createHSAPrintfPacketQueue(128);

/*
  parallel_for_each(extent<1>(1), [=](index<1> i)  {
    if (i[0] == 0) {
      hsa_printf(queue, hello);
      hsa_printf(queue, test_float, 1234.0f);
      hsa_printf(queue, test_int_float, 128, 1234.0f);
    }
  });
*/

  hsa_printf(queue, "Hello\n");
  hsa_printf(queue, hello);
  hsa_printf(queue, test_float, 1234.0f);
  hsa_printf(queue, test_int_float, 128, 1234.567f);

  hsa_process_printf_queue(queue);
  queue = destroyHSAPrintfPacketQueue(queue);
  return 0;
}
