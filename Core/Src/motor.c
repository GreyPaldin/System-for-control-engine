#include "motor.h"
#include <math.h>

void Motor_Init(DCMotorModel *motor, float K, float T1, float Ts)
{
    motor->K = K;
    motor->T1 = T1;
    motor->Ts = Ts;
    
    Motor_Reset(motor);
}

void Motor_Reset(DCMotorModel *motor)
{
    motor->y_prev = 0.0f;
    motor->output = 0.0f;
}

float Motor_Update(DCMotorModel *motor, float input)
{
    // Дискретная реализация модели двигателя (апериодическое звено)
    if (motor->T1 > 0) {
        motor->output = motor->y_prev + 
                       (motor->Ts / motor->T1) * (motor->K * input - motor->y_prev);
    } else {
        motor->output = motor->K * input; // При T1=0 - безынерционное звено
    }
    
    // Сохранение для следующего шага
    motor->y_prev = motor->output;
    
    return motor->output;
}

void Motor_SetParams(DCMotorModel *motor, float K, float T1)
{
    motor->K = K;
    motor->T1 = T1;
}