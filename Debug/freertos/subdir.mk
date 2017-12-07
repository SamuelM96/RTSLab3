################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/croutine.c \
../freertos/event_groups.c \
../freertos/fsl_tickless_lptmr.c \
../freertos/fsl_tickless_systick.c \
../freertos/heap_4.c \
../freertos/list.c \
../freertos/port.c \
../freertos/queue.c \
../freertos/tasks.c \
../freertos/timers.c 

OBJS += \
./freertos/croutine.o \
./freertos/event_groups.o \
./freertos/fsl_tickless_lptmr.o \
./freertos/fsl_tickless_systick.o \
./freertos/heap_4.o \
./freertos/list.o \
./freertos/port.o \
./freertos/queue.o \
./freertos/tasks.o \
./freertos/timers.o 

C_DEPS += \
./freertos/croutine.d \
./freertos/event_groups.d \
./freertos/fsl_tickless_lptmr.d \
./freertos/fsl_tickless_systick.d \
./freertos/heap_4.d \
./freertos/list.d \
./freertos/port.d \
./freertos/queue.d \
./freertos/tasks.d \
./freertos/timers.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/%.o: ../freertos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DDEBUG -DFSL_RTOS_FREE_RTOS -DFRDM_KW41Z -DFREEDOM -D__MCUXPRESSO -D__USE_CMSIS -DSDK_OS_FREE_RTOS -DCPU_MKW41Z512VHT4 -DCPU_MKW41Z512VHT4_cm0plus -D__REDLIB__ -I"C:\Users\Samuel Mott\Documents\RTSLab3\source" -I"C:\Users\Samuel Mott\Documents\RTSLab3" -I"C:\Users\Samuel Mott\Documents\RTSLab3\drivers" -I"C:\Users\Samuel Mott\Documents\RTSLab3\genfsk" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\XCVR\MKW41Z4\XCVR_Test" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\MemManager\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\freertos" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\Keyboard\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\SerialManager\Source\SPI_Adapter" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\Lists" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\common" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\SerialManager\Source\UART_Adapter" -I"C:\Users\Samuel Mott\Documents\RTSLab3\board" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\RNG\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\ModuleInfo" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\GPIO" -I"C:\Users\Samuel Mott\Documents\RTSLab3\CMSIS" -I"C:\Users\Samuel Mott\Documents\RTSLab3\utilities" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\SecLib" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\OSAbstraction\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\FunctionLib" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\Flash\Internal" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\LowPower\Interface\MKW41Z" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\LowPower\Source\MKW41Z" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\MWSCoexistence\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\XCVR\MKW41Z4" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\Messaging\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\SerialManager\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\TimersManager\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\TimersManager\Source" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\LED\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\Panic\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\SerialManager\Source\I2C_Adapter" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\DCDC\Interface" -O0 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -imacros "C:/Users/Samuel Mott/Documents/RTSLab3/source/app_preinclude.h" -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


