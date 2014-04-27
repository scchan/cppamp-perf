#ifndef _SIMPLETIMER_H
#define _SIMPLETIMER_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <linux/limits.h>
#include <unistd.h>
#endif

#include <string>
#include <iostream>
#include <stack>
#include <queue>

class TimerEvent {
public:
    TimerEvent(const std::string& name) {
      this->name = name;
      startTime = 0;
      endTime = 0;
      nestedLevel = 0;
    }

    void recordStartTime() {
      startTime = getCurrentTime();
    }

    void recordEndTime() {
      endTime = getCurrentTime();
    }

    long long getElapsedTime() {
      return (endTime - startTime);
    }

    ~TimerEvent() {
    }

    std::string name;
    long long startTime;
    long long endTime;
    unsigned int nestedLevel;

private:

    long long getCurrentTime() {
      long long t;
#ifdef _WIN32
      t = GetTickCount64();
#else
      struct timeval s;
      gettimeofday(&s, 0);
      t = (long long)s.tv_sec * (long long)1.0E3 +
          (long long)s.tv_usec / (long long)1.0E3;
#endif
      return t;
    }
};


class TimerStack;

class Timer {
public:
  static TimerStack* defaultTimerStack;
  Timer(const std::string& name, TimerStack* ts);
  ~Timer(); 
  private:
  std::string name;
  TimerStack* tsp;
};
TimerStack* Timer::defaultTimerStack=NULL;


class TimerStack {
public:

  TimerStack();

  void StartTimer(const std::string& name) {
     TimerEvent* e = new TimerEvent(name);
     e->recordStartTime();
     e->nestedLevel = timerStack.size();
     timerStack.push(e);
     startQueue.push(e);
  }
  void StopTimer(const std::string& name) {
     TimerEvent* e = timerStack.top();
     e->recordEndTime();
     timerStack.pop();
     endQueue.push(e);
  }

  void dumpTimerStack() {
    while(!startQueue.empty()) {
      TimerEvent* e = startQueue.front();
      for (unsigned int i = 1; i < e->nestedLevel; i++) {
        std::cout << "  ";
      }
      if (e->nestedLevel!=0)
        std::cout << "|_";
      std::cout << e->name << ": " << e->getElapsedTime() << "ms" << std::endl;
      startQueue.pop();
    }
  }

  ~TimerStack() {
    delete timer;

    dumpTimerStack();

    // delete all the timer events
    while(!endQueue.empty()) {
      delete endQueue.front();
      endQueue.pop();
    }
  }

private:
  std::stack<TimerEvent*> timerStack;
  std::queue<TimerEvent*> startQueue;
  std::queue<TimerEvent*> endQueue;
  Timer* timer;
};


Timer::Timer(const std::string& name, TimerStack* ts=Timer::defaultTimerStack) {
  tsp = ts;
  this->name = name;
  tsp->StartTimer(name);
}

Timer::~Timer() {
  tsp->StopTimer(name);
}

TimerStack::TimerStack(){
  if (Timer::defaultTimerStack==NULL)
    Timer::defaultTimerStack = this;

  timer = new Timer(std::string("TimerStack"));
}

#endif // #endif  _SIMPLETIMER_H

