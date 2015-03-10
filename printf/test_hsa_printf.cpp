
#include <cstdlib>
#include "hsa_printf.h"


#ifdef ENABLE_AMP
#include "amp.h"
using namespace concurrency;
#endif


const char* hello = "Hello World\n";
const char* test_float = "float: %.2f\n";
const char* test_int_float = "int: %d, float: %.4f\n";

int main(int argc, char* argv[]) {
  HSAPrintfPacketQueue* queue = createHSAPrintfPacketQueue(128);


#ifdef ENABLE_AMP
  parallel_for_each(extent<1>(1), [=](index<1> i)  {
    if (i[0] == 0) {
      hsa_printf(queue, hello);
      hsa_printf(queue, test_float, 1234.0f);
      hsa_printf(queue, test_int_float, 128, 1234.0f);
    }
  });
#endif

  hsa_printf(queue, "Hello\n");
  hsa_printf(queue, hello);
  hsa_printf(queue, test_float, 1234.0f);
  hsa_printf(queue, test_int_float, 128, 1234.567f);
  hsa_printf(queue, "%c, %u\n",'a',(unsigned short) 24);
  hsa_printf(queue, "%% %%%s%%\n", "foo");
  hsa_process_printf_queue(queue);
  queue = destroyHSAPrintfPacketQueue(queue);

  printf("%% %%%s%%\n","foo");
  return 0;
}
