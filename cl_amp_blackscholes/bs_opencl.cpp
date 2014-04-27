

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
//#include "Timer.h"
#include "StackTimer.h"
TimerStack timerStack;

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
  Timer timer(__FUNCTION__);

  for (unsigned int i = 0; i < n; i++) {
    array[i] = (float)rand() / (float)RAND_MAX;
  }
}

#ifdef ENABLE_CODEXL
class amdActivityLogger {
public:

  amdActivityLogger() {
    
    marker = NULL;

    int status = amdtInitializeActivityLogger();

    profilerDetected = false;

    switch (status) {
    case AL_APP_PROFILER_NOT_DETECTED:
      std::cerr << "CodeXL profiler not detected!" << std::endl;
      break;
    case AL_SUCCESS:
      profilerDetected = true;
      marker = new amdtScopedMarker("amdActivityLogger",NULL);
    default:
      std::cerr << "amdtInitializeActivityLogger error: " << status << std::endl;
      exit(1);
    };
  };

  ~amdActivityLogger() {
    if (profilerDetected) {

      if (marker!=NULL)
        delete marker;

      int status = amdtFinalizeActivityLogger();
      switch (status) {
      case AL_SUCCESS:
        break;
      case AL_UNINITIALIZED_ACTIVITY_LOGGER:
      default:
        std::cerr << "amdtFinalizeActivityLogger error: " << status << std::endl;
        exit(1);
      };
    }
  }

private:
  amdtScopedMarker* marker;
  bool profilerDetected;
};

//amdActivityLogger logger;

#endif



