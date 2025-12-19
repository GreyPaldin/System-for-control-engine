# О проекте

Проект представляет собой систему для симуляции на микроконтролелре серии STM32F103 ПИ-регулятора, фильтр и двигателя, представленного передаточной функцией.

В репрезитории есть набор папок и файлов:
- CMSIS (набор библиотек для программирования МК);
- Core (исходные файлы управляющей программы);
- STMDevice (набор данныех для компиляции программы под определённую серию МК);
- build_F103C8 (прошивка
- Makefile (файл с описанием запускаеммых утилит и программ, а также информацией об условиях компиляции прошивки).

Вся программа составлена таким образом, чтобы настраивать параметры системы (коэффициенты регулятора, коэффициенты фильтра, коэффициенты двигателя, частота обработки и таймеров, параметры буфера данных) можно было путём изменения параметров в одном единственном файле, по пути /Core
/Inc/system_config.h . 

______________________________________________________________________

# About the project

The project is a system for simulating a PI controller, filter and motor represented by a transfer function on a microcontroller of the STM32F103 series.

The repository contains a set of folders and files:
- CMSIS (a set of libraries for programming MK);
- Core (source files of the control program);
- STMDevice (a set of data for compiling a program for a specific MK series);
- build_F103C8 (firmware
- Makefile (a file with a description of the utilities and programs we run, as well as information about the firmware compilation conditions).

The entire program is designed in such a way that it is possible to adjust the system parameters (regulator coefficients, filter coefficients, engine coefficients, processing frequency and timers, data buffer parameters) by changing the parameters in a single file, following the path /Core
/Inc/system_config.h.

