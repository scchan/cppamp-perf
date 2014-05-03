#ifndef _STACKTIMER_HPP
#define _STACKTIMER_HPP

#include <string>
#include <stack>
#include <queue>

class TimerEvent;



class Timer;
class TimerStack {
public:

  TimerStack();
  void StartTimer(const std::string& name);
  void StopTimer(const std::string& name);
  void dumpTimerStack();
  void setLogPrefix(const std::string& prefix);
  void setLogPrefix(const char* prefix);
  ~TimerStack();

  static TimerStack* getDefaultTimerStack();

private:
  std::stack<TimerEvent*> timerStack;
  unsigned int maxNestedLevel;

  std::queue<TimerEvent*> startQueue;
  std::queue<TimerEvent*> endQueue;
  Timer* timer;
  std::string prefix;

  void dumpTimerStackGoogleTimeline();
};

class Timer {
public:
  Timer(const std::string& name, TimerStack* ts = TimerStack::getDefaultTimerStack());
  Timer(const char* name, TimerStack* ts = TimerStack::getDefaultTimerStack());
  ~Timer();
private:
  std::string name;
  TimerStack* tsp;
};

#ifndef DISABLE_AUTOTIMER
#define AUTOTIMER(VAR,NAME) Timer VAR (NAME) 
#else
#define AUTOTIMER(VAR,NAME)
#endif

#endif // #endif  _STACKTIMER_HPP

