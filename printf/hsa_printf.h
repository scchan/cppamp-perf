
#include <cstdlib>
#include <atomic>


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
private:
  HSAPrintfPacketData data;
};

enum HSAPrintfError {
   HSA_PRINTF_SUCCESS = 0
  ,HSA_PRINTF_BUFFER_OVERFLOW = 1
};

class HSAPrintfPacketQueue {
public:
  HSAPrintfPacketQueue(HSAPrintfPacket* buffer, unsigned int num)
        :queue(buffer),num(num) {
    cursor = 0;
    lock = 0;
  }
 
  // get the argument count
  template <typename T> 
  void countArg(unsigned int& count, const T& t) {
    count++;
  }
  template <typename T, typename... Rest> 
  void countArg(unsigned int& count, const T& t, const Rest&... rest) {
    count++;
    countArg(count,rest...);
  }

  template <typename T>
  void set_batch(const T t) {
    queue[cursor++].set(t);
  }
  template <typename T, typename... Rest>
  void set_batch(const T t, Rest... rest) {
    queue[cursor++].set(t);
    set_batch(rest...);
  }

  template <typename... All>
  HSAPrintfError printf(const char* format, All... all) {
    int unlocked = 0;
    int locked = 1;

    unsigned int count = 1;
    countArg(count, all...);

    HSAPrintfError error = HSA_PRINTF_SUCCESS;
    while(1) {
      if (lock.compare_exchange_weak(unlocked, locked
                                     , std::memory_order_acq_rel
                                     , std::memory_order_relaxed)) {
        // run out of space
        if ((count + 1) + cursor >= num) {
          error = HSA_PRINTF_BUFFER_OVERFLOW;
        }
        else {
          queue[cursor++].set(count);
          set_batch(format,all...);
        }
        lock.store(unlocked,std::memory_order_release);
        break;
      }
    }
    return error;
  }

/*
  void print(const char* d0) {
    int unlocked = 0;
    int locked = 1;
    while(1) {
      if (lock.compare_exchange_weak(unlocked, locked
                                     , std::memory_order_acq_rel
                                     , std::memory_order_relaxed)) {
        queue[cursor++].set(1);
        queue[cursor++].set(d0);
        lock.store(unlocked,std::memory_order_release);
        break;
      }
    }
  }

  template<typename T1>
  void print(const char* d0, T1 d1) {
    int unlocked = 0;
    int locked = 1;
    while(1) {
      if (lock.compare_exchange_weak(unlocked, locked
                                     , std::memory_order_acq_rel
                                     , std::memory_order_relaxed)) {
        queue[cursor++].set(2);
        queue[cursor++].set(d0);
        queue[cursor++].set(d1);
        lock.store(unlocked,std::memory_order_release);
        break;
      }
    }
  }

  template<typename T1, typename T2>
  void print(const char* d0, T1 d1, T2 d2) {
    int unlocked = 0;
    int locked = 1;
    while(1) {
      if (lock.compare_exchange_weak(unlocked, locked
                                     , std::memory_order_acq_rel
                                     , std::memory_order_relaxed)) {
        queue[cursor++].set(3);
        queue[cursor++].set(d0);
        queue[cursor++].set(d1);
        queue[cursor++].set(d2);
        lock.store(unlocked,std::memory_order_release);
        break;
      }
    }
  }
*/

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



