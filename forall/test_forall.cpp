
#include <iostream>
#include <vector>
#include "forall.hpp"

#define NUM 100

int main() {

  std::vector<int> a(NUM);
  std::vector<int> b(NUM);
  std::vector<int> c(NUM);
  for (int i = 0; i < NUM; i++) {
    a[i] = i;
    b[i] = 2*i;
    c[i] = 0;
  }

  int* da = (int*)a.data();
  int* db = (int*)b.data();
  int* dc = (int*)c.data();
  forall(0, NUM, [=](int i) {
    dc[i] = da[i] + db[i];
  });

  // verify
  int errors = 0;
  for (int i = 0; i < 100; i++) {
    if (c[i] != a[i]+b[i]) {
      std::cout << "error c[" << i << "]  expected: " << (a[i]+b[i]) << " actual: " << c[i] << std::endl;
      errors++;
    }
  }
  std::cout << errors << " errors" << std::endl;
  return 0;
}
