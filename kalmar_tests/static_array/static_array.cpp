
#include <cstdio>
#include <amp.h>

#define NUM 1024

using namespace concurrency;

int main() {

  int a[NUM];
  int b[NUM];
  int c[NUM];
  for (unsigned int i = 0; i < NUM; i++) {
    a[i] = i;
    b[i] = i * 100;
    c[i] = 0;
  }

  parallel_for_each(extent<1>(NUM), [=,&c](index<1> i) restrict(amp) {
    c[i[0]] = a[i[0]] + b[i[0]];
  });

  for (unsigned int i = 0; i < NUM; i++) {
    int expected = a[i] + b[i];
    if (c[i] != expected) {
      printf("Error c[%d]: expected=%d \t actual=%d\n", i, expected, c[i]);
      exit(1);
    }
  }
  return 0;
}
