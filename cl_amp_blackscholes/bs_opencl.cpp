

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#ifdef ENABLE_CODEXL
  #include "AMDTActivityLogger.h"
#endif

#ifdef ENABLE_CPPAMP

#include <amp.h>
#include <amp_math.h>
using namespace concurrency;

#else

#include <cmath>

#endif


#include "bs_kernel.h"
#include "readfile.hpp"
#include "SimpleTimer.h"


class Arg {

public:

  Arg(int argc, char** argv):argc(argc),argv(argv) {
    numInput = 1024*1024;
    for (int n = 1; n < argc; n++) {
      if (strcmp(argv[n],"-n")==0) {
        n++;
        if (n < argc) {
          numInput=atoi(argv[n]);
        }
        else {
          usage();
          exit(1);
        }
      }
    }

    std::cout << "Number of input: " << numInput << std::endl;
  };

  void usage() {
    std::cout << argv[0] << " -n <num input>" << std::endl;
  };

  unsigned int numInput;

private:
  int argc;
  char** argv;
};


static void genRandomInput(float* array, unsigned int n) {
#ifdef ENABLE_CODEXL
  amdtScopedMarker marker((const char*)__FUNCTION__,"CPU");
#endif
  for (int i = 0; i < n; i++) {
    array[i] = (float)rand() / (float)RAND_MAX;
  }
}

#ifdef ENABLE_CODEXL
class amdActivityLogger {
public:

  amdActivityLogger() {
    int status = amdtInitializeActivityLogger();

    profilerDetected = false;

    switch (status) {
    case AL_SUCCESS:
      profilerDetected = true;
      break;
    case AL_APP_PROFILER_NOT_DETECTED:
      break;
    default:
      std::cerr << "amdtInitializeActivityLogger error: " << status << std::endl;
      exit(1);
    };
  };

  ~amdActivityLogger() {
    int status = amdtFinalizeActivityLogger();

    switch (status) {
    case AL_SUCCESS:
      break;
    case AL_UNINITIALIZED_ACTIVITY_LOGGER:
      if (!profilerDetected)
        break;
    default:
      std::cerr << "amdtFinalizeActivityLogger error: " << status << std::endl;
      exit(1);
    };
  }

private:
  bool profilerDetected;
};

//amdActivityLogger logger;

#endif



void cpuBlackScholes(const float* input, float* call, float* put, unsigned int num) {
#ifdef ENABLE_CODEXL
  amdtScopedMarker marker((const char*)__FUNCTION__,"CPU");
#endif
  SimpleTimer timer(__FUNCTION__);
  for (unsigned int i = 0; i < num; i++) {
    float c, p;
    calculateBlackScholes(input[i], &c, &p);
    call[i] = c;
    put[i] = p;
  }
}

void verifyBlackScholes(const float* cpuCall, const float* cpuPut
                        , const float* gpuCall, const float* gpuPut
                        , const unsigned int num) {
#ifdef ENABLE_CODEXL
  amdtScopedMarker marker((const char*)__FUNCTION__,"CPU");
#endif
  SimpleTimer timer(__FUNCTION__);

  // verify the results
  for (unsigned int i = 0; i < num; i++) {
    float cPut  = cpuPut[i];
    float cCall = cpuCall[i];
    float gPut  = gpuPut[i];
    float gCall = gpuCall[i];

    if (fabs(cPut - gPut) >= 0.0001f) {
      std::cerr << "Error: cpuPut[" << i << "]=" << cPut;
      std::cerr << "\t gpuPut[" << i << "]=" << gPut << std::endl;
      exit(1);
    }

    if (fabs(cCall - gCall) >= 0.0001f) {
      std::cerr << "Error: cpuCall[" << i << "]=" << cCall;
      std::cerr << "\t gpuCall[" << i << "]=" << gCall << std::endl;
      exit(1);
    }
  }
  std::cout << "Verified!" << std::endl;
}

#ifdef ENABLE_OPENCL

#include <CL/cl.h>
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
using namespace cl;


