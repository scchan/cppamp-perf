#ifndef _SIMPLETIMER_H
#define _SIMPLETIMER_H

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

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <queue>

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
     maxNestedLevel = (e->nestedLevel > maxNestedLevel)?e->nestedLevel:maxNestedLevel;
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

  void dumpTimerStackTimeline() {

    std::stringstream filename;
    filename << (int)GETPID() << ".html";
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

    std::string htmlString = std::string(html_template);
    size_t location = htmlString.find("<TIMELINE_CHART_DATA>");
    htmlString.replace(location, std::string("<TIMELINE_CHART_DATA>").size(), table.str());
    file << htmlString;
    
    file.close();
  }



  ~TimerStack() {
    delete timer;
    dumpTimerStackTimeline();
    dumpTimerStack();

    // delete all the timer events
    while(!endQueue.empty()) {
      delete endQueue.front();
      endQueue.pop();
    }
  }

private:
  std::stack<TimerEvent*> timerStack;
  unsigned int maxNestedLevel;

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
  maxNestedLevel = 0;
  if (Timer::defaultTimerStack==NULL)
    Timer::defaultTimerStack = this;
  timer = new Timer(std::string("TimerStack"));
}

#endif // #endif  _SIMPLETIMER_H

