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
#include <iostream>
#include <fstream>
#include <sstream>

#include "StackTimer.hpp"
#include "StackTimer.h"

#include "GoogleTimelineTemplate.h"

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

TimerStack* Timer::defaultTimerStack=NULL;

// Have a global TimerStack object to bootstrap itself
TimerStack defaultTimerStack;
TimerStack* TimerStack::getDefaultTimerStack() { return &defaultTimerStack; };

TimerStack::TimerStack(){
  maxNestedLevel = 0;
  if (Timer::defaultTimerStack==NULL)
    Timer::defaultTimerStack = this;

  std::stringstream ss;
  ss << (int)GETPID();

  prefix = ss.str(); 
  timer = new Timer(std::string("TimerStack"));
}

void TimerStack::dumpTimerStackGoogleTimeline() {

  std::stringstream filename;
  filename << prefix << ".html";
  std::ofstream file;
  file.open(filename.str().c_str(),std::ios::trunc);

  std::stringstream table;

#define ADD_COLUMN(stream,type,id) (stream) << "dataTable.addColumn({ type: '" << (type) << "', id: '" << (id) << "'});" << std::endl;

  ADD_COLUMN(table,std::string("string"),std::string("Nested Level"));
  ADD_COLUMN(table,std::string("string"),std::string("Name"));
  ADD_COLUMN(table,std::string("date"),std::string("Start"));
  ADD_COLUMN(table,std::string("date"),std::string("End"));

  table << "dataTable.addRows([" << std::endl;
#define ADD_ROW(stream,nestedLevel,name,start,end) (stream) << "[ '" << (nestedLevel) << "', '" << (name) \
  << "', new Date(" << (start) << "), new Date(" << (end) << ")]" << "," << std::endl; 

  for (unsigned int i = 0; i < startQueue.size(); i++) {
    TimerEvent* e = startQueue.front();
    ADD_ROW(table,e->nestedLevel,e->name,e->startTime,e->endTime);
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

void TimerStack::StartTimer(const std::string& name) {
  TimerEvent* e = new TimerEvent(name);
  e->recordStartTime();
  e->nestedLevel = timerStack.size();
  maxNestedLevel = (e->nestedLevel > maxNestedLevel)?e->nestedLevel:maxNestedLevel;
  timerStack.push(e);
  startQueue.push(e);
}
void TimerStack::StopTimer(const std::string& name) {
  TimerEvent* e = timerStack.top();
  e->recordEndTime();
  timerStack.pop();
  endQueue.push(e);
}

void TimerStack::dumpTimerStack() {

  std::stringstream filename;
  filename << prefix << ".log";
  std::ofstream file;
  file.open(filename.str().c_str(),std::ios::trunc);

  while(!startQueue.empty()) {
    TimerEvent* e = startQueue.front();
    for (unsigned int i = 1; i < e->nestedLevel; i++) {
      file << "  ";
    }
    if (e->nestedLevel!=0)
      file << "|_";
    file << e->name << ": " << e->getElapsedTime() << "ms" << std::endl;
    startQueue.pop();
  }
  file.close();
}

void TimerStack::setLogPrefix(const std::string& prefix) {
  this->prefix = prefix;
}

void TimerStack::setLogPrefix(const char* prefix) {
  this->prefix = std::string(prefix);
}


TimerStack::~TimerStack() {
  delete timer;
  dumpTimerStackGoogleTimeline();
  dumpTimerStack();

  // delete all the timer events
  while(!endQueue.empty()) {
    delete endQueue.front();
    endQueue.pop();
  }
}


Timer::Timer(const std::string& name, TimerStack* ts)
  :tsp(ts),name(name) {
    tsp->StartTimer(name);
}

Timer::Timer(const char* name, TimerStack* ts) 
  :tsp(ts),name(std::string(name)) {
    tsp->StartTimer(name);
}

Timer::~Timer() {
  tsp->StopTimer(name);
}



struct stimer_struct {
  Timer* timer;
};

extern "C" {

  STimer timer_start(const char* name) {
    STimer t = (STimer) malloc(sizeof(stimer_struct));
    t->timer = new Timer(name);
    return t;
  }

  void timer_stop(STimer t) {
    delete t->timer;
    free(t);
  }

}




