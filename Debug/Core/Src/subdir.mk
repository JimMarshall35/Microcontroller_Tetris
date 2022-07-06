################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/AnalogStick.c \
../Core/Src/LcdGraphics.c \
../Core/Src/OS.c \
../Core/Src/TetrisCore.c \
../Core/Src/TetrisGame.c \
../Core/Src/TetrisHighLevelModesStateMachine.c \
../Core/Src/TetrisMain.c \
../Core/Src/TetrisPlayAgainScreen.c \
../Core/Src/lcd_pcd8544.c \
../Core/Src/lcd_pcd8544_ll.c \
../Core/Src/main.c \
../Core/Src/stm32f3xx_hal_msp.c \
../Core/Src/stm32f3xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f3xx.c 

OBJS += \
./Core/Src/AnalogStick.o \
./Core/Src/LcdGraphics.o \
./Core/Src/OS.o \
./Core/Src/TetrisCore.o \
./Core/Src/TetrisGame.o \
./Core/Src/TetrisHighLevelModesStateMachine.o \
./Core/Src/TetrisMain.o \
./Core/Src/TetrisPlayAgainScreen.o \
./Core/Src/lcd_pcd8544.o \
./Core/Src/lcd_pcd8544_ll.o \
./Core/Src/main.o \
./Core/Src/stm32f3xx_hal_msp.o \
./Core/Src/stm32f3xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f3xx.o 

C_DEPS += \
./Core/Src/AnalogStick.d \
./Core/Src/LcdGraphics.d \
./Core/Src/OS.d \
./Core/Src/TetrisCore.d \
./Core/Src/TetrisGame.d \
./Core/Src/TetrisHighLevelModesStateMachine.d \
./Core/Src/TetrisMain.d \
./Core/Src/TetrisPlayAgainScreen.d \
./Core/Src/lcd_pcd8544.d \
./Core/Src/lcd_pcd8544_ll.d \
./Core/Src/main.d \
./Core/Src/stm32f3xx_hal_msp.d \
./Core/Src/stm32f3xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f3xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303x8 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/AnalogStick.d ./Core/Src/AnalogStick.o ./Core/Src/AnalogStick.su ./Core/Src/LcdGraphics.d ./Core/Src/LcdGraphics.o ./Core/Src/LcdGraphics.su ./Core/Src/OS.d ./Core/Src/OS.o ./Core/Src/OS.su ./Core/Src/TetrisCore.d ./Core/Src/TetrisCore.o ./Core/Src/TetrisCore.su ./Core/Src/TetrisGame.d ./Core/Src/TetrisGame.o ./Core/Src/TetrisGame.su ./Core/Src/TetrisHighLevelModesStateMachine.d ./Core/Src/TetrisHighLevelModesStateMachine.o ./Core/Src/TetrisHighLevelModesStateMachine.su ./Core/Src/TetrisMain.d ./Core/Src/TetrisMain.o ./Core/Src/TetrisMain.su ./Core/Src/TetrisPlayAgainScreen.d ./Core/Src/TetrisPlayAgainScreen.o ./Core/Src/TetrisPlayAgainScreen.su ./Core/Src/lcd_pcd8544.d ./Core/Src/lcd_pcd8544.o ./Core/Src/lcd_pcd8544.su ./Core/Src/lcd_pcd8544_ll.d ./Core/Src/lcd_pcd8544_ll.o ./Core/Src/lcd_pcd8544_ll.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f3xx_hal_msp.d ./Core/Src/stm32f3xx_hal_msp.o ./Core/Src/stm32f3xx_hal_msp.su ./Core/Src/stm32f3xx_it.d ./Core/Src/stm32f3xx_it.o ./Core/Src/stm32f3xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f3xx.d ./Core/Src/system_stm32f3xx.o ./Core/Src/system_stm32f3xx.su

.PHONY: clean-Core-2f-Src

