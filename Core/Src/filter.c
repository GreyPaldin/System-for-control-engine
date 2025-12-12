#include "filter.h"
#include <math.h>

void Filter_Init(FirstOrderFilter *filter, float T1, float Ts, float K)
{
    filter->K = K;
    filter->T1 = T1;
    filter->Ts = Ts;
    
    // Расчет коэффициента фильтрации
    if (T1 > 0) {
        filter->alpha = expf(-Ts / T1);
    } else {
        filter->alpha = 0; // При T1=0 фильтр не работает
    }
    
    Filter_Reset(filter);
}

void Filter_Reset(FirstOrderFilter *filter)
{
    filter->y_prev = 0.0f;
    filter->output = 0.0f;
}

float Filter_Update(FirstOrderFilter *filter, float input)
{
    // Дискретная реализация фильтра первого порядка
    filter->output = filter->alpha * filter->y_prev + (1 - filter->alpha) * (filter->K * input);
    
    // Сохранение для следующего шага
    filter->y_prev = filter->output;
    
    return filter->output;
}

void Filter_SetTimeConstant(FirstOrderFilter *filter, float T1)
{
    filter->T1 = T1;
    
    if (T1 > 0) {
        filter->alpha = expf(-filter->Ts / T1);
    } else {
        filter->alpha = 0;
    }
}