void cpuBlackScholes(const float* input, float* call, float* put, unsigned int num) {
#ifdef ENABLE_CODEXL
  amdtScopedMarker marker((const char*)__FUNCTION__,"CPU");
#endif
  Timer timer(__FUNCTION__);
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
  Timer timer(__FUNCTION__);

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

typedef make_kernel<Buffer&, Buffer&, Buffer&> BlackScholesKernelType;

class OpenCLBlackScholes {
public:

  OpenCLBlackScholes(Device device=Device::getDefault()) {
#ifdef ENABLE_CODEXL
    amdtScopedMarker marker((const char*)__FUNCTION__,"");
#endif
    Timer timer(__FUNCTION__);
    try {
      context = Context(device);
      queue = CommandQueue(context, device);
      std::string file = readFile("bs_kernel.h");
      program = Program(context, file);
      program.build("-DOCL_BLACKSCHOLES");
      kernel = Kernel(program, "blackScholes");
    } catch (cl::Error error) {
      std::cerr << "Error: " << error.what() << "(" << error.err() << ")" << std::endl;
    } catch (...) {
      std::cerr << "Error: unknown exception." << std::endl;
    }
  }

  enum OpenCLBSMode {
    BS_HOST_ZERO_COPY
    ,BS_BUFFER_COPY
  };

  void run(float* input
           , float* call
           , float* put
           , const unsigned int num
           , const OpenCLBSMode mode) {

#ifdef ENABLE_CODEXL
    amdtScopedMarker marker((const char*)__FUNCTION__,"");
#endif
    Timer timer(__FUNCTION__);

    Buffer inputBuffer, putBuffer, callBuffer;
    switch(mode) {
      case BS_HOST_ZERO_COPY:
        createBufferZeroCopy(num
                             ,inputBuffer, input
                             ,callBuffer, call
                             ,putBuffer, put);
      break;
      case BS_BUFFER_COPY:
        createDeviceBuffer(num
                             ,inputBuffer, input
                             ,callBuffer
                             ,putBuffer);
      break;
      default:
      break;
    };

    runKernel(num,inputBuffer,callBuffer,putBuffer);

    switch(mode) {
      case BS_HOST_ZERO_COPY:
        mapBufferZeroCopy(num, callBuffer, putBuffer); 
      break;
      case BS_BUFFER_COPY:
        copyFromBuffer(num 
                       ,callBuffer, call
                       ,putBuffer, put);
      break;
      default:
      break;
    }
                  
  }
  

private:
  Device device;
  Context context;
  Program program;
  CommandQueue queue;
  Kernel kernel;

  void createBufferZeroCopy(const unsigned int num
                            , Buffer& inputBuffer, float* input
                            , Buffer& callBuffer, float* call
                            , Buffer& putBuffer, float* put) {
#ifdef ENABLE_CODEXL
    amdtScopedMarker marker((const char*)__FUNCTION__,"");
#endif
    Timer timer(__FUNCTION__);

    inputBuffer = Buffer(context, CL_MEM_USE_HOST_PTR|CL_MEM_READ_ONLY, num*sizeof(float), input);
    putBuffer = Buffer(context, CL_MEM_USE_HOST_PTR|CL_MEM_WRITE_ONLY, num*sizeof(float), put);
    callBuffer = Buffer(context, CL_MEM_USE_HOST_PTR|CL_MEM_WRITE_ONLY, num*sizeof(float), call);
  }

  void mapBufferZeroCopy(const unsigned int num
                         , Buffer& callBuffer
                         , Buffer& putBuffer) {
#ifdef ENABLE_CODEXL
    amdtScopedMarker marker((const char*)__FUNCTION__,"");
#endif
    Timer timer(__FUNCTION__);


    std::vector<Event> mapEvents;
    mapEvents.resize(2);
    queue.enqueueMapBuffer(putBuffer, CL_FALSE
                          , CL_MAP_READ
                          , 0, num*sizeof(float)
                          , NULL, &(mapEvents[0]), NULL);

    queue.enqueueMapBuffer(callBuffer, CL_FALSE
                         , CL_MAP_READ
                         , 0, num*sizeof(float)
                         , NULL, &(mapEvents[1]), NULL);
    WaitForEvents(mapEvents);
  }


  void createDeviceBuffer(const unsigned int num
                         , Buffer& inputBuffer, float* input
                         , Buffer& callBuffer
                         , Buffer& putBuffer) {
#ifdef ENABLE_CODEXL
    amdtScopedMarker marker((const char*)__FUNCTION__,"");
#endif
    Timer timer(__FUNCTION__);

    inputBuffer = Buffer(context, CL_MEM_READ_ONLY, num*sizeof(float));
    queue.enqueueWriteBuffer(inputBuffer,CL_FALSE,0,num*sizeof(float),input);

    putBuffer = Buffer(context, CL_MEM_WRITE_ONLY, num*sizeof(float));
    callBuffer = Buffer(context, CL_MEM_WRITE_ONLY, num*sizeof(float));
    queue.finish();
  }

  void copyFromBuffer(const unsigned int num
                      , Buffer& callBuffer, float* call
                      , Buffer& putBuffer, float* put) {
#ifdef ENABLE_CODEXL
    amdtScopedMarker marker((const char*)__FUNCTION__,"");
#endif
    Timer timer(__FUNCTION__);

    queue.enqueueReadBuffer(putBuffer, CL_FALSE, 0, num*sizeof(float), put);
    queue.enqueueReadBuffer(callBuffer, CL_FALSE, 0, num*sizeof(float), call);
    queue.finish();
  }

  void runKernel(const unsigned int num,
                 Buffer& input, Buffer& call, Buffer& put) {

#ifdef ENABLE_CODEXL
    amdtScopedMarker marker((const char*)__FUNCTION__,"");
#endif
    Timer timer(__FUNCTION__);

    //for (int i = 0; i < 10; i++)
    kernel.setArg(0, input);
    kernel.setArg(1, call);
    kernel.setArg(2, put);
    queue.enqueueNDRangeKernel(kernel, NullRange, NDRange(num));

    queue.finish();
  }


};

#endif

#ifdef ENABLE_CPPAMP


void ampZeroArray(float* a, unsigned int num) {
  Timer timer(__FUNCTION__);

  array_view<float> v(extent<1>(num), a);
  parallel_for_each(extent<1>(num), [=] (index<1> id) restrict(amp) {
    v[id[0]] = 0.0f;
  });
  v.synchronize();
}

void ampBlackScholes(float* inputPtr
                   , float* gpuCall, float* gpuPut
                   , const unsigned int num
                   , float* flag, const unsigned int f) {

  Timer timer(__FUNCTION__);

  array_view<float> inputView(extent<1>(num), inputPtr);
  array_view<float> callView(extent<1>(num), gpuCall);
  array_view<float> putView(extent<1>(num), gpuPut);

  array_view<float> flagView(extent<1>(1),flag);
  {
    char b[128];
    sprintf(b,"%s kernel",__FUNCTION__);
    Timer timer(b);

    parallel_for_each(extent<1>(num), [=] (index<1> id) restrict(amp) {
      float call, put;
      calculateBlackScholes(inputView[id[0]], &call, &put); 
      callView[id[0]] = call;
      putView[id[0]] = put;

      if (id[0]==f) flagView[0] = put;
    });
    flagView.synchronize();
  }

  {
    char b[128];
    sprintf(b,"%s: synchronize callView",__FUNCTION__);
    Timer timer(b);
    callView.synchronize();
  }
  {
    char b[128];
    sprintf(b,"%s: synchronize putView",__FUNCTION__);
    Timer timer(b);
    putView.synchronize();
  }
}

void ampArrayBlackScholes(float* inputPtr
                   , float* gpuCall, float* gpuPut
                   , const unsigned int num) {

  Timer timer(__FUNCTION__);

  array<float,1> inputArray(extent<1>(num),inputPtr);
  array<float,1> callArray(num);
  array<float,1> putArray(num);

  parallel_for_each(extent<1>(num), [&] (index<1> id) restrict(amp) {
    float call, put;
    calculateBlackScholes(inputArray[id[0]], &call, &put); 
    callArray[id[0]] = call;
    putArray[id[0]] = put;
  });

  completion_future cf = copy_async(callArray, gpuCall);
  completion_future pf = copy_async(putArray, gpuPut);
  cf.wait();
  pf.wait();
}

#endif


int main(int argc, char** argv) {

#ifdef ENABLE_CODEXL
  amdActivityLogger logger;
  amdtScopedMarker marker((const char*)__FUNCTION__,"dummy");
#endif

  Timer timer(__FUNCTION__);

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

  OpenCLBlackScholes* oclBlackScholes = new OpenCLBlackScholes();
  oclBlackScholes->run(inputPtr, gpuCall, gpuPut
                      , arg.numInput
                      , OpenCLBlackScholes::BS_HOST_ZERO_COPY);
  verifyBlackScholes(cpuCall, cpuPut, gpuCall, gpuPut, arg.numInput);


  memset(gpuPut,0,arg.numInput*sizeof(float));
  memset(gpuCall,0,arg.numInput*sizeof(float));
  oclBlackScholes->run(inputPtr, gpuCall, gpuPut
                      , arg.numInput
                      , OpenCLBlackScholes::BS_BUFFER_COPY);
  verifyBlackScholes(cpuCall, cpuPut, gpuCall, gpuPut, arg.numInput);

  {
#ifdef ENABLE_CODEXL
    amdtScopedMarker marker("~OpenCLBlackScholes()",NULL);
#endif
    Timer timer("~OpenCLBlackScholes()");

    delete oclBlackScholes;
  }

#endif


  // compute C++AMP BlackScholes

#ifdef ENABLE_CPPAMP

  ampZeroArray(gpuCall, arg.numInput);
  ampZeroArray(gpuPut, arg.numInput);

  {
    float flag;
    unsigned int f = 0;
    ampBlackScholes(inputPtr, gpuCall, gpuPut, arg.numInput
                    , &flag, f);
  }
  verifyBlackScholes(cpuCall, cpuPut, gpuCall, gpuPut, arg.numInput);

  ampZeroArray(gpuCall, arg.numInput);
  ampZeroArray(gpuPut, arg.numInput);


  ampArrayBlackScholes(inputPtr, gpuCall, gpuPut, arg.numInput);
  verifyBlackScholes(cpuCall, cpuPut, gpuCall, gpuPut, arg.numInput);

#endif

  

  delete[] cpuPut;
  delete[] cpuCall;
  delete[] gpuPut;
  delete[] gpuCall;
  delete[] inputPtr;

  return 0;
}