void openclBlackScholes(float* inputPtr
                        , float* gpuCall, float* gpuPut
                        , const unsigned int num) {
#ifdef ENABLE_CODEXL
  amdtScopedMarker marker((const char*)__FUNCTION__,"");
#endif
  SimpleTimer timer(__FUNCTION__);

  try {
    Device device = Device::getDefault();
    Context context(device);

    std::string file = readFile("bs_kernel.h");
    Program program(context, file);
    program.build("-DOCL_BLACKSCHOLES");
    typedef make_kernel<Buffer&, Buffer&, Buffer&> BlackScholesKernelType;
    BlackScholesKernelType blackScholesKernel(program,"blackScholes");

    Buffer input(context, CL_MEM_USE_HOST_PTR, num*sizeof(float), inputPtr);
    Buffer put(context, CL_MEM_USE_HOST_PTR|CL_MEM_WRITE_ONLY, num*sizeof(float), gpuPut);
    Buffer call(context, CL_MEM_USE_HOST_PTR|CL_MEM_WRITE_ONLY, num*sizeof(float), gpuCall);

    CommandQueue queue(context,device);
    EnqueueArgs args(queue,NDRange(num));

    //for (int i = 0; i < 10; i++)
    blackScholesKernel(args, input, call, put);

    std::vector<Event> mapEvents;
    mapEvents.resize(2);
    queue.enqueueMapBuffer(put, CL_FALSE
                          , CL_MAP_READ
                          , 0, num*sizeof(float)
                          , NULL, &(mapEvents[0]), NULL);

    queue.enqueueMapBuffer(call, CL_FALSE
                         , CL_MAP_READ
                         , 0, num*sizeof(float)
                         , NULL, &(mapEvents[1]), NULL);
    WaitForEvents(mapEvents);

  } catch (cl::Error error) {
    std::cerr << "Error: " << error.what() << "(" << error.err() << ")" << std::endl;
  } catch (...) {
    std::cerr << "Error: unknown exception." << std::endl;
  }
}

#endif

#ifdef ENABLE_CPPAMP




void ampBlackScholes(float* inputPtr
                   , float* gpuCall, float* gpuPut
                   , const unsigned int num) {

  SimpleTimer timer(__FUNCTION__);

  array_view<float> inputView(extent<1>(num), inputPtr);
  array_view<float> callView(extent<1>(num), gpuCall);
  array_view<float> putView(extent<1>(num), gpuPut);

  parallel_for_each(extent<1>(num), [=] (index<1> id) restrict(amp) {
    float call, put;
    calculateBlackScholes(inputView[id[0]], &call, &put); 
    callView[id[0]] = call;
    putView[id[0]] = put;
  });
  callView.synchronize();
  putView.synchronize();
}
#endif


int main(int argc, char** argv) {

#ifdef ENABLE_CODEXL
  amdActivityLogger logger;
#endif

  SimpleTimer timer(__FUNCTION__);

  Arg arg(argc,argv);

  float* inputPtr = new float[arg.numInput];
  genRandomInput(inputPtr, arg.numInput);

  // compute CPU BlackScholes
  float *cpuPut, *cpuCall;
  cpuPut = new float[arg.numInput];
  cpuCall = new float[arg.numInput];
  cpuBlackScholes(inputPtr, cpuCall, cpuPut, arg.numInput);
  

  // compute OpenCL BlackScholes 
  float* gpuPut = new float[arg.numInput];
  float* gpuCall = new float[arg.numInput];

#ifdef ENABLE_OPENCL
  openclBlackScholes(inputPtr, gpuCall, gpuPut, arg.numInput);
  verifyBlackScholes(cpuCall, cpuPut, gpuCall, gpuPut, arg.numInput);
#endif


  // compute C++AMP BlackScholes

#ifdef ENABLE_CPPAMP
  ampBlackScholes(inputPtr, gpuCall, gpuPut, arg.numInput);
  verifyBlackScholes(cpuCall, cpuPut, gpuCall, gpuPut, arg.numInput);
#endif

  delete[] cpuPut;
  delete[] cpuCall;
  delete[] gpuPut;
  delete[] gpuCall;
  delete[] inputPtr;

  return 0;
}
