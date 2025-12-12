#include "timer_manager.h"
#include "stm32f1xx.h"
#include <stddef.h>

// Статические экземпляры таймеров
static SystemTimer controller_timer = {(volatile uint32_t*)TIM2, CONTROLLER_FREQ_HZ, NULL, false};
static SystemTimer filter_timer = {(volatile uint32_t*)TIM3, FILTER_FREQ_HZ, NULL, false};
static SystemTimer motor_timer = {(volatile uint32_t*)TIM4, MOTOR_FREQ_HZ, NULL, false};

// Счетчики тиков
static volatile uint32_t controller_tick = 0;
static volatile uint32_t filter_tick = 0;
static volatile uint32_t motor_tick = 0;

// Вспомогательная функция настройки таймера
static void Timer_Configure(SystemTimer* timer, TIM_TypeDef* TIMx, 
                           uint32_t freq_hz, uint8_t irq_channel)
{
    uint32_t psc, arr;
    
    uint32_t timer_clock = 72000000; // 72 MHz
    
    // Выбираем psc так, чтобы (timer_clock / (psc + 1)) было около 1 МГц
    psc = (timer_clock / 1000000) - 1; // 72 MHz / 1 MHz = 72, значит psc = 71
    
    // Теперь arr = (1 MHz / freq_hz) - 1
    arr = (1000000 / freq_hz) - 1;
    
    // Для 20000 Гц: arr = (1000000 / 20000) - 1 = 50 - 1 = 49
    // Для 10000 Гц: arr = (1000000 / 10000) - 1 = 100 - 1 = 99  
    // Для 5000 Гц:  arr = (1000000 / 5000) - 1 = 200 - 1 = 199
    
    if (psc > 65535) psc = 65535;
    if (arr > 65535) arr = 65535;
    
    // Проверка минимального arr
    if (arr == 0) {
        // Если нужна частота 1 МГц или больше
        psc = 0; // Без предделителя
        arr = (timer_clock / freq_hz) - 1;
    }
    
    timer->timer = (volatile uint32_t*)TIMx;
    timer->freq_hz = freq_hz;
    
    // Включение тактирования таймера
    if (TIMx == TIM2) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    } else if (TIMx == TIM3) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    } else if (TIMx == TIM4) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    }
    
    // Сброс и настройка таймера
    TIMx->CR1 = 0;
    TIMx->PSC = psc;
    TIMx->ARR = arr;
    
    // Включение прерывания по переполнению
    TIMx->DIER |= TIM_DIER_UIE;
    
    // Разрешение прерывания в NVIC
    NVIC->ISER[irq_channel >> 5] = (1 << (irq_channel & 0x1F));
    NVIC_SetPriority(irq_channel, 1); // Средний приоритет
    
    timer->is_initialized = true;
}

void TimerManager_Init(void)
{
    // Настройка контроллера на TIM2 (20 кГц)
    Timer_Configure(&controller_timer, TIM2, CONTROLLER_FREQ_HZ, TIM2_IRQn);
    
    // Настройка фильтра на TIM3 (10 кГц)
    Timer_Configure(&filter_timer, TIM3, FILTER_FREQ_HZ, TIM3_IRQn);
    
    // Настройка двигателя на TIM4 (5 кГц)
    Timer_Configure(&motor_timer, TIM4, MOTOR_FREQ_HZ, TIM4_IRQn);
}

void TimerManager_Start(void)
{
    if (controller_timer.is_initialized) {
        TIM2->CR1 |= TIM_CR1_CEN;
    }
    if (filter_timer.is_initialized) {
        TIM3->CR1 |= TIM_CR1_CEN;
    }
    if (motor_timer.is_initialized) {
        TIM4->CR1 |= TIM_CR1_CEN;
    }
}

void TimerManager_Stop(void)
{
    if (controller_timer.is_initialized) {
        TIM2->CR1 &= ~TIM_CR1_CEN;
    }
    if (filter_timer.is_initialized) {
        TIM3->CR1 &= ~TIM_CR1_CEN;
    }
    if (motor_timer.is_initialized) {
        TIM4->CR1 &= ~TIM_CR1_CEN;
    }
}

void TimerManager_SetControllerCallback(TimerCallback callback)
{
    controller_timer.callback = callback;
}

void TimerManager_SetFilterCallback(TimerCallback callback)
{
    filter_timer.callback = callback;
}

void TimerManager_SetMotorCallback(TimerCallback callback)
{
    motor_timer.callback = callback;
}

uint32_t TimerManager_GetControllerTick(void)
{
    return controller_tick;
}

uint32_t TimerManager_GetFilterTick(void)
{
    return filter_tick;
}

uint32_t TimerManager_GetMotorTick(void)
{
    return motor_tick;
}

// Обработчики прерываний (вызываются из stm32f1xx_it.c)
void TimerManager_ControllerIRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF; // Сброс флага прерывания
        controller_tick++;
        
        if (controller_timer.callback != NULL) {
            controller_timer.callback();
        }
    }
}

void TimerManager_FilterIRQHandler(void)
{
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF; // Сброс флага прерывания
        filter_tick++;
        
        if (filter_timer.callback != NULL) {
            filter_timer.callback();
        }
    }
}

void TimerManager_MotorIRQHandler(void)
{
    if (TIM4->SR & TIM_SR_UIF) {
        TIM4->SR &= ~TIM_SR_UIF; // Сброс флага прерывания
        motor_tick++;
        
        if (motor_timer.callback != NULL) {
            motor_timer.callback();
        }
    }
}