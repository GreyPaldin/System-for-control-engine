#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

// Частоты дискретизации
#define CONTROLLER_FREQ_HZ     20000   // 20 кГц (int для таймеров)
#define FILTER_FREQ_HZ         10000   // 10 кГц (int для таймеров)
#define MOTOR_FREQ_HZ          5000    // 5 кГц (int для таймеров)

// Частоты как float (для вычислений)
#define CONTROLLER_FREQ_HZ_F   20000.0f
#define FILTER_FREQ_HZ_F       10000.0f  
#define MOTOR_FREQ_HZ_F        5000.0f

// Периоды дискретизации [сек]
#define CONTROLLER_TS          (1.0f / CONTROLLER_FREQ_HZ_F)
#define FILTER_TS              (1.0f / FILTER_FREQ_HZ_F)
#define MOTOR_TS               (1.0f / MOTOR_FREQ_HZ_F)

// Параметры регулятора
#define PI_KP                  5.0f
#define PI_KI                  50.0f
#define PI_MIN_LIMIT           -3.0f
#define PI_MAX_LIMIT           4.0f

// Параметры фильтра
#define FILTER_T1              0.01f
#define FILTER_K               1.00f

// Параметры двигателя
#define MOTOR_K                1.0f
#define MOTOR_T1               0.1f

// Настройки буфера для записи переходной характеристики
#define BUFFER_SIZE         1500    // Размер буфера (примерно 10 секунд при 1 кГц)

#endif