
#include <cstring>
#include <cstdio>
#include <amp.h>
using namespace concurrency;

int foo(int* a, int num) {
  array_view<int,1>* aa = new array_view<int,1>(100,a);
  parallel_for_each (aa->get_extent(), [&](index<1>idx) restrict (amp) {
    (*aa)[idx]++;
  }
  );
  aa->synchronize();
  delete aa;
  return 0;
}

int foobar(int* a, int num) {
  array_view<int,1>* aa = new array_view<int,1>(100,a);
  parallel_for_each (aa->get_extent(), [=](index<1>idx) restrict (amp) {
    (*aa)[idx]++;
  }
  );
  aa->synchronize();
  delete aa;
  return 0;
}

int bar(int* a, int num) {
  array_view<int,1> aa(100,a);
  parallel_for_each (aa.get_extent(), [=](index<1>idx) restrict (amp) {
    aa[idx]++;
  }
  );
  aa.synchronize();
  return 0;
}


int barfoo(int* a, int num) {
  array_view<int,1> aa(100,a);
  parallel_for_each (aa.get_extent(), [&](index<1>idx) restrict (amp) {
    aa[idx]++;
  }
  );
  aa.synchronize();
  return 0;
}


#define NUM 100

int main() {
  int* a = new int[NUM];
  int error;

  error = 0;
  memset(a, 0, NUM * sizeof(int));
  bar(a,NUM);
  for (int i = 0; i < NUM; i++) {
    if (a[i] != 1)
      error++;
  }
  printf("errors: %d\n",error);

  error = 0;
  memset(a, 0, NUM * sizeof(int));
  barfoo(a,NUM);
  for (int i = 0; i < NUM; i++) {
    if (a[i] != 1)
      error++;
  }
  printf("errors: %d\n",error);

  error = 0;
  memset(a, 0, NUM * sizeof(int));
  foobar(a,NUM);
  for (int i = 0; i < NUM; i++) {
    if (a[i] != 1)
      error++;
  }
  printf("errors: %d\n",error);


  error = 0;
  memset(a, 0, NUM * sizeof(int));
  foo(a,NUM);
  for (int i = 0; i < NUM; i++) {
    if (a[i] != 1)
      error++;
  }
  printf("errors: %d\n",error);


  delete[] a;
  return 0;
}
