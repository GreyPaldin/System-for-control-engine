#include "system.h"
#include "system_config.h"
#include <stdlib.h>
#include <string.h>

static ControlSystem system_instance;
static float system_time = 0.0f;  // Глобальное время системы

void System_Init(void)
{
    // Инициализация регулятора
    PI_Controller_Init(&system_instance.controller, 
                      PI_KP, PI_KI, CONTROLLER_TS,
                      PI_MIN_LIMIT, PI_MAX_LIMIT);
    
    // Инициализация фильтра
    Filter_Init(&system_instance.filter, 
               FILTER_T1, FILTER_TS, FILTER_K);
    
    // Инициализация двигателя
    Motor_Init(&system_instance.motor,
              MOTOR_K, MOTOR_T1, MOTOR_TS);
    
    // Начальные значения
    system_instance.setpoint = 0.0f;
    system_instance.controller_output = 0.0f;
    system_instance.filter_output = 0.0f;
    system_instance.motor_output = 0.0f;
    
    system_instance.controller_updated = false;
    system_instance.filter_updated = false;
    system_instance.motor_updated = false;
    
    // Инициализация буфера
    system_instance.buffer_index = 0;
    system_instance.recording = false;
    memset(system_instance.time_buffer, 0, sizeof(system_instance.time_buffer));
    memset(system_instance.value_buffer, 0, sizeof(system_instance.value_buffer));
}

// Вызывается из прерывания таймера контроллера (20 кГц)
void System_ControllerUpdate(void)
{
    // Обновление регулятора
    system_instance.controller_output = 
        PI_Controller_Update(&system_instance.controller,
                           system_instance.setpoint,
                           system_instance.motor_output);
    
    system_instance.controller_updated = true;
}

// Вызывается из прерывания таймера фильтра (10 кГц)
void System_FilterUpdate(void)
{
    // Обновление фильтра
    system_instance.filter_output = 
        Filter_Update(&system_instance.filter,
                     system_instance.controller_output);
    
    system_instance.filter_updated = true;
}

// ВЫЗЫВАЕТСЯ ИЗ ПРЕРЫВАНИЯ ТАЙМЕРА ДВИГАТЕЛЯ (5 кГц)

void System_MotorUpdate(void)
{
    static uint32_t sample_counter = 0;
    const uint32_t sample_divider = 75; // Записываем каждую N точку
    
    // Обновление модели двигателя
    system_instance.motor_output = 
        Motor_Update(&system_instance.motor,
                    system_instance.filter_output);
    
    // Увеличиваем время на период двигателя
    system_time += MOTOR_TS;
    
    system_instance.motor_updated = true;
    
    // Запись в буфер (если включена запись)
    if (system_instance.recording)
    {
        sample_counter++;
        
        // Записываем каждый N-ый вызов
        if (sample_counter >= sample_divider)
        {
            sample_counter = 0;
            
            if (system_instance.buffer_index < BUFFER_SIZE)
            {
                system_instance.time_buffer[system_instance.buffer_index] = system_time;
                system_instance.value_buffer[system_instance.buffer_index] = system_instance.motor_output;
                system_instance.buffer_index++;
            }
            else
            {
                // Буфер полон - автоматически останавливаем запись
                system_instance.recording = false;
            }
        }
    }
}

ControlSystem* System_GetInstance(void)
{
    return &system_instance;
}

void System_SetSetpoint(float setpoint)
{
    system_instance.setpoint = setpoint;
}

float System_GetControllerOutput(void)
{
    return system_instance.controller_output;
}

float System_GetFilterOutput(void)
{
    return system_instance.filter_output;
}

float System_GetMotorOutput(void)
{
    return system_instance.motor_output;
}

// ========== ФУНКЦИИ ДЛЯ БУФЕРА ==========

// Начать запись
void System_StartRecording(void)
{
    system_instance.recording = true;
}

// Остановить запись
void System_StopRecording(void)
{
    system_instance.recording = false;
}

// Очистить буфер
void System_ClearBuffer(void)
{
    system_instance.buffer_index = 0;
    system_instance.recording = false;
    memset(system_instance.time_buffer, 0, sizeof(system_instance.time_buffer));
    memset(system_instance.value_buffer, 0, sizeof(system_instance.value_buffer));
    system_time = 0.0f;
}

// Получить указатель на массив времени
float* System_GetTimeBuffer(void)
{
    return system_instance.time_buffer;
}

// Получить указатель на массив значений
float* System_GetValueBuffer(void)
{
    return system_instance.value_buffer;
}

// Получить текущий индекс (сколько точек записано)
uint32_t System_GetBufferIndex(void)
{
    return system_instance.buffer_index;
}