

#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "StackTimer.hpp"



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

#endif  // #ifdef ENABLE_CPPAMP






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

#endif // #ifdef ENABLE_CPPAMP

#ifdef ENABLE_OPENCL


#include <CL/cl.h>
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
using namespace cl;

#include "openclkernelhelper.h"

const char* kernelString = STRINGIFY(

  __kernel void deviceToHost(__global float* v, const float r) {
    v[get_global_id(0)]=r+(float)get_global_id(0);
  }

  __kernel void hostToDevice(__global float* v, __global int* i) {
    if (fabs(v[get_global_id(0)] - (float)get_global_id(0)) > 0.0001) {
      i[0] = get_global_id(0);
    }
  }

);


class OpenCLSetup {

public:
  OpenCLSetup() {

    try {
      device = Device::getDefault();
      context = Context(device);
      queue = CommandQueue(context, device);
      program = Program(context, std::string(kernelString));
      program.build();
      deviceToHostKernel = Kernel(program,"deviceToHost");
      hostToDeviceKernel = Kernel(program,"hostToDevice");
    } catch (cl::Error error) {
      std::cerr << "Error: " << error.what() << "(" << error.err() << ")" << std::endl;
    } catch (...) {
      std::cerr << "Error: unknown exception." << std::endl;
    }
  }

  Device device;
  Context context;
  Program program;
  CommandQueue queue;
  Kernel deviceToHostKernel;
  Kernel hostToDeviceKernel;
};
OpenCLSetup oclSetup;

void oclZeroCopyDeviceToHostBandwidth(std::vector<float>& v, const float r) {
  Buffer buffer(oclSetup.context,CL_MEM_USE_HOST_PTR|CL_MEM_WRITE_ONLY
                , v.size()*sizeof(float), &v[0]);
  oclSetup.deviceToHostKernel.setArg(0, buffer);
  oclSetup.deviceToHostKernel.setArg(1, r);
  oclSetup.queue.enqueueNDRangeKernel(oclSetup.deviceToHostKernel
                                    , NullRange, NDRange(v.size()));

  oclSetup.queue.enqueueMapBuffer(buffer, CL_TRUE
                                 , CL_MAP_READ
                                 , 0, v.size()*sizeof(float)
                                 , NULL, NULL, NULL);
}

void oclDeviceMemoryDeviceToHostBandwidth(std::vector<float>& v, const float r) {
  Buffer buffer(oclSetup.context, CL_MEM_WRITE_ONLY
                , v.size()*sizeof(float));
  oclSetup.deviceToHostKernel.setArg(0, buffer);
  oclSetup.deviceToHostKernel.setArg(1, r);
  oclSetup.queue.enqueueNDRangeKernel(oclSetup.deviceToHostKernel
                                    , NullRange, NDRange(v.size()));
  oclSetup.queue.enqueueReadBuffer(buffer, CL_TRUE, 0
                                  , v.size()*sizeof(float), &v[0]);
}

int oclZeroCopyHostToDeviceBandwidth(std::vector<float>& v) {
  Buffer buffer(oclSetup.context,CL_MEM_USE_HOST_PTR|CL_MEM_READ_ONLY
                , v.size()*sizeof(float), &v[0]);

  cl_int i;
  Buffer iBuffer(oclSetup.context,CL_MEM_USE_HOST_PTR|CL_MEM_WRITE_ONLY
                , sizeof(cl_int), &i);


  oclSetup.hostToDeviceKernel.setArg(0, buffer);
  oclSetup.hostToDeviceKernel.setArg(1, iBuffer);
  oclSetup.queue.enqueueNDRangeKernel(oclSetup.hostToDeviceKernel
                                    , NullRange, NDRange(v.size()));
 
  oclSetup.queue.enqueueMapBuffer(iBuffer, CL_TRUE
                                 , CL_MAP_READ
                                 , 0, sizeof(cl_int)
                                 , NULL, NULL, NULL);
  return (int)i;
}

