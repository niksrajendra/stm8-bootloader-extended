#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define IWDG_KEY_ENABLE         0xCC
#define IWDG_KEY_REFRESH        0xAA
#define IWDG_KEY_ACCESS         0x55

//extern uint16_t globalCounter;

void timer4_init(void);

void timerUs(uint16_t value);
void timerMs(uint16_t value);
void timerS(uint16_t value);

void iwdg_init(void);
void iwdg_refresh(void);

#endif // TIMER_H