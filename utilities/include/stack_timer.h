
#ifndef _STACK_TIMER_H
#define _STACK_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct stimer_struct* STimer; 

STimer timer_start(const char* name);
void timer_stop(STimer timer);

#ifdef __cplusplus
}
#endif


#endif // #ifndef _STACK_TIMER_H
