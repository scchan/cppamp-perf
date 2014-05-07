

#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>



#ifdef ENABLE_CPPAMP

#include <amp.h>
#include <amp_math.h>
using namespace concurrency;

  #define AMP_RESTRICT restrict(cpu,amp)
  #define EXPF(x)   fast_math::expf(x)
  #define FABSF(x)  fast_math::fabsf(x)
  #define LOGF(x)   fast_math::logf(x)
  #define SQRTF(x)  fast_math::sqrtf(x)


#else

#include <cmath>

  #define AMP_RESTRICT
  #define EXPF(x)   exp(x)
  #define FABSF(x)  fabs(x)
  #define LOGF(x)   log(x)
  #define SQRTF(x)  sqrt(x)
#endif


#include "StackTimer.hpp"


#ifdef ENABLE_CPPAMP

void arrayViewDeviceToHostBandwidth(std::vector<float>& v, const float r) {
  array_view<float> av(extent<1>(v.size()),&v[0]);
  av.discard_data();
  parallel_for_each(extent<1>(v.size()),[=] (index<1> id) restrict (amp) {
    av[id[0]]=r+(float)id[0];
  });
  completion_future af = av.synchronize_async();
  af.wait();
}

void arrayDeviceToHostBandwidth(std::vector<float>& v, const float r) {
  array<float,1> a(v.size());
  parallel_for_each(extent<1>(v.size()),[=,&a] (index<1> id) restrict (amp) {
    a[id[0]]=r+(float)id[0];
  });
  completion_future cf = copy_async(a, v.begin());
  cf.wait();
}

int arrayViewHostToDeviceBandwidth(std::vector<float>& v) {
  array_view<float,1> av(extent<1>(v.size()),&v[0]);
  array<int,1> b(extent<1>(1));
  parallel_for_each(extent<1>(v.size()),[=,&av,&b] (index<1> id) restrict (amp) {
    if (FABSF(av[id[0]] - (float)id[0]) > 0.0001)
      b[0] = id[0];
  });

  // tell the runtime that I don't need the data in the array_view
  av.discard_data();

  int i;
  copy(b,&i);
  return i;
}

int arrayHostToDeviceBandwidth(std::vector<float>& v) {
  array<float,1> a(extent<1>(v.size()),&v[0]);
  array<int,1> b(extent<1>(1));
  parallel_for_each(extent<1>(v.size()),[=,&a,&b] (index<1> id) restrict (amp) {
    if (FABSF(a[id[0]] - (float)id[0]) > 0.0001)
      b[0] = id[0];
  });
  int i;
  copy(b,&i);
  return i;
}

#endif



struct unique_number {
  int current;
  unique_number() { current=0; };
  float operator()() {  return current++; };
};

struct output_ref {
  int current;
  float rand;
  output_ref(float r=0.0f) { current=0; rand = r; };
  bool operator()(float f) { bool b = fabs((current+rand)-f) > 0.001f; current++; return b;} 
};



#define DEFAULT_MEM_SIZE    16
#define DEFAULT_ITERATIONS  1

class Arg {
public:
  Arg(int argc, char** argv):argc(argc),argv(argv)
    ,memSizeMB(DEFAULT_MEM_SIZE)
    ,iterations(DEFAULT_ITERATIONS)
  {

    for (int n = 1; n < argc; n++) {
      if (strcmp(argv[n],"-m")==0) {
        n++;
        if (n < argc) {
          memSizeMB=atoi(argv[n]);
        }
        else {
          usage();
          exit(1);
        }
      }
      else if (strcmp(argv[n],"-i")==0) {
        if (!(++n<argc))
          break;
        iterations = atoi(argv[n]);
      }
    }
  };

  void usage() {
    std::cout << argv[0] << " -m <memory size> -i <iterations>" << std::endl;
  };

  unsigned int memSizeMB;
  unsigned int iterations;

private:
  int argc;
  char** argv;
};



int main(int argc, char* argv[]) {

enum {
  MEMTEST_HOST_TO_DEVICE_TEST = 0

  /* Host to Device bandwidth test*/
  ,ARRAYVIEW_HOST_TO_DEVICE
  ,ARRAY_HOST_TO_DEVICE

  ,MEMTEST_DEVICE_TO_HOST_TEST

  /* Device to Host bandwidth tests*/
  ,ARRAYVIEW_DEVICE_TO_HOST
  ,ARRAY_DEVICE_TO_HOST

  ,MEMTEST_LAST
} MemTests;

  Arg parser(argc,argv);
  unsigned int numElements = (parser.memSizeMB*(1024*1024))/sizeof(float);
  unsigned int numIter = parser.iterations;
  srand(time(NULL));

  std::vector<float> v(numElements);
  std::generate(v.begin(), v.end(), unique_number());
  v[v.size()-1] = 0.0f;

  // run the host to device tests
  int (*hostToDeviceTest)(std::vector<float>&);
  for (unsigned int t = MEMTEST_HOST_TO_DEVICE_TEST+1; 
        t != MEMTEST_DEVICE_TO_HOST_TEST; t++) {

    switch(t) {

#ifdef ENABLE_CPPAMP
      case ARRAYVIEW_HOST_TO_DEVICE:
        hostToDeviceTest = arrayViewHostToDeviceBandwidth;
      break;
      case ARRAY_HOST_TO_DEVICE:
        hostToDeviceTest = arrayHostToDeviceBandwidth;
      break;
#endif

      default:
      break;
    };

    for (unsigned int i = 0; i < numIter; i++) {
      int r = hostToDeviceTest(v);
      printf("Pass %d %s\n",i,(const char*)((r==v.size()-1)?"passed":"failed"));
    }
  }


  // run the device to host tests
  printf("run device to host tests\n");
  void (*deviceToHostTest)(std::vector<float>&,const float);
  for (unsigned int t = MEMTEST_DEVICE_TO_HOST_TEST+1; 
        t != MEMTEST_LAST; t++) {

    switch(t) {

#ifdef ENABLE_CPPAMP
      case ARRAYVIEW_DEVICE_TO_HOST:
        deviceToHostTest = arrayViewDeviceToHostBandwidth;
      break;
      case ARRAY_DEVICE_TO_HOST:
        deviceToHostTest = arrayDeviceToHostBandwidth;
      break;
#endif
      
      default:
      break;
    };

    for (unsigned int i = 0; i < numIter; i++) {
      float r = rand();
      deviceToHostTest(v,r);
      int c = std::count_if(v.begin(), v.end(), output_ref(r));
      printf("Pass %d %s\n",i,(const char*)((c==0)?"passed":"failed"));
    }
  }
 


  return 0;
}
