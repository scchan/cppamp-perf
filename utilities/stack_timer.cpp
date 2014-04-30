
#include <cstdlib>
#include "StackTimer.h"
//#include "stack_timer.h"

TimerStack* Timer::defaultTimerStack=NULL;
TimerStack timerStack;

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

