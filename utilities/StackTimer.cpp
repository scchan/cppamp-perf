#ifdef _WIN32
#include <Windows.h>
#include <process.h>

#define GETPID()  _getpid()

#else
#include <sys/time.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <unistd.h>

#define GETPID()  getpid()

#endif

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include "StackTimer.hpp"
#include "StackTimer.h"

#include "GoogleTimelineTemplate.h"

#define PRINT_POINTER(p) printf("%s:%d %s=%p\n",__PRETTY_FUNCTION__,__LINE__,#p,p);


// Have a global TimerStack object to bootstrap itself
TimerStack defaultTimerStack;
TimerStack* TimerStack::getDefaultTimerStack() { return &defaultTimerStack; }

class TimerEvent {
public:
  TimerEvent(const std::string& name) {
    this->name = name;
    startTime = 0;
    endTime = 0;
    nestedLevel = 0;
  }

  TimerEvent(const TimerEvent& e):name(e.name),startTime(e.startTime)
                                  ,endTime(e.endTime),nestedLevel(e.nestedLevel){}

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

class TimerStackImpl {
public:

  TimerStackImpl() {
    maxNestedLevel = 0;
    std::stringstream ss;
    ss << (int)GETPID();
    prefix = ss.str();
  }

  void StartTimer(const char* name);
  void StopTimer(const char* name);
  void dumpTimerStack();
  void setLogPrefix(const std::string& prefix);
  void setLogPrefix(const char* prefix);
  ~TimerStackImpl();

protected:
  std::stack<TimerEvent*> timerStack;
  unsigned int maxNestedLevel;

  std::queue<TimerEvent*> startQueue;
  std::queue<TimerEvent*> endQueue;
  std::string prefix;

