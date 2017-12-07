################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_MKW41Z4.S 

OBJS += \
./startup/startup_MKW41Z4.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__REDLIB__ -I"C:\Users\Samuel Mott\Documents\RTSLab3\source" -I"C:\Users\Samuel Mott\Documents\RTSLab3" -I"C:\Users\Samuel Mott\Documents\RTSLab3\drivers" -I"C:\Users\Samuel Mott\Documents\RTSLab3\genfsk" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\XCVR\MKW41Z4\XCVR_Test" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\MemManager\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\freertos" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\Keyboard\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\SerialManager\Source\SPI_Adapter" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\Lists" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\common" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\SerialManager\Source\UART_Adapter" -I"C:\Users\Samuel Mott\Documents\RTSLab3\board" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\RNG\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\ModuleInfo" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\GPIO" -I"C:\Users\Samuel Mott\Documents\RTSLab3\CMSIS" -I"C:\Users\Samuel Mott\Documents\RTSLab3\utilities" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\SecLib" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\OSAbstraction\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\FunctionLib" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\Flash\Internal" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\LowPower\Interface\MKW41Z" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\LowPower\Source\MKW41Z" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\MWSCoexistence\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\XCVR\MKW41Z4" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\Messaging\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\SerialManager\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\TimersManager\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\TimersManager\Source" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\LED\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\Panic\Interface" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\SerialManager\Source\I2C_Adapter" -I"C:\Users\Samuel Mott\Documents\RTSLab3\framework\DCDC\Interface" -g3 -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


