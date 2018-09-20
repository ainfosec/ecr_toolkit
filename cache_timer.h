#ifndef CACHE_TIMER_H
#define CACHE_TIMER_H

#include <linux/sched.h>
#include <linux/delay.h>

//#include "main.h"


#define COUNT 100

int cache_timing_handler(int offset);
int cache_timing_start(void);

#endif
