#include "controller.h"
#include <math.h>

void PI_Controller_Init(PI_Controller *ctrl, float Kp, float Ki, float Ts,
                       float min_limit, float max_limit)
{
    ctrl->Kp = Kp;
    ctrl->Ki = Ki;
    ctrl->Ts = Ts;
    ctrl->min_limit = min_limit;
    ctrl->max_limit = max_limit;
    
    PI_Controller_Reset(ctrl);
}

void PI_Controller_Reset(PI_Controller *ctrl)
{
    ctrl->integral = 0.0f;
    ctrl->integral_prev = 0.0f;
    ctrl->output = 0.0f;
    ctrl->error = 0.0f;
}

float PI_Controller_Update(PI_Controller *ctrl, float setpoint, float feedback)
{
    // Вычисление ошибки
    ctrl->error = setpoint - feedback;
    
    // Пропорциональная составляющая
    float proportional = ctrl->Kp * ctrl->error;
    
    // Интегральная составляющая (с антивиндупом)
    float integral_temp = ctrl->integral_prev + ctrl->Ki * ctrl->error * ctrl->Ts;
    
    // Ограничение интегральной составляющей
    if (integral_temp > ctrl->max_limit) {
        ctrl->integral = ctrl->max_limit;
    } else if (integral_temp < ctrl->min_limit) {
        ctrl->integral = ctrl->min_limit;
    } else {
        ctrl->integral = integral_temp;
    }
    
    // Сохранение для следующего шага
    ctrl->integral_prev = ctrl->integral;
    
    // Выход регулятора
    ctrl->output = proportional + ctrl->integral;
    
    return ctrl->output;
}

void PI_Controller_SetParams(PI_Controller *ctrl, float Kp, float Ki)
{
    ctrl->Kp = Kp;
    ctrl->Ki = Ki;
}