int oclDeviceMemoryHostToDeviceBandwidth(std::vector<float>& v) {
  Buffer buffer(oclSetup.context, CL_MEM_READ_ONLY
                , v.size()*sizeof(float));
  oclSetup.queue.enqueueWriteBuffer(buffer,CL_FALSE,0
                , v.size()*sizeof(float),&v[0]);
  
  cl_int i;
  Buffer iBuffer(oclSetup.context,CL_MEM_USE_HOST_PTR|CL_MEM_WRITE_ONLY
                , sizeof(cl_int), &i);

  oclSetup.hostToDeviceKernel.setArg(0, buffer);
  oclSetup.hostToDeviceKernel.setArg(1, iBuffer);
  oclSetup.queue.enqueueNDRangeKernel(oclSetup.hostToDeviceKernel
                                    , NullRange, NDRange(v.size()));
 
  oclSetup.queue.enqueueMapBuffer(iBuffer, CL_TRUE
                                 , CL_MAP_READ
                                 , 0, sizeof(cl_int)
                                 , NULL, NULL, NULL);
  return (int)i;
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


  ,OCL_ZEROCOPY_HOST_TO_DEVICE
  ,OCL_DEVICE_MEM_HOST_TO_DEVICE

  ,MEMTEST_DEVICE_TO_HOST_TEST
 

  /* Device to Host bandwidth tests*/
  ,ARRAYVIEW_DEVICE_TO_HOST
  ,ARRAY_DEVICE_TO_HOST

  ,OCL_ZEROCOPY_DEVICE_TO_HOST
  ,OCL_DEVICE_MEM_DEVICE_TO_HOST

  ,MEMTEST_LAST
} MemTests;
const char* MemTestNames[] {
  ""
  ,"ArrayView (Host to Device)"
  ,"Array (Host to Device)"
  ,"OpenCL zero-copy (Host to Device)"
  ,"OpenCL device memory (Host to Device)"
  ,""
  ,"ArrayView (Device to Host)"
  ,"Array (Device to Host)"
  ,"OpenCL zero-copy (Device to Host)"
  ,"OpenCL device memory (Device to Host)"
  ,""
};


  Arg parser(argc,argv);
  unsigned int numElements = (parser.memSizeMB*(1024*1024))/sizeof(float);
  unsigned int numIter = parser.iterations;

  std::stringstream output;
  std::string separator(":");
  output << "Transfer Type";
  output << separator << "Transfer Size (MB)";
  output << separator << "Average Time (ms)";
  output << separator << "Bandwidth (GB/s)";
  output << separator << "Errors";
  output << std::endl;

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

#ifdef ENABLE_OPENCL
      case OCL_ZEROCOPY_HOST_TO_DEVICE:
        hostToDeviceTest = oclZeroCopyHostToDeviceBandwidth;
      break;
      case OCL_DEVICE_MEM_HOST_TO_DEVICE:
        hostToDeviceTest = oclDeviceMemoryHostToDeviceBandwidth;
      break;
#endif

      default:
      continue;
      break;
    };

    TimerEventQueue tq;
    int error = 0;
    for (unsigned int i = 0; i < numIter; i++) {
      int result;
      {
        SimpleTimer timer(tq);
        result = hostToDeviceTest(v);
      }
      if (result!=v.size()-1)
        error++;
    }

    if (tq.getNumEvents()>0) {
      double bandwidth = (parser.memSizeMB/1024.0)/(tq.getAverageTime()/1000.0f);
      output << MemTestNames[t];
      output << separator << parser.memSizeMB;
      output << separator << tq.getAverageTime();
      output << separator << bandwidth;
      output << separator << error;
      output << std::endl;
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
#ifdef  ENABLE_OPENCL
      case OCL_ZEROCOPY_DEVICE_TO_HOST:
        deviceToHostTest = oclZeroCopyDeviceToHostBandwidth;
      break;
      case OCL_DEVICE_MEM_DEVICE_TO_HOST:
        deviceToHostTest = oclDeviceMemoryDeviceToHostBandwidth;
      break;
#endif
    
      default:
      continue;
      break;
    };

    TimerEventQueue tq;
    int error = 0;
    for (unsigned int i = 0; i < numIter; i++) {
      float r = rand();
      {
        SimpleTimer timer(tq);
        deviceToHostTest(v,r);
      }
      error+=std::count_if(v.begin(), v.end(), output_ref(r));
    }

    if (tq.getNumEvents() > 0) {
      double bandwidth = (parser.memSizeMB/1024.0)/(tq.getAverageTime()/1000.0f);
      output << MemTestNames[t];
      output << separator << parser.memSizeMB;
      output << separator << tq.getAverageTime();
      output << separator << bandwidth;
      output << separator << error;
      output << std::endl;
    }
  }
 
  std::stringstream filename;
  filename << argv[0] << "_m_" << parser.memSizeMB << "_i_" << parser.iterations << ".log";
  std::ofstream file;
  file.open(filename.str().c_str(), std::ios::trunc);
  file << output.str();
  file.close();

  return 0;
}
