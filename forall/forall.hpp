#include <amp.h>

template <typename LOOP_BODY>
void forall (int begin, int end, LOOP_BODY body) {
  concurrency::extent<1> range(end-begin);
  concurrency::parallel_for_each(range, [=](concurrency::index<1>id) restrict(amp) {
    body(id[0]+begin);
  });
}
