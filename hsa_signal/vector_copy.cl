
#include "signals.cl.h"
#include "print_signal.h"

//unsigned long signal_add_scar_u64_sig64(unsigned long signalHandle, unsigned long src0);

/*
__kernel void test_signal(unsigned long testSignal) {
  if (get_global_id(0)==0) {
//    signal_add_scar_u64_sig64(testSignal, (unsigned long)0x1 );
  }
}
*/

__kernel void vector_copy(__global int *a
                          , __global int *b
                          , unsigned long test_signal) {
  int id = get_global_id(0);
  b[id] = a[id];

  if (id == 0) {
    signal_add_scar_u64_sig64(test_signal, 0x1);
  }
}




