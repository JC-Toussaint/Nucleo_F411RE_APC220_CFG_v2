################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SSD1306/Src/bluetooth_logo.c \
../Drivers/SSD1306/Src/fonts.c \
../Drivers/SSD1306/Src/hal_lcd.c \
../Drivers/SSD1306/Src/ssd1306.c 

OBJS += \
./Drivers/SSD1306/Src/bluetooth_logo.o \
./Drivers/SSD1306/Src/fonts.o \
./Drivers/SSD1306/Src/hal_lcd.o \
./Drivers/SSD1306/Src/ssd1306.o 

C_DEPS += \
./Drivers/SSD1306/Src/bluetooth_logo.d \
./Drivers/SSD1306/Src/fonts.d \
./Drivers/SSD1306/Src/hal_lcd.d \
./Drivers/SSD1306/Src/ssd1306.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SSD1306/Src/%.o Drivers/SSD1306/Src/%.su: ../Drivers/SSD1306/Src/%.c Drivers/SSD1306/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/SSD1306/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-SSD1306-2f-Src

clean-Drivers-2f-SSD1306-2f-Src:
	-$(RM) ./Drivers/SSD1306/Src/bluetooth_logo.d ./Drivers/SSD1306/Src/bluetooth_logo.o ./Drivers/SSD1306/Src/bluetooth_logo.su ./Drivers/SSD1306/Src/fonts.d ./Drivers/SSD1306/Src/fonts.o ./Drivers/SSD1306/Src/fonts.su ./Drivers/SSD1306/Src/hal_lcd.d ./Drivers/SSD1306/Src/hal_lcd.o ./Drivers/SSD1306/Src/hal_lcd.su ./Drivers/SSD1306/Src/ssd1306.d ./Drivers/SSD1306/Src/ssd1306.o ./Drivers/SSD1306/Src/ssd1306.su

.PHONY: clean-Drivers-2f-SSD1306-2f-Src

