
#ifndef SYSTEM_H
#define SYSTEM_H

#include "controller.h"
#include "filter.h"
#include "motor.h"
#include "system_config.h"

// Структура всей системы управления
typedef struct {
    PI_Controller controller;
    FirstOrderFilter filter;
    DCMotorModel motor;
    
    float setpoint;
    float controller_output;
    float filter_output;
    float motor_output;  // Это будет обратной связью
    
    // Флаги обновления
    volatile bool controller_updated;
    volatile bool filter_updated;
    volatile bool motor_updated;
    
    // Простой буфер для записи [время, значение]
    float time_buffer[BUFFER_SIZE];   // Массив времени (секунды)
    float value_buffer[BUFFER_SIZE];  // Массив значений (выход двигателя)
    uint32_t buffer_index;            // Текущая позиция записи
    bool recording;                   // Флаг записи
} ControlSystem;

// Инициализация системы
void System_Init(void);

// Функции для вызова из прерываний
void System_ControllerUpdate(void);
void System_FilterUpdate(void);
void System_MotorUpdate(void);

// Получение глобального экземпляра системы
ControlSystem* System_GetInstance(void);

// Установка задания
void System_SetSetpoint(float setpoint);

// Получение выходных значений
float System_GetControllerOutput(void);
float System_GetFilterOutput(void);
float System_GetMotorOutput(void);

// Функции для работы с буфером
void System_StartRecording(void);
void System_StopRecording(void);
void System_ClearBuffer(void);
float* System_GetTimeBuffer(void);
float* System_GetValueBuffer(void);
uint32_t System_GetBufferIndex(void);

#endif