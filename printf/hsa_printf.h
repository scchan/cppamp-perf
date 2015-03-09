
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <atomic>
#include <string>
#include <regex>
#include <iostream>

//#define HSA_PRINTF_DEBUG   1

#define VA_NARGS_IMPL(_1, _2, _3, _4, _5, N, ...) N
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__, 5, 4, 3, 2, 1)

#define HSA_PRINTF_IMPL2(count, ...) hsa_printf ## count (__VA_ARGS__)
#define HSA_PRINTF_IMPL(count, ...) HSA_PRINTF_IMPL2(count, __VA_ARGS__) 
#define HSA_PRINTF(...) HSA_PRINTF_IMPL(VA_NARGS(__VA_ARGS__), __VA_ARGS__)


union HSAPrintfPacketData {
  unsigned int ui;
  int i;
  float f;
  void* ptr;
  const void* cptr;
};

enum HSAPrintfPacketDataType {
   HSA_PRINTF_UNSIGNED_INT = 0
  ,HSA_PRINTF_SIGNED_INT
  ,HSA_PRINTF_FLOAT
  ,HSA_PRINTF_VOID_PTR
  ,HSA_PRINTF_CONST_VOID_PTR
};


class HSAPrintfPacket {
public:
  void set(unsigned int d)  { type = HSA_PRINTF_UNSIGNED_INT;   data.ui = d; }
  void set(int d)           { type = HSA_PRINTF_SIGNED_INT;     data.i = d; }
  void set(float d)         { type = HSA_PRINTF_FLOAT;          data.f = d; }
  void set(void* d)         { type = HSA_PRINTF_VOID_PTR;       data.ptr = d; }
  void set(const void* d)   { type = HSA_PRINTF_CONST_VOID_PTR; data.cptr = d; }
  HSAPrintfPacketDataType type;
  HSAPrintfPacketData data;
};

enum HSAPrintfError {
   HSA_PRINTF_SUCCESS = 0
  ,HSA_PRINTF_BUFFER_OVERFLOW = 1
};

class HSAPrintfPacketQueue {
public:
  HSAPrintfPacketQueue(HSAPrintfPacket* buffer, unsigned int num)
        :queue(buffer),num(num),cursor(0){
    lock = 0;
  }
  HSAPrintfPacket* queue;
  unsigned int num;
  unsigned int cursor;
  std::atomic_int lock;
};


static inline HSAPrintfPacketQueue* createHSAPrintfPacketQueue(unsigned int num) {
  HSAPrintfPacket* buffer = new HSAPrintfPacket[num];
  HSAPrintfPacketQueue* queue = new HSAPrintfPacketQueue(buffer, num);
  return queue;
}

static inline HSAPrintfPacketQueue* destroyHSAPrintfPacketQueue(HSAPrintfPacketQueue* queue) {
  delete[]  queue->queue;
  delete queue;
  return NULL;
}



 
// get the argument count
static inline void countArg(unsigned int& count) {}

template <typename T> 
static inline void countArg(unsigned int& count, const T& t) {
  count++;
}
template <typename T, typename... Rest> 
static inline void countArg(unsigned int& count, const T& t, const Rest&... rest) {
  count++;
  countArg(count,rest...);
}

template <typename T>
static inline void set_batch(HSAPrintfPacketQueue* queue, const T t) {
  queue->queue[queue->cursor++].set(t);
}
template <typename T, typename... Rest>
static inline void set_batch(HSAPrintfPacketQueue* queue, const T t, Rest... rest) {
  queue->queue[queue->cursor++].set(t);
  set_batch(queue, rest...);
}

template <typename... All>
static inline HSAPrintfError hsa_printf(HSAPrintfPacketQueue* queue, const char* format, All... all) {
  int unlocked = 0;
  int locked = 1;

  unsigned int count = 0;      
  countArg(count, format, all...);

#ifdef HSA_PRINTF_DEBUG
  printf("%s:%d \t count = %d\n", __FUNCTION__, __LINE__, count);
#endif

  HSAPrintfError error = HSA_PRINTF_SUCCESS;
  while(1) {
    if (queue->lock.compare_exchange_weak(unlocked, locked
                                   , std::memory_order_acq_rel
                                   , std::memory_order_relaxed)) {
      // run out of space
      if ((count + 1) + queue->cursor >= queue->num) {
        error = HSA_PRINTF_BUFFER_OVERFLOW;
      }
      else {
        set_batch(queue, count, format, all...);
      }
      queue->lock.store(unlocked,std::memory_order_release);
      break;
    }
  }
  return error;
}


static inline void hsa_process_printf_queue(HSAPrintfPacketQueue* queue) {
  int unlocked = 0;
  int locked = 1;
  while(!queue->lock.compare_exchange_weak(unlocked, locked
                                   , std::memory_order_acq_rel
                                   , std::memory_order_relaxed)) ; 


  // regex for finding format string specifiers
  std::regex specifierPattern("\%[idfs]");

  unsigned int numPackets = 0;
  for (unsigned int i = 0; i < queue->cursor; ) {

    numPackets = queue->queue[i++].data.ui;
    if (numPackets == 0)
      continue;

    // get the format
    unsigned int formatStringIndex = i++;
    assert(queue->queue[formatStringIndex].type == HSA_PRINTF_VOID_PTR
           || queue->queue[formatStringIndex].type == HSA_PRINTF_CONST_VOID_PTR);
    std::string formatString((const char*)queue->queue[formatStringIndex].data.cptr);


    unsigned int formatStringCursor = 0;
    std::smatch specifierMatches;

#ifdef HSA_PRINTF_DEBUG
    //printf("%s:%d \t number of matches = %d\n", __FUNCTION__, __LINE__, (int)specifierMatches.size());
#endif

    
    for (unsigned int j = 1; j < numPackets; j++,i++) {

      std::regex_search(formatString, specifierMatches, specifierPattern);



      std::string specifier = specifierMatches.str();
#ifdef HSA_PRINTF_DEBUG
      std::cout << " (specifier found: " << specifier << ") ";
#endif

      printf("%s", std::string(specifierMatches.prefix()).c_str());

      if (specifier == std::string("%d")) {

        int value = 0;
        switch(queue->queue[i].type) {
          case HSA_PRINTF_UNSIGNED_INT:
          case HSA_PRINTF_SIGNED_INT:
            value = queue->queue[i].data.i;
            break;

          case HSA_PRINTF_FLOAT:
            value = (int)queue->queue[i].data.f;
            break;
          default:
            break;
        };
        printf(specifier.c_str(), value);


      } else if (specifier == std::string("%f")) {


        float value = 0.0f;
        switch(queue->queue[i].type) {
          case HSA_PRINTF_UNSIGNED_INT:
          case HSA_PRINTF_SIGNED_INT:
            value = (float)queue->queue[i].data.i;
            break;

          case HSA_PRINTF_FLOAT:
            value = queue->queue[i].data.f;
            break;
          default:
            break;
        };
        printf(specifier.c_str(), value);
        
      }
      else {
        assert(false);
      }
      formatString = specifierMatches.suffix();
    }
    printf("%s", formatString.c_str());
  }
  queue->cursor = 0;
  queue->lock.store(unlocked,std::memory_order_release);
}