  void dumpTimerStackGoogleTimeline();
};



void TimerStackImpl::dumpTimerStackGoogleTimeline() {

  std::stringstream filename;
  filename << prefix << ".html";
  std::ofstream file;
  file.open(filename.str().c_str(), std::ios::trunc);

  std::stringstream table;

#define ADD_COLUMN(stream,type,id) (stream) << "dataTable.addColumn({ type: '" << (type) << "', id: '" << (id) << "'});" << std::endl;

  ADD_COLUMN(table, std::string("string"), std::string("Nested Level"));
  ADD_COLUMN(table, std::string("string"), std::string("Name"));
  ADD_COLUMN(table, std::string("date"), std::string("Start"));
  ADD_COLUMN(table, std::string("date"), std::string("End"));

  table << "dataTable.addRows([" << std::endl;
#define ADD_ROW(stream,nestedLevel,name,start,end) (stream) << "[ '" << (nestedLevel) << "', '" << (name) \
  << "', new Date(" << (start) << "), new Date(" << (end) << ")]" << "," << std::endl;

  for (unsigned int i = 0; i < startQueue.size(); i++) {
    TimerEvent* e = startQueue.front();
    ADD_ROW(table, e->nestedLevel, e->name, e->startTime, e->endTime);
    startQueue.pop();
    startQueue.push(e);
  }

  table << "]);" << std::endl;

  std::string htmlString = std::string(google_html_template);
  size_t location = htmlString.find("<TIMELINE_CHART_DATA>");
  htmlString.replace(location, std::string("<TIMELINE_CHART_DATA>").size(), table.str());
  file << htmlString;

  file.close();
}

void TimerStackImpl::StartTimer(const char* name) {
  TimerEvent* e = new TimerEvent(std::string(name));
  e->nestedLevel = (unsigned int)timerStack.size();
  maxNestedLevel = (e->nestedLevel > maxNestedLevel) ? e->nestedLevel : maxNestedLevel;
  timerStack.push(e);
  startQueue.push(e);
  e->recordStartTime();
}
void TimerStackImpl::StopTimer(const char* name) {
  TimerEvent* e = timerStack.top();
  e->recordEndTime();
  timerStack.pop();
  endQueue.push(e);
}



class EventAverage {
public:
  EventAverage(std::string name):name(name),num(0),total(0){}
  EventAverage(std::string name, long long value):name(name),num(1),total(value){}
  void add(long long v) {
    total+=v;
    num++;
  }
  double getAverage() {
    if (num==0) 
      return 0.0;
    else
      return (double)total/(double)num;
  }
  std::string name;
  unsigned int num;
  long long total;
};

class EventAverageManager {
public:
  void addEvent(std::string name, long long value) {
    for (std::vector<EventAverage>::iterator iter = data.begin();
        iter != data.end(); iter++) {
      if (iter->name==name) {
        iter->add(value);
        return;
      }
    }
    data.push_back(EventAverage(name,value));
  }
  std::vector<EventAverage> data;
};

void TimerStackImpl::dumpTimerStack() {

  std::stringstream filename;
  filename << prefix << ".log";
  std::ofstream file;
  file.open(filename.str().c_str(), std::ios::trunc);

  EventAverageManager avg;

  while (!startQueue.empty()) {
    TimerEvent* e = startQueue.front();
    for (unsigned int i = 1; i < e->nestedLevel; i++) {
      file << "  ";
    }
    if (e->nestedLevel != 0)
      file << "|_";
    file << e->name << ": " << e->getElapsedTime() << "ms" << std::endl;

    avg.addEvent(e->name, e->getElapsedTime());
    startQueue.pop();
  }

  // print the averages
  file << std::endl;
  file << "Event averages:" << std::endl;
  for (std::vector<EventAverage>::iterator iter = avg.data.begin();
    iter != avg.data.end(); iter++) {
    file << "\t" << iter->name << ": " << iter->getAverage() << "ms" << std::endl;
  }
  file.close();
}

void TimerStackImpl::setLogPrefix(const char* prefix) {
  this->prefix = std::string(prefix);
}


TimerStackImpl::~TimerStackImpl() {
  dumpTimerStackGoogleTimeline();
  dumpTimerStack();
  // delete all the timer events
  while (!endQueue.empty()) {
    delete endQueue.front();
    endQueue.pop();
  }
}



TimerStack::TimerStack() {
  impl = new TimerStackImpl();
  timer = new Timer("TimerStack",this);
}
TimerStack::~TimerStack() {
  delete timer;
  delete impl;
}
void TimerStack::StartTimer(const char* name) {
  impl->StartTimer(name);
}
void TimerStack::StopTimer(const char* name) {
  impl->StopTimer(name);
}
void TimerStack::dumpTimerStack() {
  impl->dumpTimerStack();
}
void TimerStack::setLogPrefix(const char* prefix) {
  impl->setLogPrefix(prefix);
}


Timer::Timer(const char* name, TimerStack* ts):tsp(ts) {
  this->name = strdup(name);
  tsp->StartTimer(this->name);
}

Timer::~Timer() {
  tsp->StopTimer(name);
  free(this->name);
}





class TimerEventQueueImpl {
public:
  unsigned int getNewTimer(const char* name) {
    timers.push_back(TimerEvent(std::string(name)));
    return timers.size()-1;
  }
  TimerEvent* getTimer(const unsigned int index) {
    if (index < timers.size())
      return &timers[index];
    else
      return NULL;
  }
  void clear()                 { timers.clear(); }
  unsigned long getNumEvents() { return timers.size(); }
  long long getTotalTime() {
    long long total = 0;
    for (std::vector<TimerEvent>::iterator it = timers.begin();
      it != timers.end(); it++) {
      total+=it->getElapsedTime();
    }
    return total;
  }
  double getAverageTime() {
    if (timers.size() == 0) 
      return 0.0;
    else
      return (getTotalTime()/(double)timers.size());
  }
protected:
  std::vector<TimerEvent> timers;
};


TimerEventQueue::TimerEventQueue() {
  impl = new TimerEventQueueImpl();
}

TimerEventQueue::~TimerEventQueue() {
  if (impl)
    delete impl;
}

unsigned int TimerEventQueue::getNewTimer(const char* name) {
  return impl->getNewTimer(name);
}

TimerEvent* TimerEventQueue::getTimerEvent(const unsigned int index) {
  return impl->getTimer(index);
}

void TimerEventQueue::clear()                 { impl->clear(); }
unsigned long TimerEventQueue::getNumEvents() { return impl->getNumEvents(); }
double TimerEventQueue::getAverageTime()   { return impl->getAverageTime(); }


SimpleTimer::SimpleTimer(TimerEventQueue& q, const char* name)  {
  this->q = &q;
  index = q.getNewTimer(name);
  q.getTimerEvent(index)->recordStartTime();
}
SimpleTimer::~SimpleTimer() {
  q->getTimerEvent(index)->recordEndTime();
}

struct stimer_struct {
  Timer* timer;
};

extern "C" {

  STimer timer_start(const char* name) {
    STimer t = (STimer)malloc(sizeof(stimer_struct));
    t->timer = new Timer(name);
    return t;
  }

  void timer_stop(STimer t) {
    delete t->timer;
    free(t);
  }

}
