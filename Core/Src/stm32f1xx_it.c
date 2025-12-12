#include "stm32f1xx.h"
#include "timer_manager.h"

// Обработчик прерывания TIM2 (контроллер, 20 кГц)
void TIM2_IRQHandler(void) __attribute__((interrupt));
void TIM2_IRQHandler(void)
{
    TimerManager_ControllerIRQHandler();
}

// Обработчик прерывания TIM3 (фильтр, 10 кГц)
void TIM3_IRQHandler(void) __attribute__((interrupt));
void TIM3_IRQHandler(void)
{
    TimerManager_FilterIRQHandler();
}

// Обработчик прерывания TIM4 (двигатель, 5 кГц)
void TIM4_IRQHandler(void) __attribute__((interrupt));
void TIM4_IRQHandler(void)
{
    TimerManager_MotorIRQHandler();
}

// Обработчик SysTick (для общего времени) (в итоге не используется)
void SysTick_Handler(void) __attribute__((interrupt));
void SysTick_Handler(void)
{

}