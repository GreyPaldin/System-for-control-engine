#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float K;  
    float T1;          // Постоянная времени фильтра [сек]
    float Ts;          // Период дискретизации [сек]
    
    float alpha;       // Коэффициент фильтрации
    float y_prev;      // Предыдущее значение выхода
    
    float output;      // Текущее значение выхода
} FirstOrderFilter;

// Инициализация фильтра первого порядка
void Filter_Init(FirstOrderFilter *filter, float T1, float Ts, float K);

// Обновление фильтра (один шаг)
float Filter_Update(FirstOrderFilter *filter, float input);

// Сброс фильтра
void Filter_Reset(FirstOrderFilter *filter);

// Установка новой постоянной времени
void Filter_SetTimeConstant(FirstOrderFilter *filter, float T1);

#endif