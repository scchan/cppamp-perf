
#include <cstdlib>
#include "StackTimer.hpp"
#include "StackTimer.h"

#include "GoogleTimelineTemplate.h"

TimerStack* Timer::defaultTimerStack=NULL;
TimerStack timerStack;


void TimerStack::dumpTimerStackGoogleTimeline() {

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

  std::string htmlString = std::string(google_html_template);
  size_t location = htmlString.find("<TIMELINE_CHART_DATA>");
  htmlString.replace(location, std::string("<TIMELINE_CHART_DATA>").size(), table.str());
  file << htmlString;
    
  file.close();
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




