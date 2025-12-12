#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "system_config.h"

// Типы колбэков
typedef void (*TimerCallback)(void);

// Структура таймера
typedef struct {
    volatile uint32_t* timer;  // Указатель на регистры таймера
    uint32_t freq_hz;          // Частота прерывания
    TimerCallback callback;    // Колбэк функция
    bool is_initialized;       // Флаг инициализации
} SystemTimer;

// Инициализация всех таймеров
void TimerManager_Init(void);

// Запуск всех таймеров
void TimerManager_Start(void);

// Остановка всех таймеров
void TimerManager_Stop(void);

// Установка колбэков
void TimerManager_SetControllerCallback(TimerCallback callback);
void TimerManager_SetFilterCallback(TimerCallback callback);
void TimerManager_SetMotorCallback(TimerCallback callback);

// Получение счетчиков таймеров
uint32_t TimerManager_GetControllerTick(void);
uint32_t TimerManager_GetFilterTick(void);
uint32_t TimerManager_GetMotorTick(void);

// Обработчики прерываний
void TimerManager_ControllerIRQHandler(void);
void TimerManager_FilterIRQHandler(void);
void TimerManager_MotorIRQHandler(void);

#endif