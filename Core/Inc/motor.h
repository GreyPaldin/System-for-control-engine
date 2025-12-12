#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float K;           // Коэффициент усиления двигателя
    float T1;          // Постоянная времени двигателя [сек]
    float Ts;          // Период дискретизации [сек]
    
    float y_prev;      // Предыдущее значение выхода
    
    float output;      // Текущее значение выхода
} DCMotorModel;

// Инициализация модели двигателя
void Motor_Init(DCMotorModel *motor, float K, float T1, float Ts);

// Обновление модели двигателя (один шаг)
float Motor_Update(DCMotorModel *motor, float input);

// Сброс модели двигателя
void Motor_Reset(DCMotorModel *motor);

// Установка новых параметров двигателя
void Motor_SetParams(DCMotorModel *motor, float K, float T1);

#endif