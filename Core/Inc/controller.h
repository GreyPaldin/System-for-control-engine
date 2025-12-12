#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float Kp;          // Пропорциональный коэффициент
    float Ki;          // Интегральный коэффициент
    float Ts;          // Период дискретизации [сек]
    
    float integral;    // Текущее значение интеграла
    float integral_prev; // Предыдущее значение интеграла
    
    float max_limit;   // Верхнее ограничение интеграла
    float min_limit;   // Нижнее ограничение интеграла
    
    float output;      // Выход регулятора
    float error;       // Текущая ошибка
} PI_Controller;

// Инициализация PI-регулятора
void PI_Controller_Init(PI_Controller *ctrl, float Kp, float Ki, float Ts, 
                       float min_limit, float max_limit);

// Сброс регулятора (обнуление интеграла)
void PI_Controller_Reset(PI_Controller *ctrl);

// Обновление регулятора (один шаг)
float PI_Controller_Update(PI_Controller *ctrl, float setpoint, float feedback);

// Установка новых коэффициентов
void PI_Controller_SetParams(PI_Controller *ctrl, float Kp, float Ki);

#endif