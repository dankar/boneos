#ifndef _TIMER_H_
#define _TIMER_H_

typedef void (*timer_callback)();

void timer2_register_callback(timer_callback callback);
void timer2_isr(uint32_t addr);
void timer2_enable();

#endif
