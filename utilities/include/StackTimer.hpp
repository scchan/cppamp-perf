#ifndef _STACKTIMER_HPP
#define _STACKTIMER_HPP

#include <string>
#include <stack>
#include <queue>

class TimerEvent;
class Timer;


class TimerStackImpl;
class TimerStack {
public:
  TimerStack();
  ~TimerStack();

  void StartTimer(const char* name);
  void StopTimer(const char* name);
  
  void dumpTimerStack();
  void setLogPrefix(const char* prefix);
  static TimerStack* getDefaultTimerStack();
protected:
  TimerStackImpl* impl;
  Timer* timer;
};

class Timer {
public:
  Timer(const char* name, TimerStack* ts = TimerStack::getDefaultTimerStack());
  ~Timer();
private:
  char* name;
  TimerStack* tsp;
};


class TimerEventQueueImpl;
class TimerEventQueue {
public:
  TimerEventQueue();
  ~TimerEventQueue();
  unsigned int getNewTimer(const char* name="");
  TimerEvent* getTimerEvent(unsigned int index);
  double getAverageTime();
  unsigned int getNumEvents();
  void clear();
private:
  TimerEventQueueImpl* impl;
};


class SimpleTimer {
public:
  SimpleTimer(TimerEventQueue& q, const char* name="");
  ~SimpleTimer();
private:
  TimerEventQueue* q;
  unsigned int index;
};

#ifndef DISABLE_AUTOTIMER
#define AUTOTIMER(VAR,NAME) Timer VAR (NAME) 
#else
#define AUTOTIMER(VAR,NAME)
#endif

#endif // #endif  _STACKTIMER_HPP

