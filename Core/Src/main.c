#include "stm32f1xx.h"
#include "system.h"
#include "timer_manager.h"
#include "system_config.h"
#include "main.h" 

int main(void)
{
    // Настройка тактовой частоты (72 MHz от HSE 8MHz)
    SystemClock_Config();
    
    // Инициализация GPIO
    GPIO_Init();
    
    // Инициализация системы управления
    System_Init();
    
    // Инициализация таймеров
    TimerManager_Init();
    
    // Установка колбэков для прерываний
    TimerManager_SetControllerCallback(System_ControllerUpdate);
    TimerManager_SetFilterCallback(System_FilterUpdate);
    TimerManager_SetMotorCallback(System_MotorUpdate);
    
    // НАЧАЛО ЭКСПЕРИМЕНТА:
    // Сбрасываем буфер
    System_ClearBuffer();
    
    // Начинаем запись
    System_StartRecording();
    
    // Устанавливаем задание (ступенька)
    System_SetSetpoint(1.0f);
    
    // Запуск таймеров 
    TimerManager_Start();
    
    // Включение глобальных прерываний
    __enable_irq();
    
    while (1)
    {
        // Мигание светодиодом для индикации работы
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        Delay(1000000); // 1 секунда задержки
        
        //Стоп записи через 10 секунд
        static uint32_t loop_counter = 0;
        loop_counter++;
        if (loop_counter == 10)
        {
            System_StopRecording();
            
        }
    }
}

// Базовая настройка тактирования
void SystemClock_Config(void)
{
    // Включение HSE
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    
    // Настройка флэш-памяти
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;
    
    // Настройка PLL:  72 MHz
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
    RCC->CFGR |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9;
    
    // Включение PLL
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));
    
    // Переключение на PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
    
    // Настройка шин
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE2_DIV1;
    
    // Настройка SysTick на 1 мс
    SysTick_Config(SystemCoreClock / 1000);
}

// Инициализация GPIO
void GPIO_Init(void)
{
    // Включение тактирования порта C
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    
    // PC13 как выход push-pull (светодиод на Blue Pill)
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0; // Output mode, max speed 10 MHz
}

// Простая функция задержки
void Delay(uint32_t count)
{
    for(uint32_t i = 0; i < count; i++)
    {
        __asm__("nop");
    }
}