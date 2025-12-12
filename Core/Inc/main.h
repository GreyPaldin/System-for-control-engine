#ifndef MAIN_H
#define MAIN_H

#include "stm32f1xx.h"

// Прототипы функций
void SystemClock_Config(void);
void GPIO_Init(void);
void Delay(uint32_t count);

#